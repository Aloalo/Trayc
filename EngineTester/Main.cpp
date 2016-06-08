/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Engine.h>
#include <Engine/Core/DefaultCameraHandler.h>
#include <Engine/Engine/AssetLoader.h>
#include <Engine/Engine/Light.h>
#include <Engine/Core/Defines.h>
#include <Engine/Utils/Setting.h>

#include <glm/gtc/matrix_transform.hpp>

#include "GUIView.h"
#include "LightHandler.h"

#if PRODUCTION
#include "DebugView.h"
#endif

using namespace engine;
using namespace std;
using namespace glm;

DefaultCameraHandler ConstructCameraHandler(ivec2 ss, float FOV, float farDist)
{
    const float nearDist = 10.0f;
    const vec3 cameraPos(0.0f, 200.0f, 0.0f);
    const Camera camera(cameraPos, float(ss.x) / float(ss.y), FOV, nearDist, farDist);

    const float moveSpeed = 600.0f;
    const float rotationSpeed = 0.0015f;
    const float springiness = 20.0f;

    return DefaultCameraHandler(camera, moveSpeed, rotationSpeed, springiness);
}

int main(int argc, char *argv[])
{
    InitLogging(argc, argv);

    const ivec2 SSize(Setting<int>("screenWidth"), Setting<int>("screenHeight"));
    const float timeStep = 1.0f / 60.0f;
    //Init Camera handler
    DefaultCameraHandler camHandler(ConstructCameraHandler(SSize, Setting<float>("FOV"), 4000.0f));

    //Init Game
    Game game(timeStep);
    game.Init(&camHandler, argv[0], "EngineTester", SSize.x, SSize.y);
    game.mContextHandler.VsyncMode(Setting<int>("vsync"));

#if PRODUCTION
    //Init DebugView
    DebugView dView(&game.mRenderer, camHandler.GetCamera().mNearDistance, camHandler.GetCamera().mFarDistance);
    game.mInputHandler.AddEventListener(&dView);
    game.mRenderer.AddRenderable(&dView);
#endif

    //Init GUIView
    GUIView guiView(&game);
    game.mInputHandler.AddEventListener(&guiView);
    game.mRenderer.AddRenderable(&guiView);

    // Init scene
    Scene scene = AssetLoader::Get().LoadScene(AssetLoader::Get().ModelPath("crytek-sponza/"), "sponza.obj");

    // Init Light
    LightHandler lHandler(&scene);
    game.mRenderer.SetScene(&scene);
    game.mUpdateableMenager.AddUpdateable(&lHandler);
    
    game.GameLoop();
    return 0;
}
