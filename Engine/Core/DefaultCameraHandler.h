/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_DEFAULTCAMERAHANDLER_H
#define EN_DEFAULTCAMERAHANDLER_H

#include <Engine/Core/CameraHandler.h>

namespace engine
{
    class DefaultCameraHandler : public CameraHandler
    {
    public:
        DefaultCameraHandler(const Camera &cam, float speed, float rotationSpeed);
        ~DefaultCameraHandler(void);

        void KeyPress(const SDL_KeyboardEvent &e) override;
        void MouseMotion(const SDL_MouseMotionEvent &e) override;
        void WindowEvent(const SDL_WindowEvent &e) override;
        void MouseWheel(const SDL_MouseWheelEvent &e) override;

        void Update(float dt) override;

        glm::mat4 GetProjectionMatrix() const override;
        glm::mat4 GetViewMatrix() const override;

    private:
        bool mObserveMouse;

        float mSpeed;
        float mRotationSpeed;
        //Move direction in local camera space
        glm::vec3 mLocalMoveDir;
        float mSpringiness;
        float mDx, mDy;
    };
}

#endif
