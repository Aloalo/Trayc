
in vec2 UV;

layout(location = 0) out vec4 fragColor;

uniform sampler2D inTex;

const float blurWidth = 2.0;
const float invBlurSize = 1.0 / (2.0 * blurWidth + 1.0);

#ifdef HORIZONTAL
    const vec2 p = vec2(1.0, 0.0);
#elif VERTICAL
    const vec2 p = vec2(0.0, 1.0);
#endif

void main()
{             
    vec2 pixel = p / vec2(textureSize(inTex));

    vec3 sum = vec3(0.0);
    for(float i = -blurWidth; i < blurWidth; i += 1.0)
    {
        sum += texture(inTex, UV + i * pixel);
    }
    
    fragColor = vec4(sum * invBlurSize, 1.0);
}
