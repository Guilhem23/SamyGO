// launcher.c
//
// libGame.so launcher (restores the function blocked in exeDSP)
// for Samsung TV with T-RBYDEUC firmware
//
///////////////////////////////////////////////////////////////////////////////

#define _GNU_SOURCE
#include <stdio.h>
#include <setjmp.h>
#include <signal.h>
#include <string.h>
#include <dlfcn.h>
#include <errno.h>
#include <unistd.h>
#include <sys/mman.h>
#include <math.h>

#include "exedsp.h"
#include "exedsp-abs.h"

///////////////////////////////////////////////////////////////////////////////

static sigjmp_buf env;

///////////////////////////////////////////////////////////////////////////////
// must be first in .text for reporting correct load address

static void sighandler(int signum)
{
  siglongjmp(env, signum);
}

static sighandler_t InjSignal(int sig, sighandler_t handler)
{
  struct sigaction act, oact;
  
  act.sa_handler = handler;
  if (sigemptyset (&act.sa_mask) < 0
      || sigaddset (&act.sa_mask, sig) < 0)
    return SIG_ERR;
  act.sa_flags = 0;
  if (sigaction (sig, &act, &oact) < 0)
    return SIG_ERR;
  
  return oact.sa_handler;
}

///////////////////////////////////////////////////////////////////////////////


static int PatchCode(void*dest, void*patch, int size)
{
  int pagesize;
  void* page;

  pagesize = sysconf(_SC_PAGE_SIZE);
  if (pagesize == -1) {
    printf("sysconf err %s\n", strerror(errno));
    return -1;
  }

  page= (void*)((long)dest & (~(pagesize-1)));
//  printf("Start of region:        %p\n", page);

//  printf("Addr %p", dest);
//  printf(" orig %p patch %p\n", *(void**)dest, *(void**)patch);
  if (mprotect(page, pagesize, PROT_READ|PROT_WRITE|PROT_EXEC) == -1) {
    printf("mprotect err %s\n", strerror(errno));
    return -1;
  }

  memcpy(dest, patch, size);
  if (mprotect(page, pagesize, PROT_READ|PROT_EXEC) == -1) {
    printf("mprotect err %s\n", strerror(errno));
    return -1;
  }
  return 0;
}

///////////////////////////////////////////////////////////////////////////////

struct CLauncher {
  int dummy00, dummy04, dummy08;
  void**methods;
  int null10;
  void**methods14;
  int dummy18;
  void**methods1c;
  int dummy20;
  void* ptrGPlayerAppBase;
  void* hLib;
  char* name;
  char* par1;
  char* par2;
  int err;  // 0x38 set to 1 dlopen err, 2 dlsym err, 3 before exec, 4 before unload, 5 after
}; 

void* ptrGPlayerAppBase;

void CLauncherMainPatched(struct CLauncher* this)
{
  sighandler_t old= InjSignal(SIGSEGV, &sighandler);
  int r= sigsetjmp(env, 0);
  if (r == 0) {
    printf("CLauncher patched %s %s %s\n", this->name, this->par1, this->par2);
//    printf("this %p\n", this);
    MPEG_Mem_Alloc();
    ptrGPlayerAppBase= this->ptrGPlayerAppBase;
    if (! strcmp(this->name,"DBowling")) {
      this->err= 3;
      DBowling_main(this->par1);
      this->err= 5;
    } else if (! strcmp(this->name,"MiniCook")) {
      this->err= 3;
      MiniCook_main();
      this->err= 5;
    } else {
      this->hLib= dlopen(this->name, RTLD_LAZY);
      if (! this->hLib) {
        printf("launcher.so main dlopen error: %s\n", dlerror());
        this->err= 1;
      } else {
        printf("launcher.so main dlopen ok\n");
        void *p= dlsym(this->hLib, "Game_Main");
        if (!p) {
          printf("launcher.so main dlsym error: %s\n", dlerror());
          this->err= 2;
        } else {
          printf("launcher.so main dlsym ok\n");
          this->err= 3;
          ((void (*)(char *p1, char *p2))p)(this->par1, this->par2);
          this->err= 4;
          printf("launcher.so main: Game_Main returned\n");
        }
        dlclose(this->hLib);
        this->hLib= NULL;
        if (this->err == 4) this->err= 5;
        char* err= dlerror();
        if (err) printf("launcher.so main dlclose: %s\n", err);
      }
    }
    MPEG_Mem_Free();
    CLuncher__UnloadGame(this->ptrGPlayerAppBase);
    ptrGPlayerAppBase= NULL;

    InjSignal(SIGSEGV, old);
    return;
  }
  InjSignal(SIGSEGV, old);
  this->err= 5;
  MPEG_Mem_Free();
  CLuncher__UnloadGame(this->ptrGPlayerAppBase);
  ptrGPlayerAppBase= NULL;
  printf("launcher.so main: signal caught\n");
}

///////////////////////////////////////////////////////////////////////////////

void GameExit(void)
{
  MPEG_Mem_Free();
  if (ptrGPlayerAppBase) CLuncher__UnloadGame(ptrGPlayerAppBase);
  ptrGPlayerAppBase= NULL;
  printf("launcher.so GameExit\n");
}

///////////////////////////////////////////////////////////////////////////////

static int PatchAll(int bPatch)
{
  int r;
  void *dest= &_ZTV8CLuncher + 2; //(void*)0x168638c;
  void *a= bPatch ? (void*)&CLauncherMainPatched : (void*)&_ZN8CLuncher6t_MainEv;
  r= PatchCode(dest, &a, 4);
  if (r) return r;
  unsigned char d= bPatch ? 9 : 2; 
  return PatchCode((void*)CTLibCategoryWin__ScanCategory_enable_game, &d, 1);
}

///////////////////////////////////////////////////////////////////////////////

void _init(void)
{
  int r;
  printf("\n************* add-symbol-file launcher.so %p\n CLauncherMainPatched %p\n", &sighandler, &CLauncherMainPatched);

  sighandler_t old= InjSignal(SIGSEGV, &sighandler);
  r= sigsetjmp(env, 0);
  if (r == 0) {
    PatchAll(1);

    InjSignal(SIGSEGV, old);
    return;
  }
  
  InjSignal(SIGSEGV, old);
  printf("launcher.so init: signal caught\n");
}

///////////////////////////////////////////////////////////////////////////////

void _fini(void)
{
  printf("************* launcher.so: unloading\n");
  PatchAll(0);
}
