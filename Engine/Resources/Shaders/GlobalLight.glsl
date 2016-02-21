
struct Light
{
    vec3 aIntensity; // Ambient intesity
    vec3 intensity; // Directional intesity
    vec3 direction;
};
uniform Light light;

vec3 GetLightDir(in vec3 fragPos)
{
    return light.direction;
}

float GetLightAttenuation(in vec3 fragPos)
{
    return 1.0;
}
