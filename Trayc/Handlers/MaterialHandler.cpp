/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Trayc/Handlers/MaterialHandler.h>
#include <Trayc/Handlers/ProgramHandler.h>
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



        optix::Material material = ctx->createMaterial();

        if(mat.IoR > 1.01f)
        {
            material->setClosestHitProgram(0, ProgramHandler::Get().Get("material_shaders.cu", "closest_hit_glass"));
            material->setAnyHitProgram(1, ProgramHandler::Get().Get("material_shaders.cu", "any_hit_glass"));
            material["cutoff_color"]->setFloat(0.55f, 0.55f, 0.55f);
            material["fresnel_exponent"]->setFloat(3.0f);
            material["fresnel_minimum"]->setFloat(0.1f);
            material["fresnel_maximum"]->setFloat(1.0f);
            material["refraction_index"]->setFloat(mat.IoR);
            material["refraction_color"]->setFloat(1.0f, 1.0f, 1.0f);
            material["reflection_color"]->setFloat(1.0f, 1.0f, 1.0f);
            const float3 extinction = make_float3(1.0f, 1.0f, 1.0f);
            material["extinction_constant"]->setFloat(logf(extinction.x), logf(extinction.y), logf(extinction.z));
            material["shadow_attenuation"]->setFloat(0.3f, 0.3f, 0.3f);
        }
        else
        {
            const bool isReflective = mat.reflectivity.x > 0.0f && mat.reflectivity.y > 0.0f && mat.reflectivity.z > 0.0f;

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
            material["phong_exponent"]->setFloat(mat.phongExponent);


            material["diffuse_map"]->setTextureSampler(OptixTextureHandler::Get().Get(mat.diffuse_map, 0.0f, RT_WRAP_REPEAT, Utils::DefTexture("diffDefault.png")));
            material["specular_map"]->setTextureSampler(OptixTextureHandler::Get().Get(mat.specular_map, 0.0f, RT_WRAP_REPEAT, Utils::DefTexture("specDefault.png")));

            materials[mat.index] = material;
        }

        return material;
    }

    void MaterialHandler::Clear()
    {
        for(auto item : materials)
            item.second->destroy();

        materials.clear();
    }

}
