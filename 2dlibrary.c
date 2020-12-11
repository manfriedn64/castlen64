#include  <nusys.h>

#include "2dlibrary.h"

Gfx rdpInit_dl[] = {
	gsDPPipeSync(),
	gsDPPipelineMode(G_PM_1PRIMITIVE),
	gsDPSetTextureLOD(G_TL_TILE),
	gsDPSetTextureLUT(G_TT_NONE),
	gsDPSetTextureDetail(G_TD_CLAMP),
	gsDPSetTexturePersp(G_TP_NONE),
	gsDPSetTextureFilter(G_TF_BILERP),
	gsDPSetTextureConvert(G_TC_FILT),
	gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
	gsDPSetCombineKey(G_CK_NONE),
	gsDPSetAlphaCompare(G_AC_NONE),
	gsDPSetColorDither(G_CD_DISABLE),
	gsDPSetScissor(G_SC_NON_INTERLACE, 0, 0,640, 480),
	gsSPEndDisplayList(),
};

Gfx spriteInit_dl[] = {
	gsDPPipeSync(),
	gsDPSetTexturePersp(G_TP_NONE),
	gsDPSetTextureLOD(G_TL_TILE),
	gsDPSetTextureLUT(G_TT_NONE),
	gsDPSetTextureConvert(G_TC_FILT),
	gsDPSetAlphaCompare(G_AC_THRESHOLD),
	gsDPSetBlendColor(0, 0, 0, 0x01),
	gsDPSetCombineMode(G_CC_DECALRGBA, G_CC_DECALRGBA),
	gsSPEndDisplayList(),
};

Gfx	gfxListBuf[GFX_LIST_NUM][GFX_LIST_SIZE];
u32	gfxListCnt = 0;
Gfx*	gfxListPtr;
Gfx*	gfxListStartPtr;

extern My2DLibrary my2dlibrary;

void gfxClearCfb()
{
	gSPSegment(gfxListPtr++, 0, 0x0);
	gDPSetColorImage(gfxListPtr++, G_IM_FMT_RGBA, my2dlibrary.colors, my2dlibrary.width, OS_K0_TO_PHYSICAL(nuGfxCfb_ptr)); 	
	my2D_drawRectangle(0, 0, my2dlibrary.width-1, my2dlibrary.height-1, 0, 64, 0, 1);
}

void gfxListBufferChange()
{
	gfxListStartPtr = &gfxListBuf[gfxListCnt][0];
	gfxListPtr = gfxListStartPtr;
	return;
}

void RCPInit() {
	/* Set the display list buffer. */
	gfxListBufferChange();
	gSPDisplayList(gfxListPtr++, rdpInit_dl);
    gDPSetDepthSource(gfxListPtr++, G_ZS_PRIM);
	/* Clear the screen. */

	my2D_clear();
	gfxClearCfb();
	
	gSPDisplayList(gfxListPtr++, rdpInit_dl);
	gSPDisplayList(gfxListPtr++, spriteInit_dl);
	gDPSetRenderMode(gfxListPtr++, G_RM_SPRITE, G_RM_SPRITE2);
	my2D_setCycleType(G_CYC_COPY);
	//gDPSetCycleType(gfxListPtr++, G_CYC_COPY);
	gDPSetTextureFilter(gfxListPtr++, G_TF_POINT);
	

}

void RCPEnd() {
	gDPFullSync(gfxListPtr++);
	gSPEndDisplayList(gfxListPtr++);
	
	if (my2dlibrary.debug) {
		nuGfxTaskStart(gfxListStartPtr, gfxListPtr - gfxListStartPtr, NU_GFX_UCODE_S2DEX,NU_SC_NOSWAPBUFFER);
		nuDebConDisp(NU_SC_NOSWAPBUFFER);
		nuDebTaskPerfBar1(1, 400, NU_SC_SWAPBUFFER);
	}
	else
		nuGfxTaskStart(gfxListStartPtr, gfxListPtr - gfxListStartPtr, NU_GFX_UCODE_S2DEX,NU_SC_SWAPBUFFER);
	
	
	my2dlibrary.frames++;
}



void my2D_clear() {
	my2dlibrary.currentTextureInTMEM	= NULL;
	my2dlibrary.cycleType				= NULL;
	my2dlibrary.rgb						= (Rgb){NULL, NULL, NULL};
	my2d_moveObjectsClear();
	my2D_backgroundListClear();
	my2D_spriteListClear();
}

