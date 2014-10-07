// -*- C++ -*-
#pragma once

#include <string>
#include <memory>
#include <Dimension.h>

using std::string;
using std::unique_ptr;

class GLWindow;

class Application
{
public:

    //!
    //!
    //!
    Application(const string &title, const Dimension windowSize, const Dimension viewportSize);

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
};
