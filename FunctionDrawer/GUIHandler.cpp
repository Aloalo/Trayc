/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include "GUIHandler.h"
#include "UserSettings.h"
#include <Engine/Core/SDLHandler.h>
#include <fstream>

using namespace std;
using namespace engine;

CameraHandler *GUIHandler::camera = nullptr;
FunctionDrawer *GUIHandler::rasterizer = nullptr;
FunctionDrawer *GUIHandler::tracer = nullptr;
FunctionDrawer *GUIHandler::currentRenderer = nullptr;
GLbitfield GUIHandler::clearMask = 0;

void GUIHandler::CreateTweakBars(CameraHandler *cam, FunctionDrawer *rasterizer, FunctionDrawer *tracer)
{
    camera = cam;
    GUIHandler::tracer = tracer;
    GUIHandler::rasterizer = rasterizer;
    ApplyFunction(nullptr);

    currentRenderer = rasterizer;
    glEnable(GL_DEPTH_TEST);
    clearMask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;

    TwDefine(" GLOBAL help='Two variable function drawer.\n"
        "Press [w/a/s/d/q/e] to move and use the mouse to rotate the camera.\n"
        "Press [LSHIFT] to release mouse.\n"
        "Press [9/0] to speed up or speed down.\n"
        "Press [L] to switch between renderers.' ");

    TwBar *twfunction;
    twfunction = TwNewBar("Function");
    TwDefine(" Function size='240 240' ");
    TwAddVarRW(twfunction, "minX", TW_TYPE_FLOAT, &UserSettings::Get().minX.x, "");
    TwAddVarRW(twfunction, "minY", TW_TYPE_FLOAT, &UserSettings::Get().minY.x, "");
    TwAddVarRW(twfunction, "maxX", TW_TYPE_FLOAT, &UserSettings::Get().maxX.x, "");
    TwAddVarRW(twfunction, "maxY", TW_TYPE_FLOAT, &UserSettings::Get().maxY.x, "");

    TwAddSeparator(twfunction, nullptr, " group='Rasterizer' ");
    TwAddVarRW(twfunction, "Mesh resolution", TW_TYPE_INT32, &UserSettings::Get().ctVertices.x, "min=10 max=8000 group=Rasterizer");

    TwAddSeparator(twfunction, nullptr, " group='Tracer' ");

    TwAddButton(twfunction, "Apply", ApplyFunction, nullptr, " label='Apply' ");
    TwAddButton(twfunction, "Switch", SwitchDrawer, nullptr, " label='Switch Drawer [L]' ");

    TwBar *twBar;
    twBar = TwNewBar("Settings");
    TwDefine(" Settings iconified=true ");
    TwAddVarRW(twBar, "Screen Width", TW_TYPE_INT32, &UserSettings::Get().screenWidth.x, "min=100 max=1920");
    TwAddVarRW(twBar, "Screen Height", TW_TYPE_INT32, &UserSettings::Get().screenHeight.x, "min=100 max=1080");
    TwAddVarRW(twBar, "FOV", TW_TYPE_FLOAT, &UserSettings::Get().FOV.x, "min=30 max=120");
    TwAddButton(twBar, "Apply", ApplySettings, nullptr, " label='Apply' ");
}

void TW_CALL GUIHandler::ApplySettings(void *userData)
{
    camera->cam.FoV = UserSettings::Get().FOV;
    SDLHandler::SetWindowSize(UserSettings::Get().screenWidth, UserSettings::Get().screenHeight);
}

void TW_CALL GUIHandler::ApplyFunction(void *userData)
{
    ifstream in("Function.txt");
    string F, Fx, Fy;
    getline(in, F);
    getline(in, Fx);
    getline(in, Fy);
    in.close();

    rasterizer->SetFunction(F, Fx, Fy);
    rasterizer->ApplyFunction();

    tracer->SetFunction(F, Fx, Fy);
    tracer->ApplyFunction();
}

void GUIHandler::HandleEvent(const SDL_Event &e)
{
    if(e.type == SDL_WINDOWEVENT && (e.window.event == SDL_WINDOWEVENT_RESIZED))
    {
        UserSettings::Get().screenWidth = e.window.data1;
        UserSettings::Get().screenHeight = e.window.data2;
        glViewport(0, 0, UserSettings::Get().screenWidth, UserSettings::Get().screenHeight);
    }
    else if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_l)
        SwitchDrawer(nullptr);
}

void TW_CALL GUIHandler::SwitchDrawer(void *userData)
{
    if(currentRenderer == tracer)
    {
        currentRenderer = rasterizer;
        glEnable(GL_DEPTH_TEST);
        clearMask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
    }
    else
    {
        currentRenderer = tracer;
        glDisable(GL_DEPTH_TEST);
        clearMask = GL_COLOR_BUFFER_BIT;
    }
}


