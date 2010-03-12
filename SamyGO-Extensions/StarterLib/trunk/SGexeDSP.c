/* 
 * $Id: SGexeDSP.c 399 2010-01-30 09:57:48Z arris $
 *
 * find descrioption at: http://www.yolinux.com/TUTORIALS/LibraryArchives-StaticAndDynamic.html
 *
 */

#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>

#include <sys/mman.h>

// void *mmap(void *addr, size_t length, int prot, int flags,
//                         int fd, off_t offset);

#include <SDL.h>

struct SDL_PrivateVideoData {
	int w, h;
	void *buffer;
};

#include "SDL_sysvideo.h"
#define WIDTH 960
#define HEIGHT 540
#define BPP 4
#define DEPTH 32
Uint32 rmask, gmask, bmask, amask;
char *SG_V_buff[WIDTH * HEIGHT];
extern char* org_video_buffer;

#include "savesurf.h"
int ScCo;
void TakeShoot(void){
	char ScreenshotName[100];
	SDL_Surface *ss_surface = SDL_CreateRGBSurface(SDL_SWSURFACE, WIDTH, HEIGHT, DEPTH, rmask, gmask, bmask, amask);
	SDL_BlitSurface(current_video->screen, NULL, ss_surface, NULL);

	loginfo("Save %s\n", ScreenshotName);

	sprintf(ScreenshotName,"/mtd_wiselink/Screenshot-%03d.png",ScCo);

	if(!png_save_surface(ScreenshotName, ss_surface))
		ScCo++;
	else
		unlink(ScreenshotName); /* may done by write_png? */

}
void TakeShootSurface(SDL_Surface *surface){
	char ScreenshotName[100];
	SDL_Surface *ss_surface = SDL_CreateRGBSurface(SDL_SWSURFACE, WIDTH, HEIGHT, DEPTH, rmask, gmask, bmask, amask);
	SDL_BlitSurface(surface, NULL, ss_surface, NULL);

	loginfo("Save %s\n", ScreenshotName);

	sprintf(ScreenshotName,"/mtd_wiselink/ScreenshotS-%03d.png",ScCo);

	if(!png_save_surface(ScreenshotName, ss_surface))
		ScCo++;
	else
		unlink(ScreenshotName); /* may done by write_png? */

}

/* Abstract:
 * call to GetFrameBufferAddress() from your library indicates execution of:
 * _Z11s_GetScreeniPPcPtS1_Pi:
 */
unsigned int *GetFrameBufferAddress(void){
	static unsigned int (*GetFrameBufferAddress_real)(void)=NULL;
	printf("our %s: \n",__FUNCTION__);
	
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;

	/*
	 * #define SDL_VideoSurface        (current_video->screen)
	 * #define SDL_ShadowSurface       (current_video->shadow)
	 * #define SDL_PublicSurface       (current_video->visible)
	 */

	if (!GetFrameBufferAddress_real) GetFrameBufferAddress_real=dlsym(RTLD_NEXT,"GetFrameBufferAddress");
	printf("our %s: GetFrameBufferAddress_real 0x%08x \n",__FUNCTION__,GetFrameBufferAddress_real());
	// return (calloc(1,sizeof(SG_V_buff)));
/*	if(!current_video->screen)
	current_video->screen = SDL_CreateRGBSurface(SDL_SWSURFACE, HEIGHT, WIDTH, DEPTH, 
			rmask, gmask, bmask, amask); */
//	current_video->shadow = SDL_CreateRGBSurface(SDL_SWSURFACE, HEIGHT, WIDTH, DEPTH, 
//			rmask, gmask, bmask, amask);
//	current_video->visible = SDL_CreateRGBSurface(SDL_SWSURFACE, HEIGHT, WIDTH, DEPTH, 
//			rmask, gmask, bmask, amask);
	// 48486000;
	// current_video->hidden->buffer = current_video->screen->pixels;
	if(!current_video->hidden->buffer)
	current_video->hidden->buffer = SG_V_buff;
//	current_video->screen->refcount++;
//	return current_video->screen->pixels;
	return current_video->hidden->buffer; 
//	return calloc(1,sizeof(SG_V_buff));
}

int SDL_Init(Uint32 flags){
	int res;
	static int (*SDL_Init_real)(Uint32 flags)=NULL;
	printf("our %s flags 0x%08x\n",__FUNCTION__,flags);
	if (!SDL_Init_real) SDL_Init_real=dlsym(RTLD_NEXT,"SDL_Init");
	printf("and now call original, res: ");
	res = SDL_Init_real(flags);
	printf("%d\n",res);
	return res;
}

