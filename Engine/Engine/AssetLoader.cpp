#include <Engine/Engine/AssetLoader.h>
#include <Engine/Engine/GlobalRenderingParams.h>
#include <Engine/Utils/Utilities.h>
#include <Engine/Utils/StlExtensions.hpp>

#include <easylogging++.h>
#include <jsonxx/jsonxx.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <IL/il.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <experimental/filesystem>
#endif

using namespace std;
using namespace stdext;
using namespace glm;
using namespace jsonxx;

namespace engine
{
    AssetLoader::AssetLoader(void)
    {
        mResourcePath = "../Resources/";
        mShadersPath = "Shaders/";
        mTexturesPath = "Textures/";
        mModelsPath = "Models/";
        mGUIPath = "GUI/";

        mCacheFolderName = "cache/";
        mMatCacheSuffix = "_materials.json";
        mMeshCacheSuffix = "_meshes.json";
        mObjCacheSuffix = "_objects.json";
    }

    string AssetLoader::TexturePath(const string &name) const
    {
        return mResourcePath + mTexturesPath + name;
    }

    string AssetLoader::ShaderPath(const string &name) const
    {
        return mResourcePath + mShadersPath + name;
    }

    string AssetLoader::ModelPath(const string &name) const
    {
        return mResourcePath + mModelsPath + name;
    }

    string AssetLoader::GUIPath(const string &name) const
    {
        return mResourcePath + mGUIPath + name;
    }

    string AssetLoader::ResourcePath(const string &name) const
    {
        return mResourcePath + name;
    }

    Scene AssetLoader::LoadScene(const string &path, const string &name) const
    {
        const string nameNoExt = name.substr(0, name.find_last_of("."));
        const string matJsonPath = path + mCacheFolderName + nameNoExt + mMatCacheSuffix;
        ifstream matFile(matJsonPath);

        // Open from cache if it exists
        if(matFile.is_open()) {
            matFile.close();
            return LoadSceneCached(path, name);
        }
        matFile.close();
        
        // Else load via assimp and then optimize and cache
        Scene scene = LoadSceneAssimp(path, name);
        OptimizeScene(scene);
        CacheScene(path, name, scene);
        return scene;
    }

    Object3D AssetLoader::CreateObject(const Scene *scene, int meshIdx, int materialIdx) const
    {
        Object3D ret(meshIdx, materialIdx);

        ret.SetBoundingVolumes(scene->mTriMeshes[meshIdx].GetAABB(), scene->mTriMeshes[meshIdx].GetBSphere());
        return ret;
    }

    void AssetLoader::SavePicture(const std::string &path, void *pixels, int width, int height)
    {
        ILuint ilImage;
        ilGenImages(1, &ilImage);
        ilBindImage(ilImage);
        ilTexImage(width, height, 1, 3, IL_RGB, IL_UNSIGNED_BYTE, pixels);
        ilSaveImage(path.c_str());
        ilDeleteImage(ilImage);
    }

