// -*- C++ -*-
#include <Application.h>
#include <GLWindow.h>
#include "DesktopStreamFactory.h"

// TODO :assert that windowSize and viewportSize has the same aspect ratio
Application::Application(const string &title, const Dimension windowSize, const Dimension viewportSize)
{
    window = unique_ptr<GLWindow>(new GLWindow(title, windowSize, viewportSize));
    window->setFrameLimit(60);

    this->game = shared_ptr<Game>(new Game(shared_ptr<StreamFactory>(new DesktopStreamFactory("assets"))));
}

Application::~Application()
{
}

int Application::run()
{
    window->init();
    game->init();

    while (window->isOpen())
    {
        float frameTime = window->getFrameTime();

        window->processEvents();

        game->render();

        window->swapBuffers();
    }

    window->destroy();

    return EXIT_SUCCESS;
}
