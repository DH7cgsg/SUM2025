/* FILE NAME  : rnd.h
 * PROGRAMMER : DH7

 * LAST UPDATE: 09.06.2025
 * PURPOSE    : anim
 *              
 */

#include "def.h"
#include "rnd.h"


/* OpenGL render data */
HWND DH7_hRndWnd;        /* Work window handle */
HDC DH7_hRndDC;          /* Work window device context  */
HGLRC DH7_hRndGLRC;         /*OpenGL render context */ 
INT DH7_RndFrameW, DH7_RndFrameH; /* Work window size */

DBL
  DH7_RndProjSize = 0.1,     /* Project plane fit square */
  DH7_RndProjDist = 0.1,     /* Distance to project plane from viewer (near) */
  DH7_RndProjFarClip = 5000;  /* Distance to project far clip plane (far) */
 
MATR
  DH7_RndMatrView, /* View coordinate system matrix */
  DH7_RndMatrProj, /* Projection coordinate system matrix */
  DH7_RndMatrVP;   /* Stored (View * Proj) matrix */

VEC 
  DH7_RndCamLoc,
  DH7_RndCamDir,
  DH7_RndCamAt,
  DH7_RndCamRight,
  DH7_RndCamUp;

BOOL DH7_RndCamMode;
FLT CamD, CamH;

INT DH7_RndShdAddonI[5];
FLT DH7_RndShdAddonF[5];


