/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_UTILITIES_H
#define EN_UTILITIES_H

#include <GL/glew.h>
#include <string>

namespace engine
{
    void StringReplace(std::string &str, const std::string &a, const std::string &b);
    GLsizeiptr SizeOfGLType(GLenum type);
}
#endif
