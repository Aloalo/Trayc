#include <Engine/Engine/AssetLoader.h>
#include <Engine/Engine/GlobalRenderingParams.h>
#include <Engine/Utils/Utilities.h>

#include <easylogging++.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace std;
using namespace glm;

namespace engine
{
    AssetLoader::AssetLoader(void)
    {
        mResourcePath = "../Resources/";
        mShadersPath = "Shaders/";
        mTexturesPath = "Textures/";
        mModelsPath = "Models/";
    }

    string AssetLoader::TexturePath(const std::string &name) const
    {
        return mResourcePath + mTexturesPath + name;
    }

    string AssetLoader::ShaderPath(const std::string &name) const
    {
        return mResourcePath + mShadersPath + name;
    }

    std::string AssetLoader::ModelPath(const std::string &name) const
    {
        return mResourcePath + mModelsPath + name;
    }

    static void RecursiveLoadSceneAssimp(const aiScene *aiScene, const aiNode *aiNode, Scene &scene, const mat4 &currTransform = mat4(1.0f))
    {
        const aiMatrix4x4 m = aiNode->mTransformation;
        const mat4 newTransform = (*(mat4*)&m) * currTransform;

        // Add all objects in this node to scene
        for(int i = 0; i < aiNode->mNumMeshes; ++i)
        {
            const int meshIdx = aiNode->mMeshes[i];
            const aiMesh *mesh = aiScene->mMeshes[aiNode->mMeshes[i]];
            const int matIdx = mesh->mMaterialIndex;

            scene.mObjects3D.push_back(Object3D(meshIdx, matIdx));
            Object3D &obj = scene.mObjects3D.back();
            obj.SetTransform(newTransform, scene.mTriMeshes[meshIdx].GetAABB());
        }

        // Do the same for children
        for(int i = 0; i < aiNode->mNumChildren; ++i) {
            RecursiveLoadSceneAssimp(aiScene, aiNode->mChildren[i], scene, newTransform);
        }
    }

