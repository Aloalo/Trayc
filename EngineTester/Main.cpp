/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Engine.h>
#include <Engine/Core/DefaultCameraHandler.h>
#include <Engine/Engine/AssetLoader.h>

#include <glm/gtc/matrix_transform.hpp>

#include "GUIView.h"
//#include "DebugView.h"

using namespace engine;
using namespace std;
using namespace glm;

DefaultCameraHandler ConstructCameraHandler(ivec2 ss, float FOV, float farDist)
{
    const float nearDist = 0.1f;
    const vec3 cameraPos(0.0f, 2.0f, 0.0f);
    const Camera camera(cameraPos, float(ss.x) / float(ss.y), FOV, nearDist, farDist);

    const float moveSpeed = 6.0f;
    const float rotationSpeed = 0.0015f;
    const float springiness = 20.0f;

    return DefaultCameraHandler(camera, moveSpeed, rotationSpeed, springiness);
}

int main(int argc, char *argv[])
{
    InitLogging(argc, argv);

    const ivec2 SSize(1280, 720);
    const float timeStep = 1.0f / 60.0f;
    //Init Camera handler
    DefaultCameraHandler camHandler(ConstructCameraHandler(SSize, 60.0f, 5000.0f));

    //Init Game
    Game game(timeStep);
    game.Init(&camHandler, argv[0], "Test", SSize.x, SSize.y);
    game.mContextHandler.VsyncMode(1);
    game.mRenderer.SetClearColor(vec4(0.0f));

    //Init GUIView
    GUIView guiView(&game);
    game.mInputHandler.AddEventListener(&guiView);
    game.mRenderer.AddRenderable(&guiView);

    //Init DebugView
    //DebugView dView(&game);
    //game.mInputHandler.AddEventListener(&dView);
    //game.mRenderer.AddRenderable(&dView);

    Scene scene = AssetLoader::Get().LoadSceneAssimp(AssetLoader::Get().ModelPath("crytek-sponza/"), "sponza.obj", scale(mat4(1.0f), vec3(0.01f)));
    //Scene scene = AssetLoader::Get().LoadSceneAssimp(AssetLoader::Get().ModelPath("cube/"), "cube.obj");
    game.mRenderer.SetScene(&scene);

    game.GameLoop();
    return 0;
}
