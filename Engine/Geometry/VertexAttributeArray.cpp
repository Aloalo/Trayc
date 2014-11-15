/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Geometry/VertexAttributeArray.h>
#include <Engine/Common/Utilities.h>

using namespace glm;

namespace engine
{
    VertexAttributeArray::VertexAttributeArray(void)
        : attributeArray(NULL), count(0), sizeInBytes(0)
    {
    }

    VertexAttributeArray::VertexAttributeArray(const VertexAttributeArray &other)
    {
        *this = other;
    }

    VertexAttributeArray::VertexAttributeArray(const VertexAttribute &attribute)
        : attribute(attribute), attributeArray(NULL), count(0), sizeInBytes(0)
    {
    }

    VertexAttributeArray::~VertexAttributeArray()
    {
        if(attributeArray != NULL)
            delete [] attributeArray;
    }

    VertexAttributeArray& VertexAttributeArray::operator=(const VertexAttributeArray &other)
    {
        attribute = other.GetVertexAttrib();
        count = other.GetCount();
        sizeInBytes = other.GetSizeInBytes();

        if(attributeArray != NULL)
        {
            delete [] attributeArray;
            attributeArray = NULL;
        }

        const GLvoid *otherData = other.GetData();
        if(otherData != NULL)
        {
            attributeArray = new GLbyte[sizeInBytes];
            memcpy(attributeArray, otherData, sizeInBytes);
        }

        return *this;
    }

    void VertexAttributeArray::Construct(const GLvoid *data, GLuint count)
    {
        this->count = count;
        sizeInBytes = count * SizeOfGLType(attribute.dataType) * attribute.size;
        if(attributeArray != NULL)
            delete [] attributeArray;
        attributeArray = (GLvoid*)(new GLbyte[sizeInBytes]);
        memcpy(attributeArray, data, sizeInBytes);
    }

    const VertexAttribute& VertexAttributeArray::GetVertexAttrib() const
    {
        return attribute;
    }

    const GLvoid* VertexAttributeArray::GetData() const
    {
        return attributeArray;
    }

    GLsizeiptr VertexAttributeArray::GetSizeInBytes() const
    {
        return sizeInBytes;
    }

    GLuint VertexAttributeArray::GetCount() const
    {
        return count;
    }
}
