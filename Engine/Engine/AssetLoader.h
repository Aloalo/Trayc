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

        Scene LoadScene(const std::string &path, const std::string &name) const;

    private:
        void CacheScene(const std::string &path, const std::string &name, const Scene &scene) const;
        Scene LoadSceneCached(const std::string &path, const std::string &name) const;

        Scene LoadSceneAssimp(const std::string &path, const std::string &name) const;
        void RecursiveLoadSceneAssimp(const aiScene *aiScene, const aiNode *aiNode, Scene &scene) const;

        friend struct Singleton<AssetLoader>;
        AssetLoader(void);

        std::string mResourcePath;
        std::string mShadersPath;
        std::string mTexturesPath;
        std::string mModelsPath;

        // Cache stuff
        std::string mCacheFolderName;
        std::string mMatCacheSuffix;
        std::string mMeshCacheSuffix;
        std::string mObjCacheSuffix;
    };
}

#endif
