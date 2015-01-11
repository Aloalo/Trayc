/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef FD_GUIHANDLER_H
#define FD_GUIHANDLER_H

#include <AntTweakBar.h>
#include <Engine/Core/EventListener.h>
#include <Engine/Core/CameraHandler.h>
#include <Engine/Core/Updateable.h>
#include <map>
#include "FunctionDrawer.h"

class GUIHandler : public engine::EventListener
{
public:
    static void CreateTweakBars(engine::CameraHandler *cam, FunctionDrawer *fd);

private:
    static engine::CameraHandler *camera;
    static FunctionDrawer *fDrawer;

    static std::string F;
    static std::string Fx;
    static std::string Fy;

    static void TW_CALL CopyStdStringToClient(std::string& destinationClientString, const std::string& sourceLibraryString);
    static void TW_CALL ApplySettings(void *userData);
    static void TW_CALL ApplyFunction(void *userData);

    void HandleEvent(const SDL_Event &e);
};

#endif
