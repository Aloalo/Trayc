/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef BP_VECTORFIELDS_H
#define BP_VECTORFIELDS_H

#include <glm/glm.hpp>


struct VectorField
{
    virtual glm::vec3 F(const glm::vec3 &p) const = 0;
};

struct Gravity : public VectorField
{
    Gravity(const glm::vec3 &gravity = glm::vec3(0.0f, -9.81f, 0.0f))
        : mGravity(gravity)
    {
    }

    virtual glm::vec3 F(const glm::vec3 &p) const override
    {
        return mGravity;
    }
private:
    glm::vec3 mGravity;
};

#endif
