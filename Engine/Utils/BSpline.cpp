
#include <Engine/Utils/BSpline.h>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

namespace engine
{
    mat4 CubicBSpline::mB = 1.0f / 6.0f * transpose(mat4(-1.0f,  3.0f, -3.0f, 1.0f,
                                                         3.0f, -6.0f,  3.0f, 0.0f,
                                                        -3.0f,  0.0f,  3.0f, 0.0f,
                                                         1.0f,  4.0f,  1.0f, 0.0f));

    mat4x3 CubicBSpline::mB_ = 0.5f * transpose(mat3x4(-1.0f, 3.0f, -3.0f, 1.0f,
                                                       2.0f,-4.0f,  2.0f, 0.0f,
                                                      -1.0f, 0.0f,  1.0f, 0.0f));

    void CubicBSpline::SetControlPoints(const vector<vec3> &r)
    {
        for(const vec3 &v : r)
            mR.push_back(vec4(v, 1.0f));
        mR.shrink_to_fit();
    }


    vec3 CubicBSpline::operator[](float t) const
    {
        const int idx = int(t);
        //scaled t
        const float st = t - float(idx);

        const vec4 T(st * st * st, st * st, st, 1.0f);
        const mat4 R(mR[idx], mR[idx + 1], mR[idx + 2], mR[idx + 3]);

        return vec3(T * mB * transpose(R));
    }

    vec3 CubicBSpline::Tangent(float t) const
    {
        const int idx = int(t);
        //scaled t
        const float st = t - float(idx);

        const vec3 T(st * st, st, 1.0f);
        const mat4 R(mR[idx], mR[idx + 1], mR[idx + 2], mR[idx + 3]);

        return vec3(T * mB_ * transpose(R));
    }

    int CubicBSpline::NumControlPoints() const
    {
        return static_cast<int>(mR.size());
    }
}
