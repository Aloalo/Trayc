
#include <Engine/Engine.h>
#include <Engine/Core/Defines.h>
#include <Engine/Core/RotationalCameraHandler.h>
#include <Engine/Core/DefaultCameraHandler.h>
#include <Engine/Utils/Setting.h>
#include <Engine/Engine/RayTraceRenderPass.h>
#include "GUIView.h"
#include "RTRTEditor.h"

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
                RTSphere sphere = { vec4(offsets[i], offsets[j], offsets[k], R), vec4(0.1f, 0.6f, 0.4f, 0.5f), vec2(512.0f, 0.0f) };

                if (i == N-1) {
                    sphere.materialData.y = 0.5f;
                }

                game.mRenderer.mRTPass->mSpheres.push_back(sphere);
            }
        }
    }

    const RTLight light = { vec4(10.0f, 10.0f, 10.0f, 0.3f), vec3(1.0f) };
	game.mRenderer.mRTPass->mLights.push_back(light);

    const float recOffset = offsets[0] - 30.0f * R;
    RTRectangle r1 = { vec4(-50.0f, -50.0f, 50.0f, 50.0f), vec4(0.1f, 0.6f, 0.4f, 0.5f), vec2(128.0f, 0.0f), recOffset, 0 };
    RTRectangle r2 = r1;
    r2.normal = 1;
    RTRectangle r3 = r1;
    r3.normal = 2;

	game.mRenderer.mRTPass->mRectangles.push_back(r1);
    game.mRenderer.mRTPass->mRectangles.push_back(r2);
    game.mRenderer.mRTPass->mRectangles.push_back(r3);

    for (auto r : { r1, r2, r3 }) {
        r.offset *= -1.0f;
        game.mRenderer.mRTPass->mRectangles.push_back(r);
    }

	const RTRectangle reflectiveRect = { vec4(-5.0f, -5.0f, 10.0f, 10.0f), vec4(1.0f), vec2(1024.0f, 1.0f), recOffset + 2.0f, 2 };
	game.mRenderer.mRTPass->mRectangles.push_back(reflectiveRect);

    const RTBox box = { vec4(1.0f, 0.6f, 0.4f, 0.5f), vec3(0.0f, 15.0f, 0.0f), 1.0f, vec3(5.0f, 25.0f, 5.0f), 512.0f };
    game.mRenderer.mRTPass->mBoxes.push_back(box);
}

int main(int argc, char *argv[])
{
    InitLogging(argc, argv, true);
    RayTracedGame game(1.0f / 60.0f);
    Init(game, argv[0]);

    // game.mContextHandler.VsyncMode(0);

    GUIView guiView(&game);
    game.mInputHandler.AddEventListener(&guiView);
    game.mRenderer.AddRenderable(&guiView);

#if PRODUCTION
    RTRTEditor editor(dynamic_cast<RayTraceRenderPass*>(game.mRenderer.GetRenderPass("rtPass")));
    editor.SetLevelFromPass();
    editor.SaveLevel();
    //editor.LoadLevel("default");
    game.mUpdateableMenager.AddUpdateable(&editor);
#else
#endif

    game.GameLoop();
    return 0;
}
