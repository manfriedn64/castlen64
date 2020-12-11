#include "nusys.h"

beginseg
	name	"code"
	flags	BOOT OBJECT
	entry 	nuBoot
	address 0x80000400
	/*stack	nuMainStack + 0x8000
	address NU_SPEC_BOOT_ADDR*/
	stack	NU_SPEC_BOOT_STACK
	include "codesegment.o"
	include "$(ROOT)/usr/lib/PR/rspboot.o"
	include "$(ROOT)/usr/lib/PR/aspMain.o"
	include "$(ROOT)/usr/lib/PR/n_aspMain.o"
	include "$(ROOT)/usr/lib/PR/gspF3DEX2.fifo.o"
	include "$(ROOT)/usr/lib/PR/gspL3DEX2.fifo.o"
	include "$(ROOT)/usr/lib/PR/gspF3DEX2.Rej.fifo.o"
      	include "$(ROOT)/usr/lib/PR/gspF3DEX2.NoN.fifo.o"
      	include "$(ROOT)/usr/lib/PR/gspF3DLX2.Rej.fifo.o"
	include "$(ROOT)/usr/lib/PR/gspS2DEX2.fifo.o"
	include "$(ROOT)/usr/lib/PR/aspMain.o"
	include "$(ROOT)/usr/lib/PR/n_aspMain.o"
endseg

beginseg
  name    "16_BorderBottom"
  flags   RAW
  include "assets/tiles/16x16/BorderBottom.551"
endseg
beginseg
  name    "16_BorderCornerBL"
  flags   RAW
  include "assets/tiles/16x16/BorderCornerBL.551"
endseg
beginseg
  name    "16_BorderCornerBR"
  flags   RAW
  include "assets/tiles/16x16/BorderCornerBR.551"
endseg
beginseg
  name    "16_BorderCornerTL"
  flags   RAW
  include "assets/tiles/16x16/BorderCornerTL.551"
endseg
beginseg
  name    "16_BorderCornerTR"
  flags   RAW
  include "assets/tiles/16x16/BorderCornerTR.551"
endseg
beginseg
  name    "16_BorderLeft"
  flags   RAW
  include "assets/tiles/16x16/BorderLeft.551"
endseg
beginseg
  name    "16_BorderRight"
  flags   RAW
  include "assets/tiles/16x16/BorderRight.551"
endseg
beginseg
  name    "16_BorderUp"
  flags   RAW
  include "assets/tiles/16x16/BorderUp.551"
endseg
beginseg
  name    "16_DownStairsL"
  flags   RAW
  include "assets/tiles/16x16/DownStairsL.551"
endseg
beginseg
  name    "16_DownStairsR"
  flags   RAW
  include "assets/tiles/16x16/DownStairsR.551"
endseg
beginseg
  name    "16_Ground1"
  flags   RAW
  include "assets/tiles/16x16/Ground1.551"
endseg
beginseg
  name    "16_Ground2"
  flags   RAW
  include "assets/tiles/16x16/Ground2.551"
endseg
beginseg
  name    "16_Ground3"
  flags   RAW
  include "assets/tiles/16x16/Ground3.551"
endseg
beginseg
  name    "16_UpCornerBL"
  flags   RAW
  include "assets/tiles/16x16/UpCornerBL.551"
endseg
beginseg
  name    "16_UpCornerBR"
  flags   RAW
  include "assets/tiles/16x16/UpCornerBR.551"
endseg
beginseg
  name    "16_UpCornerTl"
  flags   RAW
  include "assets/tiles/16x16/UpCornerTl.551"
endseg
beginseg
  name    "16_UpCornerTR"
  flags   RAW
  include "assets/tiles/16x16/UpCornerTR.551"
endseg
beginseg
  name    "16_UpCross"
  flags   RAW
  include "assets/tiles/16x16/UpCross.551"
endseg
beginseg
  name    "16_UpDown"
  flags   RAW
  include "assets/tiles/16x16/UpDown.551"
endseg
beginseg
  name    "16_UpHorizontal"
  flags   RAW
  include "assets/tiles/16x16/UpHorizontal.551"
endseg
beginseg
  name    "16_UpLeft"
  flags   RAW
  include "assets/tiles/16x16/UpLeft.551"
endseg
beginseg
  name    "16_UpRight"
  flags   RAW
  include "assets/tiles/16x16/UpRight.551"
endseg
beginseg
  name    "16_UpStairsL1"
  flags   RAW
  include "assets/tiles/16x16/UpStairsL1.551"
