/* FILE NAME  : rndmtl.c
 * PROGRAMMER : DH7
 * LAST UPDATE: 13.06.2025
 * PURPOSE    : 3D animation project.
 *              Rendering materials module.
 */


#include <stdio.h>
#include <string.h>

#include "anim/rnd/rnd.h"
#include "anim/anim.h"

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

INT DH7_RndMtlAdd( dh7MATERIAL *Mtl )
{
  if (DH7_RndMaterialsSize >= DH7_MAX_MATERIALS)
    return 0;
  DH7_RndMaterials[DH7_RndMaterialsSize] = *Mtl;
  return DH7_RndMaterialsSize++;
}

UINT DH7_RndMtlApply( INT MtlNo )
{
  UINT prg;
  dh7MATERIAL *mtl;
  INT loc, i;

  /* Set material pointer */
  if (MtlNo < 0 || MtlNo >= DH7_RndMaterialsSize)
    MtlNo = 0;
  mtl = &DH7_RndMaterials[MtlNo];

   /* Set shader program Id */
  prg = mtl->ShdNo;
  if (prg < 0 || (INT)prg >= DH7_RndShadersSize)
    prg = 0;
  prg = DH7_RndShaders[prg].ProgId;
 
  if (prg == 0)
    return 0;

  glUseProgram(prg);

  if ((loc = glGetUniformLocation(prg, "Time")) != -1)
    glUniform1f(loc, DH7_Anim.Time);
  if ((loc = glGetUniformLocation(prg, "GlobalTime")) != -1)
    glUniform1f(loc, DH7_Anim.GlobalTime);

   /* Set shading parameters */
  if ((loc = glGetUniformLocation(prg, "Ka")) != -1)
    glUniform3fv(loc, 1, &mtl->Ka.X);
  if ((loc = glGetUniformLocation(prg, "Kd")) != -1)
    glUniform3fv(loc, 1, &mtl->Kd.X);
  if ((loc = glGetUniformLocation(prg, "Ks")) != -1)
    glUniform3fv(loc, 1, &mtl->Ks.X);
  if ((loc = glGetUniformLocation(prg, "Ph")) != -1)
    glUniform1f(loc, mtl->Ph);
  if ((loc = glGetUniformLocation(prg, "Trans")) != -1)
    glUniform1f(loc, mtl->Trans);

   /* Set textures */
  for (i = 0; i < 8; i++)
  {
    CHAR tname[] = "IsTexture0";
 
    tname[9] = '0' + i;
    if (mtl->Tex[i] != -1)
    {
      glActiveTexture(GL_TEXTURE0 + i); //sampler create
      glBindTexture(GL_TEXTURE_2D, DH7_RndTextures[mtl->Tex[i]].TexId); //bind sampler with texture
    }
    if ((loc = glGetUniformLocation(prg, tname)) != -1)
      glUniform1i(loc, mtl->Tex[i] != -1);
  }
  return prg;
}

VOID DH7_RndMtlInit( VOID )
{
  dh7MATERIAL def = DH7_RndMtlGetDef();
  dh7MATERIAL gold_mtl =
  {
    "gold",
    {0.24725,0.1995,0.0745},
    {0.75164,0.60648,0.22648},
    {0.628281,0.555802,0.366065},
    51.2, 1,
    {-1, -1, -1, -1, -1, -1, -1, -1},
    0
  };

  DH7_RndMaterialsSize = 0;
  DH7_RndMtlAdd(&def);
  DH7_RndMtlAdd(&gold_mtl);
}

VOID DH7_RndMtlClose( VOID )
{
  dh7MATERIAL def = DH7_RndMtlGetDef();

  DH7_RndMaterialsSize = 0;

  DH7_RndMtlAdd(&def);
}

dh7MATERIAL * DH7_RndMtlGet( INT MtlNo )
{
  if (MtlNo < 0 || MtlNo >= DH7_RndMaterialsSize)
    MtlNo = 0;
  return &DH7_RndMaterials[MtlNo];
}





