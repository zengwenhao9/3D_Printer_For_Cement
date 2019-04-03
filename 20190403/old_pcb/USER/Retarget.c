/*
*********************************************************************************************************
*
*	模块名称 : 重定向文件
*	文件名称 : Retarget.c
*	版    本 : V1.0
*	说    明 : 此重定向文档，串口和FlashFS都要使用。
*              1. FlashFS不支持使用MDK中的MicroLib功能，不使用MicroLib的话，默认半主模式是开启的，
*                 因此需要重新做重定向文件，以支持串口打印和FlashFS的使用。
*              2. 本文件来自KEIL官方，用户只需提供
*                  extern int sendchar (int ch);
*                  extern int getkey (void);
*                  两个函数即可，这两个函数在bsp_uart_fifo.c文件实现
*
*	修改记录 :
*		版本号    日期         作者            说明
*       V1.0    2015-09-10   Eric2013          首发
*
*	Copyright (C), 2015-2020, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/
/*----------------------------------------------------------------------------
 *      RL-ARM - FlashFS
 *----------------------------------------------------------------------------
 *      Name:    RETARGET.C 
 *      Purpose: Retarget low level functions
 *      Rev.:    V4.20
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2011 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <rt_sys.h>
#include <File_Config.h>


#define  STDIO
#pragma import(__use_no_semihosting_swi)

/* The following macro definitions may be used to translate this file:

  STDIO - use standard Input/Output device
          (default is NOT used)
 */

/* Standard IO device handles. */
#define STDIN   0x8001
#define STDOUT  0x8002
#define STDERR  0x8003

/* Standard IO device name defines. */
const char __stdin_name[]  = "STDIN";
const char __stdout_name[] = "STDOUT";
const char __stderr_name[] = "STDERR";

struct __FILE { int handle; /* Add whatever you need here */ };

#ifdef STDIO

/*------------------------------------------------------------------------------
  Write character to the Serial Port
 *----------------------------------------------------------------------------*/
int sendchar (int ch) {


  return (ch);
}


/*------------------------------------------------------------------------------
  Read character from the Serial Port
 *----------------------------------------------------------------------------*/
int getkey (void) {

  return 0;
}
#endif

/*--------------------------- _ttywrch --------------------------------------*/

void _ttywrch (int ch) {
#ifdef STDIO
  sendchar(ch);
#endif
}

/*--------------------------- _sys_open -------------------------------------*/

FILEHANDLE _sys_open (const char *name, int openmode) {
  /* Register standard Input Output devices. */
  if (strcmp(name, "STDIN") == 0) {
    return (STDIN);
  }
  if (strcmp(name, "STDOUT") == 0) {
    return (STDOUT);
  }
  if (strcmp(name, "STDERR") == 0) {
    return (STDERR);
  }
  return (__sys_open (name, openmode));
}

/*--------------------------- _sys_close ------------------------------------*/

int _sys_close (FILEHANDLE fh) {
  if (fh > 0x8000) {
    return (0);
  }
  return (__sys_close (fh));
}

/*--------------------------- _sys_write ------------------------------------*/

int _sys_write (FILEHANDLE fh, const U8 *buf, U32 len, int mode) {
#ifdef STDIO
  if (fh == STDOUT) {
    /* Standard Output device. */
    for (  ; len; len--) {
      sendchar (*buf++);
    }
    return (0);
  }
#endif
  if (fh > 0x8000) {
    return (-1);
  }
  return (__sys_write (fh, buf, len));
}

/*--------------------------- _sys_read -------------------------------------*/

int _sys_read (FILEHANDLE fh, U8 *buf, U32 len, int mode) {
#ifdef STDIO
  if (fh == STDIN) {
    /* Standard Input device. */
    for (  ; len; len--) {
      *buf++ = getkey ();
    }
    return (0);
  }
#endif
  if (fh > 0x8000) {
    return (-1);
  }
  return (__sys_read (fh, buf, len));
}

/*--------------------------- _sys_istty ------------------------------------*/

int _sys_istty (FILEHANDLE fh) {
  if (fh > 0x8000) {
    return (1);
  }
  return (0);
}

/*--------------------------- _sys_seek -------------------------------------*/

int _sys_seek (FILEHANDLE fh, long pos) {
  if (fh > 0x8000) {
    return (-1);
  }
  return (__sys_seek (fh, pos));
}

/*--------------------------- _sys_ensure -----------------------------------*/

int _sys_ensure (FILEHANDLE fh) {
  if (fh > 0x8000) {
    return (-1);
  }
  return (__sys_ensure (fh));
}

/*--------------------------- _sys_flen -------------------------------------*/

long _sys_flen (FILEHANDLE fh) {
  if (fh > 0x8000) {
    return (0);
  }
  return (__sys_flen (fh));
}

/*--------------------------- _sys_tmpnam -----------------------------------*/

int _sys_tmpnam (char *name, int sig, unsigned maxlen) {
  return (1);
}

/*--------------------------- _sys_command_string ---------------------------*/

char *_sys_command_string (char *cmd, int len) {
  return (cmd);
}

/*--------------------------- _sys_exit -------------------------------------*/

void _sys_exit (int return_code) {
  /* Endless loop. */
  while (1);
}

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
