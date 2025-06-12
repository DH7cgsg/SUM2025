/* FILE NAME  : u_ctrl.c
 * PROGRAMMER : DH7
 * LAST UPDATE: 11.06.2025
 * PURPOSE    : 3D animation project.
 *          Control unit module.
 */

#include "units/units.h"

typedef struct
{
  DH7_UNIT_BASE_FIELDS;
  VEC CamLoc, CamDir;
} dh7UNIT_CTRL;

static VOID DH7_UnitInit( dh7UNIT_CTRL *Uni, dh7ANIM *Ani )
{
  Uni->CamLoc = VecSet1(11);
  Uni->CamDir =
    VecSet(-DH7_RndMatrView.A[0][2],
           -DH7_RndMatrView.A[1][2],
           -DH7_RndMatrView.A[2][2]);
}

static VOID DH7_UnitClose( dh7UNIT_CTRL *Uni, dh7ANIM *Ani )
{
}

static VOID DH7_UnitResponse( dh7UNIT_CTRL *Uni, dh7ANIM *Ani )
{
  if (Ani->KeysClick['P'])
    Ani->IsPause = !Ani->IsPause;
  
  /* Cam movement */
  Uni->CamLoc =
    VecAddVec(Uni->CamLoc,
       VecMulNum(Uni->CamDir, Ani->GlobalDeltaTime * Ani->Mdz * 10));
  Uni->CamLoc =
    PointTransform(Uni->CamLoc,
      MatrRotateY(Ani->Keys[VK_LBUTTON] *
                  Ani->DeltaTime * Ani->Mdx * 200));

  DH7_RndCamSet(Uni->CamLoc, Uni->CamDir, VecSet(0, 1, 0));

}
static VOID DH7_UnitRender( dh7UNIT_CTRL *Uni, dh7ANIM *Ani )
{
  static CHAR Buf[102];

  sprintf(Buf, "CGSG Animation: FPS = %.5f %s %s %s", Ani->FPS, glGetString(GL_RENDERER), glGetString(GL_VENDOR), glGetString(GL_VERSION));
  SetWindowText(Ani->hWnd, Buf);
}

dh7UNIT * DH7_UnitCreateCTRL( VOID )
{
  dh7UNIT_CTRL *Uni;

  if ((Uni = (dh7UNIT_CTRL *)DH7_AnimUnitCreate(sizeof(dh7UNIT_CTRL))) == NULL)
    return NULL;
  Uni->Init = (VOID *)DH7_UnitInit;
  Uni->Response = (VOID *)DH7_UnitResponse;
  Uni->Render = (VOID *)DH7_UnitRender;
  return (dh7UNIT *)Uni;
}
