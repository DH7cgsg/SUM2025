/* FILE NAME  : rndprim.c
 * PROGRAMMER : DH7

 * LAST UPDATE: 09.06.2025
 * PURPOSE    : anim
 *              
 */

#include <string.h>
#include <stdio.h>

#include "rnd.h"
#include "anim/anim.h"

VOID DH7_RndPrimCreate( dh7PRIM *Pr, dh7PRIM_TYPE Type, dh7VERTEX *V, INT NoofV, INT *I, INT NoofI )
{ 
  INT i;

  //memset(Pr, 0, sizeof(dh7PRIM));

  glGenVertexArrays(1, &Pr->VA);
  if (V != NULL && NoofV != 0)
  {
    glGenBuffers(1, &Pr->VBuf);
    glBindVertexArray(Pr->VA);
    glBindBuffer(GL_ARRAY_BUFFER, Pr->VBuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(dh7VERTEX) * NoofV, V, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(dh7VERTEX),
                      (VOID *)0); 
    glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, sizeof(dh7VERTEX),
                      (VOID *)sizeof(VEC)); 
    glVertexAttribPointer(2, 3, GL_FLOAT, FALSE, sizeof(dh7VERTEX),
                      (VOID *)(sizeof(VEC) + sizeof(VEC2))); 
    glVertexAttribPointer(3, 4, GL_FLOAT, FALSE, sizeof(dh7VERTEX),
                      (VOID *)(sizeof(VEC) * 2 + sizeof(VEC2)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);
    
    Pr->MinBB = Pr->MaxBB = V[0].P;
    for (i = 1; i < NoofV; i++)
    {
      Pr->MinBB = VecMinVec(Pr->MinBB, V[i].P);
      Pr->MaxBB = VecMaxVec(Pr->MaxBB, V[i].P);
    }
    
  }

  if (I != NULL && NoofI != 0)
  {
    glGenBuffers(1, &Pr->IBuf);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INT) * NoofI, I, GL_STATIC_DRAW);
    Pr->NumOfElements = NoofI;
  }
  else
    Pr->NumOfElements = NoofV;
  
  Pr->Type = Type;
  Pr->Trans = MatrIdentity();
}

VOID DH7_RndPrimFree( dh7PRIM *Pr )
{
  glBindVertexArray(Pr->VA);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &Pr->VBuf); 

  glBindVertexArray(0);
  glDeleteVertexArrays(1, &Pr->VA);
}

VOID DH7_RndPrimTriMeshAutoNormals( dh7VERTEX *V, INT NumOfV, INT *Ind, INT NumOfI )
{
  INT i;
  //VEC L = VecNormalize(VecSet(1, 3, 2));

  for (i = 0; i < NumOfV; i++)
     V[i].N = VecSet1(0);

  for (i = 0; i < NumOfI; i += 3)
  {
    dh7VERTEX 
      *P0 = &V[Ind[i]],
      *P1 = &V[Ind[i + 1]],
      *P2 = &V[Ind[i + 2]];
    VEC N = VecNormalize(VecCrossVec(VecSubVec(P1->P, P0->P), VecSubVec(P2->P, P0->P)));
 
    P0->N = VecAddVec(P0->N, N);
    P1->N = VecAddVec(P1->N, N);
    P2->N = VecAddVec(P2->N, N);
  }

  for (i = 0; i < NumOfV; i++)
     V[i].N = VecNormalize(V[i].N);

  for (i = 0; i < NumOfV; i++)
  {
    //FLT nl = VecDotVec(L, V[i].N);
    
    //V[i].C = Vec4SetVec3(VecMulNum(VecSet(0, 0.5, 0), nl < 0.1 ? 0.1 : nl));
    V[i].C = Vec4SetVec3(VecSet(0, 0.5, 0));
  }
}


VOID DH7_RndPrimDraw( dh7PRIM *Pr, MATR World )
{
  INT loc, i;
  UINT ProgId;
  MATR 
    w = MatrMulMatr(Pr->Trans, World),
    winv = MatrTranspose(MatrInverse(w)),
    M = MatrMulMatr(w, DH7_RndMatrVP);
  INT gl_prim_type = Pr->Type == DH7_RND_PRIM_LINES ? GL_LINES :
                     Pr->Type == DH7_RND_PRIM_TRIMESH ? GL_TRIANGLES :
                     Pr->Type == DH7_RND_PRIM_TRISTRIP ? GL_TRIANGLE_STRIP :
                     GL_POINTS;
  

  //M = MatrMulMatr(Pr->Trans, MatrMulMatr(World, DH7_RndMatrVP)),
  //glLoadMatrixf(M.A[0]);

  if ((ProgId = DH7_RndMtlApply(Pr->MtlNo)) == 0)
    return;
  glUseProgram(ProgId);
  if ((loc = glGetUniformLocation(ProgId, "MatrWVP")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, M.A[0]);
  if ((loc = glGetUniformLocation(ProgId, "MatrW")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, w.A[0]);
  if ((loc = glGetUniformLocation(ProgId, "MatrWinv")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, winv.A[0]);
  if ((loc = glGetUniformLocation(ProgId, "CamLoc")) != -1) 
    glUniform3fv(loc, 1, &DH7_RndCamLoc.X);
  if ((loc = glGetUniformLocation(ProgId, "ProjSize")) != -1) 
    glUniform1f(loc, DH7_RndProjSize);  
  if ((loc = glGetUniformLocation(ProgId, "ProjDist")) != -1) 
    glUniform1f(loc, DH7_RndProjDist);  
  if ((loc = glGetUniformLocation(ProgId, "FrameW")) != -1) 
    glUniform1f(loc, DH7_RndFrameW);  
  if ((loc = glGetUniformLocation(ProgId, "FrameH")) != -1) 
    glUniform1f(loc, DH7_RndFrameH);

  for (i = 0; i < 5; i++)
  {
    CHAR Name[] = "AddonI0";

    Name[6] = '0' + i;
    if ((loc = glGetUniformLocation(ProgId, Name)) != -1) 
      glUniform1f(loc, DH7_RndShdAddonI[i]);
  }


  glBindVertexArray(Pr->VA);
  if (Pr->IBuf == 0)
  {
    glDrawArrays(gl_prim_type, 0, Pr->NumOfElements);
    glBindVertexArray(0);
  }
  else
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
    glDrawElements(gl_prim_type, Pr->NumOfElements, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
  }
  
  glUseProgram(0);
}
BOOL DH7_RndPrimLoad( dh7PRIM *Pr, CHAR *FileName )
{
  FILE *F;
  INT vn = 0, fn = 0, size, an;
  CHAR Buf[1000];
  dh7VERTEX *V;
  INT *I;

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

  an = fn * 3;
  size = sizeof(dh7VERTEX) * vn + sizeof(INT) * fn * 3;
  if ((V = malloc(size)) == NULL)
  {
    fclose(F);
    return FALSE;
  }
  I = (INT *)(V + vn);
  memset(V, 0, size);
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
      V[vn].C = Vec4Set(0.8, 0.47, 0.29, 1);
      V[vn++].P = VecSet(x, y, z);
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
  
            I[fn++] = c0;
            I[fn++] = c1;
            I[fn++] = c2;           

            c1 = c2;
          }
          n++;
        }
        i++;
      }
    }
  }
 
  fclose(F);
  DH7_RndPrimTriMeshAutoNormals(V, vn, I, an);
  DH7_RndPrimCreate(Pr, DH7_RND_PRIM_TRIMESH, V, vn, I, an);
  free(V);
  return TRUE;
}                