    void AssetLoader::CacheScene(const string &path, const string &name, const Scene &scene) const
    {
        // Prepare
        const string nameNoExt = name.substr(0, name.find_last_of("."));
        const string basePath = path + mCacheFolderName + nameNoExt;

        #ifdef _WIN32
        CreateDirectory((path + mCacheFolderName).c_str(), nullptr);
        #else
        experimental::filesystem::create_directory(path + mCacheFolderName);
        #endif


        // Cache materials
        Array materials;
        for(const Material &mat : scene.mMaterials)
        {
            Object material;

            Array Kd;
            Kd << mat.mKd[0] << mat.mKd[1] << mat.mKd[2];
            material << "Kd" << Kd;

            Array Ks;
            Ks << mat.mKs[0] << mat.mKs[1] << mat.mKs[2];
            material << "Ks" << Ks;

            material << "gloss" << mat.mGloss;
            material << "hasAlphaMask" << mat.mHasAlphaMask;
            material << "needsForwardRender" << mat.mNeedsForwardRender;

            Array matTexMaps;
            for(const Material::TextureInfo &texInfo : mat.mTextureMaps)
            {
                Object texMap;
                texMap << "type" << texInfo.type;
                texMap << "name" << texInfo.name;
                matTexMaps << texMap;
            }

            material << "textureMaps" << matTexMaps;
            materials << material;
        }
        ofstream(basePath + mMatCacheSuffix, ofstream::out) << materials.json();

        // Cache meshes
        Array meshes;
        const int ctMeshes = scene.mTriMeshes.size();
        for(int i = 0; i < ctMeshes; ++i)
        {
            const TriangleMesh &eMesh = scene.mTriMeshes[i];
            const string meshFileName = basePath + "mesh" + to_string(i) + ".dat";
            Object jMesh;

            jMesh << "drawMode" << eMesh.GetDrawMode();
            jMesh << "ctPositions" << eMesh.mPositions.size();
            jMesh << "ctUVs" << eMesh.mUVs.size();
            jMesh << "ctNormals" << eMesh.mNormals.size();
            jMesh << "ctTangents" << eMesh.mTangents.size();
            jMesh << "ctBitangents" << eMesh.mBitangents.size();
            jMesh << "ctIndices" << eMesh.mIndices.size();
            jMesh << "file" << meshFileName;

            const vec3 mi = eMesh.GetAABB().mMinv;
            const vec3 ma = eMesh.GetAABB().mMaxv;
            Array aabb;
            aabb << mi.x << mi.y << mi.z << ma.x << ma.y << ma.z;
            jMesh << "AABB" << aabb;

            const vec3 c = eMesh.GetBSphere().mCenter;
            const float r = eMesh.GetBSphere().mRadius;
            Array sphere;
            sphere << c.x << c.y << c.z << r;
            jMesh << "BSphere" << sphere;

            std::ofstream meshFile(meshFileName, std::ios::out | std::ofstream::binary);
            meshFile.write(reinterpret_cast<const char*>(eMesh.mPositions.data()), eMesh.mPositions.size() * sizeof(vec3));
            meshFile.write(reinterpret_cast<const char*>(eMesh.mUVs.data()), eMesh.mUVs.size() * sizeof(vec2));
            meshFile.write(reinterpret_cast<const char*>(eMesh.mNormals.data()), eMesh.mNormals.size() * sizeof(vec3));
            meshFile.write(reinterpret_cast<const char*>(eMesh.mTangents.data()), eMesh.mTangents.size() * sizeof(vec3));
            meshFile.write(reinterpret_cast<const char*>(eMesh.mBitangents.data()), eMesh.mBitangents.size() * sizeof(vec3));
            meshFile.write(reinterpret_cast<const char*>(eMesh.mIndices.data()), eMesh.mIndices.size() * sizeof(unsigned int));

            meshes << jMesh;
        }
        ofstream(basePath + mMeshCacheSuffix, ofstream::out) << meshes.json();

        // Cache objects
        Array objects;
        for(const Object3D &obj : scene.mObjects3D)
        {
            Object object3D;

            object3D << "dynamicGeometry" << obj.mDynamicGeometry;
            object3D << "shadowCaster" << obj.mShadowCaster;
            object3D << "meshIdx" << obj.GetMeshIdx();
            object3D << "materialIdx" << obj.GetMaterialIdx();

            const mat4 &t = obj.GetTransform();
            Array transform;
            transform << t[0][0] << t[0][1] << t[0][2] << t[0][3] <<
                t[1][0] << t[1][1] << t[1][2] << t[1][3] <<
                t[2][0] << t[2][1] << t[2][2] << t[2][3] <<
                t[3][0] << t[3][1] << t[3][2] << t[3][3];

            object3D << "transform" << transform;
            objects << object3D;
        }
        ofstream(basePath + mObjCacheSuffix, ofstream::out) << objects.json();
    }

    static inline Array LoadArray(const string &path)
    {
        ifstream jsonFile(path);
        const string jsonStr((istreambuf_iterator<char>(jsonFile)), istreambuf_iterator<char>());
        Array ret;
        ret.parse(jsonStr);
        return ret;
    }

