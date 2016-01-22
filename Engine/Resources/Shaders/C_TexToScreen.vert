#version 330 core

layout(location = 0) in vec4 position;

out vec2 uv;

void main()
{
	gl_Position = position;
	uv = (position.xy + vec2(1.0, 1.0)) * 0.5;
}
