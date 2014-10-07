// -*- C++ -*-
#include <Activity.h>
#include <Log.h>

const int DEFAULT_FPS = 60;

Activity::Activity(const Dimension viewportSize, android_app *app)
        : app(app), viewportSize(viewportSize)
{
    LOGD("Activity", "ACTIVITY::CONSTRUCTOR");

    // EGL Window - the surface we draw to
    window = unique_ptr<EGLWindow>(new EGLWindow(app, viewportSize.width, viewportSize.height));
    window->setFramerateLimit(DEFAULT_FPS);
}

int32_t Activity::handleInput(AInputEvent *event)
{
    return 0;
}

static int32_t handle_input(struct android_app *app, AInputEvent *event)
{
    return ((Activity *) app->userData)->handleInput(event);
}

void Activity::handleCmd(int32_t cmd)
{
    switch (cmd)
    {
        case APP_CMD_SAVE_STATE:
            // TODO: save state support
            break;

        case APP_CMD_CONFIG_CHANGED:
            LOGD("Activity", "APP_CMD_CONFIG_CHANGED");
            break;

        case APP_CMD_INIT_WINDOW:
            LOGD("Activity", "APP_CMD_INIT_WINDOW");
            if (app->window != NULL)
            {
                window->init();
            }
            break;

        case APP_CMD_TERM_WINDOW:
            LOGD("Activity", "APP_CMD_TERM_WINDOW");
            window->destroy();
            break;

        case APP_CMD_START:
            LOGD("Activity", "APP_CMD_START");
            break;

        case APP_CMD_RESUME:
            LOGD("Activity", "APP_CMD_RESUME");
            break;

        case APP_CMD_PAUSE:
            LOGD("Activity", "APP_CMD_PAUSE");
            break;

        case APP_CMD_STOP:
            LOGD("Activity", "APP_CMD_STOP");
            break;

        case APP_CMD_DESTROY:
            LOGD("Activity", "APP_CMD_DESTROY");
            break;
    }
}

static void handle_cmd(struct android_app *app, int32_t cmd)
{
    ((Activity *) app->userData)->handleCmd(cmd);
}

void Activity::run()
{
    app->userData = this;
    app->onAppCmd = handle_cmd;
    app->onInputEvent = handle_input;

    // TODO: restore state

    // Main loop
    while (1)
    {
        int ident;
        int events;
        android_poll_source *source;

        // Process events
        while ((ident = ALooper_pollAll(0, NULL, &events, (void **) &source)) >= 0)
        {
            if (source != NULL)
            {
                source->process(app, source);
            }

            //We are exiting
            if (app->destroyRequested != 0)
            {
                window->destroy();
                return;
            }
        }


        if (window->hasDisplay())
        {
            float frameTime = window->getFrameTime();

            game.render();

            window->swapBuffers();
        }
    }
}
