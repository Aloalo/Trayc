/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Trayc/GameEngine.h>
#include <Trayc/Environment.h>

using namespace std;
using namespace optix;
using namespace engine;

namespace trayc
{
	GameEngine::GameEngine(void)
		: player(Camera(glm::vec3(7.0f, 9.2f, -6.0f), (float)Environment::Get().screenWidth.x / Environment::Get().screenHeight.x, 60.0f), 7.0f, 0.008f),
		mouseLocked(true)
	{
		Environment::Get().ctx = Context::create();
		tracer.Initialize(drawer.CreateGLBuffer());
	}


	GameEngine::~GameEngine(void)
	{
	}

    void GameEngine::HandleEvent(const SDL_Event &e)
    {
        switch (e.type)
        {
        case SDL_WINDOWEVENT:
            if(e.window.event == SDL_WINDOWEVENT_RESIZED)
            {
                Environment::Get().screenWidth = e.window.data1;
                Environment::Get().screenHeight = e.window.data2;
                glViewport(0, 0, e.window.data1, e.window.data2);
            }
        case SDL_KEYDOWN:
            if(e.key.keysym.sym == SDLK_o)
                tracer.RenderToPPM("screen.ppm");
        }
        player.HandleEvent(e);
    }

	/*void GameEngine::mouseClick(const reng::MouseClickEvent &e)
	{
		static float3 color = tracer.getLight(1).color;
		const int idx = 1;
		if(e.state == GLFW_PRESS && e.button == GLFW_MOUSE_BUTTON_RIGHT)
		{
			tracer.getLight(idx).spot_direction = Utils::glmToOptix(player->cam.cam.getDirection());
			tracer.getLight(idx).pos = Utils::glmToOptix(player->cam.getPosition());
			tracer.getLight(idx).color = color;
			tracer.updateLight(idx);
		}
	}*/

	void GameEngine::Update(float deltaTime)
	{
		static float timepassed = 0.0f;
		timepassed += deltaTime;

		tracer.GetLight(0).pos = make_float3(0.f, 30.f, 0.f) + 5.f * make_float3(cosf(timepassed), -sinf(timepassed), -sinf(timepassed));

		tracer.UpdateLight(0);
		player.Update(deltaTime);
		tracer.SetCamera(player.cam);
	}

	void GameEngine::Init()
	{
		try
		{
			tracer.CompileSceneGraph();
			tracer.SetCamera(player.cam);
			drawer.Init(tracer.outBuffer);
		}
		catch(exception &ex)
		{
			puts(ex.what());
            system("pause");
			exit(0);
		}
	}

	void GameEngine::Draw()
	{
		try
		{
			tracer.Trace(0, Environment::Get().bufferWidth, Environment::Get().bufferHeight);
			drawer.Draw(tracer.outBuffer);
		}
		catch(exception &ex)
		{
			puts(ex.what());
            system("pause");
			exit(0);
		}
	}
}
