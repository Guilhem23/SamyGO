/*******************************************************************************
**
** Filename:      hexstr.c
**
** Function List: STR2BIN()
**                BIN2STR()
**                STR2FBIN()
**
** Function:      This is ...
**
** Copyright:     DIALOG SOFTWARE DEVELOPMENT INTERNAL USE ONLY
**
**                (C) COPYRIGHT DIALOG SOFTWARE DEVELOPMENT 2001
**
** Version:       Release 1, Level 2
**
** Change Activity:
**
** 010322 -- CMO: Module created
**
** 011003 -- CMO: Added support for fixed-length binary arrays.
**
*******************************************************************************/

// #ifndef lint
// static char sccsid[] = "@(#)string.c	1.2 (dialog) 10/3/01";
// #endif


#include <string.h>
#include <stdio.h>
#include <linux/ctype.h>

#include "hexstr.h"


/*******************************************************************************
** Function: PrintHex()
**
** Description: 
**
*******************************************************************************/

void PrintHex(unsigned char *d, int l) 
{
  int i;

 // printf("\n");
 // for (i = 0; i < l; i++)
 //   printf("%02X", d[i]);
  //printf("\n");
}
/*******************************************************************************
** Function: STR2BIN()
**
** Description: 
**
*******************************************************************************/
void STR2BIN(char *s, unsigned char *d, int *l)
{
  int i, j, f, n;

  i = j = f = 0;
  while (1) {
    if (s[i] == '\0') {
      break;
    }
    n = toupper(s[i]);
    if (((n >= '0') && (n <= '9')) || ((n >= 'A') && (n <= 'F'))) {
      n -= '0';
      if (n > 9) {
        n -= 7;
      }
      if (f) {
        d[j] |= n;
        j++;
        f = 0;
      } else {
        d[j] = n << 4;
        f = 1;
      }
    }
    i++;
  }

  *l = j;

  return;
}


/*******************************************************************************
** Function: BIN2STR()
**
** Description: 
**
*******************************************************************************/

void BIN2STR(unsigned char *s, char *d, int l, int f)
{
  int   i;
  char  bb[5];

  d[0] = '\0';

  for (i = 0; i < l; i++) {
    if (f & 1) {
      sprintf(bb, "%02X", s[i]);
    } else {
      sprintf(bb, "%02x", s[i]);
    }
    strcat(d, bb);
    if (f & 2) {
      strcat(d, " ");
    }
  }

  return;
}


/*******************************************************************************
** Function: STR2FBIN()
**
** Description: 
**
*******************************************************************************/

void STR2FBIN(char *s, unsigned char *d, int l)
{
  int i, j, dig;

  /*
  ** This is still complete and utter bullshit and needs severe fixing
  */

  memset(d, 0x00, l);
  j = 0;

  for (i = (strlen(s) - 1); i >= 0; i--) {
    dig = toupper(s[i]);

    if (((dig >= '0') && (dig <= '9')) || ((dig >= 'A') && (dig <= 'F'))) {
      dig -= '0';
      if (dig > 9) {
        dig -= 7;
      }

      if (j & 1) {
        dig <<= 4;
      }
      
      d[l - (j / 2) - 1] |= dig;
      j++;
    }
  }

  return;
}

void rotate(unsigned char *d)
{
   unsigned char dd[8];
   int i;
   for (i= 0; i < 8; i++) dd[i]   =   d[i];
   
   d[0] = dd[7];
   d[1] = dd[6];
   d[2] = dd[5];
   d[3] = dd[4];
   d[4] = dd[3];
   d[5] = dd[2];
   d[6] = dd[1];
   d[7] = dd[0];
   return;
}