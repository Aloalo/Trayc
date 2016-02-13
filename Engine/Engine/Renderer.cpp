/*
* Copyright (c) 2014 Jure Ratkovic
*/
#include <Engine/Engine/Renderer.h>
#include <Engine/Engine/GeometryRenderPass.h>
#include <Engine/Engine/LightRenderPass.h>

#include <Engine/Utils/StlExtensions.hpp>

#include <Engine/Core/Defines.h>
#include <Engine/Core/CameraHandler.h>

#if PRODUCTION
#include <Engine/Engine/DebugDraw.h>
#endif

#include <easylogging++.h>

using namespace glm;
using namespace std;
using namespace stdext;

namespace engine
{
    Renderer::Renderer(Game *scene)
        : mCamera(nullptr)
    {
    }

    Renderer::~Renderer(void)
    {
        for(RenderPass *rPass : mRenderPasses)
        {
            rPass->Destroy();
            delete rPass;
        }

        TextureCombiner::DestroyVAO();

#if PRODUCTION
        DebugDraw::Get().Destroy();
#endif
    }

    void Renderer::SetScene(const Scene *scene)
    {
        GeometryRenderPass *gPass = static_cast<GeometryRenderPass*>(GetRenderPass("gPass"));
        gPass->InitScene(scene);
    }

    void Renderer::AddRenderable(Renderable *renderable)
    {
        mRenderables.push_back(renderable);
    }

    void Renderer::RemoveRenderable(Renderable *renderable)
    {
        erase(mRenderables, renderable);
    }

    void Renderer::AddLight(const Light *light)
    {
        static_cast<LightRenderPass*>(GetRenderPass("lPass"))->AddLight(light);
    }

    void Renderer::RemoveLight(const Light *light)
    {
        static_cast<LightRenderPass*>(GetRenderPass("lPass"))->RemoveLight(light);
    }

    void Renderer::SetScreenSize(int width, int height)
    {
        for(RenderPass *rPass : mRenderPasses) {
            rPass->ResizeDstBuffer(width, height);
        }

        glViewport(0, 0, width, height);
    }

    const RenderPass* Renderer::GetRenderPass(const string &name) const
    {
        return *find_if(mRenderPasses.begin(), mRenderPasses.end(), [&](const RenderPass *rPass)
        {
            return rPass->GetName() == name;
        });
    }

    RenderPass* Renderer::GetRenderPass(const string &name)
    {
        return *find_if(mRenderPasses.begin(), mRenderPasses.end(), [&](const RenderPass *rPass)
        {
            return rPass->GetName() == name;
        });
    }

    void Renderer::InitRendering(const CameraHandler *camera)
    {
        if(mCamera) {
            return;
        }

        mCamera = camera;

        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

        // Init render passes
        mRenderPasses.push_back(new GeometryRenderPass());
        mRenderPasses.push_back(new LightRenderPass());

        for(RenderPass *rPass : mRenderPasses)
            rPass->Init();
    }

    void Renderer::Render() const
    {
        RenderingContext rContext;
        rContext.mV = mCamera->GetViewMatrix();
        rContext.mP = mCamera->GetProjectionMatrix();
        rContext.mVP = rContext.mP * rContext.mV;
        rContext.mCamera = &mCamera->mCamera;

        // -------------------------- Deferred render -------------------------- //

        for(const RenderPass *rPass : mRenderPasses)
        {
            rPass->BeginRender();
            rPass->Render(mRenderPasses, rContext);
        }

        // -------------------------- Custom forward render -------------------------- //
        FrameBuffer::UnBind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Temporary
        /*const RenderPass *gPass = GetRenderPass("gPass");
        const Texture2D &tex = gPass->GetDstBuffer().GetAttachment(3);
        DebugDraw::Get().DrawTexture(tex);*/

        const RenderPass *gPass = GetRenderPass("lPass");
        const Texture2D &tex = gPass->GetDstBuffer().GetAttachment(0);
        DebugDraw::Get().DrawTexture(tex);

        for(Renderable *renderable : mRenderables) {
            if(renderable->mIsActive) {
                renderable->Draw(rContext);
            }
        }
    }
}