int SDL_VideoInit(const char *driver_name, Uint32 flags){
	int res;
	static int (*SDL_VideoInit_real)(const char *driver_name, Uint32 flags)=NULL;
	printf("our %s name %s flags 0x%08x\n",__FUNCTION__,driver_name,flags);
/*	printf("%s: current_video->name %s \n",__FUNCTION__,current_video->name);
	printf("%s: current_video->hidden->buffer at %p\n",__FUNCTION__,
			current_video->hidden->buffer);
	printf("%s: current_video->screen at %p\n",__FUNCTION__,current_video->screen);
	printf("%s: current_video h %d \n",__FUNCTION__,current_video->hidden->w);
	printf("%s: current_video w %d \n",__FUNCTION__,current_video->hidden->h); */
	//      printf("screen->screen->pixels at %p\n",current_video->screen->pixels);
	if (!SDL_VideoInit_real) SDL_VideoInit_real=dlsym(RTLD_NEXT,"SDL_VideoInit");
	printf("and now call original, res: ");
	res = SDL_VideoInit_real(driver_name,flags);
	printf("%d\n",res);
/*	current_video->screen = SDL_CreateRGBSurface(SDL_SWSURFACE, HEIGHT, WIDTH, DEPTH, 
			rmask, gmask, bmask, amask);
	current_video->shadow = SDL_CreateRGBSurface(SDL_SWSURFACE, HEIGHT, WIDTH, DEPTH, 
			rmask, gmask, bmask, amask);
	current_video->visible = SDL_CreateRGBSurface(SDL_SWSURFACE, HEIGHT, WIDTH, DEPTH, 
			rmask, gmask, bmask, amask);
	current_video->hidden->buffer = 0;	*/
}

void SDL_VideoQuit(void){
	static void (*SDL_VideoQuit_real)(void)=NULL;
	printf("our %s\n",__FUNCTION__);
	printf("%s: current_video->name %s \n",__FUNCTION__,current_video->name);
	printf("%s: current_video->hidden->buffer at %p\n",__FUNCTION__,
			current_video->hidden->buffer);
	printf("%s: current_video->screen at %p\n",__FUNCTION__,current_video->screen);
	printf("%s: current_video h %d \n",__FUNCTION__,current_video->hidden->w);
	printf("%s: current_video w %d \n",__FUNCTION__,current_video->hidden->h);
	//      printf("screen->screen->pixels at %p\n",current_video->screen->pixels);
	if (!SDL_VideoQuit_real) SDL_VideoQuit_real=dlsym(RTLD_NEXT,"SDL_VideoQuit");
	printf("and now call original\n");
//	current_video->screen = SDL_CreateRGBSurface(SDL_SWSURFACE, HEIGHT, WIDTH, DEPTH, 
//			rmask, gmask, bmask, amask);
//	current_video->shadow = SDL_CreateRGBSurface(SDL_SWSURFACE, HEIGHT, WIDTH, DEPTH, 
//			rmask, gmask, bmask, amask);
//	current_video->visible = SDL_CreateRGBSurface(SDL_SWSURFACE, HEIGHT, WIDTH, DEPTH, 
//			rmask, gmask, bmask, amask);
//	current_video->hidden->buffer = 0;	
	SDL_VideoQuit_real();
}

/* need to run in standalone mode */
// unsigned int _Z11s_GetScreeniPPcPtS1_Pi(void *a, void *b, void *c){
SDL_Surface *_Z11s_GetScreeniPPcPtS1_Pi(void *a, void *b, void *c){
	printf("%s: a %p b %p c %p\n",__FUNCTION__, a, b, c);
	// SDL_Surface *ss_surface = SDL_GetVideoSurface();
	SDL_Surface *ss_surface = SDL_CreateRGBSurface(SDL_SWSURFACE, HEIGHT, WIDTH, DEPTH,
			                        rmask, gmask, bmask, amask);
	// printf("%s: w %d h %d\n",__FUNCTION__,ss_surface->w,ss_surface->h);
	return ss_surface;
}


/* may for furter analyze */
void _ZN9SPCScreen12EnableScreenEi(void *a, int h, int c, int d, void *e, void *f){
	printf("%s: a %p h %d c %d d %d e %p f %p\n",__FUNCTION__, a, h, c, d, e, f);
	// SDL_Init(SDL_INIT_VIDEO);
// if(fmt->BitsPerPixel!=8){
//		     fprintf(stderr, "Not an 8-bit surface.\n");
//		       return(-1);
//		     } else {
//		     fprintf(stderr, "8-bit surface.\n");
//		     }

//	SDL_Init(SDL_INIT_EVERYTHING);
	// SDL_Surface *SDL_GetVideoSurface(void);
	// SDL_SetVideoMode() and use that instead.
	// >> SDL_GetVideoSurface() is deprecated.
	//
	// 	buffer[y] = SDL_CreateRGBSurface(SDL_SWSURFACE, x_res/4, y_res/4, 
	// 						SDL_GetVideoSurface()->format->Rmask,
	// 											SDL_GetVideoSurface()->format->Gmask,
	// 																SDL_GetVideoSurface()->format->Bmask,
	// 																					SDL_GetVideoSurface()->format->Amask );
	//
	//
	printf("%s: screen at %p \n",__FUNCTION__, current_video->screen);
}
void _ZN16SCShadowGraphics20GetDisplayDeviceInfoElP12STScreenInfo(void *a, SDL_Surface *b, void *c){
	printf("%s: a %p b %p c %p\n",__FUNCTION__,a,b,c);

	// b = SDL_CreateRGBSurface(SDL_SWSURFACE, HEIGHT, WIDTH, DEPTH,
	//		                        rmask, gmask, bmask, amask);
	const SDL_VideoInfo* Info = SDL_GetVideoInfo();
	printf("%s: Resolution -> %d x %d, hw_available %d wm_available %d MemSize: %u\n",
			__FUNCTION__, 
			Info->current_w, 
			Info->current_h, 
			Info->hw_available, 
			Info->wm_available, 
			Info->video_mem);

	printf("%s: End.\n",__FUNCTION__);
}

