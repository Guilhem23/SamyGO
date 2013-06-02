

#ifndef DL_PROCINFO_H
#define DL_PROCINFO_H



static const char _dl_arm_cap_flags[10][10]
;

static inline int
__attribute__ ((unused))
_dl_procinfo (int word)
{
  int i;

  _dl_dprintf (STDOUT_FILENO, "AT_HWCAP:   ");

  for (i = 0; i < 10; ++i)
    if (word & (1 << i))
      _dl_dprintf (STDOUT_FILENO, " %s", _dl_arm_cap_flags[i]);

  _dl_dprintf (STDOUT_FILENO, "\n");

  return 0;
}

static inline const char *
__attribute__ ((unused))
_dl_hwcap_string (int idx)
{
  return _dl_arm_cap_flags[idx];
};



static inline int
__attribute__ ((unused))
_dl_string_hwcap (const char *str)
{
  int i;

  for (i = 0; i < 10; i++)
    {
      if (__extension__ ({ size_t __s1_len, __s2_len; (__builtin_constant_p (str) && __builtin_constant_p (_dl_arm_cap_flags[i]) && (__s1_len = strlen (str), __s2_len = strlen (_dl_arm_cap_flags[i]), (!((size_t)(const void *)((str) + 1) - (size_t)(const void *)(str) == 1) || __s1_len >= 4) && (!((size_t)(const void *)((_dl_arm_cap_flags[i]) + 1) - (size_t)(const void *)(_dl_arm_cap_flags[i]) == 1) || __s2_len >= 4)) ? __builtin_strcmp (str, _dl_arm_cap_flags[i]) : (__builtin_constant_p (str) && ((size_t)(const void *)((str) + 1) - (size_t)(const void *)(str) == 1) && (__s1_len = strlen (str), __s1_len < 4) ? (__builtin_constant_p (_dl_arm_cap_flags[i]) && ((size_t)(const void *)((_dl_arm_cap_flags[i]) + 1) - (size_t)(const void *)(_dl_arm_cap_flags[i]) == 1) ? __builtin_strcmp (str, _dl_arm_cap_flags[i]) : (__extension__ ({ __const unsigned char *__s2 = (__const unsigned char *) (__const char *) (_dl_arm_cap_flags[i]); register int __result = (((__const unsigned char *) (__const char *) (str))[0] - __s2[0]); if (__s1_len > 0 && __result == 0) { __result = (((__const unsigned char *) (__const char *) (str))[1] - __s2[1]); if (__s1_len > 1 && __result == 0) { __result = (((__const unsigned char *) (__const char *) (str))[2] - __s2[2]); if (__s1_len > 2 && __result == 0) __result = (((__const unsigned char *) (__const char *) (str))[3] - __s2[3]); } } __result; }))) : (__builtin_constant_p (_dl_arm_cap_flags[i]) && ((size_t)(const void *)((_dl_arm_cap_flags[i]) + 1) - (size_t)(const void *)(_dl_arm_cap_flags[i]) == 1) && (__s2_len = strlen (_dl_arm_cap_flags[i]), __s2_len < 4) ? (__builtin_constant_p (str) && ((size_t)(const void *)((str) + 1) - (size_t)(const void *)(str) == 1) ? __builtin_strcmp (str, _dl_arm_cap_flags[i]) : (__extension__ ({ __const unsigned char *__s1 = (__const unsigned char *) (__const char *) (str); register int __result = __s1[0] - ((__const unsigned char *) (__const char *) (_dl_arm_cap_flags[i]))[0]; if (__s2_len > 0 && __result == 0) { __result = (__s1[1] - ((__const unsigned char *) (__const char *) (_dl_arm_cap_flags[i]))[1]); if (__s2_len > 1 && __result == 0) { __result = (__s1[2] - ((__const unsigned char *) (__const char *) (_dl_arm_cap_flags[i]))[2]); if (__s2_len > 2 && __result == 0) __result = (__s1[3] - ((__const unsigned char *) (__const char *) (_dl_arm_cap_flags[i]))[3]); } } __result; }))) : __builtin_strcmp (str, _dl_arm_cap_flags[i])))); }) == 0)
 return i;
    }
  return -1;
};

static const char _dl_arm_cap_flags[10][10]


= {
    "swp", "half", "thumb", "26bit", "fast-mult", "fpa", "vfp", "edsp",
    "java", "iwmmxt",
  }


;


#define _DL_HWCAP_PLATFORM 0


#define _DL_HWCAP_COUNT 10
#define _dl_string_platform(str) (-1)


#define HWCAP_IMPORTANT (2 | 16)


#endif
