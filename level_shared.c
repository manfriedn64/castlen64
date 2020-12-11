#include <nusys.h>
#include "2dlibrary.h"
#include "2dmap.h"
#include "level_shared.h"
#include "main.h"
#include "castle.h"

extern Map map;

void initMainCharacter(int x, int y, int life) {
	game.character.keys			= 0;
	game.character.x			= x;
	game.character.y			= y;
	game.character.move_x		= 320;
	game.character.move_y		= 240;
	game.character.state		= CHARACTER_IDLE;
	game.character.life			= life;
	game.character.next			= NULL;
	game.character.previous		= NULL;
	game.character.collision	= testCollisionFeet;
	game.character.back.collision	= NULL;
	game.character.front.collision	= NULL;
	game.character.left.collision	= NULL;
	game.character.right.collision	= NULL;
	game.character.back.action      = testCollisionActionBack;
	game.character.front.action     = testCollisionActionFront;
	game.character.left.action      = testCollisionActionLeft;
	game.character.right.action     = testCollisionActionRight;
	game.character.back.push        = testCollisionPushBack;
	game.character.front.push       = testCollisionPushFront;
	game.character.left.push        = testCollisionPushLeft;
	game.character.right.push       = testCollisionPushRight;
	game.character.back.attack  = (AnimatedSprite){&texture[40], 96, 96,  5, 0};
	game.character.back.defend  = (AnimatedSprite){&texture[41], 96, 96,  5, 1};
	game.character.back.hurt    = (AnimatedSprite){&texture[42], 64, 96,  5, 1};
	game.character.back.idle    = (AnimatedSprite){&texture[43], 64, 96,  5, 1};
	game.character.back.walk    = (AnimatedSprite){&texture[44], 64, 96,  6, 1};
	game.character.front.attack = (AnimatedSprite){&texture[45], 96, 96,  5, 0};
	game.character.front.defend = (AnimatedSprite){&texture[46], 64, 96,  5, 1};
	game.character.front.hurt   = (AnimatedSprite){&texture[47], 64, 96,  5, 1};
	game.character.front.idle   = (AnimatedSprite){&texture[48], 64, 96,  5, 1};
	game.character.front.walk   = (AnimatedSprite){&texture[49], 64, 96,  6, 1};
	game.character.left.attack  = (AnimatedSprite){&texture[50], 96, 96,  5, 0};
	game.character.left.defend  = (AnimatedSprite){&texture[51], 64, 96,  5, 1};
	game.character.left.hurt    = (AnimatedSprite){&texture[52], 64, 96,  5, 1};
	game.character.left.idle    = (AnimatedSprite){&texture[53], 64, 96,  5, 1};
	game.character.left.walk    = (AnimatedSprite){&texture[54], 64, 96,  6, 1};
	game.character.right.attack = (AnimatedSprite){&texture[55], 96, 96,  5, 0};
	game.character.right.defend = (AnimatedSprite){&texture[56], 64, 96,  5, 1};
	game.character.right.hurt   = (AnimatedSprite){&texture[57], 64, 96,  5, 1};
	game.character.right.idle   = (AnimatedSprite){&texture[58], 64, 96,  5, 1};
	game.character.right.walk   = (AnimatedSprite){&texture[59], 64, 96,  6, 1};
	game.character.side   = &game.character.front;
	game.character.animation.sprite       = &game.character.front.idle;
	game.character.animation.frame        = 0;
	game.character.animation.x            = 0;
	game.character.animation.y            = 0;
	game.character.animation.is_character = 1;
	time_lastmove = osGetTime();
}

