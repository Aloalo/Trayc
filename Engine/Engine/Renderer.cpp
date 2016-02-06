/*
* Copyright (c) 2014 Jure Ratkovic
*/
#include <Engine/Engine/Renderer.h>
#include <Engine/Engine/Game.h>
#include <Engine/Engine/TextureCombiner.h>
#include <Engine/Utils/StlExtensions.hpp>
#include <Engine/Engine/AssetLoader.h>

#include <Engine/Core/Defines.h>
#ifdef PRODUCTION
#include <Engine/Engine/DebugDraw.h>
#endif

using namespace glm;
using namespace std;
using namespace stdext;

namespace engine
{
    Renderer::Renderer(Game *scene)
        : mScene(nullptr), mCamera(nullptr), mGame(scene), mClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    {
    }

    Renderer::~Renderer(void)
    {
        mGBuffer.Destroy();
        TextureCombiner::DestroyVAO();
        ClearVertexArrays();
        for(Program &p : mGPrograms)
            p.Delete();

        for(auto &pst : mNameToTex)
            pst.second.Delete();
#ifdef PRODUCTION
        DebugDraw::Get().Destroy();
#endif
    }

    void Renderer::SetClearColor(const glm::vec4 &clearColor) const
    {
        glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    }

    void Renderer::SetScene(const Scene *scene)
    {
        ClearVertexArrays();
        InitScene(scene);
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
        mGBuffer.AttachRBO(); // For depth testing
        mGBuffer.Compile();

        // Init material to program map, and material to vertex arrays map
        const int ctProgs = 1 << TextureType::CT_MAT_TEXTURE_TYPES;
        for(int i = 0; i < ctProgs; ++i) {
            int progDefines = 0;
            vector<string> defines;

            for(int j = 0; j < TextureType::CT_MAT_TEXTURE_TYPES; ++j) {
                if((1 << j) & i) {
                    progDefines |= (1 << j);
                    defines.push_back(MAT_TEXTURE_DEFINES[j]);
                }
            }

            // Init program with height map only if it has a normal map
            const int hasNormal = progDefines & (1 << TextureType::NORMAL_MAP);
            const int hasHeight = progDefines & (1 << TextureType::HEIGHT_MAP);
            if(!hasHeight || hasNormal) {
                mGPrograms[progDefines].Init(AssetLoader::Get().ShaderPath("G_GeometryPass").data(), defines);
            }
        }

        // Init Texture Sampling
        mTexMapSampler.InitForDiffuse();
        mTexMapSampler.BindToSlot(0);
        mTexMapSampler.BindToSlot(1);
        mTexMapSampler.BindToSlot(2);
        mTexMapSampler.BindToSlot(3);
    }

    void Renderer::Render() const
    {
        RenderingContext rContext;
        rContext.mV = mCamera->GetViewMatrix();
        rContext.mP = mCamera->GetProjectionMatrix();
        rContext.mVP = rContext.mP * rContext.mV;
        rContext.mCamera = &mCamera->mCamera;

        // -------------------------- Deferred render -------------------------- //
        mGBuffer.Bind();
        glClear(mClearMask);

        for(const Object3D &obj : mScene->mObjects3D)
        {
            const VertexArray &VA = mVertexArrays[obj.GetMeshIdx()];
            const Material &mat = mScene->mMaterials[obj.GetMaterialIdx()];
            const int renderFlags = mat.GetRenderFlags();
            const Program &prog = mGPrograms[renderFlags];

            prog.Use();

            // Mesh uniforms
            prog.SetUniform("MVP", rContext.mVP * obj.GetTransform());
            prog.SetUniform("MV", rContext.mV * obj.GetTransform());

            // Material uniforms
            prog.SetUniform("diffuseColor", mat.mKd);
            prog.SetUniform("specularGloss", vec4(mat.mKs, mat.mGloss));

            // Textures
            for(const Material::TextureInfo &texInfo : mat.mTextureMaps) {
                prog.SetUniform(TEXTURE_UNIFORM_NAMES[texInfo.type], texInfo.type);
                glActiveTexture(GL_TEXTURE0 + texInfo.type);
                mNameToTex.at(texInfo.name).Bind();
            }

            VA.RenderIndexed(GL_TRIANGLES);
        }

        Program::Unbind();
        FrameBuffer::UnBind();

        // -------------------------- Custom forward render -------------------------- //
        glClear(mClearMask);

        // Debug Draw
        DebugDraw::Get().DrawTexture(mGBuffer.GetAttachment(3));
        //DebugDraw::Get().DrawGloss(mGBuffer.GetAttachment(2));
        //DebugDraw::Get().DrawTexture(mGBuffer.GetAttachment(2));
        //DebugDraw::Get().DrawNormal(mGBuffer.GetAttachment(1));
        //DebugDraw::Get().DrawDepth(mGBuffer.GetAttachment(0), rContext.mCamera->mNearDistance, rContext.mCamera->mFarDistance / 100.0f);

        for(Renderable *renderable : mRenderables) {
            if(renderable->mIsActive) {
                renderable->Draw(rContext);
            }
        }
    }

    void Renderer::ClearVertexArrays()
    {
        for(auto &VA : mVertexArrays)
            VA.Destroy();

        mVertexArrays.clear();
    }

    void Renderer::InitScene(const Scene *scene)
    {
        mScene = scene;

        // Load mesh to memory
        for(const TriangleMesh &mesh : scene->mTriMeshes)
        {
            mVertexArrays.push_back(VertexArray(GL_STATIC_DRAW));
            VertexArray &VA = mVertexArrays.back();

            const int ctVertices = mesh.mPositions.size();
            const int ctIndices = mesh.mIndices.size();

            VA.AddAttributes(mesh.GetVertexAttribDefs());
            VA.Init(ctVertices, ctVertices);

            VA.SetVertices(static_cast<const GLvoid*>(mesh.GetVertexArray().data()), 0, ctVertices);

            if(ctIndices > 0)
            {
                void *indices = mesh.GetIndices();
                VA.SetIndices(indices, ctIndices, mesh.GetIndexType());
                delete[] indices;
            }
        }

        // Load textures to memory
        for(const Material &mat : scene->mMaterials) {
            for(const Material::TextureInfo &texInfo : mat.mTextureMaps) {
                mNameToTex[texInfo.name] = Texture2D(texInfo.name.c_str());
            }
        }
    }
}
