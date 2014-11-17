/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include "TweakBarHandler.h"
#include <optix_world.h>
#include <optix_math.h>
#include <Trayc/Environment.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <Engine/Core/SDLHandler.h>

using namespace trayc;
using namespace engine;
using namespace optix;
using namespace std;

GameEngine *TweakBarHandler::gameEngine = nullptr;
int TweakBarHandler::labSize = 15;
LabMaterials TweakBarHandler::mat;
RTsize TweakBarHandler::bw;
RTsize TweakBarHandler::bh;
RTsize TweakBarHandler::sbw;
RTsize TweakBarHandler::sbh;


void TweakBarHandler::CreateTweakBars(GameEngine *gameEngine)
{
    bw = Environment::Get().bufferWidth;
    bh = Environment::Get().bufferHeight;
    sbw = gameEngine->tracer.SSbufferWidth;
    sbh = gameEngine->tracer.SSbufferHeight;
    TweakBarHandler::gameEngine = gameEngine;
    mat.createLabMaterials();

    TwDefine(" GLOBAL help='Trayc library tester.\nPress [w/a/s/d/q/e] to move and use the mouse to rotate the camera.\nPress [shift] to release mouse.\nPress [9/0] to speed up or speed down.' ");

    TwBar *loadbar;
    loadbar = TwNewBar("Tests");
    TwAddButton(loadbar, "Load Nissan", LoadNissan, NULL, " label='Load Nissan' ");
    TwAddButton(loadbar, "Load Sponza", LoadSponza, NULL, " label='Load Sponza' ");
    TwAddButton(loadbar, "Load Labyrinth", LoadLabyrinth, NULL, " label='Load Labyrinth' ");
    TwAddVarRW(loadbar, "Labyrinth Size", TW_TYPE_INT32, &labSize, " label='Labyrinth Size' ");

    TwBar *rtsettings;
    rtsettings = TwNewBar("rtsettings");
    TwDefine(" rtsettings label='Real Time Settings' ");
    TwDefine(" rtsettings iconified=true ");
    TwAddVarCB(rtsettings, "FXAA", TW_TYPE_INT32, SetFXAA, GetFXAA, nullptr, "");
    TwAddVarCB(rtsettings, "Screen texture filtering", TW_TYPE_INT32, SetTextureFilter, GetTextureFilter, nullptr, "");
    TwAddVarRW(rtsettings, "Screen Width", TW_TYPE_INT32, &Environment::Get().screenWidth.x, "");
    TwAddVarRW(rtsettings, "Screen Height", TW_TYPE_INT32, &Environment::Get().screenHeight.x, "");
    TwAddVarRW(rtsettings, "OutBuffer Width", TW_TYPE_INT32, &bw, "");
    TwAddVarRW(rtsettings, "OutBuffer Height", TW_TYPE_INT32, &bh, "");
    TwAddVarRW(rtsettings, "maxRayDepth", TW_TYPE_INT32, &gameEngine->tracer.maxRayDepth.x, "");
    TwAddVarRW(rtsettings, "shadowSamples", TW_TYPE_INT32, &gameEngine->tracer.shadowSamples.x, "");
    TwAddVarRW(rtsettings, "dofSamples", TW_TYPE_INT32, &gameEngine->tracer.dofSamples.x, "");
    TwAddVarRW(rtsettings, "MSAA", TW_TYPE_INT32, &gameEngine->tracer.MSAA.x, "");
    TwAddButton(rtsettings, "Apply", ApplySettings, NULL, " label='Apply' ");

    TwBar *sssettings;
    sssettings = TwNewBar("sssettings");
    TwDefine(" sssettings label='Screenshot Settings' ");
    TwDefine(" sssettings iconified=true ");
    TwAddVarRW(sssettings, "Screenshot Width", TW_TYPE_INT32, &sbw, "");
    TwAddVarRW(sssettings, "Screenshot Height", TW_TYPE_INT32, &sbh, "");
    TwAddVarRW(sssettings, "maxRayDepth", TW_TYPE_INT32, &gameEngine->tracer.SSmaxRayDepth.x, "");
    TwAddVarRW(sssettings, "renderingDivisionLevel", TW_TYPE_INT32, &gameEngine->tracer.SSrenderingDivisionLevel.x, "");
    TwAddVarRW(sssettings, "shadowSamples", TW_TYPE_INT32, &gameEngine->tracer.SSshadowSamples.x, "");
    TwAddVarRW(sssettings, "dofSamples", TW_TYPE_INT32, &gameEngine->tracer.SSdofSamples.x, "");
    TwAddVarRW(sssettings, "MSAA", TW_TYPE_INT32, &gameEngine->tracer.SSMSAA.x, "");
    TwAddButton(sssettings, "High Quality Screenshot", RenderPPM, NULL, " label='High Quality Screenshot' ");

    TwBar *camerasettings;
    camerasettings = TwNewBar("camerasettings");
    TwDefine(" camerasettings label='Camera Settings' ");
    TwDefine(" camerasettings iconified=true ");
    TwAddVarRW(camerasettings, "FOV", TW_TYPE_FLOAT, &gameEngine->FOV.x, "");
    TwAddVarRW(camerasettings, "Aparture Radius", TW_TYPE_FLOAT, &gameEngine->tracer.apertureRadius.x, "");
    TwAddVarRW(camerasettings, "Focal Length", TW_TYPE_FLOAT, &gameEngine->tracer.focalLength.x, "");
    TwAddButton(camerasettings, "Apply", ApplySettings, NULL, " label='Apply' ");
}

