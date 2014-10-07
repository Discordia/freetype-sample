// -*- C++ -*-
#pragma once

#include <EGL/egl.h>
#include <GLES/gl.h>

#include <Time.h>
#include <Dimension.h>

struct android_app;

class EGLWindow
{
public:

    //!
    //!
    //!
    EGLWindow(android_app *app, int32_t width, int32_t height);

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
    Dimension getSize();


private:

    // android app
    android_app *app;

    // EGL context
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;

    // dimensions
    int32_t scrWidth;
    int32_t scrHeight;
    int32_t width;
    int32_t height;

    //!
    Timer timer;

    //!
    milliseconds frameTime;

    //
    uint32_t frameLimit;
};
