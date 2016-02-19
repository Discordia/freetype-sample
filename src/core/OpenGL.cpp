// -*- C++ -*-
#include <core/OpenGL.h>
#include <cmath>

void oglFrustumf(float* matrix, float left, float right, float bottom, float top, float znear, float zfar)
{
    float temp, temp2, temp3, temp4;

    temp = 2.0f * znear;
    temp2 = right - left;
    temp3 = top - bottom;
    temp4 = zfar - znear;

    matrix[0] = temp / temp2;
    matrix[1] = 0.0f;
    matrix[2] = 0.0f;
    matrix[3] = 0.0f;
    matrix[4] = 0.0f;
    matrix[5] = temp / temp3;
    matrix[6] = 0.0f;
    matrix[7] = 0.0f;
    matrix[8] = (right + left) / temp2;
    matrix[9] = (top + bottom) / temp3;
    matrix[10] = (-zfar - znear) / temp4;
    matrix[11] = -1.0f;
    matrix[12] = 0.0f;
    matrix[13] = 0.0f;
    matrix[14] = (-temp * zfar) / temp4;
    matrix[15] = 1.0f;
}

void oglPerspectivef(float* matrix, float fovyInDegrees, float aspectRatio, float znear, float zfar)
{
    float ymax, xmax;
    ymax = znear * tanf(fovyInDegrees * (float) M_PI / 360.0f);
    xmax = ymax * aspectRatio;

    oglFrustumf(matrix, -xmax, xmax, -ymax, ymax, znear, zfar);
}

void oglOrthof(float* matrix, float l, float r, float b, float t, float n, float f)
{
    matrix[0]  = 2.0f / (r - l);
    matrix[1]  = 0.0f;
    matrix[2]  = 0.0f;
    matrix[3]  = 0.0f;
    matrix[4]  = 0.0f;
    matrix[5]  = 2.0f / (t - b);
    matrix[6]  = 0.0f;
    matrix[7]  = 0.0f;
    matrix[8]  = 0.0f;
    matrix[9]  = 0.0f;
    matrix[10] = -2.0f / (f - n);
    matrix[11] = 0.0f;
    matrix[12] = -(r + l) / (r - l);
    matrix[13] = -(t + b) / (t - b);
    matrix[14] = -(f + n) / (f - n);
    matrix[15] = 1.0f;
}