/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include "TweakBarHandler.h"
#include <optix_world.h>
#include <optix_math.h>

#include <Trayc/Environment.h>
#include <Trayc/Handlers/OptixTextureHandler.h>
#include <Trayc/Handlers/ProgramHandler.h>

#include <Engine/Core/SDLHandler.h>
#include <Engine/Core/EventHandler.h>
#include <Engine/Common/MathFunctions.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/gtc/matrix_transform.hpp>

using namespace trayc;
using namespace engine;
using namespace optix;
using namespace std;
using namespace glm;

GameEngine *TweakBarHandler::gameEngine = nullptr;
map<std::string, engine::Scene> TweakBarHandler::tests;
int TweakBarHandler::labSize = 15;
LabMaterials TweakBarHandler::mat;
RTsize TweakBarHandler::bw;
RTsize TweakBarHandler::bh;
BasicLight TweakBarHandler::pointlight = BasicLight(//light0 - point light
                                            make_float3(0.0f, 30.0f, 10.0f), //pos/dir
                                            make_float3(0.0f), //color
                                            make_float3(1.0f, 0.025f, 0.0005f),//attenuation
                                            make_float3(0.0f, 0.0f, 0.0f), //spot_direction
                                            360.0f, //spot_cutoff
                                            0.0f, //spot_exponent
                                            0.1f, //radius
                                            LightFlags::SHADOWS_POINT
                                            );
bool TweakBarHandler::inSponza = false;
bool TweakBarHandler::movingLight = true;
float3 TweakBarHandler::mainLightColor = make_float3(0.0f);


