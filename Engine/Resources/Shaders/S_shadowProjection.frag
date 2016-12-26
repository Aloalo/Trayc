
in vec2 UV;
in vec3 viewRay;

layout(location = 0) out vec4 fragColor;

#include "Shadowmapping.glsl"

uniform sampler2D gDepth;
uniform sampler2D gNormal;

#ifdef DIRECTIONAL_LIGHT
    #include "DirectionalLight.glsl"
#endif

#ifdef GLOBAL_LIGHT
    #include "GlobalLight.glsl"
#endif

#ifdef POINT_LIGHT
    #include "PointLight.glsl"
#endif

#ifdef SPOT_LIGHT
    #include "SpotLight.glsl"
#endif

void main() {
    vec3 fragPos = texture(gDepth, UV).r * viewRay;
    vec3 N = normalize(texture(gNormal, UV).xyz);
    vec3 L = GetLightDir(fragPos);
    float dotNL = max(0.0, dot(N, L));
    
    float v = GetVisibilityFactor(fragPos, dotNL);
    fragColor = vec4(v, v, v, 1.0f);
}