void my2D_init(int width, int height, u32* texturePointer) {
	my2dlibrary.currentTextureInTMEM	= NULL;
	my2dlibrary.width					= width;
	my2dlibrary.height					= height;
	my2dlibrary.ratio					= 640 / width;
	my2dlibrary.cycleType				= NULL;
	my2dlibrary.rgb						= (Rgb){NULL, NULL, NULL};
	my2dlibrary.texturePointer			= texturePointer;
	my2dlibrary.debug					= False;
	my2dlibrary.colors					= G_IM_SIZ_16b;
	my2dlibrary.frames					= 0;
	return;
}

u32* my2D_loadSegmentToRam(u32 segmentStart, u32 segmentEnd) {
	my2dlibrary.texturePointer = (u32*)((u32)my2dlibrary.texturePointer - (segmentEnd - segmentStart));
	nuPiReadRom(segmentStart, my2dlibrary.texturePointer, segmentEnd - segmentStart);
	return my2dlibrary.texturePointer;
}

void my2D_loadTexture(Texture* texture, u32 segmentStart, u32 segmentEnd, int width, int height) {
	texture->romStart			= segmentStart;
	texture->romEnd				= segmentEnd;
	texture->pointer32			= my2D_loadSegmentToRam(segmentStart, segmentEnd);
	texture->pointer64			= (u64*)texture->pointer32;
	texture->width				= width;
	texture->height				= height;
	texture->malloc				= False;
	texture->flip				= 0;
}

/*
	void my2D_setCycleType(u32 newCycleType)
	
	change the RDP Cycle Type only if needed
	newCycleType = cycle type as defined in n64 sdk
*/
void my2D_setCycleType(u32 newCycleType) {
	if (my2dlibrary.cycleType != newCycleType) {
		my2dlibrary.cycleType = newCycleType;
		gDPPipeSync(gfxListPtr++);
		gDPSetCycleType(gfxListPtr++, newCycleType);
		my2dlibrary.currentTextureInTMEM = NULL;
	}
}

/*
	void my2D_setColorFill(Rgb rgb)
	
	change the RDP fill color only if needed
	rgb = red, green, blue values
	
*/
void my2D_setColorFill(Rgb rgb) {
	if (rgb.red != my2dlibrary.rgb.red || rgb.green != my2dlibrary.rgb.green || rgb.blue != my2dlibrary.rgb.blue) {
		gDPPipeSync(gfxListPtr++);
		gDPSetFillColor(gfxListPtr++, (GPACK_RGBA5551(rgb.red, rgb.green, rgb.blue, 1) << 16 | GPACK_RGBA5551(rgb.red, rgb.green, rgb.blue, 1)));
		my2dlibrary.rgb.red = rgb.red;
		my2dlibrary.rgb.green = rgb.green;
		my2dlibrary.rgb.blue = rgb.blue;
	}
}

void my2d_moveObjectsClear() {
	my2d_moveOjectsCount = -1;
}

My2DMoveObject* my2D_getNextMoveObjects() {
	if (my2d_moveOjectsCount < MY2D_MOVE_OBJECTS_LIMIT) {
		my2d_moveOjectsPointer = &my2d_moveOjectsList[my2d_moveOjectsCount++];
		return my2d_moveOjectsPointer;
	}
	return NULL;
}

Bool my2D_doMoveOject(My2DMoveObject* moveobject) {
	if (moveobject->destY == moveobject->posY && moveobject->destX == moveobject->posX)
		return True;
	else {
		if (moveobject->destX != moveobject->posX)
			moveobject->posX += moveobject->moveX;
		if (moveobject->destY != moveobject->posY)
			moveobject->posY += moveobject->moveY;
		return False;
	}
}


