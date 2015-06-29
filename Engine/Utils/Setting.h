/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_SETTING_H
#define EN_SETTING_H

#include <Engine/Utils/InitialSettings.h>

#define SETTING(s) s(#s)

namespace engine
{
    template<class T>
    struct Setting
    {
        Setting(const std::string &name) :
            mName(name)
        {
            const void *value = InitialSettings::Get()[mName];
            x = *(T*)&value;
        }

        operator T&()
        {
            return x;
        }

        T& operator=(const T &v)
        {
            return x = v;
        }

        T x;
        std::string mName;
    };
}

#endif
