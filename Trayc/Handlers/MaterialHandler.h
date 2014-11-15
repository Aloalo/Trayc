/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef TRAYC_MATERIALHANDLER_H
#define TRAYC_MATERIALHANDLER_H


#include <assimp/material.h>
#include <Trayc/Handlers/OptixTextureHandler.h>
#include <Engine/Utils/Singleton.h>

namespace trayc
{
	class MaterialHandler : public Singleton<MaterialHandler>
	{
	public:
		optix::Material CreateMaterial(const std::string &path, const aiMaterial *mat);
		std::string GetTextureName(const aiMaterial *mat, aiTextureType type, const std::string &path, const std::string &def = "error.png") const;

	private:
		MaterialHandler(void);
		friend Singleton<MaterialHandler>;
	};
}

#endif
