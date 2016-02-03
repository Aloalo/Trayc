/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Geometry/Material.h>
#include <Engine/Engine/GlobalRenderingParams.h>

using namespace std;

namespace engine
{
    Material::Material(void)
        : mKd(1.0f), mKs(1.0f), mGloss(1.0f)
    {
    }

    int Material::GetRenderFlags() const
    {
        return mRenderFlags;
    }

    bool Material::HasDiffuseMap() const
    {
        return mAlbedoMap != "";
    }

    bool Material::HasNormalMap() const
    {
        return mNormalMap != "";
    }

    bool Material::HasSpecularMap() const
    {
        return mSpecularMap != "";
    }

    bool Material::HasHeightMap() const
    {
        return mHeightMap != "";
    }

    void Material::CalcRenderFlags()
    {
        mRenderFlags = 0;
        if(HasDiffuseMap()) {
            mRenderFlags |= (1 << MatTextureType::DIFFUSE_MAP);
        }
        if(HasNormalMap()) {
            mRenderFlags |= (1 << MatTextureType::NORMAL_MAP);

            if(HasHeightMap()) {
                mRenderFlags |= (1 << MatTextureType::HEIGHT_MAP);
            }
        }
        if(HasSpecularMap()) {
            mRenderFlags |= (1 << MatTextureType::SPECULAR_MAP);
        }
    }
}
