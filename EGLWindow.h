// -*- C++ -*-
#pragma once

#include <EGL/egl.h>
#include <GLES/gl.h>

#include <audrey/app/android/Clock.h>
#include <audrey/math/Dimension.h>

struct android_app;

namespace audrey
{
    class EGLWindow
    {
    public:

        //!
        //!
        //!
        EGLWindow(android_app* app, int32_t width, int32_t height);

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
        float32 getFrameTime() const;

        //!
        //!
        //!
        void setFramerateLimit(const int32_t fpsLimit);

        //!
        //!
        //!
        Dimension getSize();


    private:
        android_app* app;
        EGLDisplay display;
        EGLSurface surface;
        EGLContext context;

        int32_t scrWidth;
        int32_t scrHeight;
        int32_t width;
        int32_t height;

        uint32_t fpsLimit;
        Clock clock;
        float32 lastFrameTime;
    };
}
