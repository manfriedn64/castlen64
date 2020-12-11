#ifndef _2DLIBRARY_
#define _2DLIBRARY_

#define MY2D_SPRITE_LIST_SIZE		10
#define MY2D_OBJ_LIST_SIZE			200
#define MY2D_MOVE_OBJECTS_LIMIT		10
#define MY2D_CENTER					-9999

#define MY2D_COLOR16				0
#define MY2D_COLOR32				1

#define GFX_LIST_NUM		1
#define GFX_LIST_SIZE		4096

#define ANIMATION_STATUS_RUNNING	0
#define ANIMATION_STATUS_ENDED		1

typedef struct ScrollingBackground ScrollingBackground;

typedef enum
{ False = 0, True = 1 }
Bool;

typedef struct {
	int		start_x;
	int		start_y;
	int		end_x;
	int		end_y;
} Box;

typedef struct {
	int 	red;
	int		green;
	int		blue;
} Rgb;

/* defining a texture */

typedef struct  {
	u32*	pointer32;
	u64*	pointer64;
	u32		romStart;
	u32		romEnd;
	u32		romSize;
	int		width;
	int		height;
	int		flip;
	Bool	malloc;
} Texture; 

typedef struct  {
	Texture*	texture;
	int			width;
	int			height;
	int			frames;
	Bool		loop;
} AnimatedSprite; 

typedef struct  {
	AnimatedSprite*		sprite;
	int					frame;
	int					x;
	int					y;
	//float				inertia;
	int					status;
	Bool				is_character;
	int					border_x;
	int					border_y;
	
} Animation;



typedef struct {
	int x;
	int y;
	Texture* texture;
	int width;
	int height;
} Coordinates;

typedef struct {
	Coordinates		coordinates;
	int				posX;
	int 			posY;
	int				destX;
	int 			destY;
	int				moveX;
	int				moveY;
} My2DMoveObject;

typedef struct {
	int 	width;
	int		height;
	int		ratio;
	u32		cycleType;
	u32*	currentTextureInTMEM;
	Rgb		rgb;
	u32*	texturePointer;
	uObjBg		background_list[MY2D_OBJ_LIST_SIZE];
	uObjBg*		background_pointer;
	int			background_count;
	uObjSprite	sprite_list[MY2D_SPRITE_LIST_SIZE];
	uObjSprite*	sprite_pointer;
	int			sprite_count;
	u32			colors;
	Bool		debug;
	int			frames;
} My2DLibrary;

struct ScrollingBackground {
	int						startX;
	int						startY;
	float					scrollX;
	float					scrollY;
	float					increment_ratio;
	Texture*				texture;
	ScrollingBackground*	nextBackground;
};

void 			my2D_init(int width, int height, u32* texturePointer);
void 			my2D_clear();

void 			my2d_moveObjectsClear();
My2DMoveObject* getNextMoveObjects();
Bool 			my2D_doMoveOject(My2DMoveObject* moveobject);

void			my2D_backgroundListClear();
uObjBg*			my2D_getNextBackground();
void			my2D_spriteListClear();
uObjSprite*		my2D_getNextSprite();

void my2D_drawSprite(Coordinates* coordinates, int x, int y);
void my2D_drawRepeatedSprite(Coordinates* coordinates, int x, int y, int exp_x, int exp_y);
void my2D_drawBackGroundCoordinates(Coordinates* coordinates, int x, int y, int line);
void my2D_drawBackGroundScaledCoordinates(Coordinates* coordinates, int x, int y, float ratio_x, float ratio_y, int line);
void my2D_drawFullBackGround(Texture* texture, int x, int y);

void my2D_drawRectangle(int start_x, int start_y, int end_x, int end_y, int red, int green, int blue, int isFilled);

void my2D_setColorFill(Rgb rgb);
void my2D_setCycleType(u32 newCycleType);

u32* my2D_loadSegmentToRam(u32 segmentStart, u32 segmentEnd);
void my2D_loadTexture(Texture* texture, u32 segmentStart, u32 segmentEnd, int width, int height);

void my2D_animationNextFrame(Animation* animation);
void my2D_animationCropp(Animation* , Box* , int, int);
void my2D_animationDraw(Animation* ) ;

void drawScrollingBackground(ScrollingBackground*);
void scrollScrollingBackground(ScrollingBackground*, int, int);

My2DLibrary	my2dlibrary;

My2DMoveObject		my2d_moveOjectsList[MY2D_MOVE_OBJECTS_LIMIT];
My2DMoveObject*		my2d_moveOjectsPointer;
int					my2d_moveOjectsCount;



#endif /* _2DLIBRARY_ */