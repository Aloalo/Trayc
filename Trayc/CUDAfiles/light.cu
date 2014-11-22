/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Trayc/CUDAfiles/common.h>

using namespace optix;

rtDeclareVariable(int, light_idx, , );

rtDeclareVariable(float3, geometric_normal, attribute geometric_normal, ); 
rtDeclareVariable(float3, shading_normal, attribute shading_normal, );

RT_PROGRAM void intersect(int)
{
    const float3 center = lights[light_idx].pos;
    const float3 O = ray.origin - center;
    const float3 D = ray.direction;
    const float radius = lights[light_idx].radius;

    const float b = dot(O, D);
    const float c = dot(O, O) - radius * radius;
    const float disc = b * b - c;
    if(disc > 0.0f)
    {
        const float sdisc = sqrtf(disc);
        const float root1 = (-b - sdisc);

        bool check_second = true;
        if(rtPotentialIntersection(root1))
        {
            shading_normal = geometric_normal = (O + root1 * D) / radius;
            if(rtReportIntersection(0))
                check_second = false;
        } 
        if(check_second)
        {
            const float root2 = (-b + sdisc);
            if(rtPotentialIntersection(root2))
            {
                shading_normal = geometric_normal = (O + root2*D)/radius;
                rtReportIntersection(0);
            }
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
