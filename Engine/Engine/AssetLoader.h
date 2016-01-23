#ifndef EN_ASSET_LOADER_H
#define EN_ASSET_LOADER_H

#include <string>

namespace engine
{
    class AssetLoader
    {
    public:
        AssetLoader(void);
        ~AssetLoader(void);

        static std::string TexturePath(const std::string &name);
        static std::string ShaderPath(const std::string &name);

    private:
        static std::string mResourcePath;
        static std::string mShadersPath;
        static std::string mTexturesPath;
    };
}

#endif
