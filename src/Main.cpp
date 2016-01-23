
#if defined(__ANDROID__)

//
// Android
//

#include <Activity.h>

void android_main(android_app* app)
{
    app_dummy();

    const Dimension viewportSize(480, 320);
    unique_ptr<Activity> activity(new Activity(viewportSize, app));
    activity->run();
}

#else

//
// Desktop
//

#include <Application.h>

int main()
{
    const Dimension windowSize(1024, 768);

    unique_ptr<Application> application(new Application("Freetype rendering example", windowSize));
    application->run();
}

#endif
