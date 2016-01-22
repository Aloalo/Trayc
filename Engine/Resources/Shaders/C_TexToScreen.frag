#version 330 core

in vec2 uv;

uniform sampler2D tex;

layout(location = 0) out vec4 outColor;

void main()
{
    outColor = texture(tex, uv);
}