/*
	void my2D_drawRectangle(int start_x, int start_y, int end_x, int end_y, int red, int green, int blue, int isFilled)
	
	draw rectangle of the desired shape and color. This rectangle can be filled with color or not.
	start_x		= the x-coordinate of upper-left corner of rectangle
	start_y		= the y-coordinate of upper-left corner of rectangle
	end_x		= the x-coordinate of lower-right corner of rectangle
	end_y		= the y-coordinate of lower-right corner of rectangle
	red			= RGB red value
	green		= RGB green value
	blue		= RGB blue value
	isFilled	= is this rectangle filled with color or not ?
*/
void my2D_drawRectangle(int start_x, int start_y, int end_x, int end_y, int red, int green, int blue, int isFilled) {
	my2D_setCycleType(G_CYC_FILL);
	my2D_setColorFill((Rgb){red, green, blue});
	if (isFilled) {
		gDPFillRectangle(gfxListPtr++, start_x, start_y, end_x, end_y);
		/*gDPSetCycleType(gfxListPtr++, G_CYC_1CYCLE);
		gDPPipeSync(gfxListPtr++);*/
	} else {
		gDPFillRectangle(gfxListPtr++, start_x, start_y, end_x, start_y);
		gDPFillRectangle(gfxListPtr++, start_x, start_y, start_x, end_y);
		gDPFillRectangle(gfxListPtr++, start_x, end_y, end_x, end_y);
		gDPFillRectangle(gfxListPtr++, end_x, start_y, end_x, end_y);
	}
}

/*
	void my2D_drawSprite(int x, int y, Coordinates* coordinates)
	
	draw a sprite and the requested position
	coordinates	= definition of the sprite
	x			= the x-coordinate of upper-left corner of the sprite
	y			= the y-coordinate of upper-left corner of the sprite
*/
void my2D_drawSprite(Coordinates* coordinates, int x, int y) {
	uObjSprite* sprite;
	my2D_setCycleType(G_CYC_COPY);
	// load texture to TMEM if not the current one
	if (my2dlibrary.currentTextureInTMEM != coordinates->texture->pointer32) {
		gDPLoadTextureBlock(
			gfxListPtr++, 
			coordinates->texture->pointer32, 
			G_IM_FMT_RGBA, G_IM_SIZ_16b, 
			coordinates->texture->width, coordinates->texture->height, 0, 
			G_TX_WRAP, G_TX_WRAP, 
			G_TX_NOMASK, G_TX_NOMASK, 
			G_TX_NOLOD, G_TX_NOLOD
		);
		my2dlibrary.currentTextureInTMEM = coordinates->texture->pointer32;
	}
	// configure the sprite and draw it
	sprite = (uObjSprite*)my2D_getNextSprite();
	sprite->s.objX = x << 2; // x screen coordinate
	sprite->s.objY = y << 2; // y screen coordinate
	sprite->s.imageW = coordinates->width << 5; // width
	sprite->s.imageH = coordinates->height << 5; // height
	sprite->s.imageAdrs = GS_PIX2TMEM (coordinates->x + coordinates->y * coordinates->texture->width, my2dlibrary.colors);
	sprite->s.imageStride = GS_PIX2TMEM (coordinates->texture->width, my2dlibrary.colors);
	gSPObjRectangle(gfxListPtr++, sprite);
	return;
}

void my2D_drawRepeatedSprite(Coordinates* coordinates, int x, int y, int exp_x, int exp_y) {
	uObjSprite* sprite;
	my2D_setCycleType(G_CYC_1CYCLE);
	// load texture to TMEM if not the current one
	if (my2dlibrary.currentTextureInTMEM != coordinates->texture->pointer32) {
		if (my2dlibrary.colors == G_IM_SIZ_16b) {
			gDPLoadTextureBlock(
				gfxListPtr++, 
				coordinates->texture->pointer32, 
				G_IM_FMT_RGBA, 
				G_IM_SIZ_16b, 
				coordinates->texture->width, coordinates->texture->height, 0, 
				G_TX_WRAP, G_TX_WRAP, 
				exp_x, exp_y,
				G_TX_NOLOD, G_TX_NOLOD
			);
		}
		else {
			gDPLoadTextureBlock(
				gfxListPtr++, 
				coordinates->texture->pointer32, 
				G_IM_FMT_RGBA, 
				G_IM_SIZ_32b, 
				coordinates->texture->width, coordinates->texture->height, 0, 
				G_TX_WRAP, G_TX_WRAP, 
				exp_x, exp_y,
				G_TX_NOLOD, G_TX_NOLOD
			);
		}
		my2dlibrary.currentTextureInTMEM = coordinates->texture->pointer32;
	}
	//gSPTextureRectangle(gfxListPtr++,
	gSPScisTextureRectangle(gfxListPtr++,
		x << 2,
		y << 2,
		x+coordinates->width << 2,
		y+coordinates->height << 2,
		G_TX_RENDERTILE, 
		coordinates->x << 5, 
		coordinates->y << 5, 
		1 << 10,
		1 << 10
	)
	return;
}
/*
	void my2D_drawBackGroundCoordinates(Coordinates* coordinates, int x, int y, int line)
	
	draw a part of a background at the request x and y position.
	coordinates	= definition of the background part
	x			= x position in screen
	y			= y position in screen
	line		= multiplies y coordinates of texture. 
	
*/
void my2D_drawBackGroundCoordinates(Coordinates* coordinates, int x, int y, int line) {
	uObjBg* background = (uObjBg*)my2D_getNextBackground();
	if (background) {
		my2D_setCycleType(G_CYC_COPY);
		if (x == MY2D_CENTER)
			x = my2dlibrary.width / 2 - coordinates->width / 2;
		if (y == MY2D_CENTER)
			y = my2dlibrary.height / 2 - coordinates->height / 2;
		background->b.imagePtr 	= coordinates->texture->pointer64;
		background->b.frameX 		= x << 2;
		background->b.frameY 		= y << 2;
		background->b.imageX 		= coordinates->x << 5;
		background->b.imageY 		= (coordinates->y + line * coordinates->height) << 5;
		background->b.imageW 		= coordinates->texture->width << 2;
		background->b.imageH 		= coordinates->texture->height << 2;
		background->b.frameW 		= coordinates->width << 2;
		background->b.frameH 		= coordinates->height << 2;
		background->b.imageFlip		= coordinates->texture->flip;
		guS2DInitBg(background);
		gSPBgRectCopy(gfxListPtr++, background);	
		my2dlibrary.currentTextureInTMEM = NULL;
	}
	return;
}

