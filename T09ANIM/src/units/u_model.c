/* FILE NAME  : anim.c
 * PROGRAMMER : DH7
 * LAST UPDATE: 10.06.2025
 * PURPOSE    : 3D game project.
 *          Common declaration module.
 */

#include "units/units.h"
#include "game/game.h"

typedef struct 
{
  DH7_UNIT_BASE_FIELDS;
  dh7PRIMS Prs;
  VEC Pos, Vplane, Vy, g, Orient;
  FLT AngleRot;
  
} dh7UNIT_MODEL;

static VOID DH7_UnitInit( dh7UNIT_MODEL *Uni, dh7ANIM *Ani )
{
  VEC B;

  DH7_RndPrimsLoad(&Uni->Prs, "bin/models/yoshi.g3dm");
  Uni->Pos = VecSet(605, 85, 558);
  DH7_Anim.PlayerPos = Uni->Pos;
  Uni->Vplane = VecSet(0, 0, 0);
  Uni->Vy = VecSet(0, 0, 0);
  Uni->g =  VecSet(0, 700, 0);
  Uni->Orient = VecSet(0, 0, 1);

  
  B = VecSubVec(Uni->Prs.MaxBB, Uni->Prs.MinBB);
  Uni->Prs.mTrans =
    MatrMulMatr(MatrTranslate(VecAddVec(VecNeg(Uni->Prs.MinBB), VecSet(-B.X / 2, 0, -B.Z / 2))), 
                MatrScale(VecSet1(15 / B.Z)));
}
static VOID DH7_UnitResponse( dh7UNIT_MODEL *Uni, dh7ANIM *Ani )
{
  static CHAR buf[1000];

  /* movement */
  if (Ani->Keys['W'])
    Uni->Vplane = VecAddVec(VecMulNum(Uni->Orient, 200), Uni->Vplane);
  if (Ani->KeysClick[VK_SPACE] && VecCompare(Uni->Vy, VecSet(0, 0, 0)))
    Uni->Vy = VecSet(0, 300, 0);

  Uni->AngleRot += Ani->Keys[VK_RIGHT] * -200 * Ani->GlobalDeltaTime;
  Uni->AngleRot += Ani->Keys[VK_LEFT] * 200 * Ani->GlobalDeltaTime;
  

  /* Pos and orient */
  Uni->Orient = VectorTransform(VecSet(0, 0, 1), MatrRotateY(Uni->AngleRot));
  
  Uni->Pos = VecAddVec(Uni->Pos, VecMulNum(Uni->Vplane, Ani->GlobalDeltaTime));
  Uni->Pos = VecAddVec(Uni->Pos, VecMulNum(Uni->Vy, Ani->GlobalDeltaTime));

  /* Gravity */

  if (Uni->Pos.Y < 15)
  {
    Uni->Pos = VecSet(605, 85, 558); 
  }
  DH7_Anim.PlayerPos = Uni->Pos;
  if (DH7_Game.MapHeights[(INT)Uni->Pos.Z][(INT)Uni->Pos.X] != 0)
  {
    if (Uni->Pos.Y > 85 || Uni->Pos.Y < 75)
      Uni->Vy = VecSubVec(Uni->Vy, VecMulNum(Uni->g, Ani->GlobalDeltaTime));
    else 
      Uni->Vy = VecSet1(0);
  } 
  else
  {
    Uni->Vy = VecSubVec(Uni->Vy, VecMulNum(Uni->g, Ani->GlobalDeltaTime));
  }  
  Uni->Vplane = VecSet(0, 0, 0);

  /* Cam mode */
  if (Ani->KeysClick[VK_F8])
    DH7_RndCamMode = !DH7_RndCamMode;

  if (DH7_RndCamMode == DH7_RND_CAM_LOCKED)
  {
    VEC poi;

    poi = Uni->Pos;
    poi = VecAddVec(VecAddVec(VecSet(0, CamH, 0), VecMulNum(VecNeg(VecNormalize(Uni->Orient)), CamD)), poi);
    DH7_RndCamLoc = VecAddVec(DH7_RndCamLoc, VecMulNum(VecSubVec(poi, DH7_RndCamLoc), Ani->GlobalDeltaTime));
    DH7_RndCamSet(DH7_RndCamLoc, Uni->Pos, VecSet(0, 1, 0));
  }

}

//p = MatrMulMatr(p, MatrScale(VecSet1(2)));
  //p = MatrMulMatr(p, MatrMulMatr(MatrTranslate(Uni->Pos), MatrTranslate(VecSet(-4, 0, 86))));

static VOID DH7_UnitRender( dh7UNIT_MODEL *Uni, dh7ANIM *Ani )
{
  MATR p;
  static CHAR buf[1000];
  

  p = MatrIdentity();

  
  p = MatrMulMatr(p, MatrRotateY(Uni->AngleRot));
  p = MatrMulMatr(p, MatrTranslate(Uni->Pos));
  
  
  sprintf(buf,"Player Pos:  %i, %i, %i", (INT)Uni->Pos.X, (INT)Uni->Pos.Y, (INT)Uni->Pos.Z);
  DH7_RndFntDraw(buf, VecSet(0, -200, 0), 30);

  memset(buf, 0, sizeof(buf));

  sprintf(buf,"hgt:  %i", DH7_Game.MapHeights[(INT)Uni->Pos.Z][(INT)Uni->Pos.X]);
  DH7_RndFntDraw(buf, VecSet(0, -400, 0), 30);

  memset(buf, 0, sizeof(buf));

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
