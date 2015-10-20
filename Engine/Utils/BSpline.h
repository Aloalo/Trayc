/*
* Copyright (c) 2014 Jure Ratkovic
*/

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
        CubicBSpline(const std::vector<glm::vec3> &controlPoints);

        //Position at t
        glm::vec3 operator[](float t) const;
        //Tangent at t
        glm::vec3 Tangent(float t) const;

    private:
        std::vector<glm::vec4> r;
        static glm::mat4 B;
        static glm::mat3x4 B_;
    };
}

#endif
