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

template<class T>
void CheckSetting(T minVal, T maxVal, T val, const string &name)
{
    if(val < minVal || val > maxVal)
    {
        cerr << "ERROR: Incorrect " << name << ", accepted values are " << minVal << " to " << maxVal << "." << endl;
        std::exit(-1);
    }
}

RenderingParams GetRenderingParams()
{
    const Setting<int> screenWidth("screenWidth");
    const Setting<int> screenHeight("screenHeight");
    const Setting<float> FOV("FOV");
    const Setting<bool> Vsync("Vsync");

    CheckSetting(100, 1920, screenWidth.mValue, "screenWidth");
    CheckSetting(100, 1920, screenHeight.mValue, "screenHeight");
    CheckSetting(30.0f, 120.0f, FOV.mValue, "FOV");

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
    ret.mVsync = Vsync;

    return ret;
}

RotationalCameraHandler ConstructCameraHandler(ivec2 ss, float FOV, float cubeSize)
{
    const float nearDist = 0.1f;
    const float farDist = cubeSize * 10.0f;
    const vec3 cameraPos(cubeSize * 2.5f);
    const Camera camera(cameraPos, float(ss.x) / float(ss.y), FOV, nearDist, farDist);

    const float rotSpeed = 0.006f;
    const float zoomSpeed = 6.0f;
    const float springiness = 15.0f;

    return RotationalCameraHandler(camera, vec3(0.0f), rotSpeed, zoomSpeed, springiness);
}

/*
Speed:
    -decrease mCtBalls
    -increase timeStep
    -decrease max number of balls in cell
    -Vsync off
*/

int main(int argc, char *argv[])
{
    RenderingParams rParams = GetRenderingParams();

    const Setting<int> numBalls("numBalls");
    const Setting<float> invTimeStep("invTimeStep");
    CheckSetting(30.0f, 120.0f, invTimeStep.mValue, "invTimeStep");
    CheckSetting(10, 1 << 20, numBalls.mValue, "numBalls");

    //Simulation params
    SimulationParams simParams;
    simParams.mField = nullptr;
    simParams.mCtBalls = numBalls;
    simParams.mBallRadius = 0.25f;
    simParams.mCubeSize = 16.0f;
    simParams.mGlobalDamping = 1.0f;
    simParams.mSpring = 1.0f;
    simParams.mDamping = 0.02f;
    simParams.mShear = 0.0f;
    simParams.mAttraction = 0.0f;
    simParams.mBoundaryDamping = -0.5f;
    const float timeStep = 1.0f / invTimeStep;

    //Init Camera handler
    RotationalCameraHandler camHandler(ConstructCameraHandler(rParams.mSSize, rParams.mFOV, simParams.mCubeSize));
    rParams.mCamera = &camHandler.GetCamera();
    //Init Game
    Game game(timeStep);
    game.Init(&camHandler, argv[0], "Ball physics", rParams.mSSize.x, rParams.mSSize.y);
    game.mContextHandler.VsyncMode(rParams.mVsync);
    game.mRenderer.SetClearColor(vec4(0.3f, 0.3f, 0.3f, 1.0f));

    SimulationHandler simHandler(simParams, rParams);
    simHandler.Init(game);
    game.GameLoop();
    return 0;
}
