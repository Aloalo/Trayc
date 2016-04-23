#version 330 core

layout(location = 0) in vec4 position;

#ifdef DIFFUSE_MAP
    layout(location = 1) in vec2 uv;
#endif

uniform mat4 MVP;

#ifdef DIFFUSE_MAP
    out VS_OUT
    {
        vec2 uv;
    } vs_out;
#endif

void main()
{
	gl_Position = MVP * position;
#ifdef DIFFUSE_MAP
    vs_out.uv = uv;
#endif
}
