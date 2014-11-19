#include <Trayc/CUDAfiles/phong.h>

rtDeclareVariable(float3, shading_normal, attribute shading_normal, ); 
rtDeclareVariable(float3, geometric_normal, attribute geometric_normal, ); 

//
//ADS phong shader with shadows and reflections, no textures
//
rtDeclareVariable(float3, Ka, , );
rtDeclareVariable(float3, Kd, , );
rtDeclareVariable(float3, Ks, , );
rtDeclareVariable(float, phong_exp, , );
rtDeclareVariable(float3, reflectivity, , );

RT_PROGRAM void closest_hit_phong()
{
    const float3 world_geo_normal = normalize(rtTransformNormal(RT_OBJECT_TO_WORLD, geometric_normal));
    const float3 world_shade_normal = normalize(rtTransformNormal(RT_OBJECT_TO_WORLD, shading_normal));
    const float3 ffnormal = faceforward(world_shade_normal, -ray.direction, world_geo_normal);
    const float3 hit_point = ray.origin + t_hit * ray.direction;

    const uint2 screen = output_buffer.size();
    unsigned int seed = tea<16>(screen.x*launch_index.y+launch_index.x, frame);

    phongShade(hit_point, Ka, Kd, Ks, ffnormal, phong_exp, seed);
    phongReflect(hit_point, ffnormal, reflectivity);
    prd_radiance.result *= ambientOcclusion(hit_point, ffnormal, seed);
}

RT_PROGRAM void any_hit()
{
    phongShadowed();
}
