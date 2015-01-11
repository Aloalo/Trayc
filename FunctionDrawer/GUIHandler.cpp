/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include "GUIHandler.h"
#include "UserSettings.h"
#include <GL/glew.h>
#include <Engine/Core/SDLHandler.h>

using namespace engine;
using namespace std;

CameraHandler *GUIHandler::camera = nullptr;
FunctionDrawer * GUIHandler::fDrawer = nullptr;
string GUIHandler::F = "sin(x) + cos(y)";
string GUIHandler::Fx = "cos(x)";
string GUIHandler::Fy = "-sin(y)";

void GUIHandler::CreateTweakBars(CameraHandler *cam, FunctionDrawer *fd)
{
    camera = cam;
    fDrawer = fd;

    ApplyFunction(nullptr);

    TwDefine(" GLOBAL help='Two variable function drawer.\n"
        "Press [w/a/s/d/q/e] to move and use the mouse to rotate the camera.\n"
        "Press [TAB] to release mouse.\n"
        "Press [9/0] to speed up or speed down.' ");

    TwBar *twfunction;
    twfunction = TwNewBar("Function");
    TwCopyStdStringToClientFunc(CopyStdStringToClient);
    TwAddVarRW(twfunction, "Function", TW_TYPE_STDSTRING, &F, "");
    TwAddVarRW(twfunction, "X Derivative", TW_TYPE_STDSTRING, &Fx, "");
    TwAddVarRW(twfunction, "Y Derivative", TW_TYPE_STDSTRING, &Fy, "");
    TwAddVarRW(twfunction, "Mesh resolution", TW_TYPE_INT32, &UserSettings::Get().ctVertices.x, "min=10 max=5000");
    TwAddButton(twfunction, "Apply", ApplyFunction, NULL, " label='Apply' ");

    TwBar *twBar;
    twBar = TwNewBar("Settings");
    //TwAddVarCB(twBar, "FXAA", TW_TYPE_BOOL8, SetFXAA, GetFXAA, nullptr, "");
    //TwAddVarCB(twBar, "SSAA", TW_TYPE_BOOL8, SetTextureFilter, GetTextureFilter, nullptr, "");
    TwAddVarRW(twBar, "Screen Width", TW_TYPE_INT32, &UserSettings::Get().screenWidth.x, "min=100 max=1920");
    TwAddVarRW(twBar, "Screen Height", TW_TYPE_INT32, &UserSettings::Get().screenHeight.x, "min=100 max=1080");
    TwAddVarRW(twBar, "FOV", TW_TYPE_FLOAT, &UserSettings::Get().FOV.x, "min=30 max=120");
    TwAddButton(twBar, "Apply", ApplySettings, NULL, " label='Apply' ");

    //TwAddVarRO(twBar, "FPS", TW_TYPE_FLOAT, &gameEngine->FPS, "");
}

void TW_CALL GUIHandler::ApplySettings(void *userData)
{
    camera->cam.FoV = UserSettings::Get().FOV;
    SDLHandler::SetWindowSize(UserSettings::Get().screenWidth, UserSettings::Get().screenHeight);
}

void GUIHandler::HandleEvent(const SDL_Event &e)
{
    if(e.type == SDL_WINDOWEVENT && (e.window.event == SDL_WINDOWEVENT_RESIZED))
    {
        UserSettings::Get().screenWidth = e.window.data1;
        UserSettings::Get().screenHeight = e.window.data2;
        glViewport(0, 0, UserSettings::Get().screenWidth, UserSettings::Get().screenHeight);
    }
}

void TW_CALL GUIHandler::ApplyFunction(void *userData)
{
    fDrawer->SetFunction(F);
    fDrawer->SetXDerivative(Fx);
    fDrawer->SetYDerivative(Fy);
    fDrawer->GenerateIndices(UserSettings::Get().ctVertices);
    fDrawer->GenerateMesh(UserSettings::Get().ctVertices);
}

void TW_CALL GUIHandler::CopyStdStringToClient(std::string& destinationClientString, const std::string& sourceLibraryString)
{
    destinationClientString = sourceLibraryString;
}

