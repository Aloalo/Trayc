/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include "GUIHandler.h"
#include "UserSettings.h"
#include <Engine/Core/SDLHandler.h>
#include <fstream>

#include "FunctionRasterizer.h"
#include "FunctionTracer.h"

using namespace std;
using namespace engine;

Scene *GUIHandler::mScene = nullptr;
FunctionDrawer *GUIHandler::mRasterizer = nullptr;
FunctionDrawer *GUIHandler::mTracer = nullptr;
FunctionDrawer *GUIHandler::mCurrentRenderer = nullptr;
float GUIHandler::mFPS = 0.0f;
float GUIHandler::mMiliseconds = 0.0f;

GUIHandler::~GUIHandler(void)
{
    delete mTracer;
    delete mRasterizer;
}

void GUIHandler::CreateTweakBars(Scene *scene)
{
    mScene = scene;
    mTracer = new FunctionTracer();
    mRasterizer = new FunctionRasterizer();
    ApplyFunction(nullptr);

    mCurrentRenderer = mRasterizer;

    glEnable(GL_DEPTH_TEST);

    TwDefine(" GLOBAL help='Two variable function drawer.\n"
        "Press [w/a/s/d/q/e] to move and use the mouse to rotate the camera.\n"
        "Press [LSHIFT] to release mouse.\n"
        "Press [9/0] to speed up or speed down.\n"
        "Press [L] to switch between renderers.' ");

    TwBar *twfunction;
    twfunction = TwNewBar("Function");
    TwDefine(" Function size='250 310' position='10 10' ");
    TwAddVarRW(twfunction, "minX", TW_TYPE_FLOAT, &UserSettings::Get().minX.mValue, "");
    TwAddVarRW(twfunction, "minY", TW_TYPE_FLOAT, &UserSettings::Get().minY.mValue, "");
    TwAddVarRW(twfunction, "maxX", TW_TYPE_FLOAT, &UserSettings::Get().maxX.mValue, "");
    TwAddVarRW(twfunction, "maxY", TW_TYPE_FLOAT, &UserSettings::Get().maxY.mValue, "");

    TwAddSeparator(twfunction, nullptr, " group='Rasterizer' ");
    TwAddVarRW(twfunction, "Mesh resolution", TW_TYPE_INT32, &UserSettings::Get().ctVertices.mValue, "min=10 max=8000 group=Rasterizer");
    TwAddVarRW(twfunction, "Draw Distance", TW_TYPE_FLOAT, &UserSettings::Get().drawDistance.mValue, "min=10 max=30000 group=Rasterizer");
    TwAddButton(twfunction, "ApplyRasterizer", ApplyRasterizer, nullptr, " label='Apply' group=Rasterizer");

    TwAddSeparator(twfunction, nullptr, " group='Tracer' ");
    TwAddVarRW(twfunction, "Sampling interval", TW_TYPE_FLOAT, &UserSettings::Get().Lstep.mValue, "min=0.005 max=0.5 group=Tracer");
    TwAddVarRW(twfunction, "Intersection tolerance", TW_TYPE_FLOAT, &UserSettings::Get().tolerance.mValue, "min=0.0000001 max=0.5 group=Tracer");
    TwAddVarRW(twfunction, "AA", TW_TYPE_INT32, &UserSettings::Get().AAlevel.mValue, "min=1 max=8 group=Tracer");
    TwAddButton(twfunction, "ApplyTracer", ApplyTracer, nullptr, " label='Apply' group=Tracer");


    TwAddButton(twfunction, "ApplyFunction", ApplyFunction, nullptr, " label='Apply Function' ");
    TwAddButton(twfunction, "Switch", SwitchDrawer, nullptr, " label='Switch Drawer [L]' ");

    TwBar *twBar;
    twBar = TwNewBar("Settings");
    TwDefine(" Settings iconified=false size='200 130' position='10 325' ");
    TwAddVarRW(twBar, "Screen Width", TW_TYPE_INT32, &UserSettings::Get().screenWidth.mValue, "min=100 max=1920");
    TwAddVarRW(twBar, "Screen Height", TW_TYPE_INT32, &UserSettings::Get().screenHeight.mValue, "min=100 max=1080");
    TwAddVarRW(twBar, "FOV", TW_TYPE_FLOAT, &UserSettings::Get().FOV.mValue, "min=30 max=120");
    TwAddVarRO(twBar, "FPS", TW_TYPE_FLOAT, &mFPS, "");
    TwAddVarRO(twBar, "ms", TW_TYPE_FLOAT, &mMiliseconds, "");
    TwAddButton(twBar, "Apply", ApplySettings, nullptr, " label='Apply' ");

    ApplySettings(nullptr);
}

void TW_CALL GUIHandler::ApplyTracer(void *userData)
{
    mTracer->SetAABB(mRasterizer->GetAABB());
    mTracer->ApplyFunction();
}

void TW_CALL GUIHandler::ApplyRasterizer(void *userData)
{
    mScene->GetCamera().mFarDistance = UserSettings::Get().drawDistance;
    mRasterizer->ApplyFunction();
}

void TW_CALL GUIHandler::ApplySettings(void *userData)
{
    mScene->GetCamera().mFoV = UserSettings::Get().FOV;
    mScene->GetCamera().mFarDistance = UserSettings::Get().drawDistance;
    mScene->GetCamera().mAspectRatio = float(UserSettings::Get().screenWidth) / float(UserSettings::Get().screenHeight);
    mScene->mSDLHandler.SetWindowSize(UserSettings::Get().screenWidth, UserSettings::Get().screenHeight);

    mTracer->ApplyFunction();
}

void TW_CALL GUIHandler::ApplyFunction(void *userData)
{
    ifstream in("Function.txt");
    string F, Fx, Fy;
    getline(in, F);
    getline(in, Fx);
    getline(in, Fy);
    in.close();

    mRasterizer->SetFunction(F, Fx, Fy);
    mRasterizer->ApplyFunction();

    mTracer->SetAABB(mRasterizer->GetAABB());
    mTracer->SetFunction(F, Fx, Fy);
    mTracer->ApplyFunction();
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

    if(mScene->mInputHandler.IsCursorFree() == false)
        TwEventSDL(&e, mScene->mSDLHandler.GetLinkedVersion().major, mScene->mSDLHandler.GetLinkedVersion().minor);
}

void TW_CALL GUIHandler::SwitchDrawer(void *userData)
{
    if(mCurrentRenderer == mTracer)
    {
        mCurrentRenderer = mRasterizer;
        glEnable(GL_DEPTH_TEST);
    }
    else
    {
        mCurrentRenderer = mTracer;
        glDisable(GL_DEPTH_TEST);
    }
}

void GUIHandler::Draw(const engine::RenderingContext &rContext) const
{
    mCurrentRenderer->Draw(mScene->GetCamera());
    mMiliseconds = mScene->GetAverageFrameLength();
    mFPS = 1000.0f / mMiliseconds;

    TwDraw();
}
