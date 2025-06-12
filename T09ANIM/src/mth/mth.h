/* FILE NAME: mth.h
 * PROGRAMMER: DH7
 * DATE: 07.06.2022
 * PURPOSE: 3D math implementation module.
 */
 
#ifndef __mth_h_
#define __mth_h_
 
#include <math.h>
#include <stdio.h>
#include <windows.h>
 
/* Pi math constant */
#define PI 3.14159265358979323846
 
/* Degrees to radians conversion */
#define D2R(A) ((A) * (PI / 180.0))
#define Degree2Radian(a) D2R(a)
 
/* Base float point types */
typedef double DBL;
typedef float FLT;
 
/* Space vector/point representation type */
typedef struct tagVEC
{
  FLT X, Y, Z; /* Vector coordinates */
} VEC;

typedef struct tag2VEC
{
  FLT X, Y; /* Vector coordinates */
} VEC2;

typedef struct tag4VEC
{
  FLT X, Y, Z, W; /* Vector coordinates */
} VEC4;



typedef struct tagMATR
{
  FLT A[4][4];
} MATR;


/* Vector functions */


__inline VEC VecSet( FLT X, FLT Y, FLT Z )
{
  VEC v = {X, Y, Z};
 
  return v;
} /* End of 'VecSet' function */

__inline VEC VecSet1( FLT A )
{
  VEC v = {A, A, A};
 
  return v;
} /* End of 'VecSet1' function */

__inline VEC4 Vec4Set( FLT A, FLT B, FLT C, FLT D )
{
  VEC4 v = {A, B, C, D};
 
  return v;
} /* End of 'Vec4Set' function */

__inline VEC4 Vec4Set1( FLT A )
{
  VEC4 v = {A, A, A, A};
 
  return v;
} /* End of 'Vec4Set1' function */

__inline VEC2 Vec2Set( FLT A, FLT B )
{
  VEC2 v = {A, B};
 
  return v;
}/* End of 'Vec4Set1' function */

__inline VEC2 Vec2Set1( FLT A )
{
  VEC2 v = {A, A};
 
  return v;
} /* End of 'Vec4Set1' function */

__inline VEC4 Vec4SetVec3( VEC v )
{
  VEC4 v1 = {v.X, v.Y, v.Z, 0};

  return v1;
}


__inline VEC VecMinVec( VEC v1, VEC v2 )
{
  VEC r;

  r.X = v1.X > v2.X ? v2.X : v1.X;
  r.Y = v1.Y > v2.Y ? v2.Y : v1.Y;
  r.Z = v1.Z > v2.Z ? v2.Z : v1.Z;

  return r;
}


__inline VEC VecMaxVec( VEC v1, VEC v2 )
{
  VEC r;

  r.X = v1.X < v2.X ? v2.X : v1.X;
  r.Y = v1.Y < v2.Y ? v2.Y : v1.Y;
  r.Z = v1.Z < v2.Z ? v2.Z : v1.Z;

  return r;
}


__inline VEC VecAddVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X + V2.X, V1.Y + V2.Y, V1.Z + V2.Z);
} /* End of 'VecAddVec' function */

__inline VEC VecSubVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X - V2.X, V1.Y - V2.Y, V1.Z - V2.Z);
}

__inline VEC VecMulNum( VEC V1, FLT N )
{
  return VecSet(V1.X * N, V1.Y * N, V1.Z * N);
}

__inline VEC VecDivNum( VEC V1, FLT N )
{
  return VecSet(V1.X / N, V1.Y / N, V1.Z / N);
}

__inline VEC VecNeg( VEC V )              
{
  return VecSet(-V.X, -V.Y, -V.Z);
}

__inline FLT VecDotVec( VEC V1, VEC V2 )
{
  return V1.X * V2.X + V1.Y * V2.Y + V1.Z * V2.Z;
}

__inline VEC VecCrossVec( VEC V1, VEC V2 )
{
  return VecSet(V1.Y * V2.Z - V1.Z * V2.Y, V1.Z * V2.X - V1.X * V2.Z, V1.X * V2.Y - V1.Y * V2.X);
}

__inline FLT VecLen2( VEC V )
{
  return VecDotVec(V, V);
}

__inline FLT VecLen( VEC V ) 
{
  FLT len = VecDotVec(V, V);
 
  if (len == 1 || len == 0)
    return len;
  return sqrt(len);
}

__inline VEC VecNormalize( VEC V ) 
{
  FLT len = VecDotVec(V, V);
 
  if (len == 1 || len == 0)
    return V;
  return VecDivNum(V, sqrt(len));
}

__inline VEC PointTransform( VEC V, MATR M )
{
  return VecSet(V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0],
         V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1],
         V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]); 
}

