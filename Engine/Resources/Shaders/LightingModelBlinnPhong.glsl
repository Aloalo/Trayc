
vec3 Lighting(in vec3 N, in vec3 L, in vec3 P, in vec3 lightIntensity, in vec3 albedo, in vec3 specularColor, in float atten, in float gloss)
{
    // Diffuse
    float dNL = max(0.0, dot(N, L));
    vec3 diffuse = dNL * albedo;
    
    float shadow = GetShadowFactor(P, dNL);
    
    // Specular
    vec3 specular = vec3(0.0);
    if(dNL > 0.0)
    {
        vec3 V = -normalize(P);
        vec3 H = normalize(V + L);
        float dotNH = max(0.0, dot(N, H));
        specular = specularColor * pow(dotNH, gloss * 512.0);
    }
    
    return atten * lightIntensity * shadow * (diffuse + specular);
}
