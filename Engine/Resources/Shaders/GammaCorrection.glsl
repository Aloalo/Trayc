
uniform vec3 invGammaExp;
vec3 gammaCorrect(in vec3 color)
{
    return pow(color, invGammaExp);
}
