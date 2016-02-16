#version 330 core

layout(location = 0) in vec4 position;

out vec2 UV;

#ifndef AMBIENT_LIGHT
    out vec3 viewRay;
    uniform float tanHalfFovy; // tan(fovy / 2)
    uniform float aspectTanHalfFovy; // aspectRatio * tan(fovy / 2)
#endif

void main()
{
	gl_Position = position;
	UV = (position.xy + vec2(1.0, 1.0)) * 0.5;
    
#ifndef AMBIENT_LIGHT
    viewRay = vec3(-position.x * aspectTanHalfFovy, -position.y * tanHalfFovy, 1.0);
#endif
}
