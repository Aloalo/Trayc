
#include <Engine/Engine/BackBufferRenderPass.h>
#include <Engine/Engine/Renderer.h>
#include <Engine/Engine/DebugDraw.h>
#include <Engine/Utils/Setting.h>
#include <Engine/Engine/AssetLoader.h>

using namespace glm;
using namespace std;

namespace engine
{
    BackBufferRenderPass::BackBufferRenderPass(void)
        : RenderPass("bbPass", GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT), mFinalTex(nullptr)
    {
    }

    void BackBufferRenderPass::Init()
    {
        const auto& renderPasses = mRenderer->GetRenderPasses();
        // BackBufferRenderPass is actually the last pass 
        const RenderPass* lastPass = renderPasses[renderPasses.size() - 2];
        mFinalTex = &lastPass->GetDstBuffer().GetAttachment(0);

        CompileShaders();
    }

    void BackBufferRenderPass::Destroy()
    {
        mDraw.Destroy();
    }

    void BackBufferRenderPass::CompileShaders()
    {
        mDraw.Init(AssetLoader::Get().ShaderPath("C_TexToScreen").data(), vector<string>(1, "TONEMAPPING"));
        mDraw.Prog().Use();
        mDraw.Prog().SetUniform("tex", TextureType::FINAL_SLOT);
    }

    void BackBufferRenderPass::Render(const RenderingContext &rContext) const
    {
        mDraw.Prog().Use();
        mDraw.Prog().SetUniform("exposure", Setting<float>("exposure"));
        glEnable(GL_FRAMEBUFFER_SRGB);
        mDraw.Draw();
        glDisable(GL_FRAMEBUFFER_SRGB);
    }

}
