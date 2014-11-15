/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef TRAYC_OPTIXTEXTUREHANDLER_H
#define TRAYC_OPTIXTEXTUREHANDLER_H

#include <GL/glew.h>
#include <Trayc/SafeOptix.h>
#include <map>
#include <Trayc/Utils.h>
#include <Engine/Utils/Singleton.h>

namespace trayc
{
	class OptixTextureHandler : public Singleton<OptixTextureHandler>
	{
	public:
		using Singleton<OptixTextureHandler>::Get;

		optix::TextureSampler Get(const std::string &path, GLfloat anisotropy = 0.0f, RTwrapmode wrapMode = RT_WRAP_REPEAT, 
			const std::string &def = Utils::DefTexture("error.png"), GLenum format = GL_RGBA8);

	private:
		OptixTextureHandler(void);
		friend Singleton<OptixTextureHandler>;
        std::map<std::string, optix::TextureSampler> existingTextures;
	};
}

#endif
