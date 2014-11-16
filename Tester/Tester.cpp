/*
* Copyright (c) 2014 Jure Ratkovic
*/

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    #define NOMINMAX
#endif

#include <iostream>
#include <Engine/Engine.h>

#include <IL/il.h>
#include <IL/ilu.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <optix_math.h>
#include <AntTweakBar.h>

#include <Trayc/GameEngine.h>
#include <Trayc/Environment.h>
#include <Trayc/Utils.h>

#include "Labyrinth.h"
#include "LabMaterials.h"

using namespace engine;
using namespace optix;
using namespace std;
using namespace trayc;

GameEngine gameEngine;
LabMaterials mat;

void TW_CALL LoadSponza(void *userData);
void TW_CALL LoadNissan(void *userData);
void TW_CALL LoadLabyrinth(void *userData);
void TW_CALL RenderPPM(void *userData);

struct BarHandler
{
    BarHandler(void)
        : labSize(15)
    {
    }

    void CreateTweakBars()
    {
        TwBar *loadbar;
        loadbar = TwNewBar("Tests");
        TwAddButton(loadbar, "Load Nissan", LoadNissan, NULL, " label='Load Nissan' ");
        TwAddButton(loadbar, "Load Sponza", LoadSponza, NULL, " label='Load Sponza' ");
        TwAddButton(loadbar, "Load Labyrinth", LoadLabyrinth, NULL, " label='Load Labyrinth' ");
        TwAddButton(loadbar, "High Quality Screen Shot", RenderPPM, NULL, " label='High Quality Screen Shot' ");
        TwAddVarRW(loadbar, "Labyrinth Size", TW_TYPE_INT32, &labSize, " label='Labyrinth Size' ");
    }

    int labSize;
};

BarHandler barHandler;

void RenderingLoop()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    EventHandler::AddEventListener(&gameEngine);
    EventHandler::AddUpdateable(&gameEngine);

    do
    {
        glClear(GL_COLOR_BUFFER_BIT);
        gameEngine.Draw();
        // Swap buffers
        TwDraw();
        SDLHandler::SwapBuffers();
        EventHandler::ProcessPolledEvents();
        EventHandler::Update();

    } while(!EventHandler::Quit());

}

int main(int argc, char *argv[])
{
    SDLHandler::Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    Environment::Get().screenWidth;
    engine::SDLHandler::CreateGLWindow(
        "Test",                             // window title
        SDL_WINDOWPOS_UNDEFINED,            // initial x position
        SDL_WINDOWPOS_UNDEFINED,            // initial y position
        Environment::Get().screenWidth,     // width, in pixels
        Environment::Get().screenHeight,    // height, in pixels
        SDL_WINDOW_OPENGL |                 // flags
        SDL_WINDOW_RESIZABLE |
        SDL_WINDOW_SHOWN           
        );

    SDLHandler::InitGL(3, 3, SDL_GL_CONTEXT_PROFILE_CORE);
    SDLHandler::PrintSoftwareVersions();

    ilInit();
    iluInit();

    barHandler.CreateTweakBars();

    gameEngine.Init();
    ctx->setExceptionEnabled(RT_EXCEPTION_ALL, false);
    mat.createLabMaterials();
    
    RenderingLoop();

    SDLHandler::CleanUp();

	return 0;
}

void TW_CALL LoadSponza(void *userData)
{
    gameEngine.tracer.ClearSceneGraph();

    try
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(Utils::Resource("crytek-sponza/sponza.obj"), aiProcessPreset_TargetRealtime_MaxQuality);
		if(!scene)
		{
		    printf("%s\n", importer.GetErrorString());
		    exit(-1);
		}
		gameEngine.tracer.AddScene(Utils::Resource("crytek-sponza/"), scene);
	}
	catch(exception &ex)
	{
		printf("%s\n", ex.what());
		exit(-1);
	}

	gameEngine.tracer.AddLight(BasicLight(//light0 - point light
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

    gameEngine.tracer.CompileSceneGraph();
}

void TW_CALL LoadNissan(void *userData)
{
    gameEngine.tracer.ClearSceneGraph();
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
		gameEngine.tracer.AddScene(mat.getLabyrinthMaterial(LabMaterials::MIRROR), nissan);
        cout << "Imported file: " + Utils::Resource("nissan/nissan.obj") << endl;
	}
	catch(exception &ex)
	{
		printf("%s\n", ex.what());
		exit(-1);
	}

	gameEngine.tracer.AddLight(BasicLight(//light2 - directional light
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

    gameEngine.tracer.CompileSceneGraph();
}

void addLabyrinth(const Labyrinth &lab)
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
        gameEngine.tracer.AddGeometryInstance(ctx->createGeometryInstance(box, &mat.getLabyrinthMaterial(walls[i].matIdx), 
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

    gameEngine.tracer.AddGeometryInstance(ctx->createGeometryInstance(floor, &mat.getLabyrinthMaterial(LabMaterials::WALL), 
        &mat.getLabyrinthMaterial(LabMaterials::WALL)+1));
}

void TW_CALL LoadLabyrinth(void *userData)
{
    gameEngine.tracer.ClearSceneGraph();
	Labyrinth lab;
	lab.generateLabyrinth(barHandler.labSize, barHandler.labSize);
	addLabyrinth(lab);

	gameEngine.tracer.AddLight(BasicLight(//light0 - point light
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

    gameEngine.tracer.CompileSceneGraph();
}

void TW_CALL RenderPPM(void *userData)
{
    gameEngine.tracer.RenderToPPM("screen.ppm");
}
