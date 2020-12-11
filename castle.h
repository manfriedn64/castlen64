#include "2dlibrary.h"
#include "2dmap.h"
#include "audio.h"

#ifndef _PLATFORM_H_
#define _PLATFORM_H_


#define CHARACTER_IDLE			0
#define CHARACTER_WALK			1
#define CHARACTER_START_JUMP	3
#define CHARACTER_JUMP			4
#define CHARACTER_FALL			5
#define CHARACTER_ATTACK		6
#define CHARACTER_DEFEND		7


#define COLLISION_NONE			0
#define COLLISION_BLOCK			1
#define COLLISION_HURT			2
#define COLLISION_WEAKNESS		3
#define COLLISION_ACTION_NONE	0
#define COLLISION_ACTION_BLOCK	1
#define COLLISION_ACTION_SMASH	2

#define SIDE_LEFT				0
#define SIDE_RIGHT				1
#define SIDE_FRONT				2
#define SIDE_BACK				3


#define GAMESTATE_NORMAL		0
#define GAMESTATE_LOADING		1


typedef struct Level Level;
typedef struct LevelMap LevelMap;
typedef struct Game Game;
typedef struct GameMenu GameMenu;

struct LevelMap {
	int			width;
	int			height;
	char*		tiles;
	u32*	    pointer;
};
struct Level {
	char		name[40];
	void		(*loadLevel)();
	void		(*loadControls)();
	void		(*drawScreen)();
	LevelMap	map;
}; 

struct Game {
	int			state;
	Level*		level;
	int			lifes;
	Bool		godmod;
	Character	character;
	int			scale;
	Bool 		rumble;
	int			speed;
} ;

struct RomSegment {
	u32		start;
	u32		end;
};

Bool externalDevicesInit(int);

int	collisionScenery(CollisionBox* collision);
int	collisionBloc(CollisionBox* collision);
int	collisionBlocFloat(CollisionBox* collision);

void drawScreen();
void updateLevel();
void drawLevel();

void updateRez();

void setup_640();
void platformInit_640();
void setup_320();
void platformInit_320();
void platformInit();


void initLevel_00();
void initShip(int x, int y, int angle);
void initGame();
void checkControls();
void drawDebug();

int collisionBigDoor(CollisionBox* , MapRow*, int);
int collisionTrap(CollisionBox* , MapRow*, int);
int collisionButton(CollisionBox* , MapRow*, int);
int collisionKey(CollisionBox* , MapRow*, int);
int collisionDoor(CollisionBox* , MapRow*, int);
int collisionCandelier(CollisionBox* , MapRow*, int);
int collisionPillar(CollisionBox* , MapRow*, int);
int collisionWallPillar(CollisionBox* , MapRow*, int);
int collisionLever(CollisionBox* , MapRow* , int );
int collisionBarrel(CollisionBox* , MapRow* , int );
int collisionStairs(CollisionBox* , MapRow* , int );

int ratCollisionRight(Map* , int , int , int );
int ratCollisionLeft(Map* , int , int , int );
int ratCollisionBack(Map* , int , int , int );
int ratCollisionFront(Map* , int , int , int );

int testCollisionFeet(Map* , int , int , int );
int testCollisionActionRight(Map* , int , int , int );
int testCollisionActionLeft(Map* , int , int , int );
int testCollisionActionBack(Map* , int , int , int );
int testCollisionActionFront(Map* , int , int , int );
int testCollisionPushRight(Map* , int , int , int );
int testCollisionPushLeft(Map* , int , int , int );
int testCollisionPushBack(Map* , int , int , int );
int testCollisionPushFront(Map* , int , int , int );

void setBarrelToKey(MapRow*);
void level01End(MapRow *);

void setFrameBuffer32b();
void setFrameBuffer16b();

Game					game;
Texture					texture[300];
Animation				animations[30];
AnimatedSprite			animated_sprites[30];
Level					levels[10];
u64						time_lastmove;
MusicTrack				music_tracks[3];

Map						map;

#endif /* _PLATFORM_H_*/