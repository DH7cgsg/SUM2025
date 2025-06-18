/* FILE NAME  : anim.c
 * PROGRAMMER : DH7
 * LAST UPDATE: 10.06.2025
 * PURPOSE    : 3D animation project.
 *          Common declaration module.
 */

#include "units/units.h"

typedef struct 
{
  DH7_UNIT_BASE_FIELDS;
  dh7PRIMS Prs;
  VEC Pos;
} dh7UNIT_MODEL;

static VOID DH7_UnitInit( dh7UNIT_MODEL *Uni, dh7ANIM *Ani )
{
  INT i;

  DH7_RndPrimsLoad(&Uni->Prs, "bin/models/kirby.g3dm");
  Uni->Pos = VecSet(0, 2, 20);
  for (i = 0; i < Uni->Prs.NumOfPrims; i++)
  {
    Uni->Prs.MinBB = VecMinVec(Uni->Prs.Prims[i].MinBB, Uni->Prs.MinBB);
    Uni->Prs.MaxBB = VecMaxVec(Uni->Prs.Prims[i].MaxBB, Uni->Prs.MaxBB);
  }
  
}
static VOID DH7_UnitResponse( dh7UNIT_MODEL *Uni, dh7ANIM *Ani )
{
  INT i;
  VEC axe;

  Uni->Pos = PointTransform(Uni->Pos, MatrTranslate(VecSet((Ani->JX + 0.007828) * 30 * Ani->GlobalDeltaTime, 0, 0)));
  Uni->Pos = PointTransform(Uni->Pos, MatrTranslate(VecSet(0, 0, (Ani->JY + 0.007828) * 30 * Ani->GlobalDeltaTime)));
  //printf("%lf\n", Ani->JZ);

  //for (i = 0; i < Uni->Prs.NumOfPrims; i++)
    //Uni->Prs.Prims[i].Trans = MatrRotate(Ani->JZ * 100000 * Ani->GlobalDeltaTime, axe);

}
static VOID DH7_UnitRender( dh7UNIT_MODEL *Uni, dh7ANIM *Ani )
{
  MATR m;

  m = MatrIdentity();
  m = MatrMulMatr(m, MatrScale(VecSet1(0.2)));
  m = MatrMulMatr(m, MatrTranslate(Uni->Pos));

  DH7_RndPrimsDraw(&Uni->Prs, m);  

}
static VOID DH7_UnitClose( dh7UNIT_MODEL *Uni, dh7ANIM *Ani )
{
  DH7_RndPrimsFree(&Uni->Prs);
}

dh7UNIT * DH7_UnitCreateModel( VOID )
{
  dh7UNIT_MODEL *Uni;

  if ((Uni = (dh7UNIT_MODEL *)DH7_AnimUnitCreate(sizeof(dh7UNIT_MODEL))) == NULL)
    return NULL;
  Uni->Init = (VOID *)DH7_UnitInit;
  Uni->Response = (VOID *)DH7_UnitResponse;
  Uni->Render = (VOID *)DH7_UnitRender;
  Uni->Close = (VOID *)DH7_UnitClose;
  return (dh7UNIT *)Uni;
}
