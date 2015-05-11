#version 330 core

layout(location = 0) in vec4 v_coord;

out vec4 position;  // position of the vertex (and fragment) in world space

uniform mat4 MVP;
 
void main()
{
    position = v_coord;
    gl_Position = MVP * v_coord;
}