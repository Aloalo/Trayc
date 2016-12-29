
#ifndef EN_TEXTURE_EFFECTS_H
#define EN_TEXTURE_EFFECTS_H

#include <Engine/Utils/Singleton.h>
#include <Engine/Engine/TextureCombiner.h>
#include <Engine/GL/Texture2D.h>
#include <Engine/GL/Program.h>
#include <set>
#include <map>

namespace engine
{
    struct TextureEffects : public Singleton<TextureEffects>
    {
        void Init();
        void Destroy();

        void Blur(const Texture2D &tex);

    private:
        friend engine::Singleton<TextureEffects>;
        TextureEffects(void);

        const Texture2D& GetTexture(const Texture2D &tex);
        void SetAsRenderTarget(const Texture2D &tex);

        struct TexCompare
        {
            int operator()(const Texture2D &l, const Texture2D &r) const
            {
                return l.GetTextureDescription() < r.GetTextureDescription();
            }
        };

        std::set<Texture2D, TexCompare> mTextures;

        enum ProgramType
        {
            BLUR_VERTICAL = 0,
            BLUR_HORIZONTAL,

            CT_PROGRAMS
        };

        FrameBuffer mFrameBuffer;

        // vertex name, fragment name, num textures, defines
        std::map<ProgramType, std::vector<std::string>> mProgramDefinitions;
        std::map<ProgramType, TextureCombiner> mCombiners;
    };
}

#endif
