
in vec2 uv;

uniform sampler2D tex;

layout(location = 0) out vec4 outColor;

void main()
{
    outColor = vec4(texture(tex, uv).aaa, 1.0);
}
