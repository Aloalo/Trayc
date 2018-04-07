in vec2 clipSpaceCoords;
layout(location = 0) out vec3 outColor;

#include "UB_Primitives.glsl"
uniform int ctSpheres;
uniform int ctLights;

uniform vec3 ambientColor;
uniform float lightFallofFactor;

uniform vec3 cameraPos;
uniform vec3 U;
uniform vec3 V;
uniform vec3 W;

bool intersectSphere(in vec3 origin, in vec3 direction, in vec4 positionRadius, inout float minLambda, out vec3 N, out vec3 P)
{
	vec3 L = positionRadius.xyz - origin;
	float t = dot(L, direction);
	
	if (t < 0.0) {
        return false;
    }
	float d2 = dot(L, L) - t * t;
	
	float radius2 = positionRadius.w * positionRadius.w;
	if (d2 > radius2) {
        return false;
    }

	float lambda = t - sqrt(radius2 - d2);
    
    if (lambda > minLambda) {
        return false;
    }
    
    minLambda = lambda;
    P = origin + lambda * direction;
    N = (P - positionRadius.xyz) / positionRadius.w;
    
    return true;
}

vec3 shade(in vec3 P, in vec3 N, in vec4 diffuseSpecular, in vec4 materailData)
{
    vec3 ret = ambientColor;
    for (int i = 0; i < ctLights; ++i) {
        Light light = lights[i];
        vec3 L = light.positionRadius.xyz - P;
        float atten = 1.0 / length(L);
        L *= atten;
        vec3 lightIntensity = lightFallofFactor * atten * atten * light.intensity;
        
        // Diffuse
        float dNL = max(0.0, dot(N, L));
        ret += dNL * diffuseSpecular.rgb * lightIntensity;
        
        // Specular
        if(dNL > 0.0)
        {
            vec3 V = normalize(cameraPos - P);
            vec3 H = normalize(V + L);
            float dotNH = max(0.0, dot(N, H));
            ret += diffuseSpecular.a * pow(dotNH, materailData.r) * lightIntensity;
        }
    }
    
    return ret;
}

void rayTrace(in vec3 origin, in vec3 direction)
{
    vec3 N, P;
    float minLambda = 100000.0;
    int hitIdx = -1;
    for (int i = 0; i < ctSpheres; ++i) {
        if (intersectSphere(origin, direction, spheres[i].positionRadius, minLambda, N, P)) {
            hitIdx = i;
        }
    }
    
    if(hitIdx != -1) {
        outColor = shade(P, N, spheres[hitIdx].diffuseSpecular, spheres[hitIdx].materailData);
        hitIdx = -1;
    }
    
    for (int i = 0; i < ctLights; ++i) {
        if (intersectSphere(origin, direction, lights[i].positionRadius, minLambda, N, P)) {
            hitIdx = i;
        }
    }
    
    if(hitIdx != -1) {
        outColor = lights[hitIdx].intensity;
    }
}

void main()
{
    vec3 viewRay = normalize(clipSpaceCoords.x * U + clipSpaceCoords.y * V + W);
    outColor = vec3(0.0);
    rayTrace(cameraPos, viewRay);

    //outColor = outColor * 0.0001 + (dot(-normalize(cameraPos), normalize(viewRay)) > 0.98 ? vec4(1.0) : vec4(0.0));
}