void TW_CALL TweakBarHandler::LoadSponza(void *userData)
{
    gameEngine->tracer.ClearSceneGraph();

    try
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(Utils::Resource("crytek-sponza/sponza.obj"), aiProcessPreset_TargetRealtime_MaxQuality);
        if(!scene)
        {
            printf("%s\n", importer.GetErrorString());
            exit(-1);
        }
        gameEngine->tracer.AddScene(Utils::Resource("crytek-sponza/"), scene);
    }
    catch(exception &ex)
    {
        printf("%s\n", ex.what());
        exit(-1);
    }

    gameEngine->tracer.AddLight(BasicLight(//light0 - point light
        make_float3(0.0f, 30.0f, 10.0f), //pos/dir
        make_float3(2.0f), //color
        make_float3(1.0f, 0.01f, 0.0005f),//attenuation
        make_float3(0.0f, 0.0f, 0.0f), //spot_direction
        360.0f, //spot_cutoff
        0.0f, //spot_exponent
        0.25f, //radius
        1, //casts_shadows
        0 //is_directional
        ));

    gameEngine->tracer.CompileSceneGraph();
}

void TW_CALL TweakBarHandler::LoadNissan(void *userData)
{
    gameEngine->tracer.ClearSceneGraph();
    try
    {
        Assimp::Importer importer;

        const aiScene* nissan = importer.ReadFile(Utils::Resource("nissan/nissan.obj"), aiProcessPreset_TargetRealtime_MaxQuality);
        cout << "Loaded file: " + Utils::Resource("nissan/nissan.obj") << endl;
        if(!nissan)
        {
            printf("%s\n", importer.GetErrorString());
            exit(-1);
        }
        gameEngine->tracer.AddScene(mat.getLabyrinthMaterial(LabMaterials::MIRROR), nissan);
        cout << "Imported file: " + Utils::Resource("nissan/nissan.obj") << endl;
    }
    catch(exception &ex)
    {
        printf("%s\n", ex.what());
        exit(-1);
    }

    gameEngine->tracer.AddLight(BasicLight(//light2 - directional light
        make_float3(1, 1, 1), //pos/dir
        make_float3(0.1f), //color
        make_float3(0.0f), //attenuation
        make_float3(0.0f), //spot_direction
        360.0f, //spot_cutoff
        0.0f, //spot_exponent
        0.0f, //radius
        1, //casts_shadows
        1 //is_directional
        ));

    gameEngine->tracer.CompileSceneGraph();
}

