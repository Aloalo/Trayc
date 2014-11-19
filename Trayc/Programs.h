/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef TRAYC_PROGRAMS_H
#define TRAYC_PROGRAMS_H

#include <optix_world.h>
#include <Engine/Utils/Singleton.h>
#include <map>
#include <string>

namespace trayc
{
    class ProgramHandler : public Singleton<ProgramHandler>
    {
    public:
        using Singleton<ProgramHandler>::Get;

        optix::Program Get(const std::string &filename, const std::string &programName);
    private:
        std::map<std::string, optix::Program> programs;
    };
}

#endif
