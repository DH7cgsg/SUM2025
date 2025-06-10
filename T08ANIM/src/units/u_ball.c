/* FILE NAME  : anim.c
 * PROGRAMMER : DH7
 * LAST UPDATE: 10.06.2025
 * PURPOSE    : 3D animation project.
 *          Common declaration module.
 */

#include "anim/anim.h"
#include "anim/timer.h"

typedef struct 
{
  DH7_UNIT_BASE_FIELDS;
  VEC Pos;
} dh7UNIT_BALL;

static VOID DH7_UnitInit( dh7UNIT_BALL *Uni, dh7ANIM *Ani )
{
  Uni->Pos = VecSet(0, 1, 0);
}
static VOID DH7_UnitClose( dh7UNIT *Uni, dh7ANIM *Ani )
{
  
}
static VOID DH7_UnitResponse( dh7UNIT *Uni, dh7ANIM *Ani )
{
}
static VOID DH7_UnitRender( dh7UNIT *Uni, dh7ANIM *Ani )
{
}

dh7UNIT * DH7_UnitCreateBall( VOID )
{
  dh7UNIT_BALL *Uni;

  if ((Uni = (dh7UNIT_BALL *)DH7_AnimUnitCreate(sizeof(dh7UNIT_BALL))) == NULL)
    return NULL;
  Uni->Init = (VOID *)DH7_UnitInit;
  Uni->Response = (VOID *)DH7_UnitResponse;
  Uni->Render = (VOID *)DH7_UnitRender;
  Uni->Close = (VOID *)DH7_UnitClose;
  return (dh7UNIT *)Uni;
}