void _ZN16SCShadowGraphics9ChangeResElll(void *a,int w,int h,void *d,void *e,void *f,void *g,void *i){
	printf("%s: a %p w %d h %d d %p e %p f %p g %p i %p\n",__FUNCTION__,a,w,h,d,e,f,g,i);
}

void _Z14s_UpdateScreeniiiii(void *a, void *b, void *c){
	printf("%s: a %p b %p c %p\n",__FUNCTION__, a, b, c);
	// draw routine?
	TakeShoot();
//	TakeShootSurface();
}

/* ? funks */
void _ZN9TDBuilder12GetTDiFeederEi(void *a, void *b, void *c){
	        printf("%s: a %p b %p c %p\n",__FUNCTION__,a,b,c);
}

void _ZN9TDBuilder10GetTDiMpegEi(void *a, void *b, void *c){
	        printf("%s: a %p b %p c %p\n",__FUNCTION__,a,b,c);
}
void _ZN9TDBuilder20GetTDiAudioProcessorEi(void *a, void *b, void *c){
	        printf("%s: a %p b %p c %p\n",__FUNCTION__,a,b,c);
}

/* never ending storie? */
void g_hGFX(void *a, void *b, void *c){
	printf("%s: a %p b %p c %p\n",__FUNCTION__,a,b,c);
}
void _Z11spiFillRectPvPKjmiiiii(SDL_Surface *dst, SDL_Rect *dstrect, Uint32 color, void *a, void *b, void *c){
	printf("%s: color 0x%08x a %p b %p c %p\n",__FUNCTION__,color,a,b,c);
	printf("%s: surface at %p x %d y %d w %d h %d\n",__FUNCTION__,&dst,
			dstrect->x,dstrect->y,
			dstrect->w,dstrect->h); 
}


// int InputKeyState, InputKeyData;

int main(int argc, const char *argv[])
{
	void *lib_handle;
	char *UDN = "1111-1111-1111-1111";
	char *error, *lib_dir, lib_toopen[100], *lib_name;
	int (*fn)(char *, char *), len;

	*SG_V_buff = calloc(1, WIDTH * HEIGHT);
	org_video_buffer = SG_V_buff;

	if(argc < 2)
		lib_dir = "/mtd_tlib/GGame/SamyGO/";
	else
		lib_dir = argv[1];

	fprintf(stdout,"Path: %s\n",lib_dir);

	if(argc < 3)
		lib_name = "libSamyGO.so";
	else
		lib_name = argv[2];

	fprintf(stdout,"Library: %s\n",lib_name);

	/* get ssl into */
	// OpenSSL_add_all_algorithms();

	len = strlen(lib_dir);
	strcpy(lib_toopen, lib_dir);
	fprintf(stdout,"Lib to load: %s\n",lib_toopen);
	strncat(lib_toopen, lib_name, strlen(lib_name));
	fprintf(stdout,"Lib to load: %s\n",lib_toopen);

	lib_handle = dlopen(lib_toopen, RTLD_LAZY);
	if (!lib_handle) 
	{
		fprintf(stderr, "%s\n", dlerror());
		exit(1);
	}

	fn = dlsym(lib_handle, "Game_Main");
	if ((error = dlerror()) != NULL)  
	{
		fprintf(stderr, "%s\n", error);
		exit(1);
	} else {
		fprintf(stderr, "Found function Game_Main\nCall it with %s %s\n",lib_dir,UDN);
	}
// SDL_Init(SDL_INIT_VIDEO);
// SDL_SetVideoMode(WIDTH, HEIGHT, DEPTH, SDL_HWSURFACE);
	/* lib_name, normaly uidn is passed to function
	 * have to figure out what it realy is
	 */
	(*fn)(lib_dir, UDN);

	dlclose(lib_handle);
	exit(0);
}
