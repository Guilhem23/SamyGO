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

  .patch1 0x56f43c : { *(patch1STMovie_SetDisplaySize) *(.patchtext*) *(.rodata*) }
  ASSERT( . <= 0x56fa28, "STMovie_SetDisplaySize is too big to fit into exeDSP")

  .patch4 0x73d334 : { *(patch4ParserReset) }
  ASSERT( . <= 0x740340, "ParserReset is too big to fit into exeDSP")

  .patch7 0x740320 : {
    BYTE(0x40)
    BYTE(0x20)	/* switch (sourceType) in CMovieManager__Parsing */
    BYTE(0x40)
    BYTE(0x60)
  }

  .patch6 0x740470 : {
    LONG(ParserResetPatched)
  }

  .patch5 0xb04e8c : {
    LONG(MpegProbeSetSizePatched)
  }

  .patch2 0xd16520 : { *(patch2GetZoomOptionString) }
  ASSERT( . <= 0xd165c0, "GetZoomOptionString is too big to fit into exeDSP")

  .patch3 0xd167a0 : { *(patch3ZoomOptionLeftRight) }
  ASSERT( . <= 0xd16880, "ZoomOptionLeftRight is too big to fit into exeDSP")

  start = 0x783efe; /* Not real start actually. Just a frequent address */
             /* where gdb stops in the exeDSP - so you can use continue */

  GetProfile = 0x405d00;
  SetProfile = 0x405c60;

  SendEvent = 0x503e20;

  ParserReset = 0x73d340;

  GetApplication = 0x7b4520;
  GetWString = 0x7b83c0;

  STVID_GetInputWindowMode = 0x9a6168;
  STVID_GetOutputWindowMode = 0x9a6508;
  STVID_SetSize = 0x9a7e58;
  STVID_SetOutputWindowMode = 0x9a7d6c;
  STVID_SetInputWindowMode = 0x9a8050;

  MpegProbeSetSize = 0xb099e0;

  DebugPrintf = 0x1419d40;

  __sdivsi3_i4i = 0x13f1d80;

  profileStore = 0x208cd24;
  applicationStore = 0x208cd40;
  resourceStore = 0x20afb60;
  mm_VideoInfo = 0x22101e8;
  videoInputWidth = 0x221020c;
  videoInputHeight = 0x2210210;
  mm_VideoInfoStruct = 0x2210360;

  .bss 0x2211dd8 (NOLOAD) : { *(.patchbss*) }

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
