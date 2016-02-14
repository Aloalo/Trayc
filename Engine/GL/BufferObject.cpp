#include <Engine/GL/BufferObject.h>
#include <assert.h>

namespace engine
{
    BufferObject::BufferObject(GLenum target, GLenum usage) :
        mTarget(target), mUsage(usage), mBO(0)
    {
    }

    BufferObject::~BufferObject(void)
    {
    }

    void BufferObject::Init()
    {
        glDeleteBuffers(1, &mBO);
        glGenBuffers(1, &mBO);
    }

    void BufferObject::Destroy()
    {
        glDeleteBuffers(1, &mBO);
        mBO = 0;
    }

    void BufferObject::Bind() const
    {
        glBindBuffer(mTarget, mBO);
    }

    void BufferObject::UnBind() const
    {
        glBindBuffer(mTarget, 0);
    }

    void BufferObject::SetData(GLsizeiptr size, const GLvoid *data) const
    {
        Bind();
        glBufferData(mTarget, size, data, mUsage);
        UnBind();
    }

    void BufferObject::SubData(GLintptr offset, GLsizeiptr size, const GLvoid *data) const
    {
        Bind();
        glBufferSubData(mTarget, offset, size, data);
        UnBind();
    }

    void BufferObject::CopyDataFrom(const BufferObject &readBuff, GLintptr readoffset, GLintptr writeoffset, GLsizeiptr size) const
    {
        // Bind self as write buffer
        glBindBuffer(GL_COPY_WRITE_BUFFER, mBO);
        readBuff.Bind();
        glCopyBufferSubData(readBuff.Target(), GL_COPY_WRITE_BUFFER, readoffset, writeoffset, size);
        readBuff.UnBind();
        glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
    }

    GLvoid* BufferObject::Map(GLintptr offset, GLsizeiptr length, GLbitfield access) const
    {
        Bind();
        return glMapBufferRange(mTarget, offset, length, access);
    }

    void BufferObject::Flush(GLintptr offset, GLsizeiptr length) const
    {
        glFlushMappedBufferRange(mTarget, offset, length);
    }

    void BufferObject::UnMap() const
    {
        assert(glUnmapBuffer(mTarget));
        UnBind();
    }

    GLenum BufferObject::Target() const
    {
        return mTarget;
    }

    bool BufferObject::Exists() const
    {
        return mBO != 0;
    }
}

