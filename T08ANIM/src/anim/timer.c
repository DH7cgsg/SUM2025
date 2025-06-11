/* FILE NAME: timer.c
 * PROGRAMMER: DH7
 * DATE: 10.06.2022
 * PURPOSE: timer implementation module
 */


#include "anim.h"


typedef unsigned long long UINT64;

static UINT64
    StartTime,    /* Start program time */
    OldTime,      /* Previous frame time */
    OldTimeFPS,   /* Old time FPS measurement */
    PauseTime,    /* Time during pause period */
    TimePerSec,   /* Timer resolution */
    FrameCounter; /* Frames counter */
 
VOID DH7_TimerInit( VOID )
{
  LARGE_INTEGER t;
 
  QueryPerformanceFrequency(&t);
  TimePerSec = t.QuadPart;
  QueryPerformanceCounter(&t);
  StartTime = OldTime = OldTimeFPS = t.QuadPart;
  FrameCounter = 0;
  DH7_Anim.DH7_IsPause = FALSE;
  DH7_Anim.DH7_FPS = 30.0;
  PauseTime = 0; 
}

VOID DH7_TimerResponse( VOID )
{
  LARGE_INTEGER t;
 
  QueryPerformanceCounter(&t);
  /* Global time */
  DH7_Anim.DH7_GlobalTime = (DBL)(t.QuadPart - StartTime) / TimePerSec;
  DH7_Anim.DH7_GlobalDeltaTime = (DBL)(t.QuadPart - OldTime) / TimePerSec;
  /* Time with pause */
  if (DH7_Anim.DH7_IsPause)
  {
    DH7_Anim.DH7_DeltaTime = 0;
    PauseTime += t.QuadPart - OldTime;
  }
  else
  {
    DH7_Anim.DH7_DeltaTime = DH7_Anim.DH7_GlobalDeltaTime;
    DH7_Anim.DH7_Time = (DBL)(t.QuadPart - PauseTime - StartTime) / TimePerSec;
  }
 
  /* FPS */
  FrameCounter++;
  if (t.QuadPart - OldTimeFPS > TimePerSec)
  {
    DH7_Anim.DH7_FPS = FrameCounter * TimePerSec / (DBL)(t.QuadPart - OldTimeFPS);
    OldTimeFPS = t.QuadPart;
    FrameCounter = 0;
  }
  OldTime = t.QuadPart; 
}
