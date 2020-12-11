#include <nusys.h>
#include <malloc.h>
#include "2dmap.h"

/*
Tile					tiles[55];
MapColumn*				map_start;
MapColumn				map_columns[40];
MapRow					map_rows[300];
int						nbr_columns;
int						nbr_rows;
*/

void freeMap(Map* map) {
	mt_free(map->rows);
	mt_free(map->columns);
	mt_free(map->tiles);
}

void initMap(Map* map, int columns, int rows, int tiles, int width, int height) {
	map->max_row        = 0;
	map->max_column     = 0;
	map->nbr_columns    = 0;
	map->nbr_rows       = 0;
	map->nbr_tiles      = 0;
	map->max_columns    = columns;
	map->max_rows       = rows;
	map->max_tiles      = tiles;
	map->column_width   = width;
	map->row_height     = height;
	map->rows           = (MapRow *) mt_malloc(rows * sizeof(MapRow));
	map->columns        = (MapColumn *) mt_malloc(columns * sizeof(MapColumn));
	map->tiles          = (Tile *) mt_malloc(tiles * sizeof(Tile));
	map->start          = NULL;
}


MapColumn* findMapColumn(Map* map, int x) {
	MapColumn*	tmp_column = map->start;
	while (tmp_column) {
		if (tmp_column->x != x)
			tmp_column = tmp_column->next;
		else 
			break;
	}
	if (!tmp_column || tmp_column->x != x)
		return NULL;
	else
		return tmp_column;
}

void appendMapColumn(Map* map, MapColumn* new_column) {
	MapColumn*	tmp_column;
	if (map->start == NULL) {
		map->start = new_column;
		return;
	}
	tmp_column = map->start;
	while (tmp_column->next) {
		tmp_column = tmp_column->next;
	}
	tmp_column->next = new_column;
	return;
}

MapColumn* nextMapColumn(Map* map, int x) {
	MapColumn* column;
	if (map->nbr_columns < map->max_columns) {
		map->columns[map->nbr_columns] = (MapColumn){x, NULL, NULL, NULL};
		if (x > map->max_column)
			map->max_column = x;
		return &map->columns[map->nbr_columns++];
	}
	return NULL;
}

MapRow* findMapRow(MapRow* row, int y) {
	MapRow* tmp_row = row;
	while(tmp_row) {
		if (tmp_row->y != y)
			tmp_row = tmp_row->next;
		else 
			break;
	}
	if (!tmp_row || tmp_row->y != y)
		return NULL;
	else
		return tmp_row;
		
}

void appendMapRow(MapColumn* column, MapRow* row) {
	MapRow* last_row = column->first_row;
	if (column->first_row == NULL) {
		column->first_row = row;
		return;
	}
	while (last_row->next)
		last_row = last_row->next;
	last_row->next = row;
	row->previous = last_row;
}

MapRow* nextMapRow(Map* map, int y, Tile* tile) {
	if (map->nbr_rows < map->max_rows) {
		map->rows[map->nbr_rows] = (MapRow){y, NULL, NULL, tile, TILE_ACTION_NONE, NULL};
		map->rows[map->nbr_rows].state = TILE_ACTION_CLOSED;
		map->rows[map->nbr_rows].related_row = NULL;
		map->rows[map->nbr_rows].action = NULL;
		map->rows[map->nbr_rows].over = NULL;
		if (y > map->max_row)
			map->max_row = y;
		return &map->rows[map->nbr_rows++];
	}
	return NULL;
}

MapDraw checkMapPosition(Map* map, int x, int y) {
	MapDraw position;
	position.x = x - my2dlibrary.width / 2;
	if (position.x + my2dlibrary.width > (map->max_column+1) * map->column_width)
		position.x = (map->max_column +1)* map->column_width - my2dlibrary.width;
	if (position.x < 0)
		position.x = 0;
	position.y = y - my2dlibrary.height / 2;	
	if (position.y + my2dlibrary.height > (map->max_row+1) * map->row_height)
		position.y = (map->max_row +1)* map->row_height - my2dlibrary.height;
	if (position.y < 0)
		position.y = 0;
	position.first_column = position.x / map->column_width - 1;
	if (position.first_column + my2dlibrary.width / map->column_width > map->max_column)
		position.first_column = map->max_column - 20;
	if (position.first_column < 0)
		position.first_column = 0;
	position.first_row = position.y / map->row_height - 1;
	if (position.first_row +  my2dlibrary.height / map->row_height > map->max_row)
		position.first_row = map->max_row - my2dlibrary.height / map->row_height;
	if (position.first_row < 0)
		position.first_row = 0;
	
	return position;
}