endseg
beginseg
  name    "16_UpStairsL2"
  flags   RAW
  include "assets/tiles/16x16/UpStairsL2.551"
endseg
beginseg
  name    "16_UpStairsR1"
  flags   RAW
  include "assets/tiles/16x16/UpStairsR1.551"
endseg
beginseg
  name    "16_UpStairsR2"
  flags   RAW
  include "assets/tiles/16x16/UpStairsR2.551"
endseg
beginseg
  name    "16_UpTBottom"
  flags   RAW
  include "assets/tiles/16x16/UpTBottom.551"
endseg
beginseg
  name    "16_UpTLeft"
  flags   RAW
  include "assets/tiles/16x16/UpTLeft.551"
endseg
beginseg
  name    "16_UpTop"
  flags   RAW
  include "assets/tiles/16x16/UpTop.551"
endseg
beginseg
  name    "16_UpTRight"
  flags   RAW
  include "assets/tiles/16x16/UpTRight.551"
endseg
beginseg
  name    "16_UpTTop"
  flags   RAW
  include "assets/tiles/16x16/UpTTop.551"
endseg
beginseg
  name    "16_UpVertical"
  flags   RAW
  include "assets/tiles/16x16/UpVertical.551"
endseg
beginseg
  name    "16_WallDown"
  flags   RAW
  include "assets/tiles/16x16/WallDown.551"
endseg
beginseg
  name    "16_WallUp"
  flags   RAW
  include "assets/tiles/16x16/WallUp.551"
endseg
beginseg
  name    "16_WallWater"
  flags   RAW
  include "assets/tiles/16x16/WallWater.551"
endseg
beginseg
  name    "16_UpAlone"
  flags   RAW
  include "assets/tiles/16x16/UpAlone.551"
endseg
beginseg
  name    "16_Pillar"
  flags   RAW
  include "assets/tiles/16x16/Pillar.551"
endseg
beginseg
  name    "16_WallPillar"
  flags   RAW
  include "assets/tiles/16x16/WallPillar.551"
endseg

beginseg
  name    "16_Water"
  flags   RAW
  include "assets/tiles/16x16/Water.551"
endseg
beginseg
  name    "64_BorderBottom"
  flags   RAW
  include "assets/tiles/64x64/BorderBottom.551"
endseg
beginseg
  name    "64_BorderCornerBL"
  flags   RAW
  include "assets/tiles/64x64/BorderCornerBL.551"
endseg
beginseg
  name    "64_BorderCornerBR"
  flags   RAW
  include "assets/tiles/64x64/BorderCornerBR.551"
endseg
beginseg
  name    "64_BorderCornerTL"
  flags   RAW
  include "assets/tiles/64x64/BorderCornerTL.551"
endseg
beginseg
  name    "64_BorderCornerTR"
  flags   RAW
  include "assets/tiles/64x64/BorderCornerTR.551"
endseg
beginseg
  name    "64_BorderLeft"
  flags   RAW
  include "assets/tiles/64x64/BorderLeft.551"
endseg
beginseg
  name    "64_BorderRight"
  flags   RAW
  include "assets/tiles/64x64/BorderRight.551"
endseg
beginseg
  name    "64_BorderUp"
  flags   RAW
  include "assets/tiles/64x64/BorderUp.551"
endseg
beginseg
  name    "64_DownStairsL"
  flags   RAW
  include "assets/tiles/64x64/DownStairsL.551"
endseg
beginseg
  name    "64_DownStairsR"
  flags   RAW
  include "assets/tiles/64x64/DownStairsR.551"
endseg
beginseg
  name    "64_Ground1"
  flags   RAW
  include "assets/tiles/64x64/Ground1.551"
endseg
beginseg
  name    "64_Ground2"
  flags   RAW
  include "assets/tiles/64x64/Ground2.551"
endseg
beginseg
  name    "64_Ground3"
  flags   RAW
  include "assets/tiles/64x64/Ground3.551"
endseg
beginseg
  name    "64_UpCornerBL"
  flags   RAW
  include "assets/tiles/64x64/UpCornerBL.551"
endseg
beginseg
  name    "64_UpCornerBR"
  flags   RAW
  include "assets/tiles/64x64/UpCornerBR.551"
endseg
beginseg
  name    "64_UpCornerTl"
  flags   RAW
  include "assets/tiles/64x64/UpCornerTl.551"
endseg
beginseg
  name    "64_UpCornerTR"
  flags   RAW
  include "assets/tiles/64x64/UpCornerTR.551"
