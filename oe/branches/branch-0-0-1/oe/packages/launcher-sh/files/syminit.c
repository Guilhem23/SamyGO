// syminit.c
//
// initialization routine for symbol-only library
// makes symbol table zero based
// if defined MAKE_GLOBAL installs library as global
//  
///////////////////////////////////////////////////////////////////////////////

// libc internal headers 
#define internal_function
#define DT_THISPROCNUM   0
#include <libc-symbols.h>
#include <bits/libc-lock.h>
#include <link.h>

// standard headers 
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <dlfcn.h>
#include <ldsodefs.h>

#include "exedsp-abs.h"

///////////////////////////////////////////////////////////////////////////////

#define STR(x) #x
#define STRSO(x) STR(x) ".so"

#ifdef MAKE_GLOBAL
#define _init _initGlobalSym
#endif

static inline int cmplib(const char *path, const char *name)
{
  const char *p= path;
  if (path == NULL) {
    return 0;
  }
  while (*p) {
    if (*p == '/') path= p + 1;
    p++;
  }
  p= path;
  while (*name) {
    if (*p != *name) return 0;
    p++;
    name++;
  }
  return 1;
}

void _init(void)
{
  struct link_map *l;

  l= Abs_dl_ns._ns_loaded;
  AbsPrintf("\n*************" STRSO(SONAME) " .text %p\n", &_init);

#ifdef MAKE_GLOBAL
  if (l) {
    if (cmplib(l->l_name, STRSO(SONAME))) {
      AbsPrintf("set " STRSO(SONAME) " (orig %p) zero based\n\n", l->l_addr);
      l->l_addr= 0; // zero based symbols!!!!!!!!
    }
    if (Abs_dl_ns._ns_main_searchlist == NULL && Abs_dl_ns._ns_global_scope_alloc == 0) {
      int cnt;
      for (cnt = 0; cnt < l->l_searchlist.r_nlist; ++cnt) {
        l->l_searchlist.r_list[cnt]->l_global= 1;
      }
      Abs_dl_ns._ns_main_searchlist= &l->l_searchlist;
      Abs_dl_ns._ns_global_scope[0]= &l->l_searchlist;
    }
  }
#else
  while (l) {
    if (cmplib(l->l_name, STRSO(SONAME))) {
      AbsPrintf("set " STRSO(SONAME) " (orig %p) zero based\n\n", l->l_addr);
      l->l_addr= 0; // zero based symbols!!!!!!!!
      break;
    }
    l= l->l_next;
  }
#endif
}
