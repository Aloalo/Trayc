#include <Trayc/Handlers/MaterialHandler.h>
#include <Trayc/Programs.h>
#include <Trayc/Utils.h>
#include <Trayc/Environment.h>

using namespace std;
using namespace optix;

namespace trayc
{
	MaterialHandler::MaterialHandler(void)
	{
	}

	Material MaterialHandler::CreateMaterial(const string &path, const aiMaterial *mat)
	{
		Material material = ctx->createMaterial();

		material->setClosestHitProgram(0, Programs::closestHitMesh);
		material->setAnyHitProgram(1, Programs::anyHitSolid);

		aiColor3D color;
		mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
		material["Ka"]->setFloat(make_float3(color.r, color.g, color.b));

		mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		material["Kd"]->setFloat(make_float3(color.r, color.g, color.b));

		mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
		material["Ks"]->setFloat(make_float3(color.r, color.g, color.b));

		mat->Get(AI_MATKEY_COLOR_REFLECTIVE, color);
		material["reflectivity"]->setFloat(make_float3(color.r, color.g, color.b));

		material["diffuse_map"]->setTextureSampler(OptixTextureHandler::Get().Get(GetTextureName(mat, aiTextureType_DIFFUSE, path, "diffDefault.png")));
		material["specular_map"]->setTextureSampler(OptixTextureHandler::Get().Get(GetTextureName(mat, aiTextureType_SPECULAR, path, "specDefault.png")));

		return material;
	}

	string MaterialHandler::GetTextureName(const aiMaterial *mat, aiTextureType type, const string &path, const string &def) const
	{
		aiString name;
		if(mat == NULL || mat->GetTextureCount(type) == 0)
			return Utils::DefTexture(def);

		mat->GetTexture(type, 0, &name, NULL, NULL, NULL, NULL, NULL);
		return path + string(name.C_Str());
	}
}
