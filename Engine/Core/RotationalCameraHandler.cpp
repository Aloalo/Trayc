/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Core/RotationalCameraHandler.h>
#include <Engine/Core/ContextHandler.h>
#include <Engine/Core/InputHandler.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

using namespace glm;

namespace engine
{
    RotationalCameraHandler::RotationalCameraHandler(const Camera &cam, const vec3 &lookAtPoint, float rotationSpeed, float zoomSpeed, float springiness) :
        CameraHandler(cam), mMouseDown(false), mObserveMouse(true), mZoomSpeed(zoomSpeed), mRotationSpeed(rotationSpeed),
        mRadius(length(cam.mPosition - lookAtPoint)), mLookAtPoint(lookAtPoint),
        mSpringiness(springiness), mDx(0.0f), mDy(0.0f)
    {
        mCamera.SetDirection(mLookAtPoint - mCamera.mPosition);
    }

    RotationalCameraHandler::~RotationalCameraHandler(void)
    {
    }

    void RotationalCameraHandler::KeyPress(const SDL_KeyboardEvent &e)
    {
        if(e.repeat)
            return;

        if(e.keysym.sym == SDLK_LSHIFT && e.type == SDL_KEYDOWN)
            mObserveMouse = !mObserveMouse;
    }


    void RotationalCameraHandler::MouseButton(const SDL_MouseButtonEvent &e)
    {
        if(mObserveMouse && e.button == SDL_BUTTON_LEFT)
        {
            mMouseDown = (e.state == SDL_PRESSED);
        }
    }

    void RotationalCameraHandler::MouseMotion(const SDL_MouseMotionEvent &e)
    {
        if(mObserveMouse && mMouseDown)
        {
            const float yaw = float(-e.xrel) * mRotationSpeed;
            const float pitch = float(-e.yrel) * mRotationSpeed;
            mDx += yaw;
            mDy += pitch;
        }
    }

    void RotationalCameraHandler::MouseWheel(const SDL_MouseWheelEvent &e)
    {
        const float dR = mZoomSpeed * float(-e.y);
        const vec3 dP = normalize(mCamera.mPosition - mLookAtPoint) * dR;

        if(length(mCamera.mPosition + dP) > mZoomSpeed)
            mCamera.mPosition += normalize(mCamera.mPosition - mLookAtPoint) * dR;
    }

    void RotationalCameraHandler::WindowEvent(const SDL_WindowEvent &e)
    {
        if(e.event == SDL_WINDOWEVENT_RESIZED) {
            mCamera.mAspectRatio = static_cast<float>(e.data1) / static_cast<float>(e.data2);
        }
    }

    void RotationalCameraHandler::Update(float deltaTime)
    {
        const float cf = 1.0f - expf(-mSpringiness * deltaTime);
        const float dxr = cf * mDx;
        float dyr = cf * mDy;
        mCamera.Rotate(-dxr, -dyr);
        mDx -= dxr;
        mDy -= dyr;

        const vec3 Xaxis = mCamera.GetRight();
        static const vec3 Yaxis(0.0f, 1.0f, 0.0f);
        static const mat4 I(1.0f);

        const mat3 R(rotate(rotate(I, dxr, Yaxis), dyr, Xaxis));

        mCamera.mPosition = (mCamera.mPosition - mLookAtPoint) * R + mLookAtPoint;
    }


    mat4 RotationalCameraHandler::GetProjectionMatrix() const
    {
        return mCamera.GetProjectionMatrix();
    }

    mat4 RotationalCameraHandler::GetViewMatrix() const
    {
        return mCamera.GetViewMatrix();
    }
}
