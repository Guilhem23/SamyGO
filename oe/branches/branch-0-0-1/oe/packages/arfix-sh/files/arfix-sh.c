//   arfix.c
//
// patch STMovie_SetDisplaySize() to take video aspect ratio into account
// for Samsung TV with T-RBYDEUC firmware 
///////////////////////////////////////////////////////////////////////////////

// use -fshort-wchar option for gcc!
typedef unsigned short wchar16_t;

typedef struct {
  void *handle1;
  void *handle2;
} t_VideoInfoStruct;

extern t_VideoInfoStruct mm_VideoInfoStruct[];
extern int mm_VideoInfo;
extern int videoInputWidth;
extern int videoInputHeight;

int STVID_GetInputWindowMode(void *handle, int *pMode, void *twoInts);
int STVID_GetOutputWindowMode(void *handle, int *pMode, void *twoInts);

int STVID_SetOutputWindowMode(void *handle, int mode, void *twoInts);
int STVID_SetInputWindowMode(void *handle, int mode, void *twoInts);

int STVID_SetSize(void *handle,
		  int inX, int inY, int inWidth, int inHeight,
		  int outX, int outY, int outWidth, int outHeight);

int DebugPrintf(const char *format, ...);

#define PROF_FIT_MODE 115

extern void *profileStore;
int GetProfile(void *pProfileStore, int profileNum, unsigned char *bf,
	       int size);
int SetProfile(void *pProfileStore, int profileNum, unsigned char *bf,
	       int size);

extern void *resourceStore;
const wchar16_t *GetWString(void *pResourceStore, int resNum);

extern void *applicationStore;
void *GetApplication(void *pApplicationStore, int i);

struct event_s {
  int unused;
  void *pApp144c;
  int nMenu;
  int zero1;
  int zero2;
};
int SendEvent(struct event_s *, int r5, int r6);

///////////////////////////////////////////////////////////////////////////////

const struct {
  int ratio900;			// multiplied by common multiplier 900
  union {
    int nStringResource;
    wchar16_t *wstr;
  } desc;
} modeTab[] = {
// table index 2,3,4 corresponds to MPEG Sequence header (01B3) apect ratio
// *INDENT-OFF*
	{		.desc.nStringResource = 70 },	// Auto Fit
	{		.desc.nStringResource = 71 },	// Original
	{ 900 * 4 / 3,	.desc.nStringResource = 29 },	// 4:3
	{ 900 * 16 / 9,	.desc.nStringResource = 25 },	// 16:9
	{ 9 * 221,	.desc.wstr = L"2.21:1"},
// *INDENT-ON*
};

enum {
  FIT_MODE_FIRST = 1,
  FIT_MODE_AUTO = FIT_MODE_FIRST,
  FIT_MODE_ORIG,
  FIT_MODE_LAST = sizeof(modeTab) / sizeof(modeTab[0])
};

int mpegAspectRatio __attribute__ ((section(".patchbss")));

///////////////////////////////////////////////////////////////////////////////

int STMovie_SetDisplaySize(int fit)
    __attribute__ ((section("patch1STMovie_SetDisplaySize")));
