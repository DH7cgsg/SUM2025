/* Vertex shader by DH7 */
#version 460
 
 
layout(location = 0) in vec3 InPosition;
uniform mat4 MatrWVP;

 
void main( void )
{                      
  
  gl_Position = MatrWVP * vec4(InPosition, 1);   
  
}