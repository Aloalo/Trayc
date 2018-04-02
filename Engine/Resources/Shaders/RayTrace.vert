layout(location = 0) in vec4 position;

out vec2 clipSpaceCoords;

void main()
{
	gl_Position = position;
    clipSpaceCoords = position.xy;
}
