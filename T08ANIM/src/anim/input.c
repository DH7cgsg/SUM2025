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

  GetKeyboardState(DH7_Anim.DH7_Keys);
  for (i = 0; i < 256; i++)
  {
    DH7_Anim.DH7_Keys[i] >>= 7;
    DH7_Anim.DH7_KeysClick[i] = DH7_Anim.DH7_Keys[i] && !DH7_Anim.DH7_KeysOld[i];  
  }
 
  memcpy(DH7_Anim.DH7_KeysOld, DH7_Anim.DH7_Keys, 256);

}
static VOID DH7_AnimMouseInit( VOID )
{
  /* Default values */
  DH7_Anim.DH7_Mx = 0;
  DH7_Anim.DH7_My = 0;
  DH7_Anim.DH7_Mz = 0;
  DH7_MouseWheel = 0;
}
static VOID DH7_AnimMouseResponse( VOID )
{
  POINT pt;
 
  GetCursorPos(&pt);
  ScreenToClient(DH7_Anim.hWnd, &pt);
 
  /* Cords eval */
  DH7_Anim.DH7_Mdx = pt.x - DH7_Anim.DH7_Mx;
  DH7_Anim.DH7_Mdy = pt.y - DH7_Anim.DH7_My;
  /* Abs values */
  DH7_Anim.DH7_Mx = pt.x;
  DH7_Anim.DH7_My = pt.y;

  DH7_Anim.DH7_Mdz = DH7_MouseWheel;
  DH7_Anim.DH7_Mz += DH7_MouseWheel;
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