/* FILE NAME: timer.h
 * PROGRAMMER: DH7
 * DATE: 09.06.2022
 * PURPOSE: timer implementation module
 */


#include <windows.h>

double
    DH7_GlobalTime, DH7_GlobalDeltaTime, /* Global time and interframe interval */
    DH7_Time, DH7_DeltaTime,             /* Time with pause and interframe interval */
    DH7_FPS;                         /* Frames per second value */
  BOOL IsPause;

VOID DH7_TimerInit( VOID );
VOID DH7_TimerResponse( VOID );

