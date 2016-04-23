#version 330 core

layout(location = 0) in vec4 position;

#include "UB_Matrices.glsl"

out vec3 texcoords;

void main()
{
    texcoords = position.xyz;
    gl_Position = VP * position;
}
