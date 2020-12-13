#include <nualstl_n.h>

#ifndef _MAIN_H_
#define _MAIN_H_

#define	HIGH32_ZBUFFER_ADDR	(0x80800000-640*480*4*3)
#define CFB32_HIGH_ADDR0	(0x80800000-640*480*4*2)
#define CFB32_HIGH_ADDR1	(0x80800000-640*480*4*1)
#define	LOW32_ZBUFFER_ADDR	(0x80400000-640*480*4*3)
#define CFB32_LOW_ADDR0		(0x80400000-640*480*4*2)
#define CFB32_LOW_ADDR1		(0x80400000-640*480*4*1)

#define	HIGH_ZBUFFER_ADDR	(0x80800000-640*480*2*3)
#define CFB_HIGH_ADDR0		(0x80800000-640*480*2*2)
#define CFB_HIGH_ADDR1		(0x80800000-640*480*2*1)
#define	LOW_ZBUFFER_ADDR	(0x80400000-640*480*2*3)
#define CFB_LOW_ADDR0		(0x80400000-640*480*2*2)
#define CFB_LOW_ADDR1		(0x80400000-640*480*2*1)

/*
 *  Macro for extern declaration of WAVE data segment
 */
#define EXTERN_SEGMENT(name)    \
extern  char    _##name##SegmentStart[], _##name##SegmentEnd[], \
                _##name##SegmentRomStart[], _##name##SegmentRomEnd[]
				
#define EXTERN_SEGMENT_U8(name)    \
extern  u8      _##name##SegmentStart[], _##name##SegmentEnd[], \
                _##name##SegmentRomStart[], _##name##SegmentRomEnd[]

/* this is to know where the segments are in ROM (check initGame() or initExpansionPak() for example) */
EXTERN_SEGMENT(code);


EXTERN_SEGMENT(BgExpansionPak);

EXTERN_SEGMENT(IntroJam);
EXTERN_SEGMENT(IntroBrewLogo);
EXTERN_SEGMENT(IntroBrewTitle);

EXTERN_SEGMENT(Castle_BorderBottom);
EXTERN_SEGMENT(Castle_BorderCornerBL);
EXTERN_SEGMENT(Castle_BorderCornerBR);
EXTERN_SEGMENT(Castle_BorderCornerTL);
EXTERN_SEGMENT(Castle_BorderCornerTR);
EXTERN_SEGMENT(Castle_BorderLeft);
EXTERN_SEGMENT(Castle_BorderRight);
EXTERN_SEGMENT(Castle_BorderUp);
EXTERN_SEGMENT(Castle_DownStairsL);
EXTERN_SEGMENT(Castle_DownStairsR);
EXTERN_SEGMENT(Castle_Ground1);
EXTERN_SEGMENT(Castle_Ground2);
EXTERN_SEGMENT(Castle_Ground3);
EXTERN_SEGMENT(Castle_UpCornerBL);
EXTERN_SEGMENT(Castle_UpCornerBR);
EXTERN_SEGMENT(Castle_UpCornerTL);
EXTERN_SEGMENT(Castle_UpCornerTR);
EXTERN_SEGMENT(Castle_UpCross);
EXTERN_SEGMENT(Castle_UpDown);
EXTERN_SEGMENT(Castle_UpHorizontal);
EXTERN_SEGMENT(Castle_UpLeft);
EXTERN_SEGMENT(Castle_UpRight);
EXTERN_SEGMENT(Castle_UpStairsL1);
EXTERN_SEGMENT(Castle_UpStairsL2);
EXTERN_SEGMENT(Castle_UpStairsR1);
EXTERN_SEGMENT(Castle_UpStairsR2);
EXTERN_SEGMENT(Castle_UpTBottom);
EXTERN_SEGMENT(Castle_UpTLeft);
EXTERN_SEGMENT(Castle_UpTop);
EXTERN_SEGMENT(Castle_UpTRight);
EXTERN_SEGMENT(Castle_UpTTop);
EXTERN_SEGMENT(Castle_UpVertical);
EXTERN_SEGMENT(Castle_WallDown);
EXTERN_SEGMENT(Castle_WallUp);
EXTERN_SEGMENT(Castle_WallWater);
EXTERN_SEGMENT(Castle_Water);
EXTERN_SEGMENT(Castle_UpAlone);
EXTERN_SEGMENT(Castle_Pillar);
EXTERN_SEGMENT(Castle_WallPillar);

