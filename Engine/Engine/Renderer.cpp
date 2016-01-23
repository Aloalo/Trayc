/*
* Copyright (c) 2014 Jure Ratkovic
*/
#include <Engine/Engine/Renderer.h>
#include <Engine/Engine/Game.h>
#include <Engine/Engine/TextureCombiner.h>
#include <Engine/Utils/StlExtensions.hpp>

using namespace glm;
using namespace std;
using namespace stdext;

namespace engine
{
    Renderer::Renderer(Game *scene)
        : mCamera(nullptr), mGame(scene), mClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    {
    }

    Renderer::~Renderer(void)
    {
        mGBuffer.Destroy();
        TextureCombiner::DestroyVAO();
    }

    void Renderer::InitRendering(const CameraHandler *camera)
    {
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        int width;
        int height;
        mGame->mContextHandler.GetWindowSize(&width, &height);

        mGBuffer.Init(width, height);
        mGBuffer.AddAttachment(GL_R32F, GL_RED, GL_FLOAT); //Linear Depth
        mGBuffer.AddAttachment(GL_RGBA16F, GL_RGBA, GL_FLOAT); //Normal world space / x
        mGBuffer.AddAttachment(GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE); //Specular / Gloss
        mGBuffer.AddAttachment(GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE); //Albedo / x
        mGBuffer.Compile();

        mCamera = camera;
    }

    void Renderer::Render() const
    {
        // First deferred render

        // TODO : deferred render

        // Then custom forward render
        glClear(mClearMask);

        RenderingContext rContext;
        rContext.mV = mCamera->GetViewMatrix();
        rContext.mP = mCamera->GetProjectionMatrix();
        rContext.mVP = rContext.mP * rContext.mV;
        rContext.mCamera = &mCamera->mCamera;

        for(Renderable *renderable : mRenderables) {
            if(renderable->mIsActive) {
                renderable->Draw(rContext);
            }
        }
    }

    void Renderer::AddRenderable(Renderable *renderable)
    {
        mRenderables.push_back(renderable);
    }

    void Renderer::RemoveRenderable(Renderable *renderable)
    {
        erase(mRenderables, renderable);
    }

    void Renderer::AddRenderable(RenderableObject3D *renderable)
    {
        mDeferredRenderables.push_back(renderable);
    }

    void Renderer::RemoveRenderable(RenderableObject3D *renderable)
    {
        erase(mDeferredRenderables, renderable);
    }

    const std::vector<Light>& Renderer::GetLights() const
    {
        return mLights;
    }

    const Light& Renderer::GetLight(int idx) const
    {
        return mLights[idx];
    }

    Light& Renderer::GetLight(int idx)
    {
        return mLights[idx];
    }

    void Renderer::SetScreenSize(int width, int height)
    {
        mGBuffer.Resize(width, height);
        mGBuffer.Bind();
        glViewport(0, 0, width, height);
        FrameBuffer::UnBind();

        glViewport(0, 0, width, height);
    }

    void Renderer::AddLight(const Light &light)
    {
        mLights.push_back(light);
    }

    void Renderer::RemoveLight(int idx)
    {
        mLights.erase(mLights.begin() + idx);
    }

    void Renderer::SetClearColor(const glm::vec4 &clearColor) const
    {
        glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    }
}
