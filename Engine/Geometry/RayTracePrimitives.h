#include <glm/glm.hpp>

#ifndef EN_RT_SPHERE
#define EN_RT_SPHERE

namespace engine
{
    struct RTSphere
    {
        glm::vec4 positionRadius;
        glm::vec4 diffuseSpecular;
        glm::vec4 materailData;
    };
}

#endif