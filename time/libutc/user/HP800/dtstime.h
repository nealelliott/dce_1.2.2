/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */

/*
 * HISTORY
 * $Log: dtstime.h,v $
 * Revision 1.1.8.2  1996/02/18  23:12:15  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:37:12  marty]
 *
 * Revision 1.1.8.1  1995/12/08  18:12:49  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1994/06/30  12:48 UTC  pare
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1995/12/08  17:25:13  root]
 * 
 * Revision 1.1.6.1  1994/06/10  20:52:26  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:48:52  devsrc]
 * 
 * Revision 1.1.6.2  1993/07/08  19:53:23  root
 * 	Initial King Kong branch
 * 	[1993/07/08  19:52:55  root]
 * 
 * Revision 1.1.4.2  1993/03/24  18:43:10  truitt
 * 	Initial GAMERA submit.
 * 	[1993/03/24  18:42:09  truitt]
 * 
 * Revision 1.1.2.3  1992/12/07  14:43:54  smythe
 * 
 * 	changed name from adjtime.h to dtstime.h
 * 	changed key for enhanced privacy
 * 	[1992/12/07  14:20:53  smythe]
 * 
 * Revision 1.1.2.2  1992/12/07  13:39:25  jake
 * 
 * 	First entry of dtstime.h for adjtime.h
 * 	jake for the smythe-man
 * 	[1992/12/07  13:37:34  jake]
 * 
 * Revision 1.1.2.3  1992/10/30  19:48:03  smythe
 * 
 * 	rewritten and simplified for hpux dce dts
 * 	[1992/10/30  19:44:24  smythe]
 * 
 * Revision 1.1.2.2  1992/06/26  20:11:55  smythe
 * 
 * 	adjtime include file for adjtime library module and
 * 	adjtime daemon which emulate the adjtime call on hpux
 * 	[1992/06/26  20:11:07  smythe]
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

/* dtstime header file */
/* rewritten and simplified for hpux dce dts */

#define msgkey 659618L

#define msgsize sizeof(struct timeval)

typedef struct {
  long mtype;
  struct timeval delta;
} msgbuf;