    Scene AssetLoader::LoadSceneCached(const string &path, const string &name) const
    {
        Scene scene;

        // Prepare
        const string nameNoExt = name.substr(0, name.find_last_of("."));
        const string basePath = path + mCacheFolderName + nameNoExt;

        // Load materials
        const Array materials = LoadArray(basePath + mMatCacheSuffix);
        const int ctMaterials = materials.size();
        for(int i = 0; i < ctMaterials; ++i)
        {
            const Object &jMat = materials.get<Object>(i);
            scene.mMaterials.push_back(Material());
            Material &mat = scene.mMaterials.back();

            const Array &Kd = jMat.get<Array>("Kd");
            mat.mKd = vec3(Kd.get<Number>(0), Kd.get<Number>(1), Kd.get<Number>(2));

            const Array &Ks = jMat.get<Array>("Ks");
            mat.mKs = vec3(Ks.get<Number>(0), Ks.get<Number>(1), Ks.get<Number>(2));
            mat.mGloss = float(jMat.get<Number>("gloss"));
            mat.mHasAlphaMask = jMat.get<Boolean>("hasAlphaMask");
            mat.mNeedsForwardRender = jMat.get<Boolean>("needsForwardRender");

            const Array &matTexMaps = jMat.get<Array>("textureMaps");
            const int ctTexMaps = matTexMaps.size();
            for(int j = 0; j < ctTexMaps; ++j)
            {
                const Object &texMap = matTexMaps.get<Object>(j);
                mat.mTextureMaps.push_back(Material::TextureInfo(texMap.get<String>("name"), int(texMap.get<Number>("type"))));
            }
        }

        // Load meshes
        const Array meshes = LoadArray(basePath + mMeshCacheSuffix);
        const int ctMeshes = meshes.size();
        for(int i = 0; i < ctMeshes; ++i)
        {
            const Object &jMesh = meshes.get<Object>(i);

            // Load geometry data
            scene.mTriMeshes.push_back(TriangleMesh((unsigned int)jMesh.get<Number>("drawMode")));
            TriangleMesh &eMesh = scene.mTriMeshes.back();

            const int ctPositions = (int)jMesh.get<Number>("ctPositions");
            const int ctUVs = (int)jMesh.get<Number>("ctUVs");
            const int ctNormals = (int)jMesh.get<Number>("ctNormals");
            const int ctTangents = (int)jMesh.get<Number>("ctTangents");
            const int ctBitangents = (int)jMesh.get<Number>("ctBitangents");
            const int ctIndices = (int)jMesh.get<Number>("ctIndices");

            const string meshFileName = jMesh.get<String>("file");

            std::ifstream meshFile(meshFileName, std::ios::in | std::ofstream::binary);
            eMesh.mPositions.resize(ctPositions);
            meshFile.read(reinterpret_cast<char*>(eMesh.mPositions.data()), ctPositions * sizeof(vec3));
            eMesh.mUVs.resize(ctUVs);
            if(ctUVs)
                meshFile.read(reinterpret_cast<char*>(eMesh.mUVs.data()), ctUVs * sizeof(vec2));
            eMesh.mNormals.resize(ctNormals);
            if(ctNormals)
                meshFile.read(reinterpret_cast<char*>(eMesh.mNormals.data()), ctNormals * sizeof(vec3));
            eMesh.mTangents.resize(ctTangents);
            if(ctTangents)
                meshFile.read(reinterpret_cast<char*>(eMesh.mTangents.data()), ctTangents * sizeof(vec3));
            eMesh.mBitangents.resize(ctBitangents);
            if(ctBitangents)
                meshFile.read(reinterpret_cast<char*>(eMesh.mBitangents.data()), ctBitangents * sizeof(vec3));
            eMesh.mIndices.resize(ctIndices);
            if(ctIndices)
                meshFile.read(reinterpret_cast<char*>(eMesh.mIndices.data()), ctIndices * sizeof(unsigned int));

            // Load bounding volumes
            const Array &b = jMesh.get<Array>("AABB");
            const vec3 mi(b.get<Number>(0), b.get<Number>(1), b.get<Number>(2));
            const vec3 ma(b.get<Number>(3), b.get<Number>(4), b.get<Number>(5));

            const Array &s = jMesh.get<Array>("BSphere");
            const vec3 c(s.get<Number>(0), s.get<Number>(1), s.get<Number>(2));

            eMesh.SetBoundingVolumes(AABB(mi, ma), Sphere(c, float(s.get<Number>(3))));
        }

        // Load objects
        const Array objects = LoadArray(basePath + mObjCacheSuffix);
        const int ctObjects = objects.size();
        for(int i = 0; i < ctObjects; ++i)
        {
            const Object &object = objects.get<Object>(i);

            const int meshIdx = (int)object.get<Number>("meshIdx");
            scene.mObjects3D.push_back(Object3D(meshIdx, (int)object.get<Number>("materialIdx")));
            Object3D &object3D = scene.mObjects3D.back();
            object3D.SetBoundingVolumes(scene.mTriMeshes[meshIdx].GetAABB(), scene.mTriMeshes[meshIdx].GetBSphere());

            object3D.mDynamicGeometry = object.get<Boolean>("dynamicGeometry");
            object3D.mShadowCaster = object.get<Boolean>("shadowCaster");

            mat4 transform;
            const Array &t = object.get<Array>("transform");
            for(int j = 0; j < 4; ++j)
                for(int k = 0; k < 4; ++k)
                    transform[j][k] = float(t.get<Number>(j * 4 + k));
            object3D.SetTransform(transform);
        }

        return scene;
    }

