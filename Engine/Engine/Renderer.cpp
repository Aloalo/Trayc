/*
* Copyright (c) 2014 Jure Ratkovic
*/
#include <Engine/Engine/Renderer.h>
#include <Engine/Engine/Game.h>
#include <Engine/Engine/TextureCombiner.h>
#include <Engine/Utils/StlExtensions.hpp>
#include <Engine/Engine/AssetLoader.h>

#include <Engine/Engine/GlobalRenderingParams.h>

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
        for(auto &spp : mMatToProg)
            spp.second.Delete();
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
        mGBuffer.Compile();

        // Init material to program map, and material to vertex arrays map
        const int ctProgs = 1 << MatTextureType::CT_MAT_TEXTURE_TYPES;
        for(int i = 0; i < ctProgs; ++i) {
            string strDefines;
            vector<string> defines;

            for(int j = 0; j < MatTextureType::CT_MAT_TEXTURE_TYPES; ++j) {
                if((1 << j) & i) {
                    strDefines += MAT_TEXTURE_DEFINES[j];
                    defines.push_back(MAT_TEXTURE_DEFINES[j]);
                }
            }

            mMatToProg[strDefines] = Program();
            mMatToProg[strDefines].Init(AssetLoader::ShaderPath("G_GeometryPass").data(), defines);
        }
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
            const string &renderFlags = mat.GetRenderFlags();
            const Program &prog = mMatToProg.at(renderFlags);

            prog.Use();

            prog.SetUniform("MVP", rContext.mVP * obj.GetTransform());
            prog.SetUniform("MV", rContext.mV * obj.GetTransform());
            prog.SetUniform("diffuseColor", mat.mKd);
            prog.SetUniform("specularGloss", vec4(mat.mKs, mat.mGloss));

            if(mat.HasDiffuseMap()) {
                prog.SetUniform("diffuseMap", MatTextureType::DIFFUSE_MAP);
                glActiveTexture(GL_TEXTURE0 + MatTextureType::DIFFUSE_MAP);
                mNameToTex.at(mat.mAlbedoMap).Bind();
            }
            if(mat.HasNormalMap()) {
                prog.SetUniform("normalMap", MatTextureType::NORMAL_MAP);
                glActiveTexture(GL_TEXTURE0 + MatTextureType::NORMAL_MAP);
                mNameToTex.at(mat.mNormalMap).Bind();
            }
            if(mat.HasSpecularMap()) {
                prog.SetUniform("specularMap", MatTextureType::SPECULAR_MAP);
                glActiveTexture(GL_TEXTURE0 + MatTextureType::SPECULAR_MAP);
                mNameToTex.at(mat.mSpecularMap).Bind();
            }
            if(mat.HasHeightMap()) {
                prog.SetUniform("heightMap", MatTextureType::HEIGHT_MAP);
                glActiveTexture(GL_TEXTURE0 + MatTextureType::HEIGHT_MAP);
                mNameToTex.at(mat.mHeightMap).Bind();
            }

            VA.RenderIndexed(GL_TRIANGLES);

            Program::Unbind();
        }

        FrameBuffer::UnBind();

        // -------------------------- Custom forward render -------------------------- //
        glClear(mClearMask);

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

        for(const Material &mat : scene->mMaterials)
        {
            if(mat.HasDiffuseMap()) {
                mNameToTex[mat.mAlbedoMap] = Texture2D(mat.mAlbedoMap.c_str());
            }
            if(mat.HasNormalMap()) {
                mNameToTex[mat.mNormalMap] = Texture2D(mat.mNormalMap.c_str());
            }
            if(mat.HasSpecularMap()) {
                mNameToTex[mat.mSpecularMap] = Texture2D(mat.mSpecularMap.c_str());
            }
            if(mat.HasHeightMap()) {
                mNameToTex[mat.mHeightMap] = Texture2D(mat.mHeightMap.c_str());
            }
        }
    }
}
