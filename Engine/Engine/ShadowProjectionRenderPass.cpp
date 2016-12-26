
#include <Engine/Engine/Renderer.h>
#include <Engine/Engine/ShadowProjectionRenderPass.h>
#include <Engine/Engine/GeometryRenderPass.h>
#include <Engine/Geometry/Scene.h>
#include <Engine/Engine/AssetLoader.h>
#include <Engine/Utils/Setting.h>
#include <glm/gtc/matrix_transform.hpp>
#include <array>
#include <easylogging++.h>

using namespace glm;
using namespace std;

namespace engine
{

    // ShadowRenderPass //
    ShadowProjectionRenderPass::ShadowProjectionRenderPass(void)
        : RenderPass("shadowProjectionPass", GL_COLOR_BUFFER_BIT, vec4(1.0f), true), mSceneData(nullptr)
    {
    }

    void ShadowProjectionRenderPass::Init()
    {
        // Init programs
        static const vector<string> LIGHT_PROG_DEFINES[Light::Type::CT_LIGHT_TYPES] =
        {
            vector<string>(1, "AMBIENT_LIGHT"),
            vector<string>(1, "DIRECTIONAL_LIGHT"),
            vector<string>(1, "GLOBAL_LIGHT"),
            vector<string>(1, "POINT_LIGHT"),
            vector<string>(1, "SPOT_LIGHT"),
        };

        for(int i = 0; i < Light::CT_LIGHT_TYPES; ++i) {
            if(i != Light::AMBIENT) {
                TextureCombiner &combiner = mShadowProjectionCombiners[i];
                combiner.Init(AssetLoader::Get().ShaderPath("S_shadowProjection").data(), LIGHT_PROG_DEFINES[i]);
                const Program &prog = combiner.Prog();
                prog.Use();
                prog.SetUniform("shadowBrightness", Setting<float>("shadowBrightness"));
                prog.SetUniform("gDepth", TextureType::G_DEPTH_TEXTURE);
                prog.SetUniform("gNormal", TextureType::G_NORMAL_TEXTURE);
                prog.SetUniform("shadowMap", TextureType::S_SHADOWMAP);

                const auto &matrices = mRenderer->GetMatricesUB();
                prog.SetUniformBlockBinding(matrices.GetName(), matrices.GetBlockBinding());
                const auto &viewData = mRenderer->GetViewRayDataUB();
                prog.SetUniformBlockBinding(viewData.GetName(), viewData.GetBlockBinding());
            }
        }
        Program::Unbind();

        // Init shadow projection buffer
        const int screenWidth = Setting<int>("screenWidth");
        const int screenHeight = Setting<int>("screenHeight");
        mDstFB.Init(screenWidth, screenHeight);
        mDstFB.AddAttachment(GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);
        mDstFB.Compile();

        mDstFB.GetAttachment(0).BindToSlot(TextureType::S_SHADOWPROJECTION);
    }

    void ShadowProjectionRenderPass::Destroy()
    {
        mDstFB.Destroy();
        for(TextureCombiner &c : mShadowProjectionCombiners) {
            c.Destroy();
        }
    }

    void ShadowProjectionRenderPass::Render(const RenderingContext &rContext) const
    {
        const Scene *scene = mSceneData->mScene;
        const AABB sceneAABB = scene->GetAABB();

        // Project shadowmaps to shadow projection buffer
        glDisable(GL_DEPTH_TEST);
        for(const Light *light : scene->mLights)
        {
            const Light::Type type = light->GetType();

            // TODO: fix for general lights
            if(type != Light::AMBIENT && type != Light::POINT) {
                const TextureCombiner &combiner = mShadowProjectionCombiners[type];
                const Program &p = combiner.Prog();
                p.Use();
                p.SetUniform("shadowDepthBiasVP", light->GetDepthBiasVP(sceneAABB));

                const Texture *shadowMap = light->GetShadowmap();
                shadowMap->BindToSlot(TextureType::S_SHADOWMAP);

                light->ApplyToShadowProgram(&p, rContext.mV);
                combiner.Draw();
            }
        }
        glEnable(GL_DEPTH_TEST);
    }

    void ShadowProjectionRenderPass::Init(const SceneGPUData *sceneData)
    {
        mSceneData = sceneData;
    }
}
