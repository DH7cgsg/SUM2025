/* FILE NAME  : units.c
 * PROGRAMMER : DH7
 * LAST UPDATE: 10.06.2025
 * PURPOSE    : 3D animation project.
 *          Common declaration module.
 */

 
#ifndef __units_h_
#define __units_h_

#define DH7_GET_JOYSTIC_AXIS(A) \
  (2.0 * (ji.dw ## A ## pos - jc.w ## A ## min) / (jc.w ## A ## max - jc.w ## A ## min) – 1)

#include "anim/anim.h"


dh7UNIT * DH7_UnitCreateBall( VOID );
dh7UNIT * DH7_UnitCreateCTRL( VOID );
dh7UNIT * DH7_UnitCreateGRID( VOID );
dh7UNIT * DH7_UnitCreateModel( VOID );
dh7UNIT * DH7_UnitCreateSKY( VOID );
dh7UNIT * DH7_UnitCreateDEBUG( VOID );

#endif  /* __units_h_ */