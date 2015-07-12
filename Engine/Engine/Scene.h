/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_SCENE_H
#define EN_SCENE_H

#include <vector>
#include <Engine/Utils/Profiler.h>
#include <Engine/Engine/Renderer.h>
#include <Engine/Core/UpdateableHandler.h>
#include <Engine/Core/InputHandler.h>
#include <Engine/Core/SDLHandler.h>
#include <Engine/Core/CameraHandler.h>

namespace engine
{
    class Scene
    {
    public:
        Scene(float timeStep);
        ~Scene(void);

        void Init(CameraHandler *cameraHandler, char const *programName, int screenWidth, int screenHeight);
        void GameLoop();

        const Camera& GetCamera() const;
        Camera& GetCamera();
        float GetAverageFrameLength() const;

        InputHandler mInputHandler;
        UpdateableHandler mUpdateableMenager;
        Renderer mRenderer;
        SDLHandler mSDLHandler;
        Profiler mProfiler;

    private:
        inline void GameLoopStep();

        const int mFrameCap;
        int mCtFramesPassed; //For profiling
        CameraHandler *mCameraHandler;
    };
}

#endif
