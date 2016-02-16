#include <Engine/GL/UniformBuffer.h>
#include <GL/glew.h>

namespace engine
{
    unsigned int UniformBuffer::mBoundBuffer = 0;
    unsigned int UniformBuffer::mCurrentBlockBinding = 0;

    UniformBuffer::UniformBuffer(unsigned int usage) :
        mID(0), mUsage(usage)
    {
        mBlockBinding = mCurrentBlockBinding;
        ++mCurrentBlockBinding;
    }

    UniformBuffer::~UniformBuffer(void)
    {
    }

    void UniformBuffer::Init()
    {
        if(mID == 0)
        {
            glGenBuffers(1, &mID);
            glBindBufferBase(GL_UNIFORM_BUFFER, mBlockBinding, mID);
        }
    }

    void UniformBuffer::Init(int size, const void *data)
    {
        if(mID == 0)
        {
            glGenBuffers(1, &mID);
            SetData(size, data);
            glBindBufferBase(GL_UNIFORM_BUFFER, mBlockBinding, mID);
        }

    }

    void UniformBuffer::Destroy()
    {
        glDeleteBuffers(1, &mID);
        mID = 0;
    }

    void UniformBuffer::Bind() const
    {
        if(mBoundBuffer = mID)
        {
            mBoundBuffer = mID;
            glBindBuffer(GL_UNIFORM_BUFFER, mID);
        }
    }

    void UniformBuffer::SetData(int size, const void *data) const
    {
        Bind();
        glBufferData(GL_UNIFORM_BUFFER, size, data, mUsage);
    }

    void UniformBuffer::SetSubData(int offset, int size, const void *data) const
    {
        Bind();
        glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
    }

    unsigned int UniformBuffer::GetBlockBinding() const
    {
        return mBlockBinding;
    }
}