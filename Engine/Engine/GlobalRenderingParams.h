#ifndef EN_GLOBAL_RENDERING_PARAMS_H
#define EN_GLOBAL_RENDERING_PARAMS_H

#include <string>

namespace engine
{
    // Connects texture types with texture slots
    enum TextureType
    {
        // Model texture maps
        DIFFUSE_MAP = 0,
        NORMAL_MAP = 1,
        SPECULAR_MAP = 2,
        HEIGHT_MAP = 3,

        CT_MAT_TEXTURE_TYPES = 4,

        // Geometry pass textures
        G_DEPTH_TEXTURE = 5,
        G_NORMAL_TEXTURE = 6,
        G_SPEC_GLOSS_TEXTURE = 7,
        G_ALBEDO_TEXTURE = 8,

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

    // Index of the g-pass texture in g-framebuffer
    int GetMRTIdx(TextureType type);


    enum LightType
    {
        DIRECTIONAL = 0,
        POINT = 1,
        SPOT = 2,

        CT_LIGHT_TYPES
    };

    static const std::string LIGHT_PROG_DEFINES[CT_LIGHT_TYPES] =
    {
        "DIRECTIONAL_LIGHT",
        "POINT_LIGHT",
        "SPOT_LIGHT"
    };

}

#endif
