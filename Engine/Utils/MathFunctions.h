/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_MATHFUNCTIONS_H
#define EN_MATHFUNCTIONS_H

#include <glm/glm.hpp>

namespace engine
{
    static const glm::mat4 O(0.0f);
    static const glm::mat4 I(1.0f);
    static const float PI = 3.14159165359f;
    static const float HALF_PI = PI / 2.0f;
    static const float EPS = 0.0001f;

    template<class T>
    inline float Clamp(T x, T a, T b);

    template<class T>
    inline glm::vec3 Slerp(const glm::vec3 &start, const glm::vec3 &end, T factor);

    template<class T>
    inline glm::vec3 Lerp(const glm::vec3 &start, const glm::vec3 &end, T factor);

    template<class T>
    inline glm::vec3 NLerp(const glm::vec3 &start, const glm::vec3 &end, T factor);

    inline bool Equals(float a, float b);
    inline bool Equals(const glm::vec2 &a, const glm::vec2 &b);
    inline bool Equals(const glm::vec3 &a, const glm::vec3 &b);
    inline bool Equals(const glm::vec4 &a, const glm::vec4 &b);

    inline bool IsZero(float a);
    inline bool IsZero(const glm::vec2 &a);
    inline bool IsZero(const glm::vec3 &a);
    inline bool IsZero(const glm::vec4 &a);

    inline float Ccw(const glm::vec3 &a, const glm::vec3 &b, const glm::vec3 &c);
    inline float Ccw(const glm::vec4 &a, const glm::vec4 &b, const glm::vec4 &c);
}

#include <Engine/Utils/MathFunctions.tpp>

#endif
