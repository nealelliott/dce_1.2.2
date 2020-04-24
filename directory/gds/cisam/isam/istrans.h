/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: istrans.h,v $
 * Revision 1.1.10.2  1996/02/18  23:34:54  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:24:26  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:30:10  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:55:03  root]
 * 
 * Revision 1.1.8.1  1994/02/22  16:04:29  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:04:04  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  13:13:27  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:07:33  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  18:57:11  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:16:23  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:38:29  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:41:41  zeliff]
 * 
 * $EndLog$
 */
#ifndef _ISTRANS_H
#define _ISTRANS_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char istrans_rcsid[] = "@(#)$RCSfile: istrans.h,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 23:34:54 $";
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
 *  Title:	isxaction.h
 *  Description:
 *		Global defines for C-ISAM transaction routines.
 *
 ***************************************************************************
 */




/* log record offset definition */

/* record header definition */
#define LG_LEN		0			/* current record length */
#define LG_TYPE		LG_LEN+INTSIZE		/* log record type */
#define LG_XID		LG_TYPE+2		/* transaction id */
#define LG_USER		LG_XID+INTSIZE		/* user name */
#define LG_TIME		LG_USER+2		/* transaction time */
#define LG_PREV		LG_TIME+LONGSIZE	/* previous log record */
#define LG_PREVLEN	LG_PREV+LONGSIZE	/* previous log length */


/* BEGIN, COMMIT and ROLLBACK WORK record definition */
#define LG_TXSIZE	LG_PREVLEN+INTSIZE+INTSIZE/* record size */

/* build file record definition */
#define LG_FMODE	LG_PREVLEN+INTSIZE	/* build mode */
#define LG_RECLEN	LG_FMODE+INTSIZE	/* record length */
#define LG_KFLAGS	LG_RECLEN+INTSIZE	/* key flag */
#define LG_NPARTS	LG_KFLAGS+INTSIZE	/* number of key parts */
#define LG_KLEN		LG_NPARTS+INTSIZE	/* total key length */

/* erase file record definition */
#define LG_FNAME	LG_PREVLEN+INTSIZE	/* directory path name */


/* rename file record definition */
#define LG_OLEN		LG_PREVLEN+INTSIZE	/* length of old filename */
#define LG_NLEN		LG_OLEN+INTSIZE		/* length of new filename */
#define LG_ONAME	LG_NLEN+INTSIZE		/* old filename */


/* open and close file record definition */
#define LG_ISFD		LG_PREVLEN+INTSIZE	/* isfd of file */
#define LG_FPATH	LG_ISFD+INTSIZE		/* directory path name */


/* create and drop index */
#define LG_IFLAGS	LG_ISFD+INTSIZE		/* key flags */
#define LG_INPARTS	LG_IFLAGS+INTSIZE	/* number of key parts */
#define LG_IKLEN	LG_INPARTS+INTSIZE	/* total key length */


/* set unique id */
#define LG_UNIQID	LG_ISFD+INTSIZE		/* new unique id */


/* before or after image record definition */
#define LG_RECNO	LG_ISFD+INTSIZE		/* record number */
#define LG_RECORD	LG_RECNO+LONGSIZE	/* record data */


/* update image (before and after together) */
#define LG_ILEN		LG_RECNO+LONGSIZE	/* image length */
#define LG_BUPDATE	LG_ILEN+INTSIZE		/* before image for update */


/* savepoint record */
#define LG_SAVEPT	LG_PREVLEN+INTSIZE	/* savepoint number */

#define LG_SSIZE	LG_SAVEPT+INTSIZE	/* record size */


#define LG_PAGESIZE	4096			/* default log buff size */

/* log record types */
#define LG_ERROR	0		/* log read or write error */
#define LG_BEGWORK	1		/* BEGIN WORK */
#define LG_COMWORK	2		/* COMMIT WORK */
#define LG_ROLWORK	3		/* ROLLBACK WORK */
#define LG_DELETE	4		/* deleted record */
#define LG_INSERT	5		/* newly inserted record */
#define LG_UPDATE	6		/* updated record */
#define LG_VERSION	7		/* version */
#define LG_SVPOINT	8		/* savepoint */
#define LG_FOPEN	9		/* open file */
#define LG_FCLOSE	10		/* close file */
#define LG_CKPOINT	11		/* checkpoint */
#define LG_BUILD	12		/* build new file */
#define LG_ERASE	13		/* erase old file */
#define LG_RFORWARD	14		/* ROLLFORWARD */
#define LG_CINDEX	15		/* create index */
#define LG_DINDEX	16		/* drop index */
#define LG_EOF		17		/* end of log file */
#define LG_RENAME	18		/* rename file */
#define LG_SETUNIQID	19		/* set unique id */
#define LG_UNIQUEID	20		/* get unique id */
#define LG_RBSVPT	21		/* rollback to savepoint */
#define LG_CLUSIDX      22		/* create cluster index  */


#define TRUE		1
#define FALSE		0

#define NOPNFL		16



struct txlist
    {
    int tx_xid;				/* transaction id */
    struct xrloc *tx_nextrec;		/* next log rec in transaction */
    struct txlist *tx_next;		/* next transaction */
    };

struct xrloc
    {
    int xr_logtype;			/* log record type */
    int xr_size;			/* log record size */
    long xr_loc;			/* location in log file */
    struct xrloc *xr_next;		/* next log rec in transaction */
    };


#endif /* _ISTRANS_H */
