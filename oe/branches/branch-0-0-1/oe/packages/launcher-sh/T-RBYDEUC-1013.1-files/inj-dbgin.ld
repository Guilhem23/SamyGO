/* Script for -N: mix text and data on same page; don't align data */
OUTPUT_FORMAT("elf32-sh-linux", "elf32-sh-linux",
              "elf32-sh-linux")
OUTPUT_ARCH(sh)
SECTIONS
{
  .checkVersion  0x1508978 : {
    BYTE(0x54)	/* T-RBYDEUC- */
    BYTE(0x2d)
    BYTE(0x52)
    BYTE(0x42)
    BYTE(0x59)
    BYTE(0x44)
    BYTE(0x45)
    BYTE(0x55)
    BYTE(0x43)
    BYTE(0x2d)
    BYTE(0x31)	/* 1013.1 */
    BYTE(0x30)
    BYTE(0x31)
    BYTE(0x33)
    BYTE(0x2e)
    BYTE(0x31)
  }

  .patch0 0x0400364 : {
    LONG(MainLoopPatched)
  }

  /* use stdin instead of /dev/console for debug input */
  stdin = 0x1FF4878;

  .patch1 0x6356A4 : { *(.patchtextInjlib) *(.patchtextInjOpen) *(.rodataInj*) }
  ASSERT( . <= 0x635820, "patchtext is too big to fit into exeDSP")

  .patch2 0x637920 : {
    LONG(stdin)
  }

  .patch3 0x6379B8 : {
    LONG(stdin)
  }

  .patch4 0x6379D4 : { *(.patchtextInjStart) *(.patchtextInjClose) *(.rodata*) }
  ASSERT( . <= 0x637A60, "patchtextInjlib is too big to fit into exeDSP")

  .patch5 0x637AD8 : {
    LONG(stdin)
  }

  /* disable call to init /dev/console */
  .patch6 0x63B368 : {
    BYTE(9)
    BYTE(0)
    BYTE(9)
    BYTE(0)
  }

  /* DebugDumpMonInfo - missing return */
  .patch7 0x69FDB8 : { *(.patchtextDbgMonInfoRet1) }
  .patch8 0x69FE4C : { *(.patchtextDbgMonInfoRet2) }
  ASSERT( . <= 0x69FE60, "patchtextDbgMonInfoRet is too big to fit into exeDSP")

  .patch9 0x11B1154 : { *(.patchtextStartUPnP) }
  ASSERT( . <= 0x011B1180, "patchtextStartUPnP is too big to fit into exeDSP")
/*
  /DISCARD/ : { *(.patchtextStartUPnP) }
*/

  MainLoop = 0x408540;
  DebugPrintf = 0x1419d40;

  dlerror = 0x1459700;
  dlopen = 0x1468BA0;

  .bss 0x2211ddc (NOLOAD) : { *(.patchbss*) }

  /* Stabs debugging sections.  */
  .stab          0 : { *(.stab) }
  .stabstr       0 : { *(.stabstr) }
  .stab.excl     0 : { *(.stab.excl) }
  .stab.exclstr  0 : { *(.stab.exclstr) }
  .stab.index    0 : { *(.stab.index) }
  .stab.indexstr 0 : { *(.stab.indexstr) }
  .comment       0 : { *(.comment) }
  /* DWARF debug sections.
     Symbols in the DWARF debugging sections are relative to the beginning
     of the section so we begin them at 0.  */
  /* DWARF 1 */
  .debug          0 : { *(.debug) }
  .line           0 : { *(.line) }
  /* GNU DWARF 1 extensions */
  .debug_srcinfo  0 : { *(.debug_srcinfo) }
  .debug_sfnames  0 : { *(.debug_sfnames) }
  /* DWARF 1.1 and DWARF 2 */
  .debug_aranges  0 : { *(.debug_aranges) }
  .debug_pubnames 0 : { *(.debug_pubnames) }
  /* DWARF 2 */
  .debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
  .debug_abbrev   0 : { *(.debug_abbrev) }
  .debug_line     0 : { *(.debug_line) }
  .debug_frame    0 : { *(.debug_frame) }
  .debug_str      0 : { *(.debug_str) }
  .debug_loc      0 : { *(.debug_loc) }
  .debug_macinfo  0 : { *(.debug_macinfo) }
  /* SGI/MIPS DWARF 2 extensions */
  .debug_weaknames 0 : { *(.debug_weaknames) }
  .debug_funcnames 0 : { *(.debug_funcnames) }
  .debug_typenames 0 : { *(.debug_typenames) }
  .debug_varnames  0 : { *(.debug_varnames) }
  /DISCARD/ : { *(.note.GNU-stack) }
}
