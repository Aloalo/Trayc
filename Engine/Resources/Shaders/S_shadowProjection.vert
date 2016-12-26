
layout(location = 0) in vec4 position;

#include "UB_ViewRayData.glsl"

out vec2 UV;
out vec3 viewRay;

void main()
{
	gl_Position = position;
	UV = (position.xy + vec2(1.0, 1.0)) * 0.5;
    viewRay = vec3(-position.x * aspectTanHalfFovy, -position.y * tanHalfFovy, 1.0);
}
