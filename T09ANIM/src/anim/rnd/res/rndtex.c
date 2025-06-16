/* FILE NAME  : rndtex.c
 * PROGRAMMER : DH7
 * LAST UPDATE: 13.06.2025
 * PURPOSE    : 3D animation project.
 *              Rendering textures module.
 */


#include <stdio.h>
#include <string.h>

#include "anim/rnd/rnd.h"

dh7TEXTURE DH7_RndTextures[DH7_MAX_TEXTURES];
INT DH7_RndTexturesSize;

DH7_RndTexInit( VOID )
{
  DH7_RndTexturesSize = 0;
}

DH7_RndTexClose( VOID )
{
  INT i;

  for (i = 0; i < DH7_RndTexturesSize; i++)
    glDeleteTextures(1, &DH7_RndTextures[i].TexId);
  DH7_RndTexturesSize = 0;
}

INT DH7_RndTexAddImg( CHAR *Name, INT W, INT H, VOID *Bits )
{
  INT mips;

  if (DH7_RndTexturesSize >= DH7_MAX_TEXTURES)
    return -1;
  /* Allocate textures space */
  glGenTextures(1, &DH7_RndTextures[DH7_RndTexturesSize].TexId);
  glBindTexture(GL_TEXTURE_2D, DH7_RndTextures[DH7_RndTexturesSize].TexId);
  mips = log(W > H ? W : H) / log(2);
  mips = mips < 1 ? 1 : mips;
  glTexStorage2D(GL_TEXTURE_2D, mips, GL_BGR, W, H);
  /* Upload texture */
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, W, H, GL_BGR, GL_UNSIGNED_BYTE, Bits);
  glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  strcpy(DH7_RndTextures[DH7_RndTexturesSize].Name, Name);
  DH7_RndTextures[DH7_RndTexturesSize].W = W;
  DH7_RndTextures[DH7_RndTexturesSize].H = H;
  glBindTexture(GL_TEXTURE_2D, 0);
  return DH7_RndTexturesSize++;
}

INT DH7_RndTexAddFromFile( CHAR *FileName )
{
  HBITMAP hBm;
  BITMAP bm;

  if ((hBm = LoadImage(NULL, FileName, IMAGE_BITMAP, 0, 0,
                       LR_LOADFROMFILE | LR_CREATEDIBSECTION)) != NULL)
  {
    INT w, h, ret;

    GetObject(hBm, sizeof(bm), &bm);
    w = bm.bmWidth;
    h = bm.bmHeight;
    
    ret = DH7_RndTexAddImg(FileName, w, h, bm.bmBits);
    DeleteObject(hBm);
    return ret;
  } 
  else
    return -1;
}




