/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Geometry/VertexAttribute.h>

namespace engine
{
    VertexAttribute::VertexAttribute(void)
    {
    }

    VertexAttribute::VertexAttribute(AttributeType attribType, GLuint size, GLenum dataType, GLboolean normalized)
        : attribType(attribType), size(size), dataType(dataType), normalized(normalized)
    {
    }
}
