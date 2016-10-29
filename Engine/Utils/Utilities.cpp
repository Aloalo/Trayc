/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Utils/Utilities.h>

using namespace std;

namespace engine
{
    GLsizeiptr SizeOfGLType(GLenum type)
    {
        switch(type)
        {
        case GL_BYTE:
            return sizeof(GLbyte);
        case GL_UNSIGNED_BYTE:
            return sizeof(GLubyte);
        case GL_SHORT:
            return sizeof(GLshort);
        case GL_UNSIGNED_SHORT:
            return sizeof(GLushort);
        case GL_INT:
            return sizeof(GLint);
        case GL_UNSIGNED_INT:
            return sizeof(GLuint);
        case GL_HALF_FLOAT:
            return sizeof(GLhalf);
        case GL_FLOAT:
            return sizeof(GLfloat);
        case GL_DOUBLE:
            return sizeof(GLdouble);
        case GL_INT_2_10_10_10_REV:
            return 4;
        case GL_UNSIGNED_INT_2_10_10_10_REV:
            return 4;
        default:
            return -1;
        }
    }

    vector<string> operator+(const vector<string> &a, const vector<std::string> &b)
    {
        vector<string> ret;
        ret.reserve(a.size() + b.size());
        for(const string &x : a) {
            ret.push_back(x);
        }
        for(const string &x : b) {
            ret.push_back(x);
        }
        return ret;
    }

    void StringReplace(string &str, const string &a, const string &b)
    {
        size_t index = 0;
        const size_t asize = a.length();
        const size_t bsize = b.length();
        while(true)
        {
            index = str.find(a, index);
            if(index == string::npos)
                break;

            str.replace(index, asize, b);
            index += bsize;
        }
    }

}
