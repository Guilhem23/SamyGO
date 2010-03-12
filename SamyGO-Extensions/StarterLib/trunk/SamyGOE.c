#include <stdio.h>
#include "SDL.h"

#define WIDTH 960
#define HEIGHT 540
#define BPP 4
#define DEPTH 32

FILE *log_fp;

extern unsigned int GetFrameBufferAddress(void);

void __attribute__ ((constructor)) SGO_init(void);
void __attribute__ ((destructor)) SGO_fini(void);

void setpixel(SDL_Surface *screen, int x, int y, Uint8 r, Uint8 g, Uint8 b)
{
	Uint32 *pixmem32;
	Uint32 colour;  

	colour = SDL_MapRGB( screen->format, r, g, b );

	pixmem32 = (Uint32*) screen->pixels  + y + x;
	*pixmem32 = colour;

}


void DrawScreen(SDL_Surface* screen, int h)
{ 
	int x, y, ytimesw;

	if(SDL_MUSTLOCK(screen)) 
	{
		if(SDL_LockSurface(screen) < 0) return;
	}

	for(y = 0; y < screen->h; y++ ) 
	{
		ytimesw = y*screen->pitch/BPP;
		for( x = 0; x < screen->w; x++ ) 
		{
			setpixel(screen, x, ytimesw, (x*x)/256+3*y+h, (y*y)/256+x+h, h);
		}
	}

	if(SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);

	SDL_Flip(screen); 
}


int Game_Main(const char *path, const char *udn)
{
	SDL_Surface *screen;

	int h=0,i,err; 
	char cmd[100];

	sprintf(cmd,"%s/rcSGO &\n",path);
	fprintf(log_fp,"%s try to execute %s\n",__FUNCTION__,cmd);
	err = system(cmd);
	fprintf(log_fp,"%s executed %s\n",__FUNCTION__,cmd);

	if(err)
		fprintf(log_fp,"failed to call: %s\n",cmd);

	err = SDL_Init(SDL_INIT_VIDEO);
	if (!err) 
		fprintf(log_fp,"failed to init sdl-video: %s\n",SDL_GetError());

	const SDL_VideoInfo* Info = SDL_GetVideoInfo();
	fprintf(log_fp,"%s: Resolution -> %d x %d, hw_available %d wm_available %d MemSize: %u\n",
			__FUNCTION__,
			Info->current_w,
			Info->current_h,
			Info->hw_available,
			Info->wm_available,
			Info->video_mem);

	screen = SDL_SetVideoMode(WIDTH, HEIGHT, DEPTH, SDL_HWSURFACE); 
	Info = SDL_GetVideoInfo();
	fprintf(log_fp,"%s: Resolution -> %d x %d, hw_available %d wm_available %d MemSize: %u\n",
			__FUNCTION__,
			Info->current_w,
			Info->current_h,
			Info->hw_available,
			Info->wm_available,
			Info->video_mem);

	for(i=0;i<50;i++) {
		DrawScreen(screen,h++);
	}

	fprintf(log_fp,"ready\n");

	SDL_Quit();
	return 0;
}

void SGO_init(void){
	log_fp = fopen("/mtd_rwarea/kmsg", "w");
	fprintf(log_fp,"####################\n");
	//	fprintf(log_fp,"# %08x \n", GetFrameBufferAddress());
}
void SGO_fini(void){
	fprintf(log_fp,"%s: unload\n",__FUNCTION__);
	fflush(log_fp);
	fclose(log_fp);
}
