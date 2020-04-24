/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rogenrl.h,v $
 * Revision 1.1.4.2  1996/03/11  13:20:42  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:37  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:34:43  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:19:19  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:05:38  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  14:59:14  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:15  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:57:17  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:35:22  keutel
 * 	creation
 * 	[1994/03/21  13:26:39  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROGENRL_H
#define _ROGENRL_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _rogenrl_rcsid[] = { "@(#) $RCSfile: rogenrl.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:20:42 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)                                                  */
/*  * Siemens Nixdorf Informationssysteme AG 1992                     */
/*  * All rights reserved                                             */
/*  *                                                                 */
/*  * This software is furnished under licence and may be used only   */
/*  * in accordance with the terms of that licence and with the       */
/*  * inclusion of the above copyright notice.                        */
/*  * This software may not be provided or otherwise made available   */
/*  * to, or used by, any other person. No title to or ownership of   */
/*  * the software is hereby  transferred.                            */
/*                                                                    */
/* ****************************************************************** */

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : INCLUDE                                             */
/*                                                                    */
/* NAME         : rogenrl.h                                           */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 92-03-25                                            */
/*                                                                    */
/* COMPONENT    : RTS                                                 */
/*                                                                    */
/* PRD#/VERS.   : RTS-V02.00                                          */
/*                                                                    */
/* DESCRIPTION  : RTS General and system dependend declarations       */
/*                                                                    */
/* SYSTEM DEPENDENCE :                                                */
/*                                                                    */
/*       RP0000_MicV0600                                              */
/*       RP0002_WinIncl                                               */
/*                                                                    */
/*                                                                    */
/* HISTORY      : sccsid  = "@(#) @(#)rogenrl.h1.193/11/29"                       */
/*                                                                    */
/* Vers.Nr. | Date    |  changes                       |mark| CR# EM# */
/* 2.00 K0  | 92-03-25|  original                      |jm  |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/


/* ****************** Declaration of NEAR, FAR, PASCAL and *********** */
/* ****************** EXPORT                               *********** */

#ifdef  RP0000_MicV0600

#ifndef RP0002_WinIncl
#define NEAR    near
#define FAR     far
#define PASCAL  pascal
#define HUGE    huge
#endif

#ifndef __STDC__
#define __STDC__
#endif

#define EXPORT _export
#else

#define NEAR
#define FAR
#define PASCAL
#define EXPORT
#define HUGE
#endif

/* ****************** Function prototypeing macro ******************** */
/* ******************                             ******************** */

#ifdef  __STDC__
#define PROTO(argl) argl
#else
#define PROTO(argl) ()
#endif

/* ****************** Usefull declarations *************************** */
/* ******************                      *************************** */

#ifdef ERROR            /* skh: Dos warning */
#undef ERROR
#endif
#define ERROR   -1
#define OK      0
#define READY   1
#ifdef NULL             /* skh: Dos warning */
#undef NULL
#endif
#define NULL    0
#define LNULL   0L

#ifndef RP0002_WinIncl
#ifndef FALSE
#define FALSE   0
#endif
#ifndef TRUE
#define TRUE    !FALSE
#endif

#ifndef MIN
#define MIN(a, b)       (( (a) <= (b) ) ? (a) : (b) )
#endif
#ifndef MAX
#define MAX(a, b)       (( (a) >= (b) ) ? (a) : (b) )
#endif

#define LONIBBLE(a)     ( (unsigned char) (  (char)(a)     & 0x0f))
#define HINIBBLE(a)     ( (unsigned char) ( ((char)(a)>>4) & 0x0f))

#define LOBYTE(a)       ((unsigned char)(a))
#define HIBYTE(a)       ((unsigned char)(((unsigned short)(a)>>8&0x0ff)))

#define LOWORD(a)       ((unsigned short)(a))
#define HIWORD(a)       ((unsigned short)(((unsigned long)(a)>>16)&0xffff))

#define MAKEBYTE(lo,hi) ((((unsigned char) (hi)) <<4) | ((unsigned char) (lo)))
#define MAKEWORD(lo,hi) ((((unsigned short)(hi)) <<8) | ((unsigned short)(lo)))
#define MAKELONG(lo,hi) (((unsigned long)(hi)<<16)|((unsigned short)(lo)))

/* typedef int (FAR PASCAL *FARPROC)();  GCC */
typedef int (FAR PASCAL *FARPROC)(int , void FAR *); 
#endif

#define MAXLONG_POS     (0x7fffffffL)    /* Maximum size of a +ve long value  */
                                         /* of 4 bytes cannot be greater than */
                                         /* +2147483647.                      */

#endif	/* _ROGENRL_H */
