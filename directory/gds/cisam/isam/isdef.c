/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: isdef.c,v $
 * Revision 1.1.10.2  1996/02/18  19:44:13  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:25:16  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:29:31  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:54:53  root]
 * 
 * Revision 1.1.8.1  1994/02/22  16:04:11  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:58:22  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  13:03:21  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:05:21  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  18:50:28  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:15:43  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:37:14  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:40:43  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: isdef.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:44:13 $";
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
 *  Title:	isdef.c
 *  Description:
 *		Global variable definitions for C-ISAM routines.
 *
 ************************************************************************
 */


#include <btree.h>
#include <isvers.h>

#ifdef ISKERNEL
#include <kbtree.h>
#endif /* ISKERNEL */

#ifdef VMS
char cleanup_flag = 0;
#endif /* VMS */

char *isversnumber = ISVERSION;
char *iscopyright  = COPYRT;
extern char snnumber[];
extern short demoversion;
char *isserial = snnumber;
int issingleuser   = 0;

#ifdef lint
STATIC int _VOID_ = 0;
#endif

#ifndef ISKERNEL
extern int errno;
int  iserrno;
int  iserrio;
long isrecnum;
char isstat1;
char isstat2;
int  tempisfd = -1;
long tempnum  = -1;

STATIC struct item treeitem;
STATIC short treelev;
STATIC short treeleaf;
STATIC short treeflags;
STATIC struct ifile *filep;
STATIC struct iopen *openp;
STATIC struct dict *dictp;
#endif  /* ISKERNEL */

STATIC struct reclist *rlfree = NULL;
STATIC short nfiles = NFILES;
STATIC struct ifile *files[NFILES];
STATIC short nopens = NOPENS;
#ifdef ISKERNEL
STATIC struct kobuf kotab[NOPENS];
#else
STATIC struct iopen *opens[NOPENS];
#endif /* ISKERNEL */

#ifndef ISALLOC
STATIC struct ifile filetab[NFILES];
#ifndef ISKERNEL
STATIC struct iopen opentab[NOPENS];
#endif  /* ISKERNEL */
#endif  /* ISALLOC */

STATIC short nbuffs = NBUFS;
STATIC struct buff avbuffs;

#ifdef ISALLOC
STATIC struct buff *buffs;
STATIC char *buffers;
#else
STATIC struct buff buffs[NBUFS];
#ifndef ISKERNEL
STATIC char buffers[NBUFS*NODESIZE];
#endif  /* ISKERNEL */
#endif /* ISALLOC */

#ifdef CREATLOCK
STATIC struct lock locks;
#endif

#ifdef DRVRLOCK
STATIC short lkfd = -1;
STATIC struct liocb liocb;
STATIC struct lock  locks[NLOCKS];
#endif

#ifndef ISKERNEL
STATIC char *temprecord;
STATIC short temprsize;
#endif

