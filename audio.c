#include <nusys.h>
#include "audio.h"
#include "main.h"
#include "castle.h"

void initAudio(void) {
    musConfig c;
    c.control_flag = 0; // Set to MUSCONTROL_RAM if wbk file also placed in RAM
    c.channels = NU_AU_CHANNELS; // Maximum total number of channels
    c.sched = NULL; // The address of the Scheduler structure. NuSystem uses NULL
    c.thread_priority = NU_AU_MGR_THREAD_PRI; // Thread priority (highest)
    c.heap = (unsigned char*)_codeSegmentEnd; // Heap address
    c.heap_length = NU_AU_HEAP_SIZE; // Heap size
    c.ptr = NULL; // Allows you to set a default ptr file
    c.wbk = NULL; // Allows you to set a default wbk file
    c.default_fxbank = NULL; // Allows you to set a default bfx file
    c.fifo_length = NU_AU_FIFO_LENGTH; // The size of the library's FIFO buffer
    c.syn_updates = NU_AU_SYN_UPDATE_MAX; // Number of updates for the synthesizer.
    c.syn_output_rate = 44100; // Audio output rate: 44100hz
    c.syn_rsp_cmds = NU_AU_CLIST_LEN; // Maximum length of the audio command list.
    c.syn_retraceCount = 1; // The number of frames per retrace message
    
    // Number of buffers the Audio Manager can use for DMA from ROM transfer.
    c.syn_num_dma_bufs = NU_AU_DMA_BUFFER_NUM; 
    c.syn_dma_buf_size = NU_AU_DMA_BUFFER_SIZE;

    // Initialize the Audio Manager.
    nuAuStlMgrInit(&c);

    // Register the PRENMI function.
    nuAuPreNMIFuncSet(nuAuPreNMIProc);
	
	/*
	nuPiReadRom((u32)_MusicChancePtrSegmentRomStart, (void *)ptr_buf, (u32)(_MusicChancePtrSegmentRomEnd-_MusicChancePtrSegmentRomStart));
	MusPtrBankInitialize(ptr_buf, _MusicChanceWbkSegmentRomStart);
	nuPiReadRom((u32)_MusicChanceSfxSegmentRomStart, (void *)sfx_buf, (u32)(_MusicChanceSfxSegmentRomEnd-_MusicChanceSfxSegmentRomStart));
	MusFxBankInitialize(sfx_buf);*/
	
	loadAudio((u32)_MusicAllPtrSegmentRomStart, (u32)_MusicAllPtrSegmentRomEnd, (u32)_MusicAllSfxSegmentRomStart, (u32)_MusicAllSfxSegmentRomEnd, _MusicAllWbkSegmentRomStart);

}

void loadAudio(u32 ptrStart, u32 ptrEnd, u32 sfxStart, u32 sfxEnd, void *wbkStart) {
	nuPiReadRom((u32)ptrStart, (void *)ptr_buf, (u32)(ptrEnd-ptrStart));
	MusPtrBankInitialize(ptr_buf, wbkStart);
	nuPiReadRom((u32)sfxStart, (void *)sfx_buf, (u32)(sfxEnd-sfxStart));
	MusFxBankInitialize(sfx_buf);	
}

void checkMusic(MusicTrack* music) {
	u64 current_time = osGetTime();
	if (current_music) {
		if (current_music->started == 0) {
			sndHandle[0] = nuAuStlSndPlayerPlay(current_music->track_id);
			nuAuStlSndPlayerSetSndVol(sndHandle[0], game.music);
			current_music->started = current_time;
		} else if (OS_CYCLES_TO_USEC(osGetTime()-current_music->started) / 1000000 > current_music->total_length) {
			if (current_music->next_track) {
				nuAuStlSndPlayerSndStop(sndHandle[0], 0);			
				current_music->started = 0;
				current_music->next_track->started = 0;
				current_music = current_music->next_track;
			}
		}
	}
}

void playSound(int channel, int id) {
	sndHandle[channel] = nuAuStlSndPlayerPlay(id);
	nuAuStlSndPlayerSetSndVol(sndHandle[channel], game.sound);
}

void stopMusic() {
	nuAuStlSndPlayerSndStop(sndHandle[0], 0);
	if (current_music)
		current_music->started = 0;
	current_music = NULL;
}