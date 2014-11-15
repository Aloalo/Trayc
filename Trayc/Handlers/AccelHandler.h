/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef TRAYC_ACCELHANDLER_H
#define TRAYC_ACCELHANDLER_H

#include <Trayc/SafeOptix.h>
#include <string>

namespace trayc
{
	class AccelHandler
	{
	public:
		AccelHandler(void);
		~AccelHandler(void);

		void SetMesh(const std::string &name);
		void SetAccelCaching(bool onoff);
		void SetTraverser(const char *traverser);
		void SetBuilder(const char *builder);
		void SetRefine(const char *ref);
		void SetLargeMesh(bool f);
		void LoadAccelCache(optix::GeometryGroup geometry_group);
		void SaveAccelCache(optix::GeometryGroup geometry_group);

		bool accel_cache_loaded;
	private:
		std::string GetCacheFileName() const;

		std::string filename;

		std::string accel_builder;
		std::string accel_traverser;
		std::string accel_refine;
		bool accel_caching_on;
		bool accel_large_mesh;
	};
}

#endif
