#include <core/ShaderObject.h>
#include <core/OpenGL.h>
#include "Log.h"

#define LOG_TAG "ShaderObject"

ShaderObject::ShaderObject(GLuint shaderId, const string &shaderSource)
        : shaderId(shaderId), shaderSource(shaderSource)
{
    // Load the shader source
    const char *shaderSourceData = shaderSource.c_str();

    glShaderSource(shaderId, 1, &shaderSourceData, nullptr);

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
            char *infoLog = static_cast<char *>(malloc(sizeof(char) * infoLen));

            glGetShaderInfoLog(shaderId, infoLen, nullptr, infoLog);
            LOGE("Error compiling shader:\n%s\n", infoLog);

            free(infoLog);
        }
    }
}

ShaderObject::~ShaderObject()
{
    glDeleteShader(shaderId);
    shaderId = 0;
}

shared_ptr<ShaderObject> ShaderObject::create(GLenum type, const string &shaderSource)
{
    shared_ptr <ShaderObject> shader;

    GLuint shaderId = glCreateShader(type);
    if (shaderId > 0)
    {
        shader = shared_ptr<ShaderObject>(new ShaderObject(shaderId, shaderSource));
    }

    return shader;
}

const GLuint& ShaderObject::getShaderId()
{
    return shaderId;
}

bool ShaderObject::isCompiled()
{
    return compiled == GL_TRUE;
}