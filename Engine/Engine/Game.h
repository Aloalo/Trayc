/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_GAME_H
#define EN_GAME_H

#include <vector>
#include <Engine/Utils/Profiler.h>
#include <Engine/Engine/Renderer.h>
#include <Engine/Core/UpdateableHandler.h>
#include <Engine/Core/InputHandler.h>
#include <Engine/Core/ContextHandler.h>
#include <Engine/Core/CameraHandler.h>

namespace engine
{
    class Game : public InputObserver
    {
    public:
        Game(float timeStep);
        ~Game(void);

        void Init(CameraHandler *cameraHandler, char const *programName, const char *windowTitle, int screenWidth, int screenHeight);
        void GameLoop();

        virtual void WindowEvent(const SDL_WindowEvent &e) override;

        const Camera& GetCamera() const;
        Camera& GetCamera();

        InputHandler mInputHandler;
        UpdateableHandler mUpdateableMenager;
        Profiler mProfiler;
        ContextHandler mContextHandler;
        Renderer mRenderer;

    private:
        void GameLoopStep();
        float GetAverageFrameLength() const;

        const int mFrameCap;
        int mCtFramesPassed; //For profiling
        CameraHandler *mCameraHandler;
    };
}

#endif
