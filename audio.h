#include <nualstl_n.h>

#ifndef _AUDIO_H_
#define _AUDIO_H_


typedef struct MusicTrack MusicTrack;


struct MusicTrack {
	int					track_id;
	int					total_length;
	u64					started;
	MusicTrack*			next_track;
};


void initAudio();
void loadAudio(u32 ptrStart, u32 ptrEnd, u32 sfxStart, u32 sfxEnd, void *wbkStart);
void checkMusic(MusicTrack*);
void stopMusic();

int ptr_buf[];
int sfx_buf[];
musHandle sndHandle[3];
MusicTrack*				current_music;


#endif /* _AUDIO_H_*/
