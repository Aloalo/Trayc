/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_VERTEX_ARRAY_H
#define EN_VERTEX_ARRAY_H

#include <Engine/GL/BufferObject.h>
#include <vector>

namespace engine
{
    struct VertexAttribDef
    {
        VertexAttribDef(GLint index, GLint size, GLenum type, GLboolean normalized);

        int SizeInBytes() const;

        GLuint index;
        GLint size;
        GLenum type;
        GLboolean normalized;
    };

    class VertexArray
    {
        using VertexAttribs = std::vector<VertexAttribDef>;
    public:

        VertexArray(GLenum VBOusage);
        ~VertexArray(void);

        void AddAttribute(const VertexAttribDef &attrib);
        void Init(int ctVertices, int capacity);
        // Ensures that the data store is big enaugh for size vertices
        // Sets mSize to size
        // if size > mSize not allocates size+reallocSize space
        void Resize(int size, int reallocSize);

        void Bind() const;
        static void UnBind();

        void SetVertices(const GLvoid *data, int idx, int ctVertices = 1) const;
        const BufferObject& VBO() const;

        void Render(GLenum mode, GLint first = 0) const;

    private:
        void RegisterToGPU();
        // In bytes
        int VertexSize() const;

        int mSize;
        int mCapacity;
        int mVertexSize;

        GLuint mVAO;
        BufferObject mVBO;

        VertexAttribs mVertAttribs;
    };
}

#endif

