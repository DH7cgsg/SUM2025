/* FILE NAME  : u_ctrl.c
 * PROGRAMMER : DH7
 * LAST UPDATE: 11.06.2025
 * PURPOSE    : 3D animation project.
 *          Control unit module.
 */

#include "units/units.h"

typedef struct
{
  DH7_UNIT_BASE_FIELDS;

} dh7UNIT_CTRL;

static VOID DH7_UnitInit( dh7UNIT_CTRL *Uni, dh7ANIM *Ani )
{
  
}
static VOID DH7_UnitResponse( dh7UNIT_CTRL *Uni, dh7ANIM *Ani )
{
  if (Ani->DH7_KeysClick['C'])
    DH7_RndChangeColor(RGB(rand() % 255, rand() % 255, rand() % 255));
  if (Ani->DH7_KeysClick['P'])
    Ani->DH7_IsPause = !Ani->DH7_IsPause;
  
  /* Cam movement */
  Ani->CamLoc =
    VecAddVec(Ani->CamLoc,
       VecMulNum(Ani->CamDir, Ani->DH7_GlobalDeltaTime * Ani->DH7_Mdz));

}
static VOID DH7_UnitRender( dh7UNIT_CTRL *Uni, dh7ANIM *Ani )
{
}
static VOID DH7_UnitClose( dh7UNIT_CTRL *Uni, dh7ANIM *Ani )
{
}

dh7UNIT * DH7_UnitCreateCTRL( VOID )
{
  dh7UNIT_CTRL *Uni;

  if ((Uni = (dh7UNIT_CTRL *)DH7_AnimUnitCreate(sizeof(dh7UNIT_CTRL))) == NULL)
    return NULL;
  Uni->Init = (VOID *)DH7_UnitInit;
  Uni->Response = (VOID *)DH7_UnitResponse;
  Uni->Render = (VOID *)DH7_UnitRender;
  return (dh7UNIT *)Uni;
}
