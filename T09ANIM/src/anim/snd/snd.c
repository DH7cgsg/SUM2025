/* FILE NAME  : snd.h
 * PROGRAMMER : DH7

 * LAST UPDATE: 21.06.2025
 * PURPOSE    : 3D game project.
                Sound system declaration module
 *              
 */

#include "snd.h"


VOID DH7_SndInit( VOID )
{
  mciSendString("open bin/sounds/ambient.mp3 type mpegvideo alias ambient", NULL, 0, NULL);
  mciSendString("play ambient", NULL, 0, NULL);
}
VOID DH7_SndClose( VOID )
{
  mciSendString("close ambient", NULL, 0, NULL);
}