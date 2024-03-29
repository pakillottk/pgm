#include <PGM/Application/Application.h>
#include <PGM/ECS/Components/Components.h>
#include <PGM/ECS/Entity/EntityRef.h>
#include <PGM/ECS/Scene/Scene.h>
#include <PGM/Renderer/API/Backends.h>
#include <PGM/Renderer/Camera/Camera.h>

#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>
#include <imguizmo/ImGuizmo.h>

class SandboxSystem : public PGM::ApplicationSystem
{
  public:
    inline SandboxSystem(PGM::Application &app) : PGM::ApplicationSystem(app)
    {
        m_Scene.createEntity("Entity 1");
        m_Scene.createEntity("Entity 2");
    }

    // Life cycle
    void onActivate() override
    {
        static constexpr const char *vertex_shader =
            "#version 430 core\n"
            "layout (location = 0) in vec3 in_Position;\n"
            "layout (location = 7) in vec4 in_Color;\n"
            "uniform mat4 u_ModelMat;\n"
            "uniform mat4 u_ViewProjMat;\n"
            "out vec4 frag_Color;\n"
            "void main()\n"
            "{\n"
            "    frag_Color = in_Color;\n"
            "    gl_Position = u_ViewProjMat * u_ModelMat * vec4(in_Position.xy,0,1);\n"
            "}\n";

        static constexpr const char *fragment_shader = "#version 430 core\n"
                                                       "in vec4 frag_Color;\n"
                                                       "layout (location = 0) out vec4 out_Color;\n"
                                                       "void main()\n"
                                                       "{\n"
                                                       "    out_Color = frag_Color;\n"
                                                       "}\n";

        m_DummyShader = m_App.renderer()->createShader(vertex_shader, fragment_shader);

        const int modelMatLoc = m_DummyShader->getUniformLocation("u_ModelMat");
        PGM_ASSERT(modelMatLoc >= 0, "ModelMat uniform not found");
        m_DummyShader->setUniform(modelMatLoc, m_QuadTrf);

        const int matLoc = m_DummyShader->getUniformLocation("u_ViewProjMat");
        PGM_ASSERT(matLoc >= 0, "ViewProjMat uniform not found");

        m_CameraTrf.position.z = 5.0f;
        const auto viewMat = glm::inverse(m_CameraTrf.toMatrix());
        const auto aspect = static_cast<float>(m_App.window()->width()) / static_cast<float>(m_App.window()->height());
        m_Camera.setAspectRatio(aspect);
        const auto &projMat = m_Camera.projection();
        m_DummyShader->setUniform(matLoc, projMat * viewMat);

        // clang-format off
        struct QuadVertex
        {
            PGM::Vec3 position;
            PGM::ByteColor color;
        };
        const QuadVertex vertices[] = 
        {
            {{-1.0f, -1.0f, 0.0f}, PGM::Colors::toByteColor(PGM::Colors::Red)},
            {{1.0f, -1.0f, 0.0f}, PGM::Colors::toByteColor(PGM::Colors::Green)},
            {{-1.0f,  1.0f, 0.0f}, PGM::Colors::toByteColor(PGM::Colors::Blue)},

            {{1.0f, -1.0f, 0.0f}, PGM::Colors::toByteColor(PGM::Colors::Yellow)},
            {{1.0f,  1.0f, 0.0f}, PGM::Colors::toByteColor(PGM::Colors::Magenta)},
            {{-1.0f, 1.0f, 0.0f}, PGM::Colors::toByteColor(PGM::Colors::Cyan)}
        };
        const PGM::BatchLayout layout = {
            PGM::VertexAttribLayout{PGM::Position, PGM::VertexAttribDataType::Float, 3, sizeof(QuadVertex), 0},
            PGM::VertexAttribLayout{PGM::VertexColor, PGM::VertexAttribDataType::UnsignedByte, 4, sizeof(QuadVertex), offsetof(QuadVertex, color), true}
        };
        // clang-format on

        m_QuadBatch = PGM::make_shared_ref<PGM::StaticBatch>(m_App.renderer().context(), PGM::PrimitiveType::Triangles,
                                                             layout, 6 * sizeof(QuadVertex), vertices);

        m_ResizeListener = m_App.window()->dispatcher()->suscribe<PGM::WindowEvents::WindowResized>(
            [this](const PGM::WindowEvents::WindowResized &event) {
                this->m_Camera.setAspectRatio(static_cast<float>(event.width) / static_cast<float>(event.height));
            });
    }

    void onDeactivate() override
    {
        m_ResizeListener.unsuscribe();
        m_DummyShader->destroy();
    }

    // Actual logic
    void beginFrame() override
    {
        const auto window = m_App.window();
        m_App.renderer()->setViewport({0, 0, window->width(), window->height()});
        m_App.renderer()->setClipRegion({0, 0, window->width(), window->height()});
        m_App.renderer()->clear(PGM::bColor | PGM::bDepth, PGM::Color{0, 0, 0, 1});

        m_DummyShader->bind();

        const auto viewMat = glm::inverse(m_CameraTrf.toMatrix());
        const auto &projMat = m_Camera.projection();
        const int matLoc = m_DummyShader->getUniformLocation("u_ViewProjMat");
        PGM_ASSERT(matLoc >= 0, "ViewProjMat uniform not found");
        m_DummyShader->setUniform(matLoc, projMat * viewMat);

        const int modelMatLoc = m_DummyShader->getUniformLocation("u_ModelMat");
        PGM_ASSERT(modelMatLoc >= 0, "ModelMat uniform not found");
        m_DummyShader->setUniform(modelMatLoc, m_QuadTrf);

        m_DummyShader->unbind();

        m_App.renderer().submitBatch(m_DummyShader, m_QuadBatch);
    }

