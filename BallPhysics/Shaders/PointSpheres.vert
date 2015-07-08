#version 330 core

layout(location = 0) in vec4 inVertex;
layout(location = 1) in vec3 inColor;

uniform mat4 MVP;
uniform mat4 V; //View matrix

uniform float pointRadius;  //Point size in world space
uniform float pointScale;   //Scale to calculate size in pixels

out vec3 color;

void main()
{
    float dist = length(vec3(V * inVertex));
    color = inColor;
    gl_PointSize = pointRadius * (pointScale / dist);
    gl_Position = MVP * inVertex;
}
