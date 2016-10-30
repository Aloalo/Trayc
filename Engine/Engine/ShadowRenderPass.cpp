
/*
* Copyright (c) 2014 Jure Ratkovic
*/

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
    // MATRIX GETTERS //
    static inline mat4 CalcDirectionalShadowP(const AABB &sceneAABB, const mat4 &V)
    {
        vec3 minimum = sceneAABB.mMinv;
        vec3 maximum = sceneAABB.mMaxv;
        const float width = maximum.x - minimum.x;
        const float depth = maximum.z - minimum.z;

        array<vec3, 8> vert;
        vert[0] = minimum;
        vert[1] = vec3(minimum.x + width, minimum.y, minimum.z);
        vert[2] = vec3(minimum.x + width, minimum.y, minimum.z + depth);
        vert[3] = vec3(minimum.x, minimum.y, minimum.z + depth);
        vert[4] = vec3(maximum.x - width, maximum.y, maximum.z - depth);
        vert[5] = vec3(maximum.x, maximum.y, maximum.z - depth);
        vert[6] = maximum;
        vert[7] = vec3(maximum.x - width, maximum.y, maximum.z);

        AABB orthographicAABB;
        for(const vec3 &v : vert) {
            orthographicAABB |= vec3((V * vec4(v, 1.0f)));
        }

        minimum = orthographicAABB.mMinv;
        maximum = orthographicAABB.mMaxv;
        return ortho(minimum.x, maximum.x, minimum.y, maximum.y, minimum.z, maximum.z);
    }

    static inline mat4 CalcDirectionalShadowV(const AABB &sceneAABB, const vec3 &viewDir)
    {
        static const vec3 up = vec3(0.0f, 1.0f, 0.0f);
        const vec3 pos = sceneAABB.Center();
        return lookAt(pos, pos + viewDir, up);
    }

    static inline mat4 CalcDirectionalShadowVP(const AABB &sceneAABB, const vec3 &viewDir)
    {
        const mat4 depthViewMatrix = CalcDirectionalShadowV(sceneAABB, viewDir);
        const mat4 depthProjectionMatrix = CalcDirectionalShadowP(sceneAABB, depthViewMatrix);
        return depthProjectionMatrix * depthViewMatrix;
    }



    // ShadowRenderPass //
    ShadowRenderPass::ShadowRenderPass(void)
        : RenderPass("shadowPass", GL_DEPTH_BUFFER_BIT), mSceneData(nullptr)
    {
    }

    void ShadowRenderPass::Init()
    {
        // Init program
        mProgram.Init(AssetLoader::Get().ShaderPath("S_shadow").data());
        Program::Unbind();
    }

    void ShadowRenderPass::Destroy()
    {
        for(FrameBuffer &fb : mShadowFBs) {
            fb.Destroy();
        }

        mDstFB.Destroy();
        mProgram.Destroy();
    }

    static inline mat4 CalcShadowVP(const Scene *scene, const Light *light)
    {
        switch(light->GetType())
        {
        case Light::GLOBAL_LIGHT:
        {
            const GlobalLight *l = static_cast<const GlobalLight*>(light);
            return CalcDirectionalShadowVP(scene->GetAABB(), -l->GetDirection());
        }
        case Light::DIRECTIONAL:
        {
            const DirectionalLight *l = static_cast<const DirectionalLight*>(light);
            return CalcDirectionalShadowVP(scene->GetAABB(), -l->GetDirection());
        }
        default:
            LOG(ERROR) << "[CalcShadowVP] Incorrect light type: " << light->GetType();
            return mat4(1.0f);
        }
    }

    mat4 ShadowRenderPass::GetDepthBiasVP(const Light *light) const
    {
        static const mat4 biasMatrix(
            0.5, 0.0, 0.0, 0.0,
            0.0, 0.5, 0.0, 0.0,
            0.0, 0.0, 0.5, 0.0,
            0.5, 0.5, 0.5, 1.0
        );

        return biasMatrix * CalcShadowVP(mSceneData->mScene, light);
    }

    void ShadowRenderPass::Render(const RenderingContext &rContext) const
    {
        const Scene *scene = mSceneData->mScene;
        mProgram.Use();
        
        int ctFBs = mShadowFBs.size();
        for(int i = 0; i < ctFBs; ++i)
        {
            const FrameBuffer &fb = mShadowFBs[i];

            fb.Bind();
            glViewport(0, 0, fb.Width(), fb.Height());
            if(mClearBuffer) {
                glClear(mClearMask);
            }

            const Light *l = scene->mLights[i];

            // Compute the MVP matrix from the light's point of view
            const mat4 depthVP = CalcShadowVP(scene, l);

            const auto objects = scene->GetShadowCasters();
            for(const Object3D *obj : objects)
            {
                const int meshIdx = obj->GetMeshIdx();
                const VertexArray &VA = mSceneData->mVertexArrays[obj->GetMeshIdx()];

                // Mesh uniforms
                mProgram.SetUniform("depthMVP", depthVP * obj->GetTransform());
                VA.Render(scene->mTriMeshes[meshIdx].GetDrawMode());
            }

            // TODO: fix for general light shadows
            break;
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
            FrameBuffer &fb = mShadowFBs.back();
            fb.Init(res, res);
            fb.AddAttachment(GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT, GL_FLOAT); //Depth
            fb.Compile();
            l->mShadowmap = &fb.GetAttachment(0);
        }
    }
}
