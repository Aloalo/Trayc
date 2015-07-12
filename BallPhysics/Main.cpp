/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Engine.h>
#include <Engine/Core/RotationalCameraHandler.h>
#include <Engine/Utils/Setting.h>
#include <iostream>
#include <random>

#include "SimulationHandler.h"

using namespace engine;
using namespace std;
using namespace glm;

RenderingParams GetRenderingParams()
{
    const Setting<int> screenWidth("screenWidth");
    const Setting<int> screenHeight("screenHeight");
    const Setting<float> FOV("FOV");
    const Setting<int> Vsync("Vsync");

    if(screenWidth < 100 || screenWidth > 1920 || screenHeight < 100 || screenHeight > 1080)
    {
        cerr << "ERROR: Incorrect screen size, accepted values are 100x100 to 1920x1080." << endl;
        std::exit(-1);
    }

    if(FOV < 30.0f || FOV > 120.0f)
    {
        cerr << "ERROR: Incorrect FOV, accepted values are 30 to 120." << endl;
        std::exit(-1);
    }

    if(Vsync != 0 && Vsync != 1)
    {
        cerr << "ERROR: Incorrect Vsync, accepted values are 0 or 1." << endl;
        std::exit(-1);
    }

    //Init light
    Light light;
    light.mFlag = LightFlag::LIGHT_DIRECTIONAL;
    light.mPosition = normalize(vec3(1.0f));
    light.mAttenuation = vec3(1.0f, 0.0f, 0.0f);
    light.mIntensity = vec3(1.0f);

    RenderingParams ret;
    ret.mFOV = FOV;
    ret.mSSize = ivec2(screenWidth, screenHeight);
    ret.mLight = light;
    ret.mVsync = bool(Vsync);

    return ret;
}

RotationalCameraHandler ConstructCameraHandler(ivec2 ss, float FOV)
{
    const float nearDist = 0.1f;
    const float farDist = 500.0f;
    const vec3 cameraPos(275.0f, 30.0f, 0.0f);
    const Camera camera(cameraPos, float(ss.x) / float(ss.y), FOV, nearDist, farDist);

    const float rotSpeed = 0.006f;
    const float zoomSpeed = 6.0f;
    const float springiness = 15.0f;

    return RotationalCameraHandler(camera, vec3(0.0f), rotSpeed, zoomSpeed, springiness);
}

int main(int argc, char *argv[])
{
    RenderingParams rParams = GetRenderingParams();

    //Simulation params
    Gravity gravity;
    SimulationParams simParams;
    simParams.mField = &gravity;
    simParams.mCtBalls = 1 << 10;
    simParams.mBallRadius = 1.0f;
    simParams.mCubeSize = 32.0f;
    simParams.mGlobalDamping = 1.0f;
    simParams.mSpring = 0.5f;
    simParams.mDamping = 0.02f;
    simParams.mShear = 0.1f;
    simParams.mAttraction = 0.0f;
    simParams.mBoundaryDamping = -0.5f;
    const float timeStep = 1.0f / 60.0f;

    //Init Camera handler
    RotationalCameraHandler camHandler(ConstructCameraHandler(rParams.mSSize, rParams.mFOV));

    //Init Scene
    Scene scene(timeStep);
    scene.Init(&camHandler, argv[0], rParams.mSSize.x, rParams.mSSize.y);
    scene.mSDLHandler.VsyncMode(rParams.mVsync);
    scene.mRenderer.SetClearColor(vec4(0.3f, 0.3f, 0.3f, 1.0f));

    SimulationHandler simHandler(simParams, rParams);
    simHandler.Init(scene);
    scene.GameLoop();
    return 0;
}
