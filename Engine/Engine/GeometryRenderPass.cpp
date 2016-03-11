/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Engine/GeometryRenderPass.h>
#include <Engine/Engine/AssetLoader.h>

#include <easylogging++.h>

using namespace glm;
using namespace std;
using namespace stdext;

namespace engine
{
    GeometryRenderPass::GeometryRenderPass(void)
        : RenderPass("gPass", GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    {
    }

    void GeometryRenderPass::Init()
    {
        const string MAT_TEXTURE_DEFINES[CT_MAT_TEXTURE_TYPES] =
        {
            "DIFFUSE_MAP",
            "NORMAL_MAP",
            "SPECULAR_MAP",
            "HEIGHT_MAP"
        };

        const string TEXTURE_UNIFORM_NAMES[CT_MAT_TEXTURE_TYPES] =
        {
            "diffuseMap",
            "normalMap",
            "specularMap",
            "heightMap"
        };

        const int width = 1280;
        const int height = 720;

        // Init G buffer
        mDstFB.Init(width, height);
        mDstFB.AddAttachment(GL_R32F, GL_RED, GL_FLOAT); //Linear Depth
        mDstFB.AddAttachment(GL_RGBA16F, GL_RGBA, GL_FLOAT); //Normal view space / x
        mDstFB.AddAttachment(GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE); //Specular / Gloss
        //smDstFB.AddAttachment(GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE); //Albedo / x
        mDstFB.AddAttachment(GL_SRGB8_ALPHA8, GL_RGBA, GL_UNSIGNED_BYTE); //Albedo / x

        mDstFB.AttachRBO(); // For depth testing
        mDstFB.Compile();

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
                Program &prog = mGPrograms[progDefines];
                prog.Init(AssetLoader::Get().ShaderPath("G_GeometryPass").data(), defines);

                prog.Use();
                // Set sampler uniforms
                for(int j = 0; j < TextureType::CT_MAT_TEXTURE_TYPES; ++j) {
                    if((1 << j) & progDefines) {

                        // TODO: remove if() when parallax is done
                        if(TEXTURE_UNIFORM_NAMES[j] != "heightMap")
                            prog.SetUniform(TEXTURE_UNIFORM_NAMES[j], j);

                    }
                }
            }
        }
        Program::Unbind();

        // Bind own textures to appropriate slots
        mDstFB.GetAttachment(GetMRTIdx(TextureType::G_DEPTH_TEXTURE)).BindToSlot(TextureType::G_DEPTH_TEXTURE);
        mDstFB.GetAttachment(GetMRTIdx(TextureType::G_NORMAL_TEXTURE)).BindToSlot(TextureType::G_NORMAL_TEXTURE);
        mDstFB.GetAttachment(GetMRTIdx(TextureType::G_SPEC_GLOSS_TEXTURE)).BindToSlot(TextureType::G_SPEC_GLOSS_TEXTURE);
        mDstFB.GetAttachment(GetMRTIdx(TextureType::G_ALBEDO_TEXTURE)).BindToSlot(TextureType::G_ALBEDO_TEXTURE);
    }

    void GeometryRenderPass::Destroy()
    {
        mDstFB.Destroy();
        ClearVertexArrays();
        for(Program &p : mGPrograms)
            p.Destroy();

        for(auto &pst : mNameToTex)
            pst.second.Destroy();
    }

    void GeometryRenderPass::Render(const RenderingContext &rContext) const
    {
        for(const Object3D &obj : mScene->mObjects3D)
        {
            const int meshIdx = obj.GetMeshIdx();
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
                mNameToTex.at(texInfo.name).BindToSlot(texInfo.type);
            }

            if(mat.mHasAlphaMask) {
                glDisable(GL_CULL_FACE);
            }
            VA.Render(mScene->mTriMeshes[meshIdx].GetDrawMode());
            if(mat.mHasAlphaMask) {
                glEnable(GL_CULL_FACE);
            }
        }
    }

    void GeometryRenderPass::InitScene(const Scene *scene)
    {
        mScene = scene;

        // Load mesh to memory
        for(const TriangleMesh &mesh : scene->mTriMeshes)
        {
            mVertexArrays.push_back(VertexArray(GL_STATIC_DRAW));
            VertexArray &VA = mVertexArrays.back();
            VA.Init(&mesh);
        }
        LOG(INFO) << "Loaded meshes to GPU.";
        // Load textures to memory
        for(const Material &mat : scene->mMaterials) {
            for(const Material::TextureInfo &texInfo : mat.mTextureMaps) {
                if(mNameToTex.find(texInfo.name) == mNameToTex.end()) {
                    mNameToTex[texInfo.name] = Texture2D(texInfo.name.c_str(), true);
                }
            }
        }
        LOG(INFO) << "Loaded textures to GPU.";
    }

    void GeometryRenderPass::ClearVertexArrays()
    {
        for(auto &VA : mVertexArrays)
            VA.Destroy();

        mVertexArrays.clear();
    }
}
