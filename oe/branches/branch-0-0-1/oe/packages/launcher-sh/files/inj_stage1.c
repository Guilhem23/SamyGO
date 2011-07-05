// inj_stage1.c
//
// Patch for Samsung TV with T-RBYDEUC firmware
// Loads exeDSP symbols and .so library loader
//
///////////////////////////////////////////////////////////////////////////////

#include <dlfcn.h>

///////////////////////////////////////////////////////////////////////////////

int DebugPrintf(const char *format, ...);
// actually standard printf, we just want to prevent gcc optimizer
// to use puts instead 

void MainLoop(int p);

///////////////////////////////////////////////////////////////////////////////

const char aSymFileName[] __attribute__ ((section(".rodataSym"))) = "sym.so";
const char aInjFileName[] __attribute__ ((section(".rodataInj"))) = "inj.so";

///////////////////////////////////////////////////////////////////////////////
// called in main exeDSP thread
// loads symbols of exeDSP

void MainLoopPatched(int p)
    __attribute__ ((section(".patchtextInjlib")));
void MainLoopPatched(int p)
{
    dlopen(aSymFileName, RTLD_NOW | RTLD_LOCAL);
    // as ld.so code is statically linked in and not properly initialised
    // we must load sym.so with RTLD_LOCAL otherwise dlopen fails
    // sym.so init will set up itself as RTLD_GLOBAL 
    char *err =dlerror();
    if (err) DebugPrintf("dlopen: %s\n", err); 
    MainLoop(p);
}

///////////////////////////////////////////////////////////////////////////////
// called after exeDSP initialization
// loads inj.so, real .so injection code  

void InjLibStart(void)
    __attribute__ ((section(".patchtextInjStart")));
void InjLibStart(void)
{
    // sym.so initialization alrady finished
    // loaded libraries can reference exeDSP symbols 
    void *hLib= dlopen(aInjFileName, RTLD_NOW);
    char *err= dlerror();
    if (err) DebugPrintf("dlopen: %s\n", err); 
}
