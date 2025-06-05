/* FILE NAME  : t06detg.c
 * PROGRAMMER : DH7

 * LAST UPDATE: 05.06.2025
 * PURPOSE    : det computation using gauss method.
 *              Startup entry-point module.
 */
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <math.h>


typedef double DBL;
#define MAX 10

FILE *F;
DBL M[MAX][MAX];
INT N;
DBL Det;
static SYSTEMTIME st;


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

void Swap( DBL *A, DBL *B )
{
  DBL tmp = *A;

  *A = *B;
  *B = tmp;
}

VOID PrintM( VOID )
{
  INT i, j;

  for (i = 0; i < N; i++)
  {
    for (j = 0; j < N; j++)
      printf("%f ", M[i][j]);
    printf("\n");
  }
  printf("\n");
}

VOID ComputeDetG( VOID )
{
  INT i, y, x, max_col, max_row, j;
  DBL coef;
  

  for (i = 0; i < N; i++)
  {
    max_row = max_col = i;
    for (y = i; y < N; y++)
      for (x = i; x < N; x++)
        if (fabs(M[y][x]) > fabs(M[max_row][max_col]))
        {
          max_row = y;
          max_col = x;
        }
    if (M[max_row][max_col] == 0)
    {
      Det = 0;
      break;
    }
    if (max_row != i)  //rows swap
    {
      for (x = i; x < N; x++)
      {
        Swap(&M[i][x], &M[max_row][x]);
        Det = -Det;
      }
    }
    if (max_col != i) //cols swap
    {
      for (y = i; y < N; y++)
      {
        Swap(&M[y][i], &M[y][max_col]);
        Det = -Det;
      }
    }
    for (j = i + 1; j < N; j++) //subtraction
    {
      coef = M[j][i] / M[i][i];
      M[j][i] = 0;
      for (x = i + 1; x < N; x++)
        M[j][x] -= M[i][x] * coef;
    }
  }
  for (i = 0; i < N; i++)
    Det *= M[i][i];
}

VOID main( VOID )
{
  DBL startMS, endMS, startS, endS;

  Det = 1;
  LoadMatrix("Matrix.txt");
  GetSystemTime(&st);
  startMS = st.wMilliseconds;
  startS = st.wSecond;
  ComputeDetG();
  GetSystemTime(&st);
  endMS = st.wMilliseconds;
  endS = st.wSecond;
  printf("det: %lf\n", Det);
  printf("Milliseconds: %lf\n", endMS - startMS);
  printf("Seconds: %lf", endS - startS);


  _getch();
}