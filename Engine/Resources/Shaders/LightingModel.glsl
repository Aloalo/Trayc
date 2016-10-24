float saturate(in float x)
{
    return clamp(x, 0.0, 1.0);
}

vec3 fresnelSchlick(in vec3 specularColor, in vec3 E, in vec3 H)
{
    return specularColor + (1.0 - specularColor) * pow(1.0 - saturate(dot(E, H)), 5.0);
}

vec3 LightingPhysical(in vec3 N, in vec3 L, in vec3 P, in vec3 lightIntensity, in vec3 albedo, in vec3 specularColor, in float atten, in float gloss)
{
    // calculate diffuse term
    float dotNL = saturate(dot(N, L) * atten);
    vec3 diffuse = dotNL * lightIntensity;

    float shadow = GetShadowFactor(P, dotNL);
    
    // calculate specular term
    vec3 V = normalize(P);
    vec3 H = normalize(L + V);

    float specularPower = exp2(10.0 * gloss + 1.0);
    vec3 fresnelTerm = fresnelSchlick(specularColor, L, H);
    vec3 specular = fresnelTerm * ((specularPower + 2.0) / 8.0) * pow(saturate(dot(N, H)), specularPower) * dotNL;

    return shadow * (albedo * diffuse + specular) * 0.0001 + specular;
}


vec3 LightingPhong(in vec3 N, in vec3 L, in vec3 P, in vec3 lightIntensity, in vec3 albedo, in vec3 specularColor, in float atten, in float gloss)
{
    // Diffuse
    float dNL = max(0.0, dot(N, L));
    vec3 diffuse = dNL * albedo;
    
    float shadow = GetShadowFactor(P, dNL);
    
    // Specular
    vec3 specular = vec3(0.0);
    if(dNL > 0.0)
    {
        vec3 R = reflect(L, N);
        vec3 V = normalize(P);
        float dVR = max(0.0, dot(V, R));
        specular = specularColor * pow(dVR, gloss * 512.0);
    }
    
    return atten * lightIntensity * shadow * (diffuse + specular);
}
