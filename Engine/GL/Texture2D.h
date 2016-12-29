#ifndef EN_TEXTURE2D_H
#define EN_TEXTURE2D_H

#include <Engine/GL/Texture.h>

namespace engine
{
    class Texture2D : public Texture
    {
        using uint = unsigned int;
    public:
        Texture2D(void);
        Texture2D(const char *file, bool mipmaps, TextureType type);

        // From file
        void Init(const char *file, bool mipmaps, TextureType type);
        // Empty tex
        void Init(uint internalFormat, glm::ivec2 size, uint format, uint type);
        void Init(const TextureDescription &desc);

        virtual void Resize(glm::ivec2 size) override;
    };
}

#endif

