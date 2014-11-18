/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef TRAYC_ENVIRONMENT_H
#define TRAYC_ENVIRONMENT_H

#include <Engine/Utils/Setting.h>
#include <Trayc/Handlers/MaterialHandler.h>
#include <Engine/Utils/Singleton.h>

namespace trayc
{
    class Environment : public Singleton<Environment>
    {
    public:
        engine::Setting<int> screenWidth;
        engine::Setting<int> screenHeight;
        engine::Setting<int> bufferWidth;
        engine::Setting<int> bufferHeight;

        optix::Context ctx;

    private:
        Environment(void);
        friend Singleton<Environment>;
    };

    extern optix::Context &ctx;
}

#endif
