/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Trayc/Programs.h>
#include <Trayc/Utils.h>
#include <Trayc/Environment.h>

using namespace optix;
using namespace std;

namespace trayc
{
    optix::Program ProgramHandler::Get(const std::string &filename, const std::string &programName)
    {
        if(programs.find(programName) != programs.end())
            return programs[programName];

        const string path(Utils::PathToPTX(filename));
        optix::Program prog = ctx->createProgramFromPTXFile(path, programName);
        programs[programName] = prog;
        return prog;
    }

}
