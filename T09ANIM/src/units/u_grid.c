/* FILE NAME  : u_ctrl.c
 * PROGRAMMER : DH7
 * LAST UPDATE: 11.06.2025
 * PURPOSE    : 3D game project.
 *          Control unit module.
 */

#include "units/units.h" 
#include <stdio.h>
#include "game/game.h"


typedef struct
{
  DH7_UNIT_BASE_FIELDS;
  dh7PRIM Land;
} dh7UNIT_GRID;

static VOID DH7_UnitInit( dh7UNIT_GRID *Uni, dh7ANIM *Ani )
{
  dh7GRID G;
  HBITMAP hBm;
  BITMAP bm;
  dh7MATERIAL mtl;

  if ((hBm = LoadImage(NULL, "bin/heights/map.bmp", IMAGE_BITMAP, 0, 0,
                       LR_LOADFROMFILE | LR_CREATEDIBSECTION)) != NULL)
  {
    INT w, h, x, y;
    

    GetObject(hBm, sizeof(bm), &bm);
    w = bm.bmWidth;
    h = bm.bmHeight;
    if (bm.bmBitsPixel == 8 && DH7_RndGridCreate(&G, w, h))
    {
      BYTE *Bits = bm.bmBits;
      
 
      for (y = 0; y < h; y++)
      {
        for (x = 0; x < w; x++)
        {
          INT hgt = Bits[(h - 1 - y) * bm.bmWidthBytes + x];
          VEC v = VecSet(x, hgt / 3, y);
          DH7_Game.MapHeights[y][x] = hgt;

          G.V[y * w + x].P = v;
          G.V[y * w + x].T = Vec2Set(5 * x / (w - 1.0), 5 * (1 - y / (h - 1.0)));
        }
       
      }
      mtl = DH7_RndMtlGetDef();
      strcpy(mtl.Name, "land material");
      DH7_RndGridAutoNormals(&G);
      DH7_RndPrimFromGrid(&Uni->Land, &G);
      DH7_RndGridFree(&G);
      mtl.Tex[0] = DH7_RndTexAddFromFile("bin/textures/chess.bmp");
      mtl.ShdNo = DH7_RndShdAdd("land");      
      Uni->Land.MtlNo = DH7_RndMtlAdd(&mtl);
      
    }
    DeleteObject(hBm);
  }
}

static VOID DH7_UnitClose( dh7UNIT_GRID *Uni, dh7ANIM *Ani )
{
  DH7_RndPrimFree(&Uni->Land);
}

static VOID DH7_UnitResponse( dh7UNIT_GRID *Uni, dh7ANIM *Ani )
{
  
}
static VOID DH7_UnitRender( dh7UNIT_GRID *Uni, dh7ANIM *Ani )
{
  DH7_RndPrimDraw(&Uni->Land, MatrIdentity());
}


dh7UNIT * DH7_UnitCreateGRID( VOID )
{ 
  dh7UNIT *Uni;

  if((Uni = DH7_AnimUnitCreate(sizeof(dh7UNIT_GRID))) == NULL)
    return NULL;

  Uni->Init = (VOID *)DH7_UnitInit;
  Uni->Close = (VOID *)DH7_UnitClose;
  Uni->Response = (VOID *)DH7_UnitResponse;
  Uni->Render = (VOID *)DH7_UnitRender;
  return Uni;
  
}