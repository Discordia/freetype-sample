// -*- C++ -*-
#pragma once

#if defined(__ANDROID__)
#include <GLES/gl.h>
#else
#include <GL/glew.h>
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#endif
