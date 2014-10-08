#pragma once

#include <OpenGL.h>
#include <ShaderObject.h>

class ShaderProgram
{
public:

    //!
    //!
    //!
    ShaderProgram(GLuint programId)
        : programId(programId)
    {
    }

    //!
    //!
    //!
    ~ShaderProgram()
    {
        glDeleteProgram(programId);
        programId = 0;
    }

    //!
    //!
    //!
    static shared_ptr<ShaderProgram> create()
    {
        shared_ptr<ShaderProgram> program;

        GLuint programId = glCreateProgram();
        if (programId > 0)
        {
            program = shared_ptr<ShaderProgram>(new ShaderProgram(programId));
        }

        return program;
    }

    GLuint getProgramId()
    {
        return programId;
    }

    //!
    //!
    //!
    void attachShader(shared_ptr<ShaderObject> shader)
    {
        glAttachShader(programId, shader->getShaderId());
    }

    //!
    //!
    //!
    void link()
    {
        glLinkProgram(programId);
        glGetProgramiv(programId, GL_LINK_STATUS, &linked);
    }

    //!
    //!
    //!
    bool isLinked()
    {
        return linked == GL_TRUE;
    }

    //!
    //!
    //!
    void use()
    {
        glUseProgram(programId);
    }

private:

    //
    GLuint programId;

    //
    GLint linked;

    // TODO: store shaders in a list
};