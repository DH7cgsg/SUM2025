/* Vertex shader by DH7 */
#version 460
 
uniform mat4 MatrWVP;
uniform mat4 MatrW;
uniform mat4 MatrWinv;


uniform float Time;
 
layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec2 TexCoord;
layout(location = 2) in vec3 InNormal;
layout(location = 3) in vec4 InColor;
 
out vec4 DrawColor;
out vec3 DrawNormal;
out vec3 DrawPos;
out vec2 DrawTexCoord;
 
void main( void )
{                      
  
  gl_Position = MatrWVP * vec4(InPosition, 1);   
  DrawNormal = (MatrWinv * vec4(InNormal, 1)).xyz;
  DrawPos = (MatrW * vec4(InPosition, 1)).xyz;
  DrawColor = InColor;
  
  
  DrawTexCoord = TexCoord;
  
  
 
  

}