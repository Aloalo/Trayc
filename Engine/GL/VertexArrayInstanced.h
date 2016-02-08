/*
* Copyright (c) 2014 Jure Ratkovic
*/


#ifndef EN_VERTEX_ARRAY_INSTANCED_H
#define EN_VERTEX_ARRAY_INSTANCED_H

#include <Engine/GL/VertexArray.h>

namespace engine
{
    class VertexArrayInstanced : public VertexArray
    {
    public:
        // Vertex buffer usage, instance buffer usage
        VertexArrayInstanced(GLenum VBusage, GLenum IBusage);
        ~VertexArrayInstanced(void);

        void Init(int ctVertices, int capacity, const VertexAttribDef &instanceBOdef);
        // Ensures that the data store is big enaugh for size instances
        // Sets mSize to size
        // if size > mSize not allocates size+reallocSize space
        void ResizeInstanceArray(int size, int reallocSize);

        // Instance buffer object
        const BufferObject& InstanceBuffer() const;

        void RenderInstanced(GLenum mode, GLint first, GLsizei count, GLsizei primcount) const;

    private:
        // Instance buffer
        BufferObject mInstanceBO;
        // Number of instances
        int mSizeInst;
        // Capacity of instance array
        int mCapacityInst;
        // Instance data size in bytes
        int mInstSizeInBytes;
    };
}

#endif