void TweakBarHandler::CreateTweakBars(GameEngine *gameEngine)
{
    bw = Environment::Get().bufferWidth;
    bh = Environment::Get().bufferHeight;
    TweakBarHandler::gameEngine = gameEngine;
    mat.createLabMaterials();

    TwDefine(" GLOBAL help='Trayc library tester.\n"
        "Press [w/a/s/d/q/e] to move and use the mouse to rotate the camera.\n"
        "Press [TAB] to release mouse.\n"
        "Press [9/0] to speed up or speed down.\n"
        "Press left mouse button to spawn a point light and right click to activate/deactivate it.\n"
        "Press [p] to start/stop the moving light in Sponza test\n"
        "Press [l] to turn on/off the scenes main light' ");

    TwBar *loadbar;
    loadbar = TwNewBar("Tests");
    TwAddButton(loadbar, "Load Nissan", LoadNissan, NULL, " label='Load Nissan' ");
    TwAddButton(loadbar, "Load Mustang", LoadMustang, NULL, " label='Load Mustang' ");
    TwAddButton(loadbar, "Load Cornell Box", LoadCornellBox, NULL, " label='Load Cornell Box' ");
    TwAddButton(loadbar, "Load Sponza", LoadSponza, NULL, " label='Load Sponza' ");
    TwAddButton(loadbar, "Load Labyrinth", LoadLabyrinth, NULL, " label='Load Labyrinth' ");
    TwAddVarRW(loadbar, "Labyrinth Size", TW_TYPE_INT32, &labSize, " label='Labyrinth Size' min=2 max=199 ");

    TwBar *rtsettings;
    rtsettings = TwNewBar("rtsettings");
    TwDefine(" rtsettings label='Real Time Settings' ");
    TwDefine(" rtsettings iconified=true ");
    TwAddVarCB(rtsettings, "FXAA", TW_TYPE_BOOL8, SetFXAA, GetFXAA, nullptr, "");
    TwAddVarCB(rtsettings, "Screen texture filtering", TW_TYPE_BOOL8, SetTextureFilter, GetTextureFilter, nullptr, "");
    TwAddVarRW(rtsettings, "Screen Width", TW_TYPE_INT32, &Environment::Get().screenWidth.x, "min=100 max=1920");
    TwAddVarRW(rtsettings, "Screen Height", TW_TYPE_INT32, &Environment::Get().screenHeight.x, "min=100 max=1080");
    TwAddVarRW(rtsettings, "OutBuffer Width", TW_TYPE_INT32, &bw, "min=100 max=3840");
    TwAddVarRW(rtsettings, "OutBuffer Height", TW_TYPE_INT32, &bh, "min=100 max=2160");
    TwAddVarRW(rtsettings, "maxRayDepth", TW_TYPE_INT32, &gameEngine->tracer.maxRayDepth.x, "min=0 max=100");
    TwAddVarRW(rtsettings, "shadowSamples", TW_TYPE_INT32, &gameEngine->tracer.shadowSamples.x, "min=0 max=1024");
    TwAddVarRW(rtsettings, "dofSamples", TW_TYPE_INT32, &gameEngine->tracer.dofSamples.x, "min=0 max=1024");
    TwAddVarRW(rtsettings, "ambientOcclusionSamples", TW_TYPE_INT32, &gameEngine->tracer.ambientOcclusionSamples.x, "min=0 max=1024");
    TwAddVarRW(rtsettings, "MSAA", TW_TYPE_INT32, &gameEngine->tracer.MSAA.x, "min=1 max=16");
    TwAddButton(rtsettings, "Apply", ApplySettings, NULL, " label='Apply' ");

    TwBar *sssettings;
    sssettings = TwNewBar("sssettings");
    TwDefine(" sssettings label='Screenshot Settings' ");
    TwDefine(" sssettings iconified=true ");
    TwAddVarRW(sssettings, "Screenshot Width", TW_TYPE_INT32, &gameEngine->tracer.SSbufferWidth.x, "min=100 max=3840");
    TwAddVarRW(sssettings, "Screenshot Height", TW_TYPE_INT32, &gameEngine->tracer.SSbufferHeight.x, "min=100 max=2160");
    TwAddVarRW(sssettings, "maxRayDepth", TW_TYPE_INT32, &gameEngine->tracer.SSmaxRayDepth.x, "min=0 max=100");
    TwAddVarRW(sssettings, "renderingDivisionLevel", TW_TYPE_INT32, &gameEngine->tracer.SSrenderingDivisionLevel.x, "min=1 max=2160");
    TwAddVarRW(sssettings, "shadowSamples", TW_TYPE_INT32, &gameEngine->tracer.SSshadowSamples.x, "min=0 max=1024");
    TwAddVarRW(sssettings, "dofSamples", TW_TYPE_INT32, &gameEngine->tracer.SSdofSamples.x, "min=0 max=1024");
    TwAddVarRW(sssettings, "ambientOcclusionSamples", TW_TYPE_INT32, &gameEngine->tracer.SSambientOcclusionSamples.x, "min=0 max=1024");
    TwAddVarRW(sssettings, "MSAA", TW_TYPE_INT32, &gameEngine->tracer.SSMSAA.x, "min=1 max=16");
    TwAddVarRW(sssettings, "Close When Done", TW_TYPE_BOOL8, &gameEngine->closeAfterSS, "");
    TwAddButton(sssettings, "High Quality Screenshot", ScreenShot, NULL, " label='High Quality Screenshot' ");

    TwBar *generalsettings;
    generalsettings = TwNewBar("generalsettings");
    TwDefine(" generalsettings label='General Settings' ");
    TwDefine(" generalsettings iconified=true ");
    TwDefine(" generalsettings refresh=0.1 ");
    TwAddVarRW(generalsettings, "FOV", TW_TYPE_FLOAT, &gameEngine->FOV.x, "min=30 max=120");
    TwAddVarRW(generalsettings, "Aparture Radius", TW_TYPE_FLOAT, &gameEngine->tracer.apertureRadius.x, "min=0.005 max=1");
    TwAddVarRW(generalsettings, "Focal Length", TW_TYPE_FLOAT, &gameEngine->tracer.focalLength.x, "min=1");
    TwAddVarRW(generalsettings, "AO Sampling Radius", TW_TYPE_FLOAT, &gameEngine->tracer.AOsamplingRadius.x, "min=0.1");
    TwAddVarRW(generalsettings, "Light Bloom Exponent", TW_TYPE_FLOAT, &gameEngine->tracer.bloomExp.x, "min=1 max=512");
    TwAddVarRW(generalsettings, "Time based random seed", TW_TYPE_BOOL8, &gameEngine->frameRandomSeed.x, "");
    TwAddVarRO(generalsettings, "FPS", TW_TYPE_FLOAT, &gameEngine->FPS, "");
    TwAddButton(generalsettings, "Apply", ApplySettings, NULL, " label='Apply' ");
}

