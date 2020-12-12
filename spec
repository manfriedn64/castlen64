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
  name    "Castle_BorderBottom"
  flags   RAW
  include "assets/tiles/castle/BorderBottom.551"
endseg
beginseg
  name    "Castle_BorderCornerBL"
  flags   RAW
  include "assets/tiles/castle/BorderCornerBL.551"
endseg
beginseg
  name    "Castle_BorderCornerBR"
  flags   RAW
  include "assets/tiles/castle/BorderCornerBR.551"
endseg
beginseg
  name    "Castle_BorderCornerTL"
  flags   RAW
  include "assets/tiles/castle/BorderCornerTL.551"
endseg
beginseg
  name    "Castle_BorderCornerTR"
  flags   RAW
  include "assets/tiles/castle/BorderCornerTR.551"
endseg
beginseg
  name    "Castle_BorderLeft"
  flags   RAW
  include "assets/tiles/castle/BorderLeft.551"
endseg
beginseg
  name    "Castle_BorderRight"
  flags   RAW
  include "assets/tiles/castle/BorderRight.551"
endseg
beginseg
  name    "Castle_BorderUp"
  flags   RAW
  include "assets/tiles/castle/BorderUp.551"
endseg
beginseg
  name    "Castle_DownStairsL"
  flags   RAW
  include "assets/tiles/castle/DownStairsL.551"
endseg
beginseg
  name    "Castle_DownStairsR"
  flags   RAW
  include "assets/tiles/castle/DownStairsR.551"
endseg
beginseg
  name    "Castle_Ground1"
  flags   RAW
  include "assets/tiles/castle/Ground1.551"
endseg
beginseg
  name    "Castle_Ground2"
  flags   RAW
  include "assets/tiles/castle/Ground2.551"
endseg
beginseg
  name    "Castle_Ground3"
  flags   RAW
  include "assets/tiles/castle/Ground3.551"
endseg
beginseg
  name    "Castle_UpCornerBL"
  flags   RAW
  include "assets/tiles/castle/UpCornerBL.551"
endseg
beginseg
  name    "Castle_UpCornerBR"
  flags   RAW
  include "assets/tiles/castle/UpCornerBR.551"
endseg
beginseg
  name    "Castle_UpCornerTL"
  flags   RAW
  include "assets/tiles/castle/UpCornerTL.551"
endseg
beginseg
  name    "Castle_UpCornerTR"
  flags   RAW
  include "assets/tiles/castle/UpCornerTR.551"
endseg
beginseg
  name    "Castle_UpCross"
  flags   RAW
  include "assets/tiles/castle/UpCross.551"
endseg
beginseg
  name    "Castle_UpDown"
  flags   RAW
  include "assets/tiles/castle/UpDown.551"
endseg
beginseg
  name    "Castle_UpHorizontal"
  flags   RAW
  include "assets/tiles/castle/UpHorizontal.551"
endseg
beginseg
  name    "Castle_UpLeft"
  flags   RAW
  include "assets/tiles/castle/UpLeft.551"
endseg
beginseg
  name    "Castle_UpRight"
  flags   RAW
  include "assets/tiles/castle/UpRight.551"
endseg
beginseg
  name    "Castle_UpStairsL1"
  flags   RAW
  include "assets/tiles/castle/UpStairsL1.551"
endseg
beginseg
  name    "Castle_UpStairsL2"
  flags   RAW
  include "assets/tiles/castle/UpStairsL2.551"
endseg
beginseg
  name    "Castle_UpStairsR1"
  flags   RAW
  include "assets/tiles/castle/UpStairsR1.551"
endseg
beginseg
  name    "Castle_UpStairsR2"
  flags   RAW
  include "assets/tiles/castle/UpStairsR2.551"
endseg
beginseg
  name    "Castle_UpTBottom"
  flags   RAW
  include "assets/tiles/castle/UpTBottom.551"
