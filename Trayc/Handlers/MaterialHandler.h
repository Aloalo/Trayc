/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef TRAYC_MATERIALHANDLER_H
#define TRAYC_MATERIALHANDLER_H

#include <Engine/Geometry/Material.h>
#include <optix_world.h>
#include <map>

namespace trayc
{
    //caches materials by index
	class MaterialHandler
	{
    public:
        optix::Material CreateMaterial(const engine::Material &mat);
        //destroy currently active materials
        void Clear();

	private:
        std::map<int, optix::Material> materials;
	};
}

#endif
