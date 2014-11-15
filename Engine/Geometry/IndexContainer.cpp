/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Geometry/IndexContainer.h>

using namespace std;

namespace engine
{
    IndexContainer::IndexContainer(const IndexContainer &other)
    {
        *this = other;
    }

    IndexContainer::IndexContainer(const vector<GLuint> &indexData)
        : data(NULL)
    {
        SetData(indexData);
    }

    IndexContainer::IndexContainer(void)
        : indexCount(0), sizeInBytes(0), data(NULL)
    {
    }

    IndexContainer::~IndexContainer(void)
    {
        if(data != NULL)
            delete [] data;
    }

    IndexContainer& IndexContainer::operator=(const IndexContainer &other)
    {
        if(data != NULL)
        {
            delete [] data;
            data = NULL;
        }

        indexCount = other.Size();
        sizeInBytes = other.GetSizeInBytes();

        const GLvoid *otherData = other.GetData();
        if(otherData != NULL)
        {
            data = new GLbyte[sizeInBytes];
            memcpy(data, otherData, sizeInBytes);
        }

        return *this;
    }

    GLuint IndexContainer::Get(GLint idx) const
    {
        if(indexCount <= UCHAR_MAX)
            return static_cast<GLuint>(Get<GLubyte>(idx));
        else if(indexCount <= USHRT_MAX)
            return static_cast<GLuint>(Get<GLushort>(idx));
        else
            return Get<GLuint>(idx);
    }

    GLuint IndexContainer::Size() const
    {
        return indexCount;
    }

    int IndexContainer::GetSizeInBytes() const
    {
        return sizeInBytes;
    }

    const GLvoid* IndexContainer::GetData() const
    {
        return data;
    }

    template <class T>
    void IndexContainer::Cast(const std::vector<GLuint> &indexData)
    {
        data = new T[indexCount];
        for(GLuint i = 0; i < indexCount; i++)
            *((T*)data+i) = (T)indexData[i];

        sizeInBytes = indexCount * sizeof(T);
    }

    GLenum IndexContainer::GetIndexDataType() const
    {
        return indexCount <= UCHAR_MAX ? GL_UNSIGNED_BYTE : (indexCount <= USHRT_MAX ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT);
    }

    template <class T>
    const T& IndexContainer::Get(GLint idx) const
    {
        return *((T*)data+idx);
    }

    void IndexContainer::SetData(const vector<GLuint> &indexData)
    {
        if(data != NULL)
            delete data;
        indexCount = indexData.size();
        if(indexCount <= UCHAR_MAX)
            Cast<GLubyte>(indexData);
        else if(indexCount <= USHRT_MAX)
            Cast<GLushort>(indexData);
        else
            Cast<GLuint>(indexData);
    }
}
