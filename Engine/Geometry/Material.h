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
        glm::vec3 mKa;
        glm::vec3 mKd;
        glm::vec3 mKs;
        float mPhongExponent;

        std::string mAlbedoMap;
        std::string mSpecularMap;
        std::string mNormalMap;
    };
}

#endif
