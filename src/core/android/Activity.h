// -*- C++ -*-
#pragma once

#include <jni.h>
#include <errno.h>

#include <android_native_app_glue.h>

#include <memory>

#include <core/android/EGLWindow.h>
#include <core/Dimension.h>
#include <Game.h>

using std::unique_ptr;

struct android_app;
struct AInputEvent;

class Game;

class Activity
{
public:

    //!
    //!
    //!
    Activity(android_app *app);

    //!
    //!
    //!
    int32_t handleInput(AInputEvent *event);

    //!
    //!
    //!
    void handleCmd(int32_t cmd);

    //!
    //!
    //!
    void run();

private:

    //!
    android_app *app;

    //!
    unique_ptr<EGLWindow> window;

    //!
    shared_ptr<Game> game;
};