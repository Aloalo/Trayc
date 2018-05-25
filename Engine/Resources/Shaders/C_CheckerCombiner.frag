in vec2 uv;

uniform sampler2D tex;

layout(location = 0) out vec3 outColor;

// http://alex.vlachos.com/graphics/Alex_Vlachos_Advanced_VR_Rendering_Performance_GDC2016.pdf

/*
// ORGINAL AVERAGE
void main()
{
    vec2 invTexSize = vec2(1.0) / vec2(textureSize(tex, 0).xy);
    float x = gl_FragCoord.x;
    float y = gl_FragCoord.y - 0.5;
    
    vec2 reducedCoords = (gl_FragCoord.xy - mod(gl_FragCoord.xy, 2.0)) / 2.0;
    float row2x2ID = mod(reducedCoords.y, 2.0);
    
    if(mod(reducedCoords.x + reducedCoords.y, 2.0) == 0.0) {
        vec2 uv1 = vec2(invTexSize.x * (x + mod(x, 2.0) - 2.0 * row2x2ID) / 2.0, uv.y);
        vec2 uv2 = vec2(invTexSize.x * ((x + mod(x, 2.0)) / 2.0 - 1.0 + row2x2ID), uv.y + invTexSize.y * (2.0 * mod(y, 2.0) - 1.0));
        outColor = 0.5 * (texture(tex, uv1).rgb + texture(tex, uv2).rgb);
    }
    else if (row2x2ID == 0.0) {
        vec2 uv1 = vec2(invTexSize.x * (x + mod(x, 2.0)) / 2.0, uv.y);
        vec2 uv2 = vec2(invTexSize.x * (x - 2.0 + mod(x, 2.0)) / 2.0, uv.y + mix(-invTexSize.y, invTexSize.y, mod(y, 2.0)));
        outColor = 0.5 * (texture(tex, uv1).rgb + texture(tex, uv2).rgb);
    }
    else {
        vec2 uv1 = vec2(invTexSize.x * (x - 2.0 + mod(x, 2.0)) / 2.0, uv.y);
        vec2 uv2 = vec2(invTexSize.x * (x + mod(x, 2.0)) / 2.0, uv.y + mix(-invTexSize.y, invTexSize.y, mod(y, 2.0)));
        outColor = 0.5 * (texture(tex, uv1).rgb + texture(tex, uv2).rgb);
    }
    
    
// ORIGINAL FILTER
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
}
*/

void main()
{
    vec2 invTexSize = vec2(1.0) / vec2(textureSize(tex, 0).xy);
    float x = gl_FragCoord.x;
    float y = gl_FragCoord.y - 0.5;
    
    vec2 reducedCoords = (gl_FragCoord.xy - mod(gl_FragCoord.xy, 2.0)) / 2.0;
    float row2x2ID = mod(reducedCoords.y, 2.0);
    
    float yOffset = mix(-invTexSize.y, invTexSize.y, mod(y, 2.0));
    vec2 uv1 = vec2(invTexSize.x * (x + mod(x, 2.0) - 2.0 * row2x2ID) / 2.0, uv.y);
    vec2 uv2 = vec2(invTexSize.x * (x + mod(x, 2.0) - 2.0 * (1.0 - row2x2ID)) / 2.0, uv.y + yOffset);
    
    int ySign = -int(sign(yOffset));
    int xSign = -int(sign(mod(x, 2.0) - 1.0));
    
    vec3 t1 = texture(tex, uv1).rgb;
    vec3 t2 = texture(tex, uv2).rgb;
    vec3 t3 = textureOffset(tex, uv2, ivec2(xSign, 0)).rgb;
    vec3 t4 = textureOffset(tex, uv1, ivec2(xSign, 0)).rgb;
    vec3 t5 = textureOffset(tex, uv1, ivec2(0, ySign)).rgb;
    vec3 t6 = textureOffset(tex, uv1, ivec2(xSign, ySign)).rgb;
    vec3 t7 = textureOffset(tex, uv2, ivec2(0, 3 * ySign)).rgb;
    vec3 t8 = textureOffset(tex, uv2, ivec2(xSign, 3 * ySign)).rgb;
    
    outColor = (t1 + t2) * (9.0 / 32.0) + (t3 + t4 + t5 + t7) * (3.0 / 32.0) + (t6 + t8) * (1.0 / 32.0);
}
    