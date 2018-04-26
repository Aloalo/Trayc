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

bool intersectSphere(in vec3 origin, in vec3 direction, in vec4 positionRadius, inout float minLambda, inout vec3 N, inout vec3 P)
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

bool intersectRectangle(in vec3 origin, in vec3 direction, in Rectangle rectangle, inout float minLambda, inout vec3 N, inout vec3 P)
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

bool intersectBox(in vec3 origin, in vec3 direction, in Box b, inout float minLambda, inout vec3 N, inout vec3 P)
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
    for (int i = 0; i < CT_RECTANGLES; ++i) {
        if (intersectRectangleSimple(origin, direction, rectangles[i], maxLambda)) {
            return true;
        }
    }
    
    for (int i = 0; i < CT_BOXES; ++i) {
        if (intersectBoxSimple(origin, direction, boxes[i], maxLambda)) {
            return true;
        }
    }
    
    for (int i = 0; i < CT_SPHERES; ++i) {
        if (intersectSphereSimple(origin, direction, spheres[i].positionRadius, maxLambda)) {
            return true;
        }
    }
    
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
        float dotNH = max(0.0, dot(N, H));
        return ret + diffuseSpecular.a * pow(dotNH, gloss) * lightIntensity;
    }
    return vec3(0.0);
}

vec3 shade(in vec3 P, in vec3 N, in vec4 diffuseSpecular, in float gloss, in int skipRect)
{
    vec3 ret = ambientColor * diffuseSpecular.xyz;
    for (int i = 0; i < CT_LIGHTS; ++i) {
        Light light = lights[i];
        vec3 L = light.positionRadius.xyz - P;
        float atten = 1.0 / length(L);
        L *= atten;
        
        if(anyHit(P + RAY_OFFSET * L, L, 1.0 / atten)) {
            continue;
        }
        
        ret += blinnPhongShade(diffuseSpecular, light.intensity, N, L, P, atten, gloss);
    }
    
    for (int i = 0; i < CT_REFLECTIVE_RECT; ++i) {
        if (i == skipRect) {
            continue;
        }
        
        Rectangle rect = rectangles[i];
        vec3 planeN = RECT_NORMALS[rect.normal];
        vec3 planeP = vec3(0.0);
        planeP[rect.normal] = rect.offset;
        
        for (int j = 0; j < CT_LIGHTS; ++j) {
            Light light = lights[j];
            vec3 lightP = light.positionRadius.xyz;
            
            // Are light and P on the same side of the plane
            if (sign(dot(planeN, planeP - P)) != sign(dot(planeN, planeP - lightP))) {
                continue;
            }
            
            // ret = ret * 0.0001 + vec3(0,1,0);
            
            float yl = abs(lightP[rect.normal] - rect.offset);
            float y = abs(P[rect.normal] - rect.offset);
            
            vec3 projP = P;
            lightP[rect.normal] = projP[rect.normal] = rect.offset;
            float x = distance(lightP, projP);
            float x1 = yl / (y + yl);
            
            vec3 reflectionPoint = lightP + (projP - lightP) * x1;
            
            vec2 planePoint = vec2(reflectionPoint[(rect.normal + 1) % 3], reflectionPoint[(rect.normal + 2) % 3]);
            if (any(lessThan(planePoint, rect.rect.xy)) || any(greaterThan(planePoint, rect.rect.zw))) {
                continue;
            }
            
            vec3 RtoLight = light.positionRadius.xyz - reflectionPoint;
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
            
            float atten = 1.0 / (dist1 + dist2);
            ret += blinnPhongShade(diffuseSpecular, light.intensity, N, L, P, atten, gloss);
        }
    }
    
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
    for (int i = 0; i < CT_RECTANGLES; ++i) {
        if (intersectRectangle(origin, direction, rectangles[i], minLambda, N, P)) {
            hitIdx = i;
        }
    }
    
    if (hitIdx != -1) {
        Rectangle rectangle = rectangles[hitIdx];
        diffuseSpecular = rectangle.diffuseSpecular;
        gloss = rectangle.materialData.r;
        addFactor = rectangle.materialData.y;
        skipRect = hitIdx;
        hitIdx = -1;
    }
    
    //////////////////////////////////////////////////
    // ------------------ BOXES ------------------- //
    //////////////////////////////////////////////////
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
    
    //////////////////////////////////////////////////
    // ----------------- SPHERES ------------------ //
    //////////////////////////////////////////////////
    for (int i = 0; i < CT_SPHERES; ++i) {
        if (intersectSphere(origin, direction, spheres[i].positionRadius, minLambda, N, P)) {
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
    
    //////////////////////////////////////////////////
    // ----------------- LIGHTS ------------------- //
    //////////////////////////////////////////////////
    for (int i = 0; i < CT_LIGHTS; ++i) {
        if (intersectSphere(origin, direction, lights[i].positionRadius, minLambda, N, P)) {
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
    return rayTrace(origin + RAY_OFFSET * direction, direction, new_origin, new_direction, addFactor);
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
        color += addFactor * rayTrace_##NEXT(new_origin + RAY_OFFSET * new_direction, new_direction); \
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
    vec3 viewRay = normalize(clipSpaceCoords.x * U + clipSpaceCoords.y * V + W);
    outColor = rayTrace_0(cameraPos, viewRay);

    //outColor = outColor * 0.0001 + (dot(-normalize(cameraPos), normalize(viewRay)) > 0.98 ? vec4(1.0) : vec4(0.0));
}
