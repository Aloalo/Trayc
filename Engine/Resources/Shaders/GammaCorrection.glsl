
uniform vec3 invGammaExp = vec3(1.0 / 1.0);
vec3 gammaCorrect(in vec3 color)
{
    return pow(color, invGammaExp);
}