endseg
beginseg
  name    "Castle_UpTLeft"
  flags   RAW
  include "assets/tiles/castle/UpTLeft.551"
endseg
beginseg
  name    "Castle_UpTop"
  flags   RAW
  include "assets/tiles/castle/UpTop.551"
endseg
beginseg
  name    "Castle_UpTRight"
  flags   RAW
  include "assets/tiles/castle/UpTRight.551"
endseg
beginseg
  name    "Castle_UpTTop"
  flags   RAW
  include "assets/tiles/castle/UpTTop.551"
endseg
beginseg
  name    "Castle_UpVertical"
  flags   RAW
  include "assets/tiles/castle/UpVertical.551"
endseg
beginseg
  name    "Castle_WallDown"
  flags   RAW
  include "assets/tiles/castle/WallDown.551"
endseg
beginseg
  name    "Castle_WallUp"
  flags   RAW
  include "assets/tiles/castle/WallUp.551"
endseg
beginseg
  name    "Castle_WallWater"
  flags   RAW
  include "assets/tiles/castle/WallWater.551"
endseg
beginseg
  name    "Castle_Water"
  flags   RAW
  include "assets/tiles/castle/Water.551"
endseg
beginseg
  name    "Castle_UpAlone"
  flags   RAW
  include "assets/tiles/castle/UpAlone.551"
endseg
beginseg
  name    "Castle_Pillar"
  flags   RAW
  include "assets/tiles/castle/Pillar.551"
endseg
beginseg
  name    "Castle_WallPillar"
  flags   RAW
  include "assets/tiles/castle/WallPillar.551"
endseg

beginseg
  name    "Dungeon_DownStairsL"
  flags   RAW
  include "assets/tiles/dungeon/DownStairsL.551"
endseg
beginseg
  name    "Dungeon_DownStairsR"
  flags   RAW
  include "assets/tiles/dungeon/DownStairsR.551"
endseg
beginseg
  name    "Dungeon_Ground1"
  flags   RAW
  include "assets/tiles/dungeon/Ground1.551"
endseg
beginseg
  name    "Dungeon_Ground2"
  flags   RAW
  include "assets/tiles/dungeon/Ground2.551"
endseg
beginseg
  name    "Dungeon_Ground3"
  flags   RAW
  include "assets/tiles/dungeon/Ground3.551"
endseg
beginseg
  name    "Dungeon_UpCornerBL"
  flags   RAW
  include "assets/tiles/dungeon/UpCornerBL.551"
endseg
beginseg
  name    "Dungeon_UpCornerBR"
  flags   RAW
  include "assets/tiles/dungeon/UpCornerBR.551"
endseg
beginseg
  name    "Dungeon_UpCornerTL"
  flags   RAW
  include "assets/tiles/dungeon/UpCornerTL.551"
endseg
beginseg
  name    "Dungeon_UpCornerTR"
  flags   RAW
  include "assets/tiles/dungeon/UpCornerTR.551"
endseg
beginseg
  name    "Dungeon_UpCross"
  flags   RAW
  include "assets/tiles/dungeon/UpCross.551"
endseg
beginseg
  name    "Dungeon_UpDown"
  flags   RAW
  include "assets/tiles/dungeon/UpDown.551"
endseg
beginseg
  name    "Dungeon_UpHorizontal"
  flags   RAW
  include "assets/tiles/dungeon/UpHorizontal.551"
endseg
beginseg
  name    "Dungeon_UpLeft"
  flags   RAW
  include "assets/tiles/dungeon/UpLeft.551"
endseg
beginseg
  name    "Dungeon_UpRight"
  flags   RAW
  include "assets/tiles/dungeon/UpRight.551"
endseg
beginseg
  name    "Dungeon_UpStairsL"
  flags   RAW
  include "assets/tiles/dungeon/UpStairsL.551"
endseg
beginseg
  name    "Dungeon_UpStairsR"
  flags   RAW
  include "assets/tiles/dungeon/UpStairsR.551"
