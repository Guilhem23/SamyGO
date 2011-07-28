// autobright-ui.c
//
// LCD backlight auto adjusting in accordance with ambient light level
// User interface for setting conversion from sensor reading to backlight level
//
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dlfcn.h>
#include <sys/stat.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_gfxPrimitives.h>

#include "autobright.h"

///////////////////////////////////////////////////////////////////////////////

#define WIDTH 960 
#define HEIGHT 540
#define BPP 2
#define DEPTH 16

#if BPP == 2
  typedef Uint16 pixel_t;
#elif BPP == 4
  typedef Uint32 pixel_t;
#endif 

#define FONT_SIZE 24

const int ymin= HEIGHT - 80;
const int ymax= 70;
const int xmin= 200;
const int wtest= 140;
const int val_min= 0;
const int val_max= 127;
int run;
int focus= -1;
int sliders;
unsigned lastLight;
unsigned measureCount;
Uint8 pendingEvents;

void *hLib;
sensorCallBack_t *pSensorCallBack;
aproxTable_t *pPhotoSensorConv;
int tabDirty;
int *pBackLight;
int reversedConv;

TTF_Font *font;

SDL_Surface *background;

SDL_Color c_yellow={ 255, 255, 0 };
SDL_Color c_white={ 255, 255, 255 };

///////////////////////////////////////////////////////////////////////////////

float XByTable(aproxTable_t *table, float x, int colWidth)
{
  aproxTable_t point1, point2;
  int i;

  for (i= 0; i < MAX_SENSOR_CONV; i++) {
    memcpy(&point2, &(table[i]), sizeof(point2));
    if (point2.x >= x) {
      if (i == 0) {
        return 0;
      } else {
        return ((x - point1.x) / (point2.x - point1.x) + i - 1) * colWidth;
      }
    }
    memcpy(&point1, &point2, sizeof(point2));
  }
  return 0; 
}

int ConvertY(int val)
{
  if (val <= val_min) return ymin;
  if (val >= val_max) return ymax;
  return (int)((float)(val - val_min) / (val_max - val_min) * (ymax - ymin) + ymin);  
}

aproxTable_t* PtrToConvTable(int i)
{
  if (reversedConv) {
    return &pPhotoSensorConv[sliders - 1 - i]; 
  } else {
    return &pPhotoSensorConv[i]; 
  }  
}

///////////////////////////////////////////////////////////////////////////////

void DrawSensor(SDL_Surface* screen, int x)
{
  const int y= ymax - 30;
  const int w1= 8; 
  const int h1= 8 + 2 * (measureCount % 4); // animated arrow indicates sensor data readings 
  const int w2= 12; 
  int h2= 12 + h1; 
  Sint16 ax[7]= { x-w1, x-w1, x-w2, x,  x+w2, x+w1, x+w1 };
  Sint16 ay[7]= { y,   y+h1, y+h1, y+h2, y+h1, y+h1, y };
  filledPolygonRGBA(screen, ax, ay, 7, 50, 255, 50, 255);
} 

void DrawSlider(SDL_Surface* screen, int x, int y, int focus)
{
  const int w= 2;
  const int ws= 6;
  const int hs= 8;
  const int wf= ws+4; 
  const int hf= hs+4; 
  boxRGBA(screen, x-w, ymin, x+w, ymax, 99, 99, 99, 255);
  boxRGBA(screen, x-ws, y-hs, x+ws, y+hs, 200, 200, 200, 255);
  if (focus) rectangleRGBA(screen, x-wf, y-hf, x+wf, y+hf, 255, 255, 0, 255);
}

///////////////////////////////////////////////////////////////////////////////

