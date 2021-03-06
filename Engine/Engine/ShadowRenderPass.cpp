
#include <Engine/Engine/Renderer.h>
#include <Engine/Engine/ShadowRenderPass.h>
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
    ShadowRenderPass::ShadowRenderPass(void)
        : RenderPass("shadowPass", GL_DEPTH_BUFFER_BIT, vec4(0.0f), false), mSceneData(nullptr)
    {
    }

    void ShadowRenderPass::Init()
    {
        // Init program
        mShadowMappingProgram.Init(AssetLoader::Get().ShaderPath("S_shadow").data());
        Program::Unbind();
    }

    void ShadowRenderPass::Destroy()
    {
        for(FrameBuffer &fb : mShadowFBs) {
            fb.Destroy();
        }

        mShadowFBs.clear();
        mDstFB.Destroy();
        mShadowMappingProgram.Destroy();
    }

    void ShadowRenderPass::Render(const RenderingContext &rContext) const
    {
        const Scene *scene = mSceneData->mScene;
        const AABB sceneAABB = scene->GetShadowCastersAABB();
        mShadowMappingProgram.Use();
        
        // Render shadowmaps
        const int ctFBs = static_cast<int>(mShadowFBs.size());
        for(int i = 0; i < ctFBs; ++i)
        {
            const FrameBuffer &fb = mShadowFBs[i];
            fb.Bind();
            glViewport(0, 0, fb.Width(), fb.Height());
            glClear(mClearMask);

            // TODO: fix for point light shadows
            if(scene->mLights[i]->GetType() == Light::POINT) {
                continue;
            }

            const Light *l = scene->mLights[i];

            if(l->GetType() == Light::AMBIENT) {
                continue;
            }

            // Compute the MVP matrix from the light's point of view
            const mat4 depthVP = l->GetDepthVP(sceneAABB);

            const auto objects = scene->GetShadowCasters(l);
            for(const Object3D *obj : objects)
            {
                const int meshIdx = obj->GetMeshIdx();
                const VertexArray &VA = mSceneData->mVertexArrays[obj->GetMeshIdx()];

                // Mesh uniforms
                mShadowMappingProgram.SetUniform("depthMVP", depthVP * obj->GetTransform());
                VA.Render(scene->mTriMeshes[meshIdx].GetDrawMode());
            }
            //LOG(INFO) << "Lights[" << i << "] rendered " << objects.size() << " objects";
        }
    }

    void ShadowRenderPass::Init(const SceneGPUData *sceneData)
    {
        mSceneData = sceneData;

        const int res = Setting<int>("shadowResolution");
        auto &lights = mSceneData->mScene->mLights;

        // Init Shadow FrameBuffers
        for(Light *l : lights) {
            mShadowFBs.push_back(FrameBuffer());

            if(l->GetType() == Light::AMBIENT) {
                continue;
            }

            FrameBuffer &fb = mShadowFBs.back();
            fb.Init(res, res);
            fb.AddAttachment(GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT, GL_FLOAT); //Depth
            fb.Compile();
            l->mShadowmap = &fb.GetAttachment(0);
        }
    }

    const Texture2D& ShadowRenderPass::GetShadowmap(int idx) const
    {
        return mShadowFBs[idx].GetAttachment(0);
    }
}
