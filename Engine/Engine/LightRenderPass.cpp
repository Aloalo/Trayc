
#include <Engine/Engine/LightRenderPass.h>
#include <Engine/Engine/AssetLoader.h>
#include <Engine/Utils/StlExtensions.hpp>
#include <Engine/Engine/ShadowRenderPass.h>
#include <Engine/Engine/GeometryRenderPass.h>
#include <Engine/Engine/Renderer.h>
#include <Engine/Utils/Setting.h>
#include <glm/gtc/matrix_transform.hpp>

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
            "AMBIENT_LIGHT",
            "DIRECTIONAL_LIGHT",
            "GLOBAL_LIGHT",
            "POINT_LIGHT",
            "SPOT_LIGHT"
        };

        const int width = 1280;
        const int height = 720;

        // Init L buffer
        mDstFB.Init(width, height);
        mDstFB.AddAttachment(GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE); //Lighting out / x
                             // Bind geometry pass depth renderbuffer
        const GeometryRenderPass *gPass = static_cast<const GeometryRenderPass*>(mRenderer->GetRenderPass("gPass"));
        const FrameBuffer &gFB = gPass->GetDstBuffer();
        mDstFB.AttachRBO(gFB.GetRBOID());

        mDstFB.Compile();

        // Init light programs
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

                const auto &viewRayDataUB = mRenderer->GetViewRayDataUB();
                prog.SetUniformBlockBinding(viewRayDataUB.GetName(), viewRayDataUB.GetBlockBinding());
            }
            // TODO: fix for general shadows
            if(i == Light::GLOBAL_LIGHT) {
                prog.SetUniform("shadowMap", TextureType::S_SHADOWMAP);
                prog.SetUniform("shadowBrightness", Setting<float>("shadowBrightness"));
                prog.SetUniform("reflectionMap", TextureType::SKYBOX_SLOT);
                prog.SetUniform("cubemapM", rotate(mat4(1.0f), radians(180.0f), vec3(0.0f, 0.0f, 1.0f)));
                const auto &matrices = mRenderer->GetMatricesUB();
                prog.SetUniformBlockBinding(matrices.GetName(), matrices.GetBlockBinding());
            }
            prog.SetUniform("gAlbedo", TextureType::G_ALBEDO_TEXTURE);
            Program::Unbind();
        }

        // Bind own textures to appropriate slots
        mDstFB.GetAttachment(0).BindToSlot(TextureType::LIGHT_ACCUM_TEXTURE);
        mDstFB.GetAttachment(0).BindToSlot(TextureType::FINAL_SLOT);
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
        const ShadowRenderPass *shadowPass = static_cast<const ShadowRenderPass*>(mRenderer->GetRenderPass("shadowPass"));
        const mat4 invView = inverse(rContext.mV);

        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);

        glBlendFunc(GL_ONE, GL_ONE);

        for(const Light *light : mLights)
        {
            const Light::Type type = light->GetType();
            const TextureCombiner &combiner = mLightCombiners[type];
            const Program &prog = combiner.Prog();
            const Texture *shadowMap = shadowPass->GetShadowmap(light);
            
            prog.Use();

            switch(light->GetType())
            {
            case Light::GLOBAL_LIGHT:
            {
                shadowMap->BindToSlot(TextureType::S_SHADOWMAP);
                const GlobalLight *gl = static_cast<const GlobalLight*>(light);
                prog.SetUniform("shadowDepthBiasVP", shadowPass->GetDepthBiasVP(-gl->GetDirection()));
                break;
            }
            default:
                break;
            }

            light->ApplyToProgram(&prog, rContext.mV);
            combiner.Draw();

            glBlendFunc(GL_ONE, GL_ONE);
        }

        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
    }
}
