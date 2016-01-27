
#include <Engine/Engine/DebugDraw.h>
#include <Engine/Engine/AssetLoader.h>

using namespace glm;
using namespace std;

namespace engine
{
    void DebugDraw::DrawDepth(const Texture2D &tex, float nearRange, float farRange) const
    {
        TextureCombiner::SetTexture(0, tex);
        mDrawDepth.Prog().Use();
        mDrawDepth.Prog().SetUniform("tex", 0);
        mDrawDepth.Prog().SetUniform("nearRange", nearRange);
        mDrawDepth.Prog().SetUniform("farRange", farRange);
        mDrawDepth.Draw();
    }

    void DebugDraw::DrawAlbedo(const Texture2D &tex) const
    {
        TextureCombiner::SetTexture(0, tex);
        mDrawAlbedo.Prog().Use();
        mDrawAlbedo.Prog().SetUniform("tex", 0);
        mDrawAlbedo.Draw();
    }

    void DebugDraw::DrawNormal(const Texture2D &tex) const
    {
        TextureCombiner::SetTexture(0, tex);
        mDrawNormals.Prog().Use();
        mDrawNormals.Prog().SetUniform("tex", 0);
        mDrawNormals.Draw();
    }

    void DebugDraw::DrawSpecular(const Texture2D &tex) const
    {
        TextureCombiner::SetTexture(0, tex);
        mDrawSpecular.Prog().Use();
        mDrawSpecular.Prog().SetUniform("tex", 0);
        mDrawSpecular.Draw();
    }

    void DebugDraw::DrawGloss(const Texture2D &tex) const
    {
        TextureCombiner::SetTexture(0, tex);
        mDrawGloss.Prog().Use();
        mDrawGloss.Prog().SetUniform("tex", 0);
        mDrawGloss.Draw();
    }

    void DebugDraw::Destroy()
    {
        mDrawDepth.Destroy();
        mDrawAlbedo.Destroy();
        mDrawNormals.Destroy();
        mDrawSpecular.Destroy();
        mDrawGloss.Destroy();
    }

    DebugDraw::DebugDraw(void)
    {
        const string vsPath = AssetLoader::ShaderPath("C_TexToScreen").data();
        mDrawDepth.Init(vsPath.data(), AssetLoader::ShaderPath("C_DepthToScreen").data());
        mDrawAlbedo.Init(vsPath.data(), AssetLoader::ShaderPath("C_TexToScreen").data());
        mDrawNormals.Init(vsPath.data(), AssetLoader::ShaderPath("C_NormToScreen").data());
        mDrawSpecular.Init(vsPath.data(), AssetLoader::ShaderPath("C_SpecToScreen").data());
        mDrawGloss.Init(vsPath.data(), AssetLoader::ShaderPath("C_GlossToScreen").data());
    }
}
