/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_ROTATIONALCAMERAHANDLER_H
#define EN_ROTATIONALCAMERAHANDLER_H

#include <Engine/Core/CameraHandler.h>

namespace engine
{
    class RotationalCameraHandler : public CameraHandler
    {
    public:
        RotationalCameraHandler(const Camera &cam, const glm::vec3 &lookAtPoint, float rotationSpeed, float zoomSpeed, float springiness);
        ~RotationalCameraHandler(void);

        virtual void KeyPress(const SDL_KeyboardEvent &e) override;
        virtual void MouseMotion(const SDL_MouseMotionEvent &e) override;
        virtual void MouseButton(const SDL_MouseButtonEvent &e) override;
        virtual void WindowEvent(const SDL_WindowEvent &e) override;
        virtual void MouseWheel(const SDL_MouseWheelEvent &e) override;

        virtual void Update(float dt) override;

        virtual glm::mat4 GetProjectionMatrix() const override;
        virtual glm::mat4 GetViewMatrix() const override;

        void SetLookAtPoint(const glm::vec3 &lookAtPoint);

    private:
        bool mMouseDown;
        bool mObserveMouse;

        float mZoomSpeed;
        float mRotationSpeed;
        glm::vec3 mLookAtPoint;
        float mSpringiness;
        float mDx, mDy;
    };
}

#endif
