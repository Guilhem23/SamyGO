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

/* functions from exeDSP */
unsigned _ZN10CMovieCore17m_PanelResolutionE(void){
	fprintf(stdout,"%s:\n",__FUNCTION__);
}
void *_ZN16CResourceManager10GetWStringEi(void *a, unsigned b){
	printf("_ZN16CResourceManager10GetWStringEi\n");
}
unsigned char g_ToolResMgr[80];
/* end of functions from exeDSP */

int InputKeyState, InputKeyData;

int main(int argc, const char *argv[])
{
	void *lib_handle;
	char *error, *lib_dir, lib_toopen[100], *lib_name;
	int (*fn)(char *, char *), len;

	if(argc < 2)
		lib_dir = "/home/zsolt/Desktop/SamyGO-Extensions/";
	else
		lib_dir = argv[1];

	fprintf(stdout,"Path: %s\n",lib_dir);

	if(argc < 3)
		lib_name = "libSamyGO.so";
	else
		lib_name = argv[2];

	fprintf(stdout,"Library: %s\n",lib_name);

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
		fprintf(stderr, "Found function Game_Main\n");
	}

	/* lib_name, normaly uidn is passed to function
	 * have to figure out what it realy is
	 */
	(*fn)(lib_dir, lib_name);

	dlclose(lib_handle);
	exit(0);
}
