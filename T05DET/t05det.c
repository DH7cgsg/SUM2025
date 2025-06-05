/* FILE NAME  : t05det.c
 * PROGRAMMER : DH7

 * LAST UPDATE: 05.06.2025
 * PURPOSE    : det computation
 *              Startup entry-point module.
 */
#include <stdio.h>
#include <windows.h>
#include <conio.h>

typedef double DBL;
#define MAX 10



INT P[MAX];
BOOL IsParity;
FILE *F;
DBL M[MAX][MAX];
INT N;
DBL Det;

BOOL LoadMatrix( CHAR *FileName )
{
  INT i, j;

  N = 0;
  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;
  fscanf(F, "%d", &N);
  if (N > MAX)
    N = MAX;
  else
    if (N < 0)
      N = 0;
  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
      fscanf(F, "%lf", &M[i][j]);
  fclose(F);
  return TRUE;
}



void Swap( INT *A, INT *B )
{
  INT tmp = *A;

  *A = *B;
  *B = tmp;
}

void Go( INT Pos )
{
  if (Pos == N) {
    DBL prod;
    INT k;

    for (prod = 1, k = 0; k < N; k++)
      prod *= M[k][P[k]];
    Det += prod * (IsParity ? 1 : -1);
  }
  else
  {
    INT k;

    for (k = Pos; k < N; k++) 
    {
      if (k != Pos)
        Swap(&P[Pos], &P[k]), IsParity = !IsParity;
      Go(Pos + 1);
      if (k != Pos)
        Swap(&P[Pos], &P[k]), IsParity = !IsParity;
    }
  }
}

VOID main( VOID )
{
  INT i;

  IsParity = TRUE;
  LoadMatrix("Matrix.txt");

  for (i = 0; i < N; i++)
    P[i] = i;
  Go(0);
  printf("det: %lf", Det);
  _getch();
  fclose(F);
}

