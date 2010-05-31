/* 
 * $Id: SGexeDSP.c 525 2010-05-05 13:35:36Z arris $
 *
 * find descrioption at: http://www.yolinux.com/TUTORIALS/LibraryArchives-StaticAndDynamic.html
 *
 */

#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
// #include <unistd.h>

#include <fcntl.h>
#include <sys/mman.h>

// #include <openssl/ssl.h>
// #include <openssl/evp.h>

// void *mmap(void *addr, size_t length, int prot, int flags,
//                         int fd, off_t offset);

// popen works nice in a thread
// popen stuff
// FILE *fp;
// int status;
// char ppath[PATH_MAX];

/* need for pthreads */
#define NUM_THREADS     5

/* setup code for fps calculation */
float startclock = 0;
float deltaclock = 0;
float currentFPS = 0;

void __attribute__ ((constructor)) SG_init(void);
void __attribute__ ((destructor)) SG_fini(void);

#include <execinfo.h>
static void show_stackframe(void) {
	void *trace[32];
	char **messages = (char **)NULL;
	int i, trace_size = 0;

	trace_size = backtrace(trace, 32);
	messages = backtrace_symbols(trace, trace_size);
	printf("[bt] Execution path:\n");
	for (i=0; i < trace_size; ++i)
		printf("[bt] %s\n", messages[i]);
}

void show_stackframe2(void)
{
	int j, nptrs;
#define SIZE 100
	void *buffer[100];
	char **strings;

	nptrs = backtrace(buffer, SIZE);
	printf("backtrace() returned %d addresses\n", nptrs);

	/* The call backtrace_symbols_fd(buffer, nptrs, STDOUT_FILENO)
	 *        would produce similar output to the following: */

	strings = backtrace_symbols(buffer, nptrs);
	if (strings == NULL) {
		perror("backtrace_symbols");
		exit(EXIT_FAILURE);
	}

	for (j = 0; j < nptrs; j++)
		printf("%s\n", strings[j]);

	free(strings);
}


#include <SDL.h>
#include <smpeg/smpeg.h>
#include "SDL_mixer.h"
#include "SDL_image.h"
#include "SDL_sysvideo.h"

#define _THIS   SDL_VideoDevice *this

struct SDL_PrivateVideoData {
	int w, h;
	void *buffer;
};

struct SDL_PrivateAudioData {
	/* The file descriptor for the audio device */
	Uint8 *mixbuf;
	Uint32 mixlen;
	Uint32 write_delay;
	Uint32 initial_calls;

	float FrameTick;
	float NextFrame;
};

struct thread_data{
	void *(*func);
	SDL_Surface *t_screen;
	int  sum;
	char *command;
	char *path;
	char *UDN;
};	

#define WIDTH 960
#define HEIGHT 540
#define BPP 4
#define DEPTH 32
#define P0_ADDR 0x79900000UL
#define P1_ADDR 0x79d00000UL
#define DST_ADDR P0_ADDR
/* aligned to page boundary */
#define FB_MMAP_SIZE 0x1fb000UL

Uint32 rmask, gmask, bmask, amask;
char *SG_V_buff[WIDTH * HEIGHT * 4];
static void *SG_V_buff_real;
char *SG_A_buff[8192];
extern char* org_video_buffer;

