/* FILE NAME  : anim.c
 * PROGRAMMER : DH7
 * LAST UPDATE: 10.06.2025
 * PURPOSE    : 3D animation project.
 *          Common declaration module.
 */

#include "anim.h"



dh7ANIM DH7_Anim;

VOID DH7_AnimInit( HWND hWnd )
{
  memset(&DH7_Anim, 0, sizeof(DH7_Anim));
  DH7_Anim.hWnd = hWnd;
  DH7_RndInit(hWnd);
  DH7_Anim.hDC = DH7_hRndDCFrame;
  DH7_Anim.W = DH7_RndFrameW;
  DH7_Anim.H = DH7_RndFrameH;

  DH7_Anim.CamLoc = VecSet1(11);
  DH7_Anim.CamDir = VecSet(DH7_RndMatrView.A[0][2], DH7_RndMatrView.A[1][2], DH7_RndMatrView.A[2][2]);

  DH7_TimerInit();
  DH7_AnimInputInit();
}

VOID DH7_AnimClose( VOID )
{
  INT i;

  for (i = 0; i < DH7_Anim.NumOfUnits; i++)
  {
    DH7_Anim.Units[i]->Close(DH7_Anim.Units[i], &DH7_Anim);
    free(DH7_Anim.Units[i]);
  }
  DH7_RndClose();
  memset(&DH7_Anim, 0, sizeof(dh7ANIM));
}

VOID DH7_AnimResize( INT W, INT H )
{
  DH7_RndResize(W, H);
  DH7_Anim.W = W;
  DH7_Anim.H = H;

  DH7_AnimRender();
}

VOID DH7_AnimCopyFrame( HDC hDC )
{
  DH7_RndCopyFrame(hDC);
}

VOID DH7_AnimRender( VOID )
{
  INT i;
  CHAR Buf[100];

  DH7_TimerResponse();
  DH7_AnimInputResponse();
  for (i = 0; i < DH7_Anim.NumOfUnits; i++)
    DH7_Anim.Units[i]->Response(DH7_Anim.Units[i], &DH7_Anim);
  DH7_RndStart();

  for (i = 0; i < DH7_Anim.NumOfUnits; i++)
    DH7_Anim.Units[i]->Render(DH7_Anim.Units[i], &DH7_Anim);
  DH7_RndEnd();

  DH7_RndCamSet(DH7_Anim.CamLoc, DH7_Anim.CamDir, VecSet(0, 1, 0));

  SetBkMode(DH7_Anim.hDC, TRANSPARENT);
  SetTextColor(DH7_Anim.hDC, RGB(0, 0, 0));
  TextOut(DH7_Anim.hDC, 30, 30, Buf, sprintf(Buf, "FPS: %lf", DH7_Anim.DH7_FPS));
}

VOID DH7_AnimFlipFullScreen( VOID )
{
}

VOID DH7_AnimDoExit( VOID )
{
}

VOID DH7_AnimAddUnit( dh7UNIT *Uni )
{
  if (DH7_Anim.NumOfUnits < DH7_MAX_UNITS)
    DH7_Anim.Units[DH7_Anim.NumOfUnits++] = Uni, Uni->Init(Uni, &DH7_Anim);
}

