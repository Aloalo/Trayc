#version 330 core

smooth in vec2 c;

out vec4 outputColor;

uniform float limit;
uniform float scale;
uniform int aa;

const float err = 100.0;
const float lerr = log(err);

float f(vec2 w, out vec2 z)
{
	z = vec2(0, 0);
	float i;
	for(i = 0; i < limit + 2 && dot(z, z) <= err; i += 1)
		z = vec2(z.x * z.x - z.y * z.y, 2 * z.x * z.y) + w;
	return i;
}

vec4 g(float i, vec2 z)
{
 	i -= log2(log(length(z)) / lerr);

	if(dot(z, z) <= 4)
		return vec4(0, 0, 0, 1);
 	else
 		return vec4(
 			(-cos(0.025 * i) + 1.) / 2.,
 			(-cos(0.080 * i) + 1.) / 2.,
 			(-cos(0.120 * i) + 1.) / 2.,
 			1.0
 		);
}

void main()
{
    vec2 z;
    if(aa == 0)
        outputColor = g(f(c, z), z);
    else
    {
        float d = 1 / scale / 3;
        float e = -d;
        float i = f(c + vec2(0, 0), z);
        outputColor = g(i, z);
        if(dot(z, z) > 4.0)
        {
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
}
