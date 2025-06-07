/* FILE NAME  : globe.h
 * PROGRAMMER : DH7

 * LAST UPDATE: 07.06.2025
 * PURPOSE    : 3d globe
 *              Startup entry-point module.
 */
#include <windows.h>
#include "mth.h"
#define GRID_W 50
#define GRID_H 41

VOID GLB_Init( DBL R );
VOID GLB_Resize( INT Ws, INT Hs );
VOID GLB_Draw( HDC hDC, COLORREF color, INT s, DBL angle );
VEC RotateX( VEC P, DBL angle);
VEC RotateY( VEC P, DBL angle);
VEC RotateZ( VEC P, DBL angle);
VOID ChangeDist( DBL dist );