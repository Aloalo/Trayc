#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 uv;

layout(location = 2) in vec3 normal;

#ifdef NORMAL_MAP
    layout(location = 3) in vec3 tangent;
    layout(location = 4) in vec3 bitangent;
#endif

uniform mat4 MVP;
uniform mat4 MV;

out VS_OUT
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
} vs_out;

void main()
{
	gl_Position = MVP * position;
    vs_out.depth = (MV * position).z;
	vs_out.uv = uv;
    
#ifdef NORMAL_MAP
   vec3 T = normalize(vec3(MV * vec4(tangent, 0.0)));
   vec3 B = normalize(vec3(MV * vec4(bitangent, 0.0)));
   vec3 N = normalize(vec3(MV * vec4(normal, 0.0)));
   vs_out.TBN = mat3(T, B, N);
#else
    vs_out.normal = normalize(vec3(MV * vec4(normal, 0.0)));
#endif
}
