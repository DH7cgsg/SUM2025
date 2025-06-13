/* FILE NAME  : rndres.h
 * PROGRAMMER : DH7
 * LAST UPDATE: 11.06.2025
 * PURPOSE    : 3D animation project.
 *              Rendering resources declaretion module.
 */

#ifndef __rndres_h_
#define __rndres_h_

#include "def.h"

/* Commom resource handle fnctions */

VOID DH7_RndResInit( VOID );

VOID DH7_RndResClose( VOID );

/* Shader handle functions */

#define DH7_STR_MAX 300

typedef struct tagdh7SHADER
{
  CHAR Name[DH7_STR_MAX];
  UINT ProgId;
} dh7SHADER;

#define DH7_MAX_SHADERS 30
extern dh7SHADER DH7_RndShaders[DH7_MAX_SHADERS];
extern INT DH7_RndShadersSize;

VOID DH7_RndShdInit( VOID );

VOID DH7_RndShdCLose( VOID );

INT DH7_RndShdAdd( CHAR *ShaderFileNamePrefix );

VOID DH7_RndShdUpdate( VOID );

/* Texture handle functions */

/* Material handle functions */


#endif /* __rndres_h_  */