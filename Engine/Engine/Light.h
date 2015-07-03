/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef EN_LIGHT_H
#define EN_LIGHT_H

#include <glm/glm.hpp>

namespace engine
{
    enum LightFlag
    {
        LIGHT_DIRECTIONAL = 0,
        LIGHT_POINT = 1,
        LIGHT_SPOT = 2,

        LIGHT_CASTS_SHADOWS = 1 << 3
    };

    struct Light
    {
        Light(void);

        glm::vec3 mIntensity;
        //Constant, linear, quadratic
        glm::vec3 mAttenuation;
        //This is direction is the light is directional
        glm::vec3 mPosition;
        //These can be disregarded if light isn't spot light
        glm::vec3 mSpotDirection;
        float mSpotCutoff;
        float mSpotExponent;

        LightFlag mFlag;
        bool mIsEnabled;
    };
}

#endif