endseg
beginseg
  name    "Dungeon_UpTBottom"
  flags   RAW
  include "assets/tiles/dungeon/UpTBottom.551"
endseg
beginseg
  name    "Dungeon_UpTLeft"
  flags   RAW
  include "assets/tiles/dungeon/UpTLeft.551"
endseg
beginseg
  name    "Dungeon_UpTop"
  flags   RAW
  include "assets/tiles/dungeon/UpTop.551"
endseg
beginseg
  name    "Dungeon_UpTRight"
  flags   RAW
  include "assets/tiles/dungeon/UpTRight.551"
endseg
beginseg
  name    "Dungeon_UpTTop"
  flags   RAW
  include "assets/tiles/dungeon/UpTTop.551"
endseg
beginseg
  name    "Dungeon_UpVertical"
  flags   RAW
  include "assets/tiles/dungeon/UpVertical.551"
endseg
beginseg
  name    "Dungeon_WallDown"
  flags   RAW
  include "assets/tiles/dungeon/WallDown.551"
endseg
beginseg
  name    "Dungeon_WallUp"
  flags   RAW
  include "assets/tiles/dungeon/WallUp.551"
endseg
beginseg
  name    "Dungeon_WallWater"
  flags   RAW
  include "assets/tiles/dungeon/WallWater.551"
endseg
beginseg
  name    "Dungeon_UpAlone"
  flags   RAW
  include "assets/tiles/dungeon/UpAlone.551"
endseg
beginseg
  name    "Dungeon_Pillar"
  flags   RAW
  include "assets/tiles/dungeon/Pillar.551"
endseg
beginseg
  name    "Dungeon_WallPillar"
  flags   RAW
  include "assets/tiles/dungeon/WallPillar.551"
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
  name    "SceneryWindow04"
  flags   RAW
  include "assets/scenery/window04.551"
endseg
beginseg
  name    "SceneryWater"
  flags   RAW
  include "assets/scenery/water.551"
endseg
beginseg
  name    "SceneryBook01Left"
  flags   RAW
  include "assets/scenery/bookshelf_01_left.551"
endseg
beginseg
  name    "SceneryBook01Right"
  flags   RAW
  include "assets/scenery/bookshelf_01_right.551"
endseg
beginseg
  name    "SceneryBook02Left"
  flags   RAW
  include "assets/scenery/bookshelf_02_left.551"
endseg
beginseg
  name    "SceneryBook02Middle"
  flags   RAW
  include "assets/scenery/bookshelf_02_middle.551"
endseg
beginseg
  name    "SceneryBook02Right"
  flags   RAW
  include "assets/scenery/bookshelf_02_right.551"
endseg
beginseg
  name    "SceneryCarpetTL"
  flags   RAW
  include "assets/scenery/carpetTL.551"
endseg
beginseg
  name    "SceneryCarpetT"
  flags   RAW
  include "assets/scenery/carpetT.551"
endseg
beginseg
  name    "SceneryCarpetTR"
  flags   RAW
  include "assets/scenery/carpetTR.551"
endseg
beginseg
  name    "SceneryCarpetL"
  flags   RAW
  include "assets/scenery/carpetL.551"
endseg
beginseg
  name    "SceneryCarpetC"
  flags   RAW
  include "assets/scenery/carpetC.551"
endseg
beginseg
  name    "SceneryCarpetR"
  flags   RAW
  include "assets/scenery/carpetR.551"
endseg
beginseg
  name    "SceneryCarpetBL"
  flags   RAW
  include "assets/scenery/carpetBL.551"
endseg
beginseg
  name    "SceneryCarpetB"
  flags   RAW
  include "assets/scenery/carpetB.551"
endseg
beginseg
  name    "SceneryCarpetBR"
  flags   RAW
  include "assets/scenery/carpetBR.551"
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
  name    "Level00"
  flags   RAW
  include "assets/levels/level00.blv"
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
