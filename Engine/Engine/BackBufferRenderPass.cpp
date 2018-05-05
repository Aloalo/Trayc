
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
        mLumasRGBFB.Destroy();
    }

    void BackBufferRenderPass::Init()
    {
        const int width = Setting<int>("screenWidth");
        const int height = Setting<int>("screenHeight");
        mDstFB.Resize(width, height);

        const auto& renderPasses = mRenderer->GetRenderPasses();
        // BackBufferRenderPass is actually the last pass 
        const RenderPass* lastPass = renderPasses[renderPasses.size() - 2];
        mFinalTex = &lastPass->GetDstBuffer().GetAttachment(0);

        mNoiseTex.Init(AssetLoader::Get().TexturePath("noise.png").c_str(), false, TextureType::D_NOISE);
        mNoiseTex.BindToSlot(TextureType::D_NOISE);

        SetFxaa(Setting<bool>("fxaa"));
    }

    void BackBufferRenderPass::Destroy()
    {
        mDraw.Destroy();
        mFxaaCombiner.Destroy();
    }

    void BackBufferRenderPass::SetFxaa(bool flag)
    {
        assert(mFxaa != flag);

        const int width = mDstFB.Width();
        const int height = mDstFB.Height();

        mFxaa = flag;
        if (mFxaa) {
            mLumasRGBFB.Init(width, height);
            mLumasRGBFB.AddAttachment(GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);
            mLumasRGBFB.Compile();
            mLumasRGBFB.GetAttachment(0).BindToSlot(TextureType::E_EFFECT3);
        }
        else {
            mLumasRGBFB.Destroy();
            mFxaaCombiner.Destroy();
        }

        CompileShaders();
    }

    void BackBufferRenderPass::ToggleFxaa()
    {
        SetFxaa(!mFxaa);
    }

    void BackBufferRenderPass::CompileShaders()
    {
        const auto defines = (mFxaa ? Shader::Defines{ "TONEMAPPING", "DITHERING", "GAMMA_LUMA" } : Shader::Defines{ "TONEMAPPING", "DITHERING" });
        mDraw.Init(AssetLoader::Get().ShaderPath("C_TexToScreen").data(), defines);
        mDraw.Prog().SetUniform("tex", TextureType::FINAL_SLOT);
        mDraw.Prog().SetUniform("noiseTex", TextureType::D_NOISE);

        if (mFxaa) {
            mDraw.Prog().SetUniform("gamma", vec3(1.0f / Setting<float>("gamma")));

            mFxaaCombiner.Init(AssetLoader::Get().ShaderPath("C_TexToScreen").data(), AssetLoader::Get().ShaderPath("P_FXAA").data(), Shader::Defines());
            mFxaaCombiner.Prog().SetUniform("tex", TextureType::E_EFFECT3);
        }
    }

    void BackBufferRenderPass::Render(const RenderingContext &rContext) const
    {
        TimerQuery t("BackBufferRenderPass");
        mDraw.Prog().SetUniform("exposure", Setting<float>("exposure"));
        mDraw.Prog().SetUniform("noiseScale", vec2(mFinalTex->Size()) / vec2(mNoiseTex.Size()));

        glEnable(GL_FRAMEBUFFER_SRGB);
        mDraw.Draw();
        glDisable(GL_FRAMEBUFFER_SRGB);

        if (mFxaa) {
            const int w = mDstFB.Width();
            const int h = mDstFB.Height();

            mDstFB.BindRead();
            mLumasRGBFB.BindDraw();
            glBlitFramebuffer(0, 0, w, h, 0, 0, mLumasRGBFB.Width(), mLumasRGBFB.Height(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
            FrameBuffer::UnBind();

            mFxaaCombiner.Prog().SetUniform("invScreenSize", 1.0f / vec2(static_cast<float>(w), static_cast<float>(h)));
            mDstFB.Bind();
            glViewport(0, 0, w, h);
            mFxaaCombiner.Draw();
        }
    }

    void BackBufferRenderPass::ResizeDstBuffer(int width, int height)
    {
        if (mFxaa) {
            mLumasRGBFB.Resize(width, height);
        }
        mDstFB.Resize(width, height);
    }

}
