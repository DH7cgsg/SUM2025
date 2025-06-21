/* FILE NAME  : rndres.c
 * PROGRAMMER : DH7
 * LAST UPDATE: 14.06.2025
 * PURPOSE    : 3D game project.
 *              
 */

#include "anim/rnd/rnd.h"

VOID DH7_RndResInit( VOID )
{
  DH7_RndShdInit();
  DH7_RndTexInit();
  DH7_RndMtlInit();
  DH7_RndFntInit();
}
VOID DH7_RndResClose( VOID )
{
  DH7_RndFntClose();
  DH7_RndMtlClose();
  DH7_RndTexClose();
  DH7_RndShdClose();
}

