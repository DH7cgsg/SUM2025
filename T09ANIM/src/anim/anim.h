/* FILE NAME  : anim.h
 * PROGRAMMER : DH7
 * LAST UPDATE: 10.06.2025
 * PURPOSE    : 3D animation project.
 *          Common declaration module.
 */
 
#ifndef __anim_h_
#define __anim_h_

#include "rnd/rnd.h"

#define DH7_MAX_UNITS 1000


typedef struct tagdh7UNIT dh7UNIT;
typedef struct tagdh7ANIM dh7ANIM;

#define DH7_UNIT_BASE_FIELDS \
  VOID (*Init)( dh7UNIT *Uni, dh7ANIM *Ani );    \
  VOID (*Close)( dh7UNIT *Uni, dh7ANIM *Ani );    \
  VOID (*Response)( dh7UNIT *Uni, dh7ANIM *Ani );    \
  VOID (*Render)( dh7UNIT *Uni, dh7ANIM *Ani )


struct tagdh7UNIT
{
  DH7_UNIT_BASE_FIELDS;
};
struct tagdh7ANIM
{
  /* Base anim variables */
  HWND hWnd;
  HDC hDC;
  INT W, H;
  dh7UNIT * Units[DH7_MAX_UNITS];
  INT NumOfUnits;
  BOOL IsActive;

  /* Time variables */
  DBL
    GlobalTime, GlobalDeltaTime, /* Global time and interframe interval */
    Time, DeltaTime,             /* Time with pause and interframe interval */
    FPS;                         /* Frames per second value */
  BOOL
    IsPause;

  /* Keyboard and mouse variables */
  BYTE Keys[256];      
  BYTE KeysClick[256]; 
  BYTE KeysOld[256];
  INT Mx, My, Mz, Mdx, Mdy, Mdz;

  /* Joystick variables */
  BYTE
    JBut[32], JButOld[32], JButClick[32]; /* Joystick button states */
  INT JPov;                               /* Joystick point-of-view control [-1,0..7] */
  DBL
    JX, JY, JZ, JR;                       /* Joystick axes */
};

extern dh7ANIM DH7_Anim;
extern INT DH7_MouseWheel;

VOID DH7_AnimInit( HWND hWnd );
VOID DH7_AnimClose( VOID );
VOID DH7_AnimResize( INT W, INT H );
VOID DH7_AnimCopyFrame();
VOID DH7_AnimRender( VOID );
VOID DH7_AnimFlipFullScreen( VOID );
VOID DH7_AnimDoExit( VOID );

VOID DH7_AnimAddUnit( dh7UNIT *Uni );
dh7UNIT * DH7_AnimUnitCreate( INT Size );

VOID DH7_TimerInit( VOID );
VOID DH7_TimerResponse( VOID );

VOID DH7_AnimInputInit( VOID );   
VOID DH7_AnimInputResponse( VOID ); 



#endif  /* __anim_h_ */
 
