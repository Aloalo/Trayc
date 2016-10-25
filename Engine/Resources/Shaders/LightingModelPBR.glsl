float saturate(in float x)
{
    return clamp(x, 0.0, 1.0);
}

const float pi = 3.141592;

// Fresnel - Schlick approximation
vec3 fresnel(in vec3 F0, in vec3 E, in vec3 H)
{
    return F0 + (1.0 - F0) * pow(1.0 - saturate(dot(E, H)), 5.0);
}

// Geometric shadowing term - Smith approximation
// Returns the G term devided by 4*(N.L)*(N.V)
float GSmith(in float dotNL, in float dotNV, in float a)
{
#ifdef G_TERM_GGX
    float a2 = a*a;
    float G_V = dotNV + sqrt( (dotNV - dotNV * a2) * dotNV + a2 );
    float G_L = dotNL + sqrt( (dotNL - dotNL * a2) * dotNL + a2 );
    return 1.0 / (G_V * G_L);
#else // Default is the implicit term
    return 1.0;
#endif
}

vec3 LightingPhysical(in vec3 N, in vec3 L, in vec3 P, in vec3 lightIntensity, in vec3 albedo, in vec3 specularColor, in float atten, in float gloss)
{
    // calculate shadow
    float dotNL = saturate(dot(N, L));
    float shadow = GetShadowFactor(P, dotNL);
    
    // calculate diffuse term
    vec3 diffuse = albedo / pi;
    
    // calculate specular term
    vec3 V = normalize(P);
    vec3 H = normalize(V + L);
    float a = gloss * gloss;
    vec3 fresnelTerm = fresnel(specularColor, L, H);
    float geometryTerm = GSmith(dotNL, saturate(dot(N, V)), a)
    
    vec3 specular = vec3(0.0);
    
    return shadow * pi * atten * lightIntensity * dotNL * (diffuse + specular);
}

