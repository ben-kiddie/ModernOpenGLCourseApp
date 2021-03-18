#version 330							
								
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
								
out vec4 vCol;
out vec2 vTexCoord;
								
uniform mat4 model;						
uniform mat4 projection;
uniform mat4 view;
								
void main()							
{	
	gl_Position = projection * view * model * vec4(pos, 1.0);	// Remember - the order is projection * view * model * identity matrix. Think of it as working backwards and moving through various spaces from there.
	vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);	
	vTexCoord = tex;
}					