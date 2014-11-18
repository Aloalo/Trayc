#version 330 core

layout(location = 0) in vec4 vertexPosition_modelspace;

out vec2 UV;

void main()
{
    gl_Position = vertexPosition_modelspace;
    UV = (vertexPosition_modelspace.xy + vec2(1, 1)) * 0.5;
}
