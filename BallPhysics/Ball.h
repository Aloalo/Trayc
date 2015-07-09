/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef BP_BALL_H
#define BP_BALL_H

#include <glm/glm.hpp>

struct Ball
{
    Ball(void);
    Ball(const glm::vec3 &position);

    glm::vec3 mPosition;
    glm::vec3 mVelocity;
};

#endif
