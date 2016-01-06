#version 330 core

layout(location = 0) in vec2 position;

smooth out vec2 c;

uniform vec2 translation;
uniform float scale;
uniform vec2 windowSize;

void main()
{
	gl_Position = vec4((position + translation) * vec2(1, -1) * scale / windowSize * 2 - vec2(1, -1), 0, 1);
	c = position;
}
