/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Core/DefaultCameraHandler.h>
#include <Engine/Core/SDLHandler.h>
#include <Engine/Core/EventHandler.h>

using namespace glm;

namespace engine
{
    DefaultCameraHandler::DefaultCameraHandler(const Camera &cam, float speed, float rotationSpeed) :
        CameraHandler(cam), mSpeed(speed), mRotationSpeed(rotationSpeed), mSpringiness(100), mDx(0), mDy(0)
    {
    }

    DefaultCameraHandler::~DefaultCameraHandler(void)
    {
    }
    
    void DefaultCameraHandler::HandleEvent(const SDL_Event &e)
    {
        switch(e.type)
        {
        case SDL_KEYUP:
        case SDL_KEYDOWN:
            KeyPress(e.key);
            break;
        case SDL_MOUSEMOTION:
            MouseMotion(e.motion);
            break;
        case SDL_WINDOWEVENT:
            WindowResize(e.window);
            break;
        }
    }

    void DefaultCameraHandler::KeyPress(const SDL_KeyboardEvent &e)
    {
        if(e.repeat)
            return;

        if(e.type == SDL_KEYDOWN && e.keysym.sym == SDLK_9)
        {
            mSpeed *= 2.0f;
            return;
        }
        else if(e.type == SDL_KEYDOWN && e.keysym.sym == SDLK_0)
        {
            mSpeed *= 0.5f;
            return;
        }

        const int mod = e.type == SDL_KEYDOWN ? 1 : -1;
        switch(e.keysym.sym)
        {
        case SDLK_w:
            mLocalMoveDir += vec3(0, 0, -mod);
            break;
        case SDLK_s:
            mLocalMoveDir += vec3(0, 0, mod);
            break;
        case SDLK_a:
            mLocalMoveDir += vec3(-mod, 0, 0);
            break;
        case SDLK_d:
            mLocalMoveDir += vec3(mod, 0, 0);
            break;
        case SDLK_q:
            mLocalMoveDir += vec3(0, -mod, 0);
            break;
        case SDLK_e:
            mLocalMoveDir += vec3(0, mod, 0);
            break;
        default:
            break;
        }
    }

    void DefaultCameraHandler::MouseMotion(const SDL_MouseMotionEvent &e)
    {
        if(EventHandler::IsCursorFree() == true)
            Rotate(static_cast<float>(-e.xrel) * mRotationSpeed, static_cast<float>(-e.yrel) * mRotationSpeed);
    }

    void DefaultCameraHandler::Rotate(float yaw, float pitch)
    {
        mDx += yaw;
        mDy += pitch;
    }

    void DefaultCameraHandler::WindowResize(const SDL_WindowEvent &e)
    {
        if(e.event == SDL_WINDOWEVENT_RESIZED)
            mCamera.mAspectRatio = static_cast<float>(e.data1) / static_cast<float>(e.data2);
    }

    void DefaultCameraHandler::Update(float deltaTime)
    {
        const float cf = 1 - expf(-mSpringiness * deltaTime);
        const float dxr = cf * mDx;
        const float dyr = cf * mDy;
        mCamera.Rotate(dxr, dyr);
        mDx -= dxr;
        mDy -= dyr;

        if(!(mLocalMoveDir.x || mLocalMoveDir.y || mLocalMoveDir.z))
            return;

        vec3 v(
            mLocalMoveDir.x,
            mLocalMoveDir.z * sinf(mCamera.mPitch),
            mLocalMoveDir.z * cosf(mCamera.mPitch)
            );
        v = vec3(
            v.x * cosf(mCamera.mYaw) + v.z * sinf(mCamera.mYaw),
            v.y,
            -v.x * sinf(mCamera.mYaw) + v.z * cosf(mCamera.mYaw)
            );
        mCamera.mPosition += deltaTime * mSpeed * normalize(vec3(0.0f, mLocalMoveDir.y, 0.0f) - v);
    }


    mat4 DefaultCameraHandler::GetProjectionMatrix() const
    {
        return mCamera.GetProjectionMatrix();
    }

    mat4 DefaultCameraHandler::GetViewMatrix() const
    {
        return mCamera.GetViewMatrix();
    }
}
