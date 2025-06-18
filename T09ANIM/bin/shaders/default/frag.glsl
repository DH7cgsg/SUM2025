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
                                    
vec3 Shade( vec3 P, vec3 N )
{
  vec3 LPos = vec3(0, 100, 100); 
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
    vec4 tex_color = texture(Tex, DrawTexCoord);
    diff = tex_color.rgb;
  }
  color += LC * diff * max(0, dot(N, L)); //diffuse
  color += LC * Ks * pow(max(0, dot(R, L)), Ph); //specular
 
  return color;

  
}

 
void main( void )
{                              
  OutColor = vec4(Shade(DrawPos, normalize(DrawNormal)), 1); 
}
