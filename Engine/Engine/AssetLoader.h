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
        static std::string TexturePath(const std::string &name);
        static std::string ShaderPath(const std::string &name);
        static std::string ModelPath(const std::string &name);

        static Scene LoadSceneAssimp(const std::string &path, const std::string &name);

    private:
        static std::string mResourcePath;
        static std::string mShadersPath;
        static std::string mTexturesPath;
        static std::string mModelsPath;
    };
}

#endif
