/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Trayc/CUDAfiles/common.h>

using namespace optix;

rtDeclareVariable(float, bloom_exponent, , );

rtDeclareVariable(int, light_idx, , );
rtDeclareVariable(float, light_brightness, attribute light_brightness, );
rtDeclareVariable(float, importance_cutoff, , );

RT_PROGRAM void intersect(int)
{

    const float3 center = lights[light_idx].pos;
    const float radius = lights[light_idx].radius;
    const float bloomFactor = 2.0f;
    const float bloomRadius = bloomFactor * radius;

    const float t = dot(ray.direction, center - ray.origin);
    const float3 planePoint = ray.origin + t * ray.direction;

    const float hitDistToCenter2 = dot(planePoint - center, planePoint - center);
    const float bloomRadius2 = bloomRadius * bloomRadius;
    const float adjusted_t = t + sqrtf(bloomRadius2 - hitDistToCenter2);

    
    light_brightness = 1.0f;
    if(hitDistToCenter2 <= radius * radius)
    {
        if(rtPotentialIntersection(adjusted_t))
            rtReportIntersection(0);
    }
    else if(hitDistToCenter2 <= bloomRadius2 || fmaxf(lights[light_idx].color) > importance_cutoff)
    {
        if(rtPotentialIntersection(adjusted_t))
        {
            const float bloom = 1.0f - (sqrtf(hitDistToCenter2) - radius) / (radius * (bloomFactor - 1));
            light_brightness = powf(bloom, bloom_exponent);
            rtReportIntersection(0);
        }
    }
}

RT_PROGRAM void bounds(int, float result[6])
{
    const float3 cen = lights[light_idx].pos;
    const float3 rad = make_float3(lights[light_idx].radius);

    optix::Aabb* aabb = (optix::Aabb*)result;

    if(rad.x > 0.0f  && !isinf(rad.x))
    {
        aabb->m_min = cen - rad;
        aabb->m_max = cen + rad;
    }
    else
    {
        aabb->invalidate();
    }
}
