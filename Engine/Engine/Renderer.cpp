
#include <Engine/Engine/Renderer.h>
#include <Engine/Engine/GeometryRenderPass.h>
#include <Engine/Engine/LightRenderPass.h>
#include <Engine/Engine/ForwardRenderPass.h>
#include <Engine/Engine/BackBufferRenderPass.h>
#include <Engine/Engine/ShadowRenderPass.h>

#include <Engine/Utils/StlExtensions.hpp>
#include <Engine/Utils/Setting.h>

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
    Renderer::Renderer(void)
        : mCamera(nullptr), mUsePBR(true)
    {
    }

    Renderer::~Renderer(void)
    {
        for(RenderPass *rPass : mRenderPasses) {
            rPass->Destroy();
            delete rPass;
        }

        TextureCombiner::DestroyVAO();
        mLinearMipMapSampler.Destroy();
        mLinearSampler.Destroy();
        mShadowmapSampler.Destroy();
        mViewRayDataUB.Destroy();
        mMatricesUB.Destroy();

#if PRODUCTION
        DebugDraw::Get().Destroy();
#endif
    }

    void Renderer::SetUsePBR(bool usePBR)
    {
        mUsePBR = usePBR;
    }

    void Renderer::SetScene(Scene *scene)
    {
        GeometryRenderPass *gPass = static_cast<GeometryRenderPass*>(GetRenderPass("gPass"));
        gPass->Init(scene);

        ShadowRenderPass *shadowPass = static_cast<ShadowRenderPass*>(GetRenderPass("shadowPass"));
        shadowPass->Init(gPass->GetGPUSceneData());

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

        const mat4 P = mCamera->GetProjectionMatrix();
        mMatricesUB.P(P);
        mMatricesUB.invP(inverse(P));

        const float resolutionScale = Setting<float>("resolutionScale");
        const int scaledWidth = int(float(width) * resolutionScale);
        const int scaledHeight = int(float(height) * resolutionScale);

        GetRenderPass("gPass")->ResizeDstBuffer(scaledWidth, scaledHeight);
        GetRenderPass("lPass")->ResizeDstBuffer(scaledWidth, scaledHeight);
        GetRenderPass("bbPass")->ResizeDstBuffer(width, height);
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

    const RenderPass* Renderer::GetRenderPass(int idx) const
    {
        return mRenderPasses[idx];
    }

    const Renderer::RenderPasses& Renderer::GetRenderPasses() const
    {
        return mRenderPasses;
    }

    const ViewRayDataUB& Renderer::GetViewRayDataUB() const
    {
        return mViewRayDataUB;
    }

    const MatricesUB& Renderer::GetMatricesUB() const
    {
        return mMatricesUB;
    }

    const Camera* Renderer::GetCamera() const
    {
        return &mCamera->GetCamera();
    }

    bool Renderer::UsePBR() const
    {
        return mUsePBR;
    }

    AABB Renderer::GetSceneAABB() const
    {
        const GeometryRenderPass *gPass = static_cast<const GeometryRenderPass*>(GetRenderPass("gPass"));
        return gPass->GetGPUSceneData()->mScene->GetAABB();
    }

    void Renderer::InitRendering(const CameraHandler *camera)
    {
        if(mCamera) {
            return;
        }

        mCamera = camera;

        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
        glDepthFunc(GL_LEQUAL);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glDisable(GL_SCISSOR_TEST);
        glDisable(GL_FRAMEBUFFER_SRGB);

        // Init render passes
        mRenderPasses.push_back(new ShadowRenderPass());
        mRenderPasses.push_back(new GeometryRenderPass());
        mRenderPasses.push_back(new LightRenderPass());
        mRenderPasses.push_back(new ForwardRenderPass());
        mRenderPasses.push_back(new BackBufferRenderPass());

        for(RenderPass *rPass : mRenderPasses) {
            rPass->SetRenderer(this);
            rPass->Init();
        }

        // Init Texture Samplers
        mLinearSampler.InitForDataTexture();
        mLinearSampler.BindToSlot(TextureType::G_DEPTH_TEXTURE);
        mLinearSampler.BindToSlot(TextureType::G_NORMAL_TEXTURE);
        mLinearSampler.BindToSlot(TextureType::G_ALBEDO_TEXTURE);
        mLinearSampler.BindToSlot(TextureType::G_SPEC_GLOSS_TEXTURE);
        mLinearSampler.BindToSlot(TextureType::LIGHT_ACCUM_TEXTURE);
        mLinearSampler.BindToSlot(TextureType::FINAL_SLOT);

        mLinearMipMapSampler.InitForDiffuse();
        mLinearMipMapSampler.BindToSlot(TextureType::DIFFUSE_MAP);
        mLinearMipMapSampler.BindToSlot(TextureType::NORMAL_MAP);
        mLinearMipMapSampler.BindToSlot(TextureType::SPECULAR_MAP);
        mLinearMipMapSampler.BindToSlot(TextureType::HEIGHT_MAP);

        mShadowmapSampler.InitForShadowmapTexture();
        mShadowmapSampler.BindToSlot(TextureType::S_SHADOWMAP);

        // Init uniform buffers
        const Camera &cam = mCamera->GetCamera();
        mViewRayDataUB.Init(8);
        const float tanHalfFovy = tanf(radians(cam.mFoV) * 0.5f);
        const float aspectTanHalfFovy = cam.mAspectRatio * tanHalfFovy;
        mViewRayDataUB.tanHalfFovy(tanHalfFovy);
        mViewRayDataUB.aspectTanHalfFovy(aspectTanHalfFovy);

        mMatricesUB.Init(6 * sizeof(glm::mat4));
    }

    void Renderer::Render() const
    {
        RenderingContext rContext;
        rContext.mV = mCamera->GetViewMatrix();
        rContext.mP = mCamera->GetProjectionMatrix();
        rContext.mVP = rContext.mP * rContext.mV;
        rContext.mCamera = &mCamera->mCamera;

        mMatricesUB.V(rContext.mV);
        mMatricesUB.VP(rContext.mVP);
        mMatricesUB.invV(inverse(rContext.mV));
        mMatricesUB.invVP(inverse(rContext.mVP));

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
