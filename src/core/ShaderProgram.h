#pragma once

#include <core/ShaderObject.h>
#include <vector>

using std::vector;

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
        shaders.clear();

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
        shaders.push_back(shader);
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

    //
    vector<shared_ptr<ShaderObject>> shaders;
};