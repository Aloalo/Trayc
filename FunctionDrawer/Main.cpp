/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Engine.h>
#include <Engine/Utils/FPSCounter.h>

#include "UserSettings.h"
#include "GUIHandler.h"
#include "FunctionRasterizer.h"
#include "FunctionTracer.h"

using namespace engine;
using namespace std;
using namespace glm;

struct TweakBarInputListener : public InputObserver
{
    void HandleEvent(const SDL_Event &e)
    {
        if(inputHandler.IsCursorFree() == false)
            TwEventSDL(&e, sdlHandler.GetLinkedVersion().major, sdlHandler.GetLinkedVersion().minor);
    }
};

void RenderingLoop()
{
    glDisable(GL_CULL_FACE);
    DefaultCameraHandler camHandler(Camera(vec3(7.0f, 9.2f, -6.0f), (float)UserSettings::Get().screenWidth / UserSettings::Get().screenHeight, 
        UserSettings::Get().FOV, 0.1f, UserSettings::Get().drawDistance), 7.0f, 0.006f);

    inputHandler.AddEventListener(&camHandler);
    updateableHandler.AddUpdateable(&camHandler);
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

    GUIHandler guiHandler;
    
    inputHandler.AddEventListener(&guiHandler);

    FunctionDrawer *rasterizer = new FunctionRasterizer();
    FunctionDrawer *tracer = new FunctionTracer();

    guiHandler.CreateTweakBars(&camHandler, rasterizer, tracer);

    FPSCounter fpsCounter(30);

    while(true)
    {
        fpsCounter.StartClock();

        glClear(GUIHandler::mClearMask);

        guiHandler.mCurrentRenderer->Draw(camHandler.mCamera);

        TwDraw();

        // Swap buffers
        sdlHandler.SwapBuffers();
        inputHandler.ProcessPolledEvents();
        updateableHandler.Update();

        if(inputHandler.Quit())
            break;

        fpsCounter.StopClock();
        guiHandler.mFPS = fpsCounter.GetFPS();
        guiHandler.mMiliseconds = 1000.0f / fpsCounter.GetFPS();
    }

    delete tracer;
    delete rasterizer;
}

int main(int argc, char *argv[])
{
    //Init SDL
    sdlHandler.Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS, argv[0]);

    //Create window
    sdlHandler.CreateGLWindow(
        "Function Drawer",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        UserSettings::Get().screenWidth,
        UserSettings::Get().screenHeight,
        SDL_WINDOW_OPENGL |
        SDL_WINDOW_RESIZABLE |
        SDL_WINDOW_SHOWN           
        );

    //Init OpenGL
    const int profile = SDL_GL_CONTEXT_PROFILE_CORE;
    sdlHandler.InitGL(3, 3, profile);

    //Init AntTweakBar
    if(profile == SDL_GL_CONTEXT_PROFILE_CORE)
        TwInit(TW_OPENGL_CORE, nullptr);
    else
        TwInit(TW_OPENGL, nullptr);

    TwWindowSize(UserSettings::Get().screenWidth, UserSettings::Get().screenHeight);
    sdlHandler.PrintSoftwareVersions();

    //Start Rendering
    TweakBarInputListener twInputListener;
    inputHandler.AddEventListener(&twInputListener);
    RenderingLoop();
    inputHandler.RemoveEventListener(&twInputListener);

    TwTerminate();
    sdlHandler.CleanUp();

    return 0;
}
