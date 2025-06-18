/* FILE NAME  : rndshd.c
 * PROGRAMMER : DH7
 * LAST UPDATE: 13.06.2025
 * PURPOSE    : 3D animation project.
 *              Rendering resources module.
 */

#include <time.h>
#include <stdio.h>
#include <string.h>

#include "anim/rnd/rnd.h"




static CHAR * DH7_RndShdLoadTextFromFile( CHAR *FileName )
{
  FILE *F;
  CHAR *txt;
  INT flen;

  if ((F = fopen(FileName, "rb")) == NULL)
    return NULL;
  fseek(F, 0, SEEK_END);
  flen = ftell(F);

  if ((txt = malloc(flen + 1)) == NULL)
    return NULL;
  memset(txt, 0, flen + 1);
  rewind(F);
  fread(txt, 1, flen, F);

  fclose(F);
  return txt;
}

static VOID DH7_RndShdLog( CHAR *FileNamePrefix, CHAR *Part, CHAR *Text )
{
  FILE *F;

  if ((F = fopen("bin/shaders/DH7{30}Shd.log", "a")) == NULL)
    return;
  fprintf(F, "%s/%s\n%s\n", FileNamePrefix, Part, Text);
  printf("%s/%s\n%s\n", FileNamePrefix, Part, Text);
  fclose(F);
}

static INT DH7_RndShdLoad( CHAR *FileNamePrefix )
{
  CHAR *txt;
  struct
  {
    CHAR *Name; /* Shader name (e.g. "vert") */
    INT Type;   /* Shader type (e.g. GL_VERETX_SHADER) */
    UINT Id;    /* Created shader Id */
  } shds[] =
  {
    {"vert", GL_VERTEX_SHADER},
    {"frag", GL_FRAGMENT_SHADER},
    {"geom", GL_GEOMETRY_SHADER}
  };
  INT i, res, NoofS = sizeof(shds) / sizeof(shds[0]);
  UINT prg = 0;
  BOOL is_ok = TRUE;
  static CHAR Buf[10000];
 
  for (i = 0; i < NoofS; i++)
  {
    /* Build shader file name */
    sprintf(Buf, "bin/shaders/%s/%s.glsl", FileNamePrefix, shds[i].Name);
  
 
    /* Load shader file text */
    txt = DH7_RndShdLoadTextFromFile(Buf);
    if (txt == NULL)
    {
      DH7_RndShdLog(FileNamePrefix, shds[i].Name, "Error load file");
      is_ok = FALSE;
      break;
    }
    /* Create shader */
    shds[i].Id = glCreateShader(shds[i].Type);
    if (shds[i].Id == 0)
    {
      free(txt);
      DH7_RndShdLog(FileNamePrefix, shds[i].Name, "Error create shader");
      is_ok = FALSE;
      break;
    }
 
    /* Attach shader source */
    glShaderSource(shds[i].Id, 1, &txt, NULL);
    free(txt);
 
    /* Compile shader */
    glCompileShader(shds[i].Id);
    glGetShaderiv(shds[i].Id, GL_COMPILE_STATUS, &res);
    if (res != 1)
    {
      glGetShaderInfoLog(shds[i].Id, sizeof(Buf), &res, Buf);
      DH7_RndShdLog(FileNamePrefix, shds[i].Name, Buf);
      is_ok = FALSE;
      break;
    }
  }
 
  /* Create shader program */
  if (is_ok)
  {
    if ((prg = glCreateProgram()) == 0)
    {
      DH7_RndShdLog(FileNamePrefix, "PROG", "Error create program");
      is_ok = FALSE;
    }
    else
    {
      /* Attach shaders to program */
      for (i = 0; i < NoofS; i++)
        if (shds[i].Id != 0)
          glAttachShader(prg, shds[i].Id);
      /* Link shader program */
      glLinkProgram(prg);
      glGetProgramiv(prg, GL_LINK_STATUS, &res);
      if (res != 1)
      {
        glGetProgramInfoLog(prg, sizeof(Buf), &res, Buf);
        DH7_RndShdLog(FileNamePrefix, "PROG error", Buf);
        is_ok = FALSE;
      }
    }
  }
 
  /* Handle errors */
  if (!is_ok)
  {
    /* Delete all shaders */
    for (i = 0; i < NoofS; i++)
      if (shds[i].Id != 0)
      {
        if (prg != 0)
          glDetachShader(prg, shds[i].Id);
        glDeleteShader(shds[i].Id);
      }
    /* Delete shader program */
    if (prg != 0)
      glDeleteProgram(prg);
  }
  return prg;
}

static VOID DH7_RndShdFree( INT Prg )
{
   UINT shds[5], n, i;
 
  if (Prg == 0 || !glIsProgram(Prg))
    return;
 
  glGetAttachedShaders(Prg, 5, &n, shds);
  for (i = 0; i < n; i++)
    if (glIsShader(shds[i]))
    {
      glDetachShader(Prg, shds[i]);
      glDeleteShader(shds[i]);
    }
  glDeleteProgram(Prg);
}
  


/* shader stock */
dh7SHADER DH7_RndShaders[DH7_MAX_SHADERS];
INT DH7_RndShadersSize;




VOID DH7_RndShdInit( VOID )
{
  DH7_RndShadersSize = 0;
  DH7_RndShdAdd("default");
}

VOID DH7_RndShdClose( VOID )
{
  INT i;

  for (i = 0; i < DH7_RndShadersSize; i++)
    DH7_RndShdFree(DH7_RndShaders[i].ProgId);
  DH7_RndShadersSize = 0;
}

INT DH7_RndShdAdd( CHAR *ShaderFileNamePrefix )
{
  INT i;

  for (i = 0; i < DH7_RndShadersSize; i++)
    if (strcmp(ShaderFileNamePrefix, DH7_RndShaders[i].Name) == 0)
      return i;

  if (DH7_RndShadersSize >= DH7_MAX_SHADERS)
    return 0;

  i = DH7_RndShadersSize++;
  strncpy(DH7_RndShaders[i].Name, ShaderFileNamePrefix, DH7_STR_MAX - 1);
  DH7_RndShaders[i].ProgId = DH7_RndShdLoad(ShaderFileNamePrefix);
  return i;
}

VOID DH7_RndShdUpdate( VOID )
{
  INT t;
  static INT OldTime = -1;
 
  t = clock();
  if (OldTime == -1)
    OldTime = t;
 
  if (t - OldTime > 2 * CLOCKS_PER_SEC)
  {
    INT i;
 
    for (i = 0; i < DH7_RndShadersSize; i++)
    {
      DH7_RndShdFree(DH7_RndShaders[i].ProgId);
      DH7_RndShaders[i].ProgId = DH7_RndShdLoad(DH7_RndShaders[i].Name);
    }
    OldTime = t;
  }
}