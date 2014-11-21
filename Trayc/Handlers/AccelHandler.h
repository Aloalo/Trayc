/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef TRAYC_ACCELHANDLER_H
#define TRAYC_ACCELHANDLER_H

#include <optix_world.h>
#include <string>

namespace trayc
{
    class AccelHandler
    {
    public:

        bool LoadAccelCache(const std::string &filename, optix::GeometryGroup geometry_group);
        void SaveAccelCache(const std::string &filename, optix::GeometryGroup geometry_group);

    private:
        std::string GetCacheFileName(const std::string &filename) const;
    };
}

#endif
