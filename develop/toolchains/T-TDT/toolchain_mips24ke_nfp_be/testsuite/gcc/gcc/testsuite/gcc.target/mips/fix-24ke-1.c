/* This test ensures that this program generates code that might fall foul
   of the 24KE errata.  If this test fails then the other test, fix-24ke-2.c
   is NOT testing the flag correctly (because there was not problem to fix),
   and both test cases will need adjusting.  */
/* { dg-mips-options "-O2 -march=4kc -mbranch-likely -mno-fix-24ke" } */
float Foo (int a, float w, float x, float y, float z)
{
  w += 1;
  x += 1;
  y += 1;
  z += 1;

  __asm volatile ("nop");
  if (a)
    {
      __asm volatile ("nop");

      w += x;
      y += z;
    }
  else
    {
      w *= x;
      y *= z;
    }
  return w + x + y + z;
}
/* { dg-final { scan-assembler "\tbeql\t.*\tadd.s\t" } } */
