
uniform vec3 gamma;
vec3 gammaCorrect(in vec3 color)
{
    return pow(color, gamma);
}
