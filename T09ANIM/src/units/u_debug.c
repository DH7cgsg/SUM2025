/* FILE NAME  : u_DEBUG.c
 * PROGRAMMER : DH7
 * LAST UPDATE: 11.06.2025
 * PURPOSE    : 3D animation project.
 *              Debug unit module.
 */

#include "units/units.h"
/* NVidia OpenGL extension defines */
#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049
 


typedef struct
{
  DH7_UNIT_BASE_FIELDS;
  dh7PRIM Axes;
} dh7UNIT_DEBUG;

static VOID DH7_UnitInit( dh7UNIT_DEBUG *Uni, dh7ANIM *Ani )
{
  dh7MATERIAL mtl;

  mtl = DH7_RndMtlGetDef();
  strcpy(mtl.Name, "debug material");
  DH7_RndPrimCreate(&Uni->Axes, DH7_RND_PRIM_POINTS, NULL, 1, NULL, 0);
  mtl.ShdNo = DH7_RndShdAdd("debug");
  Uni->Axes.MtlNo = DH7_RndMtlAdd(&mtl);
}

static VOID DH7_UnitClose( dh7UNIT_DEBUG *Uni, dh7ANIM *Ani )
{
}

static VOID DH7_UnitResponse( dh7UNIT_DEBUG *Uni, dh7ANIM *Ani )
{

  
}
static VOID DH7_UnitRender( dh7UNIT_DEBUG *Uni, dh7ANIM *Ani )
{
  static CHAR Buf[10000];
  INT total_mem_kb = 0, cur_avail_mem_kb = 0;
  glGetIntegerv(GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX, &total_mem_kb);
  glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &cur_avail_mem_kb);

  sprintf(Buf, "FPS = %.5f\n %.2f MiB\n %.2f MiB\n CamX:%lf  CamY:%lf  CamZ:%lf", Ani->FPS, 
                (total_mem_kb - cur_avail_mem_kb) / 1024.0,  total_mem_kb / 1024.0,
                DH7_RndCamLoc.X, DH7_RndCamLoc.Y, DH7_RndCamLoc.Z);
  DH7_RndFntDraw(Buf, VecSet(0, 0, 0), 30);

  DH7_RndPrimDraw(&Uni->Axes, MatrIdentity());

}


dh7UNIT * DH7_UnitCreateDEBUG( VOID )
{
  dh7UNIT_DEBUG *Uni;

  if ((Uni = (dh7UNIT_DEBUG *)DH7_AnimUnitCreate(sizeof(dh7UNIT_DEBUG))) == NULL)
    return NULL;
  Uni->Init = (VOID *)DH7_UnitInit;
  Uni->Response = (VOID *)DH7_UnitResponse;
  Uni->Render = (VOID *)DH7_UnitRender;
  Uni->Close = (VOID *)DH7_UnitClose;
  return (dh7UNIT *)Uni;
}