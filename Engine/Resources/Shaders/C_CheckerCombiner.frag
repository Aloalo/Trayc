in vec2 uv;

uniform sampler2D tex;

layout(location = 0) out vec3 outColor;

// http://alex.vlachos.com/graphics/Alex_Vlachos_Advanced_VR_Rendering_Performance_GDC2016.pdf

// TODO(jure): Finish the filter

/*
void main()
{
    vec2 invTexSize = vec2(1.0) / vec2(textureSize(tex, 0).xy);
    float x = gl_FragCoord.x;
    float y = gl_FragCoord.y - 0.5;
    
    vec2 reducedCoords = (gl_FragCoord.xy - mod(gl_FragCoord.xy, 2.0)) / 2.0;
    float row2x2ID = mod(reducedCoords.y, 2.0);
    
    if(mod(reducedCoords.x + reducedCoords.y, 2.0) == 0.0) {
        float offsetU = invTexSize.x * (gl_FragCoord.x - 2.0 * row2x2ID + mod(gl_FragCoord.x, 2.0)) / 2.0;
        outColor = texture(tex, vec2(offsetU, uv.y)).rgb;
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
}
*/

void main()
{
    vec2 invTexSize = vec2(1.0) / vec2(textureSize(tex, 0).xy);
    float x = gl_FragCoord.x;
    float y = gl_FragCoord.y - 0.5;
    
    vec2 reducedCoords = (gl_FragCoord.xy - mod(gl_FragCoord.xy, 2.0)) / 2.0;
    float row2x2ID = mod(reducedCoords.y, 2.0);
    
    if(mod(reducedCoords.x + reducedCoords.y, 2.0) == 0.0) {
        float offsetU = invTexSize.x * (gl_FragCoord.x - 2.0 * row2x2ID + mod(gl_FragCoord.x, 2.0)) / 2.0;
        outColor = texture(tex, vec2(offsetU, uv.y)).rgb;
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
}
    