void my2D_drawBackGroundScaledCoordinates(Coordinates* coordinates, int x, int y, float ratio_x, float ratio_y, int line) {
	uObjBg* background = &my2dlibrary.background_list[my2dlibrary.background_count++];
	if (background) {
		my2D_setCycleType(G_CYC_1CYCLE);
		if (x == MY2D_CENTER)
			x = my2dlibrary.width / 2 - coordinates->width / 2;
		if (y == MY2D_CENTER)
			y = my2dlibrary.height / 2 - coordinates->height / 2;
		/*gDPSetRenderMode(gfxListPtr++, G_RM_SPRITE, G_RM_SPRITE2);
		gDPSetCycleType(gfxListPtr++, G_CYC_1CYCLE);
		gDPSetTextureFilter(gfxListPtr++, G_TF_POINT);*/
		background->s.imagePtr 		= coordinates->texture->pointer64;
		background->s.imageLoad 	= G_BGLT_LOADTILE;
		background->s.imageFmt 		= G_IM_FMT_RGBA;
		background->s.imageSiz 		= my2dlibrary.colors;
		background->s.frameX 		= x << 2;
		background->s.frameY 		= y << 2;
		background->s.imageX 		= coordinates->x << 5;
		background->s.imageY 		= (coordinates->y + line * coordinates->height) << 5;
		background->s.imageW 		= coordinates->texture->width << 2;
		background->s.imageH 		= coordinates->texture->height << 2;
		background->s.frameW 		= (int)((float)coordinates->width * ratio_x)<< 2;
		background->s.frameH 		= (int)((float)coordinates->height * ratio_y) << 2;
		background->s.imageFlip		= 0;
		guS2DInitBg(background);
		background->s.scaleW		= (unsigned short)(1024.0f / ratio_x);
		background->s.scaleH		= (unsigned short)(1024.0f / ratio_y);
		background->s.imageYorig	= background->s.imageY;
		gSPBgRect1Cyc(gfxListPtr++, background);
		gDPPipeSync(gfxListPtr++);
		my2dlibrary.currentTextureInTMEM = NULL;
	}
	return;
}

/*
	void my2D_drawFullBackGround(Texture* texture, int x, int y)
	
	Draw and entire "background sprite" at the requested position in screen
	texture	= definition of the background to draw
	x		= x position in screen
	y		= y position in screen
*/
void my2D_drawFullBackGround(Texture* texture, int x, int y) {
	if (texture != NULL) {
		Coordinates coordinates = (Coordinates){0, 0, texture, texture->width, texture->height};
		my2D_drawBackGroundCoordinates(&coordinates, x, y, 0);
	}
	return;
}


/* 
	The following functions handles the uObjBg objects array and pointers
 */
