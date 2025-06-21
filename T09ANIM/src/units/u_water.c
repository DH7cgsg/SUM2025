/* FILE NAME  : u_water.c
 * PROGRAMMER : DH7
 * LAST UPDATE: 18.06.2025
 * PURPOSE    : 3D game project.
 *              Water unit module.
 */

#include "units/units.h"

typedef struct 
{
  DH7_UNIT_BASE_FIELDS;
  dh7PRIM water;
} dh7UNIT_WATER;

static VOID DH7_UnitInit( dh7UNIT_WATER *Uni, dh7ANIM *Ani )
{  
  dh7MATERIAL mtl;
  
  mtl = DH7_RndMtlGetDef();
  strcpy(mtl.Name, "water material");
  DH7_RndPrimCreate(&Uni->water, DH7_RND_PRIM_POINTS, NULL, 1, NULL, 0);
  mtl.Tex[0] = DH7_RndTexAddFromFile("bin/textures/water.bmp");
  mtl.Tex[1] = DH7_RndTexAddFromFile("bin/textures/nm_water.bmp");
  mtl.Tex[2] = DH7_RndTexAddFromFile("bin/textures/dudv_water.bmp");
  mtl.ShdNo = DH7_RndShdAdd("water");
  Uni->water.MtlNo = DH7_RndMtlAdd(&mtl);
}
static VOID DH7_UnitResponse( dh7UNIT_WATER *Uni, dh7ANIM *Ani )
{
}
static VOID DH7_UnitRender( dh7UNIT_WATER *Uni, dh7ANIM *Ani )
{  
  DH7_RndPrimDraw(&Uni->water, MatrIdentity());
}
static VOID DH7_UnitClose( dh7UNIT_WATER *Uni, dh7ANIM *Ani )
{
  DH7_RndPrimFree(&Uni->water);
}

dh7UNIT * DH7_UnitCreateWATER( VOID )
{
  dh7UNIT_WATER *Uni;

  if ((Uni = (dh7UNIT_WATER *)DH7_AnimUnitCreate(sizeof(dh7UNIT_WATER))) == NULL)
    return NULL;
  Uni->Init = (VOID *)DH7_UnitInit;
  Uni->Response = (VOID *)DH7_UnitResponse;
  Uni->Render = (VOID *)DH7_UnitRender;
  Uni->Close = (VOID *)DH7_UnitClose;
  return (dh7UNIT *)Uni;
}