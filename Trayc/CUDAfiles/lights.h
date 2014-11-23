/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef TRAYC_LIGHTS_H
#define TRAYC_LIGHTS_H

#include <optixu/optixu_vector_types.h>

enum LightFlags
{
    NO_SHADOWS_POINT = 0,
    NO_SHADOWS_DIRECTIONAL = 1,
    SHADOWS_POINT = 2,
    SHADOWS_DIRECTIONAL = 3
};

//basic point light implementation
struct BasicLight
{
#ifdef _WIN32
    typedef optix::float3 float3;
#endif // _WIN32

    __host__ BasicLight(const float3 &pos, const float3 &color, const float3 &attenuation, const float3 &spot_direction,
        float spot_cutoff, float spot_exponent, float radius, LightFlags flags)
        : pos(pos), color(color), attenuation(attenuation), spot_direction(spot_direction), spot_cutoff(spot_cutoff),
        spot_exponent(spot_exponent), radius(radius), flags(flags)
    {}

    float3 pos;
    float3 color;
    
    //constant, linear, quadratic
    float3 attenuation;
    float3 spot_direction;
    float spot_cutoff;
    float spot_exponent;
    float radius;
    int flags;
};

#endif