__inline VEC VectorTransform( VEC V, MATR M )
{
  return VecSet(V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0],
         V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1],
         V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2]);
}

__inline VEC VecMulMatr( VEC V, MATR M )
{
  FLT w = V.X * M.A[0][3] + V.Y * M.A[1][3] + V.Z * M.A[2][3] + M.A[3][3];
 
  return VecSet((V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0]) / w,
                (V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1]) / w,
                (V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]) / w);
} /* End of 'VecMulMatr' function */


/* Matrix functions */

__inline MATR MatrSet(FLT A00, FLT A01, FLT A02, FLT A03,
              FLT A10, FLT A11, FLT A12, FLT A13,
              FLT A20, FLT A21, FLT A22, FLT A23,
              FLT A30, FLT A31, FLT A32, FLT A33)
{
  MATR r =
  {
    {
      {A00, A01, A02, A03},
      {A10, A11, A12, A13},
      {A20, A21, A22, A23},
      {A30, A31, A32, A33}
    }
  };
 
  return r;
}

__inline MATR MatrIdentity( VOID )
{
  return MatrSet(1, 0, 0, 0,
                 0, 1, 0, 0,
                 0, 0, 1, 0,
                 0, 0, 0, 1);
}
__inline MATR MatrTranslate( VEC T )
{
  return MatrSet(1, 0, 0, 0,
                 0, 1, 0, 0,
                 0, 0, 1, 0,
                 T.X, T.Y, T.Z, 1);
}
__inline MATR MatrScale( VEC S )
{
  return MatrSet(S.X, 0, 0, 0,
                 0, S.Y, 0, 0,
                 0, 0, S.Z, 0,
                 0, 0, 0, 1);
}
__inline MATR MatrRotateX( FLT AngleInDegree )
{
  FLT a;

  a = Degree2Radian(AngleInDegree);
  return MatrSet(1, 0, 0, 0,
                 0, cos(a), sin(a), 0,
                 0, -sin(a), cos(a), 0,
                 0, 0, 0, 1);
}

__inline MATR MatrRotateY( FLT AngleInDegree )
{
  FLT a;

  a = Degree2Radian(AngleInDegree);
  return MatrSet(cos(a), 0, -sin(a), 0,
                 0, 1, 0, 0,
                 sin(a), 0, cos(a), 0,
                 0, 0, 0, 1);
}
__inline MATR MatrRotateZ( FLT AngleInDegree )
{
  FLT a;

  a = Degree2Radian(AngleInDegree);
  return MatrSet(cos(a), sin(a), 0, 0,
                 -sin(a), cos(a), 0, 0,
                 0, 0, 1, 0,
                 0, 0, 0, 1);
}
__inline MATR MatrRotate( FLT AngleInDegree, VEC V )
{
  FLT a = D2R(AngleInDegree), s = sin(a), c = cos(a);

  return MatrSet(c + V.X * V.X * (1 - c),
                 V.Y * V.X * (1 - c) - V.Z * s,
                 V.Z * V.X * (1 - c) + V.Y * s,
                 0,
                 V.X * V.Y * (1 - c) + V.Z * s,
                 c + V.Y * V.Y * (1 - c),
                 V.Z * V.Y * (1 - c) - V.X * s,
                 0,
                 V.X * V.Z * (1 - c) - V.Y * s,
                 V.Y * V.Z * (1 - c) + V.X * s,
                 c + V.Z * V.Z * (1 - c),
                 0, 0, 0, 0, 0);

}

__inline MATR MatrMulMatr( MATR M1, MATR M2 )
{
  MATR r = {{{0}}};
  INT i, j, k;
 
  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      for (k = 0; k < 4; k++)
        r.A[i][j] += M1.A[i][k] * M2.A[k][j];
  return r;
}

__inline MATR MatrTranspose( MATR M )
{
  MATR r;
  INT i, j;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      r.A[i][j] = M.A[j][i];
  return r;
}

__inline FLT MatrDeterm3x3( FLT A11, FLT A12, FLT A13,
                   FLT A21, FLT A22, FLT A23,
                   FLT A31, FLT A32, FLT A33 )
{
  return A11 * A22 * A33 + A12 * A23 * A31 + A13 * A21 * A32 -
         A11 * A23 * A32 - A12 * A21 * A33 - A13 * A22 * A31;
         
}

