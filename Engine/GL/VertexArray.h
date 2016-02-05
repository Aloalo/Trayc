/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_VERTEX_ARRAY_H
#define EN_VERTEX_ARRAY_H

#include <Engine/GL/BufferObject.h>
#include <Engine/GL/VertexAttributeDef.h>
#include <vector>

namespace engine
{
    class VertexArray
    {
        using VertexAttribs = std::vector<VertexAttribDef>;
    public:

        VertexArray(GLenum VBOusage = GL_STATIC_DRAW);
        ~VertexArray(void);

        void AddAttribute(const VertexAttribDef &attrib);
        void AddAttributes(const VertexAttribs &attribs);
        void Init(int ctVertices, int capacity);
        void Destroy();

        void SetIndices(const void *indices, int ctIndices, GLenum type);

        // Ensures that the data store is big enaugh for size vertices
        // Sets mSize to size
        // if size > mSize not allocates size+reallocSize space
        void Resize(int size, int reallocSize);

        void Bind() const;
        static void UnBind();

        void SetVertices(const GLvoid *data, int idx, int ctVertices = 1) const;
        const BufferObject& VBO() const;

        void Render(GLenum mode, GLint first = 0) const;
        void RenderIndexed(GLenum mode) const;

        int Size() const;
        int Capacity() const;

    private:
        void RegisterToGPU();
        // In bytes
        int VertexSize() const;

        int mSize;
        int mCapacity;
        int mVertexSize;

        GLuint mVAO;
        // Vertex buffer
        BufferObject mVBO;
        // Index buffer
        BufferObject mIBO;
        GLenum mIndexType;
        int mCtIndices;

        VertexAttribs mVertAttribs;
    };
}

#endif

