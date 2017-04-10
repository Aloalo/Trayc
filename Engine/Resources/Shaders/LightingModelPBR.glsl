#define TERM_BECKMANN 0
#define TERM_GGX 1

#ifdef GLOBAL_LIGHT
    #include "ImageBasedLighting.glsl"
#endif

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

vec3 Lighting(in vec3 N, in vec3 L, in vec3 P, in vec3 lightIntensity, in vec3 albedo, in vec3 specularColor, in float atten, in float roughness, in float shadow)
{
    vec3 V = -normalize(P);
    float dotNL = saturate(dot(N, L));
    
    // calculate diffuse term
    vec3 Fdiff = fresnel(specularColor, V, N);
    vec3 diffuse = albedo * (1.0 - Fdiff);
    
    // calculate specular term
    vec3 H = normalize(V + L);
    float a = roughness * roughness;
    vec3 Fspec = fresnel(specularColor, L, H);
    float geometryTerm = GSmith(dotNL, saturate(dot(N, V)), a);
    float Nterm = NDF(N, H, a);
    
    vec3 specular = max(vec3(0.0), Fspec * geometryTerm * Nterm);
   
    vec3 Lo = shadow *  dotNL * atten * lightIntensity * (diffuse + specular);
    
    #ifdef GLOBAL_LIGHT
        Lo += IBL(V, N, roughness, Fdiff, albedo);
    #endif

    return Lo;
}

