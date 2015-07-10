/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Engine.h>
#include <Engine/Core/RotationalCameraHandler.h>
#include <Engine/Utils/Setting.h>
#include <iostream>
#include <random>

#include "WireCube.h"
#include "BallRenderer.h"

#include "BallPhysics.h"
#include "Solvers/SlowSolver.h"
#include "Solvers/VectorFields.h"

using namespace engine;
using namespace std;
using namespace glm;

void CheckSettings(int screenWidth, int screenHeight, float FOV, int vsync)
{
    if(screenWidth < 100 || screenWidth > 1920 || screenHeight < 100 || screenHeight > 1080)
    {
        cerr << "ERROR: Incorrect screen size, accepted values are 100x100 to 1920x1080." << endl;
        exit(-1);
    }

    if(FOV < 30.0f || FOV > 120.0f)
    {
        cerr << "ERROR: Incorrect FOV, accepted values are 30 to 120." << endl;
        exit(-1);
    }

    if(vsync != 0 && vsync != 1)
    {
        cerr << "ERROR: Incorrect Vsync, accepted values are 0 or 1." << endl;
        exit(-1);
    }
}

RotationalCameraHandler ConstructCameraHandler(int screenWidth, int screenHeight, float FOV)
{
    const float nearDist = 0.1f;
    const float farDist = 500.0f;
    const vec3 cameraPos(275.0f, 30.0f, 0.0f);
    const Camera camera(cameraPos, float(screenWidth) / float(screenHeight), FOV, nearDist, farDist);

    const float rotSpeed = 0.006f;
    const float zoomSpeed = 6.0f;
    const float springiness = 15.0f;

    return RotationalCameraHandler(camera, vec3(0.0f), rotSpeed, zoomSpeed, springiness);
}

int main(int argc, char *argv[])
{
    //Simulation params
    const Setting<int> screenWidth("screenWidth");
    const Setting<int> screenHeight("screenHeight");
    const Setting<float> FOV("FOV");
    const Setting<int> Vsync("Vsync");
    CheckSettings(screenWidth, screenHeight, FOV, Vsync);
    const float cubeSize = 80.0f;
    const float ballRadius = 1.0f;
    const int ctBalls = 1 << 18;
    const float timeStep = 1.0f / 60.0f;
    //Init light
    Light light;
    light.mFlag = LightFlag::LIGHT_DIRECTIONAL;
    light.mPosition = normalize(vec3(1.0f));
    light.mAttenuation = vec3(1.0f, 0.0f, 0.0f);
    light.mIntensity = vec3(1.0f);
    //Init Camera handler
    RotationalCameraHandler camHandler(ConstructCameraHandler(screenWidth, screenHeight, FOV));

    //Init Scene
    Scene scene(timeStep);
    scene.Init(&camHandler, argv[0], screenWidth, screenHeight);
    scene.mSDLHandler.VsyncMode(Vsync);
    scene.mRenderer.SetClearColor(vec4(0.3f, 0.3f, 0.3f, 1.0f));

    //Init ball physics
    Gravity gravity;
    SlowSolver slowSolver(cubeSize, ballRadius);
    slowSolver.SetVectorField(&gravity);
    BallPhysics ballPhysics(&slowSolver, ctBalls, cubeSize, ballRadius);
    ballPhysics.InitBalls();
    //scene.mUpdateableMenager.AddUpdateable(&ballPhysics);

    //Init objects
    WireCube cube(cubeSize);
    scene.mRenderer.AddRenderable(&cube);

    BallRenderer ballRenderer(&ballPhysics, ctBalls, cubeSize, ballRadius, screenHeight, FOV, light.mPosition);
    ballRenderer.SetBalls(ballPhysics.GetBallPositions().data());
    scene.mRenderer.AddRenderable(&ballRenderer);

    scene.mRenderer.AddLight(light);
    scene.GameLoop();
    return 0;
}
