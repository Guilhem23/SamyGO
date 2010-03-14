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
// #include <unistd.h>

#include <sys/mman.h>

// void *mmap(void *addr, size_t length, int prot, int flags,
//                         int fd, off_t offset);

void __attribute__ ((constructor)) SG_init(void);
void __attribute__ ((destructor)) SG_fini(void);

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
char *SG_V_buff[WIDTH * HEIGHT * 4];
extern char* org_video_buffer;

#include "savesurf.h"
int ScCo;
void TakeShoot(void){
	char ScreenshotName[100];
	SDL_Surface *ss_surface = SDL_CreateRGBSurface(SDL_SWSURFACE, WIDTH, HEIGHT, DEPTH, rmask, gmask, bmask, amask);
	SDL_BlitSurface(current_video->screen, NULL, ss_surface, NULL);

	sprintf(ScreenshotName,"/mtd_wiselink/Screenshot-%03d.png",ScCo);
	loginfo("Save %s\n", ScreenshotName);

	if(!png_save_surface(ScreenshotName, ss_surface))
		ScCo++;
	else
		unlink(ScreenshotName); /* may done by write_png? */

}
void TakeShootSurface(SDL_Surface *surface){
	char ScreenshotName[100];
	SDL_Surface *ss_surface = SDL_CreateRGBSurface(SDL_SWSURFACE, WIDTH, HEIGHT, DEPTH, rmask, gmask, bmask, amask);
	SDL_BlitSurface(surface, NULL, ss_surface, NULL);

	sprintf(ScreenshotName,"/mtd_wiselink/ScreenshotS-%03d.png",ScCo);
	loginfo("Save %s\n", ScreenshotName);

	if(!png_save_surface(ScreenshotName, ss_surface))
		ScCo++;
	else
		unlink(ScreenshotName); /* may done by write_png? */

}

#include <sys/time.h>

//       int getpriority(int which, int who);
int setpriority(int which, int who, int prio){
//	static int (*setpriority_real)(int which, int who, int prio)=NULL;
	printf("our %s: which %d who %d prio %d\n",__FUNCTION__,which,who,prio);
return 0;
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
	if(!current_video->hidden->buffer){
		current_video->hidden->buffer = SG_V_buff;
		// current_video->screen->refcount++;
	}
	//	current_video->screen->refcount++;
	//	return current_video->screen->pixels;
	return current_video->hidden->buffer; 
	//	return calloc(1,sizeof(SG_V_buff));
}

SDL_Surface* SDL_SetVideoMode(int width, int height, int bitsperpixel, Uint32 flags){
	SDL_Surface* res;
	static int (*SDL_SetVideoMode_real)(int width, int height, int bitsperpixel, Uint32 flags)=NULL;
	printf("our %s w %d h %d c %d flags 0x%08x\n",__FUNCTION__,width,height,bitsperpixel,flags);
	if (!SDL_SetVideoMode_real) SDL_SetVideoMode_real=dlsym(RTLD_NEXT,"SDL_SetVideoMode");
	printf("%s: and now call original -> ",__FUNCTION__);
	res = SDL_SetVideoMode_real(width,height,bitsperpixel,flags);
	printf("%s: ready to return %p\n",__FUNCTION__,res);
	return res;
}

int SDL_LockYUVOverlay(SDL_Overlay *overlay){
	int res;
	static int (*SDL_LockYUVOverlay_real)(SDL_Overlay *overlay)=NULL;
	printf("our %s overlay at %p\n",__FUNCTION__,overlay);
	if (!SDL_LockYUVOverlay_real) SDL_LockYUVOverlay_real=dlsym(RTLD_NEXT,"SDL_LockYUVOverlay");
	printf("and now call original, res: ");
	res = SDL_LockYUVOverlay_real(overlay);
	printf("%d\n",res);
	return res;
}

SDL_Overlay *SDL_CreateYUVOverlay(int width, int height, Uint32 format, SDL_Surface *display){
	SDL_Overlay *res;
	static SDL_Overlay *(*SDL_CreateYUVOverlay_real)(int width, int height, Uint32 format, SDL_Surface *display)=NULL;
	printf("our %s w %d h %d format %u on display at %p\n",__FUNCTION__,width,height,format,display);
	if (!SDL_CreateYUVOverlay_real) SDL_CreateYUVOverlay_real=dlsym(RTLD_NEXT,"SDL_CreateYUVOverlay");
	printf("and now call original, -> ");
	res = SDL_CreateYUVOverlay_real(width,height,format,display);
	printf("%s: ready overlay at %p on display at %p\n",res,display);
	return res;
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
/*		current_video->screen = SDL_CreateRGBSurface(SDL_SWSURFACE, HEIGHT, WIDTH, DEPTH, 
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
SDL_Surface *_Z11s_GetScreeniPPcPtS1_Pi(void *a, SDL_Surface *b, SDL_Surface *c){
	printf("%s: a %p b %p c %p\n",__FUNCTION__, a, b, c);
	// SDL_Surface *ss_surface = SDL_GetVideoSurface();
	printf("%s: surface? w %d h %d deref w %d\n",__FUNCTION__,b->w, b->h,&b->w);
	if(!current_video->screen) {
	SDL_Surface *ss_surface = SDL_CreateRGBSurface(SDL_SWSURFACE, HEIGHT, WIDTH, DEPTH,
			rmask, gmask, bmask, amask);
	printf("%s: ss_surface at %p ss_surface->pixels at %p\n",__FUNCTION__,ss_surface,ss_surface->pixels);
	return ss_surface;
	} else {
	// printf("%s: w %d h %d\n",__FUNCTION__,ss_surface->w,ss_surface->h);
	printf("%s: current_video->screen at %p surface->pixels at %p\n",__FUNCTION__,
			current_video->screen,current_video->screen->pixels);
	return current_video->screen;
	}
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

void *_ZN16SCShadowGraphics9ChangeResElll(void *a,int w,int h,void *d,void *e,void *f,void *g,void *i){
	printf("%s: a %p w %d h %d -> %p d %p e %p f %p g %p i %p\n",__FUNCTION__,a,w,h,h,d,e,f,g,i);
	if (w > 960) {
		w=960;
		h=540;
		exit(2);
	}
	printf("%s: a %p w %d h %d -> %p d %p e %p f %p g %p i %p\n",__FUNCTION__,a,w,h,h,d,e,f,g,i);
	// SDL_SetVideoMode(w,h,0,SDL_HWSURFACE);
}

void _Z14s_UpdateScreeniiiii(void *a, void *b, void *c){
	printf("%s: a %p b %p c %p\n",__FUNCTION__, a, b, c);
	// draw routine?
	TakeShoot();
	// memcpy(current_video->screen->pixels,current_video->hidden->buffer,sizeof(current_video->hidden->buffer));
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

	*SG_V_buff = calloc(1, WIDTH * HEIGHT * 4);
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
void SG_init(void){
	chdir("/");
	umask(0);
}
void SG_fini(void){
	printf("%s: unload\n",__FUNCTION__);
}

