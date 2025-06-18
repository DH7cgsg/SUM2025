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
#define DH7_MAX_TEXTURES 30
#define DH7_MAX_SHADERS  47
#define DH7_MAX_MATERIALS 50

typedef struct tagdh7SHADER
{
  CHAR Name[DH7_STR_MAX];
  UINT ProgId;
} dh7SHADER;

extern dh7SHADER DH7_RndShaders[DH7_MAX_SHADERS];
extern INT DH7_RndShadersSize;

VOID DH7_RndShdInit( VOID );

VOID DH7_RndShdClose( VOID );

INT DH7_RndShdAdd( CHAR *ShaderFileNamePrefix );

VOID DH7_RndShdUpdate( VOID );

/* Texture handle functions */

typedef struct tagdh7TEXTURE
{
  CHAR Name[DH7_STR_MAX]; /* Texture name */
  INT W, H;               /* Texture size in pixels */
  UINT TexId;             /* OpenGL texture Id */ 
} dh7TEXTURE;

extern dh7TEXTURE DH7_RndTextures[DH7_MAX_TEXTURES];
extern INT DH7_RndTexturesSize;

INT DH7_RndTexAddImg( CHAR *Name, INT W, INT H, INT C, VOID *Bits );

INT DH7_RndTexAddFromFile( CHAR *FileName );

INT DH7_RndTextureFree( INT TexNo );

DH7_RndTexInit( VOID );

DH7_RndTexClose( VOID );


/* Material handle functions */

typedef struct tagdh7MATERIAL
{
  CHAR Name[DH7_STR_MAX]; /* Material name */
 
  /* Illumination coefficients */    
  VEC Ka, Kd, Ks;           /* Ambient, diffuse, specular coefficients */
  FLT Ph;                   /* Phong power coefficient */
 
  FLT Trans;                /* Transparency factor */
 
  INT Tex[8];               /* Texture references from texture table (or -1) */
 
  INT ShdNo; 
} dh7MATERIAL;

extern dh7MATERIAL DH7_RndMaterials[DH7_MAX_MATERIALS];
extern INT DH7_RndMaterialsSize;

dh7MATERIAL DH7_RndMtlGetDef( VOID );

INT DH7_RndMtlAdd( dh7MATERIAL *Mtl ); 

UINT DH7_RndMtlApply( INT MtlNo );

VOID DH7_RndMtlInit( VOID );

VOID DH7_RndMtlClose( VOID );

dh7MATERIAL * DH7_RndMtlGet( INT MtlNo );

/* Fonts handle functions */
typedef struct tagdh7FONT
{
  DWORD LineH, BaseH; /* Font line height and base line height in pixels */
  FLT AdvanceX[256];  /* Every letter shift right value (0 if no letter present) */
} dh7FONT;

BOOL DH7_RndFntLoad( CHAR *FileName );
VOID DH7_RndFntInit( VOID );
VOID DH7_RndFntClose( VOID );
VOID DH7_RndFntDraw( CHAR *Str, VEC Pos, FLT Size );






#endif /* __rndres_h_  */