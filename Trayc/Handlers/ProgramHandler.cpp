/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Trayc/Handlers/ProgramHandler.h>
#include <Trayc/Utils.h>
#include <Trayc/Environment.h>

using namespace optix;
using namespace std;

namespace trayc
{
    optix::Program ProgramHandler::Get(const std::string &filename, const std::string &programName)
    {
        const string name = filename + programName;
        if(programs.find(name) != programs.end())
            return programs[name];

        const string path(Utils::PathToPTX(filename));
        optix::Program prog = ctx->createProgramFromPTXFile(path, programName);
        programs[name] = prog;
        return prog;
    }

}
