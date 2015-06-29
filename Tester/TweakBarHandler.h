/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef TESTER_TWEAKBARHANDLER_H
#define TESTER_TWEAKBARHANDLER_H

#include <AntTweakBar.h>
#include <Trayc/GameEngine.h>
#include <optix_world.h>
#include <Engine/Geometry/Scene.h>
#include <Engine/Core/EventListener.h>
#include <Engine/Core/Updateable.h>
#include <map>
#include "Labyrinth.h"
#include "LabMaterials.h"

class TweakBarHandler : public engine::EventListener, public engine::Updateable
{
public:
    static void CreateTweakBars(trayc::GameEngine *gameEngine);
    static float FPS;

private:
    static const engine::Scene& LoadTest(const std::string &test, const std::string &path, const glm::mat4 &transform);

    static void TW_CALL LoadSponza(void *userData);
    static void TW_CALL LoadCornellBox(void *userData);
    static void TW_CALL LoadNissan(void *userData);
    static void addLabyrinth(const Labyrinth &lab);
    static void TW_CALL LoadMustang(void *userData);
    static void TW_CALL LoadLabyrinth(void *userData);

    static void TW_CALL ScreenShot(void *userData);
    
    static void TW_CALL SetFXAA(const void *value, void *clientData);
    static void TW_CALL GetFXAA(void *value, void *clientData);
    static void TW_CALL SetTextureFilter(const void *value, void *clientData);
    static void TW_CALL GetTextureFilter(void *value, void *clientData);
    static void TW_CALL ApplySettings(void *userData);

    void HandleEvent(const SDL_Event &e);
    void Update(float dt);


    static trayc::GameEngine *gameEngine;

    static LabMaterials mat;
    static int labSize;
    static std::map<std::string, engine::Scene> tests;
    static BasicLight pointlight;
    static optix::float3 mainLightColor;
    static bool inSponza;
    static bool movingLight;

    //out buffer
    static RTsize bw;
    static RTsize bh;
};

#endif
