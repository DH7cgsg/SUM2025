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
#include <stdlib.h>


static VEC GLB_Geom[GRID_H][GRID_W];
static POINT pnts[GRID_H][GRID_W];
static INT GLB_Ws, GLB_Hs;
static DBL ProjDist = 0.1, GLB_Wp, GLB_Hp, ProjSize = 0.1;
static INT Params[4];



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

VOID ChangeParam( INT val, INT index )
{
  if (index == 3)
    Params[index] += val;
  else
    Params[index] = val;
}

INT GetParam( INT index )
{
  return Params[index];
}

VOID GLB_Init( DBL R )     
{
  INT i, j;
  DBL theta, phi;

  Params[0] = 0; //rand colors: 0 - disabled, 1 - enabled
  Params[1] = 0; //shape: 0 - standard, 1 - shrinked
  Params[2] = 1; //lines: 0 - disabled, 1 - enabled
  Params[3] = 8; //distance

  for (i = 0, theta = 0; i < GRID_H; i++, theta += PI / (GRID_H - 1))
    for (j = 0, phi = 0; j < GRID_W; j++, phi += 2 * PI / (GRID_W - 1))
    {
      GLB_Geom[i][j].X = R * sin(theta) * sin(phi);
      GLB_Geom[i][j].Y = R * cos(theta);
      GLB_Geom[i][j].Z = R * 3.7 * cos(phi) * sin(theta);
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


VOID GLB_Draw( HDC hDC, COLORREF color, INT s, DBL angle )
{
  INT i, j;                     
  static POINT ps[4];
  DBL t, xp, yp;
  VEC p;
  DBL phi, theta, a = 2, b = 2;

  t = (DOUBLE)clock() / CLOCKS_PER_SEC;

  if (!Params[2])
    SelectObject(hDC, GetStockObject(NULL_PEN));
  else
    SelectObject(hDC, GetStockObject(DC_PEN));
  SetDCPenColor(hDC, RGB(255, 255, 255));
  
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(hDC, color);
  
  for (i = 0, theta = 0; i < GRID_H; i++, theta += PI / (GRID_H - 1))
    for (j = 0, phi = 0; j < GRID_W; j++, phi += 2 * PI / (GRID_W - 1))
    {
      p = GLB_Geom[i][j];
      if (Params[1])
      {
        p.X *= (powf(sin(theta), a) * powf(sin(phi), b));
        p.Y *= powf(cos(theta), a);
        p.Z *= (powf(cos(phi), b) * powf(sin(theta), a));
      }
        
      p = RotateX(RotateY(RotateZ(p, t * 10), t * 20), t * 30); //rotate
      p.Z -= Params[3];

      xp = p.X * ProjDist / -p.Z;
      yp = p.Y * ProjDist / -p.Z; //projections

      pnts[i][j].x = (LONG)(xp * (GLB_Ws / GLB_Wp) + GLB_Ws / 2); //scrseen
      pnts[i][j].y = (LONG)(-yp * (GLB_Hs / GLB_Hp) + GLB_Hs / 2);
    }

  for (i = 0; i < GRID_H - 1; i++) //polygons
    for (j = 0; j < GRID_W - 1; j++)
    {
      if (Params[0])
        SetDCBrushColor(hDC, RGB(rand() % 256, rand() % 256, rand() % 256));
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








