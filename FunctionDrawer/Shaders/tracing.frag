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

//Tracing stuff
uniform float drawDistance;
uniform float Lstep;
uniform float tolerance; //bisection tolerance
uniform int NMAX; //maximum bisection iterations
const float EPS = 0.0001; //zero

//Material
uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

//Scene
uniform vec3 lightDirection;
uniform vec3 lightIntensity;
uniform vec3 missColor;

//x derivative
float Fx(in vec2 p)
{
    return #Fx;
}
// y derivative
float Fy(in vec2 p)
{
    return #Fy;
}
// G(L) = F(eye.x + L * d.x, eye.z + L * d.z) - eye.y - L * d.y
float G(in float L, in vec3 d)
{
    return #G;
}

vec3 getNormal(in vec2 p)
{
    return normalize(vec3(-Fx(p), 1.0, -Fy(p)));
}

//get min and max lambda
void getBounds(in vec3 d, out float Lmin, out float Lmax)
{
    float L1 = (Xmin - eye.x) / d.x;
    float L2 = (Xmax - eye.x) / d.x;
    float L3 = (Zmin - eye.z) / d.z;
    float L4 = (Zmax - eye.z) / d.z;
    
    float minmax = min(max(L1, L2), max(L3, L4));
    float maxmin = max(min(L1, L2), min(L3, L4));
    
    Lmin = max(min(minmax, maxmin), 0.0);
    Lmax = min(Lmin + drawDistance, max(minmax, maxmin));
}

float bisection(in float a, in float b, in vec3 d)
{
    float c;
    float signGa = sign(G(a, d));
    int n = 0;
    while(n < NMAX)
    {
        c = (a + b) * 0.5; // new midpoint
        float Gc = G(c, d);
        
        if(abs(Gc) < EPS || (b - a) * 0.5 < tolerance)
            return c;

        n++;
        float signGc = sign(Gc);
        if(signGc == signGa)
        {
            a = c;
            signGa = signGc;
        }
        else 
            b = c;
    }
    return c;
}

bool intersect(in vec3 d, in float Lmin, in float Lmax, out vec3 intersection_point)
{
    //check if in bounds
    vec3 start = eye + Lmin * d;
    if(start.x < Xmin || start.x > Xmax || start.z < Zmin || start.z > Zmax)
        return false;
    
    float a = Lmin;
    float Ga = G(a, d);
    for(float L = Lmin + Lstep; L < Lmax; L += Lstep)
    {
        float b = L;
        float Gb = G(b, d);
        
        if(sign(Ga) != sign(Gb))
        {
            //float iL = (a + b) * 0.5;
            float iL = bisection(a, b, d);
            intersection_point = eye + iL * d;
            return true;
        }
            
        a = b;
        Ga = Gb;
    }
    return false;
}

void main()
{
    vec3 d = normalize(pixel.x * U + pixel.y * V + W);
    
    float Lmin;
    float Lmax;
    getBounds(d, Lmin, Lmax);
    
    vec3 position;
    outColor = missColor;
    if(intersect(d, Lmin, Lmax, position))
    {
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
    }
}
