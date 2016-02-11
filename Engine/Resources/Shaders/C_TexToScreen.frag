#version 330 core

#include "GammaCorrection.glsl"

in vec2 uv;

uniform sampler2D tex;

layout(location = 0) out vec4 outColor;

void main()
{
    outColor = vec4(gammaCorrect(texture(tex, uv).rgb), 1.0);
}
