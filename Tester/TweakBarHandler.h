/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef TESTER_TWEAKBARHANDLER_H
#define TESTER_TWEAKBARHANDLER_H

#include <AntTweakBar.h>
#include <Trayc/GameEngine.h>
#include "Labyrinth.h"
#include "LabMaterials.h"

class TweakBarHandler
{
public:
    static void CreateTweakBars(trayc::GameEngine *gameEngine);

private:
    static void TW_CALL LoadSponza(void *userData);
    static void TW_CALL LoadNissan(void *userData);
    static void addLabyrinth(const Labyrinth &lab);
    static void TW_CALL LoadLabyrinth(void *userData);

    static void TW_CALL RenderPPM(void *userData);
    
    static void TW_CALL SetFXAA(const void *value, void *clientData);
    static void TW_CALL GetFXAA(void *value, void *clientData);
    static void TW_CALL SetTextureFilter(const void *value, void *clientData);
    static void TW_CALL GetTextureFilter(void *value, void *clientData);
    static void TW_CALL ApplySettings(void *userData);

    static LabMaterials mat;
    static trayc::GameEngine *gameEngine;
    static int labSize;
    //out buffer
    static RTsize bw;
    static RTsize bh;
    //SS buffer
    static RTsize sbw;
    static RTsize sbh;
};

#endif
