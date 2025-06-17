/* FILE NAME  : rnd.h
 * PROGRAMMER : DH7

 * LAST UPDATE: 14.06.2025
 * PURPOSE    : anim
                Rendering system declaration module
 *              
 */

#include "rnd.h"
#include "anim/rnd/res/rndres.h"

BOOL DH7_RndPrimsCreate( dh7PRIMS *Prs, INT NumOfPrims )
{
  memset(Prs, 0, sizeof(dh7PRIMS));
  if ((Prs->Prims = malloc(NumOfPrims * sizeof(dh7PRIM))) == NULL)
    return FALSE;
  memset(Prs->Prims, 0, NumOfPrims * sizeof(dh7PRIM));
  Prs->NumOfPrims = NumOfPrims;
  Prs->mTrans = MatrIdentity();
  return TRUE;
}
VOID DH7_RndPrimsFree( dh7PRIMS *Prs )
{
  INT i;

  if (Prs->Prims != NULL)
  {
    for (i = 0; i < Prs->NumOfPrims; i++)
      DH7_RndPrimFree(&Prs->Prims[i]);
  }
  free(Prs->Prims);
  memset(Prs, 0, sizeof(dh7PRIMS));
}
VOID DH7_RndPrimsDraw( dh7PRIMS *Prs, MATR World )
{
  INT i;
  MATR m = MatrMulMatr(Prs->mTrans, World);

  /* Draw all nonmTransparent primitives */ 
  for (i = 0; i < Prs->NumOfPrims; i++)
    if (DH7_RndMtlGet(Prs->Prims[i].MtlNo)->Trans == 1)
      DH7_RndPrimDraw(&Prs->Prims[i], m);

   /* Draw all transparent primitives with front face culling */ 
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);
  for (i = 0; i < Prs->NumOfPrims; i++)
    if (DH7_RndMtlGet(Prs->Prims[i].MtlNo)->Trans != 1)
      DH7_RndPrimDraw(&Prs->Prims[i], m);

   /* Draw all transparent primitives with back face culling */ 
  glCullFace(GL_BACK);
  for (i = 0; i < Prs->NumOfPrims; i++)
    if (DH7_RndMtlGet(Prs->Prims[i].MtlNo)->Trans != 1)
      DH7_RndPrimDraw(&Prs->Prims[i], m);

  glDisable(GL_CULL_FACE);


}
BOOL DH7_RndPrimsLoad( dh7PRIMS *Prs, CHAR *FileName )
{
  FILE *F;
  INT flen, p, m, t;
  BYTE *mem, *ptr;
  DWORD Sign; /* == "G3DM" */
  DWORD NumOfPrims;
  DWORD NumOfMaterials;
  DWORD NumOfTextures;

  struct mtls
  {
    CHAR Name[300]; /* Material name */
    /* Illumination coefficients */
    VEC Ka, Kd, Ks;     /* Ambient, diffuse, specular coefficients */
    FLT Ph;             /* Phong power coefficient – shininess */
    FLT Trans;          /* Transparency factor */
    DWORD Tex[8];       /* Texture references 
                         * (8 time: texture number in G3DM file, -1 if no texture) */
    /* Shader information */
    CHAR ShaderString[300]; /* Additional shader information */
    DWORD Shader;       /* Shader number (uses after load into memory) */
  } *mtls;

  memset(Prs, 0, sizeof(dh7PRIMS));
  if ((F = fopen(FileName, "rb")) == NULL)
    return FALSE;
  /* Measure file length */
  fseek(F, 0, SEEK_END);
  flen = ftell(F);
   /* Allocate memory */
  if ((mem = malloc(flen)) == NULL)
  {
    fclose(F);
    return FALSE;
  }
  /* Load entire file into memory */
  rewind(F);
  fread(mem, 1, flen, F);
  fclose(F);

  ptr = mem;
  /* Signature */
  Sign = *(DWORD *)ptr;
  ptr += 4;
  if (Sign != *(DWORD *)"G3DM")
  {
    free(mem);
    return FALSE;
  }
   /* Quantities read */
  NumOfPrims = *(DWORD *)ptr;
  ptr += 4;
  NumOfMaterials = *(DWORD *)ptr;
  ptr += 4;
  NumOfTextures = *(DWORD *)ptr;
  ptr += 4;

  if (!DH7_RndPrimsCreate(Prs, NumOfPrims))
  {
    free(mem);
    return FALSE;
  }

  /* Primitives */
  for (p = 0; p < (INT)NumOfPrims; p++)
  {
    DWORD NumOfVertexes;
    DWORD NumOfFacetIndexes;  /* num of facets * 3 */
    DWORD MtlNo;              /* Material number in table below (in material section) */
    dh7VERTEX *V;
    INT *Ind;
 
    NumOfVertexes = *(DWORD *)ptr;
    ptr += 4;
    NumOfFacetIndexes = *(DWORD *)ptr;
    ptr += 4;
    MtlNo = *(DWORD *)ptr;
    ptr += 4;
    V = (dh7VERTEX *)ptr;
    ptr += sizeof(dh7VERTEX) * NumOfVertexes;
    Ind = (INT *)ptr;
    ptr += sizeof(INT) * NumOfFacetIndexes;
 
    DH7_RndPrimCreate(&Prs->Prims[p], DH7_RND_PRIM_TRIMESH, V, NumOfVertexes, Ind, NumOfFacetIndexes);
    Prs->Prims[p].MtlNo = MtlNo + DH7_RndMaterialsSize;
  }
   /* Materials */
  mtls = (struct mtls *)ptr;
  ptr += sizeof(struct mtls) * NumOfMaterials;
  for (m = 0; m < (INT)NumOfMaterials; m++)
  {
    dh7MATERIAL mtl = DH7_RndMtlGetDef();
 
    mtl.Ka = mtls[m].Ka;
    mtl.Kd = mtls[m].Kd;
    mtl.Ks = mtls[m].Ks;
    mtl.Ph = mtls[m].Ph;
    mtl.Trans = mtls[m].Trans;
    strncpy(mtl.Name, mtls[m].Name, DH7_STR_MAX - 1);
 
    for (t = 0; t < 8; t++)
      if (mtls[m].Tex[t] != -1)
        mtl.Tex[t] = mtls[m].Tex[t] + DH7_RndTexturesSize;
      else
        mtl.Tex[t] = -1;
    DH7_RndMtlAdd(&mtl);
  }
  /* Textures */
  for (t = 0; t < (INT)NumOfTextures; t++)
  {
    DWORD W, H, C;
    CHAR *Name = (CHAR *)ptr;
 
    ptr += 300;
    W = *(DWORD *)ptr;
    ptr += 4;
    H = *(DWORD *)ptr;
    ptr += 4;
    C = *(DWORD *)ptr;
    ptr += 4;
    DH7_RndTexAddImg(Name, W, H, C, ptr);
    ptr += W * H * C;
  }
  free(mem);
  return TRUE;
}    /* End of 'DH7_RndPrimsDraw' function */