void TweakBarHandler::addLabyrinth(const Labyrinth &lab)
{
    string pathFloor = Utils::PathToPTX("rectangleAA.cu");
    string pathBox = Utils::PathToPTX("box.cu");

    optix::Program boxAABB = ctx->createProgramFromPTXFile(pathBox, "box_bounds");
    optix::Program boxIntersect = ctx->createProgramFromPTXFile(pathBox, "box_intersect");

    const vector<Box> &walls = lab.getWalls();
    int n = walls.size();
    for(int i = 0; i < n; ++i)
    {
        Geometry box = ctx->createGeometry();
        box->setPrimitiveCount(1);
        box->setBoundingBoxProgram(boxAABB);
        box->setIntersectionProgram(boxIntersect);
        box["boxmin"]->setFloat(walls[i].boxmin);
        box["boxmax"]->setFloat(walls[i].boxmax);
        gameEngine->tracer.AddGeometryInstance(ctx->createGeometryInstance(box, &mat.getLabyrinthMaterial(walls[i].matIdx), 
            &mat.getLabyrinthMaterial(walls[i].matIdx)+1));
    }

    Geometry floor = ctx->createGeometry();
    floor->setPrimitiveCount(1);
    floor->setBoundingBoxProgram(ctx->createProgramFromPTXFile(pathFloor, "bounds"));
    floor->setIntersectionProgram(ctx->createProgramFromPTXFile(pathFloor, "intersect"));

    float rw = lab.getRealWidth(), rh = lab.getRealHeight();
    floor["plane_normal"]->setFloat(0.0f, 1.0f, 0.0f);
    floor["recmin"]->setFloat(-rw / 2.0f, 0.0f, -rh / 2.0f);
    floor["recmax"]->setFloat(rw / 2.0f, 0.0f, rh / 2.0f);

    gameEngine->tracer.AddGeometryInstance(ctx->createGeometryInstance(floor, &mat.getLabyrinthMaterial(LabMaterials::WALL), 
        &mat.getLabyrinthMaterial(LabMaterials::WALL)+1));
}

void TW_CALL TweakBarHandler::LoadLabyrinth(void *userData)
{
    gameEngine->tracer.ClearSceneGraph();
    Labyrinth lab;
    lab.generateLabyrinth(labSize, labSize);
    TweakBarHandler::addLabyrinth(lab);

    gameEngine->tracer.AddLight(BasicLight(//light0 - point light
        make_float3(0.0f, 30.0f, 10.0f), //pos/dir
        make_float3(2.0f), //color
        make_float3(1.0f, 0.01f, 0.001f),//attenuation
        make_float3(0.0f, 0.0f, 0.0f), //spot_direction
        360.0f, //spot_cutoff
        0.0f, //spot_exponent
        0.25f, //radius
        1, //casts_shadows
        0 //is_directional
        ));

    gameEngine->tracer.CompileSceneGraph();
}

void TW_CALL TweakBarHandler::RenderPPM(void *userData)
{
    gameEngine->tracer.RenderToPPM("screen.ppm");
}

void TW_CALL TweakBarHandler::SetFXAA(const void *value, void *clientData)
{
    gameEngine->bufferDrawer.SetUseFxaa(*static_cast<const int*>(value));
}

void TW_CALL TweakBarHandler::GetFXAA(void *value, void *clientData)
{
    *(int*)(value) = gameEngine->bufferDrawer.useFxaa.x;
}

void TW_CALL TweakBarHandler::SetTextureFilter(const void *value, void *clientData)
{
    if(*static_cast<const int*>(value) == 1)
        gameEngine->bufferDrawer.SetOutBufferTextureFilter(GL_LINEAR);
    else
        gameEngine->bufferDrawer.SetOutBufferTextureFilter(GL_NEAREST);
}

void TW_CALL TweakBarHandler::GetTextureFilter(void *value, void *clientData)
{
    if(gameEngine->bufferDrawer.textureFilter == GL_LINEAR)
        *(int*)(value) = 1;
    else
        *(int*)(value) = 0;
}

void TW_CALL TweakBarHandler::ApplySettings(void *userData)
{
    TwWindowSize(Environment::Get().screenWidth, Environment::Get().screenHeight);
    Environment::Get().bufferWidth = bw;
    Environment::Get().bufferHeight = bh;
    gameEngine->ApplySettings();
}
