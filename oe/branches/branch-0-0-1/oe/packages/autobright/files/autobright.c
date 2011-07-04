// autobright.c
//
// LCD backlight auto adjusting in accordance with ambient light level
// for Samsung TV with T-RBYDEUC firmware and Local dimming LED backlight
// (models LE..A956 and LE..A756)
// and external light sensor connected to USB or Ex-link
//
///////////////////////////////////////////////////////////////////////////////

#define _GNU_SOURCE
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>
#include <signal.h>
#include <string.h>
#include <dlfcn.h>
#include <errno.h>
#include <unistd.h>
#include <sys/mman.h>
#include <time.h>

#include "exedsp.h"

///////////////////////////////////////////////////////////////////////////////

#define USB_SENSOR
#define SERIAL_SENSOR

static sigjmp_buf env;

volatile int thr_cancel= 0;
pthread_t thr;
void *status;

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

typedef struct {
  float x;
  float y;
} aproxTable_t;

#define MAX_SENSOR_CONV 20

aproxTable_t photoSensorConv[MAX_SENSOR_CONV] = {
// conversion of delay photo R,C to backlight level
  { 0, 127.9 },
  { 400, 110 },
  { 1000, 80 },
  { 3000, 65 },
  { 10000, 50 },
  { 65536, 35 },
};

int backlight, contrast, brightness;
unsigned lastLight;
time_t tmLastSet;

float ApproxByTable(aproxTable_t *table, float x)
{
  aproxTable_t point1, point2;
  int i;

  for (i= 0; i < MAX_SENSOR_CONV; i++) {
    memcpy(&point2, &(table[i]), sizeof(point2));
    if (point2.x >= x) {
      if (i == 0) {
        return point2.y;
      } else {
        return ((x - point1.x) * point2.y + (point2.x - x) * point1.y) / (point2.x - point1.x);
      }
    }
    memcpy(&point1, &point2, sizeof(point2));
  }
  return 0; 
}

void LoadApproxTable(void)
{
  char bf[256];
  int idx= 0;
  FILE* f= fopen("/mtd_rwarea/autobright.conf", "r");
  if (! f) return;
  
  while (fgets(bf, sizeof(bf), f)) {
    char* p1= strtok(bf, " \t\r\n");
    char* p2= strtok(NULL, " \t\r\n");
    if (! p1 || p1[0] == '#' || ! p2) continue;
    if (idx >= MAX_SENSOR_CONV) break;
    photoSensorConv[idx].x= strtod(p1, NULL);    
    photoSensorConv[idx].y= strtod(p2, NULL);
    idx++;
  }
  fclose(f);
  if (idx >= MAX_SENSOR_CONV) {
    photoSensorConv[MAX_SENSOR_CONV - 1].x= 65535;
  } else {
    photoSensorConv[idx].x= 65535;
    photoSensorConv[idx].y= photoSensorConv[idx-1].y;
  }    
}

void AutoBright(unsigned light)
{
  float fl;
  int dir= 0;
  int steps= 0;
  long steptime= 0;
  lastLight= light;
  fl= ApproxByTable(photoSensorConv, light);
  if (backlight == 0) {
    LockLocalDimming();
    backlight= LocalDimmingIICRead(0x30);    
    UnlockLocalDimming();
  }
  time(&tmLastSet);

  if (fl - backlight > 0.9f) {
    dir= 1;
    steps= fl - backlight;
  } else if (fl - backlight < -0.9f) {
    dir= -1;
    steps= backlight - fl;
  }
  if (steps > 1) {
    steptime= 900000 / steps;
    if (steptime > 1000000 / 25) steptime= 1000000 / 25;
    // we do not want flickering - keep minimal speed of change
  }
  if (steps > 0) {
    while (1) {
      backlight+= dir;
      LockLocalDimming();
      LocalDimmingIICWrite(0x30, backlight);    
      UnlockLocalDimming();
      if (--steps == 0) break;
      usleep(steptime);
    }
    printf("autobright.so: sensor %u, backlight %d\n", light, backlight);
  }
}


int DevLDSetEnergySavingPatched(int level, int bl)
{
  if (backlight && (unsigned long)(time(NULL) - tmLastSet) < 30) return 0;
  return DevLDSetEnergySaving(level, bl);
}

#ifdef SERIAL_SENSOR
int StrcmpDebugPatched(char *str1, const char *str2)
{
  if (strcmp(str1, "autobright") == 0) {
    unsigned light;
    int r;
    str1[0]= '\0';
    r= scanf(" %u", &light);
    if (r == 1) {
      AutoBright(light);
    } else {
      printf("autobright.so: scanf returned %d\n", r);
    }
  }
  return strcmp(str1, str2);
} 
#endif

///////////////////////////////////////////////////////////////////////////////

#ifdef USB_SENSOR
#include <usb.h>

#define VENDOR_ID 0x03eb
#define PRODUCT_ID 0x204f
#define INTERFACE 0

#define HID_REPORT_GET 0x01
#define HID_REPORT_SET 0x09

#define HID_RT_INPUT 0x01
#define HID_RT_OUTPUT 0x02
#define HID_RT_FEATURE 0x03 

