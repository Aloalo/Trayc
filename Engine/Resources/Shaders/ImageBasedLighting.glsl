
#include "UB_Matrices.glsl"

uniform samplerCube reflectionMap;
uniform samplerCube irradianceMap;
uniform mat4 cubemapM;

vec3 IBL(in vec3 V, in vec3 N, in float roughness, in vec3 F0, in vec3 albedo)
{
    // Calculate and transform the reflection vector
    vec3 R = reflect(-V, N);
    R = (cubemapM * (invV * vec4(R, 0.0))).xyz;
    
    // Diffuse contribution
    vec3 irradiance = texture(irradianceMap, R).rgb;
    
    // Specular contribution
    float lod = log2(float(textureSize(reflectionMap, 0).x));
    vec3 specular = textureLod(reflectionMap, R, lod * roughness).rgb * F0;
    
    return albedo * irradiance * (1.0 - F0) + specular;
}