endseg
beginseg
  name    "64_UpCross"
  flags   RAW
  include "assets/tiles/64x64/UpCross.551"
endseg
beginseg
  name    "64_UpDown"
  flags   RAW
  include "assets/tiles/64x64/UpDown.551"
endseg
beginseg
  name    "64_UpHorizontal"
  flags   RAW
  include "assets/tiles/64x64/UpHorizontal.551"
endseg
beginseg
  name    "64_UpLeft"
  flags   RAW
  include "assets/tiles/64x64/UpLeft.551"
endseg
beginseg
  name    "64_UpRight"
  flags   RAW
  include "assets/tiles/64x64/UpRight.551"
endseg
beginseg
  name    "64_UpStairsL1"
  flags   RAW
  include "assets/tiles/64x64/UpStairsL1.551"
endseg
beginseg
  name    "64_UpStairsL2"
  flags   RAW
  include "assets/tiles/64x64/UpStairsL2.551"
endseg
beginseg
  name    "64_UpStairsR1"
  flags   RAW
  include "assets/tiles/64x64/UpStairsR1.551"
endseg
beginseg
  name    "64_UpStairsR2"
  flags   RAW
  include "assets/tiles/64x64/UpStairsR2.551"
endseg
beginseg
  name    "64_UpTBottom"
  flags   RAW
  include "assets/tiles/64x64/UpTBottom.551"
endseg
beginseg
  name    "64_UpTLeft"
  flags   RAW
  include "assets/tiles/64x64/UpTLeft.551"
endseg
beginseg
  name    "64_UpTop"
  flags   RAW
  include "assets/tiles/64x64/UpTop.551"
endseg
beginseg
  name    "64_UpTRight"
  flags   RAW
  include "assets/tiles/64x64/UpTRight.551"
endseg
beginseg
  name    "64_UpTTop"
  flags   RAW
  include "assets/tiles/64x64/UpTTop.551"
endseg
beginseg
  name    "64_UpVertical"
  flags   RAW
  include "assets/tiles/64x64/UpVertical.551"
endseg
beginseg
  name    "64_WallDown"
  flags   RAW
  include "assets/tiles/64x64/WallDown.551"
endseg
beginseg
  name    "64_WallUp"
  flags   RAW
  include "assets/tiles/64x64/WallUp.551"
endseg
beginseg
  name    "64_WallWater"
  flags   RAW
  include "assets/tiles/64x64/WallWater.551"
endseg
beginseg
  name    "64_Water"
  flags   RAW
  include "assets/tiles/64x64/Water.551"
endseg
beginseg
  name    "64_UpAlone"
  flags   RAW
  include "assets/tiles/64x64/UpAlone.551"
endseg
beginseg
  name    "64_Pillar"
  flags   RAW
  include "assets/tiles/64x64/Pillar.551"
endseg
beginseg
  name    "64_WallPillar"
  flags   RAW
  include "assets/tiles/64x64/WallPillar.551"
endseg

beginseg
  name    "ObjectBigDoorLeft"
  flags   RAW
  include "assets/objects/bigdoor_left.551"
endseg
beginseg
  name    "ObjectBigDoorRight"
  flags   RAW
  include "assets/objects/bigdoor_right.551"
endseg
beginseg
  name    "ObjectButton"
  flags   RAW
  include "assets/objects/button.551"
endseg
beginseg
  name    "ObjectDoor"
  flags   RAW
  include "assets/objects/door.551"
endseg
beginseg
  name    "ObjectKey"
  flags   RAW
  include "assets/objects/key.551"
endseg
beginseg
  name    "ObjectLever"
  flags   RAW
  include "assets/objects/lever.551"
endseg
beginseg
  name    "ObjectTrap"
  flags   RAW
  include "assets/objects/trap.551"
endseg

beginseg
  name    "SceneryBarrel01"
  flags   RAW
  include "assets/scenery/barrel01.551"
endseg
beginseg
  name    "SceneryBarrel02"
  flags   RAW
  include "assets/scenery/barrel02.551"
endseg
beginseg
  name    "SceneryBarrel03"
  flags   RAW
  include "assets/scenery/barrel03.551"
endseg
beginseg
  name    "SceneryCandelier"
  flags   RAW
  include "assets/scenery/candelier.551"
endseg
beginseg
  name    "SceneryChair"
  flags   RAW
  include "assets/scenery/chair.551"
endseg
beginseg
  name    "SceneryFlag01"
  flags   RAW
  include "assets/scenery/flag01.551"
