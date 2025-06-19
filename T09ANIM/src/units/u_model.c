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
  VEC Pos, Vx, Vy, g;
  FLT AngleRot, AngleY;
} dh7UNIT_MODEL;

static VOID DH7_UnitInit( dh7UNIT_MODEL *Uni, dh7ANIM *Ani )
{
  INT i;

  DH7_RndPrimsLoad(&Uni->Prs, "bin/models/konteynrer.g3dm");
  Uni->Pos = VecSet(480, 85, 60);
  //Uni->Pos = VecSet1(0);
  Uni->Vx = VecSet(0, 0, 0);
  Uni->Vy = VecSet(0, 0, 0);
  Uni->g =  VecSet(0, 10, 0);
  for (i = 0; i < Uni->Prs.NumOfPrims; i++)
  {
    Uni->Prs.MinBB = VecMinVec(Uni->Prs.Prims[i].MinBB, Uni->Prs.MinBB);
    Uni->Prs.MaxBB = VecMaxVec(Uni->Prs.Prims[i].MaxBB, Uni->Prs.MaxBB);
  }
  
}
static VOID DH7_UnitResponse( dh7UNIT_MODEL *Uni, dh7ANIM *Ani )
{
  static CHAR buf[1000];

  if (Ani->Keys['A'])
    Uni->Vx = VecAddVec(VecSet(500, 0, 0), Uni->Vx);
  if (Ani->Keys['D'])
    Uni->Vx = VecAddVec(VecSet(-500, 0, 0), Uni->Vx);
  if (Ani->Keys['W'])
    Uni->Vx = VecAddVec(VecSet(0, 0, 500), Uni->Vx);
  if (Ani->Keys['S'])
    Uni->Vx = VecAddVec(VecSet(0, 0, -500), Uni->Vx);

  Uni->Pos = VecAddVec(Uni->Pos, VecMulNum(Uni->Vx, Ani->GlobalDeltaTime));
  Uni->Pos = VecAddVec(Uni->Pos, VecMulNum(Uni->Vy, Ani->GlobalDeltaTime));

  if (Ani->KeysClick['V'])
    DH7_RndCamMode = !DH7_RndCamMode;

  if (DH7_RndCamMode)
  {
  }
  //printf("%lf\n", DH7_Anim.MapHeights[(INT)Uni->Pos.Z][(INT)Uni->Pos.X]);
  //printf("Pos: %lf %lf %lf", Uni->Pos.X, Uni->Pos.Y, Uni->Pos.Z);
  
  /*if (Uni->Pos.Y != 13)
    Uni->Vy = VecSubVec(Uni->Vy, VecMulNum(Uni->g, Ani->GlobalDeltaTime));
  else
    Uni->Vy = VecSet1(0); */
  
  Uni->Vx = VecSet(0, 0, 0);
}
static VOID DH7_UnitRender( dh7UNIT_MODEL *Uni, dh7ANIM *Ani )
{
  MATR p;
  static CHAR buf[1000];

  p = MatrIdentity();
  /*m = MatrMulMatr(m, MatrScale(VecSet1(0.2)));
  m = MatrMulMatr(m, MatrRotateY(Ani->Keys['C'] * Ani-> * 100));
  m = MatrMulMatr(m, MatrTranslate(Uni->Pos));

  DH7_RndPrimsDraw(&Uni->Prs, m); */

  p = MatrMulMatr(p, MatrScale(VecSet1(10)));
  p = MatrMulMatr(p, MatrTranslate(Uni->Pos));
  
  sprintf(buf,"Player Pos:  %lf, %lf, %lf", Uni->Pos.X, Uni->Pos.Y, Uni->Pos.Z);
  DH7_RndFntDraw(buf, VecSet(0, -200, 0), 30);

  memset(buf, 0, sizeof(buf));
  sprintf(buf,"hgt:  %i", DH7_Anim.MapHeights[(INT)Uni->Pos.Z][(INT)Uni->Pos.X]);
  DH7_RndFntDraw(buf, VecSet(0, -400, 0), 30);

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
