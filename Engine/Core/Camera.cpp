/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Core/Camera.h>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdio>

using namespace glm;

namespace engine
{
    Camera::Camera(const vec3 &position, float aspectRatio, float FoV, float nearDistance, float farDistance)
        : mPosition(position), mAspectRatio(aspectRatio), mFoV(FoV), mNearDistance(nearDistance), mFarDistance(farDistance), mYaw(0.0f), mPitch(0.0f)
    {
    }

    Camera::Camera(void)
        : mPosition(vec3(0.0f, 0.0f, 0.0f)), mAspectRatio(4.0f / 3.0f), mFoV(60.0f), mNearDistance(0.1f), mFarDistance(100.0f), mYaw(0.0f), mPitch(0.0f)
    {
    }

    Camera::~Camera(void)
    {
    }

    void Camera::Rotate(float yaw, float pitch)
    {
        mYaw += yaw;
        mPitch += pitch;
    }

    mat4 Camera::GetProjectionMatrix() const
    {
        return perspective(radians(mFoV), mAspectRatio, mNearDistance, mFarDistance);
    }

    mat4 Camera::GetViewMatrix() const
    {
        const vec3 direction = GetDirection();
        const vec3 right = GetRight();
        const vec3 up = cross(right, direction);
        return lookAt(mPosition, mPosition + direction, up);
    }

    vec3 Camera::GetDirection() const
    {
        return vec3(
            cosf(mPitch) * sinf(mYaw),
            sinf(mPitch),
            cosf(mPitch) * cosf(mYaw)
            );
    }

    vec3 Camera::GetRight() const
    {
        return vec3(-cosf(mYaw), 0.0f, sinf(mYaw));
    }

    vec3 Camera::GetUp() const
    {
        return cross(GetRight(), GetDirection());
    }

    Frustum Camera::GetFrustum() const
    {
        const vec3 center = mPosition + GetDirection() * mFarDistance * 0.5f;
        return Frustum(mPosition, center, GetUp(), mFoV, mAspectRatio, mNearDistance, mFarDistance);
    }

    void Camera::SetDirection(const glm::vec3 &direction)
    {
        const vec3 dir(normalize(direction));
        mPitch = asinf(dir.y);
        mYaw = atan2f(dir.x, dir.z);
    }

    void Camera::SetLookAtPoint(const glm::vec3 &p)
    {
        SetDirection(p - mPosition);
    }

}
