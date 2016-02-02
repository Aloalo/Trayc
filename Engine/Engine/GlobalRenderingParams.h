/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <string>

namespace engine
{
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
}
