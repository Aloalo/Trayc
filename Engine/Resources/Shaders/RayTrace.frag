in vec2 clipSpaceCoords;
layout(location = 0) out vec3 outColor;

#include "UB_Primitives.glsl"
uniform int ctSpheres;
uniform int ctLights;
uniform int ctRectangles;
uniform int ctBoxes;

uniform vec3 ambientColor;
uniform float lightFallofFactor;

uniform vec3 cameraPos;
uniform vec3 U;
uniform vec3 V;
uniform vec3 W;

bool intersectSphereSimple(in vec3 origin, in vec3 direction, in vec4 positionRadius, in float maxLambda)
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
    if (lambda > maxLambda) {
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

vec3 RECT_NORMALS[3] = vec3[](
    vec3(1.0, 0.0, 0.0),
    vec3(0.0, 1.0, 0.0),
    vec3(0.0, 0.0, 1.0)
);

bool intersectRectangleSimple(in vec3 origin, in vec3 direction, in Rectangle rectangle, in float maxLambda)
{
    float lambda = (rectangle.offset - origin[rectangle.normal]) / direction[rectangle.normal];
    
    if (lambda < 0.0 || lambda > maxLambda) {
        return false;
    }
    
    vec3 hitPoint = origin + lambda * direction;
    vec2 planePoint = vec2(hitPoint[(rectangle.normal + 1) % 3], hitPoint[(rectangle.normal + 2) % 3]);
    
    if (any(lessThan(planePoint, rectangle.rect.xy)) || any(greaterThan(planePoint, rectangle.rect.zw))) {
        return false;
    }
    
    return true;
}

bool intersectRectangle(in vec3 origin, in vec3 direction, in Rectangle rectangle, inout float minLambda, out vec3 N, out vec3 P)
{
    float lambda = (rectangle.offset - origin[rectangle.normal]) / direction[rectangle.normal];
    
    if (lambda < 0.0 || lambda > minLambda) {
        return false;
    }
    
    vec3 hitPoint = origin + lambda * direction;
    // The values for the X-Z plane should be swizzled for Rectangle.rect.xy, Rectangle.rect.zw
    vec2 planePoint = vec2(hitPoint[(rectangle.normal + 1) % 3], hitPoint[(rectangle.normal + 2) % 3]);
    
    if (any(lessThan(planePoint, rectangle.rect.xy)) || any(greaterThan(planePoint, rectangle.rect.zw))) {
        return false;
    }
	
    minLambda = lambda;
    P = hitPoint;
    N = RECT_NORMALS[rectangle.normal];
    N *= -sign(dot(N, direction));
    
    return true;
}

vec3 boxnormal(in vec3 t0, in vec3 t1, in float t)
{
    vec3 neg = vec3(t == t0.x ? 1.0f : 0.0f, t == t0.y ? 1.0f : 0.0f, t == t0.z ? 1.0f : 0.0f);
    vec3 pos = vec3(t == t1.x ? 1.0f : 0.0f, t == t1.y ? 1.0f : 0.0f, t == t1.z ? 1.0f : 0.0f);
    return pos - neg;
}

bool intersectBox(in vec3 origin, in vec3 direction, in Box b, inout float minLambda, out vec3 N, out vec3 P)
{
    vec3 tmin = (b.minv - origin) / direction;
    vec3 tmax = (b.maxv - origin) / direction;
    
    vec3 near = min(tmin, tmax);
    vec3 far = max(tmin, tmax);
    float lmin = max(0.0, max(max(near.x, near.y), near.z));
    float lmax = min(min(far.x, far.y), far.z);
    
    if(lmin < 0.0 || lmax < lmin || lmin > minLambda) {
        return false;
    }
    
    minLambda = lmin;
    P = origin + direction * lmin;
    N = boxnormal(tmin, tmax, lmin);
    
    return true;
}

bool intersectBoxSimple(in vec3 origin, in vec3 direction, in Box b, in float maxLambda)
{
    vec3 tmin = (b.minv - origin) / direction;
    vec3 tmax = (b.maxv - origin) / direction;
    
    vec3 near = min(tmin, tmax);
    vec3 far = max(tmin, tmax);
    float lmin = max(0.0, max(max(near.x, near.y), near.z));
    float lmax = min(min(far.x, far.y), far.z);
    
    return lmax > lmin && lmin < maxLambda;
}


bool anyHit(in vec3 origin, in vec3 direction, in float maxLambda)
{
    for (int i = 0; i < ctSpheres; ++i) {
        if (intersectSphereSimple(origin, direction, spheres[i].positionRadius, maxLambda)) {
            return true;
        }
    }
    
    for (int i = 0; i < ctRectangles; ++i) {
        if (intersectRectangleSimple(origin, direction, rectangles[i], maxLambda)) {
            return true;
        }
    }
    
    for (int i = 0; i < ctBoxes; ++i) {
        if (intersectBoxSimple(origin, direction, boxes[i], maxLambda)) {
            return true;
        }
    }
    
    return false;
}

vec3 shade(in vec3 P, in vec3 N, in vec4 diffuseSpecular, in float gloss)
{
    vec3 ret = ambientColor;
    for (int i = 0; i < ctLights; ++i) {
        Light light = lights[i];
        vec3 L = light.positionRadius.xyz - P;
        float atten = 1.0 / length(L);
        L *= atten;
        
        // TODO(jure): proper offsetting
        if(anyHit(P + L * 0.1, L, 1.0 / atten)) {
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
            ret += diffuseSpecular.a * pow(dotNH, gloss) * lightIntensity;
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
        retColor = shade(P, N, sphere.diffuseSpecular, sphere.materailData.r);
        hitIdx = -1;
        if (sphere.materailData.y > 0.0) {
            new_origin = P;
            new_direction = reflect(direction, N);
            addFactor = sphere.materailData.y;
        }
    }
    
    for (int i = 0; i < ctRectangles; ++i) {
        if (intersectRectangle(origin, direction, rectangles[i], minLambda, N, P)) {
            hitIdx = i;
        }
    }
    
    if (hitIdx != -1) {
        Rectangle rectangle = rectangles[hitIdx];
        retColor = shade(P, N, rectangle.diffuseSpecular, rectangle.materailData.r);
        hitIdx = -1;
        
        addFactor = -1.0;
        if (rectangle.materailData.y > 0.0) {
            new_origin = P;
            new_direction = reflect(direction, N);
            addFactor = rectangle.materailData.y;
        }
    }
    
    for (int i = 0; i < ctBoxes; ++i) {
        if (intersectBox(origin, direction, boxes[i], minLambda, N, P)) {
            hitIdx = i;
        }
    }
    
    if (hitIdx != -1) {
        Box box = boxes[hitIdx];
        retColor = shade(P, N, box.diffuseSpecular, box.materailData.r);
        hitIdx = -1;
        
        addFactor = -1.0;
        if (box.materailData.y > 0.0) {
            new_origin = P;
            new_direction = reflect(direction, N);
            addFactor = box.materailData.y;
        }
    }
    
    
    for (int i = 0; i < ctLights; ++i) {
        if (intersectSphere(origin, direction, lights[i].positionRadius, minLambda, N, P)) {
            hitIdx = i;
        }
    }
    
    if (hitIdx != -1) {
        retColor = lights[hitIdx].intensity;
        addFactor = -1.0;
    }
    
    return retColor;
}

vec3 rayTrace_4(in vec3 origin, in vec3 direction)
{
    vec3 new_origin;
    vec3 new_direction;
    float addFactor;
    vec3 color = rayTrace(origin + direction * 0.1, direction, new_origin, new_direction, addFactor);
    return color;
}

#define RAY_TRACE_MACRO(DEPTH, NEXT) \
vec3 rayTrace_##DEPTH(in vec3 origin, in vec3 direction) \
{ \
    vec3 new_origin; \
    vec3 new_direction; \
    float addFactor = -1.0; \
    vec3 color = rayTrace(origin + direction * 0.1, direction, new_origin, new_direction, addFactor); \
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