    void AssetLoader::RecursiveLoadSceneAssimp(const aiScene *aiScene, const aiNode *aiNode, Scene &scene) const
    {
        const aiMatrix4x4 m = aiNode->mTransformation;
        const mat4 transform = (*(mat4*)&m);

        // Add all objects in this node to scene
        for(unsigned int i = 0; i < aiNode->mNumMeshes; ++i)
        {
            const int meshIdx = aiNode->mMeshes[i];
            const aiMesh *mesh = aiScene->mMeshes[aiNode->mMeshes[i]];
            const int matIdx = mesh->mMaterialIndex;

            scene.mObjects3D.push_back(Object3D(meshIdx, matIdx));
            Object3D &obj = scene.mObjects3D.back();
            obj.SetTransform(transform);
            obj.SetBoundingVolumes(scene.mTriMeshes[meshIdx].GetAABB(), scene.mTriMeshes[meshIdx].GetBSphere());
        }

        // Do the same for children
        for(unsigned int i = 0; i < aiNode->mNumChildren; ++i) {
            RecursiveLoadSceneAssimp(aiScene, aiNode->mChildren[i], scene);
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
            LOG(ERROR) << "[AssetLoader::LoadSceneAssimp] " << importer.GetErrorString();
            return scene;
        }
        LOG(INFO) << "[AssetLoader::LoadSceneAssimp] Loaded file: " + fullPath;
        
        // Load meshes
        for(unsigned int i = 0; i < aiScene->mNumMeshes; ++i)
        {
            scene.mTriMeshes.push_back(TriangleMesh(GL_TRIANGLES));
            TriangleMesh &mesh = scene.mTriMeshes[i];

            const aiMesh *aimesh = aiScene->mMeshes[i];
            const aiMaterial *aimaterial = aiScene->mMaterials[aimesh->mMaterialIndex];

            mesh.mIndices.reserve(aimesh->mNumFaces * 3);
            for(unsigned int j = 0; j < aimesh->mNumFaces; ++j)
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

            for(unsigned int j = 0; j < aimesh->mNumVertices; ++j)
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

            mesh.CalcBoundingVolumes();
        }

        // Load materials
        for(unsigned int i = 0; i < aiScene->mNumMaterials; ++i)
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
            if(aimaterial->GetTextureCount(aiTextureType_OPACITY) != 0)
            {
                material.mHasAlphaMask = true;
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

            for(Material::TextureInfo &texInfo : material.mTextureMaps)
                StringReplace(texInfo.name, "\\", "/");
        }

        RecursiveLoadSceneAssimp(aiScene, aiScene->mRootNode, scene);

        return scene;
    }

    void AssetLoader::OptimizeScene(Scene &scene) const
    {
        sort(scene.mObjects3D, [&](const Object3D &obj1, const Object3D &obj2) -> bool
        {
            const int matIdx1 = obj1.GetMaterialIdx();
            const int matIdx2 = obj2.GetMaterialIdx();
            return matIdx1 < matIdx2 || (matIdx1 == matIdx2 && obj1.GetMeshIdx() < obj2.GetMeshIdx());
        });
    }
}
