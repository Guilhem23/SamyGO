// exedsp.h
//
///////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>

#define GdbBreak()  asm("trapa #-0x3d\n  nop")

///////////////////////////////////////////////////////////////////////////////

extern struct {
  int sock;
  unsigned char flg[4];
} workerThreadsArray;

struct httpParser0 {
  int iWorkerThread;
};

struct httpParser5924 {
  unsigned char flg[4]; // @(5924,r14)b =flag process url
                        // @(5925,r14)b =flag process tokens
  int d1, d2;
  int httpVers; // @(5930,r14)b =HTTP version 1.1 -> 11
  char* pUrl; // @(5934,r14) =strdup of url
  char* p1;
  char* pHost; // @(593c,r14) =strdup of host
  char* pContType; // @(5940,r14) =strdup of Content-Type
  char* pBody; // @(5944,r14) =strdup of body text
  char* pWholeReq; // @(5948,r14) =strdup of whole http req
  int len; // @(594c,r14) =bf len
  int lenBody; // @(5950,r14) =len of body
// @(5960,r14) =?? 0x159
// @(5968,r14) =len of body
};

struct httpParser59cc {
  int xx;
  char * ptok; // @(59d0,r14) =pointer to token5520 for parsing 
  char * pbf; // @(59d4,r14) =pointer to bf for parsing 
  int nleft; // @(59d8,r14) =zbytek znaku v bf 
  int len; // @(59dc,r14) = len of whole req
};

extern struct { int en; char*s; void*proc; } extHandleUrlParseTab[10];


///////////////////////////////////////////////////////////////////////////////
//-- functions

int TvBase__SetProfile(void*thisUnused, int profileNum, const char*inBytes, int size);
int TvBase__GetProfile(void*thisUnused, int profileNum, char*outBytes, int size);

int main(int argc, char *const argv[]);
int DspDebugPrintf(int level,char* format, ...);
int FormatString(char**outString,char* format, ...);

int LockLocalDimming(void);
int LocalDimmingIICWrite(unsigned reg, int val);    
int LocalDimmingIICRead(unsigned reg);
int LocalDimmingIICBitMaskWrite(unsigned reg, int mask, int bits);    
int UnlockLocalDimming(void);

int DevLDSetEnergySaving(int level, int backlight);    

int VDH_SetBrightness(unsigned char brightness,int x);
int VDH_SetContrast(unsigned char contrast,int x);
int VDH_SetEnergySave(int level, int backlight);

void SetBrightness(void*this, int x);
void* GetPtrMainOrPIP(void*thisUnused,int x);

void** _ZTV8CLuncher;
void _ZN8CLuncher6t_MainEv(void*this);
void CLuncher__UnloadGame(void*this);
void MPEG_Mem_Alloc(void);
void MPEG_Mem_Free(void);
void DBowling_main(const char *par);
void MiniCook_main(void);

#ifdef DEBUG
char *strchr_0(const char *s, int c);
int strlen_0(char*str);
char *strncpy_0(char *dest, const char *src, size_t n);
char *strcpy_0(char *dest, const char *src);
int strncmp_0(const char *s1, const char *s2, size_t n);
int strcmp_0(const char*s1, const char*s2);
char *strncat_0(char *dest, const char *src, size_t n);
char *strcat_0(char *dest, const char *src);
void *memmove_0(void *dest, const void *src, size_t n);
void *memcpy_0(void *dest, const void *src, size_t n);
int memcmp_0(const void *s1, const void *s2, size_t n);
void *memset_0(void *s, int c, size_t n);
char *strcpy_1(char *dest, const char *src);
int strcmp_1(const char*s1, const char*s2);
char *strncat_1(char *dest, const char *src, size_t n);
char *strcat_1(char *dest, const char *src);
int abs(int j);
int abs_0(int j);
int abs_1(int j);
double atof (const char *nptr);
#endif

int SendEvent(int* ptr5int, int r5, int r6);
int STMovie_SetDisplaySize(int fitMode);
int PLAYREC_SetDisplaySize(int fitMode);

int HandleWatchdogOnOff(void*thisUnused,int onOff); 

void MediaInfo(void*infoStruct);
// -0x60??? Video PID
// 0 .. file name
// 0x208 container 0=TS
// 0x218
// 0x21c PCR PID
// 0x220 aspect???
// 0x228 2 MPEG2
// 0x22c
// 0x23c,240 long64 frameCount
// 0x244 float frameRate
// 0x248 frameWidth
// 0x24c frameHeight

int CMovieManager__GetMediaInfo_File(char*fn, void*tMcpMediaInfo);
int CMovieManager__GetMediaInfo_UCC(char*fn, void*tMcpMediaInfo);
int MediaInfoHelper3(char*fn, void*tMcpMediaInfo);
int CMovieManager__Parsing(void*this,char*fn, int x);
int UiSetDisplaySize(void*unusedThis, int fitMode);

int GetApplication(void *pApplicationStore, int i);

void ResourceManager(void*ptr, int val6, short**ptrTab, int numResources);
short* GetWString(void*this, int resNum);
// this= 0x20afb60
// (this+40)->r4 = 0x20afb88
// (r4+8 = 0x2533800) -> numOfResInTable
// (r4+4 = 0x25337fc) -> ptrToTable (0x1db5634 english, 0x01e1ca44 czech)

