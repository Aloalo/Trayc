
#ifndef EN_UTILITIES_H
#define EN_UTILITIES_H

#include <GL/glew.h>
#include <string>
#include <vector>

namespace engine
{
    void StringReplace(std::string &str, const std::string &a, const std::string &b);
    GLsizeiptr SizeOfGLType(GLenum type);
    std::vector<std::string> operator+(const std::vector<std::string> &a, const std::vector<std::string> &b);
}
#endif
