#include <nusys.h>
#include "main.h"
#include "castle.h"
#include "intro.h"
#include "audio.h"
#include "2dlibrary.h"
#include "assets/music/all.h"

extern Game game;
extern Texture texture[300];
extern Level levels[10];

float ratio[3];
int step;

NUContData			contData[NU_CONT_MAXCONTROLLERS];
u8 					contPattern;


void initExpansionPak() {
	my2dlibrary.texturePointer = (u32*)(0x80400000 - (640*480*2*2));
	my2D_loadTexture(&texture[0], (u32)_BgExpansionPakSegmentRomStart, (u32)_BgExpansionPakSegmentRomEnd, 640, 480);
}
void drawExpansionPak() {
	Coordinates		coordinates;
	coordinates = (Coordinates){0, 0, &texture[0], texture[0].width, texture[0].height};
	my2D_drawBackGroundCoordinates(&coordinates, 0, 0, 0);
}

void initIntro() {
	my2dlibrary.texturePointer = (u32*)(0x80400000 - (640*480*2*2));
	my2D_loadTexture(&texture[0], (u32)_IntroJamSegmentRomStart, (u32)_IntroJamSegmentRomEnd, 152, 150);
	my2D_loadTexture(&texture[1], (u32)_IntroBrewLogoSegmentRomStart, (u32)_IntroBrewLogoSegmentRomEnd, 200, 220);
	my2D_loadTexture(&texture[2], (u32)_IntroBrewTitleSegmentRomStart, (u32)_IntroBrewTitleSegmentRomEnd, 328, 175);
	ratio[0] = 0.01;
	ratio[1] = 0.01;
	ratio[2] = 0.01;
	step = 0;
	loadAudio((u32)_MusicAllPtrSegmentRomStart, (u32)_MusicAllPtrSegmentRomEnd, (u32)_MusicAllSfxSegmentRomStart, (u32)_MusicAllSfxSegmentRomEnd, _MusicAllWbkSegmentRomStart);
	sndHandle[1] = nuAuStlSndPlayerPlay(FX_BOING);
}

void drawIntro() {
	Coordinates		coordinates;

	my2D_drawRectangle(0, 0, my2dlibrary.width-1, my2dlibrary.height-1, 1, 29, 169, 1);
	
	coordinates = (Coordinates){0, 0, &texture[2], texture[2].width, texture[2].height};
	my2D_drawBackGroundScaledCoordinates(&coordinates, 320 - (texture[2].width / 2) * ratio[0], 137 - (texture[2].height / 2) * ratio[0], ratio[2], ratio[2], 0);
	
	coordinates = (Coordinates){0, 0, &texture[1], texture[1].width, texture[1].height};
	my2D_drawBackGroundScaledCoordinates(&coordinates, 510 - (texture[1].width / 2) * ratio[1], 330 - (texture[1].height / 2) * ratio[1], ratio[1], ratio[1], 0);
	
	coordinates = (Coordinates){0, 0, &texture[0], texture[0].width, texture[0].height};
	my2D_drawBackGroundScaledCoordinates(&coordinates, 116 - (texture[0].width / 2) * ratio[0], 365 - (texture[0].height / 2) * ratio[0], ratio[0], ratio[0], 0);
	
	if (ratio[0] < 1.5 || step == 1) {
		if (step == 0) {
			ratio[0] += 0.01;
			ratio[1] += 0.01;
			ratio[2] += 0.01;
		} else if (ratio[0] > 1) {
			ratio[0] -= 0.01;
			ratio[1] -= 0.01;
			ratio[2] -= 0.01;			
		} else {
			ratio[0] = 1;
			ratio[1] = 1;
			ratio[2] = 1;
			step = 2;
			
		}		
	} else {
		externalDevicesInit(0);
		step = 1;
	}
	if (my2dlibrary.debug) {	
		sprintf(conbuf, "osTvType: %d  ", osTvType);
		nuDebConTextPos(0,2, 6);
		nuDebConCPuts(0, conbuf);
	}
}

void updateIntro() {
	nuContDataGetExAll(contData);
	if(contData[0].trigger & Z_TRIG) 
		if (my2dlibrary.debug) 
			my2dlibrary.debug = False;
		else
			my2dlibrary.debug = True;
	if (step == 2 && (contData[0].trigger & A_BUTTON || contData[0].trigger & B_BUTTON || contData[0].trigger & START_BUTTON)) {
		game.level = &levels[2];
		game.state = GAMESTATE_LOADING;
	}
}