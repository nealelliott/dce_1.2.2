/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: isdec.c,v $
 * Revision 1.1.10.2  1996/02/18  19:44:12  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:25:14  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:29:28  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:54:52  root]
 * 
 * Revision 1.1.8.1  1994/02/22  16:04:09  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:58:14  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  13:02:30  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:05:11  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  18:50:23  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:15:39  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:37:04  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:40:38  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char isdec_rcsid[] = "@(#)$RCSfile: isdec.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:44:12 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/***************************************************************************
 *
 *			   INFORMIX SOFTWARE, INC.
 *
 *			      PROPRIETARY DATA
 *
 *	THIS DOCUMENT CONTAINS TRADE SECRET DATA WHICH IS THE PROPERTY OF 
 *	INFORMIX SOFTWARE, INC.  THIS DOCUMENT IS SUBMITTED TO RECIPIENT IN
 *	CONFIDENCE.  INFORMATION CONTAINED HEREIN MAY NOT BE USED, COPIED OR 
 *	DISCLOSED IN WHOLE OR IN PART EXCEPT AS PERMITTED BY WRITTEN AGREEMENT 
 *	SIGNED BY AN OFFICER OF INFORMIX SOFTWARE, INC.
 *
 *	THIS MATERIAL IS ALSO COPYRIGHTED AS AN UNPUBLISHED WORK UNDER
 *	SECTIONS 104 AND 408 OF TITLE 17 OF THE UNITED STATES CODE. 
 *	UNAUTHORIZED USE, COPYING OR OTHER REPRODUCTION IS PROHIBITED BY LAW.
 *
 *
 *  Title:	isdec.c
 *  Description:
 *		Global variable declarations for C-ISAM routines.
 *
 ************************************************************************
 */


#ifndef DEC_INCL
#define DEC_INCL
#include <btree.h>

#ifdef ISKERNEL
#include <kbtree.h>
#endif /* ISKERNEL */

#ifdef VMS
extern char cleanup_flag;
#endif /* VMS */

extern char *isversnumber;
extern char *iscopyright;
extern char *isserial;

#ifdef lint
extern int _VOID_;
#endif

#ifndef ISKERNEL
extern int errno;
extern int iserrno;
extern int iserrio;
extern char isstat1;
extern char isstat2;
extern int  tempisfd;
extern long tempnum;
extern struct item treeitem;
extern short treelev;
extern short treeleaf;
extern short treeflags;
extern struct ifile *filep;
extern struct iopen *openp;
extern struct dict *dictp;
#endif  /* ISKERNEL */

extern struct reclist *rlfree;
extern short nfiles;
extern struct ifile *files[NFILES];
extern short nopens;
#ifdef ISKERNEL
extern struct kobuf kotab[NOPENS];
#else
extern struct iopen *opens[NOPENS];
#endif  /* ISKERNEL */
extern short nbuffs;
extern struct buff avbuffs;

#ifndef ISALLOC
extern struct ifile filetab[NFILES];
#ifndef ISKERNEL
extern struct iopen opentab[NOPENS];
#endif  /* ISKERNEL */
#endif  /* ISALLOC */

#ifdef ISALLOC
extern struct buff *buffs;
extern char *buffers;
#else
extern struct buff buffs[NBUFS];
#ifndef ISKERNEL
extern char buffers[NBUFS*NODESIZE];
#endif  /* ISKERNEL */
#endif /* ISALLOC */

#ifdef CREATLOCK
extern struct lock locks;
#endif

#ifdef DRVRLOCK
extern short lkfd;
extern struct liocb liocb;
extern struct lock  locks[NLOCKS];
#endif

#ifndef ISKERNEL
extern char *temprecord;
extern short temprsize;
#endif  /* ISKERNEL */
#endif  /* DEC_INCL */

extern short txflag;			/* transaction flag */
