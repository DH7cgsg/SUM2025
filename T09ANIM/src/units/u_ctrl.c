/* FILE NAME  : u_ctrl.c
 * PROGRAMMER : DH7
 * LAST UPDATE: 11.06.2025
 * PURPOSE    : 3D game project.
 *          Control unit module.
 */

#include "units/units.h"
/* NVidia OpenGL extension defines */
#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049
 


typedef struct
{
  DH7_UNIT_BASE_FIELDS;
  VEC CamLoc, CamDir;
  //dh7PRIM Axes;
} dh7UNIT_CTRL;

static VOID DH7_UnitInit( dh7UNIT_CTRL *Uni, dh7ANIM *Ani )
{
  /*dh7MATERIAL mtl;

  mtl = DH7_RndMtlGetDef();
  strcpy(mtl.Name, "debug material");
  DH7_RndPrimCreate(&Uni->Axes, DH7_RND_PRIM_POINTS, NULL, 1, NULL, 0);
  mtl.ShdNo = DH7_RndShdAdd("debug");
  Uni->Axes.MtlNo = DH7_RndMtlAdd(&mtl);
  */

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
  FLT Dist, cosT, sinT, plen, cosP, sinP, Azimuth, Elevator, sx, sy, Wp, Hp;
  VEC dv;

  if (Ani->KeysClick['P'])
    Ani->IsPause = !Ani->IsPause;
  if (Ani->Keys[VK_CONTROL] && Ani->KeysClick['F'])
    DH7_RndCamSet(VecSet1(11), VecSet(0, 2, 0), VecSet(0, 1, 0));
  if (Ani->KeysClick[VK_F11])
    DH7_AnimFlipFullScreen();
  if (Ani->KeysClick[VK_ESCAPE])
    PostQuitMessage(0);
  
  /* Cam movement */
  /*Uni->CamLoc =
    VecAddVec(Uni->CamLoc,
       VecMulNum(Uni->CamDir, Ani->GlobalDeltaTime * Ani->Mdz * 10)); */
  /* Uni->CamLoc =
    PointTransform(Uni->CamLoc,
      MatrRotateY(Ani->Keys[VK_LBUTTON] *
                  Ani->DeltaTime * Ani->Mdx * -200)); */

  Dist = VecLen(VecSubVec(DH7_RndCamAt, DH7_RndCamLoc));
  cosT = (DH7_RndCamLoc.Y - DH7_RndCamAt.Y) / Dist;
  sinT = sqrt(1 - cosT * cosT);
  plen = Dist * sinT;
  cosP = (DH7_RndCamLoc.Z - DH7_RndCamAt.Z) / plen;
  sinP = (DH7_RndCamLoc.X - DH7_RndCamAt.X) / plen;
  Azimuth = R2D(atan2(sinP, cosP));
  Elevator = R2D(atan2(sinT, cosT));

  Azimuth += Ani->GlobalDeltaTime *
    (-300 * Ani->Keys[VK_LBUTTON] * Ani->Mdx);
     

  Elevator += Ani->GlobalDeltaTime *
    (-100 * Ani->Keys[VK_LBUTTON] * Ani->Mdy);
     

  Dist += Ani->GlobalDeltaTime *
    (-1 * 300 * Ani->Mdz +
     8 * (1 + Ani->Keys[VK_SHIFT] * 30) *
        (Ani->Keys[VK_NEXT] - Ani->Keys[VK_PRIOR]));
     
  if (Elevator < 0.08)
      Elevator = 0.08;
  if (Elevator > 178)
      Elevator = 178;
  if (Dist < 0.1)
      Dist = 0.1; 

  Wp = DH7_RndProjSize;
  Hp = DH7_RndProjSize;
     
  if (Ani->W > Ani->H)
    Wp *= (FLT)Ani->W / Ani->H;
  else
    Hp *= (FLT)Ani->H / Ani->W;
     
  sx = Ani->Keys[VK_RBUTTON] * -Ani->Mdx * Wp / Ani->W * Dist / DH7_RndProjDist;
  sy = Ani->Keys[VK_RBUTTON] * Ani->Mdy * Hp / Ani->H * Dist / DH7_RndProjDist;

  dv = VecAddVec(VecMulNum(DH7_RndCamRight, sx),
                   VecMulNum(DH7_RndCamUp, sy));
  DH7_RndCamAt = VecAddVec(DH7_RndCamAt, dv);
  DH7_RndCamLoc = VecAddVec(DH7_RndCamLoc, dv);
     
  DH7_RndCamSet(PointTransform(VecSet(0, Dist, 0),
                                 MatrMulMatr3(MatrRotateX(Elevator),
                                              MatrRotateY(Azimuth),
                                              MatrTranslate(DH7_RndCamAt))), DH7_RndCamAt, VecSet(0, 1, 0));

}
static VOID DH7_UnitRender( dh7UNIT_CTRL *Uni, dh7ANIM *Ani )
{
  static CHAR Buf[10000];
  INT total_mem_kb = 0, cur_avail_mem_kb = 0;
  glGetIntegerv(GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX, &total_mem_kb);
  glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &cur_avail_mem_kb);

  sprintf(Buf, "FPS = %.5f\n %.2f MiB\n %.2f MiB\n CamX:%lf  CamY:%lf  CamZ:%lf", Ani->FPS, 
                (total_mem_kb - cur_avail_mem_kb) / 1024.0,  total_mem_kb / 1024.0,
                DH7_RndCamLoc.X, DH7_RndCamLoc.Y, DH7_RndCamLoc.Z);
  DH7_RndFntDraw(Buf, VecSet(0, 0, 0), 30);

  //DH7_RndPrimDraw(&Uni->Axes, MatrIdentity());


}


dh7UNIT * DH7_UnitCreateCTRL( VOID )
{
  dh7UNIT_CTRL *Uni;

  if ((Uni = (dh7UNIT_CTRL *)DH7_AnimUnitCreate(sizeof(dh7UNIT_CTRL))) == NULL)
    return NULL;
  Uni->Init = (VOID *)DH7_UnitInit;
  Uni->Response = (VOID *)DH7_UnitResponse;
  Uni->Render = (VOID *)DH7_UnitRender;
  Uni->Close = (VOID *)DH7_UnitClose;
  return (dh7UNIT *)Uni;
}