    void endFrame() override
    {
    }

    void onUpdate(const PGM::Timespan &deltaTime) override
    {
    }

    void onGui(const PGM::Timespan &deltaTime) override
    {
        const auto &projMat = m_Camera.projection();
        auto viewMat = glm::inverse(m_CameraTrf.toMatrix());

        ImGuizmo::SetID(0);

        ImGuizmo::SetOrthographic(m_Camera.type() == PGM::CameraType::Orthographic);

        auto trf = m_QuadTrf.toMatrix();
        ImGuizmo::DrawCubes(glm::value_ptr(viewMat), glm::value_ptr(projMat), glm::value_ptr(trf), 1);
        ImGuizmo::Manipulate(glm::value_ptr(viewMat), glm::value_ptr(projMat), ImGuizmo::UNIVERSAL, ImGuizmo::LOCAL,
                             glm::value_ptr(trf));
        ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(trf), glm::value_ptr(m_QuadTrf.position),
                                              glm::value_ptr(m_QuadTrf.rotation), glm::value_ptr(m_QuadTrf.scale));
        m_QuadTrf.rotation = glm::radians(m_QuadTrf.rotation);

        ImGuizmo::ViewManipulate(glm::value_ptr(viewMat), m_CameraTrf.position.length(),
                                 ImVec2{m_App.window()->width() - 256.0f, 0.0f}, ImVec2{256, 256}, 0x0);

        const auto editedViewMat = glm::inverse(viewMat);
        ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(editedViewMat), glm::value_ptr(m_CameraTrf.position),
                                              glm::value_ptr(m_CameraTrf.rotation), glm::value_ptr(m_CameraTrf.scale));
        m_CameraTrf.rotation = glm::radians(m_CameraTrf.rotation);

        if (ImGui::Begin("Camera"))
        {
            ImGui::Text("Projection");

            bool isOrtho = m_Camera.type() == PGM::CameraType::Orthographic;

            if (ImGui::Checkbox("Ortho", &isOrtho))
            {
                if (isOrtho)
                {
                    m_Camera.setOrtho(m_Camera.orthoSize(), m_Camera.near(), m_Camera.far());
                }
                else
                {
                    m_Camera.setPerspective(m_Camera.fov(), m_Camera.near(), m_Camera.far());
                }
            }

            float camViewSize = isOrtho ? m_Camera.orthoSize() : m_Camera.fov();
            if (ImGui::DragFloat(isOrtho ? "Size" : "FOV", &camViewSize, 0.1f))
            {
                if (isOrtho)
                {
                    m_Camera.setOrtho(camViewSize, m_Camera.near(), m_Camera.far());
                }
                else
                {
                    m_Camera.setPerspective(camViewSize, m_Camera.near(), m_Camera.far());
                }
            }

            ImGui::Separator();

            ImGui::Text("Position");
            ImGui::DragFloat("Pos X", &m_CameraTrf.position.x, 0.1f);
            ImGui::DragFloat("Pos Y", &m_CameraTrf.position.y, 0.1f);
            ImGui::DragFloat("Pos Z", &m_CameraTrf.position.z, 0.1f);

            ImGui::Separator();

            ImGui::Text("Rotation");

            PGM::Vec3 rot = glm::degrees(m_CameraTrf.rotation);
            ImGui::DragFloat("Rot X", &rot.x, 0.1f);
            ImGui::DragFloat("Rot Y", &rot.y, 0.1f);
            ImGui::DragFloat("Rot Z", &rot.z, 0.1f);
            m_CameraTrf.rotation = glm::radians(rot);
        }
        ImGui::End();

        return;

        // ImGui::DockSpaceOverViewport();

        if (ImGui::Begin("Scene"))
        {
            m_Scene.for_each([](PGM::EntityRef entity) {
                auto &tag = entity.getComponent<PGM::Components::TagComponent>();
                ImGui::Selectable(tag.name.c_str());
            });
        }
        ImGui::End();
    }

  private:
    PGM::Events::EventListener m_ResizeListener;

    PGM::Camera m_Camera{PGM::CameraType::Orthographic};
    PGM::Components::TransformComponent m_CameraTrf;

    PGM::SharedRef<PGM::Shader> m_DummyShader;
    PGM::SharedRef<PGM::StaticBatch> m_QuadBatch;
    PGM::Components::TransformComponent m_QuadTrf;

    PGM::Scene m_Scene;
};

int main()
{
    PGM::Application app = PGM::Application::create<PGM::OpenGL::OpenGlRenderContext>(
        PGM::window_creation_args_t{"PGM Sandbox"},
        PGM::OpenGL::OPENGL_DEFAULT_CONTEXT_FLAGS // | PGM::Renderer::bOglContextCompatibiltyMode
    );
    app.pushSystem<SandboxSystem>();
    app.run();

    return 0;
}