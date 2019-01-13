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

void init_gr_context() {
    GrContextOptions options;
    //options.fRequireDecodeDisableForSRGB = false;
    sContext = GrContext::MakeGL(nullptr);//, options);
}

void init_surface(int w, int h) {

    if (sSurface != nullptr) {
        sSurface.reset();
    }

    GrGLFramebufferInfo framebufferInfo;
    framebufferInfo.fFBOID = 0; // assume default framebuffer
    // We are always using OpenGL and we use RGBA8 internal format for both RGBA and BGRA configs in OpenGL.
    //(replace line below with this one to enable correct color spaces) framebufferInfo.fFormat = GL_SRGB8_ALPHA8;
    framebufferInfo.fFormat = GL_RGBA8;

    SkColorType colorType = kRGBA_8888_GrPixelConfig == kSkia8888_GrPixelConfig ? kRGBA_8888_SkColorType : kBGRA_8888_SkColorType;
    GrBackendRenderTarget backendRenderTarget(w, h, 0 /* sample count */, 0 /* stencil bits */, framebufferInfo);

    // (replace sSurface creation with this one to enable correct color spaces):
    // sSurface = SkSurface::MakeFromBackendRenderTarget(sContext, backendRenderTarget, kBottomLeft_GrSurfaceOrigin, colorType, SkColorSpace::MakeSRGB(), nullptr).release();

    sSurface = SkSurface::MakeFromBackendRenderTarget(sContext.get(), backendRenderTarget, kBottomLeft_GrSurfaceOrigin, colorType, nullptr, nullptr);
    if (sSurface == nullptr)
        abort();
}

void draw_content() {
    SkCanvas* canvas = sSurface->getCanvas();
    canvas->clear(SK_ColorRED);
    SkRect r = SkRect::MakeXYWH(1,1,598,398);
    SkPaint p;
    p.setColor(SK_ColorGREEN);
    canvas->drawRect(r,p);
    r.setXYWH(1,1,298,198);
    p.setColor(SK_ColorBLUE);
    canvas->drawRect(r,p);
    sContext->flush();
}

void fbCallback(GLFWwindow* window,int w,int h) {
    printf("FB w %d, h %d\n",w,h);
    init_surface(w,h);
    draw_content();
    glfwSwapBuffers(window);
}

//void sizeCallback(GLFWwindow* window,int w,int h) {
//    printf("WIN w %d, h %d\n",w,h);
//    init_skia(w,h);
//    draw_content();
//    glfwSwapBuffers(window);
//}

int main(void)
{
    printf("hello\n");
    GLFWwindow* window;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    window = glfwCreateWindow(600, 400, "Skia on GLFW", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetFramebufferSizeCallback(window,fbCallback);
    //glfwSetWindowSizeCallback(window,sizeCallback);

    int width,height;
    glfwGetWindowSize(window, &width, &height);
    printf("Win initial dim %d %d\n ",width,height);

    glfwGetFramebufferSize(window, &width, &height);
    printf("FB initial dim %d %d\n ",width,height);

    init_gr_context();
    init_surface(width,height);
    draw_content();
    glfwSwapBuffers(window);

    glfwGetWindowSize(window, &width, &height);
    printf("Win dimensions now: %d %d\n ",width,height);
    glfwGetFramebufferSize(window, &width, &height);
    printf("FB dimensions now: %d %d\n ",width,height);
    float x, y;
    glfwGetWindowContentScale(window, &x, &y);
    printf("scale %f %f\n",x,y);

    while (!glfwWindowShouldClose(window))
    {
        glfwWaitEvents(); // this blocks, use "poll" for non-blocking
        draw_content();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
