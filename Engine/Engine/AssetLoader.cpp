#include <Engine/Engine/AssetLoader.h>

using namespace std;

namespace engine
{
    string AssetLoader::mResourcePath = "../Resources/";
    string AssetLoader::mShadersPath = "Shaders/";
    string AssetLoader::mTexturesPath = "Textures/" ;

    AssetLoader::AssetLoader(void)
    {
    }

    AssetLoader::~AssetLoader(void)
    {
    }

    string AssetLoader::TexturePath(const std::string &name)
    {
        return mResourcePath + mTexturesPath + name;
    }

    string AssetLoader::ShaderPath(const std::string &name)
    {
        return mResourcePath + mShadersPath + name;
    }
}
