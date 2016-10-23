
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
    ShadowRenderPass::ShadowRenderPass(void)
        : RenderPass("shadowPass", GL_DEPTH_BUFFER_BIT), mSceneData(nullptr)
    {
    }

    void ShadowRenderPass::Init()
    {
        const int res = Setting<int>("shadowResolution");

        // Init Depth buffer
        mDstFB.Init(res, res);
        mDstFB.AddAttachment(GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT, GL_FLOAT); //Depth
        mDstFB.Compile();

        // Init program
        mProgram.Init(AssetLoader::Get().ShaderPath("S_shadow").data());
        Program::Unbind();
    }

    void ShadowRenderPass::Destroy()
    {
        mDstFB.Destroy();
        mProgram.Destroy();
    }

    static inline mat4 CalcShadowProjection(const AABB &sceneAABB, const mat4 &V)
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

    static inline mat4 CalcShadowVP(const Scene *scene, const Camera *camera, const vec3 &lightInvDir)
    {
        const AABB sceneAABB = scene->GetAABB();
        const mat4 V = camera->GetViewMatrix();
        const mat4 depthProjectionMatrix = CalcShadowProjection(sceneAABB, V);
        const mat4 depthViewMatrix = lookAt(lightInvDir, vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));
        return depthProjectionMatrix * depthViewMatrix;
    }

    mat4 ShadowRenderPass::GetDepthBiasMV(const Camera *camera, const vec3 &lightInvDir) const
    {
        static const mat4 biasMatrix(
            0.5, 0.0, 0.0, 0.0,
            0.0, 0.5, 0.0, 0.0,
            0.0, 0.0, 0.5, 0.0,
            0.5, 0.5, 0.5, 1.0
        );

        return biasMatrix * CalcShadowVP(mSceneData->mScene, camera, lightInvDir);
    }

    void ShadowRenderPass::Render(const RenderingContext &rContext) const
    {
        const Scene *scene = mSceneData->mScene;

        // TODO: fix for general light shadows
        vec3 lightInvDir;
        for(const Light *l : scene->mLights) {
            if(l->GetType() == Light::GLOBAL_LIGHT) {
                lightInvDir = -dynamic_cast<const GlobalLight*>(l)->GetDirection();
                break;
            }
        }

        // Compute the MVP matrix from the light's point of view
        const mat4 depthVP = CalcShadowVP(scene, rContext.mCamera, lightInvDir);

        mProgram.Use();
        
        glCullFace(GL_FRONT);
        const auto objects = scene->GetShadowCasters();
        for(const Object3D *obj : objects)
        {
            const int meshIdx = obj->GetMeshIdx();
            const VertexArray &VA = mSceneData->mVertexArrays[obj->GetMeshIdx()];
            const Material &mat = scene->mMaterials[obj->GetMaterialIdx()];

            // Mesh uniforms
            mProgram.SetUniform("depthMVP", depthVP * obj->GetTransform());

            if(mat.mHasAlphaMask) {
                glDisable(GL_CULL_FACE);
            }
            VA.Render(scene->mTriMeshes[meshIdx].GetDrawMode());
            if(mat.mHasAlphaMask) {
                glEnable(GL_CULL_FACE);
            }
        }
        glCullFace(GL_BACK);
    }

    void ShadowRenderPass::Init(const SceneGPUData *sceneData)
    {
        mSceneData = sceneData;

        for(const Light *l : mSceneData->mScene->mLights) {
            // TODO: fix for general light shadows
            if(l->mType == Light::GLOBAL_LIGHT) {
                mLightToShadowmap[l] = &mDstFB.GetAttachment(0);
            }
            else {
                mLightToShadowmap[l] = nullptr;
            }
        }
    }
}
