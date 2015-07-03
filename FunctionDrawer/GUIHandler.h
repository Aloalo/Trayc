/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef FD_GUIHANDLER_H
#define FD_GUIHANDLER_H

#include <AntTweakBar.h>
#include <Engine/Engine/Scene.h>
#include "FunctionDrawer.h"

class GUIHandler : public engine::InputObserver, public engine::Renderable
{
public:
    static void CreateTweakBars(engine::Scene *scene);
    ~GUIHandler(void);

private:
    static float mFPS;
    static float mMiliseconds;

    static void TW_CALL SwitchDrawer(void *userData);
    static engine::Scene *mScene;
    static FunctionDrawer *mCurrentRenderer;
    static FunctionDrawer *mTracer;
    static FunctionDrawer *mRasterizer;

    static void TW_CALL ApplySettings(void *userData);
    static void TW_CALL ApplyFunction(void *userData);
    static void TW_CALL ApplyTracer(void *userData);
    static void TW_CALL ApplyRasterizer(void *userData);

    virtual void Draw(const engine::RenderingContext &rContext) const override;
    virtual void HandleEvent(const SDL_Event &e) override;
};

#endif