endseg
beginseg
  name    "SceneryFlag02"
  flags   RAW
  include "assets/scenery/flag02.551"
endseg
beginseg
  name    "SceneryPot01"
  flags   RAW
  include "assets/scenery/pot01.551"
endseg
beginseg
  name    "SceneryPot02"
  flags   RAW
  include "assets/scenery/pot02.551"
endseg
beginseg
  name    "SceneryPot03"
  flags   RAW
  include "assets/scenery/pot03.551"
endseg
beginseg
  name    "SceneryTable"
  flags   RAW
  include "assets/scenery/table.551"
endseg
beginseg
  name    "SceneryTorch"
  flags   RAW
  include "assets/scenery/torch.551"
endseg
beginseg
  name    "SceneryWallTorch"
  flags   RAW
  include "assets/scenery/walltorch.551"
endseg
beginseg
  name    "SceneryWindow01"
  flags   RAW
  include "assets/scenery/window01.551"
endseg
beginseg
  name    "SceneryWindow02"
  flags   RAW
  include "assets/scenery/window02.551"
endseg
beginseg
  name    "SceneryWindow03"
  flags   RAW
  include "assets/scenery/window03.551"
endseg





beginseg
  name    "64_back_attack"
  flags   RAW
  include "assets/knight/64/back_attack.551"
endseg
beginseg
  name    "64_back_defend"
  flags   RAW
  include "assets/knight/64/back_defend.551"
endseg
beginseg
  name    "64_back_hurt"
  flags   RAW
  include "assets/knight/64/back_hurt.551"
endseg
beginseg
  name    "64_back_idle"
  flags   RAW
  include "assets/knight/64/back_idle.551"
endseg
beginseg
  name    "64_back_walk"
  flags   RAW
  include "assets/knight/64/back_walk.551"
endseg
beginseg
  name    "64_front_attack"
  flags   RAW
  include "assets/knight/64/front_attack.551"
endseg
beginseg
  name    "64_front_defend"
  flags   RAW
  include "assets/knight/64/front_defend.551"
endseg
beginseg
  name    "64_front_hurt"
  flags   RAW
  include "assets/knight/64/front_hurt.551"
endseg
beginseg
  name    "64_front_idle"
  flags   RAW
  include "assets/knight/64/front_idle.551"
endseg
beginseg
  name    "64_front_walk"
  flags   RAW
  include "assets/knight/64/front_walk.551"
endseg
beginseg
  name    "64_left_attack"
  flags   RAW
  include "assets/knight/64/left_attack.551"
endseg
beginseg
  name    "64_left_defend"
  flags   RAW
  include "assets/knight/64/left_defend.551"
endseg
beginseg
  name    "64_left_hurt"
  flags   RAW
  include "assets/knight/64/left_hurt.551"
endseg
beginseg
  name    "64_left_idle"
  flags   RAW
  include "assets/knight/64/left_idle.551"
endseg
beginseg
  name    "64_left_walk"
  flags   RAW
  include "assets/knight/64/left_walk.551"
endseg
beginseg
  name    "64_right_attack"
  flags   RAW
  include "assets/knight/64/right_attack.551"
endseg
beginseg
  name    "64_right_defend"
  flags   RAW
  include "assets/knight/64/right_defend.551"
endseg
beginseg
  name    "64_right_hurt"
  flags   RAW
  include "assets/knight/64/right_hurt.551"
endseg
beginseg
  name    "64_right_idle"
  flags   RAW
  include "assets/knight/64/right_idle.551"
endseg
beginseg
  name    "64_right_walk"
  flags   RAW
  include "assets/knight/64/right_walk.551"
endseg

beginseg
  name    "dead"
  flags   RAW
  include "assets/knight/dead.551"
endseg
beginseg
  name    "walk01"
  flags   RAW
  include "assets/knight/walk01.551"
endseg
beginseg
  name    "walk02"
  flags   RAW
  include "assets/knight/walk02.551"
endseg
beginseg
  name    "walk03"
  flags   RAW
  include "assets/knight/walk03.551"
endseg
beginseg
  name    "walk04"
  flags   RAW
  include "assets/knight/walk04.551"
endseg
beginseg
  name    "walk05"
  flags   RAW
  include "assets/knight/walk05.551"
endseg
beginseg
  name    "walk06"
  flags   RAW
  include "assets/knight/walk06.551"
endseg


beginseg
  name    "RatBackDead"
  flags   RAW
  include "assets/rat/rat_back_dead.551"
