/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sn.h,v $
 * Revision 1.1.10.2  1996/02/18  23:34:58  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:24:31  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:30:42  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:55:12  root]
 * 
 * Revision 1.1.8.1  1994/02/22  16:04:38  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:04:44  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  13:39:48  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:08:50  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  18:57:44  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:16:52  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:41:02  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:42:20  zeliff]
 * 
 * $EndLog$
 */
#ifndef _SN_H
#define _SN_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char sn_rcsid[] = "@(#)$RCSfile: sn.h,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 23:34:58 $";
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
 *  Title:	sn.h
 *  Description:
 *		Header file for serial number routines.
 *
 ************************************************************************
 */


#ifndef snnumber		/* here, and in incl/rdsmac.h */
#define snnumber rsnnumber
#endif /* snnumber */

#define	WRONG	0
#define	DEMO	1
#define	REAL	2
#define	IDENTSTRING	"\333\333\231\231\231\231\333\333"
#define	SIZEIDENT	8
/* The INFBLANK string is IDENTSTRING followed by "RDS#N000000SWHP"
 * that has been exclusively ored by ^0xDB character by character.
 */
#define INFBLANK	"\333\333\231\231\231\231\333\333\211\237\210\370\225\353\353\353\353\353\353\210\214\223\213\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
#define INFSTRSIZE	11	/* AAA-A123456 */
#define INFCOMPPART	3	/* ^^^         */
#define INFDEMOPART	INFCOMPPART+1
#define INFDIGITSTART	INFCOMPPART+2
#define NETNAMEPOS SIZEIDENT+INFSTRSIZE+5

/* WARNING - the following should not be changed unless modifying brand */

#define	CHECKDIGITS	2

/*
 * the following is to disguise various aspects of serial routines
 */

#ifndef snverify
#define	snverify	windw
#endif

#ifndef snchk
#define	snchk		lister
#endif 

#define	funcptr		ldv
#define	validate	untrp
#define snsave		togrset

#define real		windf
#define demo		windt
#define wrong		windx

#define demoversion	rprtag_

#define SERLETTER 27
#define LOCALONLY  (snnumber[SERLETTER]=='R'||snnumber[SERLETTER]=='D')
#define REMOTEONLY  (snnumber[SERLETTER]=='C')
extern char snnumber[];


#endif /* _SN_H */
