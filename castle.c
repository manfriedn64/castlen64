#include <nusys.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include "audio.h"
#include "main.h"
#include "castle.h"
#include "intro.h"
#include "menu.h"
#include "2dlibrary.h"
#include "2dmap.h"
#include "assets/music/chance.h"
#include "assets/music/level01.h"


extern musHandle sndHandle[2];

extern objListCnt;
extern Gfx*	gfxListPtr;
extern Gfx*	gfxListStartPtr;

NUContData			contData[NU_CONT_MAXCONTROLLERS];
u8 					contPattern;

char konami[10] = "UUDDLRLRBA";
int konami_pos = 0;

Map						map;
MapRow*					searched_row;

Character				characters[10];

u64 time_to_fetch_screen;

u16* HighFrameBuf32[2] = {
	(u16*)CFB32_HIGH_ADDR0,
	(u16*)CFB32_HIGH_ADDR1
};
u16* LowFrameBuf32[2] = {
	(u16*)CFB_LOW_ADDR0,
	(u16*)CFB_LOW_ADDR1
};

u16* HighFrameBuf[2] = {
	(u16*)CFB_HIGH_ADDR0,
	(u16*)CFB_HIGH_ADDR1
};
u16* LowFrameBuf[2] = {
	(u16*)CFB_LOW_ADDR0,
	(u16*)CFB_LOW_ADDR1
};

void checkControls() {
	if (game.level->loadControls &&  game.state == GAMESTATE_NORMAL)
		game.level->loadControls();
	return;
}

void setFrameBuffer32b() {
	my2D_init(640, 480, (u32*)(memory_size - (640*480*4*2)));
	my2dlibrary.width = 640;
	if (memory_size == 0x00800000) {
		nuGfxSetCfb(HighFrameBuf32, 2);
		nuGfxSetZBuffer((u16*)(HIGH32_ZBUFFER_ADDR));
	} else {
		nuGfxSetCfb(LowFrameBuf32, 2);
		nuGfxSetZBuffer((u16*)(LOW32_ZBUFFER_ADDR));
	}
	my2dlibrary.colors = G_IM_SIZ_32b;
}

void setFrameBuffer16b() {
	my2D_init(640, 480, (u32*)(memory_size - (640*480*2*2)));
	if (memory_size == 0x00800000) {
		nuGfxSetCfb(HighFrameBuf, 2);
		nuGfxSetZBuffer((u16*)(HIGH_ZBUFFER_ADDR));
	} else {
		nuGfxSetCfb(LowFrameBuf, 2);
		nuGfxSetZBuffer((u16*)(LOW_ZBUFFER_ADDR));
	}
	my2dlibrary.width = 640;
	my2dlibrary.colors = G_IM_SIZ_16b;
}

Bool externalDevicesInit(int controller_number) {
	s32 ret;
	ret = nuContRmbCheck(controller_number);
	if (!ret) {
		game.rumble = True;
		nuContRmbModeSet(controller_number, NU_CONT_RMB_MODE_ENABLE);
		nuContRmbStart(controller_number, 128, 10);
		return True;
	}
	return False;
}


void setup_640() {
	current_music = NULL;
	if (osTvType == OS_TV_PAL ) {
		osViSetMode(&osViModeTable[OS_VI_PAL_HAN1]);
		nuGfxDisplayOff();
		osViSetYScale(0.833);
		nuPreNMIFuncSet((NUScPreNMIFunc)callbackPrenmi);
	}
	else if (osTvType == OS_TV_MPAL )
		osViSetMode(&osViModeTable[OS_VI_MPAL_HAN1]); 
	else
		osViSetMode(&osViModeTable[OS_VI_NTSC_HAN1]); 
	nuGfxDisplayOff();
	osViSetSpecialFeatures(OS_VI_GAMMA_OFF);
	levels[0] = (Level){"Expansion Pak Warning", initExpansionPak, NULL, drawExpansionPak, NULL};
	levels[1] = (Level){"Intro", initIntro, updateIntro, drawIntro, NULL};
	levels[2] = (Level){"Main Menu", initMainMenu, updateMenu, drawMainMenu, NULL};
	levels[3] = (Level){"Level One", initLevel_00, updateLevel, drawLevel, NULL};

	game.state = GAMESTATE_NORMAL;
	game.speed = 4;
	setFrameBuffer16b();
	if (memory_size == 0x00800000) {
		game.scale = 1;
		game.godmod = False;
		game.lifes = 1;
		game.level = &levels[1];
	}
	else {
		setFrameBuffer16b();
		game.level = &levels[0];
	}
	my2dlibrary.debug = False;
	game.level->loadLevel();
}

