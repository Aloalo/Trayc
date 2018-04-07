
struct Sphere
{
    vec4 positionRadius;
    vec4 diffuseSpecular;
    vec4 materailData; // gloss, reflectivity, IoR
};

struct Light
{
    vec4 positionRadius;
    vec3 intensity;
};

layout(std140) uniform Primitives
{
    Sphere spheres[MAX_SPHERES];
    Light lights[MAX_LIGHTS];
};
