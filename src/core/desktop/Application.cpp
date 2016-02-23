// -*- C++ -*-
#include <core/desktop/Application.h>
#include <core/desktop/GLWindow.h>
#include "DesktopStreamFactory.h"

Application::Application(const string& title, const Dimension windowSize)
{
    window = unique_ptr<GLWindow>(new GLWindow(title, windowSize, false));
    window->setFrameLimit(60);

    this->game = shared_ptr<Game>(new Game(shared_ptr<StreamFactory>(new DesktopStreamFactory("assets"))));
}

Application::~Application()
{
}

int Application::run()
{
    window->init();
    game->init(window->getSize());

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
