#if defined(__ANDROID__)
#include <EGLWindow.h>

void android_main(android_app* app)
{
    app_dummy();

    const Dimension viewportSize(480, 320);
}

#else
#include <Application.h>

int main()
{
    const Dimension windowSize(720, 480);
    const Dimension viewportSize(480, 320);

    unique_ptr<Application> application(new Application("Freetype rendering example", windowSize, viewportSize));
    application->run();
}

#endif
