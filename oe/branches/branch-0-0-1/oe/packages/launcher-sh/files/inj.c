// inj.c
//
// Second stage of injecting .so libraries into exeDSP
// for Samsung TV with T-RBYDEUC firmware
//
///////////////////////////////////////////////////////////////////////////////

#include <pthread.h>
#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include <setjmp.h>
#include <signal.h>
#include <string.h>

#include "exedsp.h"

///////////////////////////////////////////////////////////////////////////////

void* hLibs[MAX_LIBS];
char* err;
static sigjmp_buf env;

///////////////////////////////////////////////////////////////////////////////
// must be first in .text for reporting correct load address in _init()

static void sighandler(int signum)
{
  siglongjmp(env, signum);
}

static sig_t InjSignal(int sig, sig_t handler)
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

int LoadLib(int ihLibs, char* libname)
{    
  printf("inj.so LoadLib[%d]: %s\n", ihLibs, libname);
  hLibs[ihLibs]= dlopen(libname, RTLD_LAZY);
  if (! hLibs[ihLibs]) {
    err= dlerror();
    if (err) printf("inj.so LoadLib dlopen: %s\n", err);
    return -1;
  }
  return 0;
}

int CallLib(void* hLib, char* startproc, char* par1)
{    
  void *pfc;
  printf("inj.so CallLib: start %s, parameter %s\n", startproc, par1);
  pfc= dlsym(hLib, startproc);
  if (! pfc) {
    err= dlerror();
    if (err) printf("inj.so CallLib dlsym: %s\n", err);
    return -1;
  }
  ((void(*)(char*))pfc)(par1);
  return 0;
}


// UnloadLib(-1, NULL) ......unload last loaded library
// UnloadLib(index, NULL) ...unload a library by hLibs index
// UnloadLib(-1, libname) ...unload a library by name
int UnloadLib(int ihLibs, char* libname)
{
  void *hLib= NULL;
  if (ihLibs == -1) { 
    if (libname) {
      // open and close to get handle by name
      hLib= dlopen(libname, RTLD_LAZY);
      if (! hLib) {
        return -1;
      }
      for (; ihLibs < MAX_LIBS; ihLibs++) if (hLibs[ihLibs] == hLib) {
        hLibs[ihLibs]= NULL;
        break;
      }
      if (dlclose(hLib)) {
        err= dlerror();
        if (err) printf("inj.so UloadLib dlclose1: %s\n", err);
        return -3;
      }
    } else {
      // look for last one
      for (ihLibs= MAX_LIBS - 1; ihLibs > 0; ihLibs--) if (hLibs[ihLibs]) {
        hLib= hLibs[ihLibs];
        hLibs[ihLibs]= NULL;
        break;
      }
    }
  } else if (hLibs[ihLibs]) {
    hLib= hLibs[ihLibs];
    hLibs[ihLibs]= NULL;
  }

  if (! hLib) {
    return -4;
  }
  if (dlclose(hLib)) {
    err= dlerror();
    if (err) printf("inj.so UloadLib dlclose: %s\n", err);
    return -5;
  }
  return 0;
}

// List of so libraries to load at startup
// is in file /mtd_rwarea/libso.autoload and has following format
// # comment
// library.so start_function_name start_function_parameter
 
void LoadAllLibs(void)
{
  char bf[256];
  int ihLibs= 0;
  FILE* f= fopen("/mtd_rwarea/libso.autoload", "r");
  if (! f) return;
  
  while (fgets(bf, sizeof(bf), f)) {
    char* libname= strtok(bf, " \t\r\n");
    char* startproc= strtok(NULL, " \t\r\n");
    if (! libname || libname[0] == '#') continue;
    for (; ihLibs < MAX_LIBS; ihLibs++) if (! hLibs[ihLibs]) break;
    if (ihLibs < MAX_LIBS) {
      if ((LoadLib(ihLibs, libname) == 0) && startproc) {
        CallLib(hLibs[ihLibs], startproc, strtok(NULL, " \t\r\n"));
      }
    }
  }
  fclose(f);
}

void *ThreadLoadAll(void *t)
{
  usleep(1000000);
  LoadAllLibs();
  return NULL;
}

void LibLoadUnload(void)
{
  if (hLibs[0]) {
    UnloadLib(-1, NULL);
  } else {
    LoadAllLibs();
  }
}

///////////////////////////////////////////////////////////////////////////////
// handles http requests:
//   NOTIFY /ExtHandle/inj load [n] library.so
//   NOTIFY /ExtHandle/inj call [n] function_name par1
//   NOTIFY /ExtHandle/inj unload [n | library.so]
//   NOTIFY /ExtHandle/inj break
// where n is 0..9, index to hLibs array 

