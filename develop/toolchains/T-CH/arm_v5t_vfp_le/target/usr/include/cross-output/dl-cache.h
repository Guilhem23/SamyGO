

#ifndef DL_CACHE_H
#define DL_CACHE_H 1

#include <file-fields.h>

struct file_entry
{
  file_int flags;
  file_uint key, value;
};

struct cache_file
{
  char magic[sizeof "ld.so-1.7.0" - 1];
  file_uint nlibs;
  struct file_entry libs[0];
};






struct file_entry_new
{
  file_int32_t flags;
  file_uint32_t key, value;
  file_uint32_t osversion;
  file_uint64_t hwcap;
};

struct cache_file_new
{
  char magic[sizeof "glibc-ld.so.cache" - 1];
  char version[sizeof "1.1" - 1];
  file_uint32_t nlibs;
  file_uint32_t len_strings;
  file_uint32_t unused[5];

  struct file_entry_new libs[0];

};






extern int _dl_cache_libcmp (const char *p1, const char *p2)
     ;


#define _DL_CACHE_DEFAULT_ID 3


#define _dl_cache_check_flags(flags) ((flags) == 1 || (flags) == 3)


#define add_system_dir(dir) add_dir (dir)


#define LD_SO_CACHE SYSCONFDIR "/ld.so.cache"


#define CACHEMAGIC "ld.so-1.7.0"


#define CACHEMAGIC_NEW "glibc-ld.so.cache"


#define CACHE_VERSION "1.1"


#define CACHEMAGIC_VERSION_NEW "glibc-ld.so.cache" "1.1"


#define ALIGN_CACHE(addr) (((addr) + __alignof__ (struct cache_file_new) -1) & (~(__alignof__ (struct cache_file_new) - 1)))


#endif
