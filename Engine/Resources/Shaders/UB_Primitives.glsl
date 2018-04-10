
struct Sphere
{
    vec4 positionRadius;
    vec4 diffuseSpecular;
    vec4 materailData; // gloss, reflectivity, refracticity, IoR
};

struct Light
{
    vec4 positionRadius;
    vec3 intensity;
};

struct Rectangle
{
    vec4 rect;
    vec4 diffuseSpecular;
    vec2 materailData;
    float offset;
    int normal;
};

layout(std140) uniform Primitives
{
    Rectangle rectangles[MAX_RECTANGLES];
    Sphere spheres[MAX_SPHERES];
    Light lights[MAX_LIGHTS];
};
