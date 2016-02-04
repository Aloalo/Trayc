/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <string>

namespace engine
{
    // Also represents the texture slot idx for the given type
    enum MatTextureType
    {
        DIFFUSE_MAP = 0,
        NORMAL_MAP,
        SPECULAR_MAP,
        HEIGHT_MAP,

        CT_MAT_TEXTURE_TYPES
    };

    static const std::string MAT_TEXTURE_DEFINES[CT_MAT_TEXTURE_TYPES] = 
    {
        "DIFFUSE_MAP",
        "NORMAL_MAP",
        "SPECULAR_MAP",
        "HEIGHT_MAP"
    };

    static const std::string TEXTURE_UNIFORM_NAMES[CT_MAT_TEXTURE_TYPES] =
    {
        "diffuseMap",
        "normalMap",
        "specularMap",
        "heightMap"
    };
}
