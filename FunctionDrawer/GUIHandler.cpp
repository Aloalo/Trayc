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
float GUIHandler::FPS = 0.0f;
float GUIHandler::ms = 0.0f;

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
    TwDefine(" Function size='250 310' position='10 10' ");
    TwAddVarRW(twfunction, "minX", TW_TYPE_FLOAT, &UserSettings::Get().minX.x, "");
    TwAddVarRW(twfunction, "minY", TW_TYPE_FLOAT, &UserSettings::Get().minY.x, "");
    TwAddVarRW(twfunction, "maxX", TW_TYPE_FLOAT, &UserSettings::Get().maxX.x, "");
    TwAddVarRW(twfunction, "maxY", TW_TYPE_FLOAT, &UserSettings::Get().maxY.x, "");

    TwAddSeparator(twfunction, nullptr, " group='Rasterizer' ");
    TwAddVarRW(twfunction, "Mesh resolution", TW_TYPE_INT32, &UserSettings::Get().ctVertices.x, "min=10 max=8000 group=Rasterizer");
    TwAddVarRW(twfunction, "Draw Distance", TW_TYPE_FLOAT, &UserSettings::Get().drawDistance.x, "min=10 max=30000 group=Rasterizer");
    TwAddButton(twfunction, "ApplyRasterizer", ApplyRasterizer, nullptr, " label='Apply' group=Rasterizer");

    TwAddSeparator(twfunction, nullptr, " group='Tracer' ");
    TwAddVarRW(twfunction, "Sampling interval", TW_TYPE_FLOAT, &UserSettings::Get().Lstep.x, "min=0.005 max=0.5 group=Tracer");
    TwAddVarRW(twfunction, "Intersection tolerance", TW_TYPE_FLOAT, &UserSettings::Get().tolerance.x, "min=0.0000001 max=0.5 group=Tracer");
    TwAddVarRW(twfunction, "AA", TW_TYPE_INT32, &UserSettings::Get().AAlevel.x, "min=1 max=8 group=Tracer");
    TwAddButton(twfunction, "ApplyTracer", ApplyTracer, nullptr, " label='Apply' group=Tracer");


    TwAddButton(twfunction, "ApplyFunction", ApplyFunction, nullptr, " label='Apply Function' ");
    TwAddButton(twfunction, "Switch", SwitchDrawer, nullptr, " label='Switch Drawer [L]' ");

    TwBar *twBar;
    twBar = TwNewBar("Settings");
    TwDefine(" Settings iconified=false size='200 130' position='10 325' ");
    TwAddVarRW(twBar, "Screen Width", TW_TYPE_INT32, &UserSettings::Get().screenWidth.x, "min=100 max=1920");
    TwAddVarRW(twBar, "Screen Height", TW_TYPE_INT32, &UserSettings::Get().screenHeight.x, "min=100 max=1080");
    TwAddVarRW(twBar, "FOV", TW_TYPE_FLOAT, &UserSettings::Get().FOV.x, "min=30 max=120");
    TwAddVarRO(twBar, "FPS", TW_TYPE_FLOAT, &FPS, "");
    TwAddVarRO(twBar, "ms", TW_TYPE_FLOAT, &ms, "");
    TwAddButton(twBar, "Apply", ApplySettings, nullptr, " label='Apply' ");

    ApplySettings(nullptr);
}

void TW_CALL GUIHandler::ApplyTracer(void *userData)
{
    tracer->SetAABB(rasterizer->GetAABB());
    tracer->ApplyFunction();
}

void TW_CALL GUIHandler::ApplyRasterizer(void *userData)
{
    camera->mCamera.mFarDistance = UserSettings::Get().drawDistance;
    rasterizer->ApplyFunction();
}

void TW_CALL GUIHandler::ApplySettings(void *userData)
{
    camera->mCamera.mFoV = UserSettings::Get().FOV;
    camera->mCamera.mFarDistance = UserSettings::Get().drawDistance;
    camera->mCamera.mAspectRatio = float(UserSettings::Get().screenWidth) / float(UserSettings::Get().screenHeight);
    SDLHandler::SetWindowSize(UserSettings::Get().screenWidth, UserSettings::Get().screenHeight);

    tracer->ApplyFunction();
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

    tracer->SetAABB(rasterizer->GetAABB());
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
