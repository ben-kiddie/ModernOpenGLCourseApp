#version 330

in vec4 vFragPos;

uniform vec3 lightPos;
uniform float farPlane;

void main() 
{
	float distance = length(vFragPos.xyz - lightPos);
	distance = distance / farPlane;
	gl_FragDepth = distance;
}