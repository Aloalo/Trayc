#ifndef EN_ASSET_LOADER_H
#define EN_ASSET_LOADER_H

#include <Engine/Geometry/Scene.h>
#include <Engine/Utils/Singleton.h>


namespace engine
{
    class AssetLoader : public Singleton<AssetLoader>
    {
    public:
        std::string TexturePath(const std::string &name) const;
        std::string ShaderPath(const std::string &name) const;
        std::string ModelPath(const std::string &name) const;

        Scene LoadSceneAssimp(const std::string &path, const std::string &name) const;

    private:
        friend struct Singleton<AssetLoader>;
        AssetLoader(void);

        std::string mResourcePath;
        std::string mShadersPath;
        std::string mTexturesPath;
        std::string mModelsPath;
    };
}

#endif
