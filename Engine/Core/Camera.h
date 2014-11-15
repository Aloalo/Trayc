/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_CAMERA_H
#define EN_CAMERA_H

#include <glm/glm.hpp>

namespace engine
{
    class Camera
    {
    public:
        Camera(void);
        Camera(const glm::vec3 &position, float aspectRatio, float FoV);
        ~Camera(void);

        void Rotate(float yaw, float pitch);
        void Rranslate(const glm::vec3 &dir);

        glm::mat4 GetProjectionMatrix() const;
        glm::mat4 GetViewMatrix() const;

        void SetDirection(const glm::vec3 &direction);

        glm::vec3 GetDirection() const;
        glm::vec3 GetRight() const;
        glm::vec3 GetUp() const;

        glm::vec3 position;
        float aspectRatio;
        float FoV;
        float phix, phiy;
    };

}

#endif