void my2D_backgroundListClear() {
	my2dlibrary.background_pointer	= &my2dlibrary.background_list[0];
	my2dlibrary.background_count		= 0;		
}
uObjBg* my2D_getNextBackground() {
	if (my2dlibrary.background_count < MY2D_OBJ_LIST_SIZE) {
		my2dlibrary.background_pointer = &my2dlibrary.background_list[my2dlibrary.background_count++];
		my2dlibrary.background_pointer->s.imageLoad 	= G_BGLT_LOADTILE;
		my2dlibrary.background_pointer->b.imageLoad 	= G_BGLT_LOADTILE;
		my2dlibrary.background_pointer->b.imageFmt 		= G_IM_FMT_RGBA;
		my2dlibrary.background_pointer->s.imageFmt 		= G_IM_FMT_RGBA;
		my2dlibrary.background_pointer->b.imageSiz 		= my2dlibrary.colors;
		my2dlibrary.background_pointer->s.imageSiz 		= my2dlibrary.colors;
		my2dlibrary.background_pointer->b.imagePal 		= 0;
		my2dlibrary.background_pointer->s.imagePal 		= 0;
		my2dlibrary.background_pointer->b.imageFlip 	= 0;
		my2dlibrary.background_pointer->s.imageFlip 	= 0;
		return my2dlibrary.background_pointer;
	}
	return 0;
}


/* 
	The following functions handles the uObjSprite objects array and pointers
 */
void my2D_spriteListClear() {
	my2dlibrary.sprite_pointer = &my2dlibrary.sprite_list[0];
	my2dlibrary.sprite_count = 0;
	return;
}
 
uObjSprite* my2D_getNextSprite() {
	if (my2dlibrary.sprite_count < MY2D_SPRITE_LIST_SIZE) {
		my2dlibrary.sprite_pointer = &my2dlibrary.sprite_list[my2dlibrary.sprite_count++];
		my2dlibrary.sprite_pointer->s.paddingX 		= 0;
		my2dlibrary.sprite_pointer->s.paddingY 		= 0;
		my2dlibrary.sprite_pointer->s.imageFmt 		= G_IM_FMT_RGBA;
		my2dlibrary.sprite_pointer->s.imageSiz 		= my2dlibrary.colors;
		my2dlibrary.sprite_pointer->s.imagePal 		= 0;
		my2dlibrary.sprite_pointer->s.imageFlags	= 0;
		my2dlibrary.sprite_pointer->s.scaleH		= 1 << 10;
		my2dlibrary.sprite_pointer->s.scaleW		= 1 << 10;
		return my2dlibrary.sprite_pointer;
	}
	else
		return 0;
}

void my2D_animationNextFrame(Animation* animation) {
	if (animation->frame+1 < animation->sprite->frames)
		animation->frame++;
	else {
		if (animation->sprite->loop)
			animation->frame = 0;
		else
			animation->status = ANIMATION_STATUS_ENDED;
	}
}

void my2D_animationCropp(Animation* animation, Box* box, int position_x, int position_y) {
	int x, y;
	Coordinates coordinates;
	x = animation->frame % (animation->sprite->texture->width / animation->sprite->width);
	y = animation->frame / (animation->sprite->texture->width / animation->sprite->width);
	
	coordinates = (Coordinates){x*animation->sprite->width, y*animation->sprite->height, animation->sprite->texture, animation->sprite->width, animation->sprite->height};
	x = animation->x;
	y = animation->y;
	
	if (animation->is_character) {
		// center character if far enough of the starting element
		if (x > (my2dlibrary.width / 2) - (animation->sprite->width / 2) )
			if (x + animation->sprite->width / 2   < animation->border_x - (my2dlibrary.width / 2)) 
				x = (my2dlibrary.width / 2) - (animation->sprite->width / 2);
			else
				x = my2dlibrary.width - (animation->border_x - x);
		else if (animation->x < 0) { // character can't go outside the level's boundaries;
			animation->x = 0;
			x = 0;
		}
		if (y > (my2dlibrary.height / 2) - (animation->sprite->height / 2)) 
			if (y + animation->sprite->height / 2  < animation->border_y - (my2dlibrary.height / 2)) 
				y = (my2dlibrary.height / 2) - (animation->sprite->height / 2);
			else
				y = my2dlibrary.height - (animation->border_y - y);
		else if (animation->y < 0) { // character can't go outside the level's boundaries;
			animation->y = 0;
			y = 0;
		}
	}
	else {
		x -= position_x;
		y -= position_y;
		// special case, if the texture is flipped negative x-axis screen coordinate is a mess (or I made a mistake and this fix it)
		if (animation->x < 0 && animation->x + coordinates.width >= 0 && animation->sprite->texture->flip == G_BG_FLAG_FLIPS) {
			coordinates.width += animation->x;
			coordinates.x += 0;
			x = 0;
		}
	}
	// if box is not null then it means when need to draw only the part of the animation that is inside this box
	if (box != NULL) {
		if (animation->x < box->start_x) {
			coordinates.width -= (box->start_x - animation->x);
			coordinates.x += (box->start_x - animation->x);
			x = box->start_x - position_x;
		}
		
		if (animation->y + coordinates.height > box->end_y) {
			coordinates.height = box->end_y - animation->y;
		}
	}
	
	if (x + coordinates.width > 0 && x < my2dlibrary.width && y + coordinates.height > 0 && y < my2dlibrary.height)
		my2D_drawBackGroundCoordinates(&coordinates, x, y, 0);
}

