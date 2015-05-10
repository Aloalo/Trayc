#version 330 core

in vec2 pixel;
out vec4 out_color;

uniform vec3 eye;
uniform vec3 U;
uniform vec3 W;
uniform vec3 V;

void main()
{
    vec2 d = pixel * 2.0 - 1.0;
    vec3 ray_direction = normalize(d.x * U + d.y * V + W);
    out_color = vec4(ray_direction, 1.0);
    
    vec3 center = vec3(0.0);
    float radius = 1.0;
    
    vec3 len =  center - eye;
    float tca = dot(len, ray_direction);

    if (tca < 0.0f)
        return;
        
    float d2 = dot(len, len) - tca * tca;

    if (d2 > radius * radius)
        return;

    float thc = sqrt(radius * radius - d2);

    out_color = vec4(1.0);
}
