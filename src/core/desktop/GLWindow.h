// -*- C++ -*-
#pragma once

#include <string>
#include <memory>
#include <core/Time.h>
#include <core/Dimension.h>

using std::shared_ptr;
using std::string;

struct GLFWwindow;

class GLWindow
{
public:

    /*! @breif Creates a new window
     */
    GLWindow(const string& title, const Dimension windowSize, bool fullscreen = false);

    /*! @breif Destruct the window
     */
    ~GLWindow();

    /*! @breif Initalize the window
     */
    void init();

    /*! @breif Tear down the window
     */
    void destroy() const;

    /*! @breif Query if the window is open or not
     */
    bool isOpen() const;

    /*! @breif Process the enents from GLFW
     */
    void processEvents() const;

    /*! @breif Swap buffers
     */
    void swapBuffers();

    /*! @breif Retrieve the frame time in milliseconds
     */
    float getFrameTime() const;

    /*! @breif Sets the frame limit.
     *
     * The int value passed is the frame limit to be used. 0 is regarded as
     * 'no limit'.
     */
    void setFrameLimit(int frameLimit);

private:

    /*! @brief GLFW key callback.
     */
    static void keyCallback(GLFWwindow* handle, int key, int scancode, int action, int mods);

    /*! @brief GLFW error callback.
     */
    static void errorCallback(int error, const char* description);

private:

    //!
    const Dimension windowSize;

    //!
    GLFWwindow* window;

    //!
    Timer timer;

    //!
    milliseconds frameTime;

    //!
    int frameLimit;
};
