#version 330 core

layout(location = 0) in vec4 vPosition;

out vec4 position;  // position of the vertex (and fragment) in world space

uniform mat4 MVP;
 
void main()
{
    position = vPosition;
    gl_Position = MVP * vPosition;
}