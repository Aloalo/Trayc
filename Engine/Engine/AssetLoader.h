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

        Scene LoadSceneAssimp(const std::string &path, const std::string &name, const glm::mat4 &transform) const;

    private:
        static void RecursiveLoadSceneAssimp(const aiScene *aiScene, const aiNode *aiNode, Scene &scene, const glm::mat4 &currTransform = glm::mat4(1.0f));

        friend struct Singleton<AssetLoader>;
        AssetLoader(void);

        std::string mResourcePath;
        std::string mShadersPath;
        std::string mTexturesPath;
        std::string mModelsPath;
    };
}

#endif
