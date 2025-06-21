/* FILE NAME  : rnd.h
 * PROGRAMMER : DH7

 * LAST UPDATE: 14.06.2025
 * PURPOSE    : anim
                Rendering system declaration module
 *              
 */

#ifndef __rnd_h_
#define __rnd_h_

#define GLEW_STATIC
#include <glew.h>

#include "res/rndres.h"

typedef struct tagdh7VERTEX
{
  VEC P;
  VEC2 T;  /* texture coordinate */
  VEC N;   /* normal */
  VEC4 C;  /* color */
} dh7VERTEX;

typedef enum tagdh7PRIM_TYPE
{
  DH7_RND_PRIM_POINTS,   /* Array of points  – GL_POINTS */
  DH7_RND_PRIM_LINES,    /* Line segments (by 2 points) – GL_LINES */
  DH7_RND_PRIM_TRIMESH,  /* Triangle mesh - array of triangles – GL_TRIANGLES */
  DH7_RND_PRIM_TRISTRIP,
} dh7PRIM_TYPE;

typedef struct tagdh7PRIM
{
  dh7PRIM_TYPE Type; /* Primitive type */
  UINT
    VA,              /* Vertex array Id */
    VBuf,            /* Vertex buffer Id */
    IBuf;            /* Index buffer Id (if 0 - use only vertex buffer) */
  INT NumOfElements; /* Number of indices/vecrtices */
  VEC MinBB, MaxBB;  /* Bound box */
  MATR Trans;  /* Additional transformation matrix */
  INT MtlNo; /* Material num */
} dh7PRIM;

typedef struct tagdh7PRIMS
{
  INT NumOfPrims; /* Number of primitives in array */  
  dh7PRIM *Prims; /* Array of primitives */
  MATR mTrans;     /* Common transformation matrix */
  VEC MinBB, MaxBB;
} dh7PRIMS;

typedef struct tagdh7GRID
{
  INT W, H;      /* Grid size (in vertices) */
  dh7VERTEX *V;  /* Array (2D) of vertex */
} dh7GRID;



extern HWND DH7_hRndWnd;        /* Work window handle */
extern HDC DH7_hRndDC;          /* Work window device context  */
extern HGLRC DH7_hRndGLRC;         /*OpenGL render context */ 
extern INT DH7_RndFrameW, DH7_RndFrameH; /* Work window size */

extern DBL
  DH7_RndProjSize,     /* Project plane fit square */
  DH7_RndProjDist,     /* Distance to project plane from viewer (near) */
  DH7_RndProjFarClip;  /* Distance to project far clip plane (far) */
 
extern MATR
  DH7_RndMatrView, /* View coordinate system matrix */
  DH7_RndMatrProj, /* Projection coordinate system matrix */
  DH7_RndMatrVP;   /* Stored (View * Proj) matrix */

extern VEC
  DH7_RndCamLoc,
  DH7_RndCamDir,
  DH7_RndCamAt,
  DH7_RndCamRight,
  DH7_RndCamUp;
typedef enum tagdh7CAM_MODES
{
  DH7_RND_CAM_FLYING,
  DH7_RND_CAM_LOCKED,
} dh7CAM_MODES;

extern FLT CamD, CamH;
extern INT DH7_RndCamMode;

/* shader addons data */
extern INT DH7_RndShdAddonI[5];
extern FLT DH7_RndShdAddonF[5];

/* Base render functions */

VOID DH7_RndInit( HWND hWnd );
VOID DH7_RndClose( VOID );
VOID DH7_RndResize( INT W, INT H );
VOID DH7_RndCopyFrame( VOID );
VOID DH7_RndStart( VOID );
VOID DH7_RndEnd( VOID );

/* Projection/camera functions */

VOID DH7_RndProjSet( VOID );
VOID DH7_RndCamSet( VEC Loc, VEC At, VEC Up );

/* Primitive handle functions */

VOID DH7_RndPrimCreate( dh7PRIM *Pr, dh7PRIM_TYPE Type, dh7VERTEX *V, INT NoofV, INT *I, INT NoofI );
VOID DH7_RndPrimFree( dh7PRIM *Pr );
VOID DH7_RndPrimDraw( dh7PRIM *Pr, MATR World );
BOOL DH7_RndPrimLoad( dh7PRIM *Pr, CHAR *FileName );
VOID DH7_RndPrimTriMeshAutoNormals( dh7VERTEX *V, INT NumOfV, INT *Ind, INT NumOfI );

VOID APIENTRY glDebugOutput( UINT Source, UINT Type, UINT Id, UINT Severity,
                             INT Length, const CHAR *Message,
                             const VOID *UserParam );

/* multiple prims handle functions */

BOOL DH7_RndPrimsCreate( dh7PRIMS *Prs, INT NumOfPrims );
VOID DH7_RndPrimsFree( dh7PRIMS *Prs );
VOID DH7_RndPrimsDraw( dh7PRIMS *Prs, MATR World );
BOOL DH7_RndPrimsLoad( dh7PRIMS *Prs, CHAR *FileName );



/* Grid handle functions */
BOOL DH7_RndGridCreate( dh7GRID *G, INT W, INT H );
VOID DH7_RndGridFree( dh7GRID *G );
VOID DH7_RndPrimFromGrid( dh7PRIM *Pr, dh7GRID *G );
VOID DH7_RndGridAutoNormals( dh7GRID *G );





#endif __rnd_h_