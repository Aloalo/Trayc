
#include <Engine/Engine.h>
#include <Engine/Core/RotationalCameraHandler.h>
#include <Engine/Core/DefaultCameraHandler.h>
#include <Engine/Utils/Setting.h>
#include "GUIView.h"

using namespace engine;
using namespace std;
using namespace glm;

DefaultCameraHandler ConstructDefaultCameraHandler(ivec2 ss, float FOV, float farDist)
{
    const float nearDist = 1.0f;
    const vec3 cameraPos(0.0f, 200.0f, 0.0f);
    const Camera camera(cameraPos, float(ss.x) / float(ss.y), FOV, nearDist, farDist);

    const float moveSpeed = 600.0f;
    const float rotationSpeed = 0.0015f;
    const float springiness = 20.0f;

    return DefaultCameraHandler(camera, moveSpeed, rotationSpeed, springiness);
}

RotationalCameraHandler ConstructRotationalCameraHandler(ivec2 ss, float FOV, float farDist, vec3 cameraPos, vec3 lookAtPoint)
{
    const float nearDist = 1.0f;
    const Camera camera(cameraPos, float(ss.x) / float(ss.y), FOV, nearDist, farDist);

    const float rotationSpeed = 0.003f;
    const float springiness = 20.0f;

    return RotationalCameraHandler(camera, lookAtPoint, rotationSpeed, 10.0f, springiness);
}

void Init(RayTracedGame &game, const char *progName)
{
    const ivec2 SSize(Setting<int>("screenWidth"), Setting<int>("screenHeight"));
    
    //Init Camera handler
    const RotationalCameraHandler camHandler(ConstructRotationalCameraHandler(SSize, Setting<float>("FOV"), 1000.0f, vec3(5.0f, 0.0f, 0.0f), vec3(0.0f)));

    //Init Game
    game.Init(camHandler, progName, "RTRTGame", SSize.x, SSize.y);
}

int main(int argc, char *argv[])
{
    InitLogging(argc, argv);
    RayTracedGame game(1.0f / 60.0f);
    Init(game, argv[0]);

    // game.mContextHandler.VsyncMode(0);

    GUIView guiView(&game);
    game.mInputHandler.AddEventListener(&guiView);
    game.mRenderer.AddRenderable(&guiView);

    game.GameLoop();
    return 0;
}
