/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cmxl3.h,v $
 * Revision 1.1.4.2  1996/03/11  13:24:21  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:12:45  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:48:02  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:25:22  root]
 * 
 * Revision 1.1.2.2  1994/06/10  21:17:23  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:54:41  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/10  16:15:26  marrek
 * 	Created for April Submission.
 * 	[1994/04/26  12:51:14  marrek]
 * 
 * $EndLog$
 */
#ifndef _CMXL3_H
#define _CMXL3_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char cmxl3_rcsid[] = "@(#)$RCSfile: cmxl3.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:24:21 $";
#endif

/*
 * COPYRIGHT (C) 1990 BY SIEMENS AG All rights reserved
 * COPYRIGHT (C) Siemens Nixdorf Informationssysteme AG 1990
 *               All Rights Reserved
 *  
 * This software is furnished under a license and may be used
 * only in accordance with the terms of that license and with the
 * inclusion of the above copyright notice. This software may not
 * be provided or otherwise made available to, or used by, any
 * other person. No title to or ownership of the software is
 * hereby transferred.
 */

/* #ident " @(#)cmxl3.h	311.3 93/08/24 CMX (NC1)" */

/*
 * CMX library header file
 *
 * Definitions for the CMX module for system coupling
 */

#define CX_NETCFILE     "/dev/cmx/net0000"      /* Name network device files */
#define CX_NETFILE      "/dev/cmx/net0000"      /* Name network device files */
#define CX_FILE         "/dev/cmx/cx"           /* Device file for CMX automoton */

#define CLOSE_ON_EXEC   1       /* Parameter for fcntl() */

#define NAMLENGTH       (TS_LCOUNTRY + TS_LADMD + TS_LPRMD + TS_LOU + TS_LPN)

/*
 * The library buffers a TIDU/ETSDU on the receive side for the caller.
 * While reading a TIDU/ETSDU bit by bit (i.e. provided this buffer is
 * not empty) other CMX calls are not permitted in the process, except for
 * t_detach(), t_disrq() (because of t_event(), t_disin() is also omitted).
 * One buffer per process is sufficient, and this is allocated and
 * released with t_[vx]datain().
 */
struct du_buf {
	int     state;  /* TIDU buffer information */
#define NOBUF   0       /* TIDU buffer not created */
#define EVENT   1       /* TIDU displayed for TIDU buffer */
#define FULL    2       /* TIDU is in TIDU buffer */
	int     attid;  /* up_aattid of CMX application, which assigns it */
	int     tref;   /* dto. tref of transport connection */
	int     cnt;    /* Length of data in TIDU buffer */
	char    *sptr;  /* Start of TIDU buffer */
	char    *aptr;  /* curr. data start in TIDU buffer (read marker) */
	int     chain;  /* TSDU end or TSDU continuation (chain) */
};

/*
 * Renaming of function names (due to station/system coupling)
 * The actual calls "t_xxxxx" branch depending on the connection
 * after "t_1ixxxxx" (system coupling) or "t_2ixxxxx" (station coupling),
 * further after "t_3ixxxxx", or (i = 0, 1, 2, ...)
 */
#define t_attach	t_100attach
#define t_detach	t_101detach
#define t_conrq		t_102conrq
#define t_conin		t_103conin
#define t_conrs		t_104conrs
#define t_concf		t_105concf
#define t_datarq	t_106datarq
#define t_datain	t_107datain
#define t_datago	t_108datago
#define t_datastop	t_109datastop
#define t_redrq		t_110redrq
#define t_redin		t_111redin
#define t_xdatrq	t_112xdatrq
#define	t_xdatin	t_113xdatin
#define t_xdatgo	t_114xdatgo
#define t_xdatstop	t_115xdatstop
#define t_disrq		t_116disrq
#define t_disin		t_117disin
#define t_event		t_118event
#define t_error		t_119error
#define t_info		t_120info
#define t_vdatarq	t_121vdatarq
#define t_vdatain	t_122vdatain
#endif /* _CMXL3_H */
