/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Geometry/Material.h>

using namespace std;

namespace engine
{
    Material::Material(void)
        : mKd(1.0f), mKs(1.0f), mGloss(1.0f)
    {
    }

    const string & Material::GetRenderFlags() const
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
        mRenderFlags = "";
        if(HasDiffuseMap()) {
            mRenderFlags += "DIFFUSE_MAP";
        }
        if(HasNormalMap()) {
            mRenderFlags += "NORMAL_MAP";
        }
        if(HasSpecularMap()) {
            mRenderFlags += "SPECULAR_MAP";
        }
        if(HasHeightMap()) {
            mRenderFlags += "HEIGHT_MAP";
        }
    }
}
