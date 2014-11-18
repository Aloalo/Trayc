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

        optix::Material material = ctx->createMaterial();

        material->setClosestHitProgram(0, Programs::closestHitMesh);
        material->setAnyHitProgram(1, Programs::anyHitSolid);

        material["Ka"]->setFloat(*(float3*)&mat.Ka);
        material["Kd"]->setFloat(*(float3*)&mat.Kd);
        material["Ks"]->setFloat(*(float3*)&mat.Ks);
        material["reflectivity"]->setFloat(*(float3*)&mat.reflectivity);

        material["diffuse_map"]->setTextureSampler(OptixTextureHandler::Get().Get(mat.diffuse_map));
        material["specular_map"]->setTextureSampler(OptixTextureHandler::Get().Get(mat.specular_map));

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
