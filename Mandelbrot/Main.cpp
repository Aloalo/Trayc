/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Engine.h>
#include <Engine/Core/DefaultCameraHandler.h>
#include "MandelbrotRenderer.h"

using namespace engine;
using namespace glm;

int main(int argc, char *argv[])
{
    const ivec2 SSize(1536, 1024);
    const float timeStep = 1.0f / 60.0f;
    DefaultCameraHandler dummyCam(Camera(), 0.0f, 0.0f, 0.0f);

    //Init Scene
    Game game(timeStep);
    game.Init(&dummyCam, argv[0], "Mandelbrot, Jure Ratkovic KAOS2015, FER", SSize.x, SSize.y);
    game.mContextHandler.VsyncMode(1);
    game.mRenderer.SetClearColor(vec4(0.0f));
    game.mInputHandler.SetCursorFree(true);

    MandelbrotRenderer mandelRenderer(SSize);

    game.mRenderer.AddRenderable(&mandelRenderer);
    game.mInputHandler.AddEventListener(&mandelRenderer);

    game.GameLoop();
    return 0;
}
