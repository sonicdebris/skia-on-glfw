#include "GLFW/glfw3.h"
#include "SkCanvas.h"
#include "GrContext.h"
#include "gl/GrGLInterface.h"
#include "SkData.h"
#include "SkImage.h"
#include "SkStream.h"
#include "SkSurface.h"

static sk_sp<GrContext> sContext;
static sk_sp<SkSurface> sSurface;

void init_skia(int w, int h) {
    GrContextOptions options;
    //options.fRequireDecodeDisableForSRGB = false;
    sContext = GrContext::MakeGL(nullptr);//, options);

    GrGLFramebufferInfo framebufferInfo;
    framebufferInfo.fFBOID = 0; // assume default framebuffer
    // We are always using OpenGL and we use RGBA8 internal format for both RGBA and BGRA configs in OpenGL.
    //(replace line below with this one to enable correct color spaces) framebufferInfo.fFormat = GL_SRGB8_ALPHA8;
    framebufferInfo.fFormat = GL_RGBA8;

    SkColorType colorType;
    if (kRGBA_8888_GrPixelConfig == kSkia8888_GrPixelConfig) {
        colorType = kRGBA_8888_SkColorType;
    }
    else {
        colorType = kBGRA_8888_SkColorType;
    }
    GrBackendRenderTarget backendRenderTarget(w, h,
        0, // sample count
        0, // stencil bits
        framebufferInfo);

    //(replace line below with this one to enable correct color spaces) sSurface = SkSurface::MakeFromBackendRenderTarget(sContext, backendRenderTarget, kBottomLeft_GrSurfaceOrigin, colorType, SkColorSpace::MakeSRGB(), nullptr).release();

    sSurface = SkSurface::MakeFromBackendRenderTarget(sContext.get(), backendRenderTarget, kBottomLeft_GrSurfaceOrigin, colorType, nullptr, nullptr);
    if (sSurface == nullptr)
        abort();
}

void gl_example(int width, int height) {
    // You've already created your OpenGL context and bound it.
    const GrGLInterface* interface = nullptr;
    // Leaving interface as null makes Skia extract pointers to OpenGL functions for the current
    // context in a platform-specific way. Alternatively, you may create your own GrGLInterface and
    // initialize it however you like to attach to an alternate OpenGL implementation or intercept
    // Skia's OpenGL calls.

    sContext = GrContext::MakeGL(nullptr);//GrContext::MakeGL(interface);
    SkImageInfo info = SkImageInfo:: MakeN32Premul(width, height);
    sSurface = SkSurface::MakeRenderTarget(sContext.get(), SkBudgeted::kNo, info);
    if (!sSurface) {
        SkDebugf("SkSurface::MakeRenderTarget returned null\n");
        return;
    }
}

void another_skia_init() {
//    // You've already created your OpenGL context and bound it.
//    sk_sp<const GrGLInterface> interface = nullptr;
//    // Leaving interface as null makes Skia extract pointers to OpenGL functions for the current
//    // context in a platform-specific way. Alternatively, you may create your own GrGLInterface and
//    // initialize it however you like to attach to an alternate OpenGL implementation or intercept
//    // Skia's OpenGL calls.

//    sk_sp<GrContext> context = GrContext::MakeGL(interface);


//    SkImageInfo info = SkImageInfo:: MakeN32Premul(300, 300);
//    sk_sp<SkSurface> gpuSurface(
//            SkSurface::MakeRenderTarget(context, SkBudgeted::kNo, info));
//    if (!gpuSurface) {
//        SkDebugf("SkSurface::MakeRenderTarget returned null\n");
//        return;
//    }
//    SkCanvas* gpuCanvas = gpuSurface->getCanvas();
//    draw(gpuCanvas);
//    sk_sp<SkImage> img(gpuSurface->makeImageSnapshot());
//    if (!img) { return; }
//    sk_sp<SkData> png(img->encode());
//    if (!png) { return; }
//    SkFILEWStream out(path);
//    (void)out.write(png->data(), png->size());

}

int main(void)
{
    GLFWwindow* window;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    window = glfwCreateWindow(640, 480, "Skia on GLFW", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

   init_skia(300,300);
    //gl_example(300,300);
    SkCanvas* canvas = sSurface->getCanvas();

    while (!glfwWindowShouldClose(window))
    {
        int width,height;
        glfwGetFramebufferSize(window, &width, &height);
        canvas->clear(SK_ColorRED);
        sContext->flush();
        glfwSwapBuffers(window);
        canvas->clear(SK_ColorRED);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

//! [code]
