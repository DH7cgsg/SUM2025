/* FILE NAME  : u_ctrl.c
 * PROGRAMMER : DH7
 * LAST UPDATE: 11.06.2025
 * PURPOSE    : 3D animation project.
 *          Control unit module.
 */

#include "units/units.h" 
#include <stdio.h>

typedef struct
{
  DH7_UNIT_BASE_FIELDS;
  dh7PRIM Land;
} dh7UNIT_GRID;

static VOID DH7_UnitInit( dh7UNIT_GRID *Uni, dh7ANIM *Ani )
{
  INT i, j;
  dh7GRID G;

  DH7_RndGridCreate(&G, 300, 300);
  for (i = 0; i < G.H; i++)
    for (j = 0; j < G.W; j++)
    {
      dh7VERTEX *V = &G.V[i * G.W + j];

      V->P = VecSet(j / (G.W - 1.0) * 50, 1.0, i / (G.H - 1.0) * 500);
    }

  DH7_RndPrimFromGrid(&Uni->Land, &G);
  DH7_RndGridFree(&G);
}

static VOID DH7_UnitClose( dh7UNIT_GRID *Uni, dh7ANIM *Ani )
{
  DH7_RndPrimFree(&Uni->Land);
}

static VOID DH7_UnitResponse( dh7UNIT_GRID *Uni, dh7ANIM *Ani )
{
  
}
static VOID DH7_UnitRender( dh7UNIT_GRID *Uni, dh7ANIM *Ani )
{
  DH7_RndPrimDraw(&Uni->Land, MatrIdentity());
}

dh7UNIT * DH7_UnitCreateGRID( VOID )
{ 
  dh7UNIT *Uni;

  if((Uni = DH7_AnimUnitCreate(sizeof(dh7UNIT_GRID))) == NULL)
    return NULL;

  Uni->Init = (VOID *)DH7_UnitInit;
  Uni->Close = (VOID *)DH7_UnitClose;
  Uni->Response = (VOID *)DH7_UnitResponse;
  Uni->Render = (VOID *)DH7_UnitRender;
  return Uni;
  
}