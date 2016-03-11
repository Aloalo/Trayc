#ifndef EN_CUBEMAP_TEXTURE_H
#define EN_CUBEMAP_TEXTURE_H

#include <Engine/GL/Texture.h>

namespace engine
{
    class CubemapTexture : public Texture
    {
    public:
        CubemapTexture(void);
        virtual void Resize(glm::ivec2 size) override;
    };
}

#endif

