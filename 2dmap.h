#ifndef _2DMAP_H_
#define _2DMAP_H_

#include "2dlibrary.h"

#define COLLISION_NONE			0
#define COLLISION_BLOCK			1
#define COLLISION_HURT			2
#define COLLISION_ACTION_NONE	0
#define COLLISION_ACTION_BLOCK	1
#define COLLISION_ACTION_SMASH	2
#define COLLISION_ACTION_PUSH	3

#define TILE_ACTION_NONE			-1
#define TILE_ACTION_CLOSED			0
#define TILE_ACTION_OPENED			1
#define TILE_ACTION_LEVER_CLOSED	0
#define TILE_ACTION_LEVER_INTER1	1
#define TILE_ACTION_LEVER_INTER2	2
#define TILE_ACTION_LEVER_OPENED	3

#define TILE_FOREGROUND_NEVER		-1
#define TILE_FOREGROUND_ALWAYS		1000

typedef struct Tile Tile;
typedef struct Map Map;
typedef struct MapRow MapRow;
typedef struct MapColumn MapColumn;
typedef struct CollisionBox CollisionBox;
typedef struct MapDraw MapDraw;
typedef struct Character Character;
typedef struct CharacterSides CharacterSides;

struct MapDraw {
	int			first_column;
	int			first_row;
	int			x;
	int			y;
};

struct MapRow {
	int			y;
	MapRow*		previous;
	MapRow*		next;
	Tile*		tile;
	int			state;
	MapRow*		related_row;
	void		(*action)(MapRow* );
	void		(*over)(MapRow* );
};

struct MapColumn {
	int			x;
	MapColumn*	previous;
	MapColumn*	next;
	MapRow*		first_row;
};

struct CollisionBox {
	int			start_x;
	int			start_y;
	int			end_x;
	int			end_y;
	int			action;
} ;

struct Tile {
	int			(*checkCollision)(CollisionBox*, MapRow*, int);
	Texture*	texture;
	Animation*	animation;
	int			(*action)(MapRow* );
	int			foreground;
} ; 

struct Map {
	int			nbr_columns;
	int			nbr_rows;
	int			nbr_tiles;
	int			max_columns;
	int			max_rows;
	int			max_tiles;
	int			column_width;
	int			row_height;
	int			max_column;
	int			max_row;
	MapColumn*	start;
	MapRow*		rows;
	MapColumn*	columns;
	Tile*		tiles;
};

struct CharacterSides {
	AnimatedSprite		attack;
	AnimatedSprite		defend;
	AnimatedSprite		hurt;
	AnimatedSprite		idle;
	AnimatedSprite		walk;
	AnimatedSprite		dead;
	int					(*collision)(Map*, int, int, int );
	int					(*action)(Map*, int, int, int );
	int					(*push)(Map*, int, int, int );
};

struct Character {
	CharacterSides		back;
	CharacterSides		front;
	CharacterSides		left;
	CharacterSides		right;
	Animation			animation;
	CharacterSides*		side;
	int					x;
	int					y;
	int					move_x;
	int					move_y;
	int					status;	// playing or not
	int					life;
	int					state;
	int					keys;
	Character*			next;
	Character*			previous;
	int					(*collision)(Map*, int, int, int );
};


void freeMap(Map* map);
void initMap(Map* map, int columns, int rows, int tiles, int width, int height);
void appendMapColumn(Map* map, MapColumn* new_column);
MapColumn* findMapColumn(Map* map, int x);
MapColumn* nextMapColumn(Map* map, int x);
MapRow* findMapRow(MapRow* row, int y);
MapRow* sortRows(Map* map, MapRow* row);
MapRow* sortMap(Map* map);
void appendMapRow(MapColumn* column, MapRow* row);
MapRow* nextMapRow(Map* map, int y, Tile* tile);
void drawMap(Map* map, Character* character);
MapDraw checkMapPosition(Map* map, int x, int y);

int characterIsInTile(Map* map, Character* character, int tile_x, int tile_y);

int testCollisionBox(Map*, CollisionBox*, int, int, int);
int	collisionWall(CollisionBox*, MapRow*, int);
int	collisionNone(CollisionBox*, MapRow*, int);

int checkIntersection(int start1, int end1, int start2, int end2);

void actionRow(MapRow* );

#endif /* _2DMAP_H_*/
