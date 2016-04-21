#ifndef EN_CUBEMAP_TEXTURE_H
#define EN_CUBEMAP_TEXTURE_H

#include <Engine/GL/Texture2D.h>

namespace engine
{
    class CubemapTexture : public Texture
    {
        using uint = unsigned int;
    public:
        CubemapTexture(void);
        virtual void Resize(glm::ivec2 size) override;

        // From file
        void Init(const std::string files[6]);

    private:
        static uint mCubeSides[6];
        Texture2D mSideTextures[6];
    };
}

#endif

