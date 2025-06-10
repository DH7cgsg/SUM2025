/* FILE NAME  : anim.c
 * PROGRAMMER : DH7
 * LAST UPDATE: 10.06.2025
 * PURPOSE    : 3D animation project.
 *          Common declaration module.
 */

#include "anim.h"
#include "def.h"


dh7ANIM DH7_Anim;

VOID DH7_AnimInit( HWND hWnd )
{
  memset(&DH7_Anim, 0, sizeof(DH7_Anim));


  DH7_RndInit(hWnd);
  //TimerInit()
}

VOID DH7_AnimClose( VOID )
{
  DH7_RndClose();
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

  //DH7_TimerResponse();
  for (i = 0; i < DH7_Anim.NumOfUnits; i++)
    DH7_Anim.Units[i]->Response(DH7_Anim.Units[i], &DH7_Anim);
  DH7_RndStart();

  for (i = 0; i < DH7_Anim.NumOfUnits; i++)
    DH7_Anim.Units[i]->Render(DH7_Anim.Units[i], &DH7_Anim);
  DH7_RndEnd();
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

