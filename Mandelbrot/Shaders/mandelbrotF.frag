#version 330 core

smooth in vec2 c;

out vec3 outputColor;

uniform float limit;
uniform float scale;
uniform int aa;

const float err = 100.0;
const float inv_lerr = 1.0 / log(err);

float f(in vec2 w, out vec2 z)
{
	z = vec2(0.0);
	float i;
	for(i = 0.0; i < limit && dot(z, z) <= err; i += 1.0)
		z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + w;
	return i;
}

vec3 g(in float i, in vec2 z)
{
 	i -= log2(log(length(z)) * inv_lerr);

	if(dot(z, z) <= 4.0)
		return vec3(0.0);
 	else
 		return vec3(-cos(0.025 * i) + 1., -cos(0.080 * i) + 1., -cos(0.120 * i) + 1.) * 0.5;
}

void main()
{
    vec2 z;
    outputColor = g(f(c, z), z);
    
    if(aa == 1)
    {
        float d = 1.0 / (scale * 3.0);
        float e = -d;
        outputColor += g(f(c + vec2(d, 0), z), z);
        outputColor += g(f(c + vec2(e, 0), z), z);
        outputColor += g(f(c + vec2(0, d), z), z);
        outputColor += g(f(c + vec2(0, e), z), z);
        outputColor += g(f(c + vec2(e, e), z), z);
        outputColor += g(f(c + vec2(d, e), z), z);
        outputColor += g(f(c + vec2(e, d), z), z);
        outputColor += g(f(c + vec2(d, d), z), z);
        outputColor /= 9.0;
    }
}
