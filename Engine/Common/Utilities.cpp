/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Common/Utilities.h>

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
}