void InjUPnPExtHandler(char *wholeHttpReq, char**ptr, struct httpParser59cc*par59cc)
{
  sig_t old= InjSignal(SIGSEGV, &sighandler);
  int r= sigsetjmp(env, 0);
  if (r == 0) {
    int ihLibs= 0;
    char*p;
    struct httpParser0 *par0= (struct httpParser0*)(((char*)par59cc) - 0x59cc);
    struct httpParser5924 *par5924= (struct httpParser5924*)(((char*)par0) + 0x5924);
    printf("inj.so UPnPExtHandler: %s\n", par5924->pBody);
    p= strtok(par5924->pBody, " \r\n");
    if (p) {
      r= HTTP_REPLY_NOT_IMPLEMENTED;

      if (strcmp(p, "load") == 0) {
        p= strtok(NULL, " \r\n");
        if (p && *p >= '0' && *p <= '9') {
          ihLibs= strtoul(p, NULL, 10);
          p= strtok(NULL, " \r\n");
        } else {
          for (; ihLibs < MAX_LIBS; ihLibs++) if (! hLibs[ihLibs]) break;
        }
        if (ihLibs < 0 || ihLibs >= MAX_LIBS || ! p || *p == '\0') {
          r= HTTP_REPLY_BAD_REQUEST;
        } else {
          if (LoadLib(ihLibs, p) == 0) {
            r= HTTP_REPLY_OK;
          } else {
            char bf[256];
            int len= snprintf(bf, sizeof(bf), "HTTP/1.1 400 dlopen error: %s\r\n", err);
            write(workerThreadsArray.sock, bf, len);
            r= 0;
          }
        }

      } else if (strcmp(p, "call") == 0) {
        p= strtok(NULL, " \r\n");
        if (p && *p >= '0' && *p <= '9') {
          ihLibs= strtoul(p, NULL, 10);
          p= strtok(NULL, " \r\n");
        } else {
          for (ihLibs= MAX_LIBS - 1; ihLibs > 0; ihLibs--) if (hLibs[ihLibs]) break;
        }
        if (! hLibs[ihLibs]) {
          r= HTTP_REPLY_BAD_REQUEST;
        } else {
          if (CallLib(hLibs[ihLibs], p, strtok(NULL, " \r\n")) == 0) {
            r= HTTP_REPLY_OK;
          } else {
            char bf[256];
            int len= snprintf(bf, sizeof(bf), "HTTP/1.1 400 dlsym error: %s\r\n", err);
            write(workerThreadsArray.sock, bf, len);
            r= 0;
          }
        }

      } else if (strcmp(p, "unload") == 0) {
        p= strtok(NULL, " \r\n");
        ihLibs= -1; // unload last one or named lib
        if (p && *p >= '0' && *p <= '9') {
          ihLibs= strtoul(p, NULL, 10);
          p= NULL;  // unload by index
        }
        if (UnloadLib(ihLibs, p) == 0) {
          r= HTTP_REPLY_OK;
        } else {
          r= HTTP_REPLY_BAD_REQUEST;
        }

      } else if (strcmp(p, "break") == 0) {
        GdbBreak();
        r= HTTP_REPLY_OK;
      }

      if (r && workerThreadsArray.flg[par0->iWorkerThread] == 1) {
        HttpReplyTab(workerThreadsArray.sock, r);
      }
    }
    InjSignal(SIGSEGV, old);
    return;
  }
  InjSignal(SIGSEGV, old);
  printf("inj.so UPnPExtHandler: signal caught\n");
}

///////////////////////////////////////////////////////////////////////////////

void _init(void)
{
  int r;

  printf("\n******** add-symbol-file inj.so %p\n", &sighandler);

  sig_t old= InjSignal(SIGSEGV, &sighandler);
  r= sigsetjmp(env, 0);
  if (r == 0) {
#ifdef STOP_WATCHDOG
    HandleWatchdogOnOff(NULL, 0);

//  printf( "thread watchdog was %s\n", bStopWatchDog ? "off" : "on");    
    bStopWatchDog= 1;
#endif  
    extendDbgCall= &LibLoadUnload;

    printf( "inj.so init: %s http ext handler\n", extHandleUrlParseTab[0].en ? "replace" : "insert");    
    extHandleUrlParseTab[0].en= 1;
    extHandleUrlParseTab[0].s= "inj";
    extHandleUrlParseTab[0].proc= &InjUPnPExtHandler;

    pthread_t thr;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);    
    pthread_create(&thr, &attr, ThreadLoadAll, NULL);
    pthread_attr_destroy(&attr);

    InjSignal(SIGSEGV, old);
    return;
  }
  InjSignal(SIGSEGV, old);
  printf("inj.so init: signal caught\n");
}

///////////////////////////////////////////////////////////////////////////////

void _fini(void)
{
  extHandleUrlParseTab[0].en= 0;
  extendDbgCall= NULL;
  printf("*************inj.so unloaded\n");
}
