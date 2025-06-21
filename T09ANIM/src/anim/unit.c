/* FILE NAME  : anim.c
 * PROGRAMMER : DH7
 * LAST UPDATE: 10.06.2025
 * PURPOSE    : 3D game project.
 *          Common declaration module.
 */

#include "anim.h"

static VOID DH7_UnitInit( dh7UNIT *Uni, dh7ANIM *Ani )
{
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
dh7UNIT * DH7_AnimUnitCreate( INT Size )
{
  dh7UNIT *Uni;
 
  /* Memory allocation */
  if (Size < sizeof(dh7UNIT) || (Uni = malloc(Size)) == NULL)
    return NULL;
  memset(Uni, 0, Size);
 
  /* Setup unit methods */
  Uni->Init = DH7_UnitInit;
  Uni->Close = DH7_UnitClose;
  Uni->Response = DH7_UnitResponse;
  Uni->Render = DH7_UnitRender;
 
  return Uni;
}
