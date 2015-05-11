/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Engine.h>

#include "UserSettings.h"
#include "GUIHandler.h"
#include "FunctionRasterizer.h"
#include "FunctionTracer.h"


using namespace engine;
using namespace std;
using namespace glm;

void RenderingLoop()
{
    glDisable(GL_CULL_FACE);
    DefaultCameraHandler camHandler(Camera(vec3(7.0f, 9.2f, -6.0f), (float)UserSettings::Get().screenWidth / UserSettings::Get().screenHeight, UserSettings::Get().FOV), 7.0f, 0.006f);
    EventHandler::AddEventListener(&camHandler);
    EventHandler::AddUpdateable(&camHandler);
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

    GUIHandler guiHandler;
    
    EventHandler::AddEventListener(&guiHandler);

    FunctionDrawer *rasterizer = new FunctionRasterizer();
    FunctionDrawer *tracer = new FunctionTracer();

    guiHandler.CreateTweakBars(&camHandler, rasterizer, tracer);

    while(true)
    {
        glClear(GUIHandler::clearMask);

        guiHandler.currentRenderer->Draw(camHandler.cam);

        TwDraw();

        // Swap buffers
        SDLHandler::SwapBuffers();
        EventHandler::ProcessPolledEvents();
        EventHandler::Update();

        if(EventHandler::Quit())
            break;
    }

    delete tracer;
    delete rasterizer;
}

int main(int argc, char *argv[])
{
    SDLHandler::Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    SDLHandler::CreateGLWindow(
        "Function Drawer",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        UserSettings::Get().screenWidth,
        UserSettings::Get().screenHeight,
        SDL_WINDOW_OPENGL |
        SDL_WINDOW_RESIZABLE |
        SDL_WINDOW_SHOWN           
        );

    SDLHandler::InitGL(3, 3, SDL_GL_CONTEXT_PROFILE_CORE);
    SDLHandler::PrintSoftwareVersions();

    RenderingLoop();

    SDLHandler::CleanUp();

    return 0;
}
