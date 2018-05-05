
#ifdef TONEMAPPING
    #include "Tonemap.glsl"
#endif

#ifdef DITHERING
    #include "Dithering.glsl"
#endif

#ifdef GAMMA_LUMA
    uniform vec3 gamma;
#endif

in vec2 uv;
uniform sampler2D tex;

layout(location = 0) out vec4 outColor;

void main()
{
    vec4 color = texture(tex, uv);
    
#ifdef TONEMAPPING
    color.rgb = Tonemap(color.rgb);
#endif

#ifdef DITHERING
    color.rgb = ApplyDither(color.rgb, uv);
#endif

#ifdef GAMMA_LUMA
    color.a = dot(pow(color.rgb, gamma), vec3(0.299, 0.587, 0.114));
#endif

    outColor = color;
}
