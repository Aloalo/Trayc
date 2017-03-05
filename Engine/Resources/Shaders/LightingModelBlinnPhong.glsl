
#include "UB_Matrices.glsl"

uniform samplerCube reflectionMap;
uniform mat4 cubemapM;

#ifdef GLOBAL_LIGHT
    vec3 Reflection(in vec3 V, in vec3 N, in float roughness)
    {
        float lod = log2(float(textureSize(reflectionMap, 0).x));
        vec3 R = reflect(-V, N);
        R = (cubemapM * (invV * vec4(R, 0.0))).xyz;
        return textureLod(reflectionMap, R, lod * roughness).rgb;
    }
#endif

vec3 Lighting(in vec3 N, in vec3 L, in vec3 P, in vec3 lightIntensity, in vec3 albedo, in vec3 specularColor, in float atten, in float gloss, in float shadow)
{
    // Diffuse
    float dNL = max(0.0, dot(N, L));
    vec3 diffuse = dNL * albedo;
    
    // Specular
    vec3 specular = vec3(0.0);
    vec3 V = -normalize(P);
    if(dNL > 0.0)
    {
        vec3 H = normalize(V + L);
        float dotNH = max(0.0, dot(N, H));
        specular = specularColor * pow(dotNH, gloss * 512.0);
    }
    
#ifdef GLOBAL_LIGHT
    vec3 reflection = Reflection(V, N, 1.0 - gloss);
    return atten * lightIntensity * shadow * (diffuse + specular) + reflection * specularColor;
#else
    return atten * lightIntensity * shadow * (diffuse + specular);
#endif
    
}
