/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_MATHFUNCTIONS_H
#define EN_MATHFUNCTIONS_H

#include <glm/glm.hpp>

namespace engine
{
    static const float pi = 3.14159165359f;
    static const float halfpi = pi / 2.0f;
    static const float EPS = 0.0001f;

    template<class T>
    float Clamp(T x, T a, T b);

    template<class T>
    glm::vec3 Slerp(const glm::vec3 &start, const glm::vec3 &end, T factor);

    template<class T>
    glm::vec3 Lerp(const glm::vec3 &start, const glm::vec3 &end, T factor);

    template<class T>
    glm::vec3 NLerp(const glm::vec3 &start, const glm::vec3 &end, T factor);

    bool Equals(float a, float b);
    bool Equals(const glm::vec2 &a, const glm::vec2 &b);
    bool Equals(const glm::vec3 &a, const glm::vec3 &b);
    bool Equals(const glm::vec4 &a, const glm::vec4 &b);

    bool IsZero(float a);
    bool IsZero(const glm::vec2 &a);
    bool IsZero(const glm::vec3 &a);
    bool IsZero(const glm::vec4 &a);

    float Ccw(const glm::vec3 &a, const glm::vec3 &b, const glm::vec3 &c);
    float Ccw(const glm::vec4 &a, const glm::vec4 &b, const glm::vec4 &c);
}

#include <Engine/Common/MathFunctions.tpp>

#endif
