#ifndef EN_ASSET_LOADER_H
#define EN_ASSET_LOADER_H

#include <string>
#include <Engine/Geometry/Scene.h>

struct aiScene;
struct aiNode;

namespace engine
{

    class AssetLoader
    {
    public:
        AssetLoader(void);
        ~AssetLoader(void);

        static std::string TexturePath(const std::string &name);
        static std::string ShaderPath(const std::string &name);

        static Scene LoadSceneAssimp(const std::string &path);

    private:
        static void RecursiveLoadSceneAssimp(const aiScene *sc, const aiNode *nd, Scene &scene, const glm::mat4 &currTransform = glm::mat4(1.0f));

        static std::string mResourcePath;
        static std::string mShadersPath;
        static std::string mTexturesPath;
    };
}

#endif
