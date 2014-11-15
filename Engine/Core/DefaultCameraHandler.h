/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_DEFAULTCAMERAHANDLER_H
#define EN_DEFAULTCAMERAHANDLER_H

#include <Engine/Core/CameraHandler.h>

namespace engine
{
    class DefaultCameraHandler :
        public CameraHandler
    {
    public:
        DefaultCameraHandler(const Camera &cam, float speed, float rotationSpeed);
        ~DefaultCameraHandler(void);
        
        void HandleEvent(const SDL_Event &e);

        void KeyPress(const SDL_KeyboardEvent &e);
        void MouseMotion(const SDL_MouseMotionEvent &e);
        void WindowResize(const SDL_WindowEvent &e);

        void Rotate(float yaw, float pitch);
        void Update(float dt);

        glm::mat4 GetProjectionMatrix() const;
        glm::mat4 GetViewMatrix() const;

        float speed;
        float rotationSpeed;

    private:
        glm::vec3 dir;
        float springiness;
        float dx, dy;
    };
}

#endif
