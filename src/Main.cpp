#if defined(__ANDROID__)
#include <EGLWindow.h>

void android_main(android_app* app)
{
    app_dummy();

    const Dimension viewportSize(480, 320);

    shared_ptr<SceneSystem> sceneSystem = shared_ptr<SceneSystem>(new SceneSystem());
    shared_ptr<ControlsFactory> controlsFactory(new DPadControlsFactory(viewportSize));

    shared_ptr<Scene> gameScene = shared_ptr<Scene>(new GameScene(controlsFactory));
    sceneSystem->addScene(gameScene);

    Activity activity(viewportSize, app, sceneSystem);
    activity.run();
}

#else
#include <GLWindow.h>

int main()
{
    const Dimension windowSize(720, 480);
    const Dimension viewportSize(480, 320);

    shared_ptr<GLWindow> window(new GLWindow("Freetype rendering example", windowSize, viewportSize));
    window->setFrameLimit(60);

    window->init();
    while (window->isOpen())
    {
        float frameTime = window->getFrameTime();

        window->processEvents();

        // TODO: Draw something



        window->swapBuffers();
    }

    window->destroy();

    return EXIT_SUCCESS;
}

#endif
