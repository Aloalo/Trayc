
struct Light
{
    vec3 intensity;
    vec3 attenuation;
    vec3 position;
    vec3 spotDirection;
    float cosSpotCutoff;
    float spotExponent;
};
uniform Light light;


/*float clampedCosine = max(0.0, dot(-lightDirection, light0.spotDirection));
	  if (clampedCosine < cos(radians(light0.spotCutoff))) // outside of spotlight cone?
	    {
	      attenuation = 0.0;
	    }
	  else
	    {
	      attenuation = attenuation * pow(clampedCosine, light0.spotExponent);   
	    }
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
    float dLS = max(0.0, dot(-GetLightDir(fragPos), light.spotDirection));
    
    return step(dLS, light.cosSpotCutoff) * atten * pow(dLS, light.spotExponent);
}
