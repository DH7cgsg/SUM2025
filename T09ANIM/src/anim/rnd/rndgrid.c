/* FILE NAME  : rndgrid.c
 * PROGRAMMER : DH7

 * LAST UPDATE: 13.06.2025
 * PURPOSE    : grid
 *              
 */

#include <string.h>

#include "rnd.h"


BOOL DH7_RndGridCreate( dh7GRID *G, INT W, INT H )
{
  if ((G->V = malloc(W * H * sizeof(dh7VERTEX))) == NULL)
    return FALSE;
  memset(G->V, 0, sizeof(G->V));

  G->W = W;
  G->H = H;

  return TRUE;
}

VOID DH7_RndGridFree( dh7GRID *G )
{
  free(G->V);
}

VOID DH7_RndPrimFromGrid( dh7PRIM *Pr, dh7GRID *G )
{
  INT *Ind;
  INT i, k, j;

  memset(Pr, 0, sizeof(dh7PRIM));
  if ((Ind = malloc(sizeof(INT) * ((G->H - 1) * (G->W * 2 + 1) - 1))) == NULL)
    return;

  for (k = 0, i = 0; i < G->H - 1; i++)
  {
    for (j = 0; j < G->W; j++)
    {
      Ind[k++] = (i + 1) * G->W + j;
      Ind[k++] = i * G->W + j;
    }
    if (i != G->H - 2)
      Ind[k++] = -1;
  }
  DH7_RndPrimCreate(Pr, DH7_RND_PRIM_TRISTRIP, G->V, G->W * G->H,
    Ind, (G->H - 1) * (G->W * 2 + 1) - 1);
  free(Ind);
}

VOID DH7_RndGridAutoNormals( dh7GRID *G )
{
  INT i, j;

  for (i = 0; i < G->W * G->H; i++)
    G->V[i].N = VecSet(0, 0, 0);

  for (i = 0; i < G->H - 1; i++)
    for (j = 0; j < G->W - 1; j++)
    {
      dh7VERTEX
        *P00 = G->V + i * G->W + j,
        *P01 = G->V + i * G->W + j + 1,
        *P10 = G->V + (i + 1) * G->W + j,
        *P11 = G->V + (i + 1) * G->W + j + 1;
      VEC N;
 
      N = VecNormalize(VecCrossVec(VecSubVec(P00->P, P10->P),
                                   VecSubVec(P11->P, P10->P)));
 
      P00->N = VecAddVec(P00->N, N);
      P10->N = VecAddVec(P10->N, N);
      P11->N = VecAddVec(P11->N, N);
 
      N = VecNormalize(VecCrossVec(VecSubVec(P11->P, P01->P),
                                   VecSubVec(P00->P, P01->P)));
 
      P00->N = VecAddVec(P00->N, N);
      P01->N = VecAddVec(P01->N, N);
      P11->N = VecAddVec(P11->N, N);
    }

   for (i = 0; i < G->W * G->H; i++)
     G->V[i].N = VecNormalize(G->V[i].N);

}





