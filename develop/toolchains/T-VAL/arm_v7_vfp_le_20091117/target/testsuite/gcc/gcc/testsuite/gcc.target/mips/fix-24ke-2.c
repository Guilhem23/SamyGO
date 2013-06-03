/* Ensure that the -mfix-24ke option works.  */
/* { dg-mips-options "-O2 -march=4kc -mbranch-likely -mfix-24ke" } */
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
/* { dg-final { scan-assembler-not "\tbeql\t.*\tadd.s\t" } } */