void TW_CALL TweakBarHandler::LoadCornellBox(void *userData)
{
    inSponza = false;
    gameEngine->tracer.ClearSceneGraph();
    const string location = Utils::Resource("CornellBox/");
    gameEngine->tracer.AddScene(LoadTest(location + "/CornellBox-Water.obj", location, I));

    gameEngine->tracer.AddLight(pointlight);
    gameEngine->tracer.AddLight(BasicLight(//light0 - point light
        make_float3(0.0f, 1.0f, -0.9f), //pos/dir
        make_float3(1.0f), //color
        make_float3(1.0f, 0.01f, 0.0005f),//attenuation
        make_float3(0.0f, 0.0f, 0.0f), //spot_direction
        360.0f, //spot_cutoff
        0.0f, //spot_exponent
        0.065f, //radius
        LightFlags::SHADOWS_POINT
        ));

    gameEngine->tracer.CompileSceneGraph(location + "accel.accelcache", true);
}

void TW_CALL TweakBarHandler::LoadSponza(void *userData)
{
    inSponza = true;
    gameEngine->tracer.ClearSceneGraph();
    const string location = Utils::Resource("crytek-sponza/");
    gameEngine->tracer.AddScene(LoadTest(location + "/sponza.obj", location, scale(mat4(1.0f), vec3(0.05f))));

    gameEngine->tracer.AddLight(pointlight);
    gameEngine->tracer.AddLight(BasicLight(//light0 - point light
        make_float3(0.0f, 20.0f, 0.0f), //pos/dir
        make_float3(2.0f), //color
        make_float3(1.0f, 0.01f, 0.0005f),//attenuation
        make_float3(0.0f, 0.0f, 0.0f), //spot_direction
        360.0f, //spot_cutoff
        0.0f, //spot_exponent
        0.25f, //radius
        LightFlags::SHADOWS_POINT
        ));


    gameEngine->tracer.CompileSceneGraph(location + "accel.accelcache", true);
}

void TW_CALL TweakBarHandler::LoadNissan(void *userData)
{
    inSponza = false;
    gameEngine->tracer.ClearSceneGraph();
    const mat4 trnissan = scale(translate(I, vec3(0.0f, -50.0f, 0.0f)), vec3(0.1f));
    gameEngine->tracer.AddScene(LoadTest(Utils::Resource("nissan/nissan.obj"), Utils::Resource("nissan/"), trnissan), mat.getLabyrinthMaterial(LabMaterials::MIRROR));

    const mat4 teapottr = scale(translate(I, vec3(50.0f, -54.0f, 0.0f)), vec3(0.1f));
    gameEngine->tracer.AddScene(LoadTest(Utils::Resource("teapot/teapot.obj"), Utils::Resource("teapot/"), teapottr), mat.getLabyrinthMaterial(LabMaterials::GLASS));

    Geometry floor = ctx->createGeometry();
    floor->setPrimitiveCount(1);
    floor->setBoundingBoxProgram(ProgramHandler::Get().Get("rectangleAA.cu", "bounds"));
    floor->setIntersectionProgram(ProgramHandler::Get().Get("rectangleAA.cu", "intersect"));

    floor["plane_normal"]->setFloat(0.0f, 1.0f, 0.0f);
    floor["recmin"]->setFloat(-80.0f, -59.0f, -80.0f);
    floor["recmax"]->setFloat(80.0f, -59.0f, 80.0f);

    gameEngine->tracer.AddGeometryInstance(ctx->createGeometryInstance(floor, &mat.getLabyrinthMaterial(LabMaterials::WALL), 
        &mat.getLabyrinthMaterial(LabMaterials::WALL)+1));

    gameEngine->tracer.AddLight(pointlight);
    gameEngine->tracer.AddLight(BasicLight(//light2 - directional light
        make_float3(1, 1, 1), //pos/dir
        make_float3(0.4f), //color
        make_float3(0.0f), //attenuation
        make_float3(0.0f), //spot_direction
        360.0f, //spot_cutoff
        0.0f, //spot_exponent
        0.0f, //radius
        LightFlags::SHADOWS_DIRECTIONAL
        ));

    gameEngine->tracer.CompileSceneGraph(Utils::Resource("nissan/") + "accel.accelcache", true);
}

