
in vec2 uv;

layout(location = 0) out vec4 fragColor;

uniform sampler2D tex;
uniform vec2 invScreenSize;

#define FXAA_QUALITY__PRESET 39
#define FXAA_GLSL_130 1
#define FXAA_PC 1
#define FXAA_GATHER4_ALPHA 1

// TODO(jure): check perf for this
// FXAA_DISCARD

#include "Fxaa.glsl"

#define DUMMY FxaaFloat4(0.0)

void main()
{
    fragColor = FxaaPixelShader(uv, uv.xxxx, tex,
        tex, tex, invScreenSize, DUMMY, DUMMY, DUMMY, 0.75, 0.125, 0.0312, DUMMY.x, DUMMY.x, DUMMY.x, DUMMY);
}
