/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Geometry/Material.h>

using namespace std;

namespace engine
{
    Material::Material(void)
        : mKd(1.0f), mKs(1.0f), mGloss(1.0f), mHasAlphaMask(false)
    {
    }

    int Material::GetRenderFlags() const
    {
        int retRenderFlags = 0;
        for(const TextureInfo &texInfo : mTextureMaps)
            retRenderFlags |= (1 << texInfo.type);
        return retRenderFlags;
    }

    Material::TextureInfo::TextureInfo(const string &name, int type)
        : name(name), type(type)
    {
    }
}