void TW_CALL TweakBarHandler::LoadMustang(void *userData)
{
    inSponza = false;
    gameEngine->tracer.ClearSceneGraph();
    const mat4 trmustang = scale(translate(I, vec3(100.0f, -50.0f, 0.0f)), vec3(0.025f));
    gameEngine->tracer.AddScene(LoadTest(Utils::Resource("mustang/mustang.obj"), Utils::Resource("mustang/"), trmustang), mat.getLabyrinthMaterial(LabMaterials::WALL));
    gameEngine->tracer.AddLight(pointlight);
    gameEngine->tracer.CompileSceneGraph(Utils::Resource("mustang/") + "accel.accelcache", true);
}

void TweakBarHandler::addLabyrinth(const Labyrinth &lab)
{
    const vector<Box> &walls = lab.getWalls();
    int n = walls.size();
    for(int i = 0; i < n; ++i)
    {
        Geometry box = ctx->createGeometry();
        box->setPrimitiveCount(1);
        box->setBoundingBoxProgram(ProgramHandler::Get().Get("box.cu", "bounds"));
        box->setIntersectionProgram(ProgramHandler::Get().Get("box.cu", "intersect"));
        box["boxmin"]->setFloat(walls[i].boxmin);
        box["boxmax"]->setFloat(walls[i].boxmax);
        gameEngine->tracer.AddGeometryInstance(ctx->createGeometryInstance(box, &mat.getLabyrinthMaterial(walls[i].matIdx), 
            &mat.getLabyrinthMaterial(walls[i].matIdx)+1));
    }

    Geometry floor = ctx->createGeometry();
    floor->setPrimitiveCount(1);
    floor->setBoundingBoxProgram(ProgramHandler::Get().Get("rectangleAA.cu", "bounds"));
    floor->setIntersectionProgram(ProgramHandler::Get().Get("rectangleAA.cu", "intersect"));

    float rw = lab.getRealWidth(), rh = lab.getRealHeight();
    floor["plane_normal"]->setFloat(0.0f, 1.0f, 0.0f);
    floor["recmin"]->setFloat(-rw / 2.0f, 0.0f, -rh / 2.0f);
    floor["recmax"]->setFloat(rw / 2.0f, 0.0f, rh / 2.0f);

    gameEngine->tracer.AddGeometryInstance(ctx->createGeometryInstance(floor, &mat.getLabyrinthMaterial(LabMaterials::WALL), 
        &mat.getLabyrinthMaterial(LabMaterials::WALL)+1));
}

void TW_CALL TweakBarHandler::LoadLabyrinth(void *userData)
{
    inSponza = false;
    gameEngine->tracer.ClearSceneGraph();
    Labyrinth lab;
    lab.generateLabyrinth(labSize, labSize);
    TweakBarHandler::addLabyrinth(lab);

    gameEngine->tracer.AddLight(pointlight);
    gameEngine->tracer.AddLight(BasicLight(//light0 - point light
        make_float3(0.0f, 30.0f, 10.0f), //pos/dir
        make_float3(2.0f), //color
        make_float3(1.0f, 0.01f, 0.001f),//attenuation
        make_float3(0.0f, 0.0f, 0.0f), //spot_direction
        360.0f, //spot_cutoff
        0.0f, //spot_exponent
        1.0f, //radius
        LightFlags::SHADOWS_POINT
        ));

    gameEngine->tracer.CompileSceneGraph("", false);
}

