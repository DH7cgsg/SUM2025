/* FILE NAME  : rnd.h
 * PROGRAMMER : DH7

 * LAST UPDATE: 09.06.2025
 * PURPOSE    : anim
                Rendering system declaration module
 *              
 */

#ifndef __rnd_h_
#define __rnd_h_

#include "def.h"

typedef struct tagdh7VERTEX
{
  VEC P;
} dh7VERTEX;

typedef struct tagdh7PRIM
{
  dh7VERTEX *V;
  INT NumOfV;
  INT *I;
  INT NumOfI;
  MATR Trans;
} dh7PRIM;

extern HWND DH7_hRndWnd;        /* Work window handle */
extern HDC DH7_hRndDCFrame;     /* Work window memory device context  */
extern HBITMAP DH7_hRndBmFrame; /* Work window background bitmap handle */
extern INT DH7_RndFrameW, DH7_RndFrameH; /* Work window size */

extern DBL
  DH7_RndProjSize,     /* Project plane fit square */
  DH7_RndProjDist,     /* Distance to project plane from viewer (near) */
  DH7_RndProjFarClip;  /* Distance to project far clip plane (far) */
 
extern MATR
  DH7_RndMatrView, /* View coordinate system matrix */
  DH7_RndMatrProj, /* Projection coordinate system matrix */
  DH7_RndMatrVP;   /* Stored (View * Proj) matrix */

/* Base render functions */

VOID DH7_RndInit( HWND hWnd );
VOID DH7_RndClose( VOID );
VOID DH7_RndResize( INT W, INT H );
VOID DH7_RndCopyFrame( HDC hDC );
VOID DH7_RndStart( VOID );
VOID DH7_RndEnd( VOID );

/* Projection/camera functions */

VOID DH7_RndProjSet( VOID );
VOID DH7_RndCamSet( VEC Loc, VEC At, VEC Up );

/* Primitive handle functions */

BOOL DH7_RndPrimCreate( dh7PRIM *Pr, INT NoofV, INT NoofI );
VOID DH7_RndPrimFree( dh7PRIM *Pr );
VOID DH7_RndPrimDraw( dh7PRIM *Pr, MATR World );
BOOL DH7_RndPrimLoad( dh7PRIM *Pr, CHAR *FileName );





#endif __rnd_h_