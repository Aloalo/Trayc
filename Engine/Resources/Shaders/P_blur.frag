
in vec2 uv;

layout(location = 0) out vec4 fragColor;

uniform sampler2D inTex1;

const float blurWidth = 3.0;
const float invBlurSize = 1.0 / (2.0 * blurWidth + 1.0);

#ifdef HORIZONTAL
    const vec2 p = vec2(1.0, 0.0);
#else // VERTICAL
    const vec2 p = vec2(0.0, 1.0);
#endif

void main()
{             
    vec2 pixel = p / vec2(textureSize(inTex1, 0));

    vec3 sum = texture(inTex1, uv).rgb;
    for(float i = -blurWidth; i < blurWidth; i += 1.0)
    {
        sum += texture(inTex1, uv + i * pixel).rgb;
    }
    
    fragColor = vec4(sum * invBlurSize, 1.0);
}
