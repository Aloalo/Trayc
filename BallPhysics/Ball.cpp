/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include "Ball.h"

using namespace glm;

Ball::Ball(void)
    : mPosition(0.0f), mVelocity(0.0f)
{
}

Ball::Ball(const vec3 &position)
    : mPosition(position), mVelocity(0.0f)
{
}
