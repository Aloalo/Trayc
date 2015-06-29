/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <glm/glm.hpp>

namespace engine
{
    template<class T>
    float Clamp(T x, T a, T b)
    {
        return x < a ? a : (x > b ? b : x);
    }

    template<class T>
    glm::vec3 Slerp(const glm::vec3 &start, const glm::vec3 &end, T factor)
    {
        const T dot = glm::dot(start, end);     
        clamp(dot, -1.0f, 1.0f);
        const T theta = acosf(dot) * factor;
        glm::vec3 relativeVec = glm::normalize(end - start * dot);
        return start * cosf(theta) + relativeVec * sinf(theta);
    }

    template<class T>
    glm::vec3 Lerp(const glm::vec3 &start, const glm::vec3 &end, T factor)
    {
        return start + factor * (end - start);
    }

    template<class T>
    glm::vec3 NLerp(const glm::vec3 &start, const glm::vec3 &end, T factor)
    {
        return glm::normalize(lerp(start, end, factor));
    }
}
