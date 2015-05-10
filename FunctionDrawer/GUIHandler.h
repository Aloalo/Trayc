/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef FD_GUIHANDLER_H
#define FD_GUIHANDLER_H

#include <AntTweakBar.h>
#include <Engine/Core/EventListener.h>
#include <Engine/Core/CameraHandler.h>
#include <Engine/Core/Updateable.h>
#include <fstream>

template<class T>
class GUIHandler : public engine::EventListener
{
public:
    template<class T>
    static void CreateTweakBars(engine::CameraHandler *cam, T *fd)
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
        TwAddVarRW(twfunction, "Mesh resolution", TW_TYPE_INT32, &UserSettings::Get().ctVertices.x, "min=10 max=7000");
        TwAddVarRW(twfunction, "X Offset", TW_TYPE_FLOAT, &UserSettings::Get().offsetX.x, "");
        TwAddVarRW(twfunction, "Y Offset", TW_TYPE_FLOAT, &UserSettings::Get().offsetY.x, "");
        TwAddVarRW(twfunction, "X Scale", TW_TYPE_FLOAT, &UserSettings::Get().scaleX.x, "min=1");
        TwAddVarRW(twfunction, "Y Scale", TW_TYPE_FLOAT, &UserSettings::Get().scaleY.x, "min=1");
        TwAddVarRW(twfunction, "Use small data", TW_TYPE_BOOL8, &UserSettings::Get().smallData.x, "");
        TwAddButton(twfunction, "Apply", ApplyFunction, NULL, " label='Apply' ");

        TwBar *twBar;
        twBar = TwNewBar("Settings");
        TwDefine(" Settings iconified=true ");
        TwAddVarRW(twBar, "Screen Width", TW_TYPE_INT32, &UserSettings::Get().screenWidth.x, "min=100 max=1920");
        TwAddVarRW(twBar, "Screen Height", TW_TYPE_INT32, &UserSettings::Get().screenHeight.x, "min=100 max=1080");
        TwAddVarRW(twBar, "FOV", TW_TYPE_FLOAT, &UserSettings::Get().FOV.x, "min=30 max=120");
        TwAddButton(twBar, "Apply", ApplySettings, NULL, " label='Apply' ");
    }

private:
    static engine::CameraHandler *camera;
    static T *fDrawer;

    static void TW_CALL ApplySettings(void *userData)
    {
        camera->cam.FoV = UserSettings::Get().FOV;
        SDLHandler::SetWindowSize(UserSettings::Get().screenWidth, UserSettings::Get().screenHeight);
    }

    static void TW_CALL ApplyFunction(void *userData)
    {
        ifstream in("Function.txt");
        string F, Fx, Fy;
        getline(in, F);
        getline(in, Fx);
        getline(in, Fy);
        in.close();

        fDrawer->SetFunction(F);
        fDrawer->SetXDerivative(Fx);
        fDrawer->SetYDerivative(Fy);
        fDrawer->ApplyFunction();
    }

    void HandleEvent(const SDL_Event &e)
    {
        if(e.type == SDL_WINDOWEVENT && (e.window.event == SDL_WINDOWEVENT_RESIZED))
        {
            UserSettings::Get().screenWidth = e.window.data1;
            UserSettings::Get().screenHeight = e.window.data2;
            glViewport(0, 0, UserSettings::Get().screenWidth, UserSettings::Get().screenHeight);
        }
    }
};

template<class T>
engine::CameraHandler *GUIHandler<T>::camera = nullptr;

template<class T>
T *GUIHandler<T>::fDrawer = nullptr;

#endif
