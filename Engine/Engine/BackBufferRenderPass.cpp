
#include <Engine/Engine/BackBufferRenderPass.h>
#include <Engine/Engine/Renderer.h>
#include <Engine/Engine/DebugDraw.h>
#include <Engine/Utils/Setting.h>
#include <Engine/Utils/TimerQuery.h>
#include <Engine/Engine/AssetLoader.h>

using namespace glm;
using namespace std;

namespace engine
{
    BackBufferRenderPass::BackBufferRenderPass(void)
        : RenderPass("bbPass", GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT), mFinalTex(nullptr)
    {
    }

    BackBufferRenderPass::~BackBufferRenderPass(void)
    {
        mNoiseTex.Destroy();
    }

    void BackBufferRenderPass::Init()
    {
        const auto& renderPasses = mRenderer->GetRenderPasses();
        // BackBufferRenderPass is actually the last pass 
        const RenderPass* lastPass = renderPasses[renderPasses.size() - 2];
        mFinalTex = &lastPass->GetDstBuffer().GetAttachment(0);

        mNoiseTex.Init(AssetLoader::Get().TexturePath("noise.png").c_str(), false, TextureType::D_NOISE);
        mNoiseTex.BindToSlot(TextureType::D_NOISE);

        CompileShaders();
    }

    void BackBufferRenderPass::Destroy()
    {
        mDraw.Destroy();
    }

    void BackBufferRenderPass::CompileShaders()
    {
        mDraw.Init(AssetLoader::Get().ShaderPath("C_TexToScreen").data(), vector<string>(1, "TONEMAPPING"));
        mDraw.Prog().SetUniform("tex", TextureType::FINAL_SLOT);
        mDraw.Prog().SetUniform("noiseTex", TextureType::D_NOISE);
    }

    void BackBufferRenderPass::Render(const RenderingContext &rContext) const
    {
        TimerQuery t("BackBufferRenderPass");

        mDraw.Prog().SetUniform("exposure", Setting<float>("exposure"));
        mDraw.Prog().SetUniform("noiseScale", vec2(mFinalTex->Size()) / vec2(mNoiseTex.Size()));
        glEnable(GL_FRAMEBUFFER_SRGB);
        mDraw.Draw();
        glDisable(GL_FRAMEBUFFER_SRGB);
    }

}
