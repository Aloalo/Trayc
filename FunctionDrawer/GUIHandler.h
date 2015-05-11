/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef FD_GUIHANDLER_H
#define FD_GUIHANDLER_H

#include <AntTweakBar.h>
#include <Engine/Core/EventListener.h>
#include <Engine/Core/CameraHandler.h>
#include "FunctionDrawer.h"

class GUIHandler : public engine::EventListener
{
public:
    static void CreateTweakBars(engine::CameraHandler *cam, FunctionDrawer *rasterizer, FunctionDrawer *tracer);
    static FunctionDrawer *currentRenderer;
    static GLbitfield clearMask;
    static void TW_CALL SwitchDrawer(void *userData);

private:
    static engine::CameraHandler *camera;
    static FunctionDrawer *tracer;
    static FunctionDrawer *rasterizer;

    static void TW_CALL ApplySettings(void *userData);
    static void TW_CALL ApplyFunction(void *userData);

    void HandleEvent(const SDL_Event &e);
};

#endif
