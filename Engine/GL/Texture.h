#ifndef EN_TEXTURE_H
#define EN_TEXTURE_H

#include <glm/glm.hpp>
#include <Engine/Engine/GlobalRenderingParams.h>

namespace engine
{
    class Texture2D
    {
        using uint = unsigned int;
    public:
        Texture2D(void);
        Texture2D(const char *file, bool mipmaps);

        // From file
        void Init(const char *file, bool mipmaps);
        // Empty tex
        void Init(uint internalFormat, glm::ivec2 size, uint format, uint type);

        void Resize(glm::ivec2 size);

        void BindToSlot(int texSlot) const;
        static void UnBindFromSlot(int texSlot);
        void Destroy();

        uint ID() const;
        glm::ivec2 Size() const;

    private:
        unsigned int mID;
        glm::ivec2 mSize;
        uint mInternalFormat;
        uint mFormat;
        uint mType;

        struct ImageLoader;

        void init(const ImageLoader &imgl, const char *file, bool mipmaps);

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

        static unsigned int mBoundTextures[TextureType::CT_TEX_SLOTS];
    };
}

#endif

