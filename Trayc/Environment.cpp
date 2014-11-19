/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Trayc/Environment.h>

namespace trayc
{
    Environment::Environment(void) :
        SETTING(screenHeight),
        SETTING(screenWidth),
        SETTING(bufferHeight),
        SETTING(bufferWidth)
    {
    }
    
    optix::Context &ctx = Environment::Get().ctx;
}
