/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <Trayc/CUDAfiles/common.h>
#include <Trayc/CUDAfiles/random.h>
#include <Trayc/CUDAfiles/helper.h>
#include <optix_math.h>

//
// Pinhole/DOF camera implementation
//
rtDeclareVariable(float3, eye, , );
rtDeclareVariable(float3, U, , );
rtDeclareVariable(float3, V, , );
rtDeclareVariable(float3, W, , );

rtDeclareVariable(int, AAlevel, , );
rtDeclareVariable(float, aperture_radius, , );
rtDeclareVariable(float, focal_length, , );
rtDeclareVariable(int, dof_samples, , );

RT_PROGRAM void dof_camera()
{
    const uint2 screen = output_buffer.size() * AAlevel;
    const float2 invScreen2x = 2.0f / make_float2(screen);
    const uint2 uiNewLaunchIndex = make_uint2(launch_index.x, launch_index.y + myStripe * output_buffer.size().y / renderingDivisionLevel);
    const int2 iNewLaunchIndex = make_int2(uiNewLaunchIndex.x, uiNewLaunchIndex.y);

    float3 result = make_float3(0.0f);
    unsigned int seed = tea<1>(screen.x * uiNewLaunchIndex.y + uiNewLaunchIndex.x, rnd_seed);
    float count = 0.0f;

    for(int i = 0; i < AAlevel; ++i)
        for(int j = 0; j < AAlevel; ++j)
        {
            const float2 d = make_float2(AAlevel * iNewLaunchIndex.x + i, AAlevel * iNewLaunchIndex.y + j) * invScreen2x - 1.f;
            const float3 ray_direction = normalize(d.x * U + d.y * V + W);

            const optix::Ray ray(eye, ray_direction, radiance_ray_type, scene_epsilon);

            PerRayData_radiance prd;
            prd.importance = 1.f;
            prd.depth = 0;

            rtTrace(top_object, ray, prd);
            result += prd.result;
            count += 1.0f;

            for(int k = 0; k < dof_samples; ++k)
            {
                optix::Ray ray_dof(ray);

                const float2 circPoint = make_float2(rnd(seed) * 2.0f - 1.0f, rnd(seed) * 2.0f - 1.0f);
                const float3 apertureOffset = make_float3(circPoint.x * aperture_radius, circPoint.y * aperture_radius, 0.0f);
                ray_dof.origin += apertureOffset;
                ray_dof.direction *= focal_length;
                ray_dof.direction -= apertureOffset;
                ray_dof.direction = normalize(ray_dof.direction);

                PerRayData_radiance prd;
                prd.importance = 1.f;
                prd.depth = 0;

                rtTrace(top_object, ray_dof, prd);
                count += 1.0f;
                result += prd.result;
            }
        }

    output_buffer[uiNewLaunchIndex] = make_color(result / count);
}

//
// Enviormement map
//
rtTextureSampler<uchar4, 2, cudaReadModeNormalizedFloat> envmap;
RT_PROGRAM void envmap_miss()
{
    const float theta = atan2f(ray.direction.x, ray.direction.z);
    const float phi = M_PI_2f - acosf(ray.direction.y);
    const float u = (theta + M_PIf) * (0.5f * M_1_PIf);
    const float v = 0.5f * (1.0f + sinf(phi));
    prd_radiance.result = make_float3(tex2D(envmap, u, v));
}

//
// Returns solid color for miss rays
//
rtDeclareVariable(float3, miss_color, , );
RT_PROGRAM void miss()
{
    prd_radiance.result = miss_color;
}

//
// Returns color from [miss_min, miss_max] lineary interpolated across ray inclination
//
rtDeclareVariable(float3, miss_min, , );
rtDeclareVariable(float3, miss_max, , );
RT_PROGRAM void gradient_miss()
{
    const float phi = asinf(ray.direction.y);
    prd_radiance.result = 2.0f * phi / M_PIf * (miss_max - miss_min) + miss_min;
}

//
// Set pixel to solid color upon failure
//
rtDeclareVariable(float3, bad_color, , );

RT_PROGRAM void exception()
{
    const uint2 newLaunchIndex = make_uint2(launch_index.x, launch_index.y + myStripe * output_buffer.size().y / renderingDivisionLevel);
    output_buffer[newLaunchIndex] = make_color(bad_color);
}
