/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Engine/LightRenderPass.h>
#include <Engine/Engine/AssetLoader.h>
#include <Engine/Utils/StlExtensions.hpp>

using namespace glm;
using namespace std;
using namespace stdext;

namespace engine
{
    LightRenderPass::LightRenderPass(void)
        : RenderPass("lPass")
    {
    }

    void LightRenderPass::Init()
    {
        const string vsPath = AssetLoader::Get().ShaderPath("L_LightPass");
        const vector<string> defines;

        for(int i = 0; i < LightType::CT_LIGHT_TYPES; ++i)
        {
            Program &prog = mLightPrograms[i];
            prog.Init("L_LightPass", vector<string>(1, LIGHT_PROG_DEFINES[i]));
            prog.Use();
            prog.SetUniform("gDepth", TextureType::G_DEPTH_TEXTURE);
            prog.SetUniform("gNormal", TextureType::G_NORMAL_TEXTURE);
            prog.SetUniform("gAlbedo", TextureType::G_ALBEDO_TEXTURE);
            prog.SetUniform("gSpecGloss", TextureType::G_SPEC_GLOSS_TEXTURE);
            Program::Unbind();
        }
    }

    void LightRenderPass::Destroy()
    {
        mDstFB.Destroy();
        for(Program &prog : mLightPrograms)
            prog.Delete();
    }

    void LightRenderPass::BeginRender() const
    {
        mDstFB.Bind();
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void LightRenderPass::EndRender() const
    {
        FrameBuffer::UnBind();
    }

    void LightRenderPass::AddLight(Light *light)
    {
        mLights.push_back(light);
    }

    void LightRenderPass::RemoveLight(Light *light)
    {
        erase(mLights, light);
    }

    void LightRenderPass::Render(const vector<RenderPass*> &renderPasses, const RenderingContext &rContext) const
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);

        for(const Light *light : mLights)
        {
            const LightType type = light->GetType();
            const Program &prog = mLightPrograms[type];

            prog.Use();
            light->ApplyToProgram(&prog, rContext.mV);
            //prog.SetUniform();
        }

        glDisable(GL_BLEND);
    }
}
