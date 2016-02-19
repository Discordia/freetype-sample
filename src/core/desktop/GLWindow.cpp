// -*- C++ -*-
#include <core/desktop/GLWindow.h>
#include <core/Log.h>

#include <thread>
#include <cstdlib>

#include <core/OpenGL.h>

using namespace std;

#define LOG_TAG "GLWindow"

//!
const int DEFAULT_FPS = 60;

//!
const int DEFAULT_BIT_DEPTH = 32;


GLWindow::GLWindow(const string& title, const Dimension windowSize, bool fullscreen)
    : windowSize(windowSize), frameTime(0), frameLimit(0)
{
    if (!glfwInit())
    {
        exit(EXIT_FAILURE);
    }

    // TODO: assert that window dimensions is of same aspect ratio as screen
    glfwSetErrorCallback(errorCallback);
    glfwWindowHint(GLFW_REFRESH_RATE, DEFAULT_FPS);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();

    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);


    if (fullscreen)
    {
        window = glfwCreateWindow(mode->width, mode->height, title.c_str(), monitor, nullptr);
    }
    else
    {
        window = glfwCreateWindow(windowSize.width, windowSize.height, title.c_str(), nullptr, nullptr);
    }

    if (!window)
    {
        LOGE("Failed to create GLFW window");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Make new window context current
    glfwMakeContextCurrent(window);

    // Initalize GLEW
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        LOGE("Failed to initalize GLEW");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Retrieve frame buffer size
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    this->windowSize.width = width;
    this->windowSize.height = height;

    LOGI("Frame Buffer size [%i,%i]", width, height);
    LOGI("Initalized");
}

GLWindow::~GLWindow()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void GLWindow::init()
{
    glfwSwapInterval(1);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // Register key listener
    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, keyCallback);

    //
    // Setup OpenGL
    //


    // Shading and color
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    //glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_DITHER);
    glDisable(GL_LIGHTING);

    glViewport(0, 0, windowSize.width, windowSize.height);

    // reset frame time
    timer.reset();
}

void GLWindow::destroy() const
{
}

bool GLWindow::isOpen() const
{
    return !glfwWindowShouldClose(window);
}

const Dimension& GLWindow::getSize() const
{
    return windowSize;
}

void GLWindow::processEvents() const
{
    glfwPollEvents();
}

void GLWindow::swapBuffers()
{
    // Limit the framerate if needed
    if (frameLimit > 0)
    {
        milliseconds defaultFrameTime = milliseconds(1000 / frameLimit);
        milliseconds elapsedTime = timer.elapsedTimeMs();

        if (elapsedTime < defaultFrameTime)
        {
            milliseconds remainingFrameTime = defaultFrameTime - elapsedTime;
            this_thread::sleep_for(remainingFrameTime);
        }
    }

    frameTime = timer.elapsedTimeMs();
    timer.reset();

    glfwSwapBuffers(window);
}

float GLWindow::getFrameTime() const
{
    return static_cast<float>(frameTime.count());
}

void GLWindow::setFrameLimit(int frameLimit)
{
    this->frameLimit = frameLimit;
}

void GLWindow::keyCallback(GLFWwindow* handle, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(handle, GL_TRUE);
    }
    else
    {
        GLWindow* window = static_cast<GLWindow*>(glfwGetWindowUserPointer(handle));
        //            window->forwardKeyInput(key, scancode, action, mods);
    }
}

void GLWindow::errorCallback(int error, const char* description)
{
    string message = "GLFW error: " + string(description);
    LOGE(message);
}
