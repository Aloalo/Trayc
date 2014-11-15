#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec4 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal_modelspace;

// Output data ; will be interpolated for each fragment.
out vec3 position_worldspace;
out vec3 normal_cameraspace;
out vec3 eyeDirection_cameraspace;
out vec3 lightDirection_cameraspace;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 V;
uniform mat4 M;
uniform mat3 normalMatrix;
uniform vec3 lightPosition_worldspace;

void main()
{
	gl_Position =  MVP * vertexPosition_modelspace;
    
	position_worldspace = (M * vertexPosition_modelspace).xyz;
	
	vec3 vertexPosition_cameraspace = (MV * vertexPosition_modelspace).xyz;
	eyeDirection_cameraspace = -vertexPosition_cameraspace;

	// Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
	vec3 lightPosition_cameraspace = (V * vec4(lightPosition_worldspace,1)).xyz;
	lightDirection_cameraspace = lightPosition_cameraspace + eyeDirection_cameraspace;
	
	// Normal of the the vertex, in camera space
	normal_cameraspace = normalMatrix * vertexNormal_modelspace;
}

