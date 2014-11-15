/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Core/Camera.h>
#include <Engine/Common/MathFunctions.h>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdio>

using namespace glm;

namespace engine
{
    Camera::Camera(const vec3 &position, float aspectRatio, float FoV)
        : position(position), aspectRatio(aspectRatio), FoV(FoV), phix(0.0f), phiy(0.0f)
    {
    }

    Camera::Camera(void)
        : position(vec3(0.0f, 0.0f, 0.0f)), aspectRatio(4.0f / 3.0f), FoV(60.0f), phix(0.0f), phiy(0.0f)
    {
    }

    Camera::~Camera(void)
    {
    }

    void Camera::Rotate(float yaw, float pitch)
    {
        phix += yaw;
        phiy += pitch;
        if(phiy > halfpi)
            phiy = halfpi;
        if(phiy < -halfpi)
            phiy = -halfpi;
    }

    mat4 Camera::GetProjectionMatrix() const
    {
        return perspective(FoV, aspectRatio, 0.1f, 100.0f);
    }

    mat4 Camera::GetViewMatrix() const
    {
        const vec3 direction = GetDirection();
        const vec3 right = GetRight();
        const vec3 up = cross(right, direction);
        return lookAt(position, position + direction, up);
    }

    vec3 Camera::GetDirection() const
    {
        return vec3(
            cosf(phiy) * sinf(phix), 
            sinf(phiy),
            cosf(phiy) * cosf(phix)
            );
    }

    vec3 Camera::GetRight() const
    {
        return vec3(
            -cosf(phix), 
            0,
            sinf(phix)
            );
    }

    vec3 Camera::GetUp() const
    {
        return cross(GetRight(), GetDirection());
    }
}
