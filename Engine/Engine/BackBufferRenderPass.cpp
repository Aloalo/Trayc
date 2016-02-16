
#include <Engine/Engine/BackBufferRenderPass.h>
#include <Engine/Engine/AssetLoader.h>

using namespace std;

namespace engine
{
    BackBufferRenderPass::BackBufferRenderPass(void)
        : RenderPass("bbPass", GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    {
    }

    void BackBufferRenderPass::Init()
    {
        mTexCombiner.Init(AssetLoader::Get().ShaderPath("C_TexToScreen").c_str(), vector<string>());
        const Program &prog = mTexCombiner.Prog();
        prog.Use();
        prog.SetUniform("tex", TextureType::LIGHT_ACCUM_TEXTURE);
        Program::Unbind();
    }

    void BackBufferRenderPass::Destroy()
    {
        mTexCombiner.Destroy();
    }

    void BackBufferRenderPass::Render(const RenderingContext &rContext) const
    {
        mTexCombiner.Draw();
    }

}