void DrawBackground(SDL_Surface* screen)
{ 
	int i, x, y, h, r, g, b, c, c2;
	pixel_t *pixmem;
	Uint32 colour;  

	if(SDL_MUSTLOCK(screen)) 
	{
		if(SDL_LockSurface(screen) < 0) return;
	}

	for (y = 0; y < screen->h; y++) 
	{
  	pixmem = (pixel_t*)((Uint8*)screen->pixels + y*screen->pitch);
		for( x = 0; x < screen->w / 2; x++ ) 
		{
      r= 60 + 110 * (x + HEIGHT - y) / (WIDTH/2 + HEIGHT + 2);
      g= 30 + 145 * (x + HEIGHT - y) / (WIDTH/2 + HEIGHT + 1);
      b= 256 * (x + HEIGHT - y) / (WIDTH/2 + HEIGHT);
	    colour = SDL_MapRGB( screen->format, r, g, b);
    	*pixmem++ = colour;
    	*pixmem++ = colour;
		}
	}

	if(SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);

  {
    SDL_Surface *btsu= TTF_RenderText_Blended(font, "Auto-brightness setup", c_yellow);
    SDL_Rect dstrect = { (xmin + wtest) / 2, 10, 0, 0 };
    SDL_BlitSurface(btsu, NULL, screen, &dstrect);
    SDL_FreeSurface(btsu);
  }

  // teset pattern with 0%, 10%.. 90% and 100% gray levels
  h= HEIGHT / 5; 
  for (i= 0; i <= 10; i++) {
    if (i == 5) continue;
    if (i < 5) {
      x= 0;
      y= h * (4 - i);
      c2= 200;
    } else {
      x= WIDTH - wtest;
      y= h * (10 - i);
      c2= 55;
    }
    c= 255 * i / 10;
    boxRGBA(screen, x, y, x + wtest, y + h - 1, c, c, c, 255);
    hlineRGBA(screen, x, x + wtest / 4, y + h, c2, c2, c2, 255);

    char bf[80];
    sprintf(bf, "%d%%", 10*i);
    SDL_Color color= { c2, c2, c2 };
    SDL_Surface *btsu= TTF_RenderText_Blended(font, bf, color);
    SDL_Rect dstrect = { x + (wtest - btsu->w) / 2, y + (h - btsu->h) / 2, 0, 0 };
    SDL_BlitSurface(btsu, NULL, screen, &dstrect);
    SDL_FreeSurface(btsu);
  }

  {
    SDL_Surface *btsu= TTF_RenderText_Blended(font, "  Reload saved values", c_white);
    x= (xmin + wtest) / 2; 
    y= HEIGHT - btsu->h - 4;
    SDL_Rect dstrect = { x + FONT_SIZE, y, 0, 0 };
    SDL_BlitSurface(btsu, NULL, screen, &dstrect);
    SDL_FreeSurface(btsu);
    boxRGBA(screen, x, y + btsu->h/4, x + FONT_SIZE, y + btsu->h*3/4, 255, 0, 0, 255);
  }
  {
    SDL_Surface *btsu= TTF_RenderText_Blended(font, "  Save & Exit", c_white);
    x= WIDTH - (xmin + wtest) / 2 - btsu->w; 
    SDL_Rect dstrect = { x, y, 0, 0 };
    SDL_BlitSurface(btsu, NULL, screen, &dstrect);
    SDL_FreeSurface(btsu);
    boxRGBA(screen, x - FONT_SIZE, y + btsu->h/4, x, y + btsu->h*3/4, 0, 0, 255, 255);
  }
}

///////////////////////////////////////////////////////////////////////////////

