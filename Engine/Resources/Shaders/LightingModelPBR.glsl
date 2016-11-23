#define TERM_BECKMANN 0
#define TERM_GGX 1

uniform samplerCube reflectionMap;
uniform mat4 cubemapM;

float saturate(in float x)
{
    return clamp(x, 0.0, 1.0);
}

// Fresnel - Schlick approximation
vec3 fresnel(in vec3 F0, in vec3 L, in vec3 H)
{
    return F0 + (1.0 - F0) * pow(1.0 - saturate(dot(L, H)), 5.0);
}

float G1(in float dotNX, in float a)
{
#if TERM_BECKMANN
    float c = dotNX / (a * sqrt(1.0 - dotNX * dotNX));
    float c2 = c * c;
    float G = (3.535 * c + 2.181 * c2) / (1.0 + 2.276 * c + 2.577 * c2);
    return mix(G, 1.0, step(1.6, c));
#else
    return 1.0;
#endif
}

// Geometric shadowing term - Smith approximation
// Returns the G term devided by 4*(N.L)*(N.V)
float GSmith(in float dotNL, in float dotNV, in float a)
{
#if TERM_GGX
    float a2 = a*a;
    float G_V = dotNV + sqrt( (dotNV - dotNV * a2) * dotNV + a2 );
    float G_L = dotNL + sqrt( (dotNL - dotNL * a2) * dotNL + a2 );
    return 1.0 / (G_V * G_L);
#elif TERM_BECKMANN
    return G1(dotNL, a) * G1(dotNV, a);
#endif
}

// Normal distribution function
float NDF(in vec3 N, in vec3 H, in float a)
{
    float dotNH = saturate(dot(N, H));
    float a2 = a*a;
#if TERM_GGX
    float denominator = (dotNH * dotNH * (a2 - 1.0) + 1.0);
    return a2 / (denominator * denominator);
#elif TERM_BECKMANN
    float dotNH2 = dotNH * dotNH;
    float e = exp((dotNH2 - 1.0) / (a2 * dotNH2));
    return e / (dotNH2 * dotNH2 * a2);
#endif
}

#ifdef GLOBAL_LIGHT
    vec3 Reflection(in vec3 V, in vec3 N, in float roughness)
    {
        float lod = log2(float(textureSize(reflectionMap, 0).x));
        vec3 R = reflect(-V, N);
        R = (cubemapM * (invV * vec4(R, 0.0))).xyz;
        return textureLod(reflectionMap, R, lod * (1.0 - roughness) * 6.0).rgb;
    }
#endif

vec3 Lighting(in vec3 N, in vec3 L, in vec3 P, in vec3 lightIntensity, in vec3 albedo, in vec3 specularColor, in float atten, in float roughness)
{
    vec3 V = -normalize(P);
    
    // calculate shadow
    float dotNL = saturate(dot(N, L));
    float shadow = GetShadowFactor(P, dotNL);
    
    // calculate diffuse term
    vec3 Fdiff = fresnel(specularColor, V, N);
    vec3 diffuse = albedo * (1.0 - Fdiff);
    
    // calculate specular term
    vec3 H = normalize(V + L);
    float a = roughness * roughness;
    vec3 Fspec = fresnel(specularColor, L, H);
    float geometryTerm = GSmith(dotNL, saturate(dot(N, V)), a);
    float Nterm = NDF(N, H, a);
    
    vec3 specular = Fspec * geometryTerm * Nterm;
    
#ifdef GLOBAL_LIGHT
    vec3 reflection = Reflection(V, N, roughness) * Fdiff * albedo;
    return shadow * (dotNL * (atten * lightIntensity * diffuse + specular) + reflection);
    //return shadow * atten * lightIntensity * dotNL * (diffuse + reflection + specular);
#else
    return shadow * atten * lightIntensity * dotNL * (diffuse + specular);
#endif
}

