/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_MATERIAL_H
#define EN_MATERIAL_H

#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace engine
{
    struct Material
    {
        Material(void);

        // Render flags for geometry program
        int GetRenderFlags() const;

        glm::vec3 mKd;
        glm::vec3 mKs;
        float mGloss;
        bool mHasAlphaMask;

        struct TextureInfo
        {
            TextureInfo(const std::string &name, int type);

            std::string name;
            // Coresponds to GlobalRenderingParams' MatTextureType
            int type;
        };

        std::vector<TextureInfo> mTextureMaps;
    };
}

#endif
