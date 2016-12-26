
in vec2 UV;

#ifndef AMBIENT_LIGHT
    in vec3 viewRay;
#endif

layout(location = 0) out vec4 fragColor;

#ifndef AMBIENT_LIGHT
    uniform sampler2D sShadowBuffer;
    uniform sampler2D gDepth;
    uniform sampler2D gNormal;
    uniform sampler2D gSpecGloss;
#endif

uniform sampler2D gAlbedo;

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

#ifdef AMBIENT_LIGHT
    #include "AmbientLight.glsl"
#endif

#ifndef AMBIENT_LIGHT
    #ifdef USE_PBR
        #include "LightingModelPBR.glsl"
    #else
        #include "LightingModelBlinnPhong.glsl"
    #endif
#endif

void main()
{             
#ifndef AMBIENT_LIGHT
    // Retrieve data from G-buffer
    vec3 fragPos = texture(gDepth, UV).r * viewRay;
    vec3 N = normalize(texture(gNormal, UV).xyz);
    vec3 albedo = texture(gAlbedo, UV).rgb;
    vec4 specularGloss = texture(gSpecGloss, UV);
    float shadow = texture(sShadowBuffer, UV).r;
    
    // Get Light params
    vec3 L = GetLightDir(fragPos);
    float atten = GetLightAttenuation(fragPos);
    vec3 lightIntensity = light.intensity;
    
    vec3 color = Lighting(N, L, fragPos, lightIntensity, albedo, specularGloss.rgb, atten, specularGloss.a, shadow);
    
    #ifdef GLOBAL_LIGHT
        vec3 ambient = light.aIntensity * albedo;
        fragColor = vec4(ambient + color, 1.0);
    #else
        fragColor = vec4(color, 1.0);
    #endif
#else
    // Ambient light
    fragColor = vec4(texture(gAlbedo, UV).rgb * light.intensity, 1.0);
#endif
}
