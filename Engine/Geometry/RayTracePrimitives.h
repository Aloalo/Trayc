#ifndef EN_RAY_TRACE_PRIMITIVES_H
#define EN_RAY_TRACE_PRIMITIVES_H

#include <Engine/Geometry/Ray.h>

namespace engine
{
    struct RTSphere
    {
        bool Intersect(const Ray &ray, float &lambda) const;

        glm::vec4 positionRadius;
        glm::vec4 diffuseSpecular;
		glm::vec2 materialData;
		float phi;
		float theta;
    };

    struct RTLight
    {
        bool Intersect(const Ray &ray, float &lambda) const;

        glm::vec4 positionRadius;
        glm::vec3 intensity;
        int __padding;
    };

    struct RTRectangle
    {
        bool Intersect(const Ray &ray, float &lambda) const;

        glm::vec4 rect;
        glm::vec4 diffuseSpecular;
        glm::vec2 materialData;
        float offset;
        int normal;
    };

    struct RTBox
    {
        bool Intersect(const Ray &ray, float &lambda) const;

        glm::vec4 diffuseSpecular;
        glm::vec3 minv;
        float reflectivity;
        glm::vec3 maxv;
		float gloss;
    };
}

#endif