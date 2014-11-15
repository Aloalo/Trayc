/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_VERTEXATTRIBUTEARRAY_H
#define EN_VERTEXATTRIBUTEARRAY_H

#include <Engine/Geometry/VertexAttribute.h>
#include <Engine/Common/Utilities.h>
#include <glm/glm.hpp>
#include <vector>

namespace engine
{
    //Vertex Attribute specification
    struct VertexAttributeArray
    {
        VertexAttributeArray(void);
        VertexAttributeArray(const VertexAttributeArray &other);
        VertexAttributeArray(const VertexAttribute &attribute);
        ~VertexAttributeArray(void);

        VertexAttributeArray& operator=(const VertexAttributeArray &other);

        //count is the number of attributes
        void Construct(const GLvoid *data, GLuint count);

        const VertexAttribute& GetVertexAttrib() const;
        const GLvoid* GetData() const;
        GLsizeiptr GetSizeInBytes() const;
        GLuint GetCount() const;

    private:
        VertexAttribute attribute;
        GLvoid *attributeArray;
        GLuint count;
        GLsizeiptr sizeInBytes;
    };
}

#endif
