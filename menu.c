#include <nusys.h>
#include "main.h"
#include "castle.h"
#include "menu.h"
#include "2dlibrary.h"
#include "assets/music/chance.h"

extern Game game;
extern Texture texture[300];
extern u64 time_lastmove;
int current_frame;
int menu;
int menu_selected;
extern Gfx*	gfxListPtr;
ScrollingBackground backgrounds[7];

NUContData			contData[NU_CONT_MAXCONTROLLERS];
u8 					contPattern;

void initMainMenu() {
	my2dlibrary.texturePointer = (u32*)(0x80800000 - (640*480*2*2));
	my2D_loadTexture(&texture[0], (u32)_walk01SegmentRomStart, (u32)_walk01SegmentRomEnd, 228, 417);
	my2D_loadTexture(&texture[1], (u32)_walk02SegmentRomStart, (u32)_walk02SegmentRomEnd, 228, 417);
	my2D_loadTexture(&texture[2], (u32)_walk03SegmentRomStart, (u32)_walk03SegmentRomEnd, 228, 417);
	my2D_loadTexture(&texture[3], (u32)_walk04SegmentRomStart, (u32)_walk04SegmentRomEnd, 228, 417);
	my2D_loadTexture(&texture[4], (u32)_walk05SegmentRomStart, (u32)_walk05SegmentRomEnd, 228, 417);
	my2D_loadTexture(&texture[5], (u32)_walk06SegmentRomStart, (u32)_walk06SegmentRomEnd, 228, 417);
	
	my2D_loadTexture(&texture[6],  (u32)_MainMenuBackground01SegmentRomStart, (u32)_MainMenuBackground01SegmentRomEnd, 640, 237);
	my2D_loadTexture(&texture[7],  (u32)_MainMenuBackground02SegmentRomStart, (u32)_MainMenuBackground02SegmentRomEnd, 640, 267);
	my2D_loadTexture(&texture[8],  (u32)_MainMenuBackground03SegmentRomStart, (u32)_MainMenuBackground03SegmentRomEnd, 640, 251);
	my2D_loadTexture(&texture[9],  (u32)_MainMenuBackground04SegmentRomStart, (u32)_MainMenuBackground04SegmentRomEnd, 640, 64);
	my2D_loadTexture(&texture[10],  (u32)_MainMenuBackground05SegmentRomStart, (u32)_MainMenuBackground05SegmentRomEnd, 640, 171);
	my2D_loadTexture(&texture[11], (u32)_MainMenuBackground06SegmentRomStart, (u32)_MainMenuBackground06SegmentRomEnd, 640, 36);
	my2D_loadTexture(&texture[12], (u32)_MainMenuBackground07SegmentRomStart, (u32)_MainMenuBackground07SegmentRomEnd, 640, 153);
	my2D_loadTexture(&texture[13], (u32)_MainMenuBackground08SegmentRomStart, (u32)_MainMenuBackground08SegmentRomEnd, 640, 166);
	
	my2D_loadTexture(&texture[14], (u32)_MainMenuTitleSegmentRomStart, (u32)_MainMenuTitleSegmentRomEnd, 432, 75);
	my2D_loadTexture(&texture[15], (u32)_MainMenuMenuSegmentRomStart, (u32)_MainMenuMenuSegmentRomEnd, 256, 200);
	my2D_loadTexture(&texture[16], (u32)_MainMenuHeadSegmentRomStart, (u32)_MainMenuHeadSegmentRomEnd, 40, 46);
	
	backgrounds[0] = (ScrollingBackground){0, 0,   0, 0, 0.1, &texture[13], &backgrounds[1]};
	backgrounds[1] = (ScrollingBackground){0, 100, 0, 0, 0.2, &texture[12], &backgrounds[2]};
	backgrounds[2] = (ScrollingBackground){0, 120, 0, 0, 0.05, &texture[11], &backgrounds[3]};
	backgrounds[3] = (ScrollingBackground){0, 90, 0, 0, 0.2, &texture[10], &backgrounds[4]};
	backgrounds[4] = (ScrollingBackground){0, 200, 0, 0, 0.3, &texture[9], &backgrounds[5]};
	backgrounds[5] = (ScrollingBackground){0, 180, 0, 0, 0.5, &texture[8], &backgrounds[6]};
	backgrounds[6] = (ScrollingBackground){0, 210, 0, 0, 0.7, &texture[7], &backgrounds[7]};
	backgrounds[7] = (ScrollingBackground){0, 243, 0, 0, 1,   &texture[6], NULL};
	//backgrounds[0] = (ScrollingBackground){0, 243, 0, 0, 1,   &texture[6], NULL};
	
	loadAudio((u32)_MusicChancePtrSegmentRomStart, (u32)_MusicChancePtrSegmentRomEnd, (u32)_MusicChanceSfxSegmentRomStart, (u32)_MusicChanceSfxSegmentRomEnd, _MusicChanceWbkSegmentRomStart);
	music_tracks[0] = (MusicTrack){SMP_CHANCE, 37, 0, &music_tracks[0]};
	music_tracks[0].next_track = &music_tracks[1];
	current_music = &music_tracks[0];

	current_frame = 0;
	menu_selected = 0;
	menu = 0;
	/*animated_sprites[0] = (AnimatedSprite){&texture[0], 200, 385,  6, 1};
	animations[0] = (Animation){&animated_sprites[0], 0, 0, 0, ANIMATION_STATUS_RUNNING, 0, 0, 0};*/
	time_lastmove = osGetTime();
}

