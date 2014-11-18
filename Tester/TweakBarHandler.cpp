/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include "TweakBarHandler.h"
#include <optix_world.h>

#include <optix_math.h>
#include <Trayc/Environment.h>
#include <Trayc/Handlers/OptixTextureHandler.h>
#include <Trayc/Utils.h>
#include <Engine/Core/SDLHandler.h>
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

    //LoadTests();
}

void TW_CALL TweakBarHandler::LoadSponza(void *userData)
{
    gameEngine->tracer.ClearSceneGraph();
    gameEngine->tracer.AddScene(LoadTest(Utils::Resource("crytek-sponza/sponza.obj"), Utils::Resource("crytek-sponza/"), scale(mat4(1.0f), vec3(0.05f))));

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
    const mat4 trnissan = scale(translate(mat4(1.0f), vec3(0.0f, -50.0f, 0.0f)), vec3(0.1f));
    gameEngine->tracer.AddScene(LoadTest(Utils::Resource("nissan/nissan.obj"), Utils::Resource("nissan/"), trnissan), mat.getLabyrinthMaterial(LabMaterials::MIRROR));

    const mat4 teapottr = scale(translate(mat4(1.0f), vec3(50.0f, -54.0f, 0.0f)), vec3(0.1f));
    gameEngine->tracer.AddScene(LoadTest(Utils::Resource("teapot/teapot.obj"), Utils::Resource("teapot/"), teapottr), mat.getLabyrinthMaterial(LabMaterials::GLASS));

    Geometry floor = ctx->createGeometry();
    floor->setPrimitiveCount(1);
    floor->setBoundingBoxProgram(mat.floorAABB);
    floor->setIntersectionProgram(mat.floorIntersect);

    floor["plane_normal"]->setFloat(0.0f, 1.0f, 0.0f);
    floor["recmin"]->setFloat(-80.0f, -59.0f, -80.0f);
    floor["recmax"]->setFloat(80.0f, -59.0f, 80.0f);

    gameEngine->tracer.AddGeometryInstance(ctx->createGeometryInstance(floor, &mat.getLabyrinthMaterial(LabMaterials::WALL), 
        &mat.getLabyrinthMaterial(LabMaterials::WALL)+1));

    gameEngine->tracer.AddLight(BasicLight(//light2 - directional light
        make_float3(1, 1, 1), //pos/dir
        make_float3(0.4f), //color
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
    const vector<Box> &walls = lab.getWalls();
    int n = walls.size();
    for(int i = 0; i < n; ++i)
    {
        Geometry box = ctx->createGeometry();
        box->setPrimitiveCount(1);
        box->setBoundingBoxProgram(mat.boxAABB);
        box->setIntersectionProgram(mat.boxIntersect);
        box["boxmin"]->setFloat(walls[i].boxmin);
        box["boxmax"]->setFloat(walls[i].boxmax);
        gameEngine->tracer.AddGeometryInstance(ctx->createGeometryInstance(box, &mat.getLabyrinthMaterial(walls[i].matIdx), 
            &mat.getLabyrinthMaterial(walls[i].matIdx)+1));
    }

    Geometry floor = ctx->createGeometry();
    floor->setPrimitiveCount(1);
    floor->setBoundingBoxProgram(mat.floorAABB);
    floor->setIntersectionProgram(mat.floorIntersect);

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

const engine::Scene& TweakBarHandler::LoadTest(const string &test, const string &path, const mat4 &transform)
{
    if(tests.find(test) != tests.end())
        return tests[test];

    Assimp::Importer importer;
    const aiScene* aiscene = importer.ReadFile(test, aiProcessPreset_TargetRealtime_MaxQuality);
    if(!aiscene)
    {
        cerr << importer.GetErrorString() << endl;
        exit(-1);
    }
    cout << "Loaded file: " + test << endl;

    tests[test] = Scene();
    Scene &scene = tests[test];
    for(int i = 0; i < aiscene->mNumMeshes; ++i)
    {
        scene.meshes.push_back(TriangleMesh());
        TriangleMesh &mesh = scene.meshes[i];
        const aiMesh *aimesh = aiscene->mMeshes[i];
        const aiMaterial *aimaterial = aiscene->mMaterials[aimesh->mMaterialIndex];
        for(int i = 0; i < aimesh->mNumFaces; ++i)
            mesh.indices.push_back(ivec3(aimesh->mFaces[i].mIndices[0], aimesh->mFaces[i].mIndices[1], aimesh->mFaces[i].mIndices[2]));
        const bool hasNormalMap = aimaterial == nullptr ? false : aimaterial->GetTextureCount(aiTextureType_NORMALS) || aimaterial->GetTextureCount(aiTextureType_HEIGHT);

        for(int i = 0; i < aimesh->mNumVertices; ++i)
        {
            mesh.positions.push_back(vec3(transform * vec4(*(vec3*)(&aimesh->mVertices[i]), 1.0)));
            mesh.normals.push_back(*(vec3*)(&aimesh->mNormals[i]));
            if(hasNormalMap)
            {
                mesh.tangents.push_back(*(vec3*)(&aimesh->mTangents[i]));
                mesh.bitangents.push_back(*(vec3*)(&aimesh->mBitangents[i]));
            }

            if(aimesh->HasTextureCoords(0))
                mesh.uvs.push_back(vec2(aimesh->mTextureCoords[0][i].x, aimesh->mTextureCoords[0][i].y));
            mesh.materialIndex = aimesh->mMaterialIndex;
        }
    }
    for(int i = 0; i < aiscene->mNumMaterials; ++i)
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

        aimaterial->Get(AI_MATKEY_COLOR_REFLECTIVE, color);
        material.reflectivity = *(vec3*)(&color);

        aimaterial->Get(AI_MATKEY_REFRACTI, material.IoR);

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
