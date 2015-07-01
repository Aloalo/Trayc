/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef FD_GUIHANDLER_H
#define FD_GUIHANDLER_H

#include <AntTweakBar.h>
#include <Engine/Core/SDLHandler.h>
#include <Engine/Core/InputObserver.h>
#include <Engine/Core/CameraHandler.h>
#include "FunctionDrawer.h"
#include "TmpHandlers.h"

class GUIHandler : public engine::InputObserver
{
public:
    static void CreateTweakBars(engine::CameraHandler *cam, FunctionDrawer *rasterizer, FunctionDrawer *tracer);
    static FunctionDrawer *mCurrentRenderer;
    static GLbitfield mClearMask;
    static float mFPS;
    static float mMiliseconds;
    static void TW_CALL SwitchDrawer(void *userData);

private:
    static engine::CameraHandler *mCamera;
    static FunctionDrawer *mTracer;
    static FunctionDrawer *mRasterizer;

    static void TW_CALL ApplySettings(void *userData);
    static void TW_CALL ApplyFunction(void *userData);
    static void TW_CALL ApplyTracer(void *userData);
    static void TW_CALL ApplyRasterizer(void *userData);

    void HandleEvent(const SDL_Event &e);
};

#endif
