/* FILE NAME  : globe.c
 * PROGRAMMER : DH7

 * LAST UPDATE: 07.06.2025
 * PURPOSE    : 3d globe
 *              Startup entry-point module.
 */

#include "globe.h"
#include "mth.h"
#include <math.h>
#include <time.h>


static VEC GLB_Geom[GRID_H][GRID_W];
static POINT pnts[GRID_H][GRID_W];
static INT GLB_Ws, GLB_Hs;
static DBL ProjDist = 0.1, GLB_Wp, GLB_Hp, ProjSize = 0.1, Dist = 8;



VEC RotateX( VEC P, DBL angle )
{
  VEC NewP;
  DBL a, si, co;

  a = angle * PI / 180, si = sin(a), co = cos(a);
  NewP.X = P.X;
  NewP.Y = P.Y * co - P.Z * si;
  NewP.Z = P.Z * co + P.Y * si;
  return NewP;
}

VEC RotateY( VEC P, DBL angle )
{
  VEC NewP;
  DBL a, si, co;

  a = angle * PI / 180, si = sin(a), co = cos(a);
  NewP.X = P.X * co + P.Z * si;
  NewP.Y = P.Y;
  NewP.Z = P.Z * co - P.X * si;
  return NewP;
}

VEC RotateZ( VEC P, DBL angle )
{
  VEC NewP;
  DBL a, si, co;

  a = angle * PI / 180, si = sin(a), co = cos(a);
  NewP.X = P.X * co - P.Y * si;
  NewP.Y = P.X * si + P.Y * co;
  NewP.Z = P.Z;
  return NewP;
}

VOID GLB_Init( DBL R )     
{
  INT i, j;
  DBL theta, phi, a = 1, b = 1;

  for (i = 0, theta = 0; i < GRID_H; i++, theta += PI / (GRID_H - 1))
    for (j = 0, phi = 0; j < GRID_W; j++, phi += 2 * PI / (GRID_W - 1))
    {
      GLB_Geom[i][j].X = R * 1.8 * powf(sin(theta), a) * powf(sin(phi), b);
      GLB_Geom[i][j].Y = R * powf(cos(theta), a);
      GLB_Geom[i][j].Z = R * 3 * powf(cos(phi), b) * powf(sin(theta), a);
    }
}
VOID GLB_Resize( INT Ws, INT Hs ) //window sizes
{
  GLB_Ws = Ws;
  GLB_Hs = Hs;
  if (GLB_Ws >= GLB_Hs)
    GLB_Wp = ProjSize * GLB_Ws / GLB_Hs, GLB_Hp = ProjSize;
  else
    GLB_Wp = ProjSize, GLB_Hp = ProjSize * GLB_Hs / GLB_Ws;
}  

VOID ChangeDist( DBL dist )
{
  Dist += dist;
}

VOID GLB_Draw( HDC hDC, COLORREF color, INT s, DBL angle )
{
  INT i, j;                     
  static POINT ps[4];
  DBL t, xp, yp;
  VEC p;
  MATR m;

  t = (DOUBLE)clock() / CLOCKS_PER_SEC;

  SelectObject(hDC, GetStockObject(DC_PEN));
  SetDCPenColor(hDC, RGB(255, 255, 255));
  
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(hDC, color);
  
  for (i = 0; i < GRID_H; i++)
    for (j = 0; j < GRID_W; j++)
    {
      p = GLB_Geom[i][j];
      p = RotateX(RotateY(RotateZ(p, t * 10), t * 20), t * 30); //rotate
      p.Z -= Dist;

      xp = p.X * ProjDist / -p.Z;
      yp = p.Y * ProjDist / -p.Z; //projections

      pnts[i][j].x = (LONG)(xp * (GLB_Ws / GLB_Wp) + GLB_Ws / 2); //screen
      pnts[i][j].y = (LONG)(-yp * (GLB_Hs / GLB_Hp) + GLB_Hs / 2);
    }

  for (i = 0; i < GRID_H - 1; i++) //polygons
    for (j = 0; j < GRID_W - 1; j++)
    {
      ps[0] = pnts[i][j];
      ps[1] = pnts[i][j + 1];
      ps[2] = pnts[i + 1][j + 1];
      ps[3] = pnts[i + 1][j];
 
      if ((ps[0].x - ps[1].x) * (ps[0].y + ps[1].y) +
          (ps[1].x - ps[2].x) * (ps[1].y + ps[2].y) +
          (ps[2].x - ps[3].x) * (ps[2].y + ps[3].y) +
          (ps[3].x - ps[0].x) * (ps[3].y + ps[0].y) <= 0)
        Polygon(hDC, ps, 4);
    }

}