EXTERN_SEGMENT(Dungeon_DownStairsL);
EXTERN_SEGMENT(Dungeon_DownStairsR);
EXTERN_SEGMENT(Dungeon_Ground1);
EXTERN_SEGMENT(Dungeon_Ground2);
EXTERN_SEGMENT(Dungeon_Ground3);
EXTERN_SEGMENT(Dungeon_UpCornerBL);
EXTERN_SEGMENT(Dungeon_UpCornerBR);
EXTERN_SEGMENT(Dungeon_UpCornerTL);
EXTERN_SEGMENT(Dungeon_UpCornerTR);
EXTERN_SEGMENT(Dungeon_UpCross);
EXTERN_SEGMENT(Dungeon_UpDown);
EXTERN_SEGMENT(Dungeon_UpHorizontal);
EXTERN_SEGMENT(Dungeon_UpLeft);
EXTERN_SEGMENT(Dungeon_UpRight);
EXTERN_SEGMENT(Dungeon_UpStairsL);
EXTERN_SEGMENT(Dungeon_UpStairsR);
EXTERN_SEGMENT(Dungeon_UpTBottom);
EXTERN_SEGMENT(Dungeon_UpTLeft);
EXTERN_SEGMENT(Dungeon_UpTop);
EXTERN_SEGMENT(Dungeon_UpTRight);
EXTERN_SEGMENT(Dungeon_UpTTop);
EXTERN_SEGMENT(Dungeon_UpVertical);
EXTERN_SEGMENT(Dungeon_WallDown);
EXTERN_SEGMENT(Dungeon_WallUp);
EXTERN_SEGMENT(Dungeon_WallWater);
EXTERN_SEGMENT(Dungeon_UpAlone);
EXTERN_SEGMENT(Dungeon_Pillar);
EXTERN_SEGMENT(Dungeon_WallPillar);

EXTERN_SEGMENT(ObjectBigDoorLeft);
EXTERN_SEGMENT(ObjectBigDoorRight);
EXTERN_SEGMENT(ObjectCastleButton);
EXTERN_SEGMENT(ObjectDungeonButton);
EXTERN_SEGMENT(ObjectDoor);
EXTERN_SEGMENT(ObjectKey);
EXTERN_SEGMENT(ObjectLever);
EXTERN_SEGMENT(ObjectCastleTrap);
EXTERN_SEGMENT(ObjectDungeonTrap);

EXTERN_SEGMENT(SceneryBarrel01);
EXTERN_SEGMENT(SceneryBarrel02);
EXTERN_SEGMENT(SceneryBarrel03);
EXTERN_SEGMENT(SceneryCandelier);
EXTERN_SEGMENT(SceneryCastleCandelier);
EXTERN_SEGMENT(SceneryDungeonCandelier);
EXTERN_SEGMENT(SceneryChair);
EXTERN_SEGMENT(SceneryDungeonFlag01);
EXTERN_SEGMENT(SceneryCastleFlag01);
EXTERN_SEGMENT(SceneryCastleFlag02);
EXTERN_SEGMENT(SceneryDungeonFlag02);
EXTERN_SEGMENT(SceneryPot01);
EXTERN_SEGMENT(SceneryPot02);
EXTERN_SEGMENT(SceneryPot03);
EXTERN_SEGMENT(SceneryTable);
EXTERN_SEGMENT(SceneryCastleTorch);
EXTERN_SEGMENT(SceneryDungeonTorch);
EXTERN_SEGMENT(SceneryCastleWallTorch);
EXTERN_SEGMENT(SceneryDungeonWallTorch);
EXTERN_SEGMENT(SceneryWindow01);
EXTERN_SEGMENT(SceneryWindow02);
EXTERN_SEGMENT(SceneryWindow03);
EXTERN_SEGMENT(SceneryWindow04);
EXTERN_SEGMENT(SceneryWater);
EXTERN_SEGMENT(SceneryBook01Left);
EXTERN_SEGMENT(SceneryBook01Right);
EXTERN_SEGMENT(SceneryBook02Left);
EXTERN_SEGMENT(SceneryBook02Middle);
EXTERN_SEGMENT(SceneryBook02Right);
EXTERN_SEGMENT(SceneryCarpetTL);
EXTERN_SEGMENT(SceneryCarpetT);
EXTERN_SEGMENT(SceneryCarpetTR);
EXTERN_SEGMENT(SceneryCarpetL);
EXTERN_SEGMENT(SceneryCarpetC);
EXTERN_SEGMENT(SceneryCarpetR);
EXTERN_SEGMENT(SceneryCarpetBL);
EXTERN_SEGMENT(SceneryCarpetB);
EXTERN_SEGMENT(SceneryCarpetBR);