void my2D_animationDraw(Animation* animation) {
	my2D_animationCropp(animation, NULL, 0, 0);
}
/*void my2D_animationDraw(Animation* animation) {
	int x, y;
	Coordinates coordinates;
	x = animation->frame % (animation->sprite->texture->width / animation->sprite->width);
	y = animation->frame / (animation->sprite->texture->width / animation->sprite->width);
	
	coordinates = (Coordinates){x*animation->sprite->width, y*animation->sprite->height, animation->sprite->texture, animation->sprite->width, animation->sprite->height};
	x = animation->x;
	y = animation->y;
	
	if (animation->is_character) {
		// center character if far enough of the starting element
		if (x > (my2dlibrary.width / 2) - (animation->sprite->width / 2) )
			if (x + animation->sprite->width / 2   < animation->border_x - (my2dlibrary.width / 2)) 
				x = (my2dlibrary.width / 2) - (animation->sprite->width / 2);
			else
				x = my2dlibrary.width - (animation->border_x - x);
		else if (animation->x < 0) { // character can't go outside the level's boundaries;
			animation->x = 0;
			x = 0;
		}
		if (y > (my2dlibrary.height / 2) - (animation->sprite->height / 2)) 
			if (y + animation->sprite->height / 2  < animation->border_y - (my2dlibrary.height / 2)) 
				y = (my2dlibrary.height / 2) - (animation->sprite->height / 2);
			else
				y = my2dlibrary.height - (animation->border_y - y);
		else if (animation->y < 0) { // character can't go outside the level's boundaries;
			animation->y = 0;
			y = 0;
		}
	}
	else
		// special case, if the texture is flipped negative x-axis screen coordinate is a mess (or I made a mistake and this fix it)
		if (animation->x < 0 && animation->x + coordinates.width >= 0 && animation->sprite->texture->flip == G_BG_FLAG_FLIPS) {
			coordinates.width += animation->x;
			coordinates.x += 0;
			x = 0;
		}
		
	my2D_drawBackGroundCoordinates(&coordinates, x, y, 0);
}*/

void drawScrollingBackground(ScrollingBackground* background) {
	ScrollingBackground* tmp_background = background;
	Coordinates coordinates;
	int x;
	
	while (tmp_background) {
		x = (int)tmp_background->scrollX;
		coordinates = (Coordinates){x, 0, tmp_background->texture, tmp_background->texture->width - x, tmp_background->texture->height};
		my2D_drawBackGroundCoordinates(&coordinates, 0, tmp_background->startY, 0);
		if (tmp_background->texture->width - x < my2dlibrary.width) {
			coordinates = (Coordinates){0, 0, tmp_background->texture, my2dlibrary.width - (tmp_background->texture->width - x), tmp_background->texture->height};
			my2D_drawBackGroundCoordinates(&coordinates, tmp_background->texture->width - x, tmp_background->startY, 0);
		}
		tmp_background = tmp_background->nextBackground;
	}
}

void scrollScrollingBackground(ScrollingBackground* background, int x, int y) {
	ScrollingBackground* tmp_background = background;
	while (tmp_background) {
		tmp_background->scrollX += (float)x * tmp_background->increment_ratio;
		tmp_background->scrollY += (float)y * tmp_background->increment_ratio;
		if (tmp_background->scrollX > (float)tmp_background->texture->width)
			tmp_background->scrollX = 0.0;
		if (tmp_background->scrollY > (float)tmp_background->texture->height)
			tmp_background->scrollY = 0.0;
		if (tmp_background->scrollX < 0)
			tmp_background->scrollX = (float)tmp_background->texture->width;
		tmp_background = tmp_background->nextBackground;
	}
}