__inline FLT MatrDeterm( MATR M )
{
  return
    M.A[0][0] * MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                              M.A[2][1], M.A[2][2], M.A[2][3],
                              M.A[3][1], M.A[3][2], M.A[3][3]) +
   -M.A[0][1] * MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                              M.A[2][0], M.A[2][2], M.A[2][3],
                              M.A[3][0], M.A[3][2], M.A[3][3]) +
    M.A[0][2] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                              M.A[2][0], M.A[2][1], M.A[2][3],
                              M.A[3][0], M.A[3][1], M.A[3][3]) +
   -M.A[0][2] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                              M.A[2][0], M.A[2][1], M.A[2][2],
                              M.A[3][0], M.A[3][1], M.A[3][2]);
}

__inline MATR MatrInverse( MATR M )
{
  FLT det = MatrDeterm(M);
  MATR r;
 
 
  if (det == 0)
    return MatrIdentity();
 
  /* Build adjoint matrix */
  r.A[0][0] =
    MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                  M.A[2][1], M.A[2][2], M.A[2][3],
                  M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][0] =
   -MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                  M.A[2][0], M.A[2][2], M.A[2][3],
                  M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][0] =
    MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                  M.A[2][0], M.A[2][1], M.A[2][3],
                  M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][0] =
   -MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                  M.A[2][0], M.A[2][1], M.A[2][2],
                  M.A[3][0], M.A[3][1], M.A[3][2]) / det;
 
  r.A[0][1] =
   -MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                  M.A[2][1], M.A[2][2], M.A[2][3],
                  M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][1] =
    MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                  M.A[2][0], M.A[2][2], M.A[2][3],
                  M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][1] =
   -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                  M.A[2][0], M.A[2][1], M.A[2][3],
                  M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][1] =
    MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                  M.A[2][0], M.A[2][1], M.A[2][2],
                  M.A[3][0], M.A[3][1], M.A[3][2]) / det;
 
  r.A[0][2] =
    MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                  M.A[1][1], M.A[1][2], M.A[1][3],
                  M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][2] =
   -MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                  M.A[1][0], M.A[1][2], M.A[1][3],
                  M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][2] =
    MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                  M.A[1][0], M.A[1][1], M.A[1][3],
                  M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][2] =
   -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                  M.A[1][0], M.A[1][1], M.A[1][2],
                  M.A[3][0], M.A[3][1], M.A[3][2]) / det;
 
 
  r.A[0][3] =
   -MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                  M.A[1][1], M.A[1][2], M.A[1][3],
                  M.A[2][1], M.A[2][2], M.A[2][3]) / det;
  r.A[1][3] =
    MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                  M.A[1][0], M.A[1][2], M.A[1][3],
                  M.A[2][0], M.A[2][2], M.A[2][3]) / det;
  r.A[2][3] =
   -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                  M.A[1][0], M.A[1][1], M.A[1][3],
                  M.A[2][0], M.A[2][1], M.A[2][3]) / det;
  r.A[3][3] =
    MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                  M.A[1][0], M.A[1][1], M.A[1][2],
                  M.A[2][0], M.A[2][1], M.A[2][2]) / det;
 
  return r;
}

__inline MATR MatrView( VEC Loc, VEC At, VEC Up1 )
{
  VEC
    Dir = VecNormalize(VecSubVec(At, Loc)),
    Right = VecNormalize(VecCrossVec(Dir, Up1)),
    Up = VecNormalize(VecCrossVec(Right, Dir));
  MATR m =
  {
    {
      {Right.X, Up.X, -Dir.X, 0},
      {Right.Y, Up.Y, -Dir.Y, 0},
      {Right.Z, Up.Z, -Dir.Z, 0},
      {-VecDotVec(Loc, Right), -VecDotVec(Loc, Up), VecDotVec(Loc, Dir), 1}
    }
  };

  return m;
} /* End of 'MatrView' function */

__inline MATR MatrFrustum( FLT L, FLT R, FLT B, FLT T, FLT N, FLT F )
{
  MATR r =
  {
    {
      {2 * N / (R - L), 0, 0, 0},
      {0, 2 * N / (T - B), 0, 0},
      {(R + L) / (R - L), (T + B) / (T - B), (F + N) / (N - F), -1},
      {0, 0, 2 * N * F / (N - F), 0}
    }
  };

  return r;
}



/* Debug functions */


__inline VOID PrintMatrix( CHAR *Text, MATR M )
{
  INT i, j;

  if (Text != NULL && Text[0] != 0)
    printf("%s:\n", Text);
  for (i = 0; i < 4; i++)
  {
    for (j = 0; j < 4; j++)
      printf(" %lf", M.A[i][j]);
    printf("\n");
  }
}

__inline VOID PrintVector( CHAR *Text, VEC V )
{
  if (Text != NULL && Text[0] != 0)
    printf("%s:\n", Text);
  printf("<%lf, %lf, %lf>\n", V.X, V.Y, V.Z);
}

 
#endif /* __mth_h_ */
 
/* END OF 'mth.h' FILE */ 