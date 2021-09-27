#version 330
				
layout (location = 0) in vec3 pos;

out vec3 vTexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	vTexCoords = pos;
	gl_Position = projection * view * vec4(pos, 1.0f);
}