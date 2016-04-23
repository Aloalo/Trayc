
#ifndef EN_PLANE_H
#define EN_PLANE_H

#include <glm/glm.hpp>

namespace engine
{
    struct Plane
    {
        Plane(const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3);
        // Set(vec3(0.0f, 1.0f, 0.0f), vec3(0.0f));
        Plane(void);

        void Set(const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3);
        void Set(const glm::vec3 &normal, const glm::vec3 &point);
        void Set(float a, float b, float c, float d);
        float Distance(const glm::vec3 &p) const;

        glm::vec3 mNormal;
        glm::vec3 mPoint;
        float mD;
    };
}


#endif