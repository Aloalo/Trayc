
uniform sampler2D noiseTex;
uniform vec2 noiseScale;
#define NOISE_SCALE vec3(0.25/255.0)

vec3 ApplyDither(in vec3 inColor, in vec2 texCoords)
{
    return inColor + mix(-NOISE_SCALE, NOISE_SCALE, texture(noiseTex, texCoords * noiseScale).r);
}
