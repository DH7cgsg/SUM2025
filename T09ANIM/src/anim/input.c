/* FILE NAME  : input.c
 * PROGRAMMER : DH7
 * LAST UPDATE: 11.06.2025
 * PURPOSE    : 3D animation project.
 *          input module
 */

#include "anim.h"


INT DH7_MouseWheel;

static VOID DH7_AnimKeyboardInit( VOID )
{

}
static VOID DH7_AnimKeyboardResponse( VOID )
{
  INT i;

  GetKeyboardState(DH7_Anim.Keys);
  for (i = 0; i < 256; i++)
  {
    DH7_Anim.Keys[i] >>= 7;
    DH7_Anim.KeysClick[i] = DH7_Anim.Keys[i] && !DH7_Anim.KeysOld[i];  
  }
 
  memcpy(DH7_Anim.KeysOld, DH7_Anim.Keys, 256);

}
static VOID DH7_AnimMouseInit( VOID )
{
  /* Default values */
  DH7_Anim.Mx = 0;
  DH7_Anim.My = 0;
  DH7_Anim.Mz = 0;
  DH7_MouseWheel = 0;
}
static VOID DH7_AnimMouseResponse( VOID )
{
  POINT pt;
 
  GetCursorPos(&pt);
  ScreenToClient(DH7_Anim.hWnd, &pt);
 
  /* Cords eval */
  DH7_Anim.Mdx = pt.x - DH7_Anim.Mx;
  DH7_Anim.Mdy = pt.y - DH7_Anim.My;
  /* Abs values */
  DH7_Anim.Mx = pt.x;
  DH7_Anim.My = pt.y;

  DH7_Anim.Mdz = DH7_MouseWheel;
  DH7_Anim.Mz += DH7_MouseWheel;
  DH7_MouseWheel = 0;
} 
static VOID DH7_AnimJoystickInit( VOID )
{
}
static VOID DH7_AnimJoystickResponse( VOID )
{
} 
VOID DH7_AnimInputInit( VOID )
{
  DH7_AnimKeyboardInit();
  DH7_AnimMouseInit();
  DH7_AnimJoystickInit();
}
VOID DH7_AnimInputResponse( VOID )
{
  DH7_AnimKeyboardResponse();
  DH7_AnimMouseResponse();
  DH7_AnimJoystickResponse();

}