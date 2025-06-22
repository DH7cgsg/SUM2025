/* FILE NAME  : u_star.c
 * PROGRAMMER : DH7
 * LAST UPDATE: 22.06.2025
 * PURPOSE    : 3D game project.
 *              Star unit module.
 */

#include "units/units.h"
#include "game/game.h"

typedef struct 
{
  DH7_UNIT_BASE_FIELDS;
  dh7PRIMS star;
  VEC Pos;
  FLT AngleRot;
  
} dh7UNIT_STAR;

static VOID DH7_UnitInit( dh7UNIT_STAR *Uni, dh7ANIM *Ani )
{
  VEC B;

  DH7_RndPrimsLoad(&Uni->star, "bin/models/star.g3dm");
  Uni->Pos = VecSet(85, 100, 570);
  Uni->AngleRot = 45;

  B = VecSubVec(Uni->star.MaxBB, Uni->star.MinBB);
  Uni->star.mTrans =
    MatrMulMatr(MatrTranslate(VecAddVec(VecNeg(Uni->star.MinBB), VecSet(-B.X / 2, 0, -B.Z / 2))), 
                MatrScale(VecSet1(10 / B.Z)));
}
static VOID DH7_UnitResponse( dh7UNIT_STAR *Uni, dh7ANIM *Ani )
{
  
}


static VOID DH7_UnitRender( dh7UNIT_STAR *Uni, dh7ANIM *Ani )
{
  MATR p;

  p = MatrIdentity();
  p = MatrMulMatr(p, MatrRotateY(Uni->AngleRot * Ani->Time * 2));
  if (DH7_Game.StarIsTaken && !DH7_Game.StarIsDelivered)
    p = MatrMulMatr(p, MatrTranslate(VecSet(DH7_Game.PlayerPos.X, DH7_Game.PlayerPos.Y + 30, DH7_Game.PlayerPos.Z)));
  else if (DH7_Game.StarIsDelivered)
    p = MatrMulMatr(p, MatrTranslate(VecSet(540, 110, 530)));
  else
  {
    p = MatrMulMatr(p, MatrTranslate(VecSet(0, fabs(50 * sin(Ani->Time)), 0)));
    p = MatrMulMatr(p, MatrTranslate(Uni->Pos));
  }
  DH7_RndPrimsDraw(&Uni->star, p);

}
static VOID DH7_UnitClose( dh7UNIT_STAR *Uni, dh7ANIM *Ani )
{
  DH7_RndPrimsFree(&Uni->star);
}

dh7UNIT * DH7_UnitCreateSTAR( VOID )
{
  dh7UNIT_STAR *Uni;

  if ((Uni = (dh7UNIT_STAR *)DH7_AnimUnitCreate(sizeof(dh7UNIT_STAR))) == NULL)
    return NULL;
  Uni->Init = (VOID *)DH7_UnitInit;
  Uni->Response = (VOID *)DH7_UnitResponse;
  Uni->Render = (VOID *)DH7_UnitRender;
  Uni->Close = (VOID *)DH7_UnitClose;
  return (dh7UNIT *)Uni;
}
