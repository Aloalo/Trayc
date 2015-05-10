/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <iostream>
#include <Engine/Engine.h>
#include <Engine/Utils/Setting.h>
#include <Engine/GL/Program.h>

#include "UserSettings.h"
#include "GUIHandler.h"
#include "FunctionRasterizer.h"
#include "FunctionTracer.h"

#include <AntTweakBar.h>

using namespace engine;
using namespace std;
using namespace glm;

template<class T>
void RenderingLoop(CameraHandler &camHandler, GLbitfield mask)
{
    GUIHandler<T> guiHandler;
    EventHandler::AddEventListener(&guiHandler);
    T functionDrawer;
    guiHandler.CreateTweakBars(&camHandler, &functionDrawer);

    do
    {
        glClear(mask);

        functionDrawer.Draw(camHandler.cam);

        TwDraw();

        // Swap buffers
        SDLHandler::SwapBuffers();
        EventHandler::ProcessPolledEvents();
        EventHandler::Update();

    } while(!EventHandler::Quit());

    functionDrawer.CleanUp();
    EventHandler::RemoveEventListener(&guiHandler);
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

    
    glDisable(GL_CULL_FACE);
    DefaultCameraHandler camHandler(Camera(vec3(7.0f, 9.2f, -6.0f), (float)UserSettings::Get().screenWidth / UserSettings::Get().screenHeight, UserSettings::Get().FOV), 7.0f, 0.006f);
    EventHandler::AddEventListener(&camHandler);
    EventHandler::AddUpdateable(&camHandler);
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

    //glEnable(GL_DEPTH_TEST);
    //RenderingLoop<FunctionRasterizer>(camHandler, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);
    RenderingLoop<FunctionTracer>(camHandler, GL_COLOR_BUFFER_BIT);

    SDLHandler::CleanUp();

    return 0;
}
