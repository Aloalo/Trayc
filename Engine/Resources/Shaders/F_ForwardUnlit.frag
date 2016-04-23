#version 330 core

// Input
#ifdef DIFFUSE_MAP
    out VS_OUT
    {
        vec2 uv;
    } fs_in;
#endif

// Uniforms
#ifdef DIFFUSE_MAP
    uniform sampler2D diffuseMap;
#endif
uniform vec4 diffuseColor = vec4(1.0);

// Outputs
layout(location = 0) out vec4 color;

void main()
{
    color = diffuseColor;
#ifdef DIFFUSE_MAP
    color *= texture(diffuseMap, fs_in.uv);
#endif
}
