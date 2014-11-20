/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef TESTER_TWEAKBARHANDLER_H
#define TESTER_TWEAKBARHANDLER_H

#include <AntTweakBar.h>
#include <Trayc/GameEngine.h>
#include <Engine/Geometry/Scene.h>
#include <map>
#include "Labyrinth.h"
#include "LabMaterials.h"

class TweakBarHandler
{
public:
    static void CreateTweakBars(trayc::GameEngine *gameEngine);

private:
    static const engine::Scene& LoadTest(const std::string &test, const std::string &path, const glm::mat4 &transform);

    static void TW_CALL LoadSponza(void *userData);
    static void TW_CALL LoadNissan(void *userData);
    static void addLabyrinth(const Labyrinth &lab);
    static void TW_CALL LoadMustang(void *userData);
    static void TW_CALL LoadSponzaAO(void *userData);
    static void TW_CALL LoadLabyrinth(void *userData);

    static void TW_CALL RenderPPM(void *userData);
    
    static void TW_CALL SetFXAA(const void *value, void *clientData);
    static void TW_CALL GetFXAA(void *value, void *clientData);
    static void TW_CALL SetTextureFilter(const void *value, void *clientData);
    static void TW_CALL GetTextureFilter(void *value, void *clientData);
    static void TW_CALL ApplySettings(void *userData);

    static trayc::GameEngine *gameEngine;

    static LabMaterials mat;
    static int labSize;
    static std::map<std::string, engine::Scene> tests;

    //out buffer
    static RTsize bw;
    static RTsize bh;
};

#endif
