#version 330 core

in VS_OUT
{
    //fragment depth
    float depth;
    vec2 uv;
    // view space normal/TBN
#ifdef NORMAL_MAP
    mat3 TBN;
#else
    vec3 normal;
#endif

#ifdef VERTEX_COLOR
    vec3 color;
#endif
} fs_in;

#ifdef DIFFUSE_MAP
    uniform sampler2D diffuseMap;
#else
    uniform vec3 constColor = vec3(1.0);
#endif

#ifdef NORMAL_MAP
    uniform sampler2D normalMap;
#endif

#ifdef SPECULAR_MAP
    uniform sampler2D specularMap;
#else
    uniform vec4 specularGloss;
#endif

vec4 GetSpecGloss()
{
#ifdef SPECULAR_MAP
    return texture(specularMap, fs_in.uv);
#else
    return specularGloss;
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

vec3 GetColor()
{
#ifdef DIFFUSE_MAP
    vec3 fragColor = texture(diffuseMap, fs_in.uv).rgb;
#else
    vec3 fragColor = constColor;
#endif

#ifdef VERTEX_COLOR
    fragColor *= fs_in.color;
#endif
    return fragColor;
}

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