MapRow* sortMap(Map* map) {
	MapColumn* column = map->start;
	while (column) {
		column->first_row = sortRows(map, column->first_row);
		column = column->next;
	}
}


MapRow* sortRows(Map* map, MapRow* row) {
	Bool sorted = False;
	MapRow* start = row;
	MapRow* current;
	MapRow* next;
	MapRow* previous;
	while(start->previous) // make sure to start from the first element
		start = start->previous;
	while (!sorted) { // loop as long as we did not make any swap
		current = start;
		sorted = True;
		while(current->next) {
			if (current->y * map->row_height + current->tile->foreground  > current->next->y * map->row_height + current->next->tile->foreground) {
				next = current->next;
				previous = current->previous;
				current->next = current->next->next;
				next->next = current;
				next->previous = previous;
				if (previous) 
					previous->next = next;
				else
					start = next;
				current->previous = next;
				sorted = False;
			}
			else
				current = current->next;
		}
	}
	// if we are here it means no item was swapped which means it's sorted
	return start;
}

Character* sortCharacters(Character* character) {
	Bool sorted = False;
	Character* start = character;
	Character* next;
	Character* previous;
	Character* current = character;
	while(current) {
		current->animation.x = current->x - current->animation.sprite->width / 2;
		current->animation.y = current->y - current->animation.sprite->height / 2;
		current = current->next;
	}
	while(start->previous) // make sure to start from the first element
		start = start->previous;
	while (!sorted) { // loop as long as we did not make any swap
		current = start;
		sorted = True;
		while(current->next) {
			if (current->animation.y + current->animation.sprite->height > current->next->animation.y + current->next->animation.sprite->height) {
				next = current->next;
				previous = current->previous;
				current->next = current->next->next;
				next->next = current;
				next->previous = previous;
				if (previous) 
					previous->next = next;
				else
					start = next;
				current->previous = next;
				sorted = False;
			}
			else
				current = current->next;
		}
	}
	// if we are here it means no item was swapped which means it's sorted
	return start;
}

int characterIsInTile(Map* map, Character* character, int tile_x, int tile_y) {
	int character_tile_x, character_tile_y ;
	for (character_tile_x = character->animation.x / map->column_width; character_tile_x <= (character->animation.x + character->animation.sprite->width ) / map->column_width; character_tile_x++) {
		for (character_tile_y = character->animation.y / map->row_height; character_tile_y <= (character->animation.y + character->animation.sprite->height ) / map->row_height; character_tile_y++) {
			if (character_tile_y == tile_y && character_tile_x == tile_x)
				return 1;
		}
	}
	return 0;
}

