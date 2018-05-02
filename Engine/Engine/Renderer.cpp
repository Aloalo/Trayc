
#include <Engine/Engine/Renderer.h>
#include <Engine/Engine/GeometryRenderPass.h>
#include <Engine/Engine/LightRenderPass.h>
#include <Engine/Engine/ForwardRenderPass.h>
#include <Engine/Engine/BackBufferRenderPass.h>
#include <Engine/Engine/ShadowRenderPass.h>
#include <Engine/Engine/ShadowProjectionRenderPass.h>
#include <Engine/Engine/RayTraceRenderPass.h>

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
        : mCamera(nullptr)
    {
    }

    Renderer::~Renderer(void)
    {
        for(RenderPass *rPass : mRenderPasses) {
            rPass->Destroy();
            delete rPass;
        }

        TextureCombiner::DestroyVAO();
        UniformBuffers::Get().Destroy();

#if PRODUCTION
        DebugDraw::Get().Destroy();
#endif
    }

    void Renderer::AddRenderable(Renderable *renderable)
    {
        mRenderables.push_back(renderable);
    }

    void Renderer::RemoveRenderable(Renderable *renderable)
    {
        erase(mRenderables, renderable);
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

    const Camera* Renderer::GetCamera() const
    {
        return &mCamera->GetCamera();
    }

    AABB Renderer::GetSceneAABB() const
    {
        const GeometryRenderPass *gPass = static_cast<const GeometryRenderPass*>(GetRenderPass("gPass"));
        return gPass->GetGPUSceneData()->mScene->GetAABB();
    }

    GLubyte* Renderer::TakeScreenshot(int &width, int &height) const
    {
        FrameBuffer::BackBuffer().Bind();
        const auto bb = GetRenderPass("bbPass");
        width = bb->GetDstBuffer().Width();
        height = bb->GetDstBuffer().Height();

        GLubyte *pixels = new GLubyte[3 * width * height];
        glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, static_cast<void*>(pixels));

        return pixels;
    }

    void Renderer::SetViewRayData()
    {
        const Camera &c = mCamera->GetCamera();

        const auto &viewRayData = UniformBuffers::Get().ViewRayData();
        const float tanHalfFovy = tanf(radians(mCamera->GetCamera().mFoV) * 0.5f);
        const float aspectTanHalfFovy = c.mAspectRatio * tanHalfFovy;
        viewRayData.fovData(vec4(tanHalfFovy, aspectTanHalfFovy, 0.0f, 0.0f));

        viewRayData.cameraDist(vec4(c.mNearDistance, c.mFarDistance, c.mFarDistance - c.mNearDistance, 1.0f / (c.mFarDistance - c.mNearDistance)));
    }

    // -------------------------------------------
    // Rasterizer
    // -------------------------------------------

    Rasterizer::Rasterizer(void)
        : mUsePBR(true)
    {

    }

    Rasterizer::~Rasterizer(void)
    {
        mLinearMipMapSampler.Destroy();
        mLinearSampler.Destroy();
        mShadowmapSampler.Destroy();
    }

    void Rasterizer::SetUsePBR(bool usePBR)
    {
        mUsePBR = usePBR;
        LightRenderPass *lPass = static_cast<LightRenderPass*>(GetRenderPass("lPass"));
        lPass->CompileShaders();

        BackBufferRenderPass *bbPass = static_cast<BackBufferRenderPass*>(GetRenderPass("bbPass"));
        bbPass->CompileShaders();
    }

    bool Rasterizer::UsePBR() const
    {
        return mUsePBR;
    }

    void Rasterizer::SetScene(Scene *scene)
    {
        GeometryRenderPass *gPass = static_cast<GeometryRenderPass*>(GetRenderPass("gPass"));
        gPass->Init(scene);

        ShadowRenderPass *shadowPass = static_cast<ShadowRenderPass*>(GetRenderPass("shadowPass"));
        shadowPass->Init(gPass->GetGPUSceneData());

        ShadowProjectionRenderPass *shadowProjectionPass = static_cast<ShadowProjectionRenderPass*>(GetRenderPass("shadowProjectionPass"));
        shadowProjectionPass->Init(gPass->GetGPUSceneData());

        LightRenderPass *lPass = static_cast<LightRenderPass*>(GetRenderPass("lPass"));
        lPass->SetLights(scene->mLights);
    }

    void Rasterizer::SetScreenSize(int width, int height)
    {
        SetViewRayData();

        const auto &matrices = UniformBuffers::Get().Matrices();
        const mat4 P = mCamera->GetProjectionMatrix();
        matrices.P(P);
        matrices.invP(inverse(P));

        const float resolutionScale = Setting<float>("resolutionScale");
        const int scaledWidth = int(float(width) * resolutionScale);
        const int scaledHeight = int(float(height) * resolutionScale);

        GetRenderPass("gPass")->ResizeDstBuffer(scaledWidth, scaledHeight);
        GetRenderPass("lPass")->ResizeDstBuffer(scaledWidth, scaledHeight);
        GetRenderPass("bbPass")->ResizeDstBuffer(width, height);
        GetRenderPass("shadowProjectionPass")->ResizeDstBuffer(width, height);
    }

    void Rasterizer::LoadSkybox(int idx)
    {
        static_cast<ForwardRenderPass*>(GetRenderPass("forwardPass"))->LoadSkybox(idx);
    }

    void Rasterizer::InitRendering(const CameraHandler *camera)
    {
        if (mCamera) {
            return;
        }

        mCamera = camera;

        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
        glDepthFunc(GL_LEQUAL);
        glDisable(GL_SCISSOR_TEST);
        glDisable(GL_FRAMEBUFFER_SRGB);

        // Init render passes
        mRenderPasses.push_back(new ShadowRenderPass());
        mRenderPasses.push_back(new GeometryRenderPass());
        mRenderPasses.push_back(new ShadowProjectionRenderPass());
        mRenderPasses.push_back(new LightRenderPass());
        mRenderPasses.push_back(new ForwardRenderPass());
        mRenderPasses.push_back(new BackBufferRenderPass());

        for (RenderPass *rPass : mRenderPasses) {
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
        mLinearSampler.BindToSlot(TextureType::S_SHADOWPROJECTION);
        mLinearSampler.BindToSlot(TextureType::FINAL_SLOT);
        mLinearSampler.BindToSlot(TextureType::E_EFFECT1);
        mLinearSampler.BindToSlot(TextureType::E_EFFECT2);
        mLinearSampler.BindToSlot(TextureType::E_EFFECT3);

        mLinearMipMapSampler.InitForDiffuse();
        mLinearMipMapSampler.BindToSlot(TextureType::DIFFUSE_MAP);
        mLinearMipMapSampler.BindToSlot(TextureType::NORMAL_MAP);
        mLinearMipMapSampler.BindToSlot(TextureType::SPECULAR_MAP);
        mLinearMipMapSampler.BindToSlot(TextureType::HEIGHT_MAP);
        mLinearMipMapSampler.BindToSlot(TextureType::SKYBOX_SLOT);

        mShadowmapSampler.InitForShadowmapTexture();
        mShadowmapSampler.BindToSlot(TextureType::S_SHADOWMAP);

        // Init uniform buffers
        SetViewRayData();
    }

    void Rasterizer::Render() const
    {
        RenderingContext rContext;
        rContext.mV = mCamera->GetViewMatrix();
        rContext.mP = mCamera->GetProjectionMatrix();
        rContext.mVP = rContext.mP * rContext.mV;
        rContext.mCamera = &mCamera->mCamera;

        const auto &matrices = UniformBuffers::Get().Matrices();
        matrices.V(rContext.mV);
        matrices.VP(rContext.mVP);
        matrices.invV(inverse(rContext.mV));
        matrices.invVP(inverse(rContext.mVP));

        // -------------------------- Deferred render -------------------------- //

        for (const RenderPass *rPass : mRenderPasses) {
            rPass->BeginRender();
            rPass->Render(rContext);
        }

        // -------------------------- Custom forward render -------------------------- //

        for (Renderable *renderable : mRenderables) {
            if (renderable->mIsActive) {
                renderable->Draw(rContext);
            }
        }
    }

    // -------------------------------------------
    // RayTracer
    // -------------------------------------------

    RayTracer::RayTracer(void)

    {
    }

    RayTracer::~RayTracer(void)
    {
        mNoiseSampler.Destroy();
    }

    void RayTracer::SetScreenSize(int width, int height)
    {
        GetRenderPass("bbPass")->ResizeDstBuffer(width, height);
        GetRenderPass("rtPass")->ResizeDstBuffer(width, height);
    }

    void RayTracer::InitRendering(const CameraHandler *camera)
    {
        if (mCamera) {
            return;
        }

        mCamera = camera;

        mNoiseSampler.InitNearestDataTexture();
        mNoiseSampler.BindToSlot(TextureType::D_NOISE);
        mNoiseSampler.BindToSlot(TextureType::CHECKERED_1);
        mNoiseSampler.BindToSlot(TextureType::CHECKERED_2);

        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
        glDisable(GL_SCISSOR_TEST);
        glDisable(GL_FRAMEBUFFER_SRGB);

        // Init render passes
        mRTPass = new RayTraceRenderPass();
        mRenderPasses.push_back(mRTPass);
        mRenderPasses.push_back(new BackBufferRenderPass());

        for (RenderPass *rPass : mRenderPasses) {
            rPass->SetRenderer(this);
            rPass->Init();
        }
    }

    void RayTracer::Render() const
    {
        RenderingContext rContext;
        rContext.mV = mCamera->GetViewMatrix();
        rContext.mP = mCamera->GetProjectionMatrix();
        rContext.mVP = rContext.mP * rContext.mV;
        rContext.mCamera = &mCamera->mCamera;

        for (const RenderPass *rPass : mRenderPasses) {
            rPass->BeginRender();
            rPass->Render(rContext);
        }

        for (Renderable *renderable : mRenderables) {
            if (renderable->mIsActive) {
                renderable->Draw(rContext);
            }
        }
    }
}
