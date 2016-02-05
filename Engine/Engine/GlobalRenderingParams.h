/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <string>

namespace engine
{
    // Connects texture types with texture slots
    enum TextureType
    {
        DIFFUSE_MAP = 0,
        NORMAL_MAP = 1,
        SPECULAR_MAP = 2,
        HEIGHT_MAP = 3,

        CT_MAT_TEXTURE_TYPES = 4,

        DEBUG_SLOT = 15
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
