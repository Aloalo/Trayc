/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef TRAYC_PROGRAMS_H
#define TRAYC_PROGRAMS_H

#include <optix_world.h>

namespace trayc
{
    class Programs
    {
    public:

        static void Init(optix::Context &ctx);

        static optix::Program anyHitSolid;
        static optix::Program closestHitMesh;
        static optix::Program closestHitGlass;
        static optix::Program anyHitGlass;

        static optix::Program meshBoundingBox;
        static optix::Program meshIntersect;

        static optix::Program rayGeneration;
        static optix::Program exception;
        static optix::Program envmapMiss;
        static optix::Program gradientMiss;
        static optix::Program solidMiss;
    };
}

#endif
