#ifndef EN_ASSET_LOADER_H
#define EN_ASSET_LOADER_H

#include <Engine/Geometry/Scene.h>
#include <Engine/Utils/Singleton.h>

struct aiScene;
struct aiNode;

namespace engine
{
    class AssetLoader : public Singleton<AssetLoader>
    {
    public:
        std::string TexturePath(const std::string &name) const;
        std::string ShaderPath(const std::string &name) const;
        std::string ModelPath(const std::string &name) const;
        std::string GUIPath(const std::string &name) const;
        // From root resources folder
        std::string ResourcePath(const std::string &name) const;
        std::string LevelsPath() const;

        Scene LoadScene(const std::string &path, const std::string &name) const;
        Object3D CreateObject(const Scene *scene, int meshIdx, int materialIdx) const;

        void SavePicture(const std::string &path, void *pixels, int width, int height);

    private:
        void CacheScene(const std::string &path, const std::string &name, const Scene &scene) const;
        Scene LoadSceneCached(const std::string &path, const std::string &name) const;

        Scene LoadSceneAssimp(const std::string &path, const std::string &name) const;
        void RecursiveLoadSceneAssimp(const aiScene *aiScene, const aiNode *aiNode, Scene &scene) const;

        // Sort objects by materials, renderflags and meshes
        void OptimizeScene(Scene &scene) const;

        friend struct Singleton<AssetLoader>;
        AssetLoader(void);

        std::string mResourcePath;
        std::string mShadersPath;
        std::string mTexturesPath;
        std::string mModelsPath;
        std::string mGUIPath;

        // Cache stuff
        std::string mCacheFolderName;
        std::string mMatCacheSuffix;
        std::string mMeshCacheSuffix;
        std::string mObjCacheSuffix;
    };
}

#endif
