#include "2dmap.h"

#ifndef _LEVEL_SHARED_H_
#define _LEVEL_SHARED_H_


void initMainCharacter(int x, int y, int life);
void initCharacter(Character* character, int x, int y, int life);
void setTexturesCastle();
void setTexturesDungeon();
void setTiles();
void setTexturesCharacters();
void setTexturesShared();

#endif /* _LEVEL_SHARED_H_*/
