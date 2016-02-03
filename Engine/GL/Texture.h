#ifndef EN_TEXTURE_H
#define EN_TEXTURE_H

#include <glm/glm.hpp>

namespace engine
{
    class Texture2D
    {
        using uint = unsigned int;
    public:
        Texture2D(void);
        Texture2D(const char *file);

        // From file
        void Init(const char *name);
        // Empty tex
        void Init(uint internalFormat, glm::ivec2 size, uint format, uint type);

        void Resize(glm::ivec2 size);

        void Bind() const;
        static void UnBind();
        void Delete();

        uint ID() const;
        glm::ivec2 Size() const;

    private:
        unsigned int mID;
        glm::ivec2 mSize;
        uint mInternalFormat;
        uint mFormat;
        uint mType;

        struct ImageLoader;

        void init(const ImageLoader &imgl, const char *name);

        struct ImageLoader
        {
            virtual bool load() const = 0;
        };

        struct FileImageLoader :
            public ImageLoader
        {
            const char *file;
            FileImageLoader(const char *file);
            bool load() const;
        };
    };
}

#endif

