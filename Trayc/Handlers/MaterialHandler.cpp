/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Trayc/Handlers/MaterialHandler.h>
#include <Trayc/Programs.h>
#include <Trayc/Handlers/OptixTextureHandler.h>
#include <Trayc/Environment.h>

using namespace std;
using namespace optix;

namespace trayc
{
    optix::Material MaterialHandler::CreateMaterial(const engine::Material &mat)
    {
        if(materials.find(mat.index) != materials.end())
            return materials[mat.index];

        const bool isReflective = mat.reflectivity.x > 0.0f && mat.reflectivity.y > 0.0f && mat.reflectivity.z > 0.0f;

        optix::Material material = ctx->createMaterial();

        if(isReflective)
        {
            material->setClosestHitProgram(0, ProgramHandler::Get().Get("material_shaders.cu", "closest_hit_reflective"));
            material["reflectivity"]->setFloat(*(float3*)&mat.reflectivity);
        }
        else
            material->setClosestHitProgram(0, ProgramHandler::Get().Get("material_shaders.cu", "closest_hit_solid"));

        material->setAnyHitProgram(1, ProgramHandler::Get().Get("material_shaders.cu", "any_hit_solid"));

        material["Ka"]->setFloat(*(float3*)&mat.Ka);
        material["Kd"]->setFloat(*(float3*)&mat.Kd);
        material["Ks"]->setFloat(*(float3*)&mat.Ks);


        material["diffuse_map"]->setTextureSampler(OptixTextureHandler::Get().Get(mat.diffuse_map, 0.0f, RT_WRAP_REPEAT, Utils::DefTexture("diffDefault.png")));
        material["specular_map"]->setTextureSampler(OptixTextureHandler::Get().Get(mat.specular_map, 0.0f, RT_WRAP_REPEAT, Utils::DefTexture("specDefault.png")));

        materials[mat.index] = material;

        return material;
    }

    void MaterialHandler::Clear()
    {
        for(auto item : materials)
            item.second->destroy();

        materials.clear();
    }

}
