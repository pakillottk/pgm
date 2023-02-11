#include "../OpenGlRenderContext.h"

#include <PGM/Core/Assert/Assert.h>
#include <PGM/Core/Logging/Logger.h>
#include <PGM/Platform/Window/Win32/WindowImpl.h>

#include <gl/GL.h>
#include <stdexcept>

#define internal static

namespace PGM::Renderer::API::Backend
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

    Logging::log_info("Created OpenGL context");
    Logging::log_info("GL Version: {}", glVersion);
    Logging::log_info("GL Vendor: {}", vendor);
    Logging::log_info("GL Renderer: {}", renderer);
#endif
}

internal HGLRC createContext(HDC dc, int desiredMajor, int desiredMinor)
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

    // GLint attribs[] = {WGL_CONTEXT_MAJOR_VERSION_ARB, desiredMajor, WGL_CONTEXT_MINOR_VERSION_ARB, desiredMinor,
    //                    // TODO(pgm) For now, compatibility mode...
    //                    // Forward compatibility mode
    //                    // WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
    //                    // Compatibility profile
    //                    // WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
    //                    0};

    // auto context = wglCreateContextAttribsARB(dc, 0, attribs);
    // wglMakeCurrent(nullptr, nullptr);
    // wglDeleteContext(dummyContext);
    // if (context == nullptr || !wglMakeCurrent(dc, context))
    // {
    //     Logging::log_error("Fatal error! The opengl context creation failed...");
    //     return nullptr;
    // }

    logOpenGlInfo();

    return dummyContext;
}

OpenGlRenderContext::OpenGlRenderContext(SharedRef<Platform::Window> window, int majorVersion /*= 4*/,
                                         int minorVersion /*= 6*/)
    : m_Window{window}, m_Context{nullptr, [](OpenGlRenderContext::context_t *) {}}
{
    // TODO(pgm) Set the desired GL Version
    auto context = createContext(m_Window->impl().hdc, majorVersion, minorVersion);
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

} // namespace PGM::Renderer::API::Backend