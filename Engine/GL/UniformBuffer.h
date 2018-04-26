
#ifndef EN_UNIFORM_BUFFER_H
#define EN_UNIFORM_BUFFER_H

#include <glm/glm.hpp>

#define UNIFORM_MACRO(tp,name,offset) \
    void (name(const tp &x) const) \
        { \
        SetSubData(offset, x); \
        }

#define UNIFORM_ARRAY_MACRO(tp,name) \
    void (name(const tp &x, size_t offset) const) \
        { \
        SetSubData(static_cast<int>(offset), static_cast<int>(sizeof(x[0]) * x.size()), x.data()); \
        }

namespace engine
{
    class UniformBuffer
    {
    public:
        UniformBuffer(unsigned int usage, const char *name);
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

        const char* GetName() const;

    protected:
        unsigned int mID;

    private:
        const char *mName;

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
