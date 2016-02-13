#include <Engine/GL/UniformBuffer.h>

namespace engine
{
    UniformBuffer::UniformBuffer(void) :
        id(0)
    {
    }

    UniformBuffer::~UniformBuffer(void)
    {
    }

    void UniformBuffer::Init()
    {
        glGenBuffers(1, &id);
    }

    void UniformBuffer::Init(int size, void *data)
    {
        glGenBuffers(1, &id);
        setData(size, data);
    }

    void UniformBuffer::Destroy()
    {
        glDeleteBuffers(1, &id);
        id = 0;
    }

    void UniformBuffer::bind()
    {
        if(boundBuffer != this)
        {
            boundBuffer = this;
            glBindBuffer(GL_UNIFORM_BUFFER, id);
        }
    }

    void UniformBuffer::setData(int size, void *data, int usage)
    {
        bind();
        glBufferData(GL_UNIFORM_BUFFER, size, data, usage);
    }

    void UniformBuffer::setSubData(int offset, int size, void *data)
    {
        bind();
        glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
    }

    void UniformBuffer::bindToPoint(int point) const
    {
        glBindBufferBase(GL_UNIFORM_BUFFER, point, id);
    }

    UniformBuffer *UniformBuffer::boundBuffer = nullptr;
}