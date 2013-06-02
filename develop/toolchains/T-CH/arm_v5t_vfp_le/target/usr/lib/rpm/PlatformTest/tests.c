#include <endian.h>	// testendian
#include <pthread.h>	// testsize_pthread_mutex_t
#include <asm/page.h>	// ac_cv_page_size
#include <sys/types.h>	// testsize_off_t
#include <time.h>	// testsize_time_t
#include <stdio.h>	// fpos_t
#include <stdint.h>	// uintptr_t

/// macros used for PlatformTest itself

// this one must be called if any strings or unsigned ints are to be exported
#define SETENDIAN(is_bigendian) const unsigned char _PT_ENDIANNESS __attribute__ ((section ("PT_ENDIANNESS"))) = is_bigendian

/// macros used to generate output

#define TESTSIZE(type, name) const unsigned char _DEF_UCHAR_ac_cv_sizeof_##name __attribute__ ((section ("DEF_UCHAR_ac_cv_sizeof_"#name))) = sizeof(type)
#define EXPORTINT(val, name) const unsigned int _DEF_UINT_##name __attribute__ ((section ("DEF_UINT_"#name))) = val
#define EXPORTSTR(val, name) const unsigned char _DEF_STR_##name __attribute__ ((section ("DEF_STR_"#name))) = val

/// the actual tests themselves

#define ac_no 0
#define ac_yes 1

// hardcoded tests
EXPORTSTR(ac_no, ac_cv_func_placeholder);
EXPORTSTR(ac_yes, ac_cv_func_getpgrp_void);
EXPORTSTR(ac_yes, ac_cv_func_setpgrp_void);
EXPORTSTR(ac_no, ac_cv_func_closedir_void);
EXPORTSTR(ac_no, ac_cv_func_setvbuf_reversed);
EXPORTSTR(ac_yes, ac_cv_sys_restartable_syscalls);
EXPORTSTR(ac_yes, ac_cv_rshift_extends_sign); // TODO: maybe actually do a test for this one?
EXPORTSTR(ac_yes, ac_cv_func_getpwuid_r);
EXPORTSTR(ac_yes, ac_cv_func_pthread_key_delete);
EXPORTSTR(ac_yes, ac_cv_func_strstr);
EXPORTSTR(ac_yes, gt_cv_func_printf_posix);
EXPORTSTR(ac_yes, ac_cv_func_mmap_fixed_mapped);

// non-hardcoded tests

#if __BYTE_ORDER == __LITTLE_ENDIAN
SETENDIAN(0);
EXPORTSTR(ac_no, ac_cv_c_bigendian);
#elif __BYTE_ORDER == __BIG_ENDIAN
SETENDIAN(1);
EXPORTSTR(ac_yes, ac_cv_c_bigendian);
#else
#error Cannot determine byte order
#endif

TESTSIZE(void*, void_p);
TESTSIZE(char, char);
TESTSIZE(short, short);
TESTSIZE(int, int);
TESTSIZE(long, long);
TESTSIZE(float, float);
TESTSIZE(double, double);
TESTSIZE(long long, long_long);
TESTSIZE(long double, long_double);
TESTSIZE(pthread_mutex_t, pthread_mutex_t);
TESTSIZE(pthread_t, pthread_t);
TESTSIZE(uintptr_t, uintptr_t);
TESTSIZE(off_t, off_t);
TESTSIZE(fpos_t, fpos_t);
TESTSIZE(wchar_t, wchar_t);
TESTSIZE(size_t, size_t);
TESTSIZE(ssize_t, ssize_t);
TESTSIZE(uint8_t, uint8_t);
TESTSIZE(uint16_t, uint16_t);
TESTSIZE(uint32_t, uint32_t);
TESTSIZE(uint64_t, uint64_t);

