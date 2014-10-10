#pragma once

#include <OpenGL.h>
#include <memory>
#include <string>
#include "Log.h"

using std::shared_ptr;
using std::string;

class ShaderObject
{
public:
    ShaderObject(GLuint shaderId, const string& shaderSource)
            : shaderId(shaderId), shaderSource(shaderSource)
    {
        // Load the shader source
        const char* shaderSourceData = shaderSource.c_str();

        glShaderSource(shaderId, 1, &shaderSourceData, NULL);

        // Compile the shader
        glCompileShader(shaderId);

        // Check the compile status
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compiled);

        if (compiled != GL_TRUE)
        {
            GLint infoLen = 0;

            glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLen);

            if (infoLen > 1)
            {
                char* infoLog = static_cast<char*>(malloc(sizeof(char) * infoLen));

                glGetShaderInfoLog(shaderId, infoLen, NULL, infoLog);
                LOGE("ShaderObject", "Error compiling shader:\n%s\n", infoLog);

                free(infoLog);
            }
        }
    }

    ~ShaderObject()
    {
        glDeleteShader(shaderId);
        shaderId = 0;
    }

    static shared_ptr<ShaderObject> create(GLenum type, const string& shaderSource)
    {
        shared_ptr<ShaderObject> shader;

        GLuint shaderId = glCreateShader(type);
        if (shaderId > 0)
        {
            shader = shared_ptr<ShaderObject>(new ShaderObject(shaderId, shaderSource));
        }

        return shader;
    }

    const GLuint& getShaderId()
    {
        return shaderId;
    }

    bool isCompiled()
    {
        return compiled == GL_TRUE;
    }

private:

//
    GLuint shaderId;

//
    string shaderSource;

//
    GLint compiled;
};