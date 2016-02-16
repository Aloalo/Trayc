
struct Light
{
    vec3 intensity;
    vec3 attenuation;
    vec3 position;
    vec3 spotDir;
    float cosSpotCutoff;
    float spotExp;
};
uniform Light light;


/*
float clampedCosine = max(0.0, dot(-lightDirection, light0.spotDir));
if (clampedCosine < cos(radians(light0.spotCutoff))) // outside of spotlight cone?
{
    attenuation = 0.0;
}
else
{
    attenuation = attenuation * pow(clampedCosine, light0.spotExp);   
}
*/
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
    atten = 1.0 / atten;
    float dLS = max(0.0, dot(-GetLightDir(fragPos), light.spotDir));
    
    return step(light.cosSpotCutoff, dLS) * atten * pow(dLS, light.spotExp);
}
