// TODO(Jure): optimize shader for less samples / get rid of this
// http://alex.vlachos.com/graphics/Alex_Vlachos_Advanced_VR_Rendering_Performance_GDC2016.pdf
#extension GL_ARB_arrays_of_arrays : require

in vec2 uv;

uniform sampler2D tex;

layout(location = 0) out vec3 outColor;

float offsets[3] = float[](-1.0, 0.0, 1.0);

float w[3][3] = float[][] (
    float[](1.0 / 16.0, 1.0 / 8.0, 1.0 / 16.0),
    float[](1.0 / 8.0, 1.0 / 4.0, 1.0 / 8.0),
    float[](1.0 / 16.0, 1.0 / 8.0, 1.0 / 16.0)
);

void main()
{
    vec2 invTexSize = vec2(1.0) / vec2(textureSize(tex, 0).xy);
    
    vec3 ret = vec3(0.0);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            ret += texture(tex, uv + invTexSize * vec2(offsets[i], offsets[j])).rgb * w[i][j];
        }
    }
    
    outColor = ret;
}
    