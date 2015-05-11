#version 330 core

layout(location = 0) in vec4 vertex;

out vec2 pixel;

void main()
{
    gl_Position = vertex;
    pixel = vertex.xy;
}
