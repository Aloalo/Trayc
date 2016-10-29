uniform float exposure;

vec3 Tonemap(in vec3 hdrColor)
{
    return vec3(1.0) - exp(-hdrColor * exposure);
}
