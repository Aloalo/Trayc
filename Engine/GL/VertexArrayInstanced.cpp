#include <Engine/GL/VertexArrayInstanced.h>
#include <assert.h>

namespace engine
{
    VertexArrayInstanced::VertexArrayInstanced(GLenum VBusage, GLenum IBusage) :
        VertexArray(VBusage), mInstanceBO(GL_ARRAY_BUFFER, IBusage), mSizeInst(0), mCapacityInst(0), mInstSizeInBytes(0)
    {
    }

    VertexArrayInstanced::~VertexArrayInstanced(void)
    {
    }

    void VertexArrayInstanced::Init(int ctVertices, int capacity, const VertexAttribDef &instanceBOdef)
    {
        VertexArray::Init(ctVertices, capacity);

        mInstanceBO.Init();

        mInstSizeInBytes = instanceBOdef.SizeInBytes();

        Bind();
        {
            mInstanceBO.Bind();

            glEnableVertexAttribArray(instanceBOdef.index);
            glVertexAttribPointer(
                instanceBOdef.index,
                instanceBOdef.size,
                instanceBOdef.type,
                instanceBOdef.normalized,
                mInstSizeInBytes,
                (void*)0);
            glVertexAttribDivisor(instanceBOdef.index, 1);

        }
        UnBind();

    }

    void VertexArrayInstanced::ResizeInstanceArray(int size, int reallocSize)
    {
        assert(reallocSize >= 0);
        assert(size >= 0);

        if(size <= mSizeInst)
        {
            mSizeInst = size;
            return;
        }

        mSizeInst = size;
        if(mSizeInst <= mCapacityInst)
            return;

        const GLsizeiptr arraySizeOld = mInstSizeInBytes * mCapacityInst;
        mCapacityInst = mSizeInst + reallocSize;
        const GLsizeiptr arraySizeNew = mInstSizeInBytes * mCapacityInst;

        // Temp buffer for copying
        BufferObject CBO(GL_COPY_READ_BUFFER, GL_STATIC_DRAW);
        CBO.Init();
        CBO.SetData(arraySizeOld);

        // Copy old buffer to new one
        CBO.CopyDataFrom(mInstanceBO, 0, 0, arraySizeOld);
        // Realloc old buffer
        mInstanceBO.SetData(arraySizeNew);
        // Copy to old buffer
        mInstanceBO.CopyDataFrom(CBO, 0, 0, arraySizeOld);
    }

    const BufferObject& VertexArrayInstanced::InstanceBuffer() const
    {
        return mInstanceBO;
    }

    void VertexArrayInstanced::RenderInstanced(GLenum mode, GLint first, GLsizei count, GLsizei primcount) const
    {
        Bind();
        glDrawArraysInstanced(mode, first, count, primcount);
        UnBind();
    }
}
