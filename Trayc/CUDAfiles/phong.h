#include <optix_world.h>
#include <optixu/optixu_math_namespace.h>
#include <optixu/optixu_math.h>
#include "helper.h"
#include "lights.h"
#include "random.h"

rtDeclareVariable(rtObject, top_object, , );
rtDeclareVariable(uint2, launch_index, rtLaunchIndex, );

rtDeclareVariable(optix::Ray, ray, rtCurrentRay, );
rtDeclareVariable(float, t_hit, rtIntersectionDistance, );
rtDeclareVariable(PerRayData_radiance, prd_radiance, rtPayload, );
rtDeclareVariable(PerRayData_shadow,   prd_shadow, rtPayload, );

rtDeclareVariable(int, max_depth, , );
rtDeclareVariable(int, shadow_samples, , );

rtBuffer<BasicLight> lights;

rtDeclareVariable(float3, ambient_light_color, , );
rtDeclareVariable(unsigned int, radiance_ray_type, , );
rtDeclareVariable(unsigned int, shadow_ray_type, , );
rtDeclareVariable(float, importance_cutoff, , );
rtDeclareVariable(float, scene_epsilon, , );

static __device__ __inline__ void phongShadowed()
{
    prd_shadow.attenuation = 0.0f;
    rtTerminateRay();
}

static __device__ __inline__ void phongShade(float3 p_Ka,
                               float3 p_Kd,
                               float3 p_Ks,
                               float3 ffnormal,
                               float  phong_exp,
                               float3 reflectivity)
{
    const float3 hit_point = ray.origin + t_hit * ray.direction;
    float3 color = p_Ka * ambient_light_color;

    for(int i = 0; i < lights.size(); ++i)
    {
        const BasicLight &light = lights[i];

        float attenuation = 0.0f;
        float Ldist = 0.0f;
        float3 L = make_float3(0.0f);

        if(light.is_directional) // directional light?
        {
            attenuation = 1.0f;
            L = normalize(light.pos);
            Ldist = FLT_MAX;
        }
        else // point light or spotlight
        {
            L = normalize(light.pos - hit_point);
            Ldist = length(light.pos - hit_point);
            attenuation = 1.0f / (light.attenuation.x + light.attenuation.y * Ldist + light.attenuation.z * Ldist * Ldist);

            if(light.spot_cutoff <= 90.0f) // spotlight?
            {
                const float clampedCosine = max(0.0f, dot(-L, light.spot_direction));
                const float cutoffRadians = light.spot_cutoff * pi / 180.0f;
                
                if(clampedCosine < cosf(cutoffRadians)) // outside of spotlight cone?
                    attenuation = 0.0f;
                else
                    attenuation = attenuation * powf(clampedCosine, light.spot_exponent);   
            }
        }

        const float nDl = dot(ffnormal, L);

        if(nDl > 0.0f && attenuation > 0.0f)
        {
            PerRayData_shadow shadow_prd;
            shadow_prd.attenuation = 1.0f;

            if(shadow_samples > 0 && light.casts_shadows)
            {
                if(shadow_samples == 1)
                {
                    const optix::Ray sray(hit_point, L, shadow_ray_type, scene_epsilon, Ldist);
                    rtTrace(top_object, sray, shadow_prd);
                    attenuation *= shadow_prd.attenuation;
                }
                else
                {
                    float lighthit = 0.0f;
                    unsigned int seed = (launch_index.x * 1920 + launch_index.y) * launch_index.x * launch_index.y;
                    for(int k = 0; k < shadow_samples; ++k)
                    {
                        const float3 u = normalize(make_float3(L.y - L.z, -L.x, L.x));

                        const float r = rnd(seed) * light.radius;
                        const float t = rnd(seed) * pi2;
                        const float3 lightpoint =  r * (cosf(t) * u + sinf(t) * cross(L, u)) + light.pos;
                        const float3 Ldir = lightpoint - hit_point;

                        shadow_prd.attenuation = 1.0f;
                        const optix::Ray shadow_ray(hit_point, normalize(Ldir), shadow_ray_type, scene_epsilon, length(Ldir));
                        rtTrace(top_object, shadow_ray, shadow_prd);
                        lighthit += shadow_prd.attenuation;
                    }
                    
                    attenuation *= (lighthit / (float)shadow_samples);
                }
            }

            if(attenuation > 0.0f)
            {
                const float3 light_color = light.color * attenuation;
                color += p_Kd * nDl * light_color;

                const float3 H = normalize(L - ray.direction);
                const float nDh = dot(ffnormal, H);
                if(nDh > 0.0f && p_Ks.x > 0.0f && p_Ks.y > 0.0f && p_Ks.z > 0.0f)
                    color += p_Ks * light_color * powf(nDh, phong_exp);
            }
        }
    }

    if(fmaxf(reflectivity) > 0.0f)
    {
        const float3 r = schlick(-dot(ffnormal, ray.direction), reflectivity);

        const float importance = prd_radiance.importance * optix::luminance(r);

        //reflection ray
        if(importance > importance_cutoff && prd_radiance.depth < max_depth)
        {
            PerRayData_radiance refl_prd;
            refl_prd.importance = importance;
            refl_prd.depth = prd_radiance.depth+1;
            const float3 R = reflect(ray.direction, ffnormal);
            const optix::Ray refl_ray(hit_point, R, radiance_ray_type, scene_epsilon);
            rtTrace(top_object, refl_ray, refl_prd);
            color += r * refl_prd.result;
        }
    }

    // pass the color back up the tree
    prd_radiance.result = color;
}
