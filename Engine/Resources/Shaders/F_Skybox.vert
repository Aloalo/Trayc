
layout(location = 0) in vec4 position;

uniform mat4 MVP;

out vec3 texcoords;

void main()
{
    gl_Position = MVP * position;
    texcoords = position.xyz;
}