int STMovie_SetDisplaySize(int fit)
{
  void *handle = mm_VideoInfoStruct[mm_VideoInfo].handle1;
  int iw = videoInputWidth;
  int ih = videoInputHeight;
  const int pw = 1920;		// hopefully all pannels are full HD
  const int ph = 1080;
  int ow, oh, ox, oy;
  int w, h;
  int ar;
  int mode;
  int po[2];
  int pi[2];

  // mm_VideoInfo sometimes points to null handle
  // use the first non null then
  if (! handle) {
    if (mm_VideoInfoStruct[0].handle1) {
      handle= mm_VideoInfoStruct[0].handle1;
      DebugPrintf("SetSize mm_VideoInfoStruct[0]\n");
    } else {
      handle= mm_VideoInfoStruct[1].handle1;
      DebugPrintf("SetSize mm_VideoInfoStruct[1]\n");
    }
  }

  if (iw < 16)
    iw = 16;			// some reasonable minimum, just for the case
  if (ih < 9)
    ih = 9;
  if (iw == 1920 && ih == 1088) {
    ih = 1080;			// don't scale, just cut
  }

  ow = pw;
  oh = ph;

  // not using switch ... case construction because of big overhead on SH4 arch

  if (fit == FIT_MODE_ORIG) {

    // original size
    if (iw > pw)
      iw = pw;			// cut if too big
    if (ih > ph)
      ih = ph;
    ow = iw;
    oh = ih;

  } else {

    if (fit > FIT_MODE_ORIG && fit <= FIT_MODE_LAST) {

      // forced aspect ratio
      w = modeTab[fit - 1].ratio900;
      h = 900;

    } else {

      // auto fit keeping aspect ratio
      w = iw;
      h = ih;

      ar = (mpegAspectRatio >> 4) & 0xf;
      if (ar > 1 && ar <= 4) {
	w = modeTab[ar].ratio900;
	h = 900;

      } else if (iw == 720 && ih == 576) {
	// PAL - never has sqare pixels
	w = 576 * 4 / 3;
      }
    }

    // same as w/h > pw/ph, but vithout division
    if (w * ph > pw * h) {
      // wider than screen
      oh = pw * h / w;
      // stays ow= pw;
    } else {
      // narrower or same
      ow = ph * w / h;
      // stays oh= ph;
    }
  }

  if (ow > pw)
    ow = pw;
  ox = (pw - ow) / 2;
  if (oh > ph)
    oh = ph;
  oy = (ph - oh) / 2;

  if (STVID_GetInputWindowMode(handle, &mode, &pi)) {
    DebugPrintf("STVID_GetInputWindowMode Error\n");
  }
  if (STVID_GetOutputWindowMode(handle, &mode, &po)) {
    DebugPrintf("STVID_GetOutputWindowMode Error\n");
  }
  if (STVID_SetInputWindowMode(handle, 0, &pi)) {
    DebugPrintf("STVID_SetInputWindowMode Error\n");
  }
  if (STVID_SetOutputWindowMode(handle, 0, &po)) {
    DebugPrintf("STVID_SetOutputWindowMode Error\n");
  }

  DebugPrintf("arfix 1.2 SetSize MPEG AR %0x, In %dx%d, Out %dx%d-%d-%d\n",
	      mpegAspectRatio, iw, ih, ow, oh, ox, oy);
  if (STVID_SetSize(handle, 0, 0, iw, ih, ox, oy, ow, oh)) {
    DebugPrintf("STVID_SetSize Error\n");
  }
  return 0;
}

///////////////////////////////////////////////////////////////////////////////

const wchar16_t *GetZoomOptionString(void)
    __attribute__ ((section("patch2GetZoomOptionString")));
const wchar16_t *GetZoomOptionString(void)
{
  unsigned char data = 1;

  GetProfile(&profileStore, PROF_FIT_MODE, &data, 1);
  data--;
  if (data >= FIT_MODE_LAST) {
    data = 0;
  }
  if (modeTab[data].desc.nStringResource < 0x400000) {
    // not an addres, use as resource number
    return GetWString(&resourceStore, modeTab[data].desc.nStringResource);
  } else {
    return modeTab[data].desc.wstr;
  }
}

///////////////////////////////////////////////////////////////////////////////

int ZoomOptionLeftRight(void *unknown, int key)
    __attribute__ ((section("patch3ZoomOptionLeftRight")));
int ZoomOptionLeftRight(void *unknown, int key)
{
  unsigned char data = 1;
  void *pApp;
  struct event_s event;

  if (GetProfile(&profileStore, PROF_FIT_MODE, &data, 1) == 0) {
    return 0;
  }

  if (key == 4) {
    data--;
    if (data < FIT_MODE_FIRST)
      data = FIT_MODE_LAST;
  } else {
    data++;
    if (data > FIT_MODE_LAST)
      data = FIT_MODE_FIRST;
  }

  if (SetProfile(&profileStore, PROF_FIT_MODE, &data, 1) == 0) {
    return 0;
  }

  pApp = GetApplication(&applicationStore, 30);
  if (pApp)
    pApp += 0x144c;

  event.pApp144c = pApp;
  event.nMenu = 0x472;
  event.zero1 = 0;
  event.zero2 = 0;
  SendEvent(&event, 0, 0);

  return 1;
}
