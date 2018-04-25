
struct Sphere
{
    vec4 positionRadius;
    vec4 diffuseSpecular;
    vec2 materialData; // gloss, reflectivity
    float phi;
    float theta;
};

struct Light
{
    vec4 positionRadius;
    vec3 intensity;
    // 4 bytes of padding here
};

struct Rectangle
{
    vec4 rect;
    vec4 diffuseSpecular;
    vec2 materialData;
    float offset;
    int normal;
};

struct Box
{
    vec4 diffuseSpecular;
    vec3 minv;
    float reflectivity;
    vec3 maxv;
    float gloss;
};

layout(std140) uniform Primitives
{
    Rectangle rectangles[MAX_RECTANGLES];
    Box boxes[MAX_BOXES];
    Sphere spheres[MAX_SPHERES];
    Light lights[MAX_LIGHTS];
};