void TW_CALL TweakBarHandler::ScreenShot(void *userData)
{
    gameEngine->HighQualitySS("screen.ppm");
}

void TW_CALL TweakBarHandler::SetFXAA(const void *value, void *clientData)
{
    gameEngine->bufferDrawer.SetUseFxaa(*static_cast<const bool*>(value));
}

void TW_CALL TweakBarHandler::GetFXAA(void *value, void *clientData)
{
    *(bool*)(value) = gameEngine->bufferDrawer.useFxaa.x;
}

void TW_CALL TweakBarHandler::SetTextureFilter(const void *value, void *clientData)
{
    if(*static_cast<const bool*>(value) == 1)
        gameEngine->bufferDrawer.SetOutBufferTextureFilter(GL_LINEAR);
    else
        gameEngine->bufferDrawer.SetOutBufferTextureFilter(GL_NEAREST);
}

void TW_CALL TweakBarHandler::GetTextureFilter(void *value, void *clientData)
{
    if(gameEngine->bufferDrawer.textureFilter == GL_LINEAR)
        *(bool*)(value) = 1;
    else
        *(bool*)(value) = 0;
}

void TW_CALL TweakBarHandler::ApplySettings(void *userData)
{
    TwWindowSize(Environment::Get().screenWidth, Environment::Get().screenHeight);
    Environment::Get().bufferWidth = bw;
    Environment::Get().bufferHeight = bh;
    gameEngine->ApplySettings();
}

const engine::Scene& TweakBarHandler::LoadTest(const string &test, const string &path, const mat4 &transform)
{
    if(tests.find(test) != tests.end())
        return tests[test];

    Assimp::Importer importer;
    const aiScene* aiscene = importer.ReadFile(test, aiProcessPreset_TargetRealtime_MaxQuality);
    if(!aiscene)
    {
        cerr << importer.GetErrorString() << endl;
        EventHandler::SetQuit();
        return *(Scene*)nullptr;
    }
    cout << "Loaded file: " + test << endl;

    tests[test] = Scene();
    Scene &scene = tests[test];
    for(optix::uint i = 0; i < aiscene->mNumMeshes; ++i)
    {
        scene.meshes.push_back(TriangleMesh());
        TriangleMesh &mesh = scene.meshes[i];
        const aiMesh *aimesh = aiscene->mMeshes[i];
        const aiMaterial *aimaterial = aiscene->mMaterials[aimesh->mMaterialIndex];
        for(optix::uint j = 0; j < aimesh->mNumFaces; ++j)
            mesh.indices.push_back(ivec3(aimesh->mFaces[j].mIndices[0], aimesh->mFaces[j].mIndices[1], aimesh->mFaces[j].mIndices[2]));
        const bool hasNormalMap = aimaterial == nullptr ? false : aimaterial->GetTextureCount(aiTextureType_NORMALS) || aimaterial->GetTextureCount(aiTextureType_HEIGHT);

        for(optix::uint j = 0; j < aimesh->mNumVertices; ++j)
        {
            mesh.positions.push_back(vec3(transform * vec4(*(vec3*)(&aimesh->mVertices[j]), 1.0)));
            mesh.normals.push_back(*(vec3*)(&aimesh->mNormals[j]));
            if(hasNormalMap)
            {
                mesh.tangents.push_back(*(vec3*)(&aimesh->mTangents[j]));
                mesh.bitangents.push_back(*(vec3*)(&aimesh->mBitangents[j]));
            }

            if(aimesh->HasTextureCoords(0))
                mesh.uvs.push_back(vec2(aimesh->mTextureCoords[0][j].x, aimesh->mTextureCoords[0][j].y));
            mesh.materialIndex = aimesh->mMaterialIndex;
        }
    }
    for(optix::uint i = 0; i < aiscene->mNumMaterials; ++i)
    {
        scene.materials.push_back(engine::Material());
        engine::Material &material = scene.materials[i];
        material.index = i;
        const aiMaterial *aimaterial = aiscene->mMaterials[i];

        aiColor3D color;
        aimaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
        material.Ka = *(vec3*)(&color);

        aimaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
        material.Kd = *(vec3*)(&color);

        aimaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
        material.Ks = *(vec3*)(&color);

        aimaterial->Get(AI_MATKEY_COLOR_EMISSIVE, color);
        material.reflectivity = *(vec3*)(&color);

        aimaterial->Get(AI_MATKEY_REFRACTI, material.IoR);
        aimaterial->Get(AI_MATKEY_SHININESS, material.phongExponent);

        aiString name;
        if(aimaterial->GetTextureCount(aiTextureType_DIFFUSE) != 0)
        {
            aimaterial->GetTexture(aiTextureType_DIFFUSE, 0, &name, NULL, NULL, NULL, NULL, NULL);
            material.diffuse_map = path + string(name.C_Str());
        }
        if(aimaterial->GetTextureCount(aiTextureType_SPECULAR) != 0)
        {
            aimaterial->GetTexture(aiTextureType_SPECULAR, 0, &name, NULL, NULL, NULL, NULL, NULL);
            material.specular_map = path + string(name.C_Str());
        }
        if(aimaterial->GetTextureCount(aiTextureType_NORMALS) != 0)
        {
            aimaterial->GetTexture(aiTextureType_NORMALS, 0, &name, NULL, NULL, NULL, NULL, NULL);
            material.normal_map = path + string(name.C_Str());
        }
        else if(aimaterial->GetTextureCount(aiTextureType_HEIGHT) != 0)
        {
            aimaterial->GetTexture(aiTextureType_HEIGHT, 0, &name, NULL, NULL, NULL, NULL, NULL);
            material.normal_map = path + string(name.C_Str());
        }
    }
    return scene;
}

