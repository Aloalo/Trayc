#version 330 core

layout(location = 0) in vec4 position;

uniform mat4 VP;

out vec3 texcoords;

void main()
{
    gl_Position = VP * position;
    texcoords = position.xyz;
    texcoords.y = -texcoords.y;
    texcoords.x = -texcoords.x;
}
