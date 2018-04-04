
struct Sphere
{
    vec4 positionRadius;
    vec4 diffuseSpecular;
    vec4 materailData;
};

layout(std140) uniform Primitives
{
    Sphere spheres[MAX_SPHERES];
};
