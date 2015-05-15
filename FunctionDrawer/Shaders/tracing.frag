#version 330 core

in vec2 pixel;
out vec3 outColor;

//Camera
uniform vec3 eye;
uniform vec3 U;
uniform vec3 W;
uniform vec3 V;

//Bounds
uniform vec3 minv;
uniform vec3 maxv;

//Tracing stuff
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
bool intersectAABB(in vec3 d, out float Lmin, out float Lmax)
{
    float Lx1 = (minv.x - eye.x) / d.x;
    float Lx2 = (maxv.x - eye.x) / d.x;
    float Ly1 = (minv.y - eye.y) / d.y;
    float Ly2 = (maxv.y - eye.y) / d.y;
    float Lz1 = (minv.z - eye.z) / d.z;
    float Lz2 = (maxv.z - eye.z) / d.z;
    
    Lmin = min(Lx1, Lx2);
    Lmax = max(Lx1, Lx2);
    
    Lmin = max(Lmin, min(Ly1, Ly2));
    Lmax = min(Lmax, max(Ly1, Ly2));
    
    Lmin = max(Lmin, min(Lz1, Lz2));
    Lmax = min(Lmax, max(Lz1, Lz2));
    
    return Lmax >= max(0.0, Lmin);
}

float bisection(in float a, in float b, in vec3 d)
{
    float c;
    float signGa = sign(G(a, d));
    int n = 0;
    while(n < NMAX)
    {
        if((b - a) * 0.5 < tolerance)
            return c;
    
        c = (a + b) * 0.5; // new midpoint
        float Gc = G(c, d);
        
        if(abs(Gc) < EPS)
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

bool intersectFunction(in vec3 d, in float Lmin, in float Lmax, out vec3 intersection_point)
{
    float a = Lmin;
    float Ga = G(a, d);
    for(float L = Lmin + Lstep; L < Lmax; L += Lstep)
    {
        float b = L;
        float Gb = G(b, d);
        
        if(sign(Ga) != sign(Gb))
        {
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
    outColor = missColor;
    
    float Lmin;
    float Lmax;
    if(intersectAABB(d, Lmin, Lmax))
    {
        vec3 position;
        if(intersectFunction(d, Lmin, Lmax, position))
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
}
