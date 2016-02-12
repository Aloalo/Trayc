
struct Light
{
    vec3 intensity;
    vec3 direction;
};
Light light;

vec3 GetLightDir(in vec3 fragPos)
{
    return direction;
}

float GetLightAttenuation(in vec3 fragPos)
{
    return 1.0;
}
