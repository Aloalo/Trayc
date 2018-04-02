in vec2 clipSpaceCoords;
layout(location = 0) out vec4 outColor;

uniform vec3 cameraPos;
uniform vec3 U;
uniform vec3 V;
uniform vec3 W;

bool intersectSphere(in vec4 sphere, in vec3 viewRay, inout float minLambda, out vec3 normal)
{
	vec3 L = sphere.xyz - cameraPos;
	float t = dot(L, viewRay);
	
	if (t < 0.0) {
        return false;
    }
	float d2 = dot(L, L) - t * t;
	
	float radius2 = sphere.w * sphere.w;
	if (d2 > radius2) {
        return false;
    }

	float lambda = t - sqrt(radius2 - d2);
    
    if (lambda > minLambda) {
        return false;
    }
    
    minLambda = lambda;
    normal = normalize(cameraPos + lambda * viewRay - sphere.xyz);
    
    return true;
}

vec4 spheres[4] = vec4[](vec4(0.0, 0.0, 0.0, 1.0), vec4(2.0, 0.0, 0.0, 1.0), vec4(-2.0, 0.0, 0.0, 1.0), vec4(0.0, 0.0, 2.0, 1.0));

void main()
{
    vec3 viewRay = normalize(clipSpaceCoords.x * U + clipSpaceCoords.y * V + W);
    
    outColor = vec4(0.0);
    vec3 normal;
    float minLambda = 100000.0;
    for (int i = 0; i < spheres.length(); ++i) {
        if(intersectSphere(spheres[i], viewRay, minLambda, normal)) {
            outColor = vec4(dot(normal, vec3(1.0)));
        }
    }

    //outColor = outColor * 0.0001 + (dot(-normalize(cameraPos), normalize(viewRay)) > 0.98 ? vec4(1.0) : vec4(0.0));
}
