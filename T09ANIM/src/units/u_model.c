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
  VEC Pos, V, OldV;
  FLT AngleRot, AngleY;
} dh7UNIT_MODEL;

static VOID DH7_UnitInit( dh7UNIT_MODEL *Uni, dh7ANIM *Ani )
{
  INT i;

  DH7_RndPrimsLoad(&Uni->Prs, "bin/models/kirby.g3dm");
  Uni->Pos = VecSet(48, 14, 90);
  Uni->V = VecSet(0, 0, 0);
  for (i = 0; i < Uni->Prs.NumOfPrims; i++)
  {
    Uni->Prs.MinBB = VecMinVec(Uni->Prs.Prims[i].MinBB, Uni->Prs.MinBB);
    Uni->Prs.MaxBB = VecMaxVec(Uni->Prs.Prims[i].MaxBB, Uni->Prs.MaxBB);
  }
  
}
static VOID DH7_UnitResponse( dh7UNIT_MODEL *Uni, dh7ANIM *Ani )
{

  if (Ani->Keys['A'])
    Uni->V = VecAddVec(VecSet(10, 0, 0), Uni->V);
  if (Ani->Keys['D'])
    Uni->V = VecAddVec(VecSet(-10, 0, 0), Uni->V);
  if (Ani->Keys['W'])
    Uni->V = VecAddVec(VecSet(0, 0, 10), Uni->V);
  if (Ani->Keys['S'])
    Uni->V = VecAddVec(VecSet(0, 0, -10), Uni->V);

  Uni->Pos = VecAddVec(Uni->Pos, VecMulNum(Uni->V, Ani->GlobalDeltaTime));

  if (Ani->KeysClick['V'])
    DH7_RndCamMode = !DH7_RndCamMode;

  if (DH7_RndCamMode)
  {
  }
  Uni->V = VecSet(0, 0, 0);
}
static VOID DH7_UnitRender( dh7UNIT_MODEL *Uni, dh7ANIM *Ani )
{
  MATR p;

  p = MatrIdentity();
  /*m = MatrMulMatr(m, MatrScale(VecSet1(0.2)));
  m = MatrMulMatr(m, MatrRotateY(Ani->Keys['C'] * Ani-> * 100));
  m = MatrMulMatr(m, MatrTranslate(Uni->Pos));

  DH7_RndPrimsDraw(&Uni->Prs, m); */

  p = MatrMulMatr(p, MatrScale(VecSet1(0.2)));
  p = MatrMulMatr(p, MatrTranslate(Uni->Pos));
  
  
  DH7_RndPrimsDraw(&Uni->Prs, p);






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
