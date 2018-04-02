in vec2 clipSpaceCoords;
layout(location = 0) out vec4 outColor;

#include "UB_Primitives.glsl"
uniform int ctSpheres;

uniform vec3 cameraPos;
uniform vec3 U;
uniform vec3 V;
uniform vec3 W;

bool intersectSphere(in vec3 origin, in vec3 direction, in Sphere sphere, inout float minLambda, out vec3 normal)
{
	vec3 L = sphere.positionRadius.xyz - origin;
	float t = dot(L, direction);
	
	if (t < 0.0) {
        return false;
    }
	float d2 = dot(L, L) - t * t;
	
	float radius2 = sphere.positionRadius.w * sphere.positionRadius.w;
	if (d2 > radius2) {
        return false;
    }

	float lambda = t - sqrt(radius2 - d2);
    
    if (lambda > minLambda) {
        return false;
    }
    
    minLambda = lambda;
    normal = (origin + lambda * direction - sphere.positionRadius.xyz) / sphere.positionRadius.w;
    
    return true;
}

void rayTrace(in vec3 origin, in vec3 direction)
{
    vec3 normal;
    float minLambda = 100000.0;
    for (int i = 0; i < ctSpheres; ++i) {
        if(intersectSphere(origin, direction, spheres[i], minLambda, normal)) {
            outColor = vec4(dot(normal, vec3(1.0)));
        }
    }
}

void main()
{
    vec3 viewRay = normalize(clipSpaceCoords.x * U + clipSpaceCoords.y * V + W);
    outColor = vec4(0.0);
    rayTrace(cameraPos, viewRay);

    //outColor = outColor * 0.0001 + (dot(-normalize(cameraPos), normalize(viewRay)) > 0.98 ? vec4(1.0) : vec4(0.0));
}
