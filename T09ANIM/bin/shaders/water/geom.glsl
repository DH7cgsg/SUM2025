/* Geometry shader by DH7 */
#version 460
layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform mat4 MatrWVP;
uniform float Time;

out vec2 DrawTexCoord;
out vec3 DrawPos;

void main( void )
{
   float time_scale = 0.05;

   gl_Position = MatrWVP * vec4(DrawPos = vec3(0, 5, 0), 1);
   DrawTexCoord = vec2(0, 0) + vec2(Time * time_scale, 0);
   EmitVertex();
   gl_Position = MatrWVP * vec4(DrawPos = vec3(1152, 5, 0), 1);
   DrawTexCoord = vec2(0, 1) + vec2(Time * time_scale, 0);
   EmitVertex();
   gl_Position = MatrWVP * vec4(DrawPos = vec3(0, 5, 648), 1);
   DrawTexCoord = vec2(1, 0) + vec2(Time * time_scale, 0);
   EmitVertex();
   gl_Position = MatrWVP * vec4(DrawPos = vec3(1152, 5, 648), 1);
   DrawTexCoord = vec2(1, 1) + vec2(Time * time_scale, 0);
   EmitVertex();
   EndPrimitive();
}
