/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef TRAYC_UTILS_H
#define TRAYC_UTILS_H

#include <optix_world.h>
#include <string>
#include <glm/glm.hpp>

namespace trayc
{
    //host Utils
    class Utils
    {
    public:
        static float eps;
        static float pi;
        static std::string ptxPath;
        static std::string DefTexture(const std::string &name);
        static std::string Resource(const std::string &name);
        static std::string Shader(const std::string &name);
        static std::string PathToPTX(const std::string &filename);

        static optix::float3 glmToOptix(const glm::vec3 &vec);
        static bool Equals(const optix::float3 &x, const optix::float3 &y);
        static bool Equals(float x, float y);
    };
}

#endif
