/* FILE NAME  : u_npc.c
 * PROGRAMMER : DH7
 * LAST UPDATE: 21.06.2025
 * PURPOSE    : 3D game project.
 *          Common declaration module.
 */

#include "units/units.h"

typedef struct 
{
  DH7_UNIT_BASE_FIELDS;
  dh7PRIMS PrNPC;
  VEC Pos, Orient;
  FLT AngleRot;
  
} dh7UNIT_NPC;

static VOID DH7_UnitInit( dh7UNIT_NPC *Uni, dh7ANIM *Ani )
{
  VEC B;

  DH7_RndPrimsLoad(&Uni->PrNPC, "bin/models/kirby.g3dm");
  Uni->Pos = VecSet(540, 85, 530);
  Uni->Orient = VecSet(0, 0, 1);

  B = VecSubVec(Uni->PrNPC.MaxBB, Uni->PrNPC.MinBB);
  Uni->PrNPC.mTrans =
    MatrMulMatr(MatrTranslate(VecAddVec(VecNeg(Uni->PrNPC.MinBB), VecSet(-B.X / 2, 0, -B.Z / 2))), 
                MatrScale(VecSet1(20 / B.Z)));
}
static VOID DH7_UnitResponse( dh7UNIT_NPC *Uni, dh7ANIM *Ani )
{
  
}


static VOID DH7_UnitRender( dh7UNIT_NPC *Uni, dh7ANIM *Ani )
{
  MATR m;

  m = MatrRotateY(30);
  m = MatrMulMatr(m, MatrTranslate(Uni->Pos));

  DH7_RndPrimsDraw(&Uni->PrNPC, m);

}
static VOID DH7_UnitClose( dh7UNIT_NPC *Uni, dh7ANIM *Ani )
{
  DH7_RndPrimsFree(&Uni->PrNPC);
}

dh7UNIT * DH7_UnitCreateNPC( VOID )
{
  dh7UNIT_NPC *Uni;

  if ((Uni = (dh7UNIT_NPC *)DH7_AnimUnitCreate(sizeof(dh7UNIT_NPC))) == NULL)
    return NULL;
  Uni->Init = (VOID *)DH7_UnitInit;
  Uni->Response = (VOID *)DH7_UnitResponse;
  Uni->Render = (VOID *)DH7_UnitRender;
  Uni->Close = (VOID *)DH7_UnitClose;
  return (dh7UNIT *)Uni;
}
