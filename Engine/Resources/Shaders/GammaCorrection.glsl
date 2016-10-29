uniform vec3 gamma;

vec3 sRGBToLinear(in vec3 color)
{
    return pow(color, gamma);
}

vec3 linearTosRGB(in vec3 color)
{
    return pow(color, 1.0 / gamma);
}