void initCharacter(Character* character, int x, int y, int life) {
	character->keys			= 0;
	character->x				= x;
	character->y				= y;
	character->move_x		= 0;
	character->move_y		= 0;
	character->state			= CHARACTER_IDLE;
	character->life			= life;
	character->next			= NULL;
	character->previous		= NULL;
	character->collision		= NULL;
	character->side			= NULL;
	character->animation.sprite       = NULL;
	character->animation.frame        = 0;
	character->animation.x            = 0;
	character->animation.y            = 0;
	character->animation.is_character = 0;
	character->back.collision         = NULL;
	character->front.collision        = NULL;
	character->left.collision         = NULL;
	character->right.collision        = NULL;
	character->back.action            = NULL;
	character->front.action           = NULL;
	character->left.action            = NULL;
	character->right.action           = NULL;
	character->back.push              = NULL;
	character->front.push             = NULL;
	character->left.push              = NULL;
	character->right.push             = NULL;	
}

void setTexturesCastle() {
	if (memory_size == 0x00800000)
		my2dlibrary.texturePointer = (u32*)(0x80800000 - (640*480*2*2)); // reset texture pointer
	else
		my2dlibrary.texturePointer = (u32*)(0x80400000 - (640*480*2*2)); // reset texture pointer
	my2D_loadTexture(&texture[1], (u32)_64_BorderBottomSegmentRomStart, (u32)_64_BorderBottomSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[2], (u32)_64_BorderCornerBLSegmentRomStart, (u32)_64_BorderCornerBLSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[3], (u32)_64_BorderCornerBRSegmentRomStart, (u32)_64_BorderCornerBRSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[4], (u32)_64_BorderCornerTLSegmentRomStart, (u32)_64_BorderCornerTLSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[5], (u32)_64_BorderCornerTRSegmentRomStart, (u32)_64_BorderCornerTRSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[6], (u32)_64_BorderLeftSegmentRomStart, (u32)_64_BorderLeftSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[7], (u32)_64_BorderRightSegmentRomStart, (u32)_64_BorderRightSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[8], (u32)_64_BorderUpSegmentRomStart, (u32)_64_BorderUpSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[9], (u32)_64_DownStairsLSegmentRomStart, (u32)_64_DownStairsLSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[10], (u32)_64_DownStairsRSegmentRomStart, (u32)_64_DownStairsRSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[11], (u32)_64_Ground1SegmentRomStart, (u32)_64_Ground1SegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[12], (u32)_64_Ground2SegmentRomStart, (u32)_64_Ground2SegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[13], (u32)_64_Ground3SegmentRomStart, (u32)_64_Ground3SegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[14], (u32)_64_UpCornerBLSegmentRomStart, (u32)_64_UpCornerBLSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[15], (u32)_64_UpCornerBRSegmentRomStart, (u32)_64_UpCornerBRSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[16], (u32)_64_UpCornerTlSegmentRomStart, (u32)_64_UpCornerTlSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[17], (u32)_64_UpCornerTRSegmentRomStart, (u32)_64_UpCornerTRSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[18], (u32)_64_UpCrossSegmentRomStart, (u32)_64_UpCrossSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[19], (u32)_64_UpDownSegmentRomStart, (u32)_64_UpDownSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[20], (u32)_64_UpHorizontalSegmentRomStart, (u32)_64_UpHorizontalSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[21], (u32)_64_UpLeftSegmentRomStart, (u32)_64_UpLeftSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[22], (u32)_64_UpRightSegmentRomStart, (u32)_64_UpRightSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[23], (u32)_64_UpStairsL1SegmentRomStart, (u32)_64_UpStairsL1SegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[24], (u32)_64_UpStairsL2SegmentRomStart, (u32)_64_UpStairsL2SegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[25], (u32)_64_UpStairsR1SegmentRomStart, (u32)_64_UpStairsR1SegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[26], (u32)_64_UpStairsR2SegmentRomStart, (u32)_64_UpStairsR2SegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[27], (u32)_64_UpTBottomSegmentRomStart, (u32)_64_UpTBottomSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[28], (u32)_64_UpTLeftSegmentRomStart, (u32)_64_UpTLeftSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[29], (u32)_64_UpTopSegmentRomStart, (u32)_64_UpTopSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[30], (u32)_64_UpTRightSegmentRomStart, (u32)_64_UpTRightSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[31], (u32)_64_UpTTopSegmentRomStart, (u32)_64_UpTTopSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[32], (u32)_64_UpVerticalSegmentRomStart, (u32)_64_UpVerticalSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[33], (u32)_64_WallDownSegmentRomStart, (u32)_64_WallDownSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[34], (u32)_64_WallUpSegmentRomStart, (u32)_64_WallUpSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[35], (u32)_64_WallWaterSegmentRomStart, (u32)_64_WallWaterSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[36], (u32)_64_WaterSegmentRomStart, (u32)_64_WaterSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[37], (u32)_64_UpAloneSegmentRomStart, (u32)_64_UpAloneSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[38], (u32)_64_PillarSegmentRomStart, (u32)_64_PillarSegmentRomEnd, 64, 192);
	my2D_loadTexture(&texture[39], (u32)_64_WallPillarSegmentRomStart, (u32)_64_WallPillarSegmentRomEnd, 64, 256);
	
	my2D_loadTexture(&texture[40], (u32)_64_back_attackSegmentRomStart, (u32)_64_back_attackSegmentRomEnd, 96, 480);
	my2D_loadTexture(&texture[41], (u32)_64_back_defendSegmentRomStart, (u32)_64_back_defendSegmentRomEnd, 96, 480);
	my2D_loadTexture(&texture[42], (u32)_64_back_hurtSegmentRomStart, (u32)_64_back_hurtSegmentRomEnd, 64, 480);
	my2D_loadTexture(&texture[43], (u32)_64_back_idleSegmentRomStart, (u32)_64_back_idleSegmentRomEnd, 64, 480);
	my2D_loadTexture(&texture[44], (u32)_64_back_walkSegmentRomStart, (u32)_64_back_walkSegmentRomEnd, 64, 576);
	my2D_loadTexture(&texture[45], (u32)_64_front_attackSegmentRomStart, (u32)_64_front_attackSegmentRomEnd, 96, 480);
	my2D_loadTexture(&texture[46], (u32)_64_front_defendSegmentRomStart, (u32)_64_front_defendSegmentRomEnd, 64, 480);
	my2D_loadTexture(&texture[47], (u32)_64_front_hurtSegmentRomStart, (u32)_64_front_hurtSegmentRomEnd, 64, 480);
	my2D_loadTexture(&texture[48], (u32)_64_front_idleSegmentRomStart, (u32)_64_front_idleSegmentRomEnd, 64, 480);
	my2D_loadTexture(&texture[49], (u32)_64_front_walkSegmentRomStart, (u32)_64_front_walkSegmentRomEnd, 64, 576);
	my2D_loadTexture(&texture[50], (u32)_64_left_attackSegmentRomStart, (u32)_64_left_attackSegmentRomEnd, 96, 480);
	my2D_loadTexture(&texture[51], (u32)_64_left_defendSegmentRomStart, (u32)_64_left_defendSegmentRomEnd, 64, 480);
	my2D_loadTexture(&texture[52], (u32)_64_left_hurtSegmentRomStart, (u32)_64_left_hurtSegmentRomEnd, 64, 480);
	my2D_loadTexture(&texture[53], (u32)_64_left_idleSegmentRomStart, (u32)_64_left_idleSegmentRomEnd, 64, 480);
	my2D_loadTexture(&texture[54], (u32)_64_left_walkSegmentRomStart, (u32)_64_left_walkSegmentRomEnd, 64, 576);
	my2D_loadTexture(&texture[55], (u32)_64_right_attackSegmentRomStart, (u32)_64_right_attackSegmentRomEnd, 96, 480);
	my2D_loadTexture(&texture[56], (u32)_64_right_defendSegmentRomStart, (u32)_64_right_defendSegmentRomEnd, 64, 480);
	my2D_loadTexture(&texture[57], (u32)_64_right_hurtSegmentRomStart, (u32)_64_right_hurtSegmentRomEnd, 64, 480);
	my2D_loadTexture(&texture[58], (u32)_64_right_idleSegmentRomStart, (u32)_64_right_idleSegmentRomEnd, 64, 480);
	my2D_loadTexture(&texture[59], (u32)_64_right_walkSegmentRomStart, (u32)_64_right_walkSegmentRomEnd, 64, 576);
	
	my2D_loadTexture(&texture[60], (u32)_ObjectBigDoorLeftSegmentRomStart, (u32)_ObjectBigDoorLeftSegmentRomEnd, 128, 166);
	my2D_loadTexture(&texture[94], (u32)_ObjectBigDoorRightSegmentRomStart, (u32)_ObjectBigDoorRightSegmentRomEnd, 128, 166);
	my2D_loadTexture(&texture[61], (u32)_ObjectButtonSegmentRomStart, (u32)_ObjectButtonSegmentRomEnd, 128, 64);
	my2D_loadTexture(&texture[62], (u32)_ObjectDoorSegmentRomStart, (u32)_ObjectDoorSegmentRomEnd, 128, 172);
	my2D_loadTexture(&texture[63], (u32)_ObjectKeySegmentRomStart, (u32)_ObjectKeySegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[64], (u32)_ObjectLeverSegmentRomStart, (u32)_ObjectLeverSegmentRomEnd, 256, 64);
	my2D_loadTexture(&texture[65], (u32)_ObjectTrapSegmentRomStart, (u32)_ObjectTrapSegmentRomEnd, 128, 66);
	
	my2D_loadTexture(&texture[66], (u32)_SceneryBarrel01SegmentRomStart, (u32)_SceneryBarrel01SegmentRomEnd, 64, 82);
	my2D_loadTexture(&texture[67], (u32)_SceneryBarrel02SegmentRomStart, (u32)_SceneryBarrel02SegmentRomEnd, 64, 82);
	my2D_loadTexture(&texture[68], (u32)_SceneryBarrel03SegmentRomStart, (u32)_SceneryBarrel03SegmentRomEnd, 64, 82);
	my2D_loadTexture(&texture[69], (u32)_SceneryCandelierSegmentRomStart, (u32)_SceneryCandelierSegmentRomEnd, 192, 96);
	my2D_loadTexture(&texture[70], (u32)_SceneryChairSegmentRomStart, (u32)_SceneryChairSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[71], (u32)_SceneryFlag01SegmentRomStart, (u32)_SceneryFlag01SegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[72], (u32)_SceneryFlag02SegmentRomStart, (u32)_SceneryFlag02SegmentRomEnd, 64, 128);
	my2D_loadTexture(&texture[73], (u32)_SceneryPot01SegmentRomStart, (u32)_SceneryPot01SegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[74], (u32)_SceneryPot02SegmentRomStart, (u32)_SceneryPot02SegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[75], (u32)_SceneryPot03SegmentRomStart, (u32)_SceneryPot03SegmentRomEnd, 64, 96);
	my2D_loadTexture(&texture[76], (u32)_SceneryTableSegmentRomStart, (u32)_SceneryTableSegmentRomEnd, 96, 128);
	my2D_loadTexture(&texture[77], (u32)_SceneryTorchSegmentRomStart, (u32)_SceneryTorchSegmentRomEnd, 256, 64);
	my2D_loadTexture(&texture[78], (u32)_SceneryWallTorchSegmentRomStart, (u32)_SceneryWallTorchSegmentRomEnd, 256, 128);
	my2D_loadTexture(&texture[79], (u32)_SceneryWindow01SegmentRomStart, (u32)_SceneryWindow01SegmentRomEnd, 64, 96);
	my2D_loadTexture(&texture[80], (u32)_SceneryWindow02SegmentRomStart, (u32)_SceneryWindow02SegmentRomEnd, 64, 96);
	my2D_loadTexture(&texture[81], (u32)_SceneryWindow03SegmentRomStart, (u32)_SceneryWindow03SegmentRomEnd, 96, 96);
	my2D_loadTexture(&texture[95], (u32)_SceneryWaterSegmentRomStart, (u32)_SceneryWaterSegmentRomEnd, 256, 64);
	
	my2D_loadTexture(&texture[96], (u32)_SceneryBook01LeftSegmentRomStart, (u32)_SceneryBook01LeftSegmentRomEnd, 64, 192);
	my2D_loadTexture(&texture[97], (u32)_SceneryBook01RightSegmentRomStart, (u32)_SceneryBook01RightSegmentRomEnd, 64, 192);
	my2D_loadTexture(&texture[98], (u32)_SceneryBook02LeftSegmentRomStart, (u32)_SceneryBook02LeftSegmentRomEnd, 64, 192);
	my2D_loadTexture(&texture[99], (u32)_SceneryBook02MiddleSegmentRomStart, (u32)_SceneryBook02MiddleSegmentRomEnd, 64, 192);
	my2D_loadTexture(&texture[100], (u32)_SceneryBook02RightSegmentRomStart, (u32)_SceneryBook02RightSegmentRomEnd, 64, 192);
	my2D_loadTexture(&texture[101], (u32)_SceneryCarpetTLSegmentRomStart, (u32)_SceneryCarpetTLSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[102], (u32)_SceneryCarpetTSegmentRomStart, (u32)_SceneryCarpetTSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[103], (u32)_SceneryCarpetTRSegmentRomStart, (u32)_SceneryCarpetTRSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[104], (u32)_SceneryCarpetLSegmentRomStart, (u32)_SceneryCarpetLSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[105], (u32)_SceneryCarpetCSegmentRomStart, (u32)_SceneryCarpetCSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[106], (u32)_SceneryCarpetRSegmentRomStart, (u32)_SceneryCarpetRSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[107], (u32)_SceneryCarpetBLSegmentRomStart, (u32)_SceneryCarpetBLSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[108], (u32)_SceneryCarpetBSegmentRomStart, (u32)_SceneryCarpetBSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[109], (u32)_SceneryCarpetBRSegmentRomStart, (u32)_SceneryCarpetBRSegmentRomEnd, 64, 64);
	
	my2D_loadTexture(&texture[82], (u32)_RatBackDeadSegmentRomStart, (u32)_RatBackDeadSegmentRomEnd, 48, 320);
	my2D_loadTexture(&texture[83], (u32)_RatBackIdleSegmentRomStart, (u32)_RatBackIdleSegmentRomEnd, 48, 320);
	my2D_loadTexture(&texture[84], (u32)_RatBackWalkSegmentRomStart, (u32)_RatBackWalkSegmentRomEnd, 48, 320);
	my2D_loadTexture(&texture[85], (u32)_RatFrontDeadSegmentRomStart, (u32)_RatFrontDeadSegmentRomEnd, 64, 320);
	my2D_loadTexture(&texture[86], (u32)_RatFrontIdleSegmentRomStart, (u32)_RatFrontIdleSegmentRomEnd, 64, 320);
	my2D_loadTexture(&texture[87], (u32)_RatFrontWalkSegmentRomStart, (u32)_RatFrontWalkSegmentRomEnd, 64, 320);
	my2D_loadTexture(&texture[88], (u32)_RatRightDeadSegmentRomStart, (u32)_RatRightDeadSegmentRomEnd, 96, 190);
	my2D_loadTexture(&texture[89], (u32)_RatRightIdleSegmentRomStart, (u32)_RatRightIdleSegmentRomEnd, 96, 190);
	my2D_loadTexture(&texture[90], (u32)_RatRightWalkSegmentRomStart, (u32)_RatRightWalkSegmentRomEnd, 96, 190);
	my2D_loadTexture(&texture[91], (u32)_RatLeftDeadSegmentRomStart, (u32)_RatLeftDeadSegmentRomEnd, 96, 190);
	my2D_loadTexture(&texture[92], (u32)_RatLeftIdleSegmentRomStart, (u32)_RatLeftIdleSegmentRomEnd, 96, 190);
	my2D_loadTexture(&texture[93], (u32)_RatLeftWalkSegmentRomStart, (u32)_RatLeftWalkSegmentRomEnd, 96, 190);
}

