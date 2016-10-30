
#ifndef EN_CUBICBSPLINE_H
#define EN_CUBICBSPLINE_H

#include <glm/glm.hpp>
#include <vector>

namespace engine
{
    //http://www.zemris.fer.hr/predmeti/ra/labosi/vj1a.pdf
    class CubicBSpline
    {
    public:
        void SetControlPoints(const std::vector<glm::vec3> &r);

        //Position at t
        glm::vec3 operator[](float t) const;
        //Tangent at t
        glm::vec3 Tangent(float t) const;

        int NumControlPoints() const;

    private:
        std::vector<glm::vec4> mR;
        static glm::mat4 mB;
        static glm::mat4x3 mB_;
    };
}

#endif
