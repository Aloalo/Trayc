/*
* Copyright (c) 2014 Jure Ratkovic
*/

#ifndef TRAYC_COMMON_H
#define TRAYC_COMMON_H

#include <optix_world.h>
#include <Trayc/CUDAfiles/lights.h>

struct PerRayData_radiance
{
    float3 result;
    float importance;
    int depth;
};

struct PerRayData_shadow
{
    float attenuation;
};

rtBuffer<uchar4, 2> output_buffer;
rtDeclareVariable(unsigned int, rnd_seed, , );
rtDeclareVariable(int, renderingDivisionLevel, , "Number of horizontal stripes");
rtDeclareVariable(int, myStripe, , "Current stripe");
rtDeclareVariable(float, scene_epsilon, , );

rtDeclareVariable(rtObject, top_object, , );

rtDeclareVariable(uint2, launch_index, rtLaunchIndex, );

rtDeclareVariable(optix::Ray, ray, rtCurrentRay, );
rtDeclareVariable(float, t_hit, rtIntersectionDistance, );

rtDeclareVariable(unsigned int, radiance_ray_type, , );
rtDeclareVariable(unsigned int, shadow_ray_type, , );
rtDeclareVariable(PerRayData_radiance, prd_radiance, rtPayload, );
rtDeclareVariable(PerRayData_shadow,   prd_shadow, rtPayload, );

rtBuffer<BasicLight> lights;

#endif