endseg
beginseg
  name    "RatBackIdle"
  flags   RAW
  include "assets/rat/rat_back_idle.551"
endseg
beginseg
  name    "RatBackWalk"
  flags   RAW
  include "assets/rat/rat_back_walk.551"
endseg
beginseg
  name    "RatFrontDead"
  flags   RAW
  include "assets/rat/rat_front_dead.551"
endseg
beginseg
  name    "RatFrontIdle"
  flags   RAW
  include "assets/rat/rat_front_idle.551"
endseg
beginseg
  name    "RatFrontWalk"
  flags   RAW
  include "assets/rat/rat_front_walk.551"
endseg
beginseg
  name    "RatLeftDead"
  flags   RAW
  include "assets/rat/rat_left_dead.551"
endseg
beginseg
  name    "RatLeftIdle"
  flags   RAW
  include "assets/rat/rat_left_idle.551"
endseg
beginseg
  name    "RatLeftWalk"
  flags   RAW
  include "assets/rat/rat_left_walk.551"
endseg
beginseg
  name    "RatRightDead"
  flags   RAW
  include "assets/rat/rat_right_dead.551"
endseg
beginseg
  name    "RatRightIdle"
  flags   RAW
  include "assets/rat/rat_right_idle.551"
endseg
beginseg
  name    "RatRightWalk"
  flags   RAW
  include "assets/rat/rat_right_walk.551"
endseg


beginseg
	name    "BgExpansionPak"
	flags   RAW
	include "assets/background/expansion_pak.551"
endseg

beginseg
	name    "IntroJam"
	flags   RAW
	include "assets/intro/gamejam.551"
endseg
beginseg
	name    "IntroBrewLogo"
	flags   RAW
	include "assets/intro/n64brew_logo.551"
endseg
beginseg
	name    "IntroBrewTitle"
	flags   RAW
	include "assets/intro/n64brew_title.551"
endseg

beginseg
  name    "Level01"
  flags   RAW
  include "assets/levels/level01.blv"
endseg

beginseg
	name	"MusicChanceWbk"
	flags	RAW
	include "assets/music/chance.wbk"
endseg
beginseg
	name	"MusicChanceSfx"
	flags	RAW
	include "assets/music/chance.bfx"
endseg
beginseg
	name	"MusicChancePtr"
	flags	RAW
	include "assets/music/chance.ptr"
endseg
beginseg
	name	"MusicLevel01Wbk"
	flags	RAW
	include "assets/music/level01.wbk"
endseg
beginseg
	name	"MusicLevel01Sfx"
	flags	RAW
	include "assets/music/level01.bfx"
endseg
beginseg
	name	"MusicLevel01Ptr"
	flags	RAW
	include "assets/music/level01.ptr"
endseg
beginseg
	name	"MusicBoingWbk"
	flags	RAW
	include "assets/music/boing.wbk"
endseg
beginseg
	name	"MusicBoingSfx"
	flags	RAW
	include "assets/music/boing.bfx"
endseg
beginseg
	name	"MusicBoingPtr"
	flags	RAW
	include "assets/music/boing.ptr"
endseg

beginseg
	name	"MainMenuBackground01"
	flags	RAW
	include "assets/mainmenu/layer_1.551"
endseg
beginseg
	name	"MainMenuBackground02"
	flags	RAW
	include "assets/mainmenu/layer_2.551"
endseg
beginseg
	name	"MainMenuBackground03"
	flags	RAW
	include "assets/mainmenu/layer_3.551"
endseg
beginseg
	name	"MainMenuBackground04"
	flags	RAW
	include "assets/mainmenu/layer_4.551"
endseg
beginseg
	name	"MainMenuBackground05"
	flags	RAW
	include "assets/mainmenu/layer_5.551"
endseg
beginseg
	name	"MainMenuBackground06"
	flags	RAW
	include "assets/mainmenu/layer_6.551"
endseg
beginseg
	name	"MainMenuBackground07"
	flags	RAW
	include "assets/mainmenu/layer_7.551"
endseg
beginseg
	name	"MainMenuBackground08"
	flags	RAW
	include "assets/mainmenu/layer_8.551"
endseg
beginseg
	name	"MainMenuTitle"
	flags	RAW
	include "assets/mainmenu/title.551"
endseg
beginseg
	name	"MainMenuMenu"
	flags	RAW
	include "assets/mainmenu/menu.551"
endseg
beginseg
	name	"MainMenuHead"
	flags	RAW
	include "assets/mainmenu/head.551"
endseg

beginwave
	name	"castle"
	include	"code"
endwave
