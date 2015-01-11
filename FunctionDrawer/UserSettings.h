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
    engine::Setting<int> ctVertices;

private:
    UserSettings(void);
    friend engine::Singleton<UserSettings>;
};

#endif
