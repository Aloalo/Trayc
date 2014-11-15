/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_INDEXCONTAINER_H
#define EN_INDEXCONTAINER_H

#include <vector>
#include <GL/glew.h>

namespace engine
{
    class IndexContainer
    {
    public:
        IndexContainer(void);
        IndexContainer(const IndexContainer &other);
        IndexContainer(const std::vector<GLuint> &indexData);
        ~IndexContainer(void);

        IndexContainer& operator=(const IndexContainer &other);

        void SetData(const std::vector<GLuint> &indexData);

        GLuint Get(GLint idx) const;
        GLuint Size() const;
        GLsizeiptr GetSizeInBytes() const;
        const GLvoid* GetData() const;
        GLenum GetIndexDataType() const;

    private:
        template <class T>
        void Cast(const std::vector<GLuint> &indexData);
        template <class T>
        const T& Get(GLint idx) const;

        GLvoid *data;
        GLuint indexCount;
        GLsizeiptr sizeInBytes;
    };
}

#endif
