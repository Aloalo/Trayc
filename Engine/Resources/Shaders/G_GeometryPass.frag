#version 330 core

// Input
in VS_OUT
{
    // Fragment depth
    float depth;
    vec2 uv;
    // View space normal/TBN
#ifdef NORMAL_MAP
    mat3 TBN;
#else
    vec3 normal;
#endif
} fs_in;

// Uniforms
#ifdef DIFFUSE_MAP
    uniform sampler2D diffuseMap;
#endif
uniform vec3 diffuseColor = vec3(1.0);

#ifdef NORMAL_MAP
    uniform sampler2D normalMap;
#endif

#ifdef SPECULAR_MAP
    uniform sampler2D specularMap;
#endif
uniform vec4 specularGloss;

// Getters
vec3 GetColor()
{
#ifdef DIFFUSE_MAP
    return diffuseColor * texture(diffuseMap, fs_in.uv).rgb;
#else
    return diffuseColor;
#endif
}

vec3 GetNormal()
{
#ifdef NORMAL_MAP
    vec3 normal = texture(normalMap, fs_in.uv).rgb;
    normal = normalize(normal * 2.0 - 1.0);   
    normal = normalize(fs_in.TBN * normal); 
    return normal;
#else
    return fs_in.normal;
#endif
}

vec4 GetSpecGloss()
{
#ifdef SPECULAR_MAP
    return specularGloss * texture(specularMap, fs_in.uv);
#else
    return specularGloss;
#endif
}

// Outputs
layout(location = 0) out float depth;
layout(location = 1) out vec3 normal;
layout(location = 2) out vec4 specGloss;
layout(location = 3) out vec3 albedo;

void main()
{
    depth = fs_in.depth;
    normal = GetNormal();
    specGloss = GetSpecGloss();
    albedo = GetColor();
}
