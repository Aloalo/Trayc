
in vec2 uv;

uniform sampler2D tex;

uniform float nearRange;
uniform float farRange;

layout(location = 0) out vec4 outColor;

void main()
{
    float normDepth = (-texture(tex, uv).r - nearRange) / (farRange - nearRange);
    outColor = vec4(normDepth, normDepth, normDepth, 1.0);
}
