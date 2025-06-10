/* FILE NAME  : rnd.h
 * PROGRAMMER : DH7

 * LAST UPDATE: 09.06.2025
 * PURPOSE    : anim
 *              
 */

#include "def.h"
#include "rnd.h"

/* Base render functions */

VOID DH7_RndInit( HWND hWnd )
{
  HDC hDC;

  DH7_hRndWnd = hWnd;
  hDC = GetDC(hWnd);
  DH7_hRndDCFrame = CreateCompatibleDC(hDC);
  ReleaseDC(hWnd, hDC);

  DH7_hRndBmFrame = NULL;

  DH7_RndResize(100, 100);
  DH7_RndCamSet(VecSet(8, 8, 8), VecSet(0, 0, 0), VecSet(0, 1, 0));
}

VOID DH7_RndClose( VOID )
{
  DeleteObject(DH7_hRndBmFrame);
  DeleteDC(DH7_hRndDCFrame);
}

VOID DH7_RndResize( INT W, INT H )
{
  HDC hDC = GetDC(DH7_hRndWnd);
 
  if (DH7_hRndBmFrame)
    DeleteObject(DH7_hRndBmFrame);
  DH7_hRndBmFrame = CreateCompatibleBitmap(hDC, W, H);
  ReleaseDC(DH7_hRndWnd, hDC);

 
  SelectObject(DH7_hRndDCFrame, DH7_hRndBmFrame);

  DH7_RndFrameW = W;
  DH7_RndFrameH = H;
  
  DH7_RndProjSet();
}
VOID DH7_RndCopyFrame( HDC hDC )
{
  BitBlt(hDC, 0, 0, DH7_RndFrameW, DH7_RndFrameH,
    DH7_hRndDCFrame, 0, 0, SRCCOPY);
}

VOID DH7_RndStart( VOID )
{
  SelectObject(DH7_hRndDCFrame, GetStockObject(NULL_PEN));
  SelectObject(DH7_hRndDCFrame, GetStockObject(DC_BRUSH));
  SetDCBrushColor(DH7_hRndDCFrame, RGB(0, 0, 0));
  Rectangle(DH7_hRndDCFrame, 0, 0, DH7_RndFrameW + 1, DH7_RndFrameH + 1);
  /* background clearing */
  
  SelectObject(DH7_hRndDCFrame, GetStockObject(DC_PEN));
  SetDCPenColor(DH7_hRndDCFrame, RGB(255, 255, 255));

}

VOID DH7_RndEnd( VOID )
{
}

/* Camera/projaction functions */

VOID DH7_RndProjSet( VOID )
{
  DBL rx, ry;
 
  rx = ry = DH7_RndProjSize;
  /* Correct aspect ratio */
  if (DH7_RndFrameW > DH7_RndFrameH)
    rx *= (DBL)DH7_RndFrameW / DH7_RndFrameH;
  else
    ry *= (DBL)DH7_RndFrameH / DH7_RndFrameW;
 
  DH7_RndMatrProj =
    MatrFrustum(-rx / 2, rx / 2, -ry / 2, ry / 2,
      DH7_RndProjDist, DH7_RndProjFarClip);
  DH7_RndMatrVP = MatrMulMatr(DH7_RndMatrView, DH7_RndMatrProj);
}
VOID DH7_RndCamSet( VEC Loc, VEC At, VEC Up )
{
  DH7_RndMatrView = MatrView(Loc, At, Up);
  DH7_RndMatrVP = MatrMulMatr(DH7_RndMatrView, DH7_RndMatrProj);
}
