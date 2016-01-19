/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Engine/Scene.h>
#include <assert.h>
#include <iostream>

using namespace std;

namespace engine
{
    Scene::Scene(float timeStep)
        : mUpdateableMenager(timeStep), mRenderer(this), mFrameCap(100), mCtFramesPassed(0), mCameraHandler(nullptr)
    {
    }

    Scene::~Scene(void)
    {
    }

    void Scene::Init(CameraHandler *cameraHandler, char const *programName, const char *windowTitle, int screenWidth, int screenHeight)
    {
        assert(cameraHandler != nullptr);
        mCameraHandler = cameraHandler;

        //Init SDL
        mSDLHandler.Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS, programName);

        //Create window
        mSDLHandler.CreateGLWindow(
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
        mSDLHandler.InitGL(3, 3, SDL_GL_CONTEXT_PROFILE_CORE);

        mSDLHandler.PrintSoftwareVersions();

        //Register camera
        mInputHandler.AddEventListener(mCameraHandler);
        mUpdateableMenager.AddUpdateable(mCameraHandler);

        mRenderer.InitRendering(mCameraHandler);

        mProfiler.AddProfileTarget("rendering", mFrameCap);
        mProfiler.AddProfileTarget("events", mFrameCap);
        mProfiler.AddProfileTarget("update", mFrameCap);
    }

    void Scene::GameLoop()
    {
        while(!mInputHandler.Quit())
            GameLoopStep();
    }

    void Scene::GameLoopStep()
    {
        mProfiler.StartClock("events");
        mInputHandler.ProcessPolledEvents();
        mProfiler.StopClock("events");

        mProfiler.StartClock("update");
        mUpdateableMenager.Update();
        mProfiler.StopClock("update");

        mProfiler.StartClock("rendering");
        mRenderer.Render();
        mSDLHandler.SwapBuffers();
        mProfiler.StopClock("rendering");

        //Profiling
        ++mCtFramesPassed;
        if(mCtFramesPassed == mFrameCap)
        {
            mCtFramesPassed = 0;
            mProfiler.PrintProfile();
        }
    }

    const Camera& Scene::GetCamera() const
    {
        return mCameraHandler->mCamera;
    }

    Camera& Scene::GetCamera()
    {
        return mCameraHandler->mCamera;
    }

    float Scene::GetAverageFrameLength() const
    {
        return mProfiler.GetAverage();
    }

}