int STVID_GetInputWindowMode(void* handle,int*pMode,int*twoInts);
// ioctl (28, 0xd721, )
int STVID_GetOutputWindowMode(void* handle,int*pMode,int*twoInts);
int STVID_SetSize(void* handle,int inX, int inY, int inWidth, int inHeight, int outX, int outY, int outWidth, int outHeight);
int STVID_SetOutputWindowMode(void* handle,int mode,int*twoInts);
int STVID_SetInputWindowMode(void* handle,int mode,int*twoInts);

int ZoomOptionLeftRight(void*unknown, int key);
// key 4 .. left, 5...right

int HttpGetSocketData(int par1FromHttpProcessing, char*bf, int sz, int n1); 
int HttpIsSeparator(int c);
int HttpFindEOL(int **pp);
int HttpGetToken(char**ptrToParsedText59d4,char*ptrToSaveTokenText);
// 0x11c6500 gotToken
int HttpGetTokenLimitSize(char**ptrToParsedText59d4,char*ptrToSaveTokenText,int size);
int HttpAllocBf(void*flags5924,char*bf,void*ptr59dc,int len);
// @r14 @par5960
// @(4,r14)  bf
int HttpParse(void*flags5924,char*bf,void*ptr59dc,int len);
// @r14 len
// @(4,r14) ptr59dc
// @(8,r14) end of bf
 
void HttpReplyTab(int sock,int msgIdx);
#define HTTP_REPLY_OK 4
#define HTTP_REPLY_BAD_REQUEST 6
#define HTTP_REPLY_UNAUTHORIZED 7
#define HTTP_REPLY_FORBIDDEN 8
#define HTTP_REPLY_NOT_FOUND 9
#define HTTP_REPLY_METHOD_NOT_ALLOWED 10
#define HTTP_REPLY_NOT_ACCEPTABLE 11
#define HTTP_REPLY_PRECONDITION_FAILED 12
#define HTTP_REPLY_REQUEST_ENTITY_TOO_LARGE 13
#define HTTP_REPLY_REQUEST_URI_TOO_LONG 14
#define HTTP_REPLY_REQUEST_RANGE_NOT_SATISFIABLE 15
#define HTTP_REPLY_EXPECTATION_FAILED 16
#define HTTP_REPLY_INTERNAL_SERVER_ERROR 17
#define HTTP_REPLY_NOT_IMPLEMENTED 18
#define HTTP_REPLY_SERVICE_UNAVAILABLE 19
#define HTTP_REPLY_HTTPVERSION_NOT_SUPPORTED 20

int HttpProcessing(void*paramBlock);
// @(8,r14) =paramBlock[0] =0
// @(12,r14) =paramBlock[1] =1
// @(20,r14) =chars in buffer
// @(32,r14) =buffer[0x5000]
// @(5020,r14) =token text - NOTIFY parameter
// @(5520,r14) =token text
// @(5924,r14)b =flag process url
// @(5925,r14)b =flag process tokens
// @(5930,r14)b =HTTP version 1.1 -> 11
// @(5934,r14) =strdup of url
// @(593c,r14) =strdup of host
// @(5940,r14) =strdup of Content-Type
// @(5944,r14) =strdup of body text
// @(5948,r14) =strdup of whole http req
// @(594c,r14) =bf len
// @(5950,r14) =len of body
// @(5960,r14) =?? 0x159
// @(5968,r14) =len of body
// @(59d0,r14) =pointer to token5520 for parsing 
// @(59d4,r14) =pointer to bf for parsing 
// @(59d8,r14) =zbytek znaku v bf 
// @(59dc,r14) = len of whole req
// 11c6fae: call to ExtHandle(char*wholeHttpReq, char**ptr, void*par59cc)
//    ptr= 0, par59cc= 0
//at 0x16b0af4
// 0 GET
// 1 POST
// 2 HEAD
// 3 NOTIFY	0x011c72c0 6->flag5925, 11c732e: url->url5934 
// 6 M-POST	0x011c7294
// 4 SUBSCRIBE
// 5 UNSUBSCRIBE
// 11 \r\n	t 0x11c76a0
// 7 connection:
// 8 host:	t 0x11c75e0
// 9 content-length:		t 0x11c7600
// 10 content-type:	t 0x11c7660
// 15 expect:   	 0x11c76a0
// 12 soapaction:
// 18 Transfer-Encoding:
// 19 Trailer:		0x11c7400
// 13 range:
// 14 getcontentFeatures.dlna.org:
// 16 TimeSeekRange.dlna.org:	0x11c7440
// 17 PlaySpeed.dlna.org:	0x11c7480
// 20 NT:		0x11c74c0
// 21 SID:		0x11c74e0
// 22 NTS:		0x11c7500
// 23 SEQ:		0x11c7520
// 24 CALLBACK:
// 25 TIMEOUT:
// 26 transferMode.dlna.org:
// 27 contentFeatures.dlna.org:
// 28 content-range:
// 29 USER-AGENT:
// 30 BATCH-MODE:	0x11c7580
// 31 BATCH-COMMAND:	0x11c7580
// 32 BATCH-DELAY:	0x11c7580

///////////////////////////////////////////////////////////////////////////////
//-- vars

extern int bStopWatchDog;

extern void* extendDbgCall;

///////////////////////////////////////////////////////////////////////////////
// from inj.so

#define MAX_LIBS 10
extern void* hLibs[MAX_LIBS];

//typedef void (*sighandler_t)(int);
//sighandler_t InjSignal(int sig, sighandler_t handler);
