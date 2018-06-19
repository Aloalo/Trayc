in vec2 clipSpaceCoords;
layout(location = 0) out vec3 outColor;

#include "UB_Primitives.glsl"

uniform vec3 ambientColor;
uniform float lightFallofFactor;

uniform vec3 cameraPos;
uniform vec3 U;
uniform vec3 V;
uniform vec3 W;

#define RAY_OFFSET 0.05

#ifdef CHECKERBOARDING
    uniform float invTexWidth;
#endif

#if CT_SPHERES
bool intersectSphereSimple(in vec3 origin, in vec3 direction, in vec4 positionRadius2, in float maxLambda)
{
	vec3 L = positionRadius2.xyz - origin;
	float t = dot(L, direction);
	
	if (t < 0.0) {
        return false;
    }
	float d2 = dot(L, L) - t * t;
	
	if (d2 > positionRadius2.w) {
        return false;
    }
    
    float lambda = t - sqrt(positionRadius2.w - d2);
    return lambda < maxLambda;
}
#endif

bool intersectSphere(in vec3 origin, in vec3 direction, in vec4 positionRadius2, inout float minLambda, inout vec3 N, inout vec3 P)
{
	vec3 L = positionRadius2.xyz - origin;
	float t = dot(L, direction);
	
	if (t < 0.0) {
        return false;
    }
	float d2 = dot(L, L) - t * t;
	
	if (d2 > positionRadius2.w) {
        return false;
    }

	float lambda = t - sqrt(positionRadius2.w - d2);
    
    if (lambda > minLambda) {
        return false;
    }
    
    minLambda = lambda;
    P = origin + lambda * direction;
    N = normalize(P - positionRadius2.xyz);
    
    return true;
}

#if CT_RECTANGLES
bool pointInRectangle(in vec3 point, in Rectangle rectangle, in vec3 v1, in vec3 v2)
{
    vec3 v4 = point - rectangle.p1.xyz;
    vec3 v5 = point - rectangle.p3.xyz;
    // p4 = rectangle.p1 + rectangle.p3 - rectangle.p2
    vec3 v6 = point - rectangle.p1.xyz - rectangle.p3.xyz + rectangle.p2.xyz;
    return !(dot(v1, v4) < 0.0 || dot(v1, v5) > 0.0 || dot(v2, v4) < 0.0 || dot(v2, v6) > 0.0);
}

bool intersectRectangleSimple(in vec3 origin, in vec3 direction, in Rectangle rectangle, in float maxLambda)
{
    vec3 v1 = rectangle.p2.xyz - rectangle.p1.xyz;
    vec3 v2 = rectangle.p3.xyz - rectangle.p2.xyz;
    vec3 normal = normalize(cross(v1, v2));
    
    float lambda = (dot(normal, rectangle.p1.xyz) - dot(origin, normal)) /  dot(direction, normal);
    
    if (lambda < 0.0 || lambda > maxLambda) {
        return false;
    }
    
    vec3 hitPoint = origin + lambda * direction;
    return pointInRectangle(hitPoint, rectangle, v1, v2);
}

bool intersectRectangle(in vec3 origin, in vec3 direction, in Rectangle rectangle, inout float minLambda, inout vec3 N, inout vec3 P)
{
    vec3 v1 = rectangle.p2.xyz - rectangle.p1.xyz;
    vec3 v2 = rectangle.p3.xyz - rectangle.p2.xyz;
    vec3 normal = normalize(cross(v1, v2));
    
    float lambda = (dot(normal, rectangle.p1.xyz) - dot(origin, normal)) /  dot(direction, normal);
    
    if (lambda < 0.0 || lambda > minLambda) {
        return false;
    }
    
    vec3 hitPoint = origin + lambda * direction;
    
    if (!pointInRectangle(hitPoint, rectangle, v1, v2)) {
        return false;
    }
	
    minLambda = lambda;
    P = hitPoint;
    N = normal;
    N *= -sign(dot(N, direction));
    
    return true;
}
#endif

#if CT_BOXES
vec3 boxnormal(in vec3 t0, in vec3 t1, in float t)
{
    vec3 neg = vec3(t == t0.x ? 1.0 : 0.0, t == t0.y ? 1.0 : 0.0, t == t0.z ? 1.0 : 0.0);
    vec3 pos = vec3(t == t1.x ? 1.0 : 0.0, t == t1.y ? 1.0 : 0.0, t == t1.z ? 1.0 : 0.0);
    return pos - neg;
}

bool intersectBox(in vec3 origin, in vec3 direction, in Box b, inout float minLambda, inout vec3 N, inout vec3 P)
{
    vec3 tmin = (b.minv - origin) / direction;
    vec3 tmax = (b.maxv - origin) / direction;
    
    vec3 near = min(tmin, tmax);
    vec3 far = max(tmin, tmax);
    float lmax = min(min(far.x, far.y), far.z);
    float lmin = max(max(near.x, near.y), near.z);
    
    lmin = mix(lmax, lmin, step(0.0, lmin));
    
    if(lmax < 0.0 || lmax < lmin || lmin > minLambda) {
        return false;
    }
    
    minLambda = lmin;
    P = origin + direction * lmin;
    N = normalize(boxnormal(tmin, tmax, lmin));
    N *= -sign(dot(N, direction));
    
    return true;
}

