#include "GLWindow.h"

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
