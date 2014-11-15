/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef TRAYC_PLAYER_H
#define TRAYC_PLAYER_H

#include <Engine/Core/Camera.h>
#include <glm/glm.hpp>
#include <Engine/Core/CameraHandler.h>

namespace trayc
{
    class Player
	{
	public:
		Player(engine::CameraHandler &cam);
		Player(const engine::Camera &cam);
		~Player(void);

        void HandleEvent(const SDL_Event &e);

		void Update(float deltaTime);
		const engine::Camera& GetCam() const;

		engine::CameraHandler &cam;
	};
}

#endif
