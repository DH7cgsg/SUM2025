/* FILE NAME  : rnd.h
 * PROGRAMMER : DH7

 * LAST UPDATE: 09.06.2025
 * PURPOSE    : anim
 *              
 */

#include "def.h"
#include "rnd.h"

HWND DH7_hRndWnd;        /* Work window handle */
HDC DH7_hRndDCFrame;     /* Work window memory device context  */
HBITMAP DH7_hRndBmFrame; /* Work window background bitmap handle */
INT DH7_RndFrameW, DH7_RndFrameH; /* Work window size */

DBL
  DH7_RndProjSize = 0.1,     /* Project plane fit square */
  DH7_RndProjDist = 0.1,     /* Distance to project plane from viewer (near) */
  DH7_RndProjFarClip = 300;  /* Distance to project far clip plane (far) */
 
MATR
  DH7_RndMatrView, /* View coordinate system matrix */
  DH7_RndMatrProj, /* Projection coordinate system matrix */
  DH7_RndMatrVP;   /* Stored (View * Proj) matrix */

