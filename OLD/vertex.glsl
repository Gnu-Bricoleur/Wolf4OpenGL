#version 130

out vec3 color;

layout(location= 0) in vec3 position;
    
void main( )
{	
	gl_Position = vec4(position, 1);
	color = vec3(145,145,145);
	
}
