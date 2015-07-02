/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_TRANSFORM_H
#define EN_TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace engine
{
    struct Transform
    {
        Transform(void);

        glm::mat4 GetTransformMatrix() const;

        glm::vec3 mPosition;
        glm::vec3 mScale;
        glm::quat mQuat;
    };
}

#endif
