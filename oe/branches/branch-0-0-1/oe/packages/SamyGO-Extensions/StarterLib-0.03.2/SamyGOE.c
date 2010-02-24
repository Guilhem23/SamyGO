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
	SDL_Event event;

	int keypress = 0;
	int h=0,i,err; 
	char cmd[100];

	sprintf(cmd,"%s/rcSGO &\n",path);
	err = system(cmd);

	if(err)
		h = 100;

	if (SDL_Init(SDL_INIT_VIDEO) < 0 ) return 1;

	if (!(screen = SDL_SetVideoMode(WIDTH, HEIGHT, DEPTH, SDL_FULLSCREEN|SDL_HWSURFACE)))
	{
		SDL_Quit();
		return 1;
	}

	for(i=0;i<50;i++) {
		DrawScreen(screen,h++);
	}
	
	fprintf(log_fp,"ready\n");

	SDL_Quit();

	return 0;
}

void SGO_init(void){
	// fprintf(stderr,"%s:\n",__FUNCTION__);
	      log_fp = fopen("/dev/kmsg", "a+");
	//      log_fp = fopen("/dev/null", "a+");
	//      log_fp = fopen("/mtd_rwarea/kmsg", "w");
	// log_fp = fopen("/dev/null", "a+");
	// log_fp = stderr;
	fprintf(log_fp,"####################\n");
	fprintf(log_fp,"# %08x \n", GetFrameBufferAddress());
}
void SGO_fini(void){
	fprintf(log_fp,"%s: unload\n",__FUNCTION__);
	fflush(log_fp);
	fclose(log_fp);

}
