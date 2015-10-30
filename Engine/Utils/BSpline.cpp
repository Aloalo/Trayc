/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Utils/BSpline.h>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

namespace engine
{
    mat4 CubicBSpline::B = 1.0f / 6.0f * transpose(mat4(-1.0f,  3.0f, -3.0f, 1.0f,
                                                         3.0f, -6.0f,  3.0f, 0.0f,
                                                        -3.0f,  0.0f,  3.0f, 0.0f,
                                                         1.0f,  4.0f,  1.0f, 0.0f));

    mat4x3 CubicBSpline::B_ = 0.5f * transpose(mat3x4(-1.0f, 3.0f, -3.0f, 1.0f,
                                                       2.0f,-4.0f,  2.0f, 0.0f,
                                                      -1.0f, 0.0f,  1.0f, 0.0f));

    CubicBSpline::CubicBSpline(const vector<vec3> &controlPoints)
    {
        for(const vec3 &v : controlPoints)
            r.push_back(vec4(v, 1.0f));
        r.shrink_to_fit();
    }

    vec3 CubicBSpline::operator[](float t) const
    {
        const int idx = int(t);
        //scaled t
        const float st = t - float(idx);

        const vec4 T(st * st * st, st * st, st, 1.0f);
        const mat4 R(r[idx], r[idx + 1], r[idx + 2], r[idx + 3]);

        return vec3(T * B * transpose(R));
    }

    vec3 CubicBSpline::Tangent(float t) const
    {
        const int idx = int(t);
        //scaled t
        const float st = t - float(idx);

        const vec3 T(st * st, st, 1.0f);
        const mat4 R(r[idx], r[idx + 1], r[idx + 2], r[idx + 3]);

        return vec3(T * B_ * transpose(R));
    }

    int CubicBSpline::NumControlPoints() const
    {
        return r.size();
    }
}
