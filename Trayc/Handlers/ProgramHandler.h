/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef TRAYC_PROGRAMHANDLER_H
#define TRAYC_PROGRAMHANDLER_H

#include <optix_world.h>
#include <Engine/Utils/Singleton.h>
#include <map>
#include <string>

namespace trayc
{
    class ProgramHandler : public engine::Singleton<ProgramHandler>
    {
    public:
        using engine::Singleton<ProgramHandler>::Get;

        optix::Program Get(const std::string &filename, const std::string &programName);
    private:
        std::map<std::string, optix::Program> programs;
    };
}

#endif
