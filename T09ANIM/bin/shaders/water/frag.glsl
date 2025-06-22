/* Fragment shader by DH7 */
#version 460
out vec4 OutColor;
 
in vec4 DrawColor;
in vec3 Normal;
in vec3 DrawPos;
in vec2 DrawTexCoord;

layout(binding = 0) uniform sampler2D Tex;
layout(binding = 1) uniform sampler2D TexN;
layout(binding = 2) uniform sampler2D TexUV;

uniform bool IsTexture0;
uniform float Time;
uniform float GlobalTime;
uniform vec3 Ka, Kd, Ks;
uniform float Ph;
uniform vec3 CamLoc, CamDir, CamRight, CamUp;
uniform float FrameW, FrameH, ProjDist, ProjSize;

uniform mat4 MatrWVP;

vec2 TexCoord;

vec3 Shade( vec3 P, vec3 N )
{
  vec3 LPos = vec3(576, 700, 324); 
  vec3 L = normalize(LPos - P);
  vec3 LC = vec3(1, 1, 1);

  float Ph = 30;
  vec3 Ka = vec3(0);
  vec3 Ks = vec3(0.9);
  vec3 Kd = vec3(0.6);
  
  vec3 V = normalize(P - CamLoc);
  //N = faceforward(N, V, N);
  vec3 color = vec3(0);
  color += Ka; //ambient
  vec3 R = reflect(V, N);
  vec3 diff = Kd;
  if (IsTexture0)
  {
    vec4 tex_color = texture(Tex, TexCoord);
    diff *= tex_color.rgb;
  }
  color += LC * diff * max(0, dot(N, L)); //diffuse
  color += LC * Ks * pow(max(0, dot(R, L)), Ph); //specular
 
  return color;
}
                              
void main( void )
{  
  vec2 dudv = texture(TexUV, DrawTexCoord * 8).xy * 2 - 1;
  vec2 ts = dudv * 0.1 + 0.018 * Time;
  TexCoord = DrawTexCoord * 2 + ts;
  vec3 N = texture(TexN, ts * 2).yzx * 2 - 1;
  vec4 tc = texture(Tex, DrawTexCoord);                            
  //OutColor = vec4(tc.r + 0.5, tc.g, tc.b + 0.5, 0.7); 
  //N = vec3(0, 1, 0);
  vec3 c = Shade(DrawPos, normalize(N));
  OutColor = vec4(c.r + 0.3, c.g, c.b + 0.3, 0.7); 
  //OutColor = vec4(tc.rgb, 0.7); 
}