bool intersectBoxSimple(in vec3 origin, in vec3 direction, in Box b, in float maxLambda)
{
    vec3 tmin = (b.minv - origin) / direction;
    vec3 tmax = (b.maxv - origin) / direction;
    
    vec3 near = min(tmin, tmax);
    vec3 far = max(tmin, tmax);
    float lmax = min(min(far.x, far.y), far.z);
    float lmin = max(max(near.x, near.y), near.z);
    lmin = mix(lmax, lmin, step(0.0, lmin));
    
    return lmax > lmin && lmin < maxLambda;
}
#endif


bool anyHit(in vec3 origin, in vec3 direction, in float maxLambda)
{
#if CT_RECTANGLES
    for (int i = 0; i < CT_RECTANGLES; ++i) {
        if (intersectRectangleSimple(origin, direction, rectangles[i], maxLambda)) {
            return true;
        }
    }
#endif
    
#if CT_BOXES
    for (int i = 0; i < CT_BOXES; ++i) {
        if (intersectBoxSimple(origin, direction, boxes[i], maxLambda)) {
            return true;
        }
    }
#endif
    
#if CT_SPHERES
    for (int i = 0; i < CT_SPHERES; ++i) {
        if (intersectSphereSimple(origin, direction, spheres[i].positionRadius2, maxLambda)) {
            return true;
        }
    }
#endif
    
    return false;
}

vec3 blinnPhongShade(in vec4 diffuseSpecular, in vec3 intensity, in vec3 N, in vec3 L, in vec3 P, in float atten, in float gloss)
{
    vec3 lightIntensity = lightFallofFactor * atten * intensity;
    
    // Diffuse
    float dNL = max(0.0, dot(N, L));
    
    // Specular
    if(dNL > 0.0)
    {
        vec3 ret = dNL * diffuseSpecular.rgb * lightIntensity;
        vec3 V = normalize(cameraPos - P);
        vec3 H = normalize(V + L);
        float dNH = max(0.0, dot(N, H));
        
        return ret + diffuseSpecular.a * pow(dNH, gloss) * lightIntensity;
    }
    return vec3(0.0);
}

vec3 shade(in vec3 P, in vec3 N, in vec4 diffuseSpecular, in float gloss, in int skipRect)
{
    vec3 ret = ambientColor * diffuseSpecular.xyz;
    for (int i = 0; i < CT_LIGHTS; ++i) {
        Light light = lights[i];
        vec3 L = light.positionRadius2.xyz - P;
        float atten = 1.0 / length(L);
        L *= atten;
        
        if(anyHit(P + RAY_OFFSET * L, L, 1.0 / atten)) {
            continue;
        }
        
        ret += blinnPhongShade(diffuseSpecular, light.intensity, N, L, P, atten, gloss);
    }
    
#if CT_RECTANGLES
    for (int i = 0; i < CT_RECTANGLES; ++i) {
        if (i == skipRect) {
            continue;
        }
        
        Rectangle rect = rectangles[i];
        vec3 v1 = rect.p2.xyz - rect.p1.xyz;
        vec3 v2 = rect.p3.xyz - rect.p2.xyz;
        vec3 planeN = normalize(cross(v1, v2));
        
        float t2 = dot(planeN, P - rect.p1.xyz);
        vec3 projectedP = P - t2 * planeN;
        
        for (int j = 0; j < CT_LIGHTS; ++j) {
            Light light = lights[j];
            vec3 lightP = light.positionRadius2.xyz;
            
            // Are light and P on the same side of the plane
            float t1 = dot(planeN, lightP - rect.p1.xyz);
            if (t1 * t2 < 0.0) {
                continue;
            }
            
            vec3 projectedLightP = lightP - t1 * planeN;
            
            t1 = abs(t1);
            float t = t1 / (t1 + abs(t2));
            
            vec3 reflectionPoint = projectedLightP + t * (projectedP - projectedLightP);
            
            if (!pointInRectangle(reflectionPoint, rect, v1, v2)) {
                continue;
            }
            
            vec3 RtoLight = light.positionRadius2.xyz - reflectionPoint;
            float dist1 = length(RtoLight);
            RtoLight /= dist1;
            if(anyHit(reflectionPoint + RAY_OFFSET * RtoLight, RtoLight, dist1 - 2.0 * RAY_OFFSET)) {
                continue;
            }
            
            vec3 L = reflectionPoint - P;
            float dist2 = length(L);
            L /= dist2;
            if(anyHit(P + RAY_OFFSET * L, L, dist2 - 2.0 * RAY_OFFSET)) {
                continue;
            }
            
            float atten = rect.p2.w / (dist1 + dist2);
            ret += blinnPhongShade(diffuseSpecular, light.intensity, N, L, P, atten, gloss);
        }
    }
#endif
    return ret;
}

