/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_CAMERA_H
#define EN_CAMERA_H

#include <glm/glm.hpp>
#include <Engine/Geometry/Frustum.h>

namespace engine
{
    class Camera
    {
    public:
        Camera(void);
        Camera(const glm::vec3 &position, float aspectRatio, float FoV, float nearDistance = 0.1f, float farDistance = 100.0f);
        ~Camera(void);

        void Rotate(float yaw, float pitch);

        glm::mat4 GetProjectionMatrix() const;
        glm::mat4 GetViewMatrix() const;

        void SetDirection(const glm::vec3 &direction);
        void SetLookAtPoint(const glm::vec3 &p);

        glm::vec3 GetDirection() const;
        glm::vec3 GetRight() const;
        glm::vec3 GetUp() const;

        Frustum GetFrustum() const;

        glm::vec3 mPosition;
        float mAspectRatio;
        float mFoV;
        float mNearDistance;
        float mFarDistance;
        float mYaw, mPitch;
    };

}

#endif
