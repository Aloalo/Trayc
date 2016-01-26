/*
* Copyright (c) 2014 Jure Ratkovic
*/
#include <Engine/Engine/Renderer.h>
#include <Engine/Engine/Game.h>
#include <Engine/Engine/TextureCombiner.h>
#include <Engine/Utils/StlExtensions.hpp>
#include <Engine/Engine/AssetLoader.h>

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
        if(mCamera) {
            return;
        }

        mCamera = camera;

        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        int width;
        int height;
        mGame->mContextHandler.GetWindowSize(&width, &height);

        // Init G buffer
        mGBuffer.Init(width, height);
        mGBuffer.AddAttachment(GL_R32F, GL_RED, GL_FLOAT); //Linear Depth
        mGBuffer.AddAttachment(GL_RGBA16F, GL_RGBA, GL_FLOAT); //Normal view space / x
        mGBuffer.AddAttachment(GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE); //Specular / Gloss
        mGBuffer.AddAttachment(GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE); //Albedo / x
        mGBuffer.Compile();

        // Init material to program map
        const int ctDefines = 3;
        const string gProgDefines[ctDefines] =
        {
            "NORMAL_MAP",
            "DIFFUSE_MAP",
            "SPECULAR_MAP"
        };
        const int ctProgs = 1 << ctDefines;
        for(int i = 0; i < ctProgs; ++i) {
            string strDefines;
            vector<string> defines;

            for(int j = 0; j < ctDefines; ++j) {
                if((i << j) & 1) {
                    strDefines += gProgDefines[j];
                    defines.push_back(gProgDefines[j]);
                }
            }

            mMatToProg[strDefines] = Program();
            mMatToProg[strDefines].Init(AssetLoader::ShaderPath("G_GeometryPass").data(), defines);
        }
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

    void Renderer::SetScreenSize(int width, int height)
    {
        mGBuffer.Resize(width, height);
        mGBuffer.Bind();
        glViewport(0, 0, width, height);
        FrameBuffer::UnBind();

        glViewport(0, 0, width, height);
    }

    void Renderer::SetClearColor(const glm::vec4 &clearColor) const
    {
        glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    }
}
