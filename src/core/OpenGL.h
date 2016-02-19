// -*- C++ -*-
#pragma once

#if defined(__ANDROID__)
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif

//!
//!
//!
void oglFrustumf(float* matrix, float left, float right, float bottom, float top, float znear, float zfar);


//!
//!
//!
void oglPerspectivef(float* matrix, float fovyInDegrees, float aspectRatio, float znear, float zfar);

//!
//!
//!
void oglOrthof(float* matrix, float l, float r, float b, float t, float n, float f);