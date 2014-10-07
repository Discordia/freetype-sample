#pragma once

#include <OpenGL.h>
#include <memory>

using std::shared_ptr;

class ShaderObject
{
public:
    ShaderObject(GLuint shaderId, const string& shaderSource)
            : shaderId(shaderId), shaderSource(shaderSource)
    {
        // Load the shader source
        glShaderSource(shaderId, 1, shaderSource, NULL);

        // Compile the shader
        glCompileShader(shaderId);

        // Check the compile status
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compiled);
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