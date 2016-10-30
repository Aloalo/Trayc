#include <Engine/Core/DefaultCameraHandler.h>
#include <Engine/Core/ContextHandler.h>
#include <Engine/Core/InputHandler.h>
#include <glm/gtc/constants.hpp>

using namespace glm;

namespace engine
{
    DefaultCameraHandler::DefaultCameraHandler(const Camera &cam, float moveSpeed, float rotationSpeed, float springiness) :
        CameraHandler(cam), mSpeed(moveSpeed), mRotationSpeed(rotationSpeed), mSpringiness(springiness), mDx(0.0f), mDy(0.0f)
    {
    }

    DefaultCameraHandler::~DefaultCameraHandler(void)
    {
    }

    void DefaultCameraHandler::KeyPress(const SDL_KeyboardEvent &e)
    {
        if(e.repeat)
            return;

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
        const float yaw = float(-e.xrel) * mRotationSpeed;
        const float pitch = float(-e.yrel) * mRotationSpeed;
        mDx += yaw;
        mDy += pitch;
    }

    void DefaultCameraHandler::MouseWheel(const SDL_MouseWheelEvent &e)
    {
        mSpeed *= (e.y > 0 ? 2.0f : 0.5f);
    }

    void DefaultCameraHandler::Update(float deltaTime)
    {
        const float cf = 1 - expf(-mSpringiness * deltaTime);
        const float dxr = cf * mDx;
        const float dyr = cf * mDy;
        mCamera.Rotate(dxr, dyr);
        if(mCamera.mPitch > half_pi<float>())
            mCamera.mPitch = half_pi<float>();
        if(mCamera.mPitch < -half_pi<float>())
            mCamera.mPitch = -half_pi<float>();
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