EXTERN_SEGMENT(64_back_attack);
EXTERN_SEGMENT(64_back_defend);
EXTERN_SEGMENT(64_back_hurt);
EXTERN_SEGMENT(64_back_idle);
EXTERN_SEGMENT(64_back_walk);
EXTERN_SEGMENT(64_front_attack);
EXTERN_SEGMENT(64_front_defend);
EXTERN_SEGMENT(64_front_hurt);
EXTERN_SEGMENT(64_front_idle);
EXTERN_SEGMENT(64_front_walk);
EXTERN_SEGMENT(64_left_attack);
EXTERN_SEGMENT(64_left_defend);
EXTERN_SEGMENT(64_left_hurt);
EXTERN_SEGMENT(64_left_idle);
EXTERN_SEGMENT(64_left_walk);
EXTERN_SEGMENT(64_right_attack);
EXTERN_SEGMENT(64_right_defend);
EXTERN_SEGMENT(64_right_hurt);
EXTERN_SEGMENT(64_right_idle);
EXTERN_SEGMENT(64_right_walk);

EXTERN_SEGMENT(dead);
EXTERN_SEGMENT(walk01);
EXTERN_SEGMENT(walk02);
EXTERN_SEGMENT(walk03);
EXTERN_SEGMENT(walk04);
EXTERN_SEGMENT(walk05);
EXTERN_SEGMENT(walk06);

EXTERN_SEGMENT(RatBackIdle);
EXTERN_SEGMENT(RatBackDead);
EXTERN_SEGMENT(RatBackWalk);
EXTERN_SEGMENT(RatFrontIdle);
EXTERN_SEGMENT(RatFrontWalk);
EXTERN_SEGMENT(RatFrontDead);
EXTERN_SEGMENT(RatLeftIdle);
EXTERN_SEGMENT(RatLeftDead);
EXTERN_SEGMENT(RatLeftWalk);
EXTERN_SEGMENT(RatRightIdle);
EXTERN_SEGMENT(RatRightDead);
EXTERN_SEGMENT(RatRightWalk);

EXTERN_SEGMENT(Level00);
EXTERN_SEGMENT(Level01);


EXTERN_SEGMENT(MusicAllWbk);
EXTERN_SEGMENT(MusicAllSfx);
EXTERN_SEGMENT(MusicAllPtr);
/*EXTERN_SEGMENT(MusicChanceWbk);
EXTERN_SEGMENT(MusicChanceSfx);
EXTERN_SEGMENT(MusicChancePtr);
EXTERN_SEGMENT(MusicLevel01Wbk);
EXTERN_SEGMENT(MusicLevel01Sfx);
EXTERN_SEGMENT(MusicLevel01Ptr);
EXTERN_SEGMENT(MusicBoingWbk);
EXTERN_SEGMENT(MusicBoingSfx);
EXTERN_SEGMENT(MusicBoingPtr);*/

EXTERN_SEGMENT(MainMenuBackground01);
EXTERN_SEGMENT(MainMenuBackground02);
EXTERN_SEGMENT(MainMenuBackground03);
EXTERN_SEGMENT(MainMenuBackground04);
EXTERN_SEGMENT(MainMenuBackground05);
EXTERN_SEGMENT(MainMenuBackground06);
EXTERN_SEGMENT(MainMenuBackground07);
EXTERN_SEGMENT(MainMenuBackground08);
EXTERN_SEGMENT(MainMenuTitle);
EXTERN_SEGMENT(MainMenuMenu);
EXTERN_SEGMENT(MainMenuHead);
EXTERN_SEGMENT(MainMenuVolume);

EXTERN_SEGMENT(SplashLevel00);
EXTERN_SEGMENT(SplashLevel01);
EXTERN_SEGMENT(SplashEnd);



/* */
u64 time_lastframe, total_time;
/* Buffer for console display */
char conbuf[40];
/* storing the amount of RAM available. Shared because calling osGetMemSize() clears memory in the expansion pak, so it needs to be done early in the code */
u32 memory_size;

u8 contPattern;

u8 show_console;

/* dedicated memory area for malloc */
char mem_heap[300 * 1024]; 

int init_heap;

void callbackPrenmi();

#endif /* _MAIN_H_*/