void drawMainMenu() {
	Coordinates		coordinates;
	
	checkMusic(current_music);
	
	my2D_drawRectangle(0, 0, my2dlibrary.width-1, 260, 255, 241, 201, 1);
	
	my2D_drawRectangle(0, 260, my2dlibrary.width-1, my2dlibrary.height-1, 104, 69, 27, 1);
	
	drawScrollingBackground(&backgrounds[0]);
	
	coordinates = (Coordinates){0, 0, &texture[current_frame], 228, 417};
	my2D_drawBackGroundCoordinates(&coordinates, 20, 50, 0);
	
	
	coordinates = (Coordinates){0, 0, &texture[14], 432, 75};
	my2D_drawBackGroundCoordinates(&coordinates, 188, 25, 0);
	
	coordinates = (Coordinates){0, menu * 100, &texture[15], 256, 100};
	my2D_drawBackGroundCoordinates(&coordinates, 364, 240, 0);
	
	coordinates = (Coordinates){0, 0, &texture[16], 40, 46};
	my2D_drawBackGroundCoordinates(&coordinates, 320, menu_selected * 50 + 238, 0);
	

	
	//out_rgb[0] * 2048 + out_rgb[1] *   64 + out_rgb[2] *    2 + out_rgb[3]
	
	
	//my2D_animationDraw(&animations[0]);
	
	if (OS_CYCLES_TO_USEC(osGetTime() - time_lastmove) / (1000000 / 60) > 1) {
		scrollScrollingBackground(&backgrounds[0], 1, 0);
	}
	if (OS_CYCLES_TO_USEC(osGetTime() - time_lastmove) / (1000000 / 15) > 1) {
		//my2D_animationNextFrame(&animations[0]);
		time_lastmove = osGetTime();
		current_frame++;
		if (current_frame > 5)
			current_frame = 0;
	}
	
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
		sprintf(conbuf, "RamLeft : %d  ", (u32)my2dlibrary.texturePointer - (u32)_codeSegmentEnd);
		nuDebConTextPos(0,19,3);
		nuDebConCPuts(0, conbuf);
		sprintf(conbuf, "Code    : %X  ", (u32)_codeSegmentEnd);
		nuDebConTextPos(0,19,4);
		nuDebConCPuts(0, conbuf);
		sprintf(conbuf, "osTvType: %d  ", osTvType);
		nuDebConTextPos(0,2, 6);
		nuDebConCPuts(0, conbuf);
	}

}

void updateMenu() {
	nuContDataGetExAll(contData);
	
	if(contData[0].trigger & Z_TRIG) 
		if (my2dlibrary.debug) 
			my2dlibrary.debug = False;
		else
			my2dlibrary.debug = True;
	
	if (contData[0].trigger & U_JPAD) {
		menu_selected -= 1;
	}
	if (contData[0].trigger & D_JPAD) {
		menu_selected += 1;
	}
	if (menu_selected < 0)
		menu_selected = 1;
	if (menu_selected > 1)
		menu_selected = 0;
	
	if (contData[0].trigger & A_BUTTON || contData[0].trigger & START_BUTTON)
		if (menu == 0)
			if (menu_selected == 0) {
				stopMusic();
				game.level = &levels[3];
				game.state = GAMESTATE_LOADING;				
			} else {
				menu = 1;
				menu_selected = 0;
			}
		else {
		
		}
	if (contData[0].trigger & B_BUTTON && menu == 1) {
		menu = 0;
		menu_selected = 1;
	}
}