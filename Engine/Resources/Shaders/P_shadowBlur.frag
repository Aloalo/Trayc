
#include "UB_ViewRayData.glsl"

in vec2 uv;

layout(location = 0) out vec4 fragColor;

uniform sampler2D inTex1;
uniform sampler2D inTex2;

const float maxBlurWidth = 10.0;
const float maxDD = 5.0;

#ifdef HORIZONTAL
    const vec2 p = vec2(1.0, 0.0);
#else // VERTICAL
    const vec2 p = vec2(0.0, 1.0);
#endif

void main()
{          
    float depth = -texture(inTex2, uv).x;
    float lerp = pow(depth / cameraDist.y, 0.25);
    float blurWidth = mix(maxBlurWidth, 0.0, lerp);

    vec2 pixel = p / vec2(textureSize(inTex1, 0));
    
    vec3 sum = texture(inTex1, uv).rgb;
    float ctSamples = 1.0;
    for(float i = -blurWidth; i < blurWidth; i += 1.0)
    {
        vec2 newUV = uv + i * pixel;
        float d = -texture(inTex2, newUV).x;
        float dd = abs(depth - d);
        float mod = step(dd, maxDD);
        ctSamples += mod;
        sum += texture(inTex1, newUV).rgb * mod;
    }
    
    fragColor = vec4(sum / ctSamples, 1.0);
}
