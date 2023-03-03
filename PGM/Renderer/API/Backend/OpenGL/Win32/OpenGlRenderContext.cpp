#include "../OpenGlRenderContext.h"

#include <PGM/Core/Assert/Assert.h>
#include <PGM/Core/Logging/Logger.h>
#include <PGM/Platform/Window/Win32/WindowImpl.h>

#include <GL/glew.h>
#include <GL/wglew.h>
#include <gl/GL.h>

#include <stdexcept>

#define internal static

namespace PGM::OpenGL
{

struct OpenGlRenderContext::context_t
{
    HGLRC oglCtx;
};

internal void logOpenGlInfo()
{
#if PGM_LOGGING_LEVEL > 2
    const char *glVersion = reinterpret_cast<const char *>(glGetString(GL_VERSION));
    const char *vendor = reinterpret_cast<const char *>(glGetString(GL_VENDOR));
    const char *renderer = reinterpret_cast<const char *>(glGetString(GL_RENDERER));
    const char *shading = reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION));

    Logging::log_info("(OpenGL) GL Version:\t{}", glVersion);
    Logging::log_info("(OpenGL) GL Vendor:\t{}", vendor);
    Logging::log_info("(OpenGL) GL Renderer:\t{}", renderer);
    Logging::log_info("(OpenGL) GL Shading Language:\t{}", shading);
#endif
}

internal inline void configureOpenGlContext()
{
    glFrontFace(GL_CCW);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glDisable(GL_CULL_FACE);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_PRIMITIVE_RESTART);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

internal HGLRC createContext(HDC dc, int desiredMajor, int desiredMinor, OpenGlContextFlagCreationMask flags)
{
    PIXELFORMATDESCRIPTOR pfd = {sizeof(PIXELFORMATDESCRIPTOR),
                                 1,
                                 PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, // Flags
                                 PFD_TYPE_RGBA, // The kind of framebuffer. RGBA or palette.
                                 32,            // Colordepth of the framebuffer.
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 24, // Number of bits for the depthbuffer
                                 8,  // Number of bits for the stencilbuffer
                                 0,  // Number of Aux buffers in the framebuffer.
                                 PFD_MAIN_PLANE,
                                 0,
                                 0,
                                 0,
                                 0};

    int pixelFormat;
    pixelFormat = ChoosePixelFormat(dc, &pfd);
    SetPixelFormat(dc, pixelFormat, &pfd);

    auto dummyContext = wglCreateContext(dc);
    if (dummyContext == nullptr || !wglMakeCurrent(dc, dummyContext))
    {
        Logging::log_error("Fatal error! The opengl context creation failed...");
        return nullptr;
    }

    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(dummyContext);
        Logging::log_error("Failed to initialize GLEW");
        return nullptr;
    }

    GLint attribs[] = {WGL_CONTEXT_MAJOR_VERSION_ARB, desiredMajor, WGL_CONTEXT_MINOR_VERSION_ARB, desiredMinor,
                       // Compatibility profile
                       WGL_CONTEXT_PROFILE_MASK_ARB,
                       (flags & bOglContextCompatibiltyMode) > 0 ? WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB
                                                                 : WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
#ifdef _DEBUG
                       // ask for debug context for non "Release" builds
                       WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
#endif
                       0};

    auto context = wglCreateContextAttribsARB(dc, 0, attribs);
    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(dummyContext);
    if (context == nullptr || !wglMakeCurrent(dc, context))
    {
        Logging::log_error("Fatal error! The opengl context creation failed...");
        return nullptr;
    }

    logOpenGlInfo();

    configureOpenGlContext();

    return context;
}

OpenGlRenderContext::OpenGlRenderContext(SharedRef<Window> window,
                                         OpenGlContextFlagCreationMask flags /*= OPENGL_DEFAULT_CONTEXT_FLAGS*/,
                                         int majorVersion /*= 4*/, int minorVersion /*= 6*/)
    : m_Window{window}, m_Context{nullptr, [](OpenGlRenderContext::context_t *) {}}
{
    if (majorVersion < 3 || (majorVersion == 3 && minorVersion < 3))
    {
        throw std::runtime_error{"An OpenGL core-profile capable version is required (3.3+)"};
    }

    // TODO(pgm) Set the desired GL Version
    auto context = createContext(m_Window->impl().hdc, majorVersion, minorVersion, flags);
    if (context == nullptr)
    {
        throw std::runtime_error{"Cant create the OpenGL context..."};
    }

    m_Context = Ref<OpenGlRenderContext::context_t, void (*)(OpenGlRenderContext::context_t *)>{
        new OpenGlRenderContext::context_t{context}, [](OpenGlRenderContext::context_t *ctx) {
            // make the rendering context not current
            wglMakeCurrent(nullptr, nullptr);
            // delete the rendering context
            wglDeleteContext(ctx->oglCtx);

            delete ctx;
        }};
}

OpenGlRenderContext::~OpenGlRenderContext() = default;

OpenGlRenderContext::OpenGlRenderContext(OpenGlRenderContext &&) noexcept = default;
OpenGlRenderContext &OpenGlRenderContext::operator=(OpenGlRenderContext &&) noexcept = default;

bool OpenGlRenderContext::bind() const
{
    PGM_ASSERT(m_Context && m_Context->oglCtx != nullptr, "Invalid OpenGL context...");
    auto contextBound = wglMakeCurrent(m_Window->impl().hdc, m_Context->oglCtx);
    PGM_ASSERT(contextBound, "Failed to bind OpenGL context...");

    return contextBound;
}

void OpenGlRenderContext::unbind() const
{
    wglMakeCurrent(nullptr, nullptr);
}

void OpenGlRenderContext::swapBuffers() const
{
    SwapBuffers(m_Window->impl().hdc);
}

} // namespace PGM::OpenGL