/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef FD_USERSETTINGS_H
#define FD_USERSETTINGS_H

#include <Engine/Utils/Setting.h>
#include <Engine/Utils/Singleton.h>

struct UserSettings : public engine::Singleton<UserSettings>
{
    engine::Setting<int> screenWidth;
    engine::Setting<int> screenHeight;
    engine::Setting<float> FOV;
    engine::Setting<float> drawDistance;
    engine::Setting<int> ctVertices;
    engine::Setting<int> AAlevel;
    engine::Setting<float> tolerance;
    engine::Setting<float> Lstep;
    engine::Setting<float> minX;
    engine::Setting<float> maxX;
    engine::Setting<float> minY;
    engine::Setting<float> maxY;

private:
    UserSettings(void);
    friend engine::Singleton<UserSettings>;
};

#endif
