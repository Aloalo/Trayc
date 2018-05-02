
#include <Engine/Engine/Game.h>
#include <Engine/Utils/TextureEffects.h>
#include <Engine/Utils/Profiler.h>
#include <assert.h>

using namespace std;

namespace engine
{
    Game::Game(float timeStep, Renderer *renderer)
        : mUpdateableMenager(timeStep), mRendererPtr(renderer), 
#if PROFILE_CPU
        mFrameCap(100), mCtFramesPassed(0),
#endif
        mCameraHandler(nullptr)
    {
    }

    Game::~Game(void)
    {
        if(mCameraHandler) {
            delete mCameraHandler;
        }

        TextureEffects::Get().Destroy();
    }

    void Game::Init(char const *programName, const char *windowTitle, int screenWidth, int screenHeight)
    {
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
        mContextHandler.InitGL(4, 0, SDL_GL_CONTEXT_PROFILE_CORE);

        // Init DevIL
        mContextHandler.InitIL();

        mContextHandler.PrintSoftwareVersions();

        // Register camera
        mInputHandler.AddEventListener(mCameraHandler);
        mUpdateableMenager.AddUpdateable(mCameraHandler);

        // Register self
        mInputHandler.AddEventListener(this);

        // Init effects
        TextureEffects::Get().Init();

        // Init rendering
        mRendererPtr->InitRendering(mCameraHandler);
        mRendererPtr->SetScreenSize(screenWidth, screenHeight);
    }

    void Game::GameLoop()
    {
        while(!mInputHandler.Quit())
            GameLoopStep();
    }

    void Game::GameLoopStep()
    {
        Profiler::Get().StartClock("events");
        mInputHandler.ProcessPolledEvents();
        Profiler::Get().StopClock("events");

        Profiler::Get().StartClock("update");
        mUpdateableMenager.Update();
        Profiler::Get().StopClock("update");

        Profiler::Get().StartClock("rendering");
        mRendererPtr->Render();
        Profiler::Get().StopClock("rendering");

        Profiler::Get().StartClock("swapBuffers");
        mContextHandler.SwapBuffers();
        Profiler::Get().StopClock("swapBuffers");

        //Profiling
#if PROFILE_CPU
        ++mCtFramesPassed;
        if(mCtFramesPassed == mFrameCap)
        {
            mCtFramesPassed = 0;
            Profiler::Get().PrintProfile();
        }
#endif
    }

    void Game::WindowEvent(const SDL_WindowEvent &e)
    {
        if(e.event == SDL_WINDOWEVENT_RESIZED) {
            mCameraHandler->SetAspectRatio(float(e.data1) / float(e.data2));
            mRendererPtr->SetScreenSize(e.data1, e.data2);
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

    const CameraHandler* Game::GetCameraHandler() const
    {
        return mCameraHandler;
    }

    CameraHandler* Game::GetCameraHandler()
    {
        return mCameraHandler;
    }


    RasterizedGame::RasterizedGame(float timeStep)
        : Game(timeStep, &mRenderer)
    {
    }

    RayTracedGame::RayTracedGame(float timeStep)
        : Game(timeStep, &mRenderer)
    {
    }
}
