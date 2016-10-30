
#include <Engine/Engine/LightRenderPass.h>
#include <Engine/Engine/AssetLoader.h>
#include <Engine/Utils/StlExtensions.hpp>
#include <Engine/Engine/ShadowRenderPass.h>
#include <Engine/Engine/GeometryRenderPass.h>
#include <Engine/Engine/ForwardRenderPass.h>
#include <Engine/Engine/Renderer.h>
#include <Engine/Utils/Setting.h>
#include <Engine/Utils/Utilities.h>
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
        static const vector<string> USE_PBR(1, "USE_PBR");
        static const vector<string> LIGHT_PROG_DEFINES[Light::Type::CT_LIGHT_TYPES] =
        {
            vector<string>(1, "AMBIENT_LIGHT"),
            vector<string>(1, "DIRECTIONAL_LIGHT"),
            vector<string>(1, "GLOBAL_LIGHT"),
            vector<string>(1, "POINT_LIGHT"),
            vector<string>(1, "SPOT_LIGHT"),
        };

        const int width = Setting<int>("screenWidth");
        const int height = Setting<int>("screenHeight");

        // Init L buffer
        mDstFB.Init(width, height);
        mDstFB.AddAttachment(GL_RGBA16F, GL_RGBA, GL_FLOAT); //Lighting out / x
        const GeometryRenderPass *gPass = static_cast<const GeometryRenderPass*>(mRenderer->GetRenderPass("gPass"));
        const FrameBuffer &gFB = gPass->GetDstBuffer();
        mDstFB.AttachRBO(gFB.GetRBOID());

        mDstFB.Compile();

        const bool usePBR = mRenderer->UsePBR();
        // Init light programs
        for(int i = 0; i < Light::Type::CT_LIGHT_TYPES; ++i)
        {
            TextureCombiner &combiner = mLightCombiners[i];
            const auto defines = usePBR ? LIGHT_PROG_DEFINES[i] + USE_PBR : LIGHT_PROG_DEFINES[i];
            combiner.Init(AssetLoader::Get().ShaderPath("L_LightPass").c_str(), defines);

            const Program &prog = combiner.Prog();
            prog.Use();
            if(i != Light::AMBIENT) {
                prog.SetUniform("gDepth", TextureType::G_DEPTH_TEXTURE);
                prog.SetUniform("gNormal", TextureType::G_NORMAL_TEXTURE);
                prog.SetUniform("gSpecGloss", TextureType::G_SPEC_GLOSS_TEXTURE);

                const auto &viewRayDataUB = mRenderer->GetViewRayDataUB();
                prog.SetUniformBlockBinding(viewRayDataUB.GetName(), viewRayDataUB.GetBlockBinding());

                // TODO: fix for general shadows
                if(i != Light::POINT) {
                    prog.SetUniform("shadowMap", TextureType::S_SHADOWMAP);
                    prog.SetUniform("shadowBrightness", Setting<float>("shadowBrightness"));
                    const auto &matrices = mRenderer->GetMatricesUB();
                    prog.SetUniformBlockBinding(matrices.GetName(), matrices.GetBlockBinding());
                }
            }

            if(i == Light::GLOBAL_LIGHT && mRenderer->UsePBR()) {
                prog.SetUniform("reflectionMap", TextureType::SKYBOX_SLOT);
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

    void LightRenderPass::SetLights(const std::vector<Light*> &lights)
    {
        mLights = lights;
    }

    void LightRenderPass::Render(const RenderingContext &rContext) const
    {
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);

        glBlendFunc(GL_ONE, GL_ONE);

        const AABB sceneAABB = mRenderer->GetSceneAABB();

        for(const Light *light : mLights)
        {
            const Light::Type type = light->GetType();
            const TextureCombiner &combiner = mLightCombiners[type];
            const Program &prog = combiner.Prog();
            
            prog.Use();

            if(light->GetType() == Light::GLOBAL_LIGHT) {
                const ForwardRenderPass *fPass = static_cast<const ForwardRenderPass*>(mRenderer->GetRenderPass("forwardPass"));
                prog.SetUniform("cubemapM", fPass->GetSkyboxM());
            }

            // TODO: fix for general lights
            if(light->GetType() != Light::AMBIENT && light->GetType() != Light::POINT) {
                const Texture *shadowMap = light->GetShadowmap();
                shadowMap->BindToSlot(TextureType::S_SHADOWMAP);
                prog.SetUniform("shadowDepthBiasVP", light->GetDepthBiasVP(sceneAABB));
            }

            light->ApplyToProgram(&prog, rContext.mV);
            combiner.Draw();

            glBlendFunc(GL_ONE, GL_ONE);
        }

        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
    }
}
