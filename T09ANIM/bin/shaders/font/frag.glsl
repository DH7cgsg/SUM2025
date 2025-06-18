/* Fragment shader by DH7 */
#version 460
out vec4 OutColor;
 
in vec4 DrawColor;
in vec3 DrawNormal;
in vec3 DrawPos;
in vec2 DrawTexCoord;

layout(binding = 0) uniform sampler2D Tex;
uniform bool IsTexture0;
uniform float Time;
uniform float GlobalTime;
uniform vec3 Ka, Kd, Ks;
uniform float Ph;
uniform vec3 CamLoc;
                                    


 
void main( void )
{
  if (DrawColor.a < 0.1)
    discard;

  vec4 tex_color = texture(Tex, DrawTexCoord);                  
  OutColor = tex_color; 
}
