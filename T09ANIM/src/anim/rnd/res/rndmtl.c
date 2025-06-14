/* FILE NAME  : rndmtl.c
 * PROGRAMMER : DH7
 * LAST UPDATE: 13.06.2025
 * PURPOSE    : 3D animation project.
 *              Rendering materials module.
 */


#include <stdio.h>
#include <string.h>

#include "anim/rnd/rnd.h"

/* material stock */
dh7MATERIAL DH7_RndMaterials[DH7_MAX_MATERIALS]; /* Array of materials */
INT DH7_RndMaterialsSize; 

dh7MATERIAL DH7_RndMtlGetDef( VOID )
{
  dh7MATERIAL def_mtl =
  {
    "Default",
    {0.1, 0.1, 0.1},
    {0.90, 0.90, 0.90},
    {0.30, 0.30, 0.30},
    30, 1,
    {-1, -1, -1, -1, -1, -1, -1, -1},
    0
  };
 
  return def_mtl;
}

INT DH7_RndMaterialAdd( dh7MATERIAL *Mtl )
{
  if (DH7_RndMaterialsSize >= DH7_MAX_MATERIALS)
    return 0;
  DH7_RndMaterials[DH7_RndMaterialsSize] = *Mtl;
  return DH7_RndMaterialsSize++;
}

UINT DH7_RndMaterialApply( INT MtlNo )
{
}

VOID DH7_RndMtlInit( VOID )
{
  dh7MATERIAL def = DH7_RndMtlGetDef();

  DH7_RndMaterialsSize = 0;
  DH7_RndMaterialAdd(&def);
}

VOID DH7_RndMtlClose( VOID )
{
  INT i;


}

dh7MATERIAL * DH7_RndMtlGet( INT MtlNo )
{
  if (MtlNo < 0 || MtlNo >= DH7_RndMaterialsSize)
    MtlNo = 0;
  return &DH7_RndMaterials[MtlNo];

}





