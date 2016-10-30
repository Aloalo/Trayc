#ifndef EN_DEFAULTCAMERAHANDLER_H
#define EN_DEFAULTCAMERAHANDLER_H

#include <Engine/Core/CameraHandler.h>

namespace engine
{
    //Models a free moving camera
    class DefaultCameraHandler : public CameraHandler
    {
    public:
        DefaultCameraHandler(const Camera &cam, float moveSpeed, float rotationSpeed, float springiness);
        ~DefaultCameraHandler(void);

        virtual void KeyPress(const SDL_KeyboardEvent &e) override;
        virtual void MouseMotion(const SDL_MouseMotionEvent &e) override;
        virtual void MouseWheel(const SDL_MouseWheelEvent &e) override;

        virtual void Update(float dt) override;

        virtual glm::mat4 GetProjectionMatrix() const override;
        virtual glm::mat4 GetViewMatrix() const override;

    private:
        float mSpeed;
        float mRotationSpeed;
        //Move direction in local camera space
        glm::vec3 mLocalMoveDir;
        float mSpringiness;
        float mDx, mDy;
    };
}

#endif
