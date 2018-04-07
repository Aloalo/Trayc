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

bool intersectSphereSimple(in vec3 origin, in vec3 direction, in vec4 positionRadius)
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
    
    return true;
}

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

bool anyHit(in vec3 origin, in vec3 direction)
{
    for (int i = 0; i < ctSpheres; ++i) {
        if (intersectSphereSimple(origin, direction, spheres[i].positionRadius)) {
            return true;
        }
    }
    return false;
}

vec3 shade(in vec3 P, in vec3 N, in vec4 diffuseSpecular, in vec4 materailData)
{
    vec3 ret = ambientColor;
    for (int i = 0; i < ctLights; ++i) {
        Light light = lights[i];
        vec3 L = light.positionRadius.xyz - P;
        float atten = 1.0 / length(L);
        L *= atten;
        
        if(anyHit(P, L)) {
            continue;
        }
        
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

vec3 rayTrace(in vec3 origin, in vec3 direction, out vec3 new_origin, out vec3 new_direction, out float addFactor)
{
    vec3 N, P, retColor = vec3(0.0);
    float minLambda = 100000.0;
    int hitIdx = -1;
    for (int i = 0; i < ctSpheres; ++i) {
        if (intersectSphere(origin, direction, spheres[i].positionRadius, minLambda, N, P)) {
            hitIdx = i;
        }
    }
    
    if (hitIdx != -1) {
        Sphere sphere = spheres[hitIdx];
        retColor = shade(P, N, sphere.diffuseSpecular, sphere.materailData);
        hitIdx = -1;
        if (sphere.materailData.y > 0.0) {
            new_origin = P;
            new_direction = reflect(direction, N);
            addFactor = sphere.materailData.y;
        }
    }
    
    for (int i = 0; i < ctLights; ++i) {
        if (intersectSphere(origin, direction, lights[i].positionRadius, minLambda, N, P)) {
            hitIdx = i;
        }
    }
    
    if (hitIdx != -1) {
        retColor = lights[hitIdx].intensity;
        new_direction = vec3(0.0);
    }
    
    return retColor;
}

vec3 rayTrace_4(in vec3 origin, in vec3 direction)
{
    vec3 new_origin;
    vec3 new_direction;
    float addFactor;
    vec3 color = rayTrace(origin, direction, new_origin, new_direction, addFactor);
    return color;
}

#define RAY_TRACE_MACRO(DEPTH, NEXT) \
vec3 rayTrace_##DEPTH(in vec3 origin, in vec3 direction) \
{ \
    vec3 new_origin; \
    vec3 new_direction; \
    float addFactor = 0.0; \
    vec3 color = rayTrace(origin, direction, new_origin, new_direction, addFactor); \
    \
    if (addFactor > 0.0) { \
        color += addFactor * rayTrace_##NEXT(new_origin, new_direction); \
    } \
    \
    return color; \
}

RAY_TRACE_MACRO(3, 4)
RAY_TRACE_MACRO(2, 3)
RAY_TRACE_MACRO(1, 2)
RAY_TRACE_MACRO(0, 1)

void main()
{
    vec3 viewRay = normalize(clipSpaceCoords.x * U + clipSpaceCoords.y * V + W);
    outColor = rayTrace_0(cameraPos, viewRay);

    //outColor = outColor * 0.0001 + (dot(-normalize(cameraPos), normalize(viewRay)) > 0.98 ? vec4(1.0) : vec4(0.0));
}
