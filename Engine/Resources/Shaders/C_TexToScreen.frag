
#include "Tonemap.glsl"

in vec2 uv;

uniform sampler2D tex;

layout(location = 0) out vec4 outColor;

void main()
{
    vec3 color = texture(tex, uv).rgb;
#ifdef TONEMAPPING
    color = Tonemap(color);
#endif
    outColor = vec4(color, 1.0);
}
