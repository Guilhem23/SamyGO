/*******************************************************************************
**
** Filename:      hexstr.h
**
** Function:      This include file contains the prototype declarations
**                for the functions contained in the file 'hexstr.c'
**
** Copyright:     DIALOG SOFTWARE DEVELOPMENT INTERNAL USE ONLY
**
**                (C) COPYRIGHT DIALOG SOFTWARE DEVELOPMENT 2002
**
** Version:       Release 1, Level 1
**
** @(#)hexstr.h	1.1 (dialog) 1/26/02";
**
** Change Activity:
**
** 020126 -- CMO: Module created.
**
*******************************************************************************/
void PrintHex(unsigned char *, int);
void STR2BIN(char *, unsigned char *, int *);
void BIN2STR(unsigned char *, char *, int, int f);
void STR2FBIN(char *, unsigned char *, int);
void rotate(unsigned char *);
