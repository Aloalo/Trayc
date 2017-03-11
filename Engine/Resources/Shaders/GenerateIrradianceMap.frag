in vec2 uv;

layout(location = 0) out vec4 fragColor;

uniform samplerCube inTex1;
uniform int cubeFace;

void main()
{
    vec2 p = uv * 2.0 - 1.0;
    vec3 normal;
    switch(cubeFace)
    {
    case 1:
        normal = normalize(vec3(p.xy, 1.0));
        break;
    case 2:
        normal = normalize(vec3(p.x, 1.0, -p.y));
        break;
    case 3:
        normal = normalize(vec3(p.x, -1.0,  p.y));
        break;
    case 5:
        normal = normalize(vec3(1.0, p.y, -p.x));
        break;
    case 4:
        normal = normalize(vec3(-1.0, p.y, p.x));
        break;
    case 0:
        normal = normalize(vec3(-p.x, p.y, -1.0));
        break;
    default:
        break;
    }
    normal.y = -normal.y;

    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = normalize(cross(up, normal));
    up = cross(normal, right);

    vec3 sum = vec3(0.0, 0.0, 0.0);
    float index = 0.0;
    for(float phi = 0.0; phi < 6.283; phi += 0.02)
    {
        for(float theta = 0.0; theta < 1.57; theta += 0.02)
        {
            vec3 temp = cos(phi) * right + sin(phi) * up;
            vec3 sampleVector = cos(theta) * normal + sin(theta) * temp;
            sum += texture(inTex1, sampleVector).rgb * cos(theta) * sin(theta);
            index += 1.0;
        }
    }

    const float PI = 3.1415;
    //fragColor = vec4(PI * sum / index, 1.0) * 0.0001 + vec4(texture(inTex1, normal).rgb, 1.0);
    fragColor = vec4(PI * sum / index, 1.0);
}
