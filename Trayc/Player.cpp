/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Trayc/Player.h>
#include <Trayc/Environment.h>
#include <Engine/Core/DefaultCameraHandler.h>

using namespace engine;

namespace trayc
{
	Player::Player(CameraHandler &cam)
		: cam(cam)
	{
	}

	Player::Player(const Camera &cam)
		: cam(*(new DefaultCameraHandler(cam, 7.0f, 0.008f)))
	{
	}

	Player::~Player(void)
	{
	}

	const Camera& Player::GetCam() const
	{
		return cam.cam;
	}

    void Player::HandleEvent(const SDL_Event &e)
    {
        if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_9)
            static_cast<DefaultCameraHandler*>(&cam)->speed *= 2.0f;
        else if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_0)
            static_cast<DefaultCameraHandler*>(&cam)->speed *= 0.5f;
        else
            cam.HandleEvent(e);
    }

    void Player::Update(float deltaTime)
    {
        cam.Update(deltaTime);
    }

}