void setTiles() {
	map.tiles[1] = (Tile){collisionWall, &texture[1], NULL, NULL, TILE_FOREGROUND_NEVER};
	map.tiles[2] = (Tile){collisionWall, &texture[2], NULL, NULL, TILE_FOREGROUND_NEVER};
	map.tiles[3] = (Tile){collisionWall, &texture[3], NULL, NULL, TILE_FOREGROUND_NEVER};
	map.tiles[4] = (Tile){collisionWall, &texture[4], NULL, NULL, TILE_FOREGROUND_NEVER};
	map.tiles[5] = (Tile){collisionWall, &texture[5], NULL, NULL, TILE_FOREGROUND_NEVER};
	map.tiles[6] = (Tile){collisionWall, &texture[6], NULL, NULL, TILE_FOREGROUND_NEVER};
	map.tiles[7] = (Tile){collisionWall, &texture[7], NULL, NULL, TILE_FOREGROUND_NEVER};
	map.tiles[8] = (Tile){collisionWall, &texture[8], NULL, NULL, TILE_FOREGROUND_NEVER};
	map.tiles[9] = (Tile){collisionNone, &texture[9], NULL, NULL, TILE_FOREGROUND_NEVER};
	map.tiles[10] = (Tile){collisionNone, &texture[10], NULL, NULL, TILE_FOREGROUND_NEVER};
	map.tiles[11] = (Tile){collisionNone, &texture[11], NULL, NULL, TILE_FOREGROUND_NEVER};
	map.tiles[12] = (Tile){collisionNone, &texture[12], NULL, NULL, TILE_FOREGROUND_NEVER};
	map.tiles[13] = (Tile){collisionNone, &texture[13], NULL, NULL, TILE_FOREGROUND_NEVER};
	map.tiles[14] = (Tile){collisionWall, &texture[14], NULL, NULL, TILE_FOREGROUND_NEVER};
	map.tiles[15] = (Tile){collisionWall, &texture[15], NULL, NULL, TILE_FOREGROUND_NEVER};
	map.tiles[16] = (Tile){collisionWall, &texture[16], NULL, NULL, TILE_FOREGROUND_NEVER};
	map.tiles[17] = (Tile){collisionWall, &texture[17], NULL, NULL, TILE_FOREGROUND_NEVER};
	map.tiles[18] = (Tile){collisionWall, &texture[18], NULL, NULL, TILE_FOREGROUND_NEVER};
	map.tiles[19] = (Tile){collisionWall, &texture[19], NULL, NULL, TILE_FOREGROUND_NEVER};
	map.tiles[20] = (Tile){collisionWall, &texture[20], NULL, NULL, TILE_FOREGROUND_NEVER};
	map.tiles[21] = (Tile){collisionWall, &texture[21], NULL, NULL, TILE_FOREGROUND_NEVER};
	map.tiles[22] = (Tile){collisionWall, &texture[22], NULL, NULL, TILE_FOREGROUND_NEVER};
	map.tiles[23] = (Tile){collisionStairs, &texture[23], NULL, NULL, TILE_FOREGROUND_NEVER};
	map.tiles[24] = (Tile){collisionNone, &texture[24], NULL, NULL, TILE_FOREGROUND_NEVER};
	map.tiles[25] = (Tile){collisionNone, &texture[25], NULL, NULL, TILE_FOREGROUND_NEVER};
	map.tiles[26] = (Tile){collisionNone, &texture[26], NULL, NULL, TILE_FOREGROUND_NEVER};
	map.tiles[27] = (Tile){collisionWall, &texture[27], NULL, NULL, TILE_FOREGROUND_NEVER};
	map.tiles[28] = (Tile){collisionWall, &texture[28], NULL, NULL, TILE_FOREGROUND_NEVER};
	map.tiles[29] = (Tile){collisionWall, &texture[29], NULL, NULL, TILE_FOREGROUND_NEVER};
	map.tiles[30] = (Tile){collisionWall, &texture[30], NULL, NULL, TILE_FOREGROUND_NEVER};
	map.tiles[31] = (Tile){collisionWall, &texture[31], NULL, NULL, TILE_FOREGROUND_NEVER};
	map.tiles[32] = (Tile){collisionWall, &texture[32], NULL, NULL, TILE_FOREGROUND_NEVER};
	map.tiles[33] = (Tile){collisionWall, &texture[33], NULL, NULL, TILE_FOREGROUND_NEVER};
	map.tiles[34] = (Tile){collisionWall, &texture[34], NULL, NULL, TILE_FOREGROUND_NEVER};
	map.tiles[35] = (Tile){collisionWall, &texture[35], NULL, NULL, TILE_FOREGROUND_NEVER};
	map.tiles[36] = (Tile){collisionWall, &texture[36], NULL, NULL, TILE_FOREGROUND_NEVER};
	map.tiles[37] = (Tile){collisionWall, &texture[37], NULL, NULL, TILE_FOREGROUND_NEVER};
	map.tiles[38] = (Tile){collisionPillar, &texture[38], NULL, NULL, 20};
	map.tiles[39] = (Tile){collisionWallPillar, &texture[39], NULL, NULL, TILE_FOREGROUND_NEVER};
	
	map.tiles[40] = (Tile){collisionBigDoor, NULL, &animations[3], NULL, 0};
	map.tiles[62] = (Tile){collisionBigDoor, NULL, &animations[8], NULL, 0};
	map.tiles[41] = (Tile){collisionButton, NULL, &animations[4], NULL, TILE_FOREGROUND_NEVER};
	map.tiles[42] = (Tile){collisionDoor, NULL, &animations[5], NULL, 50}; // foreground = 50 is an arbitrary value which shoud be good enought as long as it's lower that the tile height and above 1 (50 is a security just in case...)
	map.tiles[43] = (Tile){collisionKey, &texture[63], NULL, NULL, TILE_FOREGROUND_NEVER};
	map.tiles[44] = (Tile){collisionLever, NULL, &animations[6], NULL, 40}; // lever
	map.tiles[45] = (Tile){collisionTrap, NULL, &animations[7], NULL, TILE_FOREGROUND_NEVER}; // trap
	/*map.tiles[40] = (Tile){collisionWall, &texture[60], NULL, NULL};
	map.tiles[41] = (Tile){collisionWall, &texture[61], NULL, NULL};
	map.tiles[42] = (Tile){collisionWall, &texture[62], NULL, NULL};
	map.tiles[43] = (Tile){collisionWall, &texture[63], NULL, NULL};
	map.tiles[44] = (Tile){collisionWall, &texture[64], NULL, NULL};
	map.tiles[45] = (Tile){collisionWall, &texture[65], NULL, NULL};*/
	
	map.tiles[46] = (Tile){collisionBarrel, &texture[66], NULL, NULL, 46}; // barrel
	map.tiles[47] = (Tile){collisionBarrel, &texture[67], NULL, NULL, 46};
	map.tiles[48] = (Tile){collisionBarrel, &texture[68], NULL, NULL, 46};
	map.tiles[49] = (Tile){collisionCandelier, NULL, &animations[2], NULL, 38}; // chandelier
	//map.tiles[49] = (Tile){collisionWall, &texture[69], NULL, NULL};
	map.tiles[50] = (Tile){collisionWall, &texture[70], NULL, NULL, 45};
	map.tiles[51] = (Tile){collisionWall, &texture[71], NULL, NULL, TILE_FOREGROUND_NEVER};
	map.tiles[52] = (Tile){collisionWall, &texture[72], NULL, NULL, TILE_FOREGROUND_NEVER};
	map.tiles[53] = (Tile){collisionWall, &texture[73], NULL, NULL, 55};
	map.tiles[54] = (Tile){collisionWall, &texture[74], NULL, NULL, 55};
	map.tiles[55] = (Tile){collisionWall, &texture[75], NULL, NULL, 55};
	map.tiles[56] = (Tile){collisionWall, &texture[76], NULL, NULL, 49};
	
	map.tiles[57] = (Tile){collisionWall, NULL, &animations[0], NULL, TILE_FOREGROUND_NEVER}; // torch
	//map.tiles[57] = (Tile){collisionWall, &texture[77], NULL, NULL};
	map.tiles[58] = (Tile){collisionWall, NULL, &animations[1], NULL, TILE_FOREGROUND_NEVER}; // walltorch
	//map.tiles[58] = (Tile){collisionWall, &texture[78], NULL, NULL};
	map.tiles[59] = (Tile){collisionWall, &texture[79], NULL, NULL, TILE_FOREGROUND_NEVER};
	map.tiles[60] = (Tile){collisionWall, &texture[80], NULL, NULL, TILE_FOREGROUND_NEVER};
	map.tiles[61] = (Tile){collisionWall, &texture[81], NULL, NULL, TILE_FOREGROUND_NEVER};
	
	animations[0] = (Animation){&animated_sprites[0], 0, 0, 0, ANIMATION_STATUS_RUNNING, 0, 0, 0};
	animations[1] = (Animation){&animated_sprites[1], 0, 0, 0, ANIMATION_STATUS_RUNNING, 0, 0, 0};
	animations[2] = (Animation){&animated_sprites[2], 0, 0, 0, ANIMATION_STATUS_RUNNING, 0, 0, 0};
	animations[3] = (Animation){&animated_sprites[3], 0, 0, 0, ANIMATION_STATUS_RUNNING, 0, 0, 0};
	animations[4] = (Animation){&animated_sprites[4], 0, 0, 0, ANIMATION_STATUS_RUNNING, 0, 0, 0};
	animations[5] = (Animation){&animated_sprites[5], 0, 0, 0, ANIMATION_STATUS_RUNNING, 0, 0, 0};
	animations[6] = (Animation){&animated_sprites[6], 0, 0, 0, ANIMATION_STATUS_RUNNING, 0, 0, 0};
	animations[7] = (Animation){&animated_sprites[7], 0, 0, 0, ANIMATION_STATUS_RUNNING, 0, 0, 0};
	animations[8] = (Animation){&animated_sprites[8], 0, 0, 0, ANIMATION_STATUS_RUNNING, 0, 0, 0};
	animations[9] = (Animation){&animated_sprites[9], 0, 0, 0, ANIMATION_STATUS_RUNNING, 0, 0, 0};
	
	animated_sprites[0] = (AnimatedSprite){&texture[77], 64, 64,  4, 1};
	animated_sprites[1] = (AnimatedSprite){&texture[78], 64, 128,  4, 1};
	animated_sprites[2] = (AnimatedSprite){&texture[69], 64, 96,  3, 1};
	
	animated_sprites[3] = (AnimatedSprite){&texture[60], 64, 166,  2, 0}; // big door left
	animated_sprites[8] = (AnimatedSprite){&texture[94], 64, 166,  2, 0}; // big door right
	animated_sprites[4] = (AnimatedSprite){&texture[61], 64, 64,  2, 0};
	animated_sprites[5] = (AnimatedSprite){&texture[62], 64, 172,  2, 0};
	animated_sprites[6] = (AnimatedSprite){&texture[64], 64, 64,  4, 0}; // lever
	animated_sprites[7] = (AnimatedSprite){&texture[65], 64, 64,  2, 0}; // trap
	animated_sprites[9] = (AnimatedSprite){&texture[95], 64, 64,  4, 0}; // water
}