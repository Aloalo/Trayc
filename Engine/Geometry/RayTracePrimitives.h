#ifndef EN_RAY_TRACE_PRIMITIVES
#define EN_RAY_TRACE_PRIMITIVES

#include <glm/glm.hpp>

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

    struct RTBox
    {
        glm::vec4 diffuseSpecular;
        glm::vec4 materailData;
        glm::vec3 minv;
        int __padding1;
        glm::vec3 maxv;
        int __padding2;
    };
}

#endif