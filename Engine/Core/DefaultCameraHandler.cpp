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
        CameraHandler(cam), speed(speed), rotationSpeed(rotationSpeed), springiness(100), dx(0), dy(0)
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

        const int mod = e.type == SDL_KEYDOWN ? 1 : -1;
        switch(e.keysym.sym)
        {
        case SDLK_w:
            dir += vec3(0, 0, -mod);
            break;
        case SDLK_s:
            dir += vec3(0, 0, mod);
            break;
        case SDLK_a:
            dir += vec3(-mod, 0, 0);
            break;
        case SDLK_d:
            dir += vec3(mod, 0, 0);
            break;
        case SDLK_q:
            dir += vec3(0, -mod, 0);
            break;
        case SDLK_e:
            dir += vec3(0, mod, 0);
            break;
        default:
            break;
        }
    }

    void DefaultCameraHandler::MouseMotion(const SDL_MouseMotionEvent &e)
    {
        int w, h;
        SDLHandler::GetWindowSize(w, h);

        Rotate(static_cast<float>(-e.xrel) * rotationSpeed, static_cast<float>(-e.yrel) * rotationSpeed);
    }

    void DefaultCameraHandler::Rotate(float yaw, float pitch)
    {
        dx += yaw;
        dy += pitch;
    }

    void DefaultCameraHandler::WindowResize(const SDL_WindowEvent &e)
    {
        if(e.windowID == SDL_WINDOWEVENT_RESIZED)
            cam.aspectRatio = static_cast<float>(e.data1) / static_cast<float>(e.data2);
    }

    void DefaultCameraHandler::Update(float deltaTime)
    {
        const float cf = 1 - exp(-springiness * deltaTime);
        const float dxr = cf * dx;
        const float dyr = cf * dy;
        cam.Rotate(dxr, dyr);
        dx -= dxr;
        dy -= dyr;

        if(!(dir.x || dir.y || dir.z))
            return;

        vec3 v(
            dir.x,
            dir.z * sin(cam.phiy),
            dir.z * cos(cam.phiy)
            );
        v = vec3(
            v.x * cos(cam.phix) + v.z * sin(cam.phix),
            v.y,
            -v.x * sin(cam.phix) + v.z * cos(cam.phix)
            );
        cam.position += deltaTime * speed * normalize(-v + vec3(0, dir.y, 0));
    }


    mat4 DefaultCameraHandler::GetProjectionMatrix() const
    {
        return cam.GetProjectionMatrix();
    }

    mat4 DefaultCameraHandler::GetViewMatrix() const
    {
        return cam.GetViewMatrix();
    }
}
