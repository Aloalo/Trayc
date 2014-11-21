/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef TRAYC_SCENEGRAPH_H
#define TRAYC_SCENEGRAPH_H

#include <optix_world.h>
#include <vector>
#include <Trayc/Handlers/AccelHandler.h>

namespace trayc
{
    class SceneGraph
    {
    public:
        void Init();

        void Add(optix::GeometryInstance inst);
        void AddLight(optix::GeometryInstance inst);

        void Compile();
        void Reset();

        template<class T>
        T GetTopObject() const
        {
            if(topGroup->getChildCount() == 1)
                return topGroup->getChild<T>(0);
            return topGroup;
        }

        template<class T>
        T GetTopShadower() const
        {
            return topGroup;
        }

    private:

        optix::Group topGroup;
        optix::GeometryGroup lightGG;
        optix::GeometryGroup staticGG;
        AccelHandler staticAccel;
    };
}

#endif
