
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
            mValue = InitialSettings::Get().GetSetting<T>(mName);
        }

        operator T&()
        {
            return mValue;
        }

        operator const T&() const
        {
            return mValue;
        }

        T& operator=(const T &v)
        {
            InitialSettings::Get().SetSetting<T>(mName, v);
            return mValue = v;
        }

    private:
        T mValue;
        std::string mName;
    };
}

#endif
