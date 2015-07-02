/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Geometry/Transform.h>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

namespace engine
{
    Transform::Transform(void)
        : mPosition(0.0f), mScale(1.0f), mQuat(0.0f, vec3(0.0f))
    {
    }

    glm::mat4 Transform::GetTransformMatrix() const
    {
        return translate(rotate(scale(mat4(1.0f), mScale), angle(mQuat), axis(mQuat)), mPosition);
    }
}

