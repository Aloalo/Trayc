
#include <Engine/Engine/Renderer.h>
#include <Engine/Engine/ShadowProjectionRenderPass.h>
#include <Engine/Engine/GeometryRenderPass.h>
#include <Engine/Geometry/Scene.h>
#include <Engine/Engine/AssetLoader.h>
#include <Engine/Utils/Setting.h>
#include <Engine/Utils/TextureEffects.h>
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
    }

    void ShadowProjectionRenderPass::Destroy()
    {
        mDstFB.Destroy();
        for(TextureCombiner &c : mShadowProjectionCombiners) {
            c.Destroy();
            c = TextureCombiner();
        }

        for(FrameBuffer &fb : mProjectedShadowFBs) {
            fb.Destroy();
        }

        mProjectedShadowFBs.clear();
    }

    void ShadowProjectionRenderPass::Render(const RenderingContext &rContext) const
    {
        const Scene *scene = mSceneData->mScene;
        const AABB sceneAABB = scene->GetShadowCastersAABB();

        // Project shadowmaps to shadow projection buffer
        const int ctLights = scene->mLights.size();
        glDisable(GL_DEPTH_TEST);
        for(int i = 0; i < ctLights; ++i)
        {
            const Light *light = scene->mLights[i];
            const Light::Type type = light->GetType();

            if(type != Light::AMBIENT) {
                const FrameBuffer &fb = mProjectedShadowFBs[i];
                fb.Bind();
                glViewport(0, 0, fb.Width(), fb.Height());
                glClear(mClearMask);

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

        // Blur shadowmaps
        for(int i = 0; i < ctLights; ++i)
        {
            const Light::Type type = scene->mLights[i]->GetType();

            if(type != Light::AMBIENT) {
                const FrameBuffer &fb = mProjectedShadowFBs[i];
                TextureEffects::Get().Blur(fb.GetAttachment(0));
            }
        }
    }
    
    const Texture2D& ShadowProjectionRenderPass::GetProjectedShadowmap(int idx) const
    {
        return mProjectedShadowFBs[idx].GetAttachment(0);
    }

    void ShadowProjectionRenderPass::Init(const SceneGPUData *sceneData)
    {
        mSceneData = sceneData;

        // Init shadow projection buffers
        const int screenWidth = Setting<int>("screenWidth");
        const int screenHeight = Setting<int>("screenHeight");
        auto &lights = mSceneData->mScene->mLights;

        // Init Shadow FrameBuffers
        for(Light *l : lights) {
            mProjectedShadowFBs.push_back(FrameBuffer());

            if(l->GetType() == Light::AMBIENT) {
                continue;
            }

            FrameBuffer &fb = mProjectedShadowFBs.back();
            fb.Init(screenWidth, screenHeight);
            fb.AddAttachment(GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);
            fb.Compile();
            l->mProjectedShadow = &fb.GetAttachment(0);
        }
    }
}
