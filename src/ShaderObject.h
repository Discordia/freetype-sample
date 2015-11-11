#pragma once

#include <OpenGL.h>
#include <memory>
#include <string>

using std::shared_ptr;
using std::string;

class ShaderObject
{
public:

    //!
    //!
    //!
    ShaderObject(GLuint shaderId, const string& shaderSource);

    //!
    //!
    //!
    ~ShaderObject();

    //!
    //!
    //!
    static shared_ptr<ShaderObject> create(GLenum type, const string& shaderSource);

    //!
    //!
    //!
    const GLuint& getShaderId();

    //!
    //!
    //!
    bool isCompiled();

private:

    //
    GLuint shaderId;

    //
    string shaderSource;

    //
    GLint compiled;
};