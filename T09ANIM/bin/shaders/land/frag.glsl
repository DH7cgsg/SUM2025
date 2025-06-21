/* Fragment shader by DH7 */
#version 460
out vec4 OutColor;
 
in vec4 DrawColor;
in vec3 DrawNormal;
in vec3 DrawPos;
in vec2 DrawTexCoord;

layout(binding = 0) uniform sampler2D Tex;
layout(binding = 2) uniform sampler2D TexUV;
uniform bool IsTexture0;
uniform float Time;
uniform float GlobalTime;
uniform vec3 Ka, Kd, Ks;
uniform float Ph;
uniform vec3 CamLoc;

uniform int AddonI0, AddonI1;

vec2 TexCoord;
                        
vec3 Shade( vec3 P, vec3 N )
{
  vec3 LPos = vec3(100, 500, 100); 
  vec3 L = normalize(LPos - P);
  vec3 LC = vec3(1, 1, 1);
  
  vec3 V = normalize(P - CamLoc);
  N = faceforward(N, V, N);
  vec3 color = vec3(0);
  color += Ka; //ambient
  vec3 R = reflect(V, N);
  vec3 diff = Kd;
  if (IsTexture0)
  {
    vec4 tex_color = texture(Tex, TexCoord);
    diff = tex_color.rgb;
  }
  color += LC * diff * max(0, dot(N, L)); //diffuse
  color += LC * Ks * pow(max(0, dot(R, L)), Ph); //specular
 
  return color;

  
}

 
void main( void )
{
  vec2 dudv = texture(TexUV, DrawTexCoord * 0.2 - vec2(0, Time * .1)).xy * 2 - 1;
  vec2 ts = dudv * 0.02;
  TexCoord = DrawTexCoord;
  if (DrawPos.y < 1.31)
    TexCoord += ts;
              
  OutColor = vec4(Shade(DrawPos, normalize(DrawNormal)), 1); 
}