void initCharacter(int x, int y, int life) {
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

void level00_64() {
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

void level00_16() {
	if (memory_size == 0x00800000)
		my2dlibrary.texturePointer = (u32*)(0x80800000 - (640*480*2*2)); // reset texture pointer
	else
		my2dlibrary.texturePointer = (u32*)(0x80400000 - (640*480*2*2)); // reset texture pointer
	my2D_loadTexture(&texture[1], (u32)_16_BorderBottomSegmentRomStart, (u32)_16_BorderBottomSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[2], (u32)_16_BorderCornerBLSegmentRomStart, (u32)_16_BorderCornerBLSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[3], (u32)_16_BorderCornerBRSegmentRomStart, (u32)_16_BorderCornerBRSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[4], (u32)_16_BorderCornerTLSegmentRomStart, (u32)_16_BorderCornerTLSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[5], (u32)_16_BorderCornerTRSegmentRomStart, (u32)_16_BorderCornerTRSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[6], (u32)_16_BorderLeftSegmentRomStart, (u32)_16_BorderLeftSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[7], (u32)_16_BorderRightSegmentRomStart, (u32)_16_BorderRightSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[8], (u32)_16_BorderUpSegmentRomStart, (u32)_16_BorderUpSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[9], (u32)_16_DownStairsLSegmentRomStart, (u32)_16_DownStairsLSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[10], (u32)_16_DownStairsRSegmentRomStart, (u32)_16_DownStairsRSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[11], (u32)_16_Ground1SegmentRomStart, (u32)_16_Ground1SegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[12], (u32)_16_Ground2SegmentRomStart, (u32)_16_Ground2SegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[13], (u32)_16_Ground3SegmentRomStart, (u32)_16_Ground3SegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[14], (u32)_16_UpCornerBLSegmentRomStart, (u32)_16_UpCornerBLSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[15], (u32)_16_UpCornerBRSegmentRomStart, (u32)_16_UpCornerBRSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[16], (u32)_16_UpCornerTlSegmentRomStart, (u32)_16_UpCornerTlSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[17], (u32)_16_UpCornerTRSegmentRomStart, (u32)_16_UpCornerTRSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[18], (u32)_16_UpCrossSegmentRomStart, (u32)_16_UpCrossSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[19], (u32)_16_UpDownSegmentRomStart, (u32)_16_UpDownSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[20], (u32)_16_UpHorizontalSegmentRomStart, (u32)_16_UpHorizontalSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[21], (u32)_16_UpLeftSegmentRomStart, (u32)_16_UpLeftSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[22], (u32)_16_UpRightSegmentRomStart, (u32)_16_UpRightSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[23], (u32)_16_UpStairsL1SegmentRomStart, (u32)_16_UpStairsL1SegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[24], (u32)_16_UpStairsL2SegmentRomStart, (u32)_16_UpStairsL2SegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[25], (u32)_16_UpStairsR1SegmentRomStart, (u32)_16_UpStairsR1SegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[26], (u32)_16_UpStairsR2SegmentRomStart, (u32)_16_UpStairsR2SegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[27], (u32)_16_UpTBottomSegmentRomStart, (u32)_16_UpTBottomSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[28], (u32)_16_UpTLeftSegmentRomStart, (u32)_16_UpTLeftSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[29], (u32)_16_UpTopSegmentRomStart, (u32)_16_UpTopSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[30], (u32)_16_UpTRightSegmentRomStart, (u32)_16_UpTRightSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[31], (u32)_16_UpTTopSegmentRomStart, (u32)_16_UpTTopSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[32], (u32)_16_UpVerticalSegmentRomStart, (u32)_16_UpVerticalSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[33], (u32)_16_WallDownSegmentRomStart, (u32)_16_WallDownSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[34], (u32)_16_WallUpSegmentRomStart, (u32)_16_WallUpSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[35], (u32)_16_WallWaterSegmentRomStart, (u32)_16_WallWaterSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[36], (u32)_16_WaterSegmentRomStart, (u32)_16_WaterSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[37], (u32)_16_UpAloneSegmentRomStart, (u32)_16_UpAloneSegmentRomEnd, 64, 64);
	my2D_loadTexture(&texture[38], (u32)_16_PillarSegmentRomStart, (u32)_16_PillarSegmentRomEnd, 64, 192);
	my2D_loadTexture(&texture[39], (u32)_16_WallPillarSegmentRomStart, (u32)_16_WallPillarSegmentRomEnd, 64, 256);


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

	return;
}

void loadRAMLevel(LevelMap*	map, u32 segmentStart, u32 segmentEnd) {
	u8 width, height;
	u8* pointer8;
	map->pointer			= my2D_loadSegmentToRam(segmentStart, segmentEnd);
	pointer8 = (u8*)map->pointer;
	memcpy(&width, pointer8++, sizeof width);
	memcpy(&height,pointer8++, sizeof height);
	map->width = (int)width;
	map->height = (int)height;
}

void loadMap(Map* map) {
	int column, row;
	Tile* tile;
	u8 value;
	u8* pointer8 = (u8*)game.level->map.pointer;
	pointer8++; // width
	pointer8++; // height
	for (column = 0; column < game.level->map.width; column++)
		appendMapColumn(map, nextMapColumn(map, column));
	for (row = 0; row < game.level->map.height; row++) {
		for (column = 0; column < game.level->map.width; column++) {
			memcpy(&value, pointer8++, sizeof value);
			//if ((int)value == 38 || (int)value == 49 || (int)value == 46 || (int)value == 53 || (int)value == 55) // pillar, candelier, barrel, pot, plant in pot
			//	appendMapRow(findMapColumn(map, column), nextMapRow(map, row, &map->tiles[11]));
			if ((int)value == 57 || (int)value == 51) // torch, small flag
				appendMapRow(findMapColumn(map, column), nextMapRow(map, row, &map->tiles[34]));
			if ((int)value == 58 || (int)value ==  52) // walltorch, big flag
				appendMapRow(findMapColumn(map, column), nextMapRow(map, row, &map->tiles[33]));
			tile = &map->tiles[(int)value];
			appendMapRow(findMapColumn(map, column), nextMapRow(map, row, tile));
		}
	}
	//sortMap(map);
}

void initLevel_00() {
	Tile* tile;
	MapColumn* column;
	MapRow* button;
	MapRow* key;
	MapRow* door;
	MapRow* bigdoorleft;
	MapRow* bigdoorright;
	MapRow* trap1;
	MapRow* trap2;
	MapRow* lever;
	MapRow* barrel;
	MapRow* levelcompleted;
	
	externalDevicesInit(0);
	
	level00_64();
	loadRAMLevel(&game.level->map, (u32)_Level01SegmentRomStart, (u32)_Level01SegmentRomEnd);
	initMap(&map, game.level->map.width, game.level->map.width * game.level->map.height * 2, 300, 64, 64);

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
	
	loadMap(&map);
	
	/*appendMapRow(findMapColumn(&map, 2), nextMapRow(&map, 2, &map.tiles[58]));
	appendMapRow(findMapColumn(&map, 4), nextMapRow(&map, 3, &map.tiles[49]));
	appendMapRow(findMapColumn(&map, 5), nextMapRow(&map, 1, &map.tiles[57]));*/
	
	key = nextMapRow(&map, 8, &map.tiles[43]);
	appendMapRow(findMapColumn(&map, 59), key);
	
	button = nextMapRow(&map, 4, &map.tiles[41]);
	appendMapRow(findMapColumn(&map, 1), button);

	door = nextMapRow(&map, 56, &map.tiles[42]);
	appendMapRow(findMapColumn(&map, 34), door);
	button->related_row = door;
	
	bigdoorleft = nextMapRow(&map, 2, &map.tiles[40]);
	appendMapRow(findMapColumn(&map, 35), bigdoorleft);
	bigdoorright = nextMapRow(&map, 2, &map.tiles[62]);
	appendMapRow(findMapColumn(&map, 36), bigdoorright);
	bigdoorleft->related_row = bigdoorright;
	bigdoorright->related_row = bigdoorleft;
	
	trap1 = nextMapRow(&map, 10, &map.tiles[45]);
	appendMapRow(findMapColumn(&map, 61), trap1);
	trap2 = nextMapRow(&map, 10, &map.tiles[45]);
	appendMapRow(findMapColumn(&map, 62), trap2);
	
	lever = nextMapRow(&map, 54, &map.tiles[44]);
	appendMapRow(findMapColumn(&map, 55), lever);
	lever->related_row = trap1;
	trap1->related_row = trap2;
	
	
	/*key = nextMapRow(&map, 57, &map.tiles[43]);
	appendMapRow(findMapColumn(&map, 13), key);*/
	
	bigdoorleft = nextMapRow(&map, 60, &map.tiles[40]);
	appendMapRow(findMapColumn(&map, 10), bigdoorleft);
	bigdoorright = nextMapRow(&map, 60, &map.tiles[62]);
	appendMapRow(findMapColumn(&map, 11), bigdoorright);
	bigdoorleft->related_row = bigdoorright;
	bigdoorright->related_row = bigdoorleft;
	
	barrel = nextMapRow(&map, 57, &map.tiles[46]);
	barrel->action = setBarrelToKey;
	appendMapRow(findMapColumn(&map, 13), barrel);
	
	column = findMapColumn(&map, 35);
	levelcompleted = findMapRow(column->first_row, 1);
	column = findMapColumn(&map, 36);
	levelcompleted = findMapRow(column->first_row, 1);
	levelcompleted->over = level01End;
	levelcompleted->over = level01End;
	
	sortMap(&map);

	
	searched_row = key;
	
	// normal start position
	initCharacter(128, 3712, 255);
	
	
	//initCharacter(3264, 3392, 255);
	
	// test end game
	/*initCharacter(2240, 256, 255);
	game.character.keys = 1;*/
	
	game.character.animation.border_x = game.level->map.width * map.column_width;
	game.character.animation.border_y = game.level->map.height * map.row_height;
	game.character.next = &characters[0];
	

	characters[0].keys			= 0;
	characters[0].x				= 240;
	characters[0].y				= 284;
	characters[0].move_x		= 0;
	characters[0].move_y		= 0;
	characters[0].state			= CHARACTER_IDLE;
	characters[0].life			= 255;
	characters[0].next			= NULL;
	characters[0].previous		= &game.character;
	characters[0].collision		= testCollisionFeet;
	characters[0].back.dead    = (AnimatedSprite){&texture[82], 48, 64,  5, 1};
	characters[0].back.attack  = (AnimatedSprite){&texture[84], 48, 64,  5, 0};
	characters[0].back.defend  = (AnimatedSprite){&texture[84], 48, 64,  5, 1};
	characters[0].back.hurt    = (AnimatedSprite){&texture[84], 48, 64,  5, 1};
	characters[0].back.idle    = (AnimatedSprite){&texture[83], 48, 64,  5, 1};
	characters[0].back.walk    = (AnimatedSprite){&texture[84], 48, 64,  5, 1};
	characters[0].front.dead   = (AnimatedSprite){&texture[85], 64, 64,  5, 0};
	characters[0].front.attack = (AnimatedSprite){&texture[87], 64, 64,  5, 0};
	characters[0].front.defend = (AnimatedSprite){&texture[87], 64, 64,  5, 1};
	characters[0].front.hurt   = (AnimatedSprite){&texture[87], 64, 64,  5, 1};
	characters[0].front.idle   = (AnimatedSprite){&texture[86], 64, 64,  5, 1};
	characters[0].front.walk   = (AnimatedSprite){&texture[87], 64, 64,  5, 1};
	characters[0].left.dead    = (AnimatedSprite){&texture[88], 96, 38,  5, 0};
	characters[0].left.attack  = (AnimatedSprite){&texture[90], 96, 38,  5, 0};
	characters[0].left.defend  = (AnimatedSprite){&texture[90], 96, 38,  5, 1};
	characters[0].left.hurt    = (AnimatedSprite){&texture[90], 96, 38,  5, 1};
	characters[0].left.idle    = (AnimatedSprite){&texture[89], 96, 38,  5, 1};
	characters[0].left.walk    = (AnimatedSprite){&texture[90], 96, 38,  5, 1};
	characters[0].right.dead   = (AnimatedSprite){&texture[91], 96, 38,  5, 0};
	characters[0].right.attack = (AnimatedSprite){&texture[93], 96, 38,  5, 0};
	characters[0].right.defend = (AnimatedSprite){&texture[93], 96, 38,  5, 1};
	characters[0].right.hurt   = (AnimatedSprite){&texture[93], 96, 38,  5, 1};
	characters[0].right.idle   = (AnimatedSprite){&texture[92], 96, 38,  5, 1};
	characters[0].right.walk   = (AnimatedSprite){&texture[93], 96, 38,  5, 1};
	characters[0].side   = &characters[0].left;
	characters[0].animation.sprite       = &characters[0].left.walk;
	characters[0].animation.frame        = 0;
	characters[0].animation.x            = 0;
	characters[0].animation.y            = 0;
	characters[0].animation.is_character = 0;
	characters[0].back.collision         = NULL;
	characters[0].front.collision        = NULL;
	characters[0].left.collision         = ratCollisionLeft;
	characters[0].right.collision        = ratCollisionRight;
	characters[0].back.action            = NULL;
	characters[0].front.action           = NULL;
	characters[0].left.action            = NULL;
	characters[0].right.action           = NULL;
	characters[0].back.push            = NULL;
	characters[0].front.push           = NULL;
	characters[0].left.push            = NULL;
	characters[0].right.push           = NULL;
	
	my2D_loadTexture(&texture[82], (u32)_RatBackDeadSegmentRomStart, (u32)_RatBackDeadSegmentRomEnd, 48, 320);
	my2D_loadTexture(&texture[83], (u32)_RatBackIdleSegmentRomStart, (u32)_RatBackIdleSegmentRomEnd, 48, 320);
	my2D_loadTexture(&texture[84], (u32)_RatBackWalkSegmentRomStart, (u32)_RatBackWalkSegmentRomEnd, 48, 320);
	my2D_loadTexture(&texture[85], (u32)_RatFrontDeadSegmentRomStart, (u32)_RatFrontDeadSegmentRomEnd, 64, 320);
	my2D_loadTexture(&texture[86], (u32)_RatFrontIdleSegmentRomStart, (u32)_RatFrontIdleSegmentRomEnd, 64, 320);
	my2D_loadTexture(&texture[87], (u32)_RatFrontWalkSegmentRomStart, (u32)_RatFrontWalkSegmentRomEnd, 64, 320);
	my2D_loadTexture(&texture[91], (u32)_RatRightDeadSegmentRomStart, (u32)_RatRightDeadSegmentRomEnd, 96, 190);
	my2D_loadTexture(&texture[92], (u32)_RatRightIdleSegmentRomStart, (u32)_RatRightIdleSegmentRomEnd, 96, 190);
	my2D_loadTexture(&texture[93], (u32)_RatRightWalkSegmentRomStart, (u32)_RatRightWalkSegmentRomEnd, 96, 190);
	my2D_loadTexture(&texture[88], (u32)_RatLeftDeadSegmentRomStart, (u32)_RatLeftDeadSegmentRomEnd, 96, 190);
	my2D_loadTexture(&texture[89], (u32)_RatLeftIdleSegmentRomStart, (u32)_RatLeftIdleSegmentRomEnd, 96, 190);
	my2D_loadTexture(&texture[90], (u32)_RatLeftWalkSegmentRomStart, (u32)_RatLeftWalkSegmentRomEnd, 96, 190);
	
	
	
	loadAudio((u32)_MusicLevel01PtrSegmentRomStart, (u32)_MusicLevel01PtrSegmentRomEnd, (u32)_MusicLevel01SfxSegmentRomStart, (u32)_MusicLevel01SfxSegmentRomEnd, _MusicLevel01WbkSegmentRomStart);
	
	music_tracks[0] = (MusicTrack){FX_DANGER, 58, 0, &music_tracks[1]};
	music_tracks[1] = (MusicTrack){FX_SUPERNAT, 52, 0, &music_tracks[0]};
	music_tracks[0].next_track = &music_tracks[1];
	music_tracks[1].next_track = &music_tracks[0];
	current_music = &music_tracks[0];
}


void level01End(MapRow* row) {
	// back to start menu ?
	stopMusic();
	game.level = &levels[2];
	game.state = GAMESTATE_LOADING;
}

void setBarrelToKey(MapRow* row) {
	row->tile = &map.tiles[43];
}

void drawScreen() {
	if (game.state == GAMESTATE_LOADING) {
		game.level->loadLevel();
		game.state = GAMESTATE_NORMAL;
	}
	gDPPipeSync(gfxListPtr++);
	
	if (game.level->drawScreen && game.state == GAMESTATE_NORMAL)
		game.level->drawScreen();
}

void drawLevel() {
	int i, x, y, max_y;
	float scale;
	Character* tmp_character;
	Coordinates coordinates;

	checkMusic(current_music);
	
	// draw the map
	drawMap(&map, &game.character);
	if (game.character.animation.status == ANIMATION_STATUS_ENDED && game.character.state == CHARACTER_ATTACK)
		game.character.state = CHARACTER_IDLE;
		
	/*game.character.animation.x = game.character.x - game.character.animation.sprite->width / 2;
	game.character.animation.y = game.character.y - game.character.animation.sprite->height / 2;
	my2D_animationDraw(&game.character.animation);*/
	/*characters[0].animation.x = characters[0].x - characters[0].animation.sprite->width / 2;
	characters[0].animation.y = characters[0].y - characters[0].animation.sprite->height / 2;
	my2D_animationDraw(&characters[0].animation);*/

	if (OS_CYCLES_TO_USEC(osGetTime() - time_lastmove) / (1000000 / 15) > 1) {
		tmp_character = &game.character;
		while (tmp_character->previous)
			tmp_character = tmp_character->previous;
		while (tmp_character) {
			my2D_animationNextFrame(&tmp_character->animation);
			tmp_character = tmp_character->next;
		}
		
		// list all animation to update one by one to avoid a loop on map.tiles with a if statement
		my2D_animationNextFrame(map.tiles[49].animation);
		my2D_animationNextFrame(map.tiles[57].animation);
		my2D_animationNextFrame(map.tiles[58].animation);
		
		time_lastmove = osGetTime();
	}
		
	drawDebug();
}

void checkKonamiCode() {
	char current = NULL;
	if (contData[0].trigger & D_JPAD || contData[0].stick_y < -32)
		current = 'D';
	if (contData[0].trigger & U_JPAD || contData[0].stick_y > 32)
		current = 'U';
	if (contData[0].trigger & R_JPAD || contData[0].stick_x > 32)
		current = 'R';
	if (contData[0].trigger & L_JPAD || contData[0].stick_x < -32)
		current = 'L';
	if (contData[0].trigger & A_BUTTON)
		current = 'A';
	if (contData[0].trigger & B_BUTTON)
		current = 'B';
	if (current == konami[konami_pos])
		konami_pos++;
	else
		if (current != NULL)
			konami_pos = 0;
	if (konami_pos > 9) {
		if (game.speed == 4)
			game.speed = 8;
		else
			game.speed = 4;
		konami_pos = 0;
	}
}

void updateLevel() {
	Character* tmp_character;
	int i, original_segment, old_x, old_y, collision;
	float hypothenuse, adjacent, new_x, new_y;
	i=0;
	
	tmp_character = &game.character;
	while (tmp_character->previous)
		tmp_character = tmp_character->previous;
	
	nuContDataGetExAll(contData);
	
	checkKonamiCode();
	
	// activate debug
	if(contData[0].trigger & Z_TRIG) 
		if (my2dlibrary.debug) 
			my2dlibrary.debug = False;
		else
			my2dlibrary.debug = True;
	
	game.character.move_x = 0;
	game.character.move_y= 0;
	
	/*if(contData[0].trigger & L_TRIG) {
		level00_16();
		game.scale = 4;
	}
	if(contData[0].trigger & R_TRIG) {
		level00_64();
		game.scale = 1;
	}*/
	
	if (contData[i].button & A_BUTTON || contData[i].trigger & A_BUTTON) {
		game.character.state = CHARACTER_ATTACK;
		if (game.character.animation.sprite != &game.character.side->attack) {
			game.character.animation.sprite = &game.character.side->attack;
			game.character.animation.frame  = 0;
			game.character.animation.status = ANIMATION_STATUS_RUNNING;
			sndHandle[1] = nuAuStlSndPlayerPlay(FX_SWORD_VD);
		}
	}
	else if (contData[i].button & B_BUTTON || contData[i].trigger & B_BUTTON) {
		game.character.state = CHARACTER_DEFEND;
		if (game.character.animation.sprite != &game.character.side->defend) {
			game.character.animation.sprite = &game.character.side->defend;
			game.character.animation.frame  = 0;
			game.character.animation.status = ANIMATION_STATUS_RUNNING;
		}
	}
	else if (contData[i].stick_x > 32 || contData[0].button & R_JPAD) {
		game.character.state = CHARACTER_WALK;
		if (game.character.side != &game.character.right) {
			game.character.side = &game.character.right;
		}
		if (game.character.animation.sprite != &game.character.right.walk) {
			game.character.animation.sprite = &game.character.right.walk;
			game.character.animation.frame  = 0;
			game.character.animation.status = ANIMATION_STATUS_RUNNING;
		}
		game.character.move_x += game.speed;
	}
	else if (contData[i].stick_x < -32 || contData[0].button & L_JPAD) {
		game.character.state = CHARACTER_WALK;
		if (game.character.side != &game.character.left) {
			game.character.side = &game.character.left;
		}
		if (game.character.animation.sprite != &game.character.left.walk) {
			game.character.animation.sprite = &game.character.left.walk;
			game.character.animation.frame  = 0;
			game.character.animation.status = ANIMATION_STATUS_RUNNING;
		}
		game.character.move_x -= game.speed;
	}
	else if (contData[i].stick_y > 32 || contData[0].button & U_JPAD) {
		game.character.state = CHARACTER_WALK;
		if (game.character.side != &game.character.back) {
			game.character.side = &game.character.back;
		}
		if (game.character.animation.sprite != &game.character.back.walk) {
			game.character.animation.sprite = &game.character.back.walk;
			game.character.animation.frame  = 0;
			game.character.animation.status = ANIMATION_STATUS_RUNNING;
		}
		game.character.move_y -= game.speed;
	}
	else if (contData[i].stick_y < -32 || contData[0].button & D_JPAD) {
		game.character.state = CHARACTER_WALK;
		if (game.character.side != &game.character.front) {
			game.character.side = &game.character.front;
		}
		if (game.character.animation.sprite != &game.character.front.walk) {
			game.character.animation.sprite = &game.character.front.walk;
			game.character.animation.frame  = 0;
			game.character.animation.status = ANIMATION_STATUS_RUNNING;
		}
		game.character.move_y += game.speed;
	}
	else if ((!(contData[0].button & U_JPAD || contData[0].button & D_JPAD || contData[0].button & L_JPAD || contData[0].button & R_JPAD) && (contData[i].stick_y < 32 && contData[i].stick_y > -32 && contData[i].stick_x < 32 && contData[i].stick_x > -32)) && game.character.state != CHARACTER_ATTACK) {
		game.character.state = CHARACTER_IDLE;
		if (game.character.animation.sprite != &game.character.side->idle) {
			game.character.animation.sprite = &game.character.side->idle;
			game.character.animation.frame  = 0;
			game.character.animation.status = ANIMATION_STATUS_RUNNING;
		}
	}
	
	// button A pressed => sword is swingged. This could damage enemies or activate items
	if (game.character.state == CHARACTER_ATTACK) {
		if (game.character.side->action)
			collision = game.character.side->action(&map, game.character.x - game.character.animation.sprite->width / 2 , game.character.y - game.character.animation.sprite->height / 2, 1);
	}
	// button B pressed => shield is raised. This can prevent damages but that's not the topic here as it is also use to open big doors with a key
	if (game.character.state == CHARACTER_DEFEND && game.character.keys > 0) { 
		if (game.character.side->push)
			collision = game.character.side->push(&map, game.character.x - game.character.animation.sprite->width / 2 , game.character.y - game.character.animation.sprite->height / 2, 1);
	}
	
	// check if requested move can be made: any obstacle ? Did it hurt ?
	while (tmp_character) {
		// move NPC
		if (tmp_character->animation.is_character == 0) {
			if (&tmp_character->side->walk == tmp_character->animation.sprite) {			
				if (tmp_character->side == &tmp_character->left) {
					tmp_character->move_x = -1;
				}
				if (tmp_character->side == &tmp_character->right) {
					tmp_character->move_x = 1;
				}
				if (tmp_character->side == &tmp_character->back) {
					tmp_character->move_y = -1;
				}
				if (tmp_character->side == &tmp_character->front) {
					tmp_character->move_y = 1;
				}
			}
		}
		
		while (tmp_character->move_x) {
			if (tmp_character->move_x > 0) {
				if (tmp_character->side->collision)
					collision = tmp_character->side->collision(&map, tmp_character->x + 1 - tmp_character->animation.sprite->width / 2 , tmp_character->y - tmp_character->animation.sprite->height / 2, tmp_character->animation.is_character);
				else
					collision = tmp_character->collision(&map, tmp_character->x + 1 - tmp_character->animation.sprite->width / 2 , tmp_character->y - tmp_character->animation.sprite->height / 2, tmp_character->animation.is_character);
				if (!collision) {
					tmp_character->x += 1;
				}
				tmp_character->move_x -= 1;
			}
			else if (tmp_character->move_x < 0) {
				if (tmp_character->side->collision)
					collision = tmp_character->side->collision(&map, tmp_character->x - 1 - tmp_character->animation.sprite->width / 2, tmp_character->y - tmp_character->animation.sprite->height / 2, tmp_character->animation.is_character);
				else
					collision = tmp_character->collision(&map, tmp_character->x - 1 - tmp_character->animation.sprite->width / 2, tmp_character->y - tmp_character->animation.sprite->height / 2, tmp_character->animation.is_character);
				if (!collision) {
					tmp_character->x -= 1;
				}
				tmp_character->move_x += 1;
			}
		}
		
		if (collision == COLLISION_BLOCK && tmp_character->animation.is_character == 0 && &tmp_character->side->walk == tmp_character->animation.sprite) {
			if (tmp_character->side == &tmp_character->left)
				tmp_character->side = &tmp_character->right;
			else if (tmp_character->side == &tmp_character->right)
				tmp_character->side = &tmp_character->left;
			else if (tmp_character->side == &tmp_character->back)
				tmp_character->side = &tmp_character->front;
			else if (tmp_character->side == &tmp_character->front)
				tmp_character->side = &tmp_character->back;
			tmp_character->animation.sprite = &tmp_character->side->walk;
		}
		
		while (tmp_character->move_y) {
			if (tmp_character->move_y > 0) {
				if (tmp_character->side->collision)
					collision = tmp_character->side->collision(&map, tmp_character->x - tmp_character->animation.sprite->width / 2 , tmp_character->y - tmp_character->animation.sprite->height / 2 + 1, tmp_character->animation.is_character);
				else
					collision = tmp_character->collision(&map, tmp_character->x - tmp_character->animation.sprite->width / 2 , tmp_character->y - tmp_character->animation.sprite->height / 2 + 1, tmp_character->animation.is_character);
				if (!collision) {
					tmp_character->y += 1;
				}
				tmp_character->move_y -= 1;
			}
			else if (tmp_character->move_y < 0) {
				if (tmp_character->side->collision)
					collision = tmp_character->side->collision(&map, tmp_character->x - tmp_character->animation.sprite->width / 2, tmp_character->y - tmp_character->animation.sprite->height / 2 - 1, tmp_character->animation.is_character);
				else
					collision = tmp_character->collision(&map, tmp_character->x - tmp_character->animation.sprite->width / 2, tmp_character->y - tmp_character->animation.sprite->height / 2 - 1, tmp_character->animation.is_character);
				if (!collision) {
					tmp_character->y -= 1;
				}
				tmp_character->move_y += 1;
			}
		}
		tmp_character = tmp_character->next;
	}
	
}


int ratCollisionRight(Map* map, int x, int y, int is_character) {
	CollisionBox collision = (CollisionBox){ 29,  20,  83,  30, COLLISION_ACTION_BLOCK};
	return testCollisionBox(map, &collision, x, y, is_character);
}
int ratCollisionLeft(Map* map, int x, int y, int is_character) {
	CollisionBox collision = (CollisionBox){ 13,  20,  67,  30, COLLISION_ACTION_BLOCK};
	return testCollisionBox(map, &collision, x, y, is_character);
}


int testCollisionPushRight(Map* map, int x, int y, int is_character) {
	CollisionBox collision = (CollisionBox){ 64,  39,  95,  84, COLLISION_ACTION_PUSH};
	return testCollisionBox(map, &collision, x, y, is_character);
}
int testCollisionPushLeft(Map* map, int x, int y, int is_character) {
	CollisionBox collision = (CollisionBox){ 1,  39,  32,  84, COLLISION_ACTION_PUSH};
	return testCollisionBox(map, &collision, x, y, is_character);
}
int testCollisionPushBack(Map* map, int x, int y, int is_character) {
	CollisionBox collision = (CollisionBox){ 3,  51,  68,  90, COLLISION_ACTION_PUSH};
	return testCollisionBox(map, &collision, x, y, is_character);
}
int testCollisionPushFront(Map* map, int x, int y, int is_character) {
	CollisionBox collision = (CollisionBox){ 45,  32,  95,  76, COLLISION_ACTION_PUSH};
	return testCollisionBox(map, &collision, x, y, is_character);
}

int testCollisionActionRight(Map* map, int x, int y, int is_character) {
	CollisionBox collision = (CollisionBox){ 64,  39,  95,  84, COLLISION_ACTION_SMASH};
	return testCollisionBox(map, &collision, x, y, is_character);
}
int testCollisionActionLeft(Map* map, int x, int y, int is_character) {
	CollisionBox collision = (CollisionBox){ 1,  39,  32,  84, COLLISION_ACTION_SMASH};
	return testCollisionBox(map, &collision, x, y, is_character);
}
int testCollisionActionFront(Map* map, int x, int y, int is_character) {
	CollisionBox collision = (CollisionBox){ 3,  51,  68,  96, COLLISION_ACTION_SMASH};
	return testCollisionBox(map, &collision, x, y, is_character);
}
int testCollisionActionBack(Map* map, int x, int y, int is_character) {
	CollisionBox collision = (CollisionBox){ 45,  32,  95,  76, COLLISION_ACTION_SMASH};
	return testCollisionBox(map, &collision, x, y, is_character);
}

int testCollisionFeet(Map* map, int x, int y, int is_character) {
	int i, res = 0;
	CollisionBox	collision[1];
	collision[0] = (CollisionBox){ 19,  84,  49,  94, COLLISION_ACTION_BLOCK};
	for (i = 0; i < 1; i++) {
		res = testCollisionBox(map, &collision[i], x, y, is_character);
		if (res)
			return res;
	}
	return 0;
}

void drawDebug()  {	
	MapDraw position = checkMapPosition(&map, game.character.x, game.character.y);
	//int collision = testCollisionFeet(&map, game.character.x - game.character.animation.sprite->width / 2, game.character.y - game.character.animation.sprite->height / 2);
	if (my2dlibrary.debug) {
		nuDebConClear(0);
		nuDebConTextAttr(0, NU_DEB_CON_ATTR_BLINK);
		nuDebConTextColor(0, NU_DEB_CON_TEXT_RED);
		nuDebConTextPos(0, 32,2);
		nuDebConPuts(0, "Running");
		nuDebConTextAttr(0, NU_DEB_CON_ATTR_NORMAL);
		nuDebConTextColor(0, NU_DEB_CON_TEXT_YELLOW);
		
		sprintf(conbuf, "FPS     : %.2f  ", (1.0 / (  (float)OS_CYCLES_TO_USEC(osGetTime()-time_lastframe) / 1000000.0)));
		time_lastframe = osGetTime();
		nuDebConTextPos(0,2,3);
		nuDebConCPuts(0, conbuf);
		/*sprintf(conbuf, "Time    : %.2f  ", (float)OS_CYCLES_TO_USEC(osGetTime()-total_time) / 1000000.0);
		nuDebConTextPos(0,2,4);
		nuDebConCPuts(0, conbuf);
		//if (current_music->started > 0) {
			sprintf(conbuf, "Started : %d  ", (int)OS_CYCLES_TO_USEC(osGetTime()-current_music->started) / 1000000);
			nuDebConTextPos(0,2,5);
			nuDebConCPuts(0, conbuf);
		//}*/
		
		/*
		sprintf(conbuf, "anim x : %d  ", game.character.animation.x);
		nuDebConTextPos(0,2, 5);
		nuDebConCPuts(0, conbuf);
		sprintf(conbuf, "anim y : %d  ", game.character.animation.y);
		nuDebConTextPos(0,2, 6);
		nuDebConCPuts(0, conbuf);
		sprintf(conbuf, "anim w : %d  ", game.character.animation.sprite->width);
		nuDebConTextPos(0,2, 7);
		nuDebConCPuts(0, conbuf);
		sprintf(conbuf, "anim h : %d  ", game.character.animation.sprite->height);
		nuDebConTextPos(0,2, 8);
		nuDebConCPuts(0, conbuf);
		sprintf(conbuf, "char x : %d  ", game.character.x);
		nuDebConTextPos(0,2, 9);
		nuDebConCPuts(0, conbuf);
		sprintf(conbuf, "char y : %d  ", game.character.y);
		nuDebConTextPos(0,2, 10);
		nuDebConCPuts(0, conbuf);
		sprintf(conbuf, "is in  : %d  ", characterIsInTile(&map, &game.character, 5, 4));
		nuDebConTextPos(0,2, 10);
		nuDebConCPuts(0, conbuf);-*
		
		
		/*sprintf(conbuf, "current : %X  ", &music_tracks[0]);
		nuDebConTextPos(0,2, 6);
		nuDebConCPuts(0, conbuf);
		sprintf(conbuf, "current : %X  ", &music_tracks[1]);
		nuDebConTextPos(0,2, 7);
		nuDebConCPuts(0, conbuf);
		sprintf(conbuf, "next    : %X  ", current_music->next_track);
		nuDebConTextPos(0,2, 8);
		nuDebConCPuts(0, conbuf);*/
		/*sprintf(conbuf, "next    : %X  ", current_music);
		nuDebConTextPos(0,2, 8);
		nuDebConCPuts(0, conbuf);*/
		
		/*
		sprintf(conbuf, "ratio x : %f  ", ratio_x);
		nuDebConTextPos(0,2, 5);
		nuDebConCPuts(0, conbuf);
		sprintf(conbuf, "ratio y : %f  ", ratio_y);
		nuDebConTextPos(0,2, 6);
		nuDebConCPuts(0, conbuf);
		sprintf(conbuf, "log     : %d  ", (int)(log(32)/log(2)));
		nuDebConTextPos(0,2, 7);
		nuDebConCPuts(0, conbuf);*/
		sprintf(conbuf, "speed   : %d  ", game.speed);
		nuDebConTextPos(0,2, 4);
		nuDebConCPuts(0, conbuf);
		sprintf(conbuf, "osTvType: %d  ", osTvType);
		nuDebConTextPos(0,2, 6);
		nuDebConCPuts(0, conbuf);
		sprintf(conbuf, "x       : %d  ", game.character.x);
		nuDebConTextPos(0,2, 8);
		nuDebConCPuts(0, conbuf);
		sprintf(conbuf, "y       : %d  ", game.character.y);
		nuDebConTextPos(0,2, 9);
		nuDebConCPuts(0, conbuf);
		/*sprintf(conbuf, "max col : %d  ", map.max_column);
		nuDebConTextPos(0,2, 10);
		nuDebConCPuts(0, conbuf);*/
		sprintf(conbuf, "first_column : %d  ", position.first_column);
		nuDebConTextPos(0,2, 11);
		nuDebConCPuts(0, conbuf);
		sprintf(conbuf, "first_row    : %d  ", position.first_row);
		nuDebConTextPos(0,2, 12);
		nuDebConCPuts(0, conbuf);
		sprintf(conbuf, "position.x   : %d  ", position.x);
		nuDebConTextPos(0,2, 13);
		nuDebConCPuts(0, conbuf);
		sprintf(conbuf, "position.y   : %d  ", position.y);
		nuDebConTextPos(0,2, 14);
		nuDebConCPuts(0, conbuf);
		/*sprintf(conbuf, "x   : %d  ", game.character.animation.frame);
		nuDebConTextPos(0,2, 13);
		nuDebConCPuts(0, conbuf);
		sprintf(conbuf, "y   : %d  ", game.character.animation.sprite->frames);
		nuDebConTextPos(0,2, 14);
		nuDebConCPuts(0, conbuf);
		sprintf(conbuf, "collision : %d  ", collision);
		nuDebConTextPos(0,2, 14);
		nuDebConCPuts(0, conbuf);*/
		
		sprintf(conbuf, "keys      : %d  ", game.character.keys);
		nuDebConTextPos(0,2, 13);
		nuDebConCPuts(0, conbuf);
		/*sprintf(conbuf, "row state : %X  ", searched_row);
		nuDebConTextPos(0,2, 14);
		nuDebConCPuts(0, conbuf);
		sprintf(conbuf, "previous  : %X  ", searched_row->previous);
		nuDebConTextPos(0,2, 15);
		nuDebConCPuts(0, conbuf);
		sprintf(conbuf, "next      : %X  ", searched_row->next);
		nuDebConTextPos(0,2, 16);
		nuDebConCPuts(0, conbuf);*/
		
		/*sprintf(conbuf, "sound     : %d  ", MusHandleAsk(sndHandle[0]));
		nuDebConTextPos(0,2, 15);
		nuDebConCPuts(0, conbuf);
		sprintf(conbuf, "sound     : %d  ", MusHandleAsk(sndHandle[1]));
		nuDebConTextPos(0,2, 16);
		nuDebConCPuts(0, conbuf);*/
		
		/*sprintf(conbuf, "state     : %d  ", nuAuStlSndPlayerGetState());
		nuDebConTextPos(0,2, 17);
		nuDebConCPuts(0, conbuf);*/
		
		sprintf(conbuf, "RamLeft : %d  ", (u32)my2dlibrary.texturePointer - (u32)_codeSegmentEnd);
		nuDebConTextPos(0,19,3);
		nuDebConCPuts(0, conbuf);
		sprintf(conbuf, "Display list  : %d   ", (gfxListPtr - gfxListStartPtr));
		nuDebConTextPos(0,19,4);
		nuDebConCPuts(0, conbuf);
		/*sprintf(conbuf, "Gfx           : %d   ", sizeof(Gfx));
		nuDebConTextPos(0,19,5);
		nuDebConCPuts(0, conbuf);
		sprintf(conbuf, "gfxListPtr    : %X   ", gfxListPtr);
		nuDebConTextPos(0,10,6);
		nuDebConCPuts(0, conbuf);
		sprintf(conbuf, "gfxStart      : %X   ", gfxListStartPtr);
		nuDebConTextPos(0,10,7);
		nuDebConCPuts(0, conbuf)*/;
		/*sprintf(conbuf, "init_heap  : %d   ", init_heap);
		nuDebConTextPos(0,19,5);
		nuDebConCPuts(0, conbuf);
		sprintf(conbuf, "heap       : %X   ", &mem_heap);
		nuDebConTextPos(0,19,6);
		nuDebConCPuts(0, conbuf);
		sprintf(conbuf, "code       : %X   ", (u32)_codeSegmentEnd);
		nuDebConTextPos(0,19,7);
		nuDebConCPuts(0, conbuf)*/;


	}
}

int collisionStairs(CollisionBox* collision, MapRow* row, int is_character) {
	if (row->over)
		row->over(row);
	return COLLISION_NONE;
}

int collisionBigDoor(CollisionBox* collision, MapRow* row, int is_character) {
	if (row->state == TILE_ACTION_CLOSED && checkIntersection(collision->start_x, collision->end_x, 0, 64) && checkIntersection(collision->start_y, collision->end_y, 0, 40))
		if (collision->action == COLLISION_ACTION_BLOCK)
			return COLLISION_BLOCK;	
		else if (collision->action == COLLISION_ACTION_PUSH && row->state == TILE_ACTION_CLOSED && game.character.keys > 0) {
			game.character.keys--;
			row->state = TILE_ACTION_OPENED;
			nuContRmbStart(0, 256, 30);
			actionRow(row);
		}
	return COLLISION_NONE;
}

int collisionBarrel(CollisionBox* collision, MapRow* row, int is_character) {
	if (checkIntersection(collision->start_x, collision->end_x, 2, 61) && checkIntersection(collision->start_y, collision->end_y, 5, 55))
		if (collision->action == COLLISION_ACTION_BLOCK)
			return COLLISION_BLOCK;
		else if (collision->action == COLLISION_ACTION_SMASH && row->action != NULL) {
			row->action(row);
			if (game.rumble)
				nuContRmbStart(0, 256, 80);
		}
	return COLLISION_NONE;	
}

int collisionTrap(CollisionBox* collision, MapRow* row, int is_character) {
	if (row->state == TILE_ACTION_CLOSED)
		return COLLISION_BLOCK;
	return COLLISION_NONE;
}

int collisionLever(CollisionBox* collision, MapRow* row, int is_character) {
	if (checkIntersection(collision->start_x, collision->end_x, 20, 44) && checkIntersection(collision->start_y, collision->end_y, 38, 46))
		if (collision->action == COLLISION_ACTION_BLOCK)
			return COLLISION_BLOCK;
		else if (collision->action == COLLISION_ACTION_SMASH && row->state < TILE_ACTION_LEVER_OPENED) {
			row->state++;
			if (row->state == TILE_ACTION_LEVER_OPENED)
				actionRow(row);
			nuContRmbStart(0, 256, 20);
		}
	return COLLISION_NONE;
}

int collisionCandelier(CollisionBox* collision, MapRow* row, int is_character) {
	if (collision->action == COLLISION_ACTION_BLOCK && checkIntersection(collision->start_x, collision->end_x, 19, 41) && checkIntersection(collision->start_y, collision->end_y, 36, 49))
		return COLLISION_BLOCK;	
	return COLLISION_NONE;
	
}

int collisionPillar(CollisionBox* collision, MapRow* row, int is_character) {
	if (collision->action == COLLISION_ACTION_BLOCK && checkIntersection(collision->start_x, collision->end_x, 16, 47) && checkIntersection(collision->start_y, collision->end_y, 18, 41))
		return COLLISION_BLOCK;	
	return COLLISION_NONE;
}

int collisionWallPillar(CollisionBox* collision, MapRow* row, int is_character) {
	if (collision->action == COLLISION_ACTION_BLOCK && checkIntersection(collision->start_x, collision->end_x, 16, 47) && checkIntersection(collision->start_y, collision->end_y, 0, 12))
		return COLLISION_BLOCK;	
	return COLLISION_NONE;
}

int collisionDoor(CollisionBox* collision, MapRow* row, int is_character) {
	if (collision->action == COLLISION_ACTION_BLOCK)
		if (row->state == TILE_ACTION_CLOSED)
			return COLLISION_BLOCK;	
		else if (checkIntersection(collision->start_x, collision->end_x, 0, 13) && checkIntersection(collision->start_y, collision->end_y, 0, 40))
			return COLLISION_BLOCK;	
	return COLLISION_NONE;	
}

int collisionKey(CollisionBox* collision, MapRow* row, int is_character) {
	if (collision->action == COLLISION_ACTION_BLOCK && (
		checkIntersection(collision->start_x, collision->end_x, 12, 34) && checkIntersection(collision->start_y, collision->end_y, 27, 48) ||
		checkIntersection(collision->start_x, collision->end_x, 28, 51) && checkIntersection(collision->start_y, collision->end_y, 10, 30)
		)) {
		if (row->state == TILE_ACTION_CLOSED && is_character) {
			game.character.keys++;
			row->state = TILE_ACTION_OPENED;
			nuContRmbStart(0, 256, 10);
		}
	}
	return COLLISION_NONE;
}

int collisionButton(CollisionBox* collision, MapRow* row, int is_character) {
	// press button if on it
	if (is_character && row->tile->animation && row->state == TILE_ACTION_CLOSED && collision->action == COLLISION_ACTION_BLOCK && 
		checkIntersection(collision->start_x, collision->end_x, 7, 56) && checkIntersection(collision->start_y, collision->end_y, 3, 54)) {
		//collision->start_y  > 3 && collision->end_y < 54 && collision->start_x > 7 && collision->end_y < 56) {
		row->state = TILE_ACTION_OPENED;
		nuContRmbStart(0, 256, 40);
		actionRow(row);
	}
	return COLLISION_NONE;
}