
struct Light
{
    vec3 intensity;
};
uniform Light light;

vec3 GetIntensity()
{
    return light.intensity;
}
