
struct Light
{
    vec3 intensity;
    vec3 attenuation;
    vec3 position;
};
uniform Light light;

vec3 GetLightDir(in vec3 fragPos)
{
    return normalize(light.position - fragPos);
}

float GetLightAttenuation(in vec3 fragPos)
{
    float distance = length(light.position - fragPos);
    float atten = light.attenuation.x + 
                distance * light.attenuation.y + 
                distance * distance * light.attenuation.z;
    return 1.0f / atten;
}

float GetShadowFactor(in vec3 fragPos, in float dotNL)
{
    return 1.0;
}
