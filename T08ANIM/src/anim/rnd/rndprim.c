/* FILE NAME  : rndprim.c
 * PROGRAMMER : DH7

 * LAST UPDATE: 09.06.2025
 * PURPOSE    : anim
 *              
 */

#include <string.h>
#include <stdio.h>

#include "rnd.h"

BOOL DH7_RndPrimCreate( dh7PRIM *Pr, INT NoofV, INT NoofI )
{
  INT size;

  memset(Pr, 0, sizeof(dh7PRIM));
  size = sizeof(dh7VERTEX) * NoofV + sizeof(INT) * NoofI;
  Pr->V = malloc(size);
  if (Pr->V == NULL)
    return FALSE;
  memset(Pr->V, 0, size);
  Pr->I = (INT *)(Pr->V + NoofV);
  Pr->NumOfV = NoofV;
  Pr->NumOfI = NoofI;
  Pr->Trans = MatrIdentity();
  return TRUE;
}

VOID DH7_RndPrimFree( dh7PRIM *Pr )
{
  if (Pr->V != NULL)
    free(Pr->V);
  memset(Pr, 0, sizeof(dh7PRIM));
}
VOID DH7_RndPrimDraw( dh7PRIM *Pr, MATR World )
{
  INT i;
  POINT *pnts;

  MATR M = MatrMulMatr(Pr->Trans, MatrMulMatr(World, DH7_RndMatrVP));
  if ((pnts = malloc(sizeof(POINT) * Pr->NumOfV)) == NULL)
    return;
  for (i = 0; i < Pr->NumOfV; i++)
  {
    VEC p = VecMulMatr(Pr->V[i].P, M);

    pnts[i].x = (INT)((p.X + 1) * DH7_RndFrameW / 2.0);
    pnts[i].y = (INT)((-p.Y + 1) * DH7_RndFrameH / 2.0);
  }

  for (i = 0; i < Pr->NumOfI; i += 3)
  {
    MoveToEx(DH7_hRndDCFrame, pnts[Pr->I[i]].x, pnts[Pr->I[i]].y, NULL);
    LineTo(DH7_hRndDCFrame, pnts[Pr->I[i + 1]].x, pnts[Pr->I[i + 1]].y);
    LineTo(DH7_hRndDCFrame, pnts[Pr->I[i + 2]].x, pnts[Pr->I[i + 2]].y);
    LineTo(DH7_hRndDCFrame, pnts[Pr->I[i]].x, pnts[Pr->I[i]].y);
  }
  free(pnts);
}
BOOL DH7_RndPrimLoad( dh7PRIM *Pr, CHAR *FileName )
{
  FILE *F;
  INT vn = 0, fn = 0;
  CHAR Buf[1000];
 
  memset(Pr, 0, sizeof(dh7PRIM));
  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;
 
  /* Count quantities */
  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
      vn++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT i = 2, n = 0;
 
      while (Buf[i] != 0)
      {
        if (Buf[i - 1] == ' ' && Buf[i] != ' ')
          n++;
        i++;
      }
      fn += n - 2;
    }
  }
 
  if (!DH7_RndPrimCreate(Pr, vn, fn * 3))
  {
    fclose(F);
    return FALSE;
  }
 
  /* Read geometry */
  rewind(F);
  vn = 0;
  fn = 0;
  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      DBL x, y, z;
 
      sscanf(Buf + 2, "%lf%lf%lf", &x, &y, &z);
      Pr->V[vn++].P = VecSet(x, y, z);
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT i = 2, n = 0, c, c0, c1, c2;
 
      while (Buf[i] != 0)
      {
        if (Buf[i - 1] == ' ' && Buf[i] != ' ')
        {
          sscanf(Buf + i, "%d", &c);
          if (c > 0)
            c--;
          else if (c < 0)
            c = vn + c;
 
          if (n == 0)
            c0 = c;
          else if (n == 1)
            c1 = c;
          else
          {
            c2 = c;
            Pr->I[fn++] = c0;
            Pr->I[fn++] = c1;
            Pr->I[fn++] = c2;
            c1 = c2;
          }
          n++;
        }
        i++;
      }
    }
  }
 
  fclose(F);
  return TRUE;
}                




