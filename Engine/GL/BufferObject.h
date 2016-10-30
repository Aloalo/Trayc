
#ifndef EN_BUFFER_OBJECT_H
#define EN_BUFFER_OBJECT_H

#include <GL/glew.h>

namespace engine
{
    class BufferObject
    {
    public:
        // BufferObject(const BufferObject &other) = delete;
        // BufferObject& operator=(const BufferObject &other) = delete;

        BufferObject(GLenum target, GLenum usage);
        ~BufferObject(void);

        void Init();
        void Destroy();

        void Bind() const;
        void UnBind() const;

        void SetData(GLsizeiptr size, const GLvoid *data = nullptr) const;
        void SubData(GLintptr offset, GLsizeiptr size, const GLvoid *data) const;
        void CopyDataFrom(const BufferObject &readBuff, GLintptr readoffset, GLintptr writeoffset, GLsizeiptr size) const;
        GLvoid* Map(GLintptr offset, GLsizeiptr length, GLbitfield access) const;
        void Flush(GLintptr offset, GLsizeiptr length) const;
        void UnMap() const;

        GLenum Target() const;

        // Has the buffer object been created
        bool Exists() const;

    private:
        GLenum mTarget;
        GLenum mUsage;
        GLuint mBO;
    };
}

#endif

