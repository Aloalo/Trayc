
#include <Engine/Engine.h>
#include <Engine/Core/DefaultCameraHandler.h>
#include <Engine/Core/RotationalCameraHandler.h>
#include <Engine/Engine/AssetLoader.h>
#include <Engine/Engine/Light.h>
#include <Engine/Core/Defines.h>
#include <Engine/Utils/Setting.h>

#include <glm/gtc/matrix_transform.hpp>

#include "GUIView.h"
#include "SponzaDemo.h"
#include "PBRMaterialDemo.h"
#include "HeadDemo.h"

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

RotationalCameraHandler ConstructRotationalCameraHandler(ivec2 ss, float FOV, float farDist, vec3 cameraPos, vec3 lookAtPoint)
{
    const float nearDist = 10.0f;
    const Camera camera(cameraPos, float(ss.x) / float(ss.y), FOV, nearDist, farDist);

    const float rotationSpeed = 0.003f;
    const float springiness = 20.0f;

    return RotationalCameraHandler(camera, lookAtPoint, rotationSpeed, 10.0f, springiness);
}

SponzaDemo sponzaDemo;
void InitSponza(Game &game, Scene &scene, const char *progName, ivec2 SSize)
{
    //Init Camera handler
    DefaultCameraHandler camHandler(ConstructDefaultCameraHandler(SSize, Setting<float>("FOV"), 4000.0f));

    //Init Game
    game.Init(camHandler, progName, "Crytek Sponza Demo", SSize.x, SSize.y);
    game.mRenderer.SetUsePBR(false);

    // Init scene
    scene = AssetLoader::Get().LoadScene(AssetLoader::Get().ModelPath("crytek-sponza/"), "sponza.obj");

    // Init Light
    sponzaDemo.Init(&scene);
    game.mRenderer.SetScene(&scene);
    game.mUpdateableMenager.AddUpdateable(&sponzaDemo);
    game.mInputHandler.AddEventListener(&sponzaDemo);
}

const float headScale = 80.0f;
HeadDemo headDemo(headScale);
void InitHead(Game &game, Scene &scene, const char *progName, ivec2 SSize)
{
    // Init scene
    scene = AssetLoader::Get().LoadScene(AssetLoader::Get().ModelPath("Head/"), "Head.obj");
    scene.Scale(headScale);
    const vec3 X = scene.GetAABB().Center();

    //Init Camera handler
    RotationalCameraHandler camHandler(ConstructRotationalCameraHandler(SSize, Setting<float>("FOV"), 1000.0f, X + vec3(0.0f, 0.0f, 300.0f), X));

    //Init Game
    game.Init(camHandler, progName, "Head Demo", SSize.x, SSize.y);

    headDemo.Init(game, scene);
}

PBRMaterialDemo PBRDemo;
void InitPBR(Game &game, Scene &scene, const char *progName, ivec2 SSize)
{
    //Init Camera handler
    RotationalCameraHandler camHandler(ConstructRotationalCameraHandler(SSize, Setting<float>("FOV"), 1000.0f, vec3(0.0f, 0.0f, 200.0f), vec3(0.0f)));

    //Init Game
    game.Init(camHandler, progName, "PBR Materials Demo", SSize.x, SSize.y);

    // Init demo
    PBRDemo.Init(game, scene);
}

void PrintHelp()
{
    cout << "o - toggle pbr" << endl;

    cout << "Load sponza demo: \"-sponza\"" << endl;
    cout << "   p - pause lights" << endl;
    cout << "Load head demo: \"-head\"" << endl;
    cout << "Load pbr demo: \"-pbr\"" << endl;
    cout << "   arrows - toggle materials" << endl;
    cout << "   right click - rotate ball" << endl;

    cout << endl << endl;
}

int main(int argc, char *argv[])
{
    InitLogging(argc, argv);
    const ivec2 SSize(Setting<int>("screenWidth"), Setting<int>("screenHeight"));
    const float timeStep = 1.0f / 60.0f;

    Game game(timeStep);
    Scene scene;

    PrintHelp();
    if(argc == 1) {
        InitSponza(game, scene, argv[0], SSize);
    }
    else if(string(argv[1]) == "-head") {
        InitHead(game, scene, argv[0], SSize);
    }
    else if(string(argv[1]) == "-sponza") {
        InitSponza(game, scene, argv[0], SSize);
    }
    else if(string(argv[1]) == "-pbr") {
        InitPBR(game, scene, argv[0], SSize);
    }
    else {
        InitSponza(game, scene, argv[0], SSize);
    }

    game.mContextHandler.VsyncMode(Setting<int>("vsync"));
    //game.mContextHandler.VsyncMode(0);

#if PRODUCTION
    //Init DebugView
    CameraHandler *camHandler = game.GetCameraHandler();
    DebugView dView(&game.mRenderer, camHandler->GetCamera().mNearDistance, camHandler->GetCamera().mFarDistance);
    game.mInputHandler.AddEventListener(&dView);
    game.mRenderer.AddRenderable(&dView);
#endif

    //Init GUIView
    GUIView guiView(&game);
    game.mInputHandler.AddEventListener(&guiView);
    game.mRenderer.AddRenderable(&guiView);

    game.GameLoop();
    return 0;
}