void DrawScreen(SDL_Surface* screen)
{ 
	int i, dx;
  char bf[80];

  if (background) {
    SDL_BlitSurface(background, NULL, screen, NULL);
  }

  if (measureCount) {
    if (pBackLight) {
      sprintf(bf, "sensor %d   dimmer %d", lastLight, *pBackLight);
    } else {
      sprintf(bf, "sensor %d", lastLight);
    }
  } else if (hLib) {
    sprintf(bf, "No sensor data!");
  } else {
    sprintf(bf, "Can't load autobright.so!");
  }
  SDL_Surface *btsu= TTF_RenderText_Blended(font, bf, c_white);
  SDL_Rect dstrect = { WIDTH / 2, 10, 0, 0 };
  SDL_BlitSurface(btsu, NULL, screen, &dstrect);
  SDL_FreeSurface(btsu);

  dx= (WIDTH - 2 * xmin) / (sliders - 1);  
  if (pPhotoSensorConv) {
    int cy, cyn;
    cy= ConvertY(PtrToConvTable(0)->y);
    for (i= 0; i < sliders; i++) {
      if (i < sliders - 1) {
        cyn= ConvertY(PtrToConvTable(i+1)->y);
        lineRGBA(screen, xmin + dx * i, cy, xmin + dx * (i + 1), cyn, 150, 150, 150, 255);
        lineRGBA(screen, xmin + dx * i, cy + 1, xmin + dx * (i + 1), cyn + 1, 150, 150, 150, 255);
      }
      DrawSlider(screen, xmin + dx * i, cy, focus == i);
      cy= cyn;
    }
    if (measureCount) {
      int sx= XByTable(pPhotoSensorConv, lastLight, dx);
      DrawSensor(screen, reversedConv ? WIDTH - xmin - sx : sx + xmin);
    }
  }

	SDL_Flip(screen); 
}

///////////////////////////////////////////////////////////////////////////////

void SensorCallBack(unsigned light)
{
  SDL_Event ev= { SDL_USEREVENT };
  lastLight= light;
  if (pendingEvents == 0) {
    SDL_PushEvent(&ev);
    pendingEvents++;
  }
}

void SaveConvTable(void)
{
  int i;
  FILE* f= fopen(CONF_PATH, "w");
  if (! f) {
    printf("autobright-ui: Can't write to %s\n", CONF_PATH);
    return;
  }
  for (i= 0; i < MAX_SENSOR_CONV; i++) {
    if (pPhotoSensorConv[i].x == 0.0 && pPhotoSensorConv[i].y == 0.0) break;
    fprintf(f, "%.0f %.0f\n", pPhotoSensorConv[i].x, pPhotoSensorConv[i].y);
    if (pPhotoSensorConv[i].x >= 65535.0) break;
  }
  fclose(f);
}

void Uninit(void)
{
//  printf("**autobright-ui Uninit\n");
  run= 0;

  if (pSensorCallBack) {
    *pSensorCallBack= NULL;
  }
  
  if (hLib) {
    dlclose(hLib);
    hLib= NULL;
  }

  if (font) {
    TTF_CloseFont(font);
    font= NULL;
  }

  if (background) {
    SDL_FreeSurface(background);
    background= NULL;
  }

	SDL_Quit();
}

///////////////////////////////////////////////////////////////////////////////

