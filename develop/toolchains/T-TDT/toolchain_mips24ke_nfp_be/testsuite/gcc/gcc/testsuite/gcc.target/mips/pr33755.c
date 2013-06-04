--- /dev/null                                     2007/10/24 17:52:16     129606
+++ b/gcc/testsuite/gcc.target/mips/pr33755.c     2007/10/24 17:54:40     129607
@@ -0,0 +1,30 @@
/* { dg-do link } */
/* { dg-mips-options "-O2" } */

volatile int gv;
const char *ptrs[2];

void
foo (volatile int *v, const char **ptrs)
{
  switch (*v & 1)
    {
    case 0:
      ptrs[0] = 0;
      break;
    case 1:
      break;
    default:
      ptrs[1] = "Some text";
      break;
    }
  while (*v > 0)
    *v -= 1;
}

int
main (void)
{
  foo (&gv, ptrs);
  return 0;
}
