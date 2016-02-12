#version 330 core

in vec2 UV;
in vec3 viewRay;

out vec4 fragColor;

uniform sampler2D gDepth;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gSpecGloss;

#ifdef DIRECTIONAL_LIGHT
    #include "DirectionalLight.glsl"
#elif POINT_LIGHT
    #include "PointLight.glsl"
#else SPOT_LIGHT
    #include "SpotLight.glsl"
#endif

void main()
{             
    // Retrieve data from G-buffer
    vec3 fragPos = texture(gDepth, UV).rgb * viewRay;
    vec3 normal = texture(gNormal, UV).rgb;
    vec3 albedo = texture(gAlbedo, UV).rgb;
    vec4 specularGloss = texture(gSpecGloss, UV);
    
    // Get Light params
    vec3 lightDir = GetLightDir(fragPos);
    float atten = GetLightAttenuation(fragPos);
    vec3 lightIntensity = atten * light.intensity;
    
    // Then calculate lighting as usual
    
    // Ambient
    vec3 ambient = albedo * 0.1;

    // Diffuse
    float dNL = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = dNL * albedo * lightIntensity;
    
    // Specular
    vecr E = normalize(fragPos);
    vec3 R = reflect(-lightDir, normal);
    float dER = max(0.0, dot(E, R));
    vec3 specular = lightIntensity * specularGloss.rgb * pow(dER, specularGloss.a);

    fragColor = vec4(ambient + diffuse + specular, 1.0);
}
