
#include <Engine/Engine.h>
#include <Engine/Core/RotationalCameraHandler.h>
#include <Engine/Core/DefaultCameraHandler.h>
#include <Engine/Utils/Setting.h>
#include "GUIView.h"

using namespace engine;
using namespace std;
using namespace glm;

DefaultCameraHandler ConstructDefaultCameraHandler(ivec2 ss, float FOV, float farDist, vec3 cameraPos)
{
    const float nearDist = 0.01f;
    const Camera camera(cameraPos, float(ss.x) / float(ss.y), FOV, nearDist, farDist);

    const float moveSpeed = 10.0f;
    const float rotationSpeed = 0.0015f;
    const float springiness = 20.0f;

    return DefaultCameraHandler(camera, moveSpeed, rotationSpeed, springiness);
}

RotationalCameraHandler ConstructRotationalCameraHandler(ivec2 ss, float FOV, float farDist, vec3 cameraPos, vec3 lookAtPoint)
{
    const float nearDist = 0.01f;
    const Camera camera(cameraPos, float(ss.x) / float(ss.y), FOV, nearDist, farDist);

    const float rotationSpeed = 0.003f;
    const float springiness = 20.0f;

    return RotationalCameraHandler(camera, lookAtPoint, rotationSpeed, 10.0f, springiness);
}

void Init(RayTracedGame &game, const char *progName)
{
    const ivec2 SSize(Setting<int>("screenWidth"), Setting<int>("screenHeight"));
    
    //Init Camera handler
    //const auto camHandler(ConstructRotationalCameraHandler(SSize, Setting<float>("FOV"), 1000.0f, vec3(5.0f, 0.0f, 0.0f), vec3(0.0f)));
    const auto camHandler(ConstructDefaultCameraHandler(SSize, Setting<float>("FOV"), 1000.0f, vec3(20.0f, 0.0f, 0.0f)));

    //Init Game
    game.Init(camHandler, progName, "RTRTGame", SSize.x, SSize.y);

    const float R = 1.0f;
    const float offsets[] = {-8.0f * R, -4.0f * R , 0.0f, 4.0f * R , 8.0f * R };
    const int N = 5;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            for (int k = 0; k < N; ++k) {
                RTSphere sphere = { vec4(offsets[i], offsets[j], offsets[k], R), vec4(0.1f, 0.6f, 0.4f, 0.5f), vec4(512.0f, 0.0f, 0.0f, 0.0f) };

                if (i == N-1) {
                    sphere.materailData.y = 0.5f;
                }

                game.mRenderer.AddSphere(sphere);
            }
        }
    }

    const RTLight light = { vec4(10.0f, 10.0f, 10.0f, 0.3f), vec3(1.0f), 0 };
    game.mRenderer.AddLight(light);

    const float recOffset = offsets[0] - 20.0f * R;
    const RTRectangle r1 = { vec4(-50.0f, -50.0f, 50.0f, 50.0f), vec4(0.1f, 0.6f, 0.4f, 0.5f), vec2(128.0f, 0.0f), recOffset, 0 };
    RTRectangle r2 = r1;
    r2.offset *= -1;
    game.mRenderer.AddRectangle(r1);
    game.mRenderer.AddRectangle(r2);


    const RTBox box = { vec4(1.0f, 0.6f, 0.4f, 0.5f), vec4(512.0f, 0.0f, 0.0f, 0.0f), vec3(0.0f, 15.0f, 0.0f), 0, vec3(5.0f, 25.0f, 5.0f), 0 };
    game.mRenderer.AddBox(box);
}

int main(int argc, char *argv[])
{
    InitLogging(argc, argv, false);
    RayTracedGame game(1.0f / 60.0f);
    Init(game, argv[0]);

    // game.mContextHandler.VsyncMode(0);

    GUIView guiView(&game);
    game.mInputHandler.AddEventListener(&guiView);
    game.mRenderer.AddRenderable(&guiView);

    game.GameLoop();
    return 0;
}
