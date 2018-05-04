
#if CT_SPHERES
    struct Sphere
    {
        vec4 positionRadius;
        vec4 diffuseSpecular;
        vec2 materialData; // gloss, reflectivity
        float phi;
        float theta;
    };
#endif

struct Light
{
    vec4 positionRadius;
    vec3 intensity;
    // 4 bytes of padding here
};

#if CT_RECTANGLES
    struct Rectangle
    {
        vec4 rect;
        vec4 diffuseSpecular;
        vec2 materialData;
        float offset;
        int normal;
    };
#endif

#if CT_BOXES
    struct Box
    {
        vec4 diffuseSpecular;
        vec3 minv;
        float reflectivity;
        vec3 maxv;
        float gloss;
    };
#endif


layout(std140) uniform Primitives
{
#if CT_RECTANGLES
    Rectangle rectangles[CT_RECTANGLES];
#endif
#if CT_BOXES
    Box boxes[CT_BOXES];
#endif
#if CT_SPHERES
    Sphere spheres[CT_SPHERES];
#endif
Light lights[CT_LIGHTS];
};
