/*
* Copyright (c) 2014 Jure Ratkovic
*/

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    #define NOMINMAX
#endif

#include <iostream>
#include <Engine/Engine.h>

#include <IL/il.h>
#include <IL/ilu.h>
#include <AntTweakBar.h>

#include <Trayc/GameEngine.h>
#include <Trayc/Environment.h>
#include <Trayc/Utils.h>

#include "TweakBarHandler.h"

using namespace engine;
using namespace optix;
using namespace std;
using namespace trayc;

GameEngine gameEngine;
TweakBarHandler twBarHandler;

void RenderingLoop()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    EventHandler::AddEventListener(&gameEngine);
    EventHandler::AddUpdateable(&gameEngine);
    EventHandler::AddEventListener(&twBarHandler);
    EventHandler::AddUpdateable(&twBarHandler);


    do
    {
        glClear(GL_COLOR_BUFFER_BIT);
        gameEngine.Draw();
        TwDraw();

        // Swap buffers
        SDLHandler::SwapBuffers();
        EventHandler::ProcessPolledEvents();
        EventHandler::Update();

    } while(!EventHandler::Quit());

}

int main(int argc, char *argv[])
{
    SDLHandler::Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    Environment::Get().screenWidth;
    SDLHandler::CreateGLWindow(
        "Test",                             // window title
        SDL_WINDOWPOS_UNDEFINED,            // initial x position
        SDL_WINDOWPOS_UNDEFINED,            // initial y position
        Environment::Get().screenWidth,     // width, in pixels
        Environment::Get().screenHeight,    // height, in pixels
        SDL_WINDOW_OPENGL |                 // flags
        SDL_WINDOW_RESIZABLE |
        SDL_WINDOW_SHOWN           
        );

    SDLHandler::InitGL(3, 3, SDL_GL_CONTEXT_PROFILE_CORE);
    SDLHandler::PrintSoftwareVersions();

    ilInit();
    iluInit();

    gameEngine.Init();
    TweakBarHandler::CreateTweakBars(&gameEngine);

    ctx->setExceptionEnabled(RT_EXCEPTION_ALL, false);
    
    RenderingLoop();

    SDLHandler::CleanUp();

    return 0;
}


