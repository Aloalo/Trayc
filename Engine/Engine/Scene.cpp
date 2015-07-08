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
        : mUpdateableMenager(timeStep), mCameraHandler(nullptr), mCtFramesPassed(120)
    {
    }

    Scene::~Scene(void)
    {
    }

    void Scene::Init(CameraHandler *cameraHandler, char const *programName)
    {
        assert(cameraHandler != nullptr);
        mCameraHandler = cameraHandler;

        //Init SDL
        mSDLHandler.Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS, programName);

        //Create window
        mSDLHandler.CreateGLWindow(
            "Function Drawer",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            1280, //TODO: settings
            720,
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

        mProfiler.AddProfileTarget("rendering", mCtFramesPassed);
        mProfiler.AddProfileTarget("events", mCtFramesPassed);
        mProfiler.AddProfileTarget("update", mCtFramesPassed);
    }

    void Scene::GameLoop()
    {
        while(!mInputHandler.Quit())
            GameLoopStep();
    }

    void Scene::GameLoopStep()
    {
        mProfiler.StartClock("rendering");
        mRenderer.Render();
        mSDLHandler.SwapBuffers();
        mProfiler.StopClock("rendering");

        mProfiler.StartClock("events");
        mInputHandler.ProcessPolledEvents();
        mProfiler.StopClock("events");

        mProfiler.StartClock("update");
        mUpdateableMenager.Update();
        mProfiler.StopClock("update");

        //Profiling
        mCtFramesPassed--;
        if(!mCtFramesPassed)
        {
            mCtFramesPassed = 120;
            PrintProfile();
        }
    }

    void Scene::PrintProfile() const
    {
        float sum = 0.0f;
        cout << "------PROFILE------" << endl;
        for(const auto &psf : mProfiler.GetAllTargetsAverage())
        {
            cout << psf.first + ": " << psf.second << endl;
            sum += psf.second;
        }
        cout << "NET: " << sum << endl << endl;
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
