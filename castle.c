#include <nusys.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include "audio.h"
#include "main.h"
#include "castle.h"
#include "intro.h"
#include "menu.h"
#include "level00.h"
#include "level01.h"
#include "2dlibrary.h"
#include "level_shared.h"
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
	levels[3] = (Level){"Start Level", initLevel_00, updateLevel, drawLevel, NULL};
	levels[4] = (Level){"Level One", initLevel_01, updateLevel, drawLevel, NULL};

	game.state = GAMESTATE_NORMAL;
	game.speed = 4;
	game.rumble = False;
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
	int column, row, i_value;
	MapRow* p_row;
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
			i_value = (int)value;
			
			
			if (i_value > 250) {
				appendMapRow(findMapColumn(map, column), nextMapRow(map, row, &map->tiles[11]));
				p_row = nextMapRow(map, row, &map->tiles[11]);
				if (i_value == 253) { // door with key
					tile = &map->tiles[78];
				}
				else if (i_value == 254) {// pot with key
					tile = &map->tiles[54];
					p_row->action = setRowToKey;
				}
				else if (i_value == 255) {// barrel with key
					tile = &map->tiles[46];
					p_row->action = setRowToKey;
				}
				else // I don't why I am here
					tile = &map->tiles[11];
				
				
				p_row->tile = tile;
			} else {
				if (i_value == 38 || i_value == 49 || i_value == 46 || i_value == 53 || i_value == 55 || i_value == 43) // pillar, candelier, barrel, pot, plant in pot or keu
					appendMapRow(findMapColumn(map, column), nextMapRow(map, row, &map->tiles[11]));
				if (i_value == 63 || i_value == 64 || i_value == 65 || i_value == 66 || i_value == 67) // book shelf
					appendMapRow(findMapColumn(map, column), nextMapRow(map, row, &map->tiles[11]));
				if (i_value == 57 || i_value == 51) // torch, small flag
					appendMapRow(findMapColumn(map, column), nextMapRow(map, row, &map->tiles[34]));
				if (i_value == 58 || i_value ==  52) // walltorch, big flag
					appendMapRow(findMapColumn(map, column), nextMapRow(map, row, &map->tiles[33]));
				if (i_value == 59 || i_value ==  60 || i_value == 61 || i_value ==  77) // windows
					appendMapRow(findMapColumn(map, column), nextMapRow(map, row, &map->tiles[33]));
				tile = &map->tiles[i_value];
				p_row = nextMapRow(map, row, tile);
			}
			appendMapRow(findMapColumn(map, column), p_row);
			
		}
	}
	//sortMap(map);
}


void setRowToKey(MapRow* row) {
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
		
		sprintf(conbuf, "keys      : %d  ", game.character.keys);
		nuDebConTextPos(0,2, 15);
		nuDebConCPuts(0, conbuf);
		
		/*sprintf(conbuf, "sound     : %d  ", MusHandleAsk(sndHandle[0]));
		nuDebConTextPos(0,2, 15);
		nuDebConCPuts(0, conbuf);
		sprintf(conbuf, "sound     : %d  ", MusHandleAsk(sndHandle[1]));
		nuDebConTextPos(0,2, 16);
		nuDebConCPuts(0, conbuf);*/
		
		sprintf(conbuf, "RamLeft : %d  ", (u32)my2dlibrary.texturePointer - (u32)_codeSegmentEnd);
		nuDebConTextPos(0,19,3);
		nuDebConCPuts(0, conbuf);
		sprintf(conbuf, "Display list  : %d   ", (gfxListPtr - gfxListStartPtr));
		nuDebConTextPos(0,19,4);
		nuDebConCPuts(0, conbuf);


	}
}

int collisionGround(CollisionBox* collision, MapRow* row, int is_character) {
	if (row->over)
		row->over(row);
	return COLLISION_NONE;	
}

int collisionStairs(CollisionBox* collision, MapRow* row, int is_character) {
	if (row->over)
		row->over(row);
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

int collisionDoorKeyLeft(CollisionBox* collision, MapRow* row, int is_character) {
	if (row->state == TILE_ACTION_CLOSED && checkIntersection(collision->start_x, collision->end_x, 0, 64) && checkIntersection(collision->start_y, collision->end_y, 0, 40))
		if (collision->action == COLLISION_ACTION_BLOCK)
			return COLLISION_BLOCK;	
		else if (collision->action == COLLISION_ACTION_PUSH && row->state == TILE_ACTION_CLOSED && game.character.keys > 0) {
			game.character.keys--;
			row->state = TILE_ACTION_OPENED;
			nuContRmbStart(0, 256, 30);
			actionRow(row);
		}
	if (checkIntersection(collision->start_x, collision->end_x, 0, 16) && checkIntersection(collision->start_y, collision->end_y, 0, 40))
			return COLLISION_BLOCK;	
	return COLLISION_NONE;
}

int collisionDoorKeyRight(CollisionBox* collision, MapRow* row, int is_character) {
	if (row->state == TILE_ACTION_CLOSED && checkIntersection(collision->start_x, collision->end_x, 0, 64) && checkIntersection(collision->start_y, collision->end_y, 0, 40))
		if (collision->action == COLLISION_ACTION_BLOCK)
			return COLLISION_BLOCK;	
		else if (collision->action == COLLISION_ACTION_PUSH && row->state == TILE_ACTION_CLOSED && game.character.keys > 0) {
			game.character.keys--;
			row->state = TILE_ACTION_OPENED;
			nuContRmbStart(0, 256, 30);
			actionRow(row);
		}
	if (checkIntersection(collision->start_x, collision->end_x, 46, 64) && checkIntersection(collision->start_y, collision->end_y, 0, 40))
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