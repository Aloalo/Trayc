#version 330 core

layout(location = 0) in vec4 position;

out vec2 UV;
out vec3 viewRay;

uniform float tanHalfFovy;
uniform float aspectRatio;

void main()
{
	gl_Position = position;
	UV = (position.xy + vec2(1.0, 1.0)) * 0.5;
    viewRay = vec3(position.x * tanHalfFovy * aspectRatio, position.y * tanHalfFovy, 1.0);
}
