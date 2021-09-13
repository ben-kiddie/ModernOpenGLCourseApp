#version 330

layout (location = 0) in vec3 pos;	// We want to know where everything is relative to the light source

uniform mat4 model;	// Apply model of our object
uniform mat4 directionalLightTransform;	// Display everything in light space / from the perspective of the light source, i.e., view (where camera is) * projection (the way it looks)

void main() 
{
	gl_Position = directionalLightTransform * model * vec4(pos, 1.0f);	// Convert the point into world space relative to the light including where and how it is looking
}