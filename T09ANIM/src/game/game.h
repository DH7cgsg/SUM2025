/* FILE NAME  : game.h
 * PROGRAMMER : DH7
 * LAST UPDATE: 10.06.2025
 * PURPOSE    : 3D game project.
 *              Game header module.
 */
 
#ifndef __game_h_
#define __game_h_

#include "def.h"

typedef struct tagdh7GAME dh7GAME;

struct tagdh7GAME
{
  INT MapHeights[648][1152];
};

extern dh7GAME DH7_Game;

VOID DH7_GameInit( VOID );
VOID DH7_GameClose( VOID );

#endif  /* __game_h_ */