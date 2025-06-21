/* FILE NAME  : rndres.c
 * PROGRAMMER : DH7
 * LAST UPDATE: 18.06.2025
 * PURPOSE    : 3D game project.
                Fonts handle module.
 *              
 */
#include "anim/rnd/rnd.h"


/* Current font description */
/* Font data */
static dh7FONT DH7_RndFntFont;
 
/* Font characters primitives */
static dh7PRIM DH7_RndFntChars[256];
 
/* Font material */
static INT DH7_RndFntMtlNo;


BOOL DH7_RndFntLoad( CHAR *FileName )
{
  INT i;
  DWORD Sign, W, H, *Tex;
  dh7VERTEX Chars[256][4];
  FILE *F;
  dh7MATERIAL mtl;
 
  if ((F = fopen(FileName, "rb")) == NULL)
    return FALSE;
 
  if (fread(&Sign, 4, 1, F) != 1 || Sign != *(DWORD *)"G3DF")
  {
    fclose(F);
    return FALSE;
  }
 
  fread(&DH7_RndFntFont, sizeof(dh7FONT), 1, F);
  fread(Chars, sizeof(dh7VERTEX), 4 * 256, F);
  fread(&W, 4, 1, F);
  fread(&H, 4, 1, F);
 
  if ((Tex = malloc(W * H * 4)) == NULL)
  {
    memset(&DH7_RndFntFont, 0, sizeof(DH7_RndFntFont));
    fclose(F);
    return FALSE;
  }
  fread(Tex, 4, W * H, F);
  fclose(F);
 

  mtl = DH7_RndMtlGetDef();
  strncpy(mtl.Name, FileName, DH7_STR_MAX - 1);
  mtl.Tex[0] = DH7_RndTexAddImg(FileName, W, H, 4, Tex);
  mtl.ShdNo = DH7_RndShdAdd("font");
  DH7_RndFntMtlNo = DH7_RndMtlAdd(&mtl);
 
  for (i = 0; i < 256; i++)
  {
    DH7_RndPrimCreate(&DH7_RndFntChars[i], DH7_RND_PRIM_TRISTRIP, Chars[i], 4, NULL, 0);
    DH7_RndFntChars[i].MtlNo = DH7_RndFntMtlNo;
  }
 
  free(Tex);
  return TRUE;
}
VOID DH7_RndFntInit( VOID )
{
  DH7_RndFntLoad("bin/fonts/Consolas.g3df");
}
VOID DH7_RndFntClose( VOID )
{
  INT i;
 
  for (i = 0; i < 256; i++)
    DH7_RndPrimFree(&DH7_RndFntChars[i]);
  memset(DH7_RndFntChars, 0, sizeof(DH7_RndFntChars));
  memset(&DH7_RndFntFont, 0, sizeof(DH7_RndFntFont));
}
VOID DH7_RndFntDraw( CHAR *Str, VEC Pos, FLT Size )
{
  FLT startx = Pos.X;

 
  while (*Str != 0)
  {
    if (*Str == '\n')
    {
      Pos.X = startx;
      Pos.Y -= Size;
    }
    else
    {
      if (DH7_RndFntFont.AdvanceX[(BYTE)*Str] != 0)
      {
        DH7_RndPrimDraw(&DH7_RndFntChars[(BYTE)*Str], MatrMulMatr(MatrScale(VecSet(Size, Size, 1)), MatrTranslate(Pos)));
        Pos.X += DH7_RndFntFont.AdvanceX[(BYTE)*Str] * Size;
      }
    }
    Str++;
  }
}



