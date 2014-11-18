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
    public:
        glm::vec3 Ka;
        glm::vec3 Kd;
        glm::vec3 Ks;
        glm::vec3 reflectivity;
        float IoR;

        std::string diffuse_map;
        std::string specular_map;
        std::string normal_map;

        //Index in scene's material list
        int index;
    };
}

#endif
