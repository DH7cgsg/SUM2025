/* FILE NAME  : u_sky.c
 * PROGRAMMER : DH7
 * LAST UPDATE: 18.06.2025
 * PURPOSE    : 3D animation project.
 *              Sky unit module.
 */

#include "units/units.h"

typedef struct 
{
  DH7_UNIT_BASE_FIELDS;
  dh7PRIM sky;
} dh7UNIT_SKY;

static VOID DH7_UnitInit( dh7UNIT_SKY *Uni, dh7ANIM *Ani )
{  
  dh7MATERIAL mtl;

  mtl = DH7_RndMtlGetDef();
  strcpy(mtl.Name, "sky material");
  DH7_RndPrimCreate(&Uni->sky, DH7_RND_PRIM_POINTS, NULL, 1, NULL, 0);
  mtl.Tex[0] = DH7_RndTexAddFromFile("bin/textures/starry_sky.bmp");
  mtl.ShdNo = DH7_RndShdAdd("sky");
  Uni->sky.MtlNo = DH7_RndMtlAdd(&mtl);
}
static VOID DH7_UnitResponse( dh7UNIT_SKY *Uni, dh7ANIM *Ani )
{
}
static VOID DH7_UnitRender( dh7UNIT_SKY *Uni, dh7ANIM *Ani )
{
  glDepthMask(FALSE);
  DH7_RndPrimDraw(&Uni->sky, MatrIdentity());
  glDepthMask(TRUE);
}
static VOID DH7_UnitClose( dh7UNIT_SKY *Uni, dh7ANIM *Ani )
{
  DH7_RndPrimFree(&Uni->sky);
}

dh7UNIT * DH7_UnitCreateSKY( VOID )
{
  dh7UNIT_SKY *Uni;

  if ((Uni = (dh7UNIT_SKY *)DH7_AnimUnitCreate(sizeof(dh7UNIT_SKY))) == NULL)
    return NULL;
  Uni->Init = (VOID *)DH7_UnitInit;
  Uni->Response = (VOID *)DH7_UnitResponse;
  Uni->Render = (VOID *)DH7_UnitRender;
  return (dh7UNIT *)Uni;
}