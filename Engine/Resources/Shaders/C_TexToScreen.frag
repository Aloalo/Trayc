
#include "Tonemap.glsl"

in vec2 uv;

uniform sampler2D tex;
uniform sampler2D noiseTex;
uniform vec2 noiseScale;

#define NOISE_SCALE 0.25/255.0

layout(location = 0) out vec4 outColor;

void main()
{
    vec3 color = texture(tex, uv).rgb;
    
#ifdef TONEMAPPING
    color = Tonemap(color);
#endif

    color += mix(-NOISE_SCALE, NOISE_SCALE, texture(noiseTex, uv * noiseScale).r);

    outColor = vec4(color, 1.0);
}
