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
		: player(Camera(glm::vec3(7.0f, 9.2f, -6.0f), (float)Environment::Get().screenWidth.x / Environment::Get().screenHeight.x, 90.0f), 7.0f, 0.008f)
	{
	}


	GameEngine::~GameEngine(void)
	{
	}

    void GameEngine::HandleEvent(const SDL_Event &e)
    {
        switch (e.type)
        {
        case SDL_QUIT:
            OptixTextureHandler::Get().CleanUP();
            bufferDrawer.CleanUP();
            break;
        case SDL_WINDOWEVENT:
            if(e.window.event == SDL_WINDOWEVENT_RESIZED)
            {
                Environment::Get().screenWidth = e.window.data1;
                Environment::Get().screenHeight = e.window.data2;
                glViewport(0, 0, e.window.data1, e.window.data2);
            }
            break;
        }
        player.HandleEvent(e);
    }

	void GameEngine::Update(float deltaTime)
	{
		player.Update(deltaTime);
		tracer.SetCamera(player.cam);
	}

	void GameEngine::Init()
	{
		try
		{
            Environment::Get().ctx = Context::create();
            tracer.Initialize(bufferDrawer.CreateGLBuffer());
            bufferDrawer.Init(tracer.outBuffer->getElementSize());
			tracer.CompileSceneGraph();
			tracer.SetCamera(player.cam);
		}
		catch(exception &ex)
		{
			cerr << ex.what() << endl;
			exit(-1);
		}
	}

	void GameEngine::Draw()
	{
		try
		{
			tracer.Trace(0, Environment::Get().bufferWidth, Environment::Get().bufferHeight);
			bufferDrawer.Draw(tracer.outBuffer);
		}
		catch(exception &ex)
		{
			puts(ex.what());
            system("pause");
			exit(0);
		}
	}
}
