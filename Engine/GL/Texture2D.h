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
        Texture2D(const char *file, bool mipmaps);

        // From file
        void Init(const char *file, bool mipmaps);
        // Empty tex
        void Init(uint internalFormat, glm::ivec2 size, uint format, uint type);

        virtual void Resize(glm::ivec2 size) override;
    };
}

#endif

