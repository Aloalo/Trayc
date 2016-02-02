#include <Engine/Engine/AssetLoader.h>

#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace std;
using namespace glm;

namespace engine
{
    string AssetLoader::mResourcePath = "../Resources/";
    string AssetLoader::mShadersPath = "Shaders/";
    string AssetLoader::mTexturesPath = "Textures/";
    string AssetLoader::mModelsPath = "Models/" ;

    string AssetLoader::TexturePath(const std::string &name)
    {
        return mResourcePath + mTexturesPath + name;
    }

    string AssetLoader::ShaderPath(const std::string &name)
    {
        return mResourcePath + mShadersPath + name;
    }

    std::string AssetLoader::ModelPath(const std::string &name)
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

    Scene AssetLoader::LoadSceneAssimp(const string &path)
    {
        Scene scene;

        // Import the scene
        Assimp::Importer importer;
        const aiScene *aiScene = importer.ReadFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
        if(!aiScene)
        {
            cerr << importer.GetErrorString() << endl;
            return scene;
        }
        cout << "Loaded file: " + path << endl;
        
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

            const bool hasNormalMap = aimaterial == nullptr ? false : aimaterial->GetTextureCount(aiTextureType_NORMALS);
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

            aiString name;
            if(aimaterial->GetTextureCount(aiTextureType_DIFFUSE) != 0)
            {
                aimaterial->GetTexture(aiTextureType_DIFFUSE, 0, &name, nullptr, nullptr, nullptr, nullptr, nullptr);
                material.mAlbedoMap = path + string(name.C_Str());
            }
            if(aimaterial->GetTextureCount(aiTextureType_SPECULAR) != 0)
            {
                aimaterial->GetTexture(aiTextureType_SPECULAR, 0, &name, nullptr, nullptr, nullptr, nullptr, nullptr);
                material.mSpecularMap = path + string(name.C_Str());
            }
            if(aimaterial->GetTextureCount(aiTextureType_NORMALS) != 0)
            {
                aimaterial->GetTexture(aiTextureType_NORMALS, 0, &name, nullptr, nullptr, nullptr, nullptr, nullptr);
                material.mNormalMap = path + string(name.C_Str());
            }
            else if(aimaterial->GetTextureCount(aiTextureType_HEIGHT) != 0)
            {
                aimaterial->GetTexture(aiTextureType_HEIGHT, 0, &name, nullptr, nullptr, nullptr, nullptr, nullptr);
                material.mDepthMap = path + string(name.C_Str());
            }
        }

        RecursiveLoadSceneAssimp(aiScene, aiScene->mRootNode, scene);

        return scene;
    }
}
