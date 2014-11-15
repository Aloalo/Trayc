/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Common/MathFunctions.h>

using namespace glm;

namespace engine
{
    bool Equals(float a, float b)
    {
        return abs(a - b) < EPS;
    }

    bool Equals(const vec2 &a, const vec2 &b)
    {
        return abs(a.x - b.x) < EPS && abs(a.y - b.y) < EPS;
    }

    bool Equals(const vec3 &a, const vec3 &b)
    {
        return abs(a.x - b.x) < EPS && abs(a.y - b.y) < EPS && abs(a.z - b.z) < EPS;
    }

    bool Equals(const vec4 &a, const vec4 &b)
    {
        return abs(a.x - b.x) < EPS && abs(a.y - b.y) < EPS && abs(a.z - b.z) < EPS && abs(a.w - b.w) < EPS;
    }

    bool IsZero(float a)
    {
        return abs(a) < EPS;
    }

    bool IsZero(const vec2 &a)
    {
        return abs(a.x) < EPS && abs(a.y) < EPS;
    }

    bool IsZero(const vec3 &a)
    {
        return abs(a.x) < EPS && abs(a.y) < EPS && abs(a.z) < EPS;
    }

    bool IsZero(const vec4 &a)
    {
        return abs(a.x) < EPS && abs(a.y) < EPS && abs(a.z) < EPS && abs(a.w) < EPS;
    }

        template<class T, int N>
    bool Equals(const T &a, const T &b)
    {
        switch(N)
        {
        case 1:
            return abs(a.x - b.x) < EPS;
        case 2:
            return abs(a.x - b.x) < EPS && abs(a.y - b.y) < EPS;
        case 3:
            return abs(a.x - b.x) < EPS && abs(a.y - b.y) < EPS && abs(a.z - b.z) < EPS;
        case 4:
            return abs(a.x - b.x) < EPS && abs(a.y - b.y) < EPS && abs(a.z - b.z) < EPS && abs(a.w - b.w) < EPS;
        default:
            return false;
        }
    }

    float Ccw(const vec3 &a, const vec3 &b, const vec3 &c)
    {
        return dot(a, cross(b, c));
    }

    float Ccw(const vec4 &a, const vec4 &b, const vec4 &c)
    {
        return dot(vec3(a), cross(vec3(b), vec3(c)));
    }
}
