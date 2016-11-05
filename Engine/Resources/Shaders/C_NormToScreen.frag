
in vec2 uv;

uniform sampler2D tex;

layout(location = 0) out vec4 outColor;

void main()
{
    outColor = vec4(texture(tex, uv).rgb * 0.5 + 0.5, 1.0);
}
