/*
* Copyright (c) 2014 Jure Ratkovic
*/
#include <Engine/Engine/Renderer.h>
#include <Engine/Engine/GeometryRenderPass.h>
#include <Engine/Engine/LightRenderPass.h>
#include <Engine/Engine/BackBufferRenderPass.h>

#include <Engine/Utils/StlExtensions.hpp>

#include <Engine/Core/Defines.h>
#include <Engine/Core/CameraHandler.h>

#include <Engine/Geometry/Scene.h>

#if PRODUCTION
#include <Engine/Engine/DebugDraw.h>
#endif

#include <easylogging++.h>

using namespace glm;
using namespace std;
using namespace stdext;

namespace engine
{
    Renderer::Renderer()
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
        mLinearMipMapSampler.Destroy();
        mLinearSampler.Destroy();
        mViewRayDataUB.Destroy();

#if PRODUCTION
        DebugDraw::Get().Destroy();
#endif
    }

    void Renderer::SetScene(const Scene *scene)
    {
        GeometryRenderPass *gPass = static_cast<GeometryRenderPass*>(GetRenderPass("gPass"));
        gPass->InitScene(scene);

        LightRenderPass *lPass = static_cast<LightRenderPass*>(GetRenderPass("lPass"));
        lPass->SetLights(scene->mLights);
    }

    void Renderer::AddRenderable(Renderable *renderable)
    {
        mRenderables.push_back(renderable);
    }

    void Renderer::RemoveRenderable(Renderable *renderable)
    {
        erase(mRenderables, renderable);
    }

    void Renderer::SetScreenSize(int width, int height)
    {
        const float aspect = float(width) / float(height);
        mViewRayDataUB.aspectTanHalfFovy(aspect * tanf(radians(mCamera->GetCamera().mFoV) * 0.5f));

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
        mRenderPasses.push_back(new BackBufferRenderPass());

        for(RenderPass *rPass : mRenderPasses)
            rPass->Init();

        // Init Texture Samplers
        mLinearSampler.InitForDataTexture();
        mLinearSampler.BindToSlot(TextureType::G_DEPTH_TEXTURE);
        mLinearSampler.BindToSlot(TextureType::G_NORMAL_TEXTURE);
        mLinearSampler.BindToSlot(TextureType::G_ALBEDO_TEXTURE);
        mLinearSampler.BindToSlot(TextureType::G_SPEC_GLOSS_TEXTURE);
        mLinearSampler.BindToSlot(TextureType::LIGHT_ACCUM_TEXTURE);

        mLinearMipMapSampler.InitForDiffuse();
        mLinearMipMapSampler.BindToSlot(TextureType::DIFFUSE_MAP);
        mLinearMipMapSampler.BindToSlot(TextureType::NORMAL_MAP);
        mLinearMipMapSampler.BindToSlot(TextureType::SPECULAR_MAP);
        mLinearMipMapSampler.BindToSlot(TextureType::HEIGHT_MAP);

        const Camera &cam = mCamera->GetCamera();
        mViewRayDataUB.Init(8);
        const float tanHalfFovy = tanf(radians(cam.mFoV) * 0.5f);
        const float aspectTanHalfFovy = cam.mAspectRatio * tanHalfFovy;
        mViewRayDataUB.tanHalfFovy(tanHalfFovy);
        mViewRayDataUB.aspectTanHalfFovy(aspectTanHalfFovy);
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
            rPass->Render(rContext);
        }

        // -------------------------- Custom forward render -------------------------- //

        for(Renderable *renderable : mRenderables) {
            if(renderable->mIsActive) {
                renderable->Draw(rContext);
            }
        }
    }
}