void TweakBarHandler::HandleEvent(const SDL_Event &e)
{
    if(e.type == SDL_MOUSEBUTTONDOWN)
    {
        if(gameEngine->tracer.GetNumLights() > 0)
        {
            BasicLight &light = gameEngine->tracer.GetLight(0);
            if(e.button.button == SDL_BUTTON_RIGHT)
            {
                if(fmaxf(light.color) == 0.0f)
                    light.color = make_float3(1.0f);
                else
                    light.color = make_float3(0.0f);
            }
            else if(e.button.button == SDL_BUTTON_LEFT)
            {
                const float3 dir = *(float3*)&gameEngine->player.cam.GetDirection();
                light.pos = *(float3*)&gameEngine->player.GetPosition() + dir;
                light.spot_direction = dir;
            }
            gameEngine->tracer.UpdateLight(0);
        }
    }
    else if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_p)
        movingLight = !movingLight;
    else if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_l)
    {
        if(gameEngine->tracer.GetNumLights() > 1)
        {
            BasicLight &light = gameEngine->tracer.GetLight(1);
            if(fmaxf(light.color) == 0.0f)
                light.color = mainLightColor;
            else
            {
                mainLightColor = light.color;
                light.color = make_float3(0.0f);
            }
            gameEngine->tracer.UpdateLight(1);
        }
    }
}

void TweakBarHandler::Update(float dt)
{
    static float time_passed = 0.0f;
    if(inSponza && movingLight && gameEngine->tracer.GetNumLights() > 1)
    {
        time_passed += dt;
        BasicLight &light = gameEngine->tracer.GetLight(1);
        light.pos.x = 22.0f * cosf(time_passed * 0.5f);
        light.pos.y = 16.0f + 15.0f * sinf(time_passed * 0.5f);
        gameEngine->tracer.UpdateLight(1);
    }
}
