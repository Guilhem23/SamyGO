/* 

    TestVNC - SDL_vnc Sample Client

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>

#include <SDL/SDL.h>

#include "SDL_vnc.h"

#define DEFAULT_W	960
#define DEFAULT_H	540

/* Commandline configurable items */

char *vnc_server = NULL;
int   vnc_port = 5900;
char *vnc_method = NULL;
char *vnc_password = NULL;
int   vnc_framerate = 12;

/* Drawing loop */

void draw(SDL_Surface *screen, tSDL_vnc *vnc)
{
 SDL_Event event; 
 SDL_Rect updateRect;
 int inloop;
 Uint8 mousebuttons, buttonmask;
 int mousex, mousey;
 Uint32 key;
   
 /* Black screen */
 SDL_FillRect(screen,NULL,0);
 SDL_UpdateRect(screen,0,0,0,0);
  
 inloop=1;
 while (inloop) {
    
  /* Check for events */
  while ( SDL_PollEvent(&event) ) {
   switch (event.type) {

    case SDL_KEYDOWN:
    case SDL_KEYUP:
     /* Map SDL key to VNC key */
     key = event.key.keysym.sym;
     switch (event.key.keysym.sym) {
      case SDLK_BACKSPACE: key=0xff08; break;
      case SDLK_TAB: key=0xff09; break;
      case SDLK_RETURN: key=0xff0d; break;
      case SDLK_ESCAPE: key=0xff1b; break;
      case SDLK_INSERT: key=0xff63; break;
      case SDLK_DELETE: key=0xffff; break;
      case SDLK_HOME: key=0xff50; break;
      case SDLK_END: key=0xff57; break;
      case SDLK_PAGEUP: key=0xff55; break;
      case SDLK_PAGEDOWN: key=0xff56; break;
      case SDLK_LEFT: key=0xff51; break;
      case SDLK_UP: key=0xff52; break;
      case SDLK_RIGHT: key=0xff53; break;
      case SDLK_DOWN: key=0xff54; break;
      case SDLK_F1: key=0xffbe; break;
      case SDLK_F2: key=0xffbf; break;
      case SDLK_F3: key=0xffc0; break;
      case SDLK_F4: key=0xffc1; break;
      case SDLK_F5: key=0xffc2; break;
      case SDLK_F6: key=0xffc3; break;
      case SDLK_F7: key=0xffc4; break;
      case SDLK_F8: key=0xffc5; break;
      case SDLK_F9: key=0xffc6; break;
      case SDLK_F10: key=0xffc7; break;
      case SDLK_F11: key=0xffc8; break;
      case SDLK_F12: key=0xffc9; break;
      case SDLK_LSHIFT: key=0xffe1; break;
      case SDLK_RSHIFT: key=0xffe2; break;
      case SDLK_LCTRL: key=0xffe3; break;
      case SDLK_RCTRL: key=0xffe4; break;
      case SDLK_LMETA: key=0xffe7; break;
      case SDLK_RMETA: key=0xffe8; break;
      case SDLK_LALT: key=0xffe9; break;
      case SDLK_RALT: key=0xffea; break;
      case SDLK_POWER: inloop=0; break;
      default: key = event.key.keysym.sym;
     }
     /* Handle upper case letters. */
     if (event.key.keysym.mod & KMOD_SHIFT) {
      key=toupper(key);
     }
     /* Add client event */
     vncClientKeyevent(vnc, (event.type==SDL_KEYDOWN), key);
     break;

    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
    case SDL_MOUSEMOTION:
     /* Get current mouse state */
     mousebuttons=SDL_GetMouseState(&mousex,&mousey);
     /* Map SDL buttonmask to VNC buttonmask */
     buttonmask=0;
     if (mousebuttons & SDL_BUTTON(SDL_BUTTON_LEFT)) buttonmask       |= 1;
     if (mousebuttons & SDL_BUTTON(SDL_BUTTON_MIDDLE)) buttonmask     |= 2;
     if (mousebuttons & SDL_BUTTON(SDL_BUTTON_RIGHT)) buttonmask      |= 4;
     if (mousebuttons & SDL_BUTTON(SDL_BUTTON_WHEELUP)) buttonmask    |= 8;
     if (mousebuttons & SDL_BUTTON(SDL_BUTTON_WHEELDOWN)) buttonmask  |= 16;
     /* Add client event */
     vncClientPointerevent(vnc, buttonmask, mousex, mousey);
     break;

    case SDL_QUIT:
     inloop=0;
     break;
    }
   }

   /* Blit VNC screen */
   if (vncBlitFramebuffer(vnc, screen, &updateRect) ) {
    /* Display by updating changed parts of the display */
    SDL_UpdateRect(screen,updateRect.x,updateRect.y,updateRect.w,updateRect.h);
   }
    
   /* Delay to limit rate */                   
   SDL_Delay(1000/vnc_framerate);
   
 }
}

int Game_Main(char *path, char *udn)
{
	SDL_Surface *screen;
	int w, h;
	int desired_bpp;
	Uint32 video_flags;
	tSDL_vnc vnc;

	chdir(path);

	/* Title */
	fprintf (stderr,"SDL_vnc Sample VNC Client - LGPL, A. Schiffler, aschiffler@appwares.com\n\n");

	/* Set default options and check command-line */
	w = DEFAULT_W;
	h = DEFAULT_H;
	desired_bpp = 32;
	video_flags = SDL_FULLSCREEN;
	video_flags |= SDL_HWSURFACE;
	/* Force double buffering */
	video_flags |= SDL_DOUBLEBUF;
	vnc_server = "127.0.0.1";
	vnc_port = 5900;
	vnc_method = strdup("hextile,rre,copyrect,raw");
	vnc_password = strdup("");
	vnc_framerate = 25;

	/* Initialize SDL */
	if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
		return(0);
	}

	/* Initialize the display */
	screen = SDL_SetVideoMode(w, h, desired_bpp, video_flags);
	if ( screen == NULL ) {
		fprintf(stderr, "Couldn't set %dx%dx%d video mode: %s\n", w, h, desired_bpp, SDL_GetError());
		return(0);
	}

	/* Check for double buffering */
	if ( screen->flags & SDL_DOUBLEBUF ) {
		printf("Double-buffering enabled - good!\n");
	}

	/* Open vnc connection */
	vncConnect(&vnc,vnc_server,vnc_port,vnc_method,vnc_password,vnc_framerate);

	/* Do all the drawing work */
	draw(screen, &vnc);
	
	/* Close connection */
	vncDisconnect(&vnc);
	
	return(0);
}
