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
  VEC Pos;
  dh7PRIM Ball;
} dh7UNIT_BALL;

static VOID DH7_UnitInit( dh7UNIT_BALL *Uni, dh7ANIM *Ani )
{
  DH7_RndPrimLoad(&Uni->Ball, "bin/models/Chevrolet_Aveo_Sonic_2012.obj");
}
static VOID DH7_UnitResponse( dh7UNIT_BALL *Uni, dh7ANIM *Ani )
{
  Uni->Pos.Y += 0;
}
static VOID DH7_UnitRender( dh7UNIT_BALL *Uni, dh7ANIM *Ani )
{
  MATR m;

  m = MatrIdentity();
  //m = MatrMulMatr(MatrScale(VecSet1(0.001)), MatrRotateY(Ani->Time * 200));
  //m = MatrMulMatr(m, MatrTranslate(VecSet(0, 3 * fabs(sin(Ani->Time * 2)), 0)));
  m = MatrMulMatr(m, MatrScale(VecSet1(0.001)));
  m = MatrMulMatr(m, MatrTranslate(VecSet(Uni->Pos.X, Uni->Pos.Y, Uni->Pos.Z)));

  DH7_RndPrimDraw(&Uni->Ball, m);
}
static VOID DH7_UnitClose( dh7UNIT_BALL *Uni, dh7ANIM *Ani )
{
  DH7_RndPrimFree(&Uni->Ball);
}

dh7UNIT * DH7_UnitCreateBall( VOID )
{
  dh7UNIT_BALL *Uni;

  if ((Uni = (dh7UNIT_BALL *)DH7_AnimUnitCreate(sizeof(dh7UNIT_BALL))) == NULL)
    return NULL;
  Uni->Init = (VOID *)DH7_UnitInit;
  Uni->Response = (VOID *)DH7_UnitResponse;
  Uni->Render = (VOID *)DH7_UnitRender;
  return (dh7UNIT *)Uni;
}
