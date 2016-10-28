/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Engine.h>
#include <Engine/Core/DefaultCameraHandler.h>
#include <Engine/Core/RotationalCameraHandler.h>
#include <Engine/Engine/AssetLoader.h>
#include <Engine/Engine/Light.h>
#include <Engine/Core/Defines.h>
#include <Engine/Utils/Setting.h>

#include <glm/gtc/matrix_transform.hpp>

#include "GUIView.h"
#include "LightHandler.h"
#include "PBRMaterialDemo.h"

#if PRODUCTION
#include "DebugView.h"
#endif

using namespace engine;
using namespace std;
using namespace glm;

DefaultCameraHandler ConstructDefaultCameraHandler(ivec2 ss, float FOV, float farDist)
{
    const float nearDist = 10.0f;
    const vec3 cameraPos(0.0f, 200.0f, 0.0f);
    const Camera camera(cameraPos, float(ss.x) / float(ss.y), FOV, nearDist, farDist);

    const float moveSpeed = 600.0f;
    const float rotationSpeed = 0.0015f;
    const float springiness = 20.0f;

    return DefaultCameraHandler(camera, moveSpeed, rotationSpeed, springiness);
}

void StartSponza(const char *progName)
{
    const ivec2 SSize(Setting<int>("screenWidth"), Setting<int>("screenHeight"));
    const float timeStep = 1.0f / 60.0f;
    //Init Camera handler
    DefaultCameraHandler camHandler(ConstructDefaultCameraHandler(SSize, Setting<float>("FOV"), 4000.0f));

    //Init Game
    Game game(timeStep);
    game.Init(&camHandler, progName, "EngineTester", SSize.x, SSize.y);
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
    game.mInputHandler.AddEventListener(&lHandler);

    game.GameLoop();
}

RotationalCameraHandler ConstructRotationalCameraHandler(ivec2 ss, float FOV, float farDist, vec3 cameraPos, vec3 lookAtPoint)
{
    const float nearDist = 10.0f;
    const Camera camera(cameraPos, float(ss.x) / float(ss.y), FOV, nearDist, farDist);

    const float rotationSpeed = 0.003f;
    const float springiness = 20.0f;

    return RotationalCameraHandler(camera, lookAtPoint, rotationSpeed, 10.0f, springiness);
}

void StartHead(const char *progName)
{
    // Init scene
    Scene scene = AssetLoader::Get().LoadScene(AssetLoader::Get().ModelPath("Head/"), "Head.obj");
    scene.Scale(80.0f);
    const vec3 X = scene.GetAABB().Center();

    const ivec2 SSize(Setting<int>("screenWidth"), Setting<int>("screenHeight"));
    const float timeStep = 1.0f / 60.0f;
    //Init Camera handler
    RotationalCameraHandler camHandler(ConstructRotationalCameraHandler(SSize, Setting<float>("FOV"), 1000.0f, X + vec3(0.0f, 0.0f, 300.0f), X));

    //Init Game
    Game game(timeStep);
    game.Init(&camHandler, progName, "EngineTester", SSize.x, SSize.y);
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

    // Init Light
    GlobalLight gl(vec3(0.4f), vec3(1.5f), true, vec3(0.0f, 1.0f, 1.0f));
    scene.mLights.push_back(&gl);
    game.mRenderer.SetScene(&scene);

    game.GameLoop();
}

void StartPBR(const char *progName)
{
    const ivec2 SSize(Setting<int>("screenWidth"), Setting<int>("screenHeight"));
    const float timeStep = 1.0f / 60.0f;
    //Init Camera handler
    RotationalCameraHandler camHandler(ConstructRotationalCameraHandler(SSize, Setting<float>("FOV"), 1000.0f, vec3(0.0f, 0.0f, 300.0f), vec3(0.0f)));

    //Init Game
    Game game(timeStep);
    game.Init(&camHandler, progName, "EngineTester", SSize.x, SSize.y);
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

    Scene scene;
    PBRMaterialDemo demo(game, scene);
    game.mInputHandler.AddEventListener(&demo);

    game.GameLoop();
}


int main(int argc, char *argv[])
{
    InitLogging(argc, argv);
    StartHead(argv[0]);
    //StartSponza(argv[0]);
    //StartPBR(argv[0]);
    return 0;
}
