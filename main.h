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

EXTERN_SEGMENT(16_BorderBottom);
EXTERN_SEGMENT(16_BorderCornerBL);
EXTERN_SEGMENT(16_BorderCornerBR);
EXTERN_SEGMENT(16_BorderCornerTL);
EXTERN_SEGMENT(16_BorderCornerTR);
EXTERN_SEGMENT(16_BorderLeft);
EXTERN_SEGMENT(16_BorderRight);
EXTERN_SEGMENT(16_BorderUp);
EXTERN_SEGMENT(16_DownStairsL);
EXTERN_SEGMENT(16_DownStairsR);
EXTERN_SEGMENT(16_Ground1);
EXTERN_SEGMENT(16_Ground2);
EXTERN_SEGMENT(16_Ground3);
EXTERN_SEGMENT(16_UpCornerBL);
EXTERN_SEGMENT(16_UpCornerBR);
EXTERN_SEGMENT(16_UpCornerTl);
EXTERN_SEGMENT(16_UpCornerTR);
EXTERN_SEGMENT(16_UpCross);
EXTERN_SEGMENT(16_UpDown);
EXTERN_SEGMENT(16_UpHorizontal);
EXTERN_SEGMENT(16_UpLeft);
EXTERN_SEGMENT(16_UpRight);
EXTERN_SEGMENT(16_UpStairsL1);
EXTERN_SEGMENT(16_UpStairsL2);
EXTERN_SEGMENT(16_UpStairsR1);
EXTERN_SEGMENT(16_UpStairsR2);
EXTERN_SEGMENT(16_UpTBottom);
EXTERN_SEGMENT(16_UpTLeft);
EXTERN_SEGMENT(16_UpTop);
EXTERN_SEGMENT(16_UpTRight);
EXTERN_SEGMENT(16_UpTTop);
EXTERN_SEGMENT(16_UpVertical);
EXTERN_SEGMENT(16_WallDown);
EXTERN_SEGMENT(16_WallUp);
EXTERN_SEGMENT(16_WallWater);
EXTERN_SEGMENT(16_Water);
EXTERN_SEGMENT(16_UpAlone);
EXTERN_SEGMENT(16_Pillar);
EXTERN_SEGMENT(16_WallPillar);
EXTERN_SEGMENT(64_BorderBottom);
EXTERN_SEGMENT(64_BorderCornerBL);
EXTERN_SEGMENT(64_BorderCornerBR);
EXTERN_SEGMENT(64_BorderCornerTL);
EXTERN_SEGMENT(64_BorderCornerTR);
EXTERN_SEGMENT(64_BorderLeft);
EXTERN_SEGMENT(64_BorderRight);
EXTERN_SEGMENT(64_BorderUp);
EXTERN_SEGMENT(64_DownStairsL);
EXTERN_SEGMENT(64_DownStairsR);
EXTERN_SEGMENT(64_Ground1);
EXTERN_SEGMENT(64_Ground2);
EXTERN_SEGMENT(64_Ground3);
EXTERN_SEGMENT(64_UpCornerBL);
EXTERN_SEGMENT(64_UpCornerBR);
EXTERN_SEGMENT(64_UpCornerTl);
EXTERN_SEGMENT(64_UpCornerTR);
EXTERN_SEGMENT(64_UpCross);
EXTERN_SEGMENT(64_UpDown);
EXTERN_SEGMENT(64_UpHorizontal);
EXTERN_SEGMENT(64_UpLeft);
EXTERN_SEGMENT(64_UpRight);
EXTERN_SEGMENT(64_UpStairsL1);
EXTERN_SEGMENT(64_UpStairsL2);
EXTERN_SEGMENT(64_UpStairsR1);
EXTERN_SEGMENT(64_UpStairsR2);
EXTERN_SEGMENT(64_UpTBottom);
EXTERN_SEGMENT(64_UpTLeft);
EXTERN_SEGMENT(64_UpTop);
EXTERN_SEGMENT(64_UpTRight);
EXTERN_SEGMENT(64_UpTTop);
EXTERN_SEGMENT(64_UpVertical);
EXTERN_SEGMENT(64_WallDown);
EXTERN_SEGMENT(64_WallUp);
EXTERN_SEGMENT(64_WallWater);
EXTERN_SEGMENT(64_Water);
EXTERN_SEGMENT(64_UpAlone);
EXTERN_SEGMENT(64_Pillar);
EXTERN_SEGMENT(64_WallPillar);

EXTERN_SEGMENT(ObjectBigDoorLeft);
EXTERN_SEGMENT(ObjectBigDoorRight);
EXTERN_SEGMENT(ObjectButton);
EXTERN_SEGMENT(ObjectDoor);
EXTERN_SEGMENT(ObjectKey);
EXTERN_SEGMENT(ObjectLever);
EXTERN_SEGMENT(ObjectTrap);

EXTERN_SEGMENT(SceneryBarrel01);
EXTERN_SEGMENT(SceneryBarrel02);
EXTERN_SEGMENT(SceneryBarrel03);
EXTERN_SEGMENT(SceneryCandelier);
EXTERN_SEGMENT(SceneryChair);
EXTERN_SEGMENT(SceneryFlag01);
EXTERN_SEGMENT(SceneryFlag02);
EXTERN_SEGMENT(SceneryPot01);
EXTERN_SEGMENT(SceneryPot02);
EXTERN_SEGMENT(SceneryPot03);
EXTERN_SEGMENT(SceneryTable);
EXTERN_SEGMENT(SceneryTorch);
EXTERN_SEGMENT(SceneryWallTorch);
EXTERN_SEGMENT(SceneryWindow01);
EXTERN_SEGMENT(SceneryWindow02);
EXTERN_SEGMENT(SceneryWindow03);


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

EXTERN_SEGMENT(Level01);


EXTERN_SEGMENT(MusicChanceWbk);
EXTERN_SEGMENT(MusicChanceSfx);
EXTERN_SEGMENT(MusicChancePtr);
EXTERN_SEGMENT(MusicLevel01Wbk);
EXTERN_SEGMENT(MusicLevel01Sfx);
EXTERN_SEGMENT(MusicLevel01Ptr);
EXTERN_SEGMENT(MusicBoingWbk);
EXTERN_SEGMENT(MusicBoingSfx);
EXTERN_SEGMENT(MusicBoingPtr);

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