vec3 rayTrace(in vec3 origin, in vec3 direction, out vec3 new_origin, out vec3 new_direction, out float addFactor)
{
    vec4 diffuseSpecular;
    vec3 N, P, retColor = vec3(0.0);
    float minLambda = 100000.0;
    float gloss;
    int hitIdx = -1;
    int skipRect = -1;
    
    //////////////////////////////////////////////////
    // --------------- RECTANGLES ----------------- //
    //////////////////////////////////////////////////
#if CT_RECTANGLES
    for (int i = 0; i < CT_RECTANGLES; ++i) {
        if (intersectRectangle(origin, direction, rectangles[i], minLambda, N, P)) {
            hitIdx = i;
        }
    }
    
    if (hitIdx != -1) {
        Rectangle rectangle = rectangles[hitIdx];
        diffuseSpecular = vec4(rectangle.diffuse, rectangle.p3.w);
        gloss = rectangle.p1.w;
        addFactor = rectangle.p2.w;
        skipRect = hitIdx;
        hitIdx = -1;
    }
#endif
    
    //////////////////////////////////////////////////
    // ------------------ BOXES ------------------- //
    //////////////////////////////////////////////////
#if CT_BOXES
    for (int i = 0; i < CT_BOXES; ++i) {
        if (intersectBox(origin, direction, boxes[i], minLambda, N, P)) {
            hitIdx = i;
        }
    }
    
    if (hitIdx != -1) {
        Box box = boxes[hitIdx];
        diffuseSpecular = box.diffuseSpecular;
        gloss = box.gloss;
        addFactor = box.reflectivity;
        hitIdx = skipRect = -1;
    }
#endif
    
    //////////////////////////////////////////////////
    // ----------------- SPHERES ------------------ //
    //////////////////////////////////////////////////
#if CT_SPHERES
    for (int i = 0; i < CT_SPHERES; ++i) {
        if (intersectSphere(origin, direction, spheres[i].positionRadius2, minLambda, N, P)) {
            hitIdx = i;
        }
    }
    
    if (hitIdx != -1) {
        Sphere sphere = spheres[hitIdx];
        diffuseSpecular = sphere.diffuseSpecular;
        gloss = sphere.materialData.r;
        addFactor = sphere.materialData.y;
        hitIdx = skipRect = -1;
    }
#endif
    
    //////////////////////////////////////////////////
    // ----------------- LIGHTS ------------------- //
    //////////////////////////////////////////////////
    for (int i = 0; i < CT_LIGHTS; ++i) {
        if (intersectSphere(origin, direction, lights[i].positionRadius2, minLambda, N, P)) {
            hitIdx = i;
        }
    }
    
    if (hitIdx != -1) {
        retColor = lights[hitIdx].intensity;
        addFactor = -1.0;
    }
    else {
        retColor = shade(P, N, diffuseSpecular, gloss, skipRect);
        if (addFactor > 0.0) {
            new_direction = reflect(direction, N);
            new_origin = P;
        }
    }
    
    return retColor;
}

vec3 rayTrace_2(in vec3 origin, in vec3 direction)
{
    vec3 new_origin;
    vec3 new_direction;
    float addFactor;
    return rayTrace(origin, direction, new_origin, new_direction, addFactor);
}

#define RAY_TRACE_MACRO(DEPTH, NEXT) \
vec3 rayTrace_##DEPTH(in vec3 origin, in vec3 direction) \
{ \
    vec3 new_origin; \
    vec3 new_direction; \
    float addFactor = -1.0; \
    vec3 color = rayTrace(origin + RAY_OFFSET * direction, direction, new_origin, new_direction, addFactor); \
    \
    if (addFactor > 0.0) { \
        color = mix(color, rayTrace_##NEXT(new_origin + RAY_OFFSET * new_direction, new_direction), addFactor); \
    } \
    \
    return color; \
}

// RAY_TRACE_MACRO(3, 4)
// RAY_TRACE_MACRO(2, 3)
RAY_TRACE_MACRO(1, 2)
RAY_TRACE_MACRO(0, 1)

void main()
{
#ifdef CHECKERBOARDING
    float windowSpaceXOffset = mod((gl_FragCoord.y - mod(gl_FragCoord.y, 2.0)) / 2.0, 2.0) * 2.0;
    float clipSpaceX = (gl_FragCoord.x * 2.0 - mod(gl_FragCoord.x, 2.0) + windowSpaceXOffset) * invTexWidth * 2.0 - 1.0;
    vec3 viewRay = normalize(clipSpaceX * U + clipSpaceCoords.y * V + W);
#else
    vec3 viewRay = normalize(clipSpaceCoords.x * U + clipSpaceCoords.y * V + W);
#endif
    
    outColor = rayTrace_0(cameraPos, viewRay);
}
