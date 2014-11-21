/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef TRAYC_GAMEENGINE_H
#define TRAYC_GAMEENGINE_H

#include <Engine/Core/Updateable.h>
#include <Engine/Core/EventListener.h>
#include <Engine/Core/DefaultCameraHandler.h>
#include <Trayc/OptixTracer.h>
#include <Trayc/BufferDrawer.h>

namespace trayc
{
    class GameEngine :
        public engine::Updateable, public engine::EventListener
    {
    public:
        GameEngine(void);

        void HandleEvent(const SDL_Event &e);
        void Update(float deltaTime);

        void ApplySettings();

        void ResizeWindow(int w, int h);
        void ResizeOutBuffer(int w, int h);

        void Init();
        void Draw();

        unsigned int frame;
        //for fps calculation
        int framesPassed;
        float FPS;

        //general
        engine::Setting<float> FOV;
        engine::Setting<bool> frameRandomSeed;

        engine::DefaultCameraHandler player;
        OptixTracer tracer;
        BufferDrawer bufferDrawer;
    };
}

#endif
