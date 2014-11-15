/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef TRAYC_SCENEGRAPHHANDLER_H
#define TRAYC_SCENEGRAPHHANDLER_H

#include <optix_world.h>
#include <vector>

namespace trayc
{
	class SceneGraphHandler
	{
	public:
		SceneGraphHandler(void);
		~SceneGraphHandler(void);

		void Add(optix::GeometryGroup gg, const std::vector<optix::Transform> &trans);
		void Add(optix::GeometryGroup gg, optix::Transform trans);

	private:
		optix::Group topObject, topShadower;
		optix::GeometryGroup staticGeometry;

		std::vector<optix::Transform> transforms;
		std::vector<optix::GeometryGroup> staticGG;
		std::vector<optix::GeometryInstance> gis;

		std::vector<std::vector<int> > conectivty;
	};
}

#endif
