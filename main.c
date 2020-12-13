#include <nusys.h>
#include <malloc.h>
#include "main.h"
#include "castle.h"
#include "audio.h"

extern int ptr_buf[NU_AU_SAMPLE_SIZE];
extern int sfx_buf[NU_AU_SE_SIZE];
extern musHandle sndHandle[3];
/*extern musHandle musicHandle;
extern musHandle soundHandle;*/

extern u16* LowFrameBuf[2];
extern u16* HighFrameBuf[2];
extern NUContData contData[NU_CONT_MAXCONTROLLERS];
extern u8 contPattern;

static void callbackPlatform();

extern u64 time_lastframe;
extern u64 total_time;
extern float random;

extern char conbuf[40];
extern char mem_heap[300 * 1024]; 

extern u32 memory_size;
extern u8 show_console;

u64	time_last_update_level, tmp_time;

int init_heap;

/*----------------------------------------------------------------------*/
/*	Game startup. 											*/
/*	IN:	Nothing in particular. 							  	*/
/*	RET:	None											  */
/*----------------------------------------------------------------------*/
void mainproc(void* arg)
{
	// expansion pak detection. Needed to play sounds as I was running out of RAM for this feature
	memory_size = osGetMemSize();
	
	// variable initialization used to count frames per second
	time_lastframe = 0;
	time_last_update_level = 0;
	total_time = 0;
	show_console = 1;
	
	/* Initialize graphics */
	nuGfxInit();
	initAudio();
	
	 /* Set VI */
	 osCreateViManager(OS_PRIORITY_VIMGR);

	init_heap = InitHeap(mem_heap, sizeof(mem_heap));
	if (init_heap == -1)
        return;
	 
	 if (memory_size == 0x00800000)
		 setup_640();
	 else
		 setup_640();
	//platformInit();
	//initLevel();
	contPattern = nuContInit();
	
	
	osSetTime(0);
	/*
	while (OS_CYCLES_TO_USEC(osGetTime())/1000000 < 10) {
	}*/
	
	while(1){
		nuGfxTaskAllEndWait();
		nuGfxFuncSet(callbackPlatform);
		nuGfxDisplayOn();
		
		// update controlls on a 60fps basis => not being dependent on the Gfx task to finish. Good idea or not ?
		tmp_time = osGetTime();
		if (OS_CYCLES_TO_USEC(tmp_time-time_last_update_level) / (1000000 / 60) > 1) {
			checkControls();
			time_last_update_level = tmp_time;
		}
	}
}

void callbackPrenmi()
{
	nuGfxDisplayOff();
	osViSetYScale(1);
}

void callbackPlatform(u32 taskNum) {
	if(nuGfxTaskSpool)
		return;
	RCPInit();
	drawScreen();
	RCPEnd();
}

