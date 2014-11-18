/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include "LabMaterials.h"
#include <Trayc/Programs.h>
#include <Trayc/Environment.h>
#include <Trayc/Utils.h>
#include <Trayc/Handlers/OptixTextureHandler.h>

using namespace optix;
using namespace trayc;
using namespace std;

LabMaterials::LabMaterials(void)
{
}


LabMaterials::~LabMaterials(void)
{
}

Material& LabMaterials::getLabyrinthMaterial(int mat)
{
	return labmat[mat];
}

void LabMaterials::createLabMaterials()
{
	string shaders = Utils::PathToPTX("shaders.cu");
    string pathFloor = Utils::PathToPTX("rectangleAA.cu");
    string pathBox = Utils::PathToPTX("box.cu");

	Program closestHitSolid = ctx->createProgramFromPTXFile(shaders, "closest_hit_phong");
	Program anyHitSolid = ctx->createProgramFromPTXFile(shaders, "any_hit");
    boxAABB = ctx->createProgramFromPTXFile(pathBox, "box_bounds");
    boxIntersect = ctx->createProgramFromPTXFile(pathBox, "box_intersect");
    floorAABB = ctx->createProgramFromPTXFile(pathFloor, "bounds");
    floorIntersect = ctx->createProgramFromPTXFile(pathFloor, "intersect");


	Material wallMaterial = ctx->createMaterial();
	wallMaterial->setClosestHitProgram(0, closestHitSolid);
	wallMaterial->setAnyHitProgram(1, anyHitSolid);
	wallMaterial["Ka"]->setFloat(0.8f, 0.8f, 0.8f);
	wallMaterial["Kd"]->setFloat(0.8f, 0.8f, 0.8f);
	wallMaterial["Ks"]->setFloat(0.8f, 0.8f, 0.8f);
	wallMaterial["phong_exp"]->setFloat(88.0f);
	wallMaterial["ambient_map"]->setTextureSampler(OptixTextureHandler::Get().Get(Utils::DefTexture("opacityDefault.png")));
	labmat[WALL] = wallMaterial;

	Material mirrorMaterial = ctx->createMaterial();
	mirrorMaterial->setClosestHitProgram(0, closestHitSolid);
	mirrorMaterial->setAnyHitProgram(1, anyHitSolid);
	mirrorMaterial["Ka"]->setFloat(0.3f, 0.3f, 0.3f);
	mirrorMaterial["Kd"]->setFloat(0.7f, 0.7f, 0.7f);
	mirrorMaterial["Ks"]->setFloat(0.8f, 0.8f, 0.8f);
	mirrorMaterial["ambient_map"]->setTextureSampler(OptixTextureHandler::Get().Get(Utils::DefTexture("opacityDefault.png")));
	mirrorMaterial["phong_exp"]->setFloat(88.0f);
	mirrorMaterial["reflectivity"]->setFloat(0.7f, 0.7f, 0.7f);
	labmat[MIRROR] = mirrorMaterial;

	Material glassMaterial = ctx->createMaterial();
	glassMaterial->setClosestHitProgram(0, Programs::closestHitGlass);
	glassMaterial->setAnyHitProgram(1, Programs::anyHitGlass);
	glassMaterial["importance_cutoff"]->setFloat(1e-2f);
	glassMaterial["cutoff_color"]->setFloat(0.55f, 0.55f, 0.55f);
	glassMaterial["fresnel_exponent"]->setFloat(3.0f);
	glassMaterial["fresnel_minimum"]->setFloat(0.1f);
	glassMaterial["fresnel_maximum"]->setFloat(1.0f);
	glassMaterial["refraction_index"]->setFloat(1.4f);
	glassMaterial["refraction_color"]->setFloat(1.0f, 1.0f, 1.0f);
	glassMaterial["reflection_color"]->setFloat(1.0f, 1.0f, 1.0f);
	float3 extinction = make_float3(.80f, .80f, .80f);
	glassMaterial["extinction_constant"]->setFloat(log(extinction.x), log(extinction.y), log(extinction.z));
	glassMaterial["shadow_attenuation"]->setFloat(0.4f, 0.4f, 0.4f);
	labmat[GLASS] = glassMaterial;
}
