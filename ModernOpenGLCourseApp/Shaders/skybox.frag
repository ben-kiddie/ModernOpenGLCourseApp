#version 330

in vec3 vTexCoords;

out vec4 colour;

uniform samplerCube skybox;

void main()
{
	colour = texture(skybox, vTexCoords);
}