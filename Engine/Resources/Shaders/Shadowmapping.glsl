#include "UB_Matrices.glsl"

uniform mat4 shadowDepthBiasVP;
uniform sampler2DShadow shadowMap;
uniform float shadowBrightness;

#define SOFT_SHADOWS


const float maxBias = 0.03;
const float minBias = 0.005;
float GetVisibilityFactor(in vec3 fragPos, in float dotNL)
{
    vec4 posWorldSpace = invV * vec4(fragPos, 1.0);
    vec4 shadowCoord = shadowDepthBiasVP * posWorldSpace;
    
    float bias = maxBias + (minBias - maxBias) * dotNL;
    
    shadowCoord.z -= bias;
    
    #ifdef SOFT_SHADOWS
        float sum = 0.0;
        vec2 pix = 1.0 / vec2(textureSize(shadowMap, 0));
        for (float y = -0.5 ; y <= 0.5 ; y += 1.0) {
            for (float x = -0.5 ; x <= 0.5 ; x += 1.0) {
                vec4 coord = shadowCoord;
                coord.xy += vec2(x, y) * pix;
                sum += textureProj(shadowMap, coord);
            }
        }
        
        float sampleBrightness = (1.0 - shadowBrightness) / 4.0;
        return shadowBrightness + sum * sampleBrightness;
    #else
        return shadowBrightness + (1.0 - shadowBrightness) * textureProj(shadowMap, shadowCoord);
    #endif
}
