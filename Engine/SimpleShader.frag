#version 330 core

// Interpolated values from the vertex shaders
in vec3 position_worldspace;
in vec3 normal_cameraspace;
in vec3 eyeDirection_cameraspace;
in vec3 lightDirection_cameraspace;

// Ouput data
out vec3 color;

// Values that stay constant for the whole mesh.
uniform mat4 MV;
uniform vec3 lightPosition_worldspace;

uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;

uniform vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
uniform float lightPower = 50.0f;
uniform float shininess = 50.0f;

void main()
{
	// Distance to the light
	float distance = length(lightPosition_worldspace - position_worldspace);

	// DIFFUSE
	vec3 n = normalize(normal_cameraspace);
	vec3 l = normalize(lightDirection_cameraspace);
	float cosTheta = clamp(dot( n,-l ), 0.0f, 1.0f);
	
	// SPECULAR
	vec3 E = normalize(eyeDirection_cameraspace);
	vec3 R = reflect(-l,n);
	float cosAlpha = clamp(dot(E,R), 0.0f, 1.0f);
	
	color = 
		ambientColor +
		diffuseColor * lightColor * lightPower * cosTheta / (distance*distance) +
		specularColor * lightColor * lightPower * pow(cosAlpha, shininess) / (distance*distance);

}