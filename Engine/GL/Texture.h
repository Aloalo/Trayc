#ifndef EN_TEXTURE_H
#define EN_TEXTURE_H

#include <glm/glm.hpp>
#include <Engine/Engine/GlobalRenderingParams.h>

namespace engine
{
    struct TextureDescription
    {
        using uint = unsigned int;
    public:
        TextureDescription(const glm::ivec2 &size, uint internalFormat, uint format, uint type);

        bool operator<(const TextureDescription &other);

        glm::ivec2 size;
        uint internalFormat;
        uint format;
        uint type;
    };

    class Texture
    {
        using uint = unsigned int;
    public:
        Texture(uint target);

        // Generate texture
        void Init();

        virtual void Resize(glm::ivec2 size) = 0;

        void BindToSlot(int texSlot) const;
        static void UnBindFromSlot(int texSlot);
        void Destroy();

        glm::ivec2 Size() const;
        uint ID() const;
        uint Target() const;

        TextureDescription GetTextureDescription() const;

        void TextureParam(uint pname, uint param) const;


    protected:
        glm::ivec2 mSize;
        uint mInternalFormat;
        uint mFormat;
        uint mType;

        // Initialises texture params
        void InitFromFile(uint target, const char *file, bool mipmaps, TextureType type);
        // Doesn't initialise texture params
        void LoadFromFile(uint target, const char *file, TextureType type);
        void InitEmpty(uint target, uint internalFormat, glm::ivec2 size, uint format, uint type);

        void GenerateMipmaps() const;

    private:
        uint mID;
        uint mTarget;

        static uint mBoundTextures[TextureType::CT_TEX_SLOTS];
    };
}

#endif