const static int timeout = 5000;	/* timeout in ms */

usb_dev_handle *devh= NULL;

usb_dev_handle *setup_libusb_access()
{
  usb_dev_handle *devh = NULL;
  struct usb_bus *bus;
  struct usb_device *dev;
//  usb_set_debug(255);
  usb_find_busses();
  usb_find_devices();

  for (bus = usb_get_busses(); bus; bus = bus->next) {
    for (dev = bus->devices; dev; dev = dev->next) {
      if (dev->descriptor.idVendor == VENDOR_ID &&
          dev->descriptor.idProduct == PRODUCT_ID) {
      	printf("autobright.so: usbhid with Vendor Id: %x and Product Id: %x found.\n",
	        VENDOR_ID, PRODUCT_ID);
        if (!(devh = usb_open(dev))) {
          printf("autobright.so: Could not open USB device\n");
          return NULL;
        }
      }
    }
  }
  return devh;
}

void *ThreadUsbSensorRead(void *t)
{
  while (! thr_cancel) {
    if (! devh) {
      devh= setup_libusb_access();
    }
    if (devh) {
      int retval;
      retval = usb_claim_interface(devh, INTERFACE);
      if (retval < 0) {
        printf("autobright.so: Could not claim interface: %d\n", retval);
        usb_close(devh);
        devh= NULL;
      } else {
        int len;
        char bf[2];
        len = usb_control_msg(devh,
          USB_ENDPOINT_IN + USB_TYPE_CLASS + USB_RECIP_INTERFACE,
          HID_REPORT_GET,
          0,
          INTERFACE,
          bf, sizeof(bf), timeout);
        if (len < 0) {
          printf("autobright.so: Usb control message error\n");
          usb_close(devh);
          devh= NULL;
        } else if (len >= 2) {
          usb_release_interface(devh, INTERFACE);
          AutoBright(((unsigned char*)bf)[0] + (((unsigned char*)bf)[1] << 8));
        }
      }
    }
    usleep(1000000);
  }
  printf("autobright.so thread exit\n");
  return NULL;
}
#endif

///////////////////////////////////////////////////////////////////////////////

static int PatchCode(void*dest, void*patch, int size)
{
  int pagesize;
  void* page;

  pagesize = sysconf(_SC_PAGE_SIZE);
  if (pagesize == -1) {
    printf("autobright.so: sysconf err %s\n", strerror(errno));
    return -1;
  }

  page= (void*)((long)dest & (~(pagesize-1)));
//  printf("autobright.so: Start of region:        %p\n", page);

  if (mprotect(page, pagesize, PROT_READ|PROT_WRITE|PROT_EXEC) == -1) {
    printf("autobright.so: mprotect err %s\n", strerror(errno));
    return -1;
  }

//  printf("autobright.so PatchCode: Addr %p, orig %p patch %p\n", dest, *(void**)dest, *(void**)patch);
  memcpy(dest, patch, size);
  if (mprotect(page, pagesize, PROT_READ|PROT_EXEC) == -1) {
    printf("autobright.so: mprotect err %s\n", strerror(errno));
    return -1;
  }
  return 0;
}

static int PatchAll(int bPatch)
{
  void *dest;
  void *a;
#ifdef SERIAL_SENSOR
  dest= (void*)0x6BCCE8;
  a= bPatch ? (void*)&StrcmpDebugPatched : (void*)&strcmp;
  int r= PatchCode(dest, &a, 4);
  if (r) return r;
#endif
  dest= (void*)0x65243C;
  a= bPatch ? (void*)&DevLDSetEnergySavingPatched : (void*)&DevLDSetEnergySaving;
  return PatchCode(dest, &a, 4);
}

///////////////////////////////////////////////////////////////////////////////

void _init(void)
{
  int r;
  printf("\n************* add-symbol-file autobright.so %p\n", &sighandler);

  sighandler_t old= InjSignal(SIGSEGV, &sighandler);
  r= sigsetjmp(env, 0);
  if (r == 0) {
    LoadApproxTable();

    PatchAll(1);

#ifdef USB_SENSOR
    usb_init();
    pthread_create(&thr, NULL, ThreadUsbSensorRead, NULL);
#endif
    InjSignal(SIGSEGV, old);
    return;
  }
  
  InjSignal(SIGSEGV, old);
  printf("autobright.so init: signal caught\n");
}

int Game_Main(const char *path, const char *udn)
{
  // just increment usage count to prevent unload at Game_Main exit
  char bf[265];
  strcpy(bf, path);
  strcat(bf, "libAutobright.so");
  dlopen(bf, RTLD_LAZY);
  return 0;
}

///////////////////////////////////////////////////////////////////////////////

void _fini(void)
{
  printf("************* autobright.so: unloading\n");
  extHandleUrlParseTab[1].en= 0;
  PatchAll(0);
#ifdef USB_SENSOR
  if (! thr_cancel) {
    thr_cancel= 1;
    pthread_join(thr, &status);
  }
  if (devh) {
    usb_close(devh);
  }
#endif
}
