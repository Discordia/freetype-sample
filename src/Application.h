// -*- C++ -*-
#pragma once

#include <string>
#include <memory>
#include <Dimension.h>
#include <Game.h>

using std::string;
using std::unique_ptr;

class GLWindow;

class Application
{
public:

    //!
    //!
    //!
    Application(const string& title, const Dimension windowSize);

    //!
    //!
    //!
    ~Application();


    //!
    //!
    //!
    int run();

private:

    //!
    unique_ptr<GLWindow> window;

    //!
    shared_ptr<Game> game;
};
