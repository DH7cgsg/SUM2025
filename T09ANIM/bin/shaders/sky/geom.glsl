/* Geometry shader by DH7 */
#version 460
layout(points) in;
layout(triangle_strip, max_vertices = 3) out;


void main( void )
{
   gl_Position = vec4(-1, 3, 0, 1);
   EmitVertex();
   gl_Position = vec4(-1, -1, 0, 1);
   EmitVertex();
   gl_Position = vec4(3, -1, 0, 1);
   EmitVertex();
   EndPrimitive();
}

