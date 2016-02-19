// -*- C++ -*-
#pragma once

#include <EGL/egl.h>

#include <core/Time.h>
#include <core/Dimension.h>

struct android_app;

class EGLWindow
{
public:

    //!
    //!
    //!
    EGLWindow(android_app *app);

    //!
    //! Returns
    //!
    void init();

    //!
    //!
    //!
    void destroy();

    //!
    //!
    //!
    bool hasDisplay() const;

    //!
    //!
    //!
    void swapBuffers();

    //!
    //!
    //!
    float getFrameTime() const;

    //!
    //!
    //!
    void setFramerateLimit(const int32_t fpsLimit);

    //!
    //!
    //!
    const Dimension& getSize() const;


private:

    //! window context
    android_app *app;
    Dimension windowSize;

    //! EGL context
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;

    //! FPS limiting
    Timer timer;
    milliseconds frameTime;
    uint32_t frameLimit;
};
