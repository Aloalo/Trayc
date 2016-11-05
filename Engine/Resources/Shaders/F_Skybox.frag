
in vec3 texcoords;
uniform samplerCube skybox;

layout(location = 0) out vec4 color;

void main()
{
    color = texture(skybox, texcoords);
}
