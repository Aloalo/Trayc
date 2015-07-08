/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Engine.h>
#include <Engine/Core/RotationalCameraHandler.h>
#include <Engine/Utils/Setting.h>
#include <iostream>
#include "WireCube.h"

using namespace engine;
using namespace std;
using namespace glm;


RotationalCameraHandler ConstructCameraHandler()
{
    const Setting<int> screenWidth("screenWidth");
    const Setting<int> screenHeight("screenHeight");

    if(screenWidth < 100 || screenWidth > 1920 || screenHeight < 100 || screenHeight > 1080)
    {
        cerr << "ERROR: Incorrect screen size, accepted values are 100x100 to 1920x1080." << endl;
        exit(-1);
    }

    const Setting<float> FOV("FOV");

    if(FOV < 30.0f || FOV > 120.0f)
    {
        cerr << "ERROR: Incorrect FOV, accepted values are 30 to 120." << endl;
        exit(-1);
    }

    const float nearDist = 0.1f;
    const float farDist = 1000.0f;
    const vec3 cameraPos(7.0f, 9.2f, -6.0f);

    const Camera camera(cameraPos, float(screenWidth) / float(screenHeight), FOV, nearDist, farDist);

    const float rotSpeed = 0.006f;

    return RotationalCameraHandler(camera, vec3(0.0f), rotSpeed);
}



int main(int argc, char *argv[])
{
    RotationalCameraHandler camHandler(ConstructCameraHandler());

    Scene scene(1.0f / 60.0f);
    scene.Init(&camHandler, argv[0]);
    scene.mRenderer.SetClearColor(vec4(0.3f, 0.3f, 0.3f, 1.0f));

    WireCube cube(2.0f);
    scene.mRenderer.AddRenderable(&cube);
    scene.mRenderer.AddLight(Light());

    scene.GameLoop();
    return 0;
}
