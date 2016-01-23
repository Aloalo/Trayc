/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Engine/Game.h>
#include <assert.h>
#include <iostream>

using namespace std;

namespace engine
{
    Game::Game(float timeStep)
        : mUpdateableMenager(timeStep), mRenderer(this), mFrameCap(100), mCtFramesPassed(0), mCameraHandler(nullptr)
    {
    }

    Game::~Game(void)
    {
    }

    void Game::Init(CameraHandler *cameraHandler, char const *programName, const char *windowTitle, int screenWidth, int screenHeight)
    {
        assert(cameraHandler != nullptr);
        mCameraHandler = cameraHandler;

        //Init SDL
        mContextHandler.Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS, programName);

        //Create window
        mContextHandler.CreateGLWindow(
            windowTitle,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            screenWidth,
            screenHeight,
            SDL_WINDOW_OPENGL |
            SDL_WINDOW_RESIZABLE |
            SDL_WINDOW_SHOWN
            );

        //Init OpenGL
        mContextHandler.InitGL(3, 3, SDL_GL_CONTEXT_PROFILE_CORE);

        // Init DevIL
        mContextHandler.InitIL();

        mContextHandler.PrintSoftwareVersions();

        // Register camera
        mInputHandler.AddEventListener(mCameraHandler);
        mUpdateableMenager.AddUpdateable(mCameraHandler);

        // Register self
        mInputHandler.AddEventListener(this);

        mRenderer.InitRendering(mCameraHandler);

        mProfiler.AddProfileTarget("rendering", mFrameCap);
        mProfiler.AddProfileTarget("events", mFrameCap);
        mProfiler.AddProfileTarget("update", mFrameCap);
    }

    void Game::GameLoop()
    {
        while(!mInputHandler.Quit())
            GameLoopStep();
    }

    void Game::GameLoopStep()
    {
        mProfiler.StartClock("events");
        mInputHandler.ProcessPolledEvents();
        mProfiler.StopClock("events");

        mProfiler.StartClock("update");
        mUpdateableMenager.Update();
        mProfiler.StopClock("update");

        mProfiler.StartClock("rendering");
        mRenderer.Render();
        mContextHandler.SwapBuffers();
        mProfiler.StopClock("rendering");

        //Profiling
        ++mCtFramesPassed;
        if(mCtFramesPassed == mFrameCap)
        {
            mCtFramesPassed = 0;
            mProfiler.PrintProfile();
        }
    }

    void Game::WindowEvent(const SDL_WindowEvent &e)
    {
        if(e.event == SDL_WINDOWEVENT_RESIZED) {
            mRenderer.SetScreenSize(e.data1, e.data2);
        }
    }

    const Camera& Game::GetCamera() const
    {
        return mCameraHandler->mCamera;
    }

    Camera& Game::GetCamera()
    {
        return mCameraHandler->mCamera;
    }

    float Game::GetAverageFrameLength() const
    {
        return mProfiler.GetAverage();
    }

}
