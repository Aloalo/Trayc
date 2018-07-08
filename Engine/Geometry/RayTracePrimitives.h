#ifndef EN_RAY_TRACE_PRIMITIVES_H
#define EN_RAY_TRACE_PRIMITIVES_H

#include <Engine/Geometry/Ray.h>
#include <limits>

namespace engine
{
    struct RTSphere
    {
        bool Intersect(const Ray &ray, float &minLambda) const;

        glm::vec4 positionRadius2; // radius is squared
        glm::vec4 diffuseSpecular;
		glm::vec2 materialData;
		float phi;
		float theta;
    };

    struct RTLight
    {
        bool Intersect(const Ray &ray, float &minLambda) const;

        glm::vec4 positionRadius2; // radius is squared
        glm::vec4 directionAngle; // direction.xyz, cos(SpotCutoff)
        glm::vec3 intensity;
        float spotExponent;
    };

    struct RTRectangle
    {
        bool Intersect(const Ray &ray, float &minLambda) const;

        glm::vec4 p1; // p1.xyz, gloss 
        glm::vec4 p2; // p2.xyz, reflectivity
        glm::vec4 p3; // p3.xyz, specular
        glm::vec3 diffuse;
        int __padding;
    };

    struct RTBox
    {
        bool Intersect(const Ray &ray, float &minLambda) const;

        glm::vec4 diffuseSpecular;
        glm::vec3 minv;
        float reflectivity;
        glm::vec3 maxv;
		float gloss;
    };
}

#endif