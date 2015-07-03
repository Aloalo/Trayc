/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Engine/Engine/Light.h>

namespace engine
{
    Light::Light(void)
        : mSpotCutoff(0.0f), mSpotExponent(0.0f), mFlag(LIGHT_DIRECTIONAL), mIsEnabled(true)
    {
    }
}
