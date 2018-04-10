#include <glm/glm.hpp>

#ifndef EN_RAY_TRACE_PRIMITIVES
#define EN_RAY_TRACE_PRIMITIVES

namespace engine
{
    struct RTSphere
    {
        glm::vec4 positionRadius;
        glm::vec4 diffuseSpecular;
        glm::vec4 materailData;
    };

    struct RTLight
    {
        glm::vec4 positionRadius;
        glm::vec3 intensity;
        int __padding;
    };

    struct RTRectangle
    {
        glm::vec4 rect;
        glm::vec4 diffuseSpecular;
        glm::vec2 materailData;
        float offset;
        int normal;
    };
}

#endif