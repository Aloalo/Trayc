
#include "UB_Matrices.glsl"

uniform mat4 shadowDepthBiasVP;
uniform sampler2DShadow shadowMap;
uniform float shadowBrightness;

struct Light
{
    vec3 aIntensity; // Ambient intesity
    vec3 intensity; // Directional intesity
    vec3 direction;
};
uniform Light light;

vec3 GetLightDir(in vec3 fragPos)
{
    return light.direction;
}

float GetLightAttenuation(in vec3 fragPos)
{
    return 1.0;
}

const float maxBias = 0.005;
float GetShadowFactor(in vec3 fragPos, in float dotNL)
{
    vec4 posWorldSpace = invV * vec4(fragPos, 1.0);
    vec4 shadowCoord = shadowDepthBiasVP * posWorldSpace;
    
    float bias = maxBias * tan(acos(dotNL));
    bias = clamp(bias, 0.0, maxBias);
    
    shadowCoord.z -= bias;
    
    float visibility = (1.0 - shadowBrightness) * textureProj(shadowMap, shadowCoord);
    
    return visibility;
}
