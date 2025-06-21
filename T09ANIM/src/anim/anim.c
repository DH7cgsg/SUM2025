/* FILE NAME  : anim.c
 * PROGRAMMER : DH7
 * LAST UPDATE: 10.06.2025
 * PURPOSE    : 3D game project.
 *          Common declaration module.
 */

#include "anim.h"
#include "snd/snd.h"



dh7ANIM DH7_Anim;

VOID DH7_AnimInit( HWND hWnd )
{
  memset(&DH7_Anim, 0, sizeof(DH7_Anim));
  DH7_Anim.hWnd = hWnd;
  DH7_RndInit(hWnd);
  DH7_Anim.hDC = DH7_hRndDC;
  DH7_Anim.W = DH7_RndFrameW;
  DH7_Anim.H = DH7_RndFrameH;

  DH7_TimerInit();
  DH7_AnimInputInit();

  DH7_SndInit();
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
  DH7_SndClose();
  memset(&DH7_Anim, 0, sizeof(dh7ANIM));
}

VOID DH7_AnimResize( INT W, INT H )
{
  DH7_RndResize(W, H);
  DH7_Anim.W = W;
  DH7_Anim.H = H;

  DH7_AnimRender();
}

VOID DH7_AnimCopyFrame()
{
  DH7_RndCopyFrame();
}

VOID DH7_AnimRender( VOID )
{
  INT i;
  static BOOL OldIsActive;
  
  DH7_TimerResponse();
  if (DH7_Anim.IsActive)
  {
    if (!OldIsActive)
      DH7_AnimInputInit();
    DH7_AnimInputResponse();
  }
  OldIsActive = DH7_Anim.IsActive;
  
  for (i = 0; i < DH7_Anim.NumOfUnits; i++)
    DH7_Anim.Units[i]->Response(DH7_Anim.Units[i], &DH7_Anim);
  DH7_RndStart();

  for (i = 0; i < DH7_Anim.NumOfUnits; i++)
    DH7_Anim.Units[i]->Render(DH7_Anim.Units[i], &DH7_Anim);
  DH7_RndEnd();
}

VOID DH7_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = FALSE;
  static RECT SaveRc;

  if (!IsFullScreen)
  {
    RECT rc;
    HMONITOR hmon;
    MONITORINFOEX moninfo;

    GetWindowRect(DH7_Anim.hWnd, &SaveRc);
    hmon = MonitorFromWindow(DH7_Anim.hWnd, MONITOR_DEFAULTTONEAREST);
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    rc.left = 0;
    rc.top = 0;
    rc.right = GetSystemMetrics(SM_CXSCREEN);
    rc.bottom = GetSystemMetrics(SM_CYSCREEN);

    rc  = moninfo.rcMonitor;
    AdjustWindowRect(&rc, GetWindowLong(DH7_Anim.hWnd, GWL_STYLE), FALSE);

    SetWindowPos(DH7_Anim.hWnd, HWND_TOP, rc.left, rc.top, rc.right, rc.bottom + 201, SWP_NOOWNERZORDER);
    IsFullScreen = TRUE;
  }
  else 
  {
    SetWindowPos(DH7_Anim.hWnd, HWND_TOP, SaveRc.left, SaveRc.top, SaveRc.right - SaveRc.left, 
                                              SaveRc.bottom - SaveRc.top, SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;

  }
}

VOID DH7_AnimDoExit( VOID )
{
}

VOID DH7_AnimAddUnit( dh7UNIT *Uni )
{
  if (DH7_Anim.NumOfUnits < DH7_MAX_UNITS)
    DH7_Anim.Units[DH7_Anim.NumOfUnits++] = Uni, Uni->Init(Uni, &DH7_Anim);
}

