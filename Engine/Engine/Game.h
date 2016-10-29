/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_GAME_H
#define EN_GAME_H

#include <Engine/Utils/Profiler.h>
#include <Engine/Engine/Renderer.h>
#include <Engine/Core/UpdateableHandler.h>
#include <Engine/Core/InputHandler.h>
#include <Engine/Core/ContextHandler.h>
#include <Engine/Core/CameraHandler.h>
#include <vector>

namespace engine
{
    class Game : public InputObserver
    {
    public:
        Game(float timeStep);
        ~Game(void);

        // Copies cameraHandler and manages it by itself
        template<class CameraHandlerType>
        void Init(const CameraHandlerType &cameraHandler, char const *programName, const char *windowTitle, int screenWidth, int screenHeight)
        {
            mCameraHandler = new CameraHandlerType(cameraHandler);
            Init(programName, windowTitle, screenWidth, screenHeight);
        }

        void GameLoop();

        virtual void WindowEvent(const SDL_WindowEvent &e) override;

        const Camera& GetCamera() const;
        Camera& GetCamera();

        const CameraHandler* GetCameraHandler() const;
        CameraHandler* GetCameraHandler();

        InputHandler mInputHandler;
        UpdateableHandler mUpdateableMenager;
        ContextHandler mContextHandler;
        Renderer mRenderer;

    private:
        Game(const Game &other);
        Game& operator=(const Game &other);

        void GameLoopStep();
        float GetAverageFrameLength() const;

        void Init(char const *programName, const char *windowTitle, int screenWidth, int screenHeight);
        CameraHandler *mCameraHandler;

        Profiler mProfiler;
        int mFrameCap;
        int mCtFramesPassed; //For profiling
    };
}

#endif
