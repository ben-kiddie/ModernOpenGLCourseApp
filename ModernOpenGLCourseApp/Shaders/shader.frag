#version 330		
					
in vec4 vCol;	
in vec2 vTexCoord;
					
out vec4 colour;	

uniform sampler2D theTexture;
					
void main()			
{					
	//colour = vCol;	
	//colour = texture(theTexture, vTexCoord) * vCol;
	colour = texture(theTexture, vTexCoord);
}					
