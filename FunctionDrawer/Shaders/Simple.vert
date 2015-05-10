#version 330 core

layout(location = 0) in vec4 v_coord;
layout(location = 1) in vec3 v_normal;

out vec4 position;  // position of the vertex (and fragment) in world space
out vec3 varyingNormalDirection;  // surface normal vector in world space

uniform mat4 MVP;
 
void main()
{
    position = v_coord;
    varyingNormalDirection = v_normal;
    gl_Position = MVP * v_coord;
}