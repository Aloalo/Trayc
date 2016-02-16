
#include <Engine/Engine/LightRenderPass.h>
#include <Engine/Engine/AssetLoader.h>
#include <Engine/Utils/StlExtensions.hpp>

using namespace glm;
using namespace std;
using namespace stdext;

namespace engine
{
    LightRenderPass::LightRenderPass(void)
        : RenderPass("lPass", GL_COLOR_BUFFER_BIT)
    {
    }

    void LightRenderPass::Init()
    {
        const string LIGHT_PROG_DEFINES[Light::Type::CT_LIGHT_TYPES] =
        {
            "DIRECTIONAL_LIGHT",
            "POINT_LIGHT",
            "SPOT_LIGHT",
            "AMBIENT_LIGHT"
        };

        const int width = 1280;
        const int height = 720;

        // Init L buffer
        mDstFB.Init(width, height);
        mDstFB.AddAttachment(GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE); //Lighting out / x
        mDstFB.Compile();

        // Init light programs
        const string vsPath = AssetLoader::Get().ShaderPath("L_LightPass");
        const vector<string> defines;

        for(int i = 0; i < Light::Type::CT_LIGHT_TYPES; ++i)
        {
            TextureCombiner &combiner = mLightCombiners[i];
            combiner.Init(AssetLoader::Get().ShaderPath("L_LightPass").c_str(), vector<string>(1, LIGHT_PROG_DEFINES[i]));

            const Program &prog = combiner.Prog();
            prog.Use();
            if(i != Light::AMBIENT) {
                prog.SetUniform("gDepth", TextureType::G_DEPTH_TEXTURE);
                prog.SetUniform("gNormal", TextureType::G_NORMAL_TEXTURE);
                prog.SetUniform("gSpecGloss", TextureType::G_SPEC_GLOSS_TEXTURE);
                prog.SetUniformBlockBinding("ViewRayData", 0);
            }
            prog.SetUniform("gAlbedo", TextureType::G_ALBEDO_TEXTURE);
            Program::Unbind();
        }

        // Bind own textures to appropriate slots
        mDstFB.GetAttachment(0).BindToSlot(TextureType::LIGHT_ACCUM_TEXTURE);
    }

    void LightRenderPass::Destroy()
    {
        mDstFB.Destroy();
        for(TextureCombiner &combiner : mLightCombiners)
            combiner.Destroy();
    }

    void LightRenderPass::SetLights(const std::vector<const Light*> &lights)
    {
        mLights = lights;
    }

    void LightRenderPass::Render(const RenderingContext &rContext) const
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);

        const float tanHalfFovy = 1.0f / rContext.mP[1][1];
        const float aspectTanHalfFovy = 1.0f / rContext.mP[0][0];

        for(const Light *light : mLights)
        {
            const Light::Type type = light->GetType();
            const TextureCombiner &combiner = mLightCombiners[type];
            const Program &prog = combiner.Prog();

            prog.Use();
            light->ApplyToProgram(&prog, rContext.mV);
            combiner.Draw();
        }

        glDisable(GL_BLEND);
    }
}
