#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 VP;
uniform float scale = 500.0;

out vec3 texcoords;

void main()
{
    texcoords = position;
    gl_Position = VP * vec4(scale * position, 1.0);
}
