#include <Engine/GL/VertexArray.h>
#include <Engine/Utils/Utilities.h>
#include <assert.h>

namespace engine
{
    GLuint VertexArray::mBoundVAO = 0;

    VertexArray::VertexArray(GLenum VBOusage) :
        mSize(0), mCapacity(0), mVertexSize(0), mVAO(0), mVBO(GL_ARRAY_BUFFER, VBOusage), mIBO(GL_ELEMENT_ARRAY_BUFFER, VBOusage), mCtIndices(0)
    {
    }

    VertexArray::~VertexArray(void)
    {
    }

    void VertexArray::Resize(int size, int reallocSize)
    {
        assert(reallocSize >= 0);
        assert(size >= 0);

        if(size <= mSize)
        {
            mSize = size;
            return;
        }

        mSize = size;
        if(mSize <= mCapacity)
            return;

        const GLsizeiptr arraySizeOld = mVertexSize * mCapacity;
        mCapacity = mSize + reallocSize;
        const GLsizeiptr arraySizeNew = mVertexSize * mCapacity;

        // Temp buffer for copying
        BufferObject CBO(GL_COPY_READ_BUFFER, GL_STATIC_DRAW);
        CBO.Init();
        CBO.SetData(arraySizeOld);

        // Copy old buffer to new one
        CBO.CopyDataFrom(mVBO, 0, 0, arraySizeOld);
        // Realloc old buffer
        mVBO.SetData(arraySizeNew);
        // Copy to old buffer
        mVBO.CopyDataFrom(CBO, 0, 0, arraySizeOld);
    }

    void VertexArray::Bind() const
    {
        if(mBoundVAO != mVAO) {
            glBindVertexArray(mVAO);
            mBoundVAO = mVAO;
        }
    }

    void VertexArray::UnBind()
    {
        glBindVertexArray(0);
        mBoundVAO = 0;
    }

    void VertexArray::SetVertices(const GLvoid *data, int idx, int ctVertices) const
    {
        if(ctVertices == 0)
            return;
        assert(ctVertices > 0);
        assert(data);
        assert(idx >= 0 && idx < mSize);

        const GLsizeiptr arraySize = mVertexSize * ctVertices;
        const GLsizeiptr offset = mVertexSize * idx;

        mVBO.SubData(offset, arraySize, data);
    }

    const BufferObject& VertexArray::VBO() const
    {
        return mVBO;
    }

    void VertexArray::AddAttribute(const VertexAttribDef &attrib)
    {
        mVertAttribs.push_back(attrib);
    }

    void VertexArray::AddAttributes(const VertexAttribs &attribs)
    {
        mVertAttribs.insert(mVertAttribs.end(), attribs.begin(), attribs.end());
    }

    void VertexArray::Init(int ctVertices, int capacity)
    {
        assert(mVertAttribs.size());
        assert(capacity > 0);
        assert(ctVertices >= 0);

        mSize = ctVertices;
        mCapacity = capacity;
        RegisterToGPU();

        const GLsizeiptr arraySize = mVertexSize * mCapacity;

        mVBO.SetData(arraySize);
    }

    void VertexArray::Destroy()
    {
        glDeleteVertexArrays(1, &mVAO);
        mVBO.Destroy();
        mIBO.Destroy();
    }

    void VertexArray::SetIndices(const void *indices, int ctIndices, GLenum type)
    {
        mCtIndices = ctIndices;
        mIndexType = type;

        mIBO.Destroy();
        mIBO.Init();

        Bind();
        mIBO.Bind();
        UnBind();

        mIBO.SetData(ctIndices * SizeOfGLType(type), indices);
    }

    void VertexArray::RegisterToGPU()
    {
        glDeleteVertexArrays(1, &mVAO);
        glGenVertexArrays(1, &mVAO);
        mVBO.Init();

        mVertexSize = VertexSize();

        Bind();
        {
            mVBO.Bind();

            int offset = 0;
            for(const VertexAttribDef &attrib : mVertAttribs)
            {
                glEnableVertexAttribArray(attrib.index);
                glVertexAttribPointer(
                    attrib.index,
                    attrib.size,
                    attrib.type,
                    attrib.normalized,
                    mVertexSize,
                    (void*)offset);
                offset += attrib.SizeInBytes();
            }

        }
        UnBind();
    }

    void VertexArray::Render(GLenum mode) const
    {
        Bind();
        if(mCtIndices) {
            glDrawElements(mode, mCtIndices, mIndexType, nullptr);
        }
        else {
            glDrawArrays(mode, 0, mSize);
        }
    }

    int VertexArray::Size() const
    {
        return mSize;
    }

    int VertexArray::Capacity() const
    {
        return mCapacity;
    }

    int VertexArray::VertexSize() const
    {
        int ret = 0;
        for(const VertexAttribDef &attrib : mVertAttribs)
            ret += attrib.size * SizeOfGLType(attrib.type);
        return ret;
    }
}
