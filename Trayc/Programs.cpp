/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Trayc/Programs.h>
#include <Trayc/Utils.h>

using namespace optix;
using namespace std;

namespace trayc
{
	Program Programs::anyHitSolid;
	Program Programs::closestHitMesh;
	Program Programs::closestHitGlass;
	Program Programs::anyHitGlass;
	Program Programs::meshBoundingBox;
	Program Programs::meshIntersect;
	Program Programs::rayGeneration;

	Program Programs::exception;
	Program Programs::envmapMiss;
	Program Programs::gradientMiss;
	Program Programs::solidMiss;


	void Programs::Init(Context &ctx)
	{
		const string materialPath(Utils::PathToPTX("material_shaders.cu"));
		const string contextPath(Utils::PathToPTX("context_shaders.cu"));
		const string meshPath(Utils::PathToPTX("triangle_mesh.cu"));

		rayGeneration = ctx->createProgramFromPTXFile(contextPath, "dof_camera");
		exception = ctx->createProgramFromPTXFile(contextPath, "exception");
		envmapMiss = ctx->createProgramFromPTXFile(contextPath, "envmap_miss");
		gradientMiss = ctx->createProgramFromPTXFile(contextPath, "gradient_miss");
		solidMiss = ctx->createProgramFromPTXFile(contextPath, "miss");

		anyHitSolid = ctx->createProgramFromPTXFile(materialPath, "any_hit_solid");
		closestHitGlass = ctx->createProgramFromPTXFile(materialPath, "closest_hit_glass");
		anyHitGlass = ctx->createProgramFromPTXFile(materialPath, "any_hit_glass");
		closestHitMesh = ctx->createProgramFromPTXFile(materialPath, "closest_hit_mesh");

		meshBoundingBox = ctx->createProgramFromPTXFile(meshPath, "mesh_bounds");
		meshIntersect = ctx->createProgramFromPTXFile(meshPath, "mesh_intersect");
	}
}
