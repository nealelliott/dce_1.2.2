/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */

/*
 * HISTORY
 * $Log: dtstime.c,v $
 * Revision 1.1.8.2  1996/02/17  23:35:09  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:51:20  marty]
 *
 * Revision 1.1.8.1  1995/12/08  18:12:45  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1994/06/30  12:47 UTC  pare
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1995/12/08  17:25:12  root]
 * 
 * Revision 1.1.6.1  1994/06/10  20:52:06  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:48:51  devsrc]
 * 
 * Revision 1.1.6.2  1993/07/08  19:52:04  root
 * 	Initial King Kong branch
 * 	[1993/07/08  19:51:37  root]
 * 
 * Revision 1.1.4.2  1993/03/24  18:41:04  truitt
 * 	Initial GAMERA submit.
 * 	[1993/03/24  18:40:21  truitt]
 * 
 * Revision 1.1.2.3  1992/12/07  14:43:46  smythe
 * 
 * 	changed name from adjtime.c to dtstime.c
 * 	changed name of call from adjtime to dtstime
 * 	added code to zero olddelta if provided
 * 	[1992/12/07  14:28:24  smythe]
 * 
 * Revision 1.1.2.2  1992/12/07  13:44:01  jake
 * 
 * 	First entry of dtstime.c for adjtime.c
 * 	jake for the smythe-man
 * 	[1992/12/07  13:43:20  jake]
 * 
 * Revision 1.1.2.4  1992/10/30  19:48:00  smythe
 * 
 * 	rewritten and simplified for hpux dce dts
 * 	[1992/10/30  19:43:48  smythe]
 * 
 * Revision 1.1.2.3  1992/07/09  17:24:26  smythe
 * 
 * 	changed #include "adjtime.h" to #include <adjtime.h>
 * 	[1992/07/09  17:23:49  smythe]
 * 
 * Revision 1.1.2.2  1992/06/26  20:09:21  smythe
 * 
 * 	library module to interface to the adjtimed
 * 	daemon for adjtime call emulation on hpux
 * 	[1992/06/26  20:08:28  smythe]
 * 
 * $EndLog$
 */

/*************************************************************************/
/* (c) Copyright Tai Jin, 1988.  All Rights Reserved.                    */
/*     Hewlett-Packard Laboratories.                                     */
/*                                                                       */
/* Permission is hereby granted for unlimited modification, use, and     */
/* distribution.  This software is made available with no warranty of    */
/* any kind, express or implied.  This copyright notice must remain      */
/* intact in all versions of this software.                              */
/*                                                                       */
/* The author would appreciate it if any bug fixes and enhancements were */
/* to be sent back to him for incorporation into future versions of this */
/* software.  Please send changes to tai@iag.hp.com or ken@sdd.hp.com.   */
/*************************************************************************/

/* dtstime stub for gradual clock adjustment */
/* rewritten and simplified for hpux dce dts */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <signal.h>
#include "dtstime.h"

int dtstime(delta, olddelta)
  struct timeval *delta;
  struct timeval *olddelta;  /* unused */
{
  int mqid;
  msgbuf msg;

  /* get queue every time in case it has been recycled */
  if ((mqid = msgget(msgkey, 0)) == -1) return (-1);
  msg.mtype = msgkey;
  msg.delta = *delta;
  if (msgsnd(mqid, &msg, msgsize, 0) == -1) return (-1);
  if (olddelta != NULL) {
     olddelta->tv_sec = (unsigned long) 0L;
     olddelta->tv_usec = 0L;
  }
  return (0);
}

