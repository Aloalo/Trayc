/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_VERTEXATTRIBUTE_H
#define EN_VERTEXATTRIBUTE_H

#include <GL/glew.h>

namespace engine
{
    //type of attributes,
    //also used as shader locations
    enum AttributeType
    {
        POSITION = 0,
        NORMAL = 1,
        TANGENT = 2,
        BITANGENT = 3,
        COLOR = 4,
        UV = 5,
        USERATTRIB0 = 6
    };

    //Vertex Attribute specification 
    struct VertexAttribute
    {
        VertexAttribute(void);
        VertexAttribute(AttributeType attribType, GLuint size, GLenum dataType, GLboolean normalized);

        AttributeType attribType;
        //number of elements
        GLuint size;
        GLenum dataType;
        GLboolean normalized;
    private:
        GLsizei stride;
        GLuint offset;
    };
}

#endif