#include "savesurf.h"
int ScCo;
void TakeShootPNG(void){
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
void TakeShootPNGSurface(SDL_Surface *surface){
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
void TakeShootJPEG(void){
	char ScreenshotName[100];
	SDL_Surface *ss_surface = SDL_CreateRGBSurface(SDL_SWSURFACE, WIDTH, HEIGHT, DEPTH, rmask, gmask, bmask, amask);
	SDL_BlitSurface(current_video->screen, NULL, ss_surface, NULL);

	sprintf(ScreenshotName,"/mtd_wiselink/Screenshot-%03d.jpg",ScCo);
	loginfo("Save %s\n", ScreenshotName);

	if(!jpeg_save_surface(ScreenshotName, ss_surface))
		ScCo++;
	else
		unlink(ScreenshotName); /* may done by write_jpeg? */
}
void TakeShootJPEGSurface(SDL_Surface *surface){
	char ScreenshotName[100];
	SDL_Surface *ss_surface = SDL_CreateRGBSurface(SDL_SWSURFACE, WIDTH, HEIGHT, DEPTH, rmask, gmask, bmask, amask);
	SDL_BlitSurface(surface, NULL, ss_surface, NULL);

	sprintf(ScreenshotName,"/mtd_wiselink/ScreenshotS-%03d.jpg",ScCo);
	loginfo("Save %s\n", ScreenshotName);

	if(!jpeg_save_surface(ScreenshotName, ss_surface))
		ScCo++;
	else
		unlink(ScreenshotName); /* may done by write_jpeg? */
}

#include <sys/time.h>
// some qemu hacks
//       int getpriority(int which, int who);
int setpriority(int which, int who, int prio){
	//	static int (*setpriority_real)(int which, int who, int prio)=NULL;
	printf("our %s: which %d who %d prio %d, NOP!\n",__FUNCTION__,which,who,prio);
	return 0;
}

// harmless/cosmetic
// #include <sched.h>
int sched_getaffinity(pid_t pid, size_t cpusetsize, cpu_set_t *mask){
	printf("our %s: NOP!\n",__FUNCTION__);
}

// #ifdef SAMSUNG
/* Abstract:
 * call to GetFrameBufferAddress() from your library indicates execution of:
 * _Z11s_GetScreeniPPcPtS1_Pi:
 */
unsigned int *GetFrameBufferAddress(void){
	char *buff;
	static unsigned int (*GetFrameBufferAddress_real)(void)=NULL;
	printf("### %s: \n",__FUNCTION__);

	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;

	/*
	 * #define SDL_VideoSurface        (current_video->screen)
	 * #define SDL_ShadowSurface       (current_video->shadow)
	 * #define SDL_PublicSurface       (current_video->visible)
	 */

	if (!GetFrameBufferAddress_real) 
		GetFrameBufferAddress_real=dlsym(RTLD_NEXT,"GetFrameBufferAddress");
	buff = GetFrameBufferAddress_real();
	show_stackframe();
	printf("%s: GetFrameBufferAddress_real 0x%08x ###\n",__FUNCTION__,buff);
	return buff;
}

SDL_Surface *SDL_SetVideoMode(int width, int height, int bitsperpixel, Uint32 flags){
        SDL_Surface *res;
        int *pwidth = &width, *pheight = &height, *pbitsperpixel = &bitsperpixel, *pflags = &flags;
        static SDL_Surface *(*SDL_SetVideoMode_real)(int width, int height, int bitsperpixel, Uint32 flags)=NULL;
        printf("[exeDSP Emu] %s w %d h %d c %d flags 0x%08x\n",__FUNCTION__,width,height,bitsperpixel,flags);
        if (!SDL_SetVideoMode_real) SDL_SetVideoMode_real=dlsym(RTLD_NEXT,"SDL_SetVideoMode");
        printf("[exeDSP Emu] %s: and now call SDL_SetVideoMode_real\n",__FUNCTION__);
        *pflags = SDL_HWSURFACE;
        *pwidth = 960;
        *pheight = 540;
//        *pbitsperpixel = 32;
        printf("[exeDSP Emu] %s New!: w %d h %d c %d flags 0x%08x\n",__FUNCTION__,width,height,bitsperpixel,flags);
        res = SDL_SetVideoMode_real(width,height,bitsperpixel,flags);
        printf("[exeDSP Emu] %s: ready to return surface at %p ####\n",__FUNCTION__,res);
        return res;
}

int SDL_LockYUVOverlay(SDL_Overlay *overlay){
	int res;
	static int (*SDL_LockYUVOverlay_real)(SDL_Overlay *overlay)=NULL;
	printf("[exeDSP Emu] %s: overlay at %p\n",__FUNCTION__,overlay);
	if (!SDL_LockYUVOverlay_real) SDL_LockYUVOverlay_real=dlsym(RTLD_NEXT,"SDL_LockYUVOverlay");
	printf("[exeDSP Emu] and now call original, res: ");
	res = SDL_LockYUVOverlay_real(overlay);
	printf("%d\n",res);
	return res;
}

SDL_Overlay *SDL_CreateYUVOverlay(int width, int height, Uint32 format, SDL_Surface *display){
	SDL_Overlay *res;
	static SDL_Overlay *(*SDL_CreateYUVOverlay_real)(int width, int height, Uint32 format, SDL_Surface *display)=NULL;
	printf("[exeDSP Emu] %s: w %d h %d format %u on display at %p\n",__FUNCTION__,width,height,format,display);
	if (!SDL_CreateYUVOverlay_real) SDL_CreateYUVOverlay_real=dlsym(RTLD_NEXT,"SDL_CreateYUVOverlay");
	printf("[exeDSP Emu] and now call original, -> ");
	res = SDL_CreateYUVOverlay_real(width,height,format,display);
	printf("[exeDSP Emu] %s: ready overlay at %p on display at %p\n",res,display);
	return res;
}

// #include "SDL_thread.h"
#if !SDL_THREAD_PTHREAD_RECURSIVE_MUTEX && \
	!SDL_THREAD_PTHREAD_RECURSIVE_MUTEX_NP
#define FAKE_RECURSIVE_MUTEX
#endif

struct SDL_mutex {
	pthread_mutex_t id;
#if FAKE_RECURSIVE_MUTEX
	int recursive;
	pthread_t owner;
#endif
};

SDL_mutex *SDL_CreateMutex(void){
	SDL_mutex *res = NULL;
	static SDL_mutex *(*SDL_CreateMutex_real)(void)=NULL;
	printf("[exeDSP Emu] %s: \n",__FUNCTION__);
	if (!SDL_CreateMutex_real) SDL_CreateMutex_real=dlsym(RTLD_NEXT,"SDL_CreateMutex");
	printf("[exeDSP Emu] %s: and now call original\n",__FUNCTION__);
	res = SDL_CreateMutex_real();
	printf("[exeDSP Emu] %s: ready mutex at %p value %p\n",__FUNCTION__,&res,res);
	return res;
}
int SDL_mutexP(SDL_mutex *mutex){
	int res = 0;
	static int (*SDL_mutexP_real)(SDL_mutex *mutex)=NULL;
	printf("[exeDSP Emu] %s: mutex at %p\n",__FUNCTION__,mutex);
	if (!SDL_mutexP_real) SDL_mutexP_real=dlsym(RTLD_NEXT,"SDL_mutexP");
	printf("[exeDSP Emu] %s: and now call original\n",__FUNCTION__);
	res = SDL_mutexP_real(mutex);
	printf("[exeDSP Emu] %s: ready mutex at %p id %d\n",__FUNCTION__,&mutex,mutex->id);
	return res;
}
int SDL_mutexV(SDL_mutex *mutex){
	int res = 0;
	static int (*SDL_mutexV_real)(SDL_mutex *mutex)=NULL;
	printf("[exeDSP Emu] %s: mutex at %p\n",__FUNCTION__,mutex);
	if (!SDL_mutexV_real) SDL_mutexV_real=dlsym(RTLD_NEXT,"SDL_mutexV");
	printf("[exeDSP Emu] %s: and now call original\n",__FUNCTION__);
	res = SDL_mutexV_real(mutex);
	printf("[exeDSP Emu] %s: ready mutex at %p id %d\n",__FUNCTION__,&mutex,mutex->id);
	return res;
}

int SDL_Init(Uint32 flags){
	int res;
	static int (*SDL_Init_real)(Uint32 flags)=NULL;
	printf("[exeDSP Emu] %s: flags 0x%08x\n",__FUNCTION__,flags);
	flags = SDL_INIT_VIDEO;
	flags |= SDL_INIT_AUDIO;
	printf("[exeDSP Emu] %s: new flags 0x%08x\n",__FUNCTION__,flags);
	if (!SDL_Init_real) SDL_Init_real=dlsym(RTLD_NEXT,"SDL_Init");
	printf("[exeDSP Emu] %s: and now call SDL_Init_real\n",__FUNCTION__);
	res = SDL_Init_real(flags);
	printf("[exeDSP Emu] %s: current_video->name %s \n",__FUNCTION__,current_video->name);
	printf("[exeDSP Emu] %s: orig res: %d #####\n",__FUNCTION__,res);

	/* at beginning of loop */
	startclock = SDL_GetTicks();
	// GPlayer_TDiObjectCreate();
	// _Z23GPlayer_TDiObjectCreatev();
	return res;
}

int SDL_InitSubSystem(Uint32 flags){
	int res;
	static int (*SDL_InitSubSystem_real)(Uint32 flags)=NULL;
	printf("##### %s flags 0x%08x\n",__FUNCTION__,flags);
	if (!SDL_InitSubSystem_real) SDL_InitSubSystem_real=dlsym(RTLD_NEXT,"SDL_InitSubSystem");
	printf("%s: and now call SDL_InitSubSystem\n",__FUNCTION__);
	res = SDL_InitSubSystem_real(flags);
	printf("%s: orig res: %d #####\n",__FUNCTION__,res);
	return res;
}

void SDL_QuitSubSystem(Uint32 flags){
	static void (*SDL_QuitSubSystem_real)(Uint32 flags)=NULL;
	printf("##### %s flags 0x%08x\n",__FUNCTION__,flags);
	if (!SDL_QuitSubSystem_real) SDL_QuitSubSystem_real=dlsym(RTLD_NEXT,"SDL_QuitSubSystem");
	printf("%s: and now call SDL_QuitSubSystem\n",__FUNCTION__);
	SDL_QuitSubSystem_real(flags);
	printf("%s: done?\n",__FUNCTION__);
}

int SDL_VideoInit(const char *driver_name, Uint32 flags){
	int res = 0;
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
	/*
		current_video->screen = SDL_CreateRGBSurface(SDL_SWSURFACE, HEIGHT, WIDTH, DEPTH, 
			rmask, gmask, bmask, amask);
			current_video->shadow = SDL_CreateRGBSurface(SDL_SWSURFACE, HEIGHT, WIDTH, DEPTH, 
			rmask, gmask, bmask, amask);
			current_video->visible = SDL_CreateRGBSurface(SDL_SWSURFACE, HEIGHT, WIDTH, DEPTH, 
			rmask, gmask, bmask, amask);
			current_video->hidden->buffer = org_video_buffer; */
			return res;
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

int SDL_PollEvent(SDL_Event *event){
	static int (*SDL_PollEvent_real)(SDL_Event *event)=NULL;
	SDL_Event my_event;
	my_event = *event;
	int res = 0;
	printf("our %s\n",__FUNCTION__);
	if (!SDL_PollEvent_real) SDL_PollEvent_real=dlsym(RTLD_NEXT,"SDL_PollEvent");
	/* Check for events */
	printf("our %s: call PumpEvents\n",__FUNCTION__);
	SDL_PumpEvents();
	printf("our %s: called PumpEvents\n",__FUNCTION__);
//	while(SDL_PollEvent_real(&my_event)) {  /* Loop until there are no events left on the queue */
	while(SDL_PollEvent_real(event)) {  /* Loop until there are no events left on the queue */
	//	switch(my_event.type) { /* Process the appropriate event type */
		switch(event->type) { /* Process the appropriate event type */
			case SDL_KEYDOWN:  /* Handle a KEYDOWN event */
				printf("Oh! Key press\n");
				break;
			case SDL_KEYUP:  
				printf("Oh! Key Up\n");
				break;
			case SDL_MOUSEMOTION:
				printf("Oh! Mouse motion\n");
				break;
			case SDL_MOUSEBUTTONDOWN:
				printf("Oh! Button press\n");
				break;
			case SDL_MOUSEBUTTONUP:  
				printf("Oh! Button Up\n");
				break;
			case SDL_ACTIVEEVENT:  
				printf("Oh! Active\n");
				break;
			case SDL_QUIT:
				printf("Oh! Realy quit?\n");
				exit(0);
			case SDL_VIDEORESIZE:
				printf("User resized video mode\n");
				break;
			case SDL_VIDEOEXPOSE:
				printf("Screen needs to be redrawn\n");
				break;
			case SDL_JOYAXISMOTION:
			case SDL_JOYBALLMOTION:
			case SDL_JOYHATMOTION:
			case SDL_JOYBUTTONDOWN:
			case SDL_JOYBUTTONUP:
				printf("Oh! We have Joystick?\n");
				break;
			case SDL_USEREVENT:
				printf("USEREVENT!!!\n");
				break;
			case SDL_SYSWMEVENT:
				printf("Oh! We have a Window-Manager???\n");
				break;

			default: /* Report an unhandled event */
				printf("I don't know what this event is! (0x%08x)\n",my_event.type);
		}
	
	}
	printf("No events left on the queue.\n");	
	return res;
}

int SDL_FillRect(SDL_Surface *dst, SDL_Rect *dstrect, Uint32 color){
	int res = 0;
	static int (*SDL_FillRect_real)(SDL_Surface *dst, SDL_Rect *dstrect, Uint32 color)=NULL;
	printf("our %s\n",__FUNCTION__);
	if (!SDL_FillRect_real) SDL_FillRect_real=dlsym(RTLD_NEXT,"SDL_FillRect");
	res = SDL_FillRect_real(dst,dstrect,color);
	show_stackframe();
	if(!res)
		printf("%s: surface, refcount %d color 0x%08x\n",__FUNCTION__,dst->refcount,color);
	else
		printf("%s: failed\n",__FUNCTION__);
	return res;
}

int SDL_LockSurface(SDL_Surface *surface){
	int res = 0;
	static int (*SDL_LockSurface_real)(SDL_Surface *surface)=NULL;
	printf("our %s\n",__FUNCTION__);
	if (!SDL_LockSurface_real) SDL_LockSurface_real=dlsym(RTLD_NEXT,"SDL_LockSurface");
	        res = SDL_LockSurface_real(surface);
		if(!res)
			printf("%s: surface locked, refcount %d\n",__FUNCTION__,surface->refcount);
		else
			printf("%s: lock failed\n",__FUNCTION__);
		return res;
}

void SDL_UnlockSurface(SDL_Surface *surface){
	static int (*SDL_UnlockSurface_real)(SDL_Surface *surface)=NULL;
	printf("our %s\n",__FUNCTION__);
	if (!SDL_UnlockSurface_real) SDL_UnlockSurface_real=dlsym(RTLD_NEXT,"SDL_UnlockSurface");
	SDL_UnlockSurface_real(surface);
}

/* need to run in standalone mode */
// s_GetScreen(int, char**, unsigned short*, unsigned short*, int*)                                                         
SDL_Surface *_Z11s_GetScreeniPPcPtS1_Pi(_THIS, char **b, unsigned short *c, unsigned short *d, int *e){                     
	printf("%s: a %d at %p b at %p c %u d %u e %d\n",__FUNCTION__, this,&this, *b, *c, *d, *e); 
	// *b = SG_V_buff;
	*b = SG_V_buff_real;
	printf("%s: a %d b at %p c %u d %u e %d\n",__FUNCTION__, this, *b, *c, *d, *e); 
	show_stackframe2();
	return SDL_GetVideoSurface();
}


/* may for furter analyze */
// SPCScreen::EnableScreen(int)
SDL_VideoDevice *_ZN9SPCScreen12EnableScreenEi(_THIS){
	show_stackframe();
	printf("%s: a %d at %p\n",__FUNCTION__, this, &this);
	printf("%s: screen at %p \n",__FUNCTION__, current_video->screen);
	printf("%s: hidden at %p \n",__FUNCTION__, current_video->hidden);
	printf("%s: visible at %p \n",__FUNCTION__, current_video->visible);
	printf("%s: shadow at %p \n",__FUNCTION__, current_video->shadow);
	printf("%s: current_video at %p \n",__FUNCTION__, current_video);
	// fake hw info
//	current_video->info.current_w = WIDTH;
//	current_video->info.current_h = HEIGHT;
//	current_video->info.hw_available = 1;
////	current_video->info.wm_available = 1;
//	current_video->info.video_mem = sizeof(SG_V_buff);
	// end fake hw info
	this = current_video;
	this->screen = SDL_CreateRGBSurface(SDL_SWSURFACE, HEIGHT, WIDTH, DEPTH,
			rmask, gmask, bmask, amask);
	this->shadow = SDL_CreateRGBSurface(SDL_SWSURFACE, HEIGHT, WIDTH, DEPTH,
			rmask, gmask, bmask, amask);
	this->visible = SDL_CreateRGBSurface(SDL_SWSURFACE, HEIGHT, WIDTH, DEPTH,
			rmask, gmask, bmask, amask);
	printf("%s: this screen at %p \n",__FUNCTION__, this->screen);
	printf("%s: current screen at %p \n",__FUNCTION__, current_video->screen);
	printf("%s: current_video->screen->pixels at %p\n",__FUNCTION__,
			current_video->screen->pixels);
	return current_video->hidden;
	// return current_video->hidden;
}
typedef SDL_VideoInfo STScreenInfo;                                                                                         
// SCShadowGraphics::GetDisplayDeviceInfo(long, STScreenInfo*)                                                              
void _ZN16SCShadowGraphics20GetDisplayDeviceInfoElP12STScreenInfo(_THIS, STScreenInfo *b){ 
	Uint8 type;
	printf("%s: a %ld at %p b at %p\n",__FUNCTION__ ,this ,&this ,&b);
	type |= SDL_KEYDOWN;
	show_stackframe();
	b = SDL_GetVideoInfo();
	show_stackframe();
	const SDL_VideoInfo* Info = SDL_GetVideoInfo();
	SDL_EventState(type, SDL_ENABLE);
	printf("%s: Resolution -> %d x %d, hw_available %d wm_available %d MemSize: %u\n",
			__FUNCTION__, 
			Info->current_w, 
			Info->current_h, 
			Info->hw_available, 
			Info->wm_available, 
			Info->video_mem);

	printf("%s: End.\n",__FUNCTION__);
}

// SCShadowGraphics::ChangeRes(long, long, long)
void *_ZN16SCShadowGraphics9ChangeResElll(long a, long w, long h){
	printf("%s: a %ld w %ld h %ld\n",__FUNCTION__, a, w, h);
	if (w > 960) {
		w=960;
		h=540;
		exit(2);
	}
	printf("%s: a %ld w %ld h %ld\n",__FUNCTION__, a, w, h);
	show_stackframe();
	// SDL_SetVideoMode(w,h,0,SDL_HWSURFACE);
}

// s_UpdateScreen(int, int, int, int, int)                                                                                  
void _Z14s_UpdateScreeniiiii(int a, int b, int c, int d, int e){                                                            
	printf("%s: a %d b %d c %d d %d e %d\n",__FUNCTION__, a, b, c, d ,e); 
	// draw routine?
	// printf("%s: skip TakeShootPNG\n");
	// TakeShootPNG();
	if(ScCo < 1000)
		TakeShootJPEG();
	else
		exit(2);
	deltaclock = SDL_GetTicks() - startclock;
	startclock = SDL_GetTicks();
	if ( deltaclock != 0 )
		currentFPS = 1000 / deltaclock;
	printf("[exeDSP Emu] FPS %0.2f\n",currentFPS);
	// memcpy(current_video->screen->pixels,current_video->hidden->buffer,sizeof(current_video->hidden->buffer));
}

#undef _THIS
#include "SDL_sysaudio.h"
#define _THIS   SDL_AudioDevice *_this

int SDL_AudioInit(const char *driver_name){
	int res;
	static int (*SDL_AudioInit_real)(const char *driver_name)=NULL;
	printf("our %s name %s\n",__FUNCTION__,driver_name);
        if (!SDL_AudioInit_real) SDL_AudioInit_real=dlsym(RTLD_NEXT,"SDL_AudioInit");
        printf("and now call original, res: ");
        res = SDL_AudioInit_real(driver_name);
        printf("%s: current_audio->name %s \n",__FUNCTION__,current_audio->name);
/*	printf("%s: current_audio->desc %s \n",__FUNCTION__,current_audio->desc);
	printf("%s: current_audio->enabled %d \n",__FUNCTION__,current_audio->enabled);
	current_audio->enabled = 1;
	printf("%s: current_audio->enabled %d \n",__FUNCTION__,current_audio->enabled);
	printf("%d, %s\n",res,driver_name);
	printf("%s: current_audio->hidden at %p\n",__FUNCTION__,
			current_audio->hidden);
	printf("%s: current_audio->hidden->mixbuf at %p\n",__FUNCTION__,
			current_audio->hidden->mixbuf);
	*SG_A_buff = calloc(1, sizeof(SG_A_buff));
	current_audio->hidden->mixbuf = SG_A_buff;
	printf("%s: current_audio->hidden->mixbuf at %p\n",__FUNCTION__,
			current_audio->hidden->mixbuf);
	printf("%s: current_audio->hidden->mixlen %d\n",__FUNCTION__,
				                        current_audio->hidden->mixlen);
	current_audio->hidden->mixlen = sizeof(SG_A_buff);
	printf("%s: current_audio->hidden->mixlen %d\n",__FUNCTION__,
				                        current_audio->hidden->mixlen);
							*/
	return res;	
}

int open_music(SDL_AudioSpec *mixer){
	int res = 0;
	static int (*open_music_real)(SDL_AudioSpec *mixer)=NULL;
	if (!open_music_real) open_music_real=dlsym(RTLD_NEXT,"open_music");
	printf("our %s mixer->freq %d mixer->format 0x%08x mixer->samples %d mixer->callback at %p mixer->userdata at %p\n",__FUNCTION__,
			mixer->freq,mixer->format,mixer->samples,mixer->callback,mixer->userdata);
	// mixer->format = AUDIO_S16MSB;
	// fake result -> whole sdl shut down
	res = open_music_real(mixer);
	printf("our %s last mix_error %s mixer->freq %d mixer->format 0x%08x mixer->samples %d mixer->callback at %p mixer->userdata at %p\n",__FUNCTION__,Mix_GetError(),
			mixer->freq,mixer->format,mixer->samples,mixer->callback,mixer->userdata);
	// fake result -> whole sdl shut down
	// res = 0;
	return res;
}

int SDL_OpenAudioPath(char *path, int maxlen, int flags, int classic){
	printf("our %s: path %s\n",__FUNCTION__,path);
}
void da_callback(void *userdata, Uint8 *stream, int len){
	printf("%s: start len %d\n",__FUNCTION__,len);
	SDL_LockAudio();
	usleep(20);
	SDL_UnlockAudio();
	printf("%s: end len %d\n",__FUNCTION__,len);
}
int SDL_OpenAudio(SDL_AudioSpec *desired, SDL_AudioSpec *obtained){
	int res = 0;
	static int (*SDL_OpenAudio_real)(SDL_AudioSpec *desired, SDL_AudioSpec *obtained)=NULL;
	if (!SDL_OpenAudio_real) SDL_OpenAudio_real=dlsym(RTLD_NEXT,"SDL_OpenAudio");
	// http://www.libsdl.org/docs/html/sdlopenaudio.html
	printf("our %s desired->freq %d desired->format 0x%08x desired->samples %d desired->callback at %p desired->userdata at %p\n",__FUNCTION__,
			desired->freq,desired->format,desired->samples,desired->callback,desired->userdata);
	if(obtained)
		printf("our %s obtained->freq %d obtained->format 0x%08x obtained->samples %d obtained->callback at %p obtained->userdata at %p\n",__FUNCTION__,
				obtained->freq,obtained->format,obtained->samples,obtained->callback,obtained->userdata);
	else
		printf("our %s obtained %p\n",__FUNCTION__,obtained);

	printf("our %s fake obtained at %p\n",__FUNCTION__,obtained);
	// if we call SDL_OpenAudio_real SDL shuts down!! (calls _ZN9TDBuilder20GetTDiAudioProcessorEi:)
	// without this call just audio subsystem will be switched off.
	// res = SDL_OpenAudio_real(desired,obtained);
//	memcpy(obtained,desired,sizeof(struct SDL_AudioSpec));
/*	obtained->freq = desired->freq;
	obtained->format = desired->format;
	obtained->samples = desired->samples;
	obtained->callback = (*printf);
	// obtained->userdata = desired->userdat;
	obtained->callback = da_callback;
	obtained->userdata = calloc(1,500); */
	if(obtained)
		printf("our %s obtained->freq %d obtained->format 0x%08x obtained->samples %d obtained->callback at %p obtained->userdata at %p\n",__FUNCTION__,
				obtained->freq,obtained->format,obtained->samples,obtained->callback,obtained->userdata);
	else
		printf("our %s obtained %p\n",__FUNCTION__,obtained);
//	printf("our %s exec callback\n",__FUNCTION__);
//	desired->callback(NULL,NULL,NULL);
	return res;
}
/*
void SDL_AudioQuit(void){
	static void (*SDL_AudioQuit_real)(void)=NULL;
	printf("our %s name %s\n",__FUNCTION__);
        if (!SDL_AudioQuit_real) SDL_AudioQuit_real=dlsym(RTLD_NEXT,"SDL_AudioQuit");
        printf("and now NOT call original: ");
//        SDL_AudioQuit_real();
        printf("%s: current_audio->name %s \n",__FUNCTION__,current_audio->name);
}
*/

void Mix_HookMusic(void (*mix_func)(void *udata, Uint8 *stream, int len), void *arg){
	static void (*Mix_HookMusic_real)(void (*mix_func)(void *udata, Uint8 *stream, int len), void *arg)=NULL;
	printf("our %s start\n",__FUNCTION__);
	if (!Mix_HookMusic_real) Mix_HookMusic_real=dlsym(RTLD_NEXT,"Mix_HookMusic");
	Mix_HookMusic_real(mix_func, &arg);
	printf("our %s end\n",__FUNCTION__);
}

int Mix_QuerySpec(int *frequency, Uint16 *format, int *channels){
	static int (*Mix_QuerySpec_real)(int *frequency, Uint16 *format, int *channels)=NULL;
	int res;
	printf("our %s start\n",__FUNCTION__);
	if (!Mix_QuerySpec_real) Mix_QuerySpec_real=dlsym(RTLD_NEXT,"Mix_QuerySpec");
	res = Mix_QuerySpec(&frequency, &format, &channels);
	printf("our %s called orig frequency %d\n",__FUNCTION__,frequency);
	return res;
}
int Mix_OpenAudio(int frequency, Uint16 format, int channels, int chunksize){
	int res = 0;
	static int (*Mix_OpenAudio_real)(int frequency, Uint16 format, int channels, int chunksize)=NULL;
	printf("our %s requested frequency %d format 0x%08x channels %d chunksize %d\n",__FUNCTION__,frequency,format,channels,chunksize);
	if (!Mix_OpenAudio_real) Mix_OpenAudio_real=dlsym(RTLD_NEXT,"Mix_OpenAudio");
	printf("and now don't call original until SDL_OpenAudio is fixed!!!! ######## %s ########\n",__FUNCTION__);
	res = Mix_OpenAudio_real(frequency,format,channels,chunksize);
	if(!res)
		printf("%s: success. current_audio->hidden at %p\n",__FUNCTION__,current_audio->hidden);
	else
		printf("%s: failed -> %s.\n",__FUNCTION__,Mix_GetError());
	return res;
}
void Mix_CloseAudio(void){
	static void (*Mix_CloseAudio_real)(void)=NULL;
	printf("our %s\n",__FUNCTION__);
	if (!Mix_CloseAudio_real) Mix_CloseAudio_real=dlsym(RTLD_NEXT,"Mix_CloseAudio");
	printf("and now call original: ");
	Mix_CloseAudio_real();
	printf(".\n");
}
Mix_Chunk *Mix_LoadWAV_RW(SDL_RWops *src, int freesrc){
	Mix_Chunk *res;
	static Mix_Chunk *(*Mix_LoadWAV_RW_real)(SDL_RWops *src, int freesrc)=NULL;
	printf("our %s cleanup request %d\n",__FUNCTION__,freesrc);
	if (!Mix_LoadWAV_RW_real) Mix_LoadWAV_RW_real=dlsym(RTLD_NEXT,"Mix_LoadWAV_RW");
	res = Mix_LoadWAV_RW_real(src,freesrc);
	if(res)
		printf("%s: success. chunk at %p\n",__FUNCTION__,res);
	else
		printf("%s: failed! Mix_LoadWAV_RW: %s\n",__FUNCTION__,Mix_GetError());
	return res;

}

void SMPEG_actualSpec(SMPEG *mpeg, SDL_AudioSpec *spec){
	printf("our %s: mpeg at %p spec at %p\n",__FUNCTION__,mpeg,spec);
	printf("our %s spec->freq %d spec->format 0x%08x spec->samples %d spec->callback at %p spec->userdata at %p\n",__FUNCTION__,
			                        spec->freq,spec->format,spec->samples,spec->callback,spec->userdata);
	spec->freq = 44100;
	spec->format = 32784;
	spec->samples = 8192; 
}

SMPEG* SMPEG_new(const char *file, SMPEG_Info* info, int sdl_audio){
	SMPEG *res = NULL;
	static SMPEG* (*SMPEG_new_real)(const char *file, SMPEG_Info* info, int sdl_audio)=NULL;
	printf("our %s file %s info at %p sdl_audio %d\n",__FUNCTION__,file,info,sdl_audio);
	res = SMPEG_new_real(file,&info,sdl_audio);
	printf("our %s info->total_time %d\n",__FUNCTION__,info->total_time);
	return res;
}

void SMPEG_play(SMPEG* mpeg){
	printf("our %s mpeg at %p\n",__FUNCTION__,mpeg);
}

void SMPEG_loop(SMPEG* mpeg, int repeat){
	printf("our %s mpeg at %p repeat %d\n",__FUNCTION__,mpeg,repeat);
}

void SMPEG_enableaudio(SMPEG* mpeg, int enable){
	static void (*SMPEG_enableaudio_real)(SMPEG* mpeg, int enable)=NULL;
	printf("our %s mpeg at %p enable %d\n",__FUNCTION__,mpeg,enable);
	if (!SMPEG_enableaudio_real) SMPEG_enableaudio_real=dlsym(RTLD_NEXT,"SMPEG_enableaudio");

}
	
// GPlayer Stuff
int VideoInit(void *a, void *b, void *c, void *d){
	int res = 0;
	static int (*VideoInit_real)(void)=NULL;
	printf("[exeDSP Emu] %s: a %p b %p c %p d %p\n",__FUNCTION__,a,b,c,d);
//	res = VideoInit_real();
	return res;
}
SDL_AudioSpec *AudioInit(int a, int b, unsigned int c, int d, int e){
	// a 2 b 16 c 22050 d 8192 e 2048 f 1074457380 g 1074258320
	SDL_AudioSpec *res = NULL;
	static SDL_AudioSpec *(*AudioInit_real)(int a, int b, unsigned int c, int d, int e)=NULL;
	printf("[exeDSP Emu] %s: a %d b %d c %d d %d e %d\n",__FUNCTION__,a,b,c,d,e);
	// SDL_InitSubSystem(SDL_INIT_AUDIO);
	// res = AudioInit_real(a,b,c,d,e);
	res->freq = c;
 //       res->format = ;
        res->samples = d;
        res->callback = (*printf);
        // obtained->userdata = desired->userdat;
	//         obtained->callback = da_callback;
	//                 obtained->userdata = 	
	return res;
}


int IMG_Init(int flags){
	int res = 0;
	static int (*IMG_Init_real)(int flags)=NULL;
	printf("%s: flags %d\n",__FUNCTION__,flags);
	if (!IMG_Init_real) IMG_Init_real=dlsym(RTLD_NEXT,"IMG_Init");
	res = IMG_Init_real(flags);
	if(res)
		printf("%s: success. res %d\n",__FUNCTION__,res);
	else
		printf("%s: failed! IMG_Load_RW: %s\n",__FUNCTION__,IMG_GetError());
	return res;
}

SDL_Surface *IMG_Load_RW(SDL_RWops *src, int freesrc){
	SDL_Surface *res;
	static SDL_Surface *(*IMG_Load_RW_real)(SDL_RWops *src, int freesrc)=NULL;
	printf("our %s cleanup request %d\n",__FUNCTION__,freesrc);
	if (!IMG_Load_RW_real) IMG_Load_RW_real=dlsym(RTLD_NEXT,"IMG_Load_RW");
	res = IMG_Load_RW_real(src,freesrc);
	if(res)
		printf("%s: success. sourface at %p\n",__FUNCTION__,res);
	else
		printf("%s: failed! IMG_Load_RW: %s\n",__FUNCTION__,IMG_GetError());
	return res;
}


/* ? funks */
// TDBuilder::GetTDiFeeder(int)
void *_ZN9TDBuilder12GetTDiFeederEi(_THIS){
// int _ZN9TDBuilder12GetTDiFeederEi(_THIS){
	printf("%s: a %d at %p\n",__FUNCTION__, _this, &_this);
//	_this = current_audio;
	return calloc(2,8192);
}
// TDBuilder::GetTDiMpeg(int)
int _ZN9TDBuilder10GetTDiMpegEi(_THIS){
// int _ZN9TDBuilder10GetTDiMpegEi(_THIS){
	printf("%s: a %d at %p\n",__FUNCTION__, _this, &_this);
	return 0;
	return calloc(1,8192);
}

// TDBuilder::GetTDiAudioProcessor(int)
int _ZN9TDBuilder20GetTDiAudioProcessorEi(_THIS){
// int _ZN9TDBuilder20GetTDiAudioProcessorEi(_THIS){
	printf("%s: a %d at %p\n",__FUNCTION__, _this, &_this);
	return 0;
	return calloc(1,8192);
}

void _Z13A1_SetSpeakerb(int a){
	printf("%s: a %d at %p\n",__FUNCTION__, a, a);
//	return 0;
}

/* never ending storie? */
void g_hGFX(void *a, void *b, void *c){
	printf("%s: a %p b %p c %p\n",__FUNCTION__,a,b,c);
}
/*   from /dtv/usb/sda4/usr/lib/libSGexeDSP.so
 *   #1  0x40bfe08c in y_HWFillRect () from /dtv/usb/sda4/usr/lib/libYahooSDLHWAcceleration.so
 *   #2  0x403a26a4 in SDL_FillRect () from /mtd_cmmlib/GAME_LIB/libSDL.so
 *   #3  0x403a55f0 in SDL_SetVideoMode () from /mtd_cmmlib/GAME_LIB/libSDL.so
 *   #4  0x4109b8f8 in ?? ()
 */
// U spiFillRect(void*, unsigned int const*, unsigned long, int, int, int, int, int)
// 00000f84 T spi_FillRect(void*, char*, unsigned long, int, int, int, int, int)
/* On TV
GetFrameBufferAddress: GetFrameBufferAddress_real 0x0002cb60 ###
_Z11spiFillRectPvPKjmiiiii: a 0xe24dd00c b 0x2cb60 color 0x00000000 d 0 e 0 f 960 g 540 h 3840
_Z11spiFillRectPvPKjmiiiii: a 0xe59fc030 a -442515408 b 0x403f97d0 color 0x00000000 d 0 e 0 f 960 g 540 h 3840
*/
int y_HWFillRect(void *a, void *b, void *c, void *d, void *e, void *f){
	printf("%s:a %p b %p c %p d %p e %p f %p\n",__FUNCTION__,a,b,c,d,e,f);
	return 0;
}

// int SDL_FillRect(SDL_Surface *dst, SDL_Rect *dstrect, Uint32 color);
void *_Z11spiFillRectPvPKjmiiiii(void *a, char *b, Uint32 color, int d, int e, int f, int g, int h){
	// b is the SG_V_buff
	printf("%s:\n",__FUNCTION__);
//	printf("%s: a %p a %d b %p color 0x%08x d %d e %d f %d g %d h %d\n",__FUNCTION__,a,a,b,color,d,e,f,g,h);
//	printf("%s: a is surface ? a->w \n",__FUNCTION__,a->w);

//	SDL_FillRect(a,&rect,color);
//	return 0;
}

// called by libAe.so
void _ZN6PCTask6t_MainEv(void *a){
	printf("%s:\n",__FUNCTION__);
}
void _ZN6PCTask7DestroyEv(void *a){
	printf("%s:\n",__FUNCTION__);
}
void _ZThn36_N6PCTask7DestroyEv(void *a){
	printf("%s:\n",__FUNCTION__);
}
void _ZThn5196_N6PCTask7DestroyEv(void *a){
	printf("%s:\n",__FUNCTION__);
}
void _ZTI6PCTask(void *a){
	printf("%s:\n",__FUNCTION__);
}
void spaceCode(void *a){
	printf("%s:\n",__FUNCTION__);
}
void RegisterCallback(void *a){
	printf("%s:\n",__FUNCTION__);
}

// called by libWsal.so
void _ZN8SCWindow9t_OnEventEPK7PTEvent(void *a){
	printf("%s:\n",__FUNCTION__);
}

// from exeDSP -> called from libWsal.so
void _ZN8SCWidget10t_OnExposeEPK7PTEvent(void *a){
	printf("%s:\n",__FUNCTION__);
}

void _ZN8SCButton19t_OnShowStateChangeEPK7PTEvent(void *a){
	printf("%s:\n",__FUNCTION__);
}

void _ZN8SCWidget2GCEb(void){
	printf("%s:\n",__FUNCTION__);
}

void _ZN8SCWidget12t_ApplyBgAniEi(int a){
	printf("%s:\n",__FUNCTION__);
}

void _ZN8SCButton18t_OnContentsExposeEPK7PTEvent(void *a){
	printf("%s:\n",__FUNCTION__);
}

// #endif

// int InputKeyState, InputKeyData;

void *CallGirl(void *threadarg){
	struct thread_data *my_data;
	char *my_UDN, *my_path;
	int (*my_fn)(char *, char *);

	my_data = (struct thread_data *)threadarg;
	my_fn = my_data->func;
	my_UDN = my_data->UDN;
	my_path = my_data->path;
	(*my_fn)(my_path, my_UDN);
	pthread_exit(NULL);
}

int main(int argc, const char *argv[])
{
        pthread_t threads[NUM_THREADS];
        pthread_attr_t attr;
	struct thread_data thread_data_array;
	void *lib_handle;
	char *UDN = "1111-1111-1111-1111";
	char *error, *lib_dir, lib_toopen[100], *lib_name;
	int (*fn)(char *, char *), len, rc;

	/* For portability, explicitly create threads in a joinable state */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

//	*SG_V_buff = calloc(1, WIDTH * HEIGHT * 4);
//	org_video_buffer = SG_V_buff;

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

	chdir(lib_dir);

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
//	SDL_Init(SDL_INIT_EVERYTHING|SDL_INIT_NOPARACHUTE|SDL_INIT_EVENTTHREAD);
//	SDL_SetVideoMode(WIDTH, HEIGHT, DEPTH, SDL_HWSURFACE);
	/* lib_name, normaly uidn is passed to function
	 * have to figure out what it realy is
	 */
		      (*fn)(lib_dir, UDN);
		      pthread_exit(NULL);
	thread_data_array.func = fn;
	thread_data_array.path = lib_dir;
	thread_data_array.UDN = UDN;
	rc = pthread_create(&threads[0], &attr, CallGirl, &thread_data_array);
	printf("return code from pthread_create() is %d\n", rc);
	pthread_join(threads[0], NULL);

	dlclose(lib_handle);

	pthread_attr_destroy(&attr);
        pthread_exit(NULL);
}

/* from libTD.so -> pulls _ZN8PCThread7DestroyEv from libRCE.a or exeDSP ? */
extern void _ZN17CTDMediaComponent16StartVideoDeviceEv(void);
void _ZN8PCThread7DestroyEv(void){
	printf("%s:\n",__FUNCTION__);
}
/* _ZN15CMediaComponent8FindPortEPcPP10IMediaPort from libMMFCore.so */
/* libMMFCore.so: undefined symbol: _ZTI8PCThread */
/* typedef void *(*PCThread){
	void *(*SetPriority)(int);
	void *(*Create)(unsigned long);
	void *(*Destroy)(void);
}PCThread; */
typedef void PCThread(void);

PCThread *_ZTI8PCThread(void){
	printf("%s:\n",__FUNCTION__);
}
PCThread *_ZTV8PCThread(void){
	        printf("%s:\n",__FUNCTION__);
}

/* libMMFCore.so: undefined symbol: _ZTV11PCSemaphore */
typedef void PCSemaphore(void);
PCSemaphore *_ZTV11PCSemaphore(void){
	        printf("%s:\n",__FUNCTION__);
}


void SG_init(void){
	int fd;

	printf("%s: Load\n",__FUNCTION__);
	chdir("/dtv");
	umask(0);
	
	// SDLPorting_Test();
	// _Z15SDLPorting_Testv();

	current_audio = NULL;
	current_video = NULL;

	//	SSL_load_error_strings();                /* readable error messages */
	//	SSL_library_init();                      /* initialize library */
	//	OpenSSL_add_all_ciphers();

	if ((fd = open("/dev/mem", O_RDWR | O_SYNC)) == -1) 
		printf("%s: failed to open /dev/mem\n",__FUNCTION__);

	SG_V_buff_real = mmap(0, FB_MMAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, DST_ADDR);
	if (!SG_V_buff_real)
		printf("%s: nothing mapped\n",__FUNCTION__);
	else
		printf("%s: we have a real buff at %p\n",__FUNCTION__,SG_V_buff_real);
	
	*SG_V_buff = calloc(1, WIDTH * HEIGHT * 4);
		printf("%s: we have a buff at %p deref: %08x\n",__FUNCTION__,SG_V_buff,*SG_V_buff);
	org_video_buffer = SG_V_buff;

//	_ZN17CTDMediaComponent16StartVideoDeviceEv();
//	_THIS;
//	_ZN6PCTask6t_MainEv(_this);
}

void SG_fini(void){
	/* thread-safe cleanup */
	//	ENGINE_cleanup();
	//	CONF_modules_unload(1);

	/* global application exit cleanup (after all SSL activity is shutdown) */
	//	ERR_free_strings();
	//	EVP_cleanup();
	//	CRYPTO_cleanup_all_ex_data();
	munmap(SG_V_buff_real,FB_MMAP_SIZE);	
	printf("%s: unload\n",__FUNCTION__);
}