    Scene AssetLoader::LoadSceneAssimp(const string &path, const string &name) const
    {
        Scene scene;

        const string fullPath = path + name;
        // Import the scene
        Assimp::Importer importer;
        const aiScene *aiScene = importer.ReadFile(fullPath, aiProcessPreset_TargetRealtime_MaxQuality);
        if(!aiScene)
        {
            LOG(ERROR) << importer.GetErrorString();
            return scene;
        }
        LOG(INFO) << "Loaded file: " + fullPath;
        
        // Load meshes
        for(int i = 0; i < aiScene->mNumMeshes; ++i)
        {
            scene.mTriMeshes.push_back(TriangleMesh());
            TriangleMesh &mesh = scene.mTriMeshes[i];

            const aiMesh *aimesh = aiScene->mMeshes[i];
            const aiMaterial *aimaterial = aiScene->mMaterials[aimesh->mMaterialIndex];

            mesh.mIndices.reserve(aimesh->mNumFaces * 3);
            for(int j = 0; j < aimesh->mNumFaces; ++j)
                mesh.mIndices.insert(mesh.mIndices.end(), aimesh->mFaces[j].mIndices, aimesh->mFaces[j].mIndices+3);

            const bool hasNormalMap = aimaterial->GetTextureCount(aiTextureType_NORMALS) > 0 || aimaterial->GetTextureCount(aiTextureType_HEIGHT) > 0;
            mesh.mPositions.reserve(aimesh->mNumVertices);
            mesh.mNormals.reserve(aimesh->mNumVertices);
            if(hasNormalMap) {
                mesh.mTangents.reserve(aimesh->mNumVertices);
                mesh.mBitangents.reserve(aimesh->mNumVertices);
            }
            if(aimesh->HasTextureCoords(0)) {
                mesh.mUVs.reserve(aimesh->mNumVertices);
            }

            for(int j = 0; j < aimesh->mNumVertices; ++j)
            {
                mesh.mPositions.push_back(*(vec3*)(&aimesh->mVertices[j]));
                mesh.mNormals.push_back(*(vec3*)(&aimesh->mNormals[j]));
                if(hasNormalMap)
                {
                    mesh.mTangents.push_back(*(vec3*)(&aimesh->mTangents[j]));
                    mesh.mBitangents.push_back(*(vec3*)(&aimesh->mBitangents[j]));
                }

                if(aimesh->HasTextureCoords(0))
                    mesh.mUVs.push_back(vec2(aimesh->mTextureCoords[0][j].x, aimesh->mTextureCoords[0][j].y));
            }

            mesh.CalcAABB();
        }

        // Load materials
        for(int i = 0; i < aiScene->mNumMaterials; ++i)
        {
            scene.mMaterials.push_back(Material());
            Material &material = scene.mMaterials[i];
            const aiMaterial *aimaterial = aiScene->mMaterials[i];

            aiColor3D color;
            //aimaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
            //material.mKa = *(vec3*)(&color);

            aimaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
            material.mKd = *(vec3*)(&color);

            aimaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
            material.mKs = *(vec3*)(&color);

            //aimaterial->Get(AI_MATKEY_COLOR_EMISSIVE, color);
            //material.mReflectivity = *(vec3*)(&color);

            //aimaterial->Get(AI_MATKEY_REFRACTI, material.mIoR);
            aimaterial->Get(AI_MATKEY_SHININESS, material.mGloss);
            material.mGloss = clamp(material.mGloss, 0.0f, 255.0f) / 255.0f;

            aiString texName;
            if(aimaterial->GetTextureCount(aiTextureType_DIFFUSE) != 0)
            {
                aimaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texName, nullptr, nullptr, nullptr, nullptr, nullptr);
                material.mTextureMaps.push_back(Material::TextureInfo(path + string(texName.C_Str()), TextureType::DIFFUSE_MAP));
            }
            if(aimaterial->GetTextureCount(aiTextureType_SPECULAR) != 0)
            {
                aimaterial->GetTexture(aiTextureType_SPECULAR, 0, &texName, nullptr, nullptr, nullptr, nullptr, nullptr);
                material.mTextureMaps.push_back(Material::TextureInfo(path + string(texName.C_Str()), TextureType::SPECULAR_MAP));
            }
            if(aimaterial->GetTextureCount(aiTextureType_NORMALS) != 0)
            {
                aimaterial->GetTexture(aiTextureType_NORMALS, 0, &texName, nullptr, nullptr, nullptr, nullptr, nullptr);
                material.mTextureMaps.push_back(Material::TextureInfo(path + string(texName.C_Str()), TextureType::NORMAL_MAP));

                if(aimaterial->GetTextureCount(aiTextureType_HEIGHT) != 0)
                {
                    aimaterial->GetTexture(aiTextureType_HEIGHT, 0, &texName, nullptr, nullptr, nullptr, nullptr, nullptr);
                    material.mTextureMaps.push_back(Material::TextureInfo(path + string(texName.C_Str()), TextureType::HEIGHT_MAP));
                }
            }
            else if(aimaterial->GetTextureCount(aiTextureType_HEIGHT) != 0)
            {
                aimaterial->GetTexture(aiTextureType_HEIGHT, 0, &texName, nullptr, nullptr, nullptr, nullptr, nullptr);
                string fullPath = path + string(texName.C_Str());
                // TODO: uncomment when parallax is done
                // material.mTextureMaps.push_back(Material::TextureInfo(fullPath, TextureType::HEIGHT_MAP));

                StringReplace(fullPath, "bump", "nm");
                material.mTextureMaps.push_back(Material::TextureInfo(fullPath, TextureType::NORMAL_MAP));
            }
        }

        RecursiveLoadSceneAssimp(aiScene, aiScene->mRootNode, scene);

        return scene;
    }
}
