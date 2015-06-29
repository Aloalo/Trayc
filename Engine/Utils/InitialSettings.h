/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_INITIALSETTINGS_H
#define EN_INITIALSETTINGS_H

#include <vector>
#include <string>
#include <map>
#include <Engine/Utils/Singleton.h>

namespace engine
{
    //Only used when starting the game
    struct InitialSettings : public Singleton<InitialSettings>
    {
        using engine::Singleton<InitialSettings>::Get;

        void* operator[](const std::string &variableName);

    private:
        friend engine::Singleton<InitialSettings>;

        InitialSettings(void);

        std::map<std::string, void*> mValues;
        std::map<std::string, unsigned int> mFormat;
    };
}

#endif
