#version 330 core

in vec2 pixel;
out vec3 outColor;

//Camera
uniform vec3 eye;
uniform vec3 U;
uniform vec3 W;
uniform vec3 V;

//Bounds
uniform float Xmin;
uniform float Xmax;
uniform float Zmin;
uniform float Zmax;
uniform float Lstep;

//Material
uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

//Scene
uniform vec3 lightDirection;
uniform vec3 lightIntensity;
uniform vec3 missColor;


float Fx(in vec2 p)
{
    return #Fx;
}

float Fy(in vec2 p)
{
    return #Fy;
}

float F(in vec2 p)
{
    return #F;
}

vec3 getNormal(in vec2 p)
{
    return normalize(vec3(-Fx(p), 1.0, -Fy(p)));
}

//get min and max lambda
void getBounds(in vec3 d, out float Lmin, out float Lmax)
{
    float L, z, x;

    L = (Xmin - eye.x) / d.x;
    z = eye.z + L * d.z;
    if(L > 0.0 && z >= Zmin && z <= Zmax)
    {
        Lmin = min(Lmin, L);
        Lmax = max(Lmax, L);
    }
    
    L = (Xmax - eye.x) / d.x;
    z = eye.z + L * d.z;
    if(L > 0.0 && z >= Zmin && z <= Zmax)
    {
        Lmin = min(Lmin, L);
        Lmax = max(Lmax, L);
    }
    
    L = (Zmin - eye.z) / d.z;
    x = eye.x + L * d.x;
    if(L > 0.0 && x >= Xmin && x <= Xmax)
    {
        Lmin = min(Lmin, L);
        Lmax = max(Lmax, L);
    }
    
    L = (Zmax - eye.z) / d.z;
    x = eye.x + L * d.x;
    if(L > 0.0 && x >= Xmin && x <= Xmax)
    {
        Lmin = min(Lmin, L);
        Lmax = max(Lmax, L);
    }
}

vec3 rayPoint(in vec3 d, in float L)
{
    return eye + L * d;
}

bool intersect(in vec3 d, in float Lmin, in float Lmax, out vec3 intersection_point)
{
    vec3 point_on_ray = rayPoint(d, Lmin);
    float point_on_F = F(point_on_ray.xz);
    for(float L = Lmin + Lstep; L < Lmax; L += Lstep)
    {
        vec3 new_point = rayPoint(d, L);
        float new_F = F(new_point.xz);
        
        if(sign(point_on_ray.y - point_on_F) != sign(new_point.y - new_F))
        {
            intersection_point = (point_on_ray + new_point) * 0.5;
            return true;
        }
            
        point_on_ray = new_point;
        point_on_F = new_F;
    }
    return false;
}

void main()
{
    vec3 ray_direction = normalize(pixel.x * U + pixel.y * V + W);
    
    float Lmin = 1000000.0;
    float Lmax = -1.0;
    
    getBounds(ray_direction, Lmin, Lmax);
    vec3 position;
    if(intersect(ray_direction, Lmin, Lmax, position))
    {
        //outColor = vec4(1.0);
        vec3 normalDirection = getNormal(position.xz);
        float dotNL = dot(normalDirection, lightDirection);
        
        outColor = ambient;
        if(dotNL > 0.0)
        {
            vec3 diffuseReflection = diffuse * dotNL;
            vec3 viewDirection = normalize(eye - position);
            vec3 specularReflection = specular * pow(max(0.0, dot(reflect(-lightDirection, normalDirection), viewDirection)), shininess);
            outColor += lightIntensity * (diffuseReflection + specularReflection);
        }
        return;
    }
    outColor = missColor;
}
