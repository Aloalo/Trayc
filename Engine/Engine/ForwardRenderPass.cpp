/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Engine/ForwardRenderPass.h>
#include <Engine/Engine/GeometryRenderPass.h>
#include <Engine/Engine/LightRenderPass.h>
#include <Engine/Engine/Renderer.h>
#include <Engine/Geometry/Scene.h>
#include <Engine/Engine/AssetLoader.h>

using namespace glm;
using namespace std;

namespace engine
{
    ForwardRenderPass::ForwardRenderPass(void)
        : RenderPass("forwardPass", GL_COLOR_BUFFER_BIT, false)
    {
    }

    void ForwardRenderPass::Init()
    {
        mForwardProg.Init(AssetLoader::Get().ShaderPath("F_ForwardUnlit").data());
    }

    void ForwardRenderPass::Destroy()
    {
        mForwardProg.Destroy();
    }

    void ForwardRenderPass::BeginRender() const
    {
        const LightRenderPass *lPass = static_cast<const LightRenderPass*>(mRenderer->GetRenderPass("lPass"));
        const FrameBuffer &lFB = lPass->GetDstBuffer();

        lFB.Bind();
        glViewport(0, 0, lFB.Width(), lFB.Height());
    }

    void ForwardRenderPass::Render(const RenderingContext &rContext) const
    {
        const GeometryRenderPass *gPass = static_cast<const GeometryRenderPass*>(mRenderer->GetRenderPass("gPass"));
        const SceneGPUData *sceneData = gPass->GetGPUSceneData();
        const Scene *scene = sceneData->mScene;
        const auto objects = scene->GetObjects(rContext.mCamera, true);
        for(const Object3D *obj : objects)
        {
            const int meshIdx = obj->GetMeshIdx();
            const VertexArray &VA = sceneData->mVertexArrays[obj->GetMeshIdx()];
            const Material &mat = scene->mMaterials[obj->GetMaterialIdx()];

            mForwardProg.Use();
            mForwardProg.SetUniform("MVP", rContext.mVP * obj->GetTransform());
            mForwardProg.SetUniform("diffuseColor", vec4(mat.mKd, 1.0f));

            VA.Render(scene->mTriMeshes[meshIdx].GetDrawMode());
        }
    }

    const FrameBuffer& ForwardRenderPass::GetDstBuffer() const
    {
        const LightRenderPass *lPass = static_cast<const LightRenderPass*>(mRenderer->GetRenderPass("lPass"));
        return lPass->GetDstBuffer();
    }
}
