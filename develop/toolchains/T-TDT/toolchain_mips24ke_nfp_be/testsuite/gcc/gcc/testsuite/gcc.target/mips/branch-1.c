/* On most targets, we should implement these "if" statements using an
   "andi" instruction followed by a branch on zero.  */
/* { dg-mips-options "-O2 -mno-mips16" } */
/* Targets with bbit* instructions should use them instead.  We test
   that separately.  */
#ifdef _MIPS_ARCH_OCTEON
asm ("\tandi\t.*\tandi\t.*\tandi\t.*\tandi\t");
#else
void bar (void);
void f1 (int x) { if (x & 4) bar (); }
void f2 (int x) { if ((x >> 2) & 1) bar (); }
void f3 (unsigned int x) { if (x & 0x10) bar (); }
void f4 (unsigned int x) { if ((x >> 4) & 1) bar (); }
#endif
/* { dg-final { scan-assembler "\tandi\t.*\tandi\t.*\tandi\t.*\tandi\t" } } */
/* { dg-final { scan-assembler-not "\tsrl\t" } } */
/* { dg-final { scan-assembler-not "\tsra\t" } } */
