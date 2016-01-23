// -*- C++ -*-
#include <GLWindow.h>
#include <Log.h>

#include <thread>
#include <cstdlib>

#include <OpenGL.h>

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

    GLFWmonitor* primaryMonitor = nullptr;
    if (fullscreen)
    {
        primaryMonitor = glfwGetPrimaryMonitor();
    }

    window = glfwCreateWindow(windowSize.width, windowSize.height, title.c_str(), primaryMonitor, nullptr);
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
