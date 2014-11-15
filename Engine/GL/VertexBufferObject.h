/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_VERTEXBUFFEROBJECT_H
#define EN_VERTEXBUFFEROBJECT_H

#include <Engine/Geometry/VertexAttribute.h>
#include <vector>

namespace engine
{
    //type of formatting
    enum Formatting
    {
        //(VNCVNCVNCVNC)
        INTERLEAVED,
        //(VVVVNNNNCCCC)
        BATCH
    };

    class VertexBufferObject
    {
    public:
        Formatting formatting;
        std::vector<VertexAttribute> attributes;
        GLsizeiptr size;
        GLenum usage;
    };
}

#endif
