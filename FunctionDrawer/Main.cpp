/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <iostream>
#include <Engine/Engine.h>
#include <Engine/Utils/Setting.h>
#include <Engine/GL/Program.h>

#include "UserSettings.h"
#include "GUIHandler.h"
#include "FunctionDrawer.h"

#include <AntTweakBar.h>

using namespace engine;
using namespace std;
using namespace glm;

void RenderingLoop()
{
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    DefaultCameraHandler cam(Camera(vec3(7.0f, 9.2f, -6.0f), (float)UserSettings::Get().screenWidth / UserSettings::Get().screenHeight, UserSettings::Get().FOV), 7.0f, 0.006f);
    GUIHandler guiHandler;

    EventHandler::AddEventListener(&cam);
    EventHandler::AddEventListener(&guiHandler);
    EventHandler::AddUpdateable(&cam);

    FunctionDrawer functionDrawer;
    guiHandler.CreateTweakBars(&cam, &functionDrawer);

    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);


    Program p("Shaders/Simple");
    p.SetUniform("ambient", vec3(0.4f, 0.1f, 0.1f));
    p.SetUniform("diffuse", vec3(0.8f, 0.1f, 0.1f));
    p.SetUniform("specular", vec3(1.0f, 1.0f, 1.0f));
    p.SetUniform("shininess", 128.0f);
    p.SetUniform("lightDirection", normalize(vec3(1.0f, 1.0f, 1.0f)));
    p.SetUniform("lightIntensity", vec3(0.9f));
    p.SetUniform("M", I);

    do
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        p.Use();

        const mat4 V = cam.GetViewMatrix();
        GLErrCheck(false);
        p.SetUniform("MVP", cam.GetProjectionMatrix() * V);
        GLErrCheck(false);
        p.SetUniform("invV", inverse(V));
        GLErrCheck(false);
        p.SetUniform("normalMatrix", mat3(1.0f));
        GLErrCheck(false);

        functionDrawer.Draw();

        TwDraw();

        // Swap buffers
        SDLHandler::SwapBuffers();
        EventHandler::ProcessPolledEvents();
        EventHandler::Update();

    } while(!EventHandler::Quit());

    functionDrawer.CleanUp();
    p.Delete();
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


