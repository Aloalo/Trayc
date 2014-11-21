/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include "LabMaterials.h"
#include <Trayc/Handlers/ProgramHandler.h>
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
    Program closestHitSolid = ProgramHandler::Get().Get("shaders.cu", "closest_hit_reflective");
    Program anyHitSolid = ProgramHandler::Get().Get("shaders.cu", "any_hit_solid");
    Program boxAABB = ProgramHandler::Get().Get("box.cu", "bounds");
    Program boxIntersect = ProgramHandler::Get().Get("box.cu", "intersect");
    Program floorAABB = ProgramHandler::Get().Get("rectangleAA.cu", "bounds");
    Program floorIntersect = ProgramHandler::Get().Get("rectangleAA.cu", "intersect");

    Material wallMaterial = ctx->createMaterial();
    wallMaterial->setClosestHitProgram(0, closestHitSolid);
    wallMaterial->setAnyHitProgram(1, anyHitSolid);
    wallMaterial["Ka"]->setFloat(0.8f, 0.8f, 0.8f);
    wallMaterial["Kd"]->setFloat(0.8f, 0.8f, 0.8f);
    wallMaterial["Ks"]->setFloat(0.8f, 0.8f, 0.8f);
    wallMaterial["phong_exp"]->setFloat(88.0f);
    wallMaterial["reflectivity"]->setFloat(0.0f, 0.0f, 0.0f);
    labmat[WALL] = wallMaterial;

    Material mirrorMaterial = ctx->createMaterial();
    mirrorMaterial->setClosestHitProgram(0, closestHitSolid);
    mirrorMaterial->setAnyHitProgram(1, anyHitSolid);
    mirrorMaterial["Ka"]->setFloat(0.3f, 0.3f, 0.3f);
    mirrorMaterial["Kd"]->setFloat(0.7f, 0.7f, 0.7f);
    mirrorMaterial["Ks"]->setFloat(0.8f, 0.8f, 0.8f);
    mirrorMaterial["phong_exp"]->setFloat(88.0f);
    mirrorMaterial["reflectivity"]->setFloat(0.7f, 0.7f, 0.7f);
    labmat[MIRROR] = mirrorMaterial;

    Material glassMaterial = ctx->createMaterial();
    glassMaterial->setClosestHitProgram(0, ProgramHandler::Get().Get("material_shaders.cu", "closest_hit_glass"));
    glassMaterial->setAnyHitProgram(1, ProgramHandler::Get().Get("material_shaders.cu", "any_hit_glass"));
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
