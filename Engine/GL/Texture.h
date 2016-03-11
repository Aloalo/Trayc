#ifndef EN_TEXTURE_H
#define EN_TEXTURE_H

#include <glm/glm.hpp>
#include <Engine/Engine/GlobalRenderingParams.h>

namespace engine
{
    class Texture
    {
        using uint = unsigned int;
    public:
        Texture(uint target);

        // Empty tex
        void Init();

        virtual void Resize(glm::ivec2 size) = 0;

        void BindToSlot(int texSlot) const;
        static void UnBindFromSlot(int texSlot);
        void Destroy();

        glm::ivec2 Size() const;
        uint ID() const;
        uint Target() const;

    protected:
        glm::ivec2 mSize;
        uint mInternalFormat;
        uint mFormat;
        uint mType;

        void InitFromFile(uint target, const char *file, bool mipmaps);
        void InitEmpty(uint target, uint internalFormat, glm::ivec2 size, uint format, uint type);

    private:
        uint mID;
        uint mTarget;

        static uint mBoundTextures[TextureType::CT_TEX_SLOTS];
    };
}

#endif

