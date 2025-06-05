/* FILE NAME  : t04perm.c
 * PROGRAMMER : DH7

 * LAST UPDATE: 05.06.2025
 * PURPOSE    : permutations.
 *              Startup entry-point module.
 */


#include <stdio.h>
#include <windows.h>

#define MAX 5

INT P[MAX];
BOOL IsParity;
FILE *F;

void Store( VOID )
{
  INT j;

  IsParity = !IsParity;
  for (j = 0; j < MAX; j++)
    fprintf(F, "%d", P[j]);
  fprintf(F, " parity: %s\n", IsParity ? "even" : "odd");
}

void Swap( INT *A, INT *B )
{
  INT tmp = *A;

  *A = *B;
  *B = tmp;
}

void Go( INT Pos )
{
  if (Pos == MAX)
    Store();
  else
  {
    INT k;

    for (k = Pos; k < MAX; k++) 
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

  F = fopen("PERM.TXT", "a");
  if (F == NULL)
    return;
  for (i = 0; i < MAX; i++)
    P[i] = i + 1;
  Go(0);
  fclose(F);
}