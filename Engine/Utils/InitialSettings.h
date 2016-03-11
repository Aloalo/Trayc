/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_INITIALSETTINGS_H
#define EN_INITIALSETTINGS_H

#include <vector>
#include <string>
#include <map>
#include <Engine/Utils/Singleton.h>
#include <jsonxx/jsonxx.h>

namespace engine
{
    // Only used when starting the game
    struct InitialSettings : public Singleton<InitialSettings>
    {
        template<class T>
        T GetSetting(const std::string &name) const
        {
            return mSettings.get<T>(name);
        }

        template<>
        int GetSetting(const std::string &name) const
        {
            return static_cast<int>(mSettings.get<jsonxx::Number>(name));
        }

        template<>
        float GetSetting(const std::string &name) const
        {
            return static_cast<float>(mSettings.get<jsonxx::Number>(name));
        }

    private:
        friend engine::Singleton<InitialSettings>;
        InitialSettings(void);

        jsonxx::Object mSettings;
    };
}

#endif
