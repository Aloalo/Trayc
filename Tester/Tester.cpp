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

#include <Trayc/GameEngine.h>
#include <Trayc/Environment.h>
#include <Trayc/Utils.h>

#include "Labyrinth.h"
#include "LabMaterials.h"

using namespace engine;
using namespace optix;
using namespace std;
using namespace trayc;

struct Junk;

GameEngine *ptr;
LabMaterials mat;

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
		ptr->tracer.AddGeometryInstance(ctx->createGeometryInstance(box, &mat.getLabyrinthMaterial(walls[i].matIdx), 
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

	ptr->tracer.AddGeometryInstance(ctx->createGeometryInstance(floor, &mat.getLabyrinthMaterial(LabMaterials::WALL), 
		&mat.getLabyrinthMaterial(LabMaterials::WALL)+1));
}

void RenderingLoop()
{
    EventHandler::AddEventListener(ptr);
    EventHandler::AddUpdateable(ptr);

    do
    {
        glClear(GL_COLOR_BUFFER_BIT);
        ptr->Draw();
        // Swap buffers
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

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    //TextureHandler::generateDefaultTex();

	ptr = new GameEngine();
	mat.createLabMaterials();

	try
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(Utils::Resource("crytek-sponza/sponza.obj"), aiProcessPreset_TargetRealtime_MaxQuality);
		if(!scene)
		{
		printf("%s\n", importer.GetErrorString());
		return 0;
		}
		ptr->tracer.AddScene(Utils::Resource("crytek-sponza/"), scene);
		
		/*const aiScene* nissan = importer.ReadFile(Utils::resource("nissan/nissan.obj"), aiProcessPreset_TargetRealtime_MaxQuality);
		if(!nissan)
		{
			printf("%s\n", importer.GetErrorString());
			return 0;
		}
		ptr->tracer.addScene(mat.getLabyrinthMaterial(LabMaterials::MIRROR), nissan);*/
	}
	catch(exception &ex)
	{
		printf("%s\n", ex.what());
		return 0;
	}
	/*Labyrinth lab;
	lab.generateLabyrinth(15, 15);
	addLabyrinth(lab);*/

	ptr->tracer.AddLight(BasicLight(//light0 - point light
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

	ptr->tracer.AddLight(BasicLight(//light1 - spot light
		make_float3(0.0f, 30.0f, 0.0f), //pos/dir
		make_float3(2.0f), //color
		make_float3(1.0f, 0.001f, 0.001f), //attenuation
		make_float3(0.0f, 0.0f, 0.0f), //spot_direction
		22.5f, //spot_cutoff
		32.0f, //spot_exponent
		0.25f, //radius
		1, //casts_shadows
		0 //is_directional
		));

	ptr->tracer.AddLight(BasicLight(//light2 - directional light
		make_float3(1, 1, 1), //pos/dir
		make_float3(0.1f), //color
		make_float3(0.0f), //attenuation
		make_float3(0.0f), //spot_direction
		360.0f, //spot_cutoff
		0.0f, //spot_exponent
		0.0f, //radius
		0, //casts_shadows
		1 //is_directional
		));
	ctx->setExceptionEnabled(RT_EXCEPTION_ALL, true);

    ptr->Init();
    RenderingLoop();

    SDLHandler::CleanUp();

	delete ptr;
	return 0;
}
