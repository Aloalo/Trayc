/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Trayc/GameEngine.h>
#include <Trayc/Environment.h>
#include <Trayc/Handlers/OptixTextureHandler.h>
#include <Engine/Core/SDLHandler.h>
#include <Engine/Core/EventHandler.h>

using namespace std;
using namespace optix;
using namespace engine;

namespace trayc
{
    GameEngine::GameEngine(void)
        : closeAfterSS(false), frame(0), framesPassed(0), FPS(0.0f),
          SETTING(FOV), SETTING(frameRandomSeed),
          player(Camera(glm::vec3(7.0f, 9.2f, -6.0f), (float)Environment::Get().screenWidth.x / Environment::Get().screenHeight.x, FOV), 7.0f, 0.006f)
    {
    }

    void GameEngine::CleanUp()
    {
        tracer.ClearSceneGraph();
        tracer.matHandler.Clear();
        OptixTextureHandler::Get().CleanUP();
        bufferDrawer.CleanUP();
    }

    void GameEngine::HandleEvent(const SDL_Event &e)
    {
        switch (e.type)
        {
        case SDL_QUIT:
            CleanUp();
            break;
        case SDL_WINDOWEVENT:
            if(e.window.event == SDL_WINDOWEVENT_RESIZED)
            {
                Environment::Get().screenWidth = e.window.data1;
                Environment::Get().screenHeight = e.window.data2;
                glViewport(0, 0, e.window.data1, e.window.data2);
            }
            break;
        //case SDL_MOUSEMOTION:
        //    tracer.SetCameraDir(player.cam);
        //    break;
        }
        player.HandleEvent(e);
    }

    void GameEngine::Update(float deltaTime)
    {
        static float timePassed = 0.0f;
        timePassed += deltaTime;

        if(timePassed > 1.0f)
        {
            FPS = (float) framesPassed / timePassed;
            timePassed = 0.0f;
            framesPassed = 0;
        }

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
            tracer.CompileSceneGraph("", false);
            tracer.SetCamera(player.cam);
        }
        catch(exception &ex)
        {
            cerr << ex.what() << endl;
            EventHandler::SetQuit();
        }
    }

    void GameEngine::Draw()
    {
        ++framesPassed;

        try
        {
            ++frame;
            unsigned int rndSeed = frameRandomSeed ? frame : 1337;
            tracer.Trace(0, Environment::Get().bufferWidth, Environment::Get().bufferHeight, 1, rndSeed);
            bufferDrawer.Draw(tracer.outBuffer);
        }
        catch(exception &ex)
        {
            cerr << ex.what() << endl;
            EventHandler::SetQuit();
        }
    }

    void GameEngine::ApplySettings()
    {
        player.cam.FoV = FOV;
        ResizeWindow(Environment::Get().screenWidth, Environment::Get().screenHeight);
        ResizeOutBuffer(Environment::Get().bufferWidth, Environment::Get().bufferHeight);
        tracer.ApplySettings();
    }

    void GameEngine::ResizeWindow(int w, int h)
    {
        SDLHandler::SetWindowSize(w, h);
        Environment::Get().screenWidth = w;
        Environment::Get().screenHeight = h;
        player.cam.aspectRatio = static_cast<float>(w) / static_cast<float>(h);
        glViewport(0, 0, w, h);
    }

    void GameEngine::ResizeOutBuffer(int w, int h)
    {
        Environment::Get().bufferWidth = w;
        Environment::Get().bufferHeight = h;
        tracer.outBuffer->setSize(w, h);
        tracer.outBuffer->unregisterGLBuffer();
        bufferDrawer.AllocateBuffer(w, h);
        tracer.outBuffer->registerGLBuffer();
    }

    void GameEngine::HighQualitySS(const std::string &name)
    {
        tracer.RenderToPPM(name);
        if(closeAfterSS)
            EventHandler::SetQuit();
    }

}
