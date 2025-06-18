/* Fragment shader by DH7 */
#version 460
out vec4 OutColor;
 
in vec4 DrawColor;
in vec3 Normal;
in vec3 DrawPos;
in vec2 DrawTexCoord;

layout(binding = 0) uniform sampler2D Tex;
uniform bool IsTexture0;
uniform float Time;
uniform float GlobalTime;
uniform vec3 Ka, Kd, Ks;
uniform float Ph;
uniform vec3 CamLoc, CamDir, CamRight, CamUp;
uniform float FrameW, FrameH, ProjDist, ProjSize;
                              
void main( void )
{  
  float Wp, Hp;
  
  Wp = Hp = ProjSize;
  /* Correct aspect ratio */
  if (FrameW > FrameH)
    Wp *= FrameW / FrameH;
  else
    Hp *= FrameH / FrameW;
  float
    xp = gl_FragCoord.x * Wp / FrameW,
    yp = gl_FragCoord.y * Hp / FrameH;
 
  vec3 D = normalize(CamDir * ProjDist + CamRight * xp + CamUp * yp);
  float
    theta = acos(-D.y) / acos(-1),
    phi = atan(D.x, D.z) / (2 * acos(-1));

          


  vec4 tex_color = texture(Tex, vec2(phi, theta));                            
  OutColor = vec4(tex_color.rgb,  1); 
}
  