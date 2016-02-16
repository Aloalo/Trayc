
#ifndef EN_UNIFORM_BUFFER_H
#define EN_UNIFORM_BUFFER_H

#include <glm/glm.hpp>

#define UNIFORM_MACRO(tp,name,offset) \
    void (name(const tp &x) const) \
        { \
        SetSubData(offset, x); \
        }

namespace engine
{
    class UniformBuffer
    {
    public:
        UniformBuffer(unsigned int usage);
        virtual ~UniformBuffer(void);

        void Init();
        void Init(int size, const void *data = nullptr);

        void Destroy();

        void Bind() const;
        void SetData(int size, const void *data) const;
        void SetSubData(int offset, int size, const void *data) const;
        template<class T>
        void SetSubData(int offset, const T &data) const;

        unsigned int GetBlockBinding() const;

    protected:
        unsigned int mID;

    private:
        unsigned int mUsage;
        unsigned int mBlockBinding;

        static unsigned int mBoundBuffer;
        static unsigned int mCurrentBlockBinding;
    };

    template<class T>
    void UniformBuffer::SetSubData(int offset, const T &data) const
    {
        SetSubData(offset, sizeof data, (void*)&data);
    }
}

#endif
