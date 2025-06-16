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
  FLT Dist, cosT, sinT, cosP, sinP, plen, Azimuth, Elevator;
  VEC NewLoc;

  if (Ani->KeysClick['P'])
    Ani->IsPause = !Ani->IsPause;
  
  /* Cam movement */
  Uni->CamLoc =
    VecAddVec(Uni->CamLoc,
       VecMulNum(Uni->CamDir, Ani->GlobalDeltaTime * Ani->Mdz * 10));
  /* Uni->CamLoc =
    PointTransform(Uni->CamLoc,
      MatrRotateY(Ani->Keys[VK_LBUTTON] *
                  Ani->DeltaTime * Ani->Mdx * -200)); */

  DH7_RndCamSet(Uni->CamLoc, Uni->CamDir, VecSet(0, 1, 0));

  Dist = VecLen(VecSubVec(DH7_RndCamAt, DH7_RndCamLoc));
 
  cosT = (DH7_RndCamLoc.Y - DH7_RndCamAt.Y) / Dist;
  sinT = sqrt(1 - cosT * cosT);
 
  plen = Dist * sinT;
  cosP = (DH7_RndCamLoc.Z - DH7_RndCamAt.Z) / plen;
  sinP = (DH7_RndCamLoc.X - DH7_RndCamAt.X) / plen;
 
  Azimuth = R2D(atan2(sinP, cosP));
  Elevator = R2D(atan2(sinT, cosT));

  Azimuth += Ani->GlobalDeltaTime * (-30 * Ani->Keys[VK_LBUTTON] * Ani->Mdx);
  Elevator += Ani->GlobalDeltaTime * (-30 * Ani->Keys[VK_LBUTTON] * Ani->Mdy); 
  
  NewLoc = PointTransform(VecSet(0, Dist, 0),
                        MatrMulMatr(MatrRotateX(Elevator),
                                    MatrRotateY(Azimuth)));
  DH7_RndCamSet(NewLoc, DH7_RndCamAt, DH7_RndCamUp);
 


  



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
