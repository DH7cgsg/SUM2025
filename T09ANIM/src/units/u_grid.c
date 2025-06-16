/* FILE NAME  : u_ctrl.c
 * PROGRAMMER : DH7
 * LAST UPDATE: 11.06.2025
 * PURPOSE    : 3D animation project.
 *          Control unit module.
 */

#include "units/units.h" 
#include <stdio.h>

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
  dh7MATERIAL mtl = 
  {
    "land material",
    {0.02, 0.02, 0.02},
    {0.01, 0.01, 0.01},
    {0.4, 0.4, 0.4},
    10.0, 1,
    {-1, -1, -1, -1, -1, -1, -1, -1},
    0
  };

  if ((hBm = LoadImage(NULL, "bin/heights/hf.bmp", IMAGE_BITMAP, 0, 0,
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
        for (x = 0; x < w; x++)
        {
          INT hgt = Bits[(h - 1 - y) * bm.bmWidthBytes + x];
 
          G.V[y * w + x].P = VecMulNum(VecSet(x / (w - 1.0) - 0.5,
                                    hgt / 255.0 / 5 - 0.009,
                                    1 - y / (h - 1.0) - 0.5), 100);
        }

      mtl.Tex[0] = DH7_RndTexAddFromFile("bin/textures/hfcolor.bmp");
      Uni->Land.MtlNo = DH7_RndMaterialAdd(&mtl);
      DH7_RndGridAutoNormals(&G);
      DH7_RndPrimFromGrid(&Uni->Land, &G);
      DH7_RndGridFree(&G);
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