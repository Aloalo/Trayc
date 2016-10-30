#include "UB_Matrices.glsl"

uniform mat4 shadowDepthBiasVP;
uniform sampler2DShadow shadowMap;
uniform float shadowBrightness;

const float maxBias = 0.005;
const float minBias = 0.0005;
float GetVisibilityFactor(in vec3 fragPos, in float dotNL)
{
    vec4 posWorldSpace = invV * vec4(fragPos, 1.0);
    vec4 shadowCoord = shadowDepthBiasVP * posWorldSpace;
    
    float bias = maxBias + (minBias - maxBias) * dotNL;
    
    shadowCoord.z -= bias;
    
    float visibility = (1.0 - shadowBrightness) * textureProj(shadowMap, shadowCoord);
    
    return visibility;
}
