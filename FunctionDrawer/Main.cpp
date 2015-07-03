/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Engine.h>
#include <Engine/Core/DefaultCameraHandler.h>

#include "UserSettings.h"
#include "GUIHandler.h"

using namespace engine;
using namespace std;
using namespace glm;

struct TweakBarHandler : public InputObserver, public Renderable
{
    TweakBarHandler(const Scene &scene)
        : mScene(scene)
    {}

    ~TweakBarHandler(void)
    {
        TwTerminate();
    }

    virtual void HandleEvent(const SDL_Event &e) override
    {
        if(mScene.mInputHandler.IsCursorFree() == false)
            TwEventSDL(&e, mScene.mSDLHandler.GetLinkedVersion().major, mScene.mSDLHandler.GetLinkedVersion().minor);
    }

    virtual void Draw(const engine::RenderingContext &rContext) const override
    {
        TwDraw();
    }

private:
    const Scene &mScene;
};

void RenderingLoop(char const *programName)
{
    DefaultCameraHandler camHandler(Camera(vec3(7.0f, 9.2f, -6.0f), (float)UserSettings::Get().screenWidth / UserSettings::Get().screenHeight,
        UserSettings::Get().FOV, 0.1f, UserSettings::Get().drawDistance), 7.0f, 0.006f);

    Scene scene(1.0f / 60.0f);

    scene.Init(&camHandler, programName);

    //Init AntTweakBar
    TwInit(TW_OPENGL_CORE, nullptr);

    TwWindowSize(UserSettings::Get().screenWidth, UserSettings::Get().screenHeight);

    scene.mRenderer.SetClearColor(vec4(0.3f, 0.3f, 0.3f, 1.0f));

    GUIHandler guiHandler;
    guiHandler.CreateTweakBars(&scene);
    scene.mInputHandler.AddEventListener(&guiHandler);
    scene.mRenderer.AddRenderable(&guiHandler);

    TweakBarHandler twHandler(scene);
    scene.mInputHandler.AddEventListener(&twHandler);
    scene.mRenderer.AddRenderable(&twHandler);

    scene.mRenderer.AddLight(Light());
    scene.GameLoop();
}

int main(int argc, char *argv[])
{
    RenderingLoop(argv[0]);
    return 0;
}
