/*
* Copyright (c) 2014 Jure Ratkovic
*/


#ifndef EN_UNIFORM_BUFFER_H
#define EN_UNIFORM_BUFFER_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#define UNIFORM_MACRO(tp,name,offset) \
    void (name(const tp &x)) \
        { \
        setSubData(offset, x); \
        }

namespace engine
{
    class UniformBuffer
    {
    public:
        UniformBuffer(void);
        virtual ~UniformBuffer(void);

        void Init();
        void Init(int size, void *data = nullptr);

        void Delete();

        void bind();
        void setData(int size, void *data, int usage = GL_DYNAMIC_DRAW);
        void setSubData(int offset, int size, void *data);
        template<class T>
        void setSubData(int offset, const T &data);
        void bindToPoint(int point) const;

    protected:
        unsigned int id;
    private:
        static UniformBuffer *boundBuffer;
    };

    template<class T>
    void UniformBuffer::setSubData(int offset, const T &data)
    {
        setSubData(offset, sizeof data, (void*)&data);
    }
}

#endif
