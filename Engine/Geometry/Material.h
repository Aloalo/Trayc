/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_MATERIAL_H
#define EN_MATERIAL_H

#include <glm/glm.hpp>
#include <string>

namespace engine
{
    struct Material
    {
        Material(void);

        // Render flags for geometry program
        // Call wheh done with initialisation
        void CalcRenderFlags();
        int GetRenderFlags() const;
        bool HasDiffuseMap() const;
        bool HasNormalMap() const;
        bool HasSpecularMap() const;
        bool HasHeightMap() const;

        glm::vec3 mKd;
        glm::vec3 mKs;
        float mGloss;

        std::string mAlbedoMap;
        std::string mSpecularMap;
        std::string mNormalMap;
        std::string mHeightMap;

    private:
        int mRenderFlags;
    };
}

#endif
