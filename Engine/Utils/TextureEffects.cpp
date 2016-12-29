
#include <Engine/Utils/TextureEffects.h>
#include <Engine/Engine/AssetLoader.h>

using namespace std;
using namespace glm;

namespace engine
{
   
    TextureEffects::TextureEffects(void)
    {
        mProgramDefinitions[BLUR_HORIZONTAL] = { "C_TexToScreen", "P_blur", "1", "HORIZONTAL" };
        mProgramDefinitions[BLUR_VERTICAL] = { "C_TexToScreen", "P_blur",  "1", "VERTICAL" };
    }

    const Texture2D& TextureEffects::GetTexture(const Texture2D &tex)
    {
        const auto it = mTextures.find(tex);

        if(it != mTextures.end()) {
            return *it;
        }

        Texture2D ret;
        ret.Init(tex.GetTextureDescription());
        const auto p = mTextures.insert(ret);
        return *p.first;
    }

    void TextureEffects::SetAsRenderTarget(const Texture2D &tex)
    {
        mFrameBuffer.Clear();
        mFrameBuffer.AddColorAttachment(tex);
        mFrameBuffer.Compile();

        mFrameBuffer.Bind();
        const ivec2 &size = tex.Size();
        glViewport(0, 0, size.x, size.y);
    }

    void TextureEffects::Init()
    {
        for(int i = 0; i < CT_PROGRAMS; ++i) {
            const ProgramType pt = ProgramType(i);
            const auto &def = mProgramDefinitions.at(pt);

            mCombiners[pt] = TextureCombiner();
            TextureCombiner &combiner = mCombiners[pt];
            combiner.Init(AssetLoader::Get().ShaderPath(def[0]), AssetLoader::Get().ShaderPath(def[1]), vector<string>(def.begin() + 3, def.end()));
            const Program &p = combiner.Prog();
            p.Use();
            const int ctTextures = stoi(def[2]);
            string sampler = "inTex0";
            for(int j = 0; j < ctTextures; ++j) {
                sampler.back()++;
                p.SetUniform(sampler, TextureType::E_EFFECT1 + j);
            }
        }

        Program::Unbind();

        mFrameBuffer.Init();
    }

    void TextureEffects::Destroy()
    {
        for(auto &c : mCombiners) {
            c.second.Destroy();
        }

        for(auto t: mTextures) {
            t.Destroy();
        }

        mFrameBuffer.Destroy();
    }

    void TextureEffects::Blur(const Texture2D &tex)
    {
        const TextureCombiner &blurH = mCombiners[BLUR_HORIZONTAL];
        const TextureCombiner &blurV = mCombiners[BLUR_VERTICAL];

        const Texture2D &temp = GetTexture(tex);

        glDisable(GL_DEPTH_TEST);

        SetAsRenderTarget(temp);
        blurH.SetTexture(E_EFFECT1, tex);
        blurH.Draw();

        SetAsRenderTarget(tex);
        blurV.SetTexture(E_EFFECT1, temp);
        blurV.Draw();

        glEnable(GL_DEPTH_TEST);
    }
}
