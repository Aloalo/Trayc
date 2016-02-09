#version 330 core

in vec2 uv;

uniform sampler2D tex;

layout(location = 0) out vec4 outColor;

uniform vec3 invGammaExp = vec3(1.0 / 2.2);
vec3 gammaCorrect(in vec3 color)
{
    return pow(color, invGammaExp);
}

void main()
{
    outColor = vec4(gammaCorrect(texture(tex, uv).rgb), 1.0);
}