int Game_Main(const char *path, const char *udn)
{
  int i;
	SDL_Surface *screen;
  char *fpath= "/mtd_appdata/Font/shadow.ttf";
  char libpath[256];
  struct stat sts;

  atexit(Uninit);  
  printf("**autobright-ui Game_Main\n");
  if (stat(fpath, &sts)) {
    printf("TTF font not found\n");
    return(-1);
  }

	if (SDL_Init(SDL_INIT_VIDEO) < 0 ) {
    printf("SDL_Init failed\n");
		SDL_Quit();  // without Quit TV cannot restart launcher app
                // Quit throws segmentation fault, but launcher can continue
    return 1;
  }
	screen= SDL_SetVideoMode(WIDTH, HEIGHT, DEPTH, SDL_HWSURFACE);
  if (! screen) {  
    printf("SDL_SetVideoMode failed\n");
		SDL_Quit();
		return 1;
	}

  if (TTF_Init()) {
    printf("TTF_Init failed\n");
		SDL_Quit();
		return 1;
	}

  font= TTF_OpenFont(fpath, FONT_SIZE);
  if (! font) {
    printf("TTF_OpenFont failed\n");
		SDL_Quit();
		return 1;
	}

	sprintf(libpath, "%s%s", path, "autobright.so");
  hLib= dlopen(libpath, RTLD_LAZY | RTLD_NODELETE);
  if (hLib) {
    pSensorCallBack= dlsym(hLib, "sensorCallBack");
    pPhotoSensorConv= dlsym(hLib, "photoSensorConv");
    pBackLight= dlsym(hLib, "backlight");
  } else {
    char *err= dlerror();
    if (err) printf("dlopen %s: %s\n", libpath, err);
    else printf("dlopen %s error\n", libpath);
  }    
  if (pSensorCallBack) {
    *pSensorCallBack= SensorCallBack;
  }    
  if (pPhotoSensorConv) {
    reversedConv= (pPhotoSensorConv[0].y > pPhotoSensorConv[1].y);
    for (i= 0; i < MAX_SENSOR_CONV; i++) {
      if (pPhotoSensorConv[i].x == 65535) {
        i++;
        break;
      }
      if (pPhotoSensorConv[i].x >= 65536) break; // not reachable value
      if (pPhotoSensorConv[i].x == 0.0 && pPhotoSensorConv[i].y == 0.0) break;
    }
    sliders= i;
    printf("%d sliders, axis x %s\n", sliders, reversedConv ? "reversed" : "normal");
  } 

  background= SDL_CreateRGBSurface(SDL_HWSURFACE, screen->w, screen->h, screen->format->BitsPerPixel, 0, 0, 0, 0);
  if (background) {
    DrawBackground(background);
    DrawScreen(screen);
  }

  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

  run= 1;
  while (run) {
    SDL_Event ev;
    if (SDL_PollEvent(&ev)) {
//      printf("event %d\n", ev.type);
      if (ev.type == SDL_USEREVENT) {
        pendingEvents--;
        measureCount++;
        if (! SDL_PollEvent(&ev)) {
          if (pPhotoSensorConv && focus == -1) {
            for (i= 0; i < MAX_SENSOR_CONV - 1; i++) {
              if (pPhotoSensorConv[i].x + pPhotoSensorConv[i+1].x > lastLight * 2) break;
            }
            focus= reversedConv ? sliders - 1 - i : i;
          } 
          DrawScreen(screen);
        }
      }      
      if (ev.type == SDL_KEYDOWN) {
//        printf("key down %d\n", ev.key.keysym.sym);
        switch (ev.key.keysym.sym) {

        case SDLK_LEFT:
          if (focus > 0) focus--;
          else if (focus == -1) focus= sliders - 1;
          break;
        case SDLK_RIGHT:
          if (focus < sliders - 1) focus++;
          break;

        case SDLK_UP:
          if (PtrToConvTable(focus)->y < val_max) PtrToConvTable(focus)->y++;
          tabDirty= 1;
          break;
        case SDLK_DOWN:
          if (PtrToConvTable(focus)->y > val_min) PtrToConvTable(focus)->y--;
          tabDirty= 1;
          break;

        case SDLK_ESCAPE:
//          printf("**autobright-ui SDLK_ESCAPE\n");
          if (tabDirty && pPhotoSensorConv) {
            SaveConvTable();
          }
          run= 0;
          break;

        case SDLK_HOME:
          {
            void (*pLoadApproxTable) (void);
            pLoadApproxTable= dlsym(hLib, "LoadApproxTable");
            if (pLoadApproxTable) pLoadApproxTable();
          }
          
          break;

        case SDLK_POWER:  // received after TV channel changing keys (P+, P-, PreCh) 
          run= 0;
          break;

        default:
          break;                  
        }
        DrawScreen(screen);
      }
    }    
  }

  printf("**autobright-ui call Uninit\n");
  Uninit();

	return 0;
}
