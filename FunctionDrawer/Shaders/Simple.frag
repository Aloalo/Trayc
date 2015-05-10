#version 330 core

in vec4 position;  // position of the vertex (and fragment) in world space
in vec3 varyingNormalDirection;  // surface normal vector in world space

// Output data
out vec3 color;

uniform vec4 invVxeye; //invV * vec4(0.0, 0.0, 0.0, 1.0)
 
//Material
uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

//Scene
uniform vec3 lightDirection;
uniform vec3 lightIntensity;

void main()
{
    vec3 normalDirection = normalize(varyingNormalDirection);
    float dotNL = dot(normalDirection, lightDirection);
    
    color = ambient;
    if(dotNL > 0.0)
    {
        vec3 viewDirection = normalize((invVxeye - position).xyz);
        vec3 diffuseReflection = diffuse * dotNL;
        vec3 specularReflection = specular * pow(max(0.0, dot(reflect(-lightDirection, normalDirection), viewDirection)), shininess);
        color += lightIntensity * (diffuseReflection + specularReflection);
    }
}