void drawMap(Map* map, Character* character) {
	Box				box;
	Coordinates		coordinates;
	MapDraw			position = checkMapPosition(map, character->x, character->y);
	MapColumn*	current_column	= map->start;
	MapRow*		current_row		= NULL;
	Character* characters = sortCharacters(character);
	//Character* characters = character;
	Character* tmp_characters = characters;
	while(tmp_characters) {
		tmp_characters->animation.x = tmp_characters->x - tmp_characters->animation.sprite->width / 2;
		tmp_characters->animation.y = tmp_characters->y - tmp_characters->animation.sprite->height / 2;
		tmp_characters = tmp_characters->next;
	}
	while (current_column) {
		// check the column is in the visible area
		if (current_column->x >= position.first_column && current_column->x < position.first_column + my2dlibrary.width / map->column_width + 2) {
			current_row = current_column->first_row;
			while (current_row) {
				//do not draw a row if it was an object already picked
				if (current_row->state != TILE_ACTION_CLOSED && current_row->tile->texture) {
					current_row = current_row->next;
					continue;
				}
				// check the row is in the visible area
				if (current_row->y >= position.first_row && current_row->y < position.first_row + my2dlibrary.height / map->row_height + 5) {
					box.start_x = current_column->x * map->column_width;
					box.start_y = current_row->y * map->row_height;
					box.end_x = (current_column->x+1) * map->column_width - 1;
					box.end_y = (current_row->y+1) * map->row_height - 1;
					tmp_characters = characters;
					while(tmp_characters) {
						if (characterIsInTile(map, tmp_characters, current_column->x, current_row->y) && tmp_characters->animation.y + tmp_characters->animation.sprite->height <= current_row->y * map->row_height + current_row->tile->foreground) {
							my2D_animationCropp(&tmp_characters->animation, &box, position.x, position.y);
						}
						tmp_characters = tmp_characters->next;
					}
					
					if (current_row->tile->texture) {
						coordinates = (Coordinates){0, 0, current_row->tile->texture, current_row->tile->texture->width, current_row->tile->texture->height};
						if (current_column->x * map->column_width - position.x + coordinates.width > my2dlibrary.width)
							coordinates.width = my2dlibrary.width - current_column->x * map->column_width + position.x;
						
						/*if (current_row->y * map->row_height - position.y + coordinates.height > my2dlibrary.height)
							coordinates.height = my2dlibrary.height - current_row->y * map->row_height + position.y;*/
						if (coordinates.width > 0 && current_column->x * map->column_width - position.x < my2dlibrary.width && 
							coordinates.height > 0 && current_row->y * map->row_height - position.y - coordinates.texture->height + map->row_height < my2dlibrary.height ) {
							my2D_drawBackGroundCoordinates(&coordinates, (current_column->x * map->column_width) - position.x, current_row->y * map->row_height - position.y - coordinates.texture->height + map->row_height, 0);
						}
					} else {
						current_row->tile->animation->x = (current_column->x * map->column_width) - position.x;
						current_row->tile->animation->y = current_row->y * map->row_height - position.y - current_row->tile->animation->sprite->height + map->row_height;
						if (!current_row->tile->animation->sprite->loop)
							current_row->tile->animation->frame = current_row->state;
						my2D_animationDraw(current_row->tile->animation);
					}
					
					tmp_characters = characters;
					while(tmp_characters) {
						if (characterIsInTile(map, tmp_characters, current_column->x, current_row->y) && tmp_characters->animation.y + tmp_characters->animation.sprite->height > current_row->y * map->row_height + current_row->tile->foreground) {
							my2D_animationCropp(&tmp_characters->animation, &box, position.x, position.y);
						}
						tmp_characters = tmp_characters->next;
					}
				}
				current_row = current_row->next;
			}
		}
		current_column = current_column->next;
	}
	return;
}

int	collisionWall(CollisionBox* collision, MapRow* row, int is_character) {
	if (collision->action == COLLISION_ACTION_BLOCK)
		return COLLISION_BLOCK;	
	return COLLISION_NONE;
}

int	collisionNone(CollisionBox* collision, MapRow* row, int is_character) {
	return COLLISION_NONE;
}

int testCollisionBox(Map* map, CollisionBox* collision, int x, int y, int is_character) {
	MapColumn* column = map->start;
	MapRow* row;
	CollisionBox tmp_box;
	int block_x, block_y, res, temp;
	for (block_x = (x + collision->start_x) / map->column_width; block_x <= (x + collision->end_x) / map->column_width; block_x++) {
		column = findMapColumn(map, block_x);
		if (column) 
			for (block_y = (y + collision->start_y) / map->row_height; block_y <= (y + collision->end_y) / map->row_height; block_y++) {
				row = column->first_row;
				while (row = findMapRow(row, block_y)) {
					tmp_box.start_x = (x + collision->start_x) - column->x * map->column_width ;
					tmp_box.start_y = (y + collision->start_y) - row->y * map->row_height ;
					tmp_box.end_x = (x + collision->end_x) - column->x * map->column_width ;
					tmp_box.end_y = (y + collision->end_y) - row->y * map->row_height ;
					tmp_box.action = collision->action;
					if (tmp_box.start_x < 0)
						tmp_box.start_x = 0;
					if (tmp_box.start_y < 0)
						tmp_box.start_y = 0;
					if (tmp_box.end_x > map->column_width - 1)
						tmp_box.end_x = map->column_width - 1;
					if (tmp_box.end_y > map->row_height - 1)
						tmp_box.end_y = map->row_height - 1;
					res = row->tile->checkCollision(&tmp_box, row, is_character);
					if (res)
						return res;
					row = row->next;
				}
			}
	}
	return COLLISION_NONE;
}

int checkIntersection(int start1, int end1, int start2, int end2) {
	if ((start1 <= start2 && end1 >= end2) || (start1 >= start2 && start1 <= end2) || (end1 >= start2 && end1 <= end2))
		return 1;
	return 0;
}

void actionRow(MapRow* row) {
	MapRow* tmp_row = row;
	while (tmp_row->related_row) {
		tmp_row->related_row->state = TILE_ACTION_OPENED;
		tmp_row = tmp_row->related_row;
		if (tmp_row == row) // avoid infinite loop (ex: big doors, each side is connected with the other to open both of them)
			break;
	}
}
