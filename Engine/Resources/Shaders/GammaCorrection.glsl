
uniform vec3 invGammaExp = vec3(1.0 / 2.2);
vec3 gammaCorrect(in vec3 color)
{
    return pow(color, invGammaExp);
}
