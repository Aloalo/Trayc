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

using namespace engine;
using namespace std;
using namespace glm;


RotationalCameraHandler ConstructCameraHandler(int screenWidth, int screenHeight, float FOV)
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

    const float nearDist = 0.1f;
    const float farDist = 2000.0f;
    const vec3 cameraPos(275.0f, 30.0f, 0.0f);

    const Camera camera(cameraPos, float(screenWidth) / float(screenHeight), FOV, nearDist, farDist);

    const float rotSpeed = 0.006f;
    const float zoomSpeed = 6.0f;

    return RotationalCameraHandler(camera, vec3(0.0f), rotSpeed, zoomSpeed);
}

vector<vec3> InitBalls(int ctBalls, float cubeSize, float ballRadius)
{
    random_device rd;
    mt19937 gen(rd());
    const uniform_real_distribution<float> dis(-cubeSize + ballRadius, cubeSize - ballRadius);

    vector<vec3> balls;
    balls.reserve(ctBalls);
    for(int i = 0; i < ctBalls; ++i)
        balls.push_back(vec3(dis(gen), dis(gen), dis(gen)));

    return balls;
}

int main(int argc, char *argv[])
{
    //Simulation params
    const Setting<int> screenWidth("screenWidth");
    const Setting<int> screenHeight("screenHeight");
    const Setting<float> FOV("FOV");
    const Setting<int> Vsync("Vsync");
    const float cubeSize = 80.0f;
    const float ballRadius = 1.0f;
    const int ctBalls = 1 << 14;
    const float timeStep = 1.0f / 100.0f;

    Light light;
    light.mFlag = LightFlag::LIGHT_DIRECTIONAL;
    light.mPosition = normalize(vec3(1.0f));
    light.mAttenuation = vec3(1.0f, 0.0f, 0.0f);
    light.mIntensity = vec3(1.0f);

    //Camera handler
    RotationalCameraHandler camHandler(ConstructCameraHandler(screenWidth, screenHeight, FOV));

    Scene scene(timeStep);
    scene.Init(&camHandler, argv[0], screenWidth, screenHeight);
    scene.mSDLHandler.VsyncMode(Vsync);
    scene.mRenderer.SetClearColor(vec4(0.3f, 0.3f, 0.3f, 1.0f));

    WireCube cube(cubeSize);
    scene.mRenderer.AddRenderable(&cube);

    BallRenderer ballRenderer(ctBalls, cubeSize, ballRadius, screenHeight, FOV, light.mPosition);
    ballRenderer.SetBalls(InitBalls(ctBalls, cubeSize, ballRadius).data());
    scene.mRenderer.AddRenderable(&ballRenderer);

    

    scene.mRenderer.AddLight(light);
    scene.GameLoop();
    return 0;
}
