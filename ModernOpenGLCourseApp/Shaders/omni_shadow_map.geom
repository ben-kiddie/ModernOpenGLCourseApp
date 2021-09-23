#version 330

layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;

uniform mat4 lightMatrices[6];	// Each light matrix represents a direction pointing towards the positive or negative of an axis.

out vec4 vFragPos;

void main() 
{
	for(int face = 0; face < 6; face++)
	{
		gl_Layer = face;	// gl_Layer is built in with GLSL, much like gl_Position. The layer specifies which face to draw on, and in this case, to draw depth values to.
		for(int i = 0; i < 3; i++)
		{
			vFragPos = gl_in[i].gl_Position;
			gl_Position = lightMatrices[face] * vFragPos;	// Note that the light matrices are a combination of projection * view.
			EmitVertex();
		}
		EndPrimitive();
	}
}