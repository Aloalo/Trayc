#version 330 core

in vec2 UV;

#ifndef AMBIENT_LIGHT
    in vec3 viewRay;
#endif

layout(location = 0) out vec4 fragColor;

#ifndef AMBIENT_LIGHT
    uniform sampler2D gDepth;
    uniform sampler2D gNormal;
    uniform sampler2D gSpecGloss;
#endif

uniform sampler2D gAlbedo;

#ifdef DIRECTIONAL_LIGHT
    #include "DirectionalLight.glsl"
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

void main()
{             
#ifndef AMBIENT_LIGHT
    // Retrieve data from G-buffer
    vec3 fragPos = texture(gDepth, UV).r * viewRay;
    vec3 normal = texture(gNormal, UV).rgb;
    vec3 albedo = texture(gAlbedo, UV).rgb;
    
    // Get Light params
    vec3 lightDir = GetLightDir(fragPos);
    float atten = GetLightAttenuation(fragPos);
    vec3 lightIntensity = atten * light.intensity;
    
    // Calculate lighting
    // Diffuse
    float dNL = max(0.0, dot(normal, lightDir));
    vec3 diffuse = dNL * albedo * lightIntensity;
    
    // Specular
    vec3 specular = vec3(0.0);
    if(dNL > 0.0)
    {
        vec4 specularGloss = texture(gSpecGloss, UV);
        vec3 E = normalize(fragPos);
        vec3 R = reflect(lightDir, normal);
        float dER = max(0.0, dot(E, R));
        specular = lightIntensity * specularGloss.rgb * pow(dER, specularGloss.a * 255.0);
    }
    
    fragColor = vec4(diffuse + specular, 1.0);
#else
    // Ambient light
    fragColor = vec4(texture(gAlbedo, UV).rgb * light.intensity, 1.0);
#endif
}
