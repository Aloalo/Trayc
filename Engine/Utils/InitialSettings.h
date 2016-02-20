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
        const T& GetSetting(const std::string &name) const
        {
            return mSettings.get<T>(name);
        }

        template<>
        const int& GetSetting(const std::string &name) const
        {
            return (int)mSettings.get<jsonxx::Number>(name);
        }

        template<>
        const float& GetSetting(const std::string &name) const
        {
            return (float)mSettings.get<jsonxx::Number>(name);
        }

    private:
        friend engine::Singleton<InitialSettings>;
        InitialSettings(void);

        jsonxx::Object mSettings;
        std::map<std::string, unsigned int> mFormat;
    };
}

#endif
