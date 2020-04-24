/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: isam.h,v $
 * Revision 1.1.10.2  1996/02/18  23:34:53  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:24:24  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:28:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:54:46  root]
 * 
 * Revision 1.1.8.2  1994/06/21  14:44:48  marrek
 * 	June 1994 code submission.
 * 	[1994/06/17  11:40:08  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  16:03:56  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:57:10  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  12:55:56  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:13:15  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  18:49:41  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:15:11  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:36:10  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:39:55  zeliff]
 * 
 * $EndLog$
 */
#ifndef _ISAM_H
#define _ISAM_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char isam_rcsid[] = "@(#)$RCSfile: isam.h,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 23:34:53 $";
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
 *  Title:	isam.h
 *  Description:
 *		Header file for programs using C-ISAM.
 *
 ***************************************************************************
 */


/*
 *       C-ISAM version 3.00
 *  Indexed Sequential Access Method
 *  Relational Database Systems, Inc.
 */

#define CHARTYPE	0
#define DECIMALTYPE	0
#define CHARSIZE	1

#define INTTYPE		1
#define INTSIZE		2

#define LONGTYPE	2
#define LONGSIZE	4

#define DOUBLETYPE	3
#ifndef NOFLOAT
#define DOUBLESIZE	(sizeof(double))
#endif /* NOFLOAT */

#ifndef NOFLOAT
#define FLOATTYPE	4
#define FLOATSIZE	(sizeof(float))
#endif /* NOFLOAT */

#define USERCOLL(x)	((x))

#define COLLATE1	0x10
#define COLLATE2	0x20
#define COLLATE3	0x30
#define COLLATE4	0x40
#define COLLATE5	0x50
#define COLLATE6	0x60
#define COLLATE7	0x70

#define MAXTYPE		5
#define ISDESC		0x80	/* add to make descending type	*/
#define TYPEMASK	0x7F	/* type mask			*/

#ifdef BBN			/* BBN Machine has 10 bits/byte	*/
#define BYTEMASK  0x3FF		/* mask for one byte		*/
#define BYTESHFT  10		/* shift for one byte		*/
#else
#define BYTEMASK  0xFF		/* mask for one byte		*/
#define BYTESHFT  8		/* shift for one byte		*/
#endif

#ifndef	ldint
#define ldint(p)	((short)(((p)[0]<<BYTESHFT)+((p)[1]&BYTEMASK)))
#define stint(i,p)	((p)[0]=(i)>>BYTESHFT,(p)[1]=(i))
#endif	

#ifndef	ldlong
long ldlong(register char *);
#endif	

#ifndef NOFLOAT
#ifndef	ldfloat
double	ldfloat(register char *);
#endif	
#ifndef	lddbl
double	lddbl(register char *);
#endif	
double ldfltnull(char *, short *);
double lddblnull(char *, short *);
#endif

#define ISFIRST		0	/* position to first record	*/
#define ISLAST		1	/* position to last record	*/
#define ISNEXT		2	/* position to next record	*/
#define ISPREV		3	/* position to previous record	*/
#define ISCURR		4	/* position to current record	*/
#define ISEQUAL		5	/* position to equal value	*/
#define ISGREAT		6	/* position to greater value	*/
#define ISGTEQ		7	/* position to >= value		*/

/* isread lock modes */
#define ISLOCK     	0x100	/* record lock			*/
#define ISWAIT		0x400	/* wait for record lock		*/
#define ISLCKW		0x500   /* ISLOCK + ISWAIT              */

/* isopen, isbuild lock modes */
#define ISAUTOLOCK	0x200	/* automatic record lock	*/
#define ISMANULOCK	0x400	/* manual record lock		*/
#define ISEXCLLOCK	0x800	/* exclusive isam file lock	*/

#define ISINPUT		0	/* open for input only		*/
#define ISOUTPUT	1	/* open for output only		*/
#define ISINOUT		2	/* open for input and output	*/
#define ISTRANS		4	/* open for transaction proc	*/
#define ISNOLOG		8	/* no loggin for this file	*/

/* audit trail mode parameters */
#define AUDSETNAME	0	/* set new audit trail name	*/
#define AUDGETNAME	1	/* get audit trail name		*/
#define AUDSTART	2	/* start audit trail 		*/
#define AUDSTOP		3	/* stop audit trail 		*/
#define AUDINFO		4	/* audit trail running ?	*/

#define MAXKEYSIZE	120	/* max number of bytes in key	*/
#define NPARTS		8	/* max number of key parts	*/

struct keypart
    {
    short kp_start;		/* starting byte of key part	*/
    short kp_leng;		/* length in bytes		*/
    short kp_type;		/* type of key part		*/
    };

struct keydesc
    {
    short k_flags;		/* flags			*/
    short k_nparts;		/* number of parts in key	*/
    struct keypart
	k_part[NPARTS];		/* each key part		*/
		    /* the following is for internal use only	*/
    short k_len;		/* length of whole key		*/
    long k_rootnode;		/* pointer to rootnode		*/
    };
#define k_start   k_part[0].kp_start
#define k_leng    k_part[0].kp_leng
#define k_type    k_part[0].kp_type

#define ISNODUPS  000		/* no duplicates allowed	*/
#define ISDUPS	  001		/* duplicates allowed		*/
#define DCOMPRESS 002		/* duplicate compression	*/
#define LCOMPRESS 004		/* leading compression		*/
#define TCOMPRESS 010		/* trailing compression		*/
#define COMPRESS  016		/* all compression		*/
#define ISCLUSTER 020		/* index is a cluster one       */

struct dictinfo
    {
    short di_nkeys;		/* number of keys defined	*/
    short di_recsize;		/* data record size		*/
    short di_idxsize;		/* index record size		*/
    long di_nrecords;		/* number of records in file	*/
    };

#define EDUPL	  100		/* duplicate record	*/
#define ENOTOPEN  101		/* file not open	*/
#define EBADARG   102		/* illegal argument	*/
#define EBADKEY   103		/* illegal key desc	*/
#define ETOOMANY  104		/* too many files open	*/
#define EBADFILE  105		/* bad isam file format	*/
#define ENOTEXCL  106		/* non-exclusive access	*/
#define ELOCKED   107		/* record locked	*/
#define EKEXISTS  108		/* key already exists	*/
#define EPRIMKEY  109		/* is primary key	*/
#define EENDFILE  110		/* end/begin of file	*/
#define ENOREC    111		/* no record found	*/
#define ENOCURR   112		/* no current record	*/
#define EFLOCKED  113		/* file locked		*/
#define EFNAME    114		/* file name too long	*/
#define ENOLOK    115		/* can't create lock file */
#define EBADMEM   116		/* can't alloc memory	*/
#define EBADCOLL  117		/* bad custom collating	*/
#define ELOGREAD  118		/* cannot read log rec  */
#define EBADLOG   119		/* bad log record	*/
#define ELOGOPEN  120		/* cannot open log file	*/
#define ELOGWRIT  121		/* cannot write log rec */
#define ENOTRANS  122		/* no transaction	*/
#define ENOSHMEM  123		/* no shared memory	*/
#define ENOBEGIN  124		/* no begin work yet	*/
#define ENONFS    125		/* can't use nfs 	*/
#define EBADROWID 126		/* bad rowid		*/
#define ENOPRIM   127		/* no primary key	*/
#define ENOLOG    128		/* no logging		*/
#define EUSER     129		/* too many users	*/
#define ENODBS    130		/* no such dbspace	*/
#define ENOFREE   131		/* no free disk space	*/
#define EROWSIZE  132		/* row size too big	*/
#define EAUDIT	  133		/* audit trail exists   */
#define ENOLOCKS  134		/* no more locks	*/
#define ENOPARTN  135		/* partition doesn't exist */
#define ENOEXTN   136		/* no more extents	*/
#define EOVCHUNK  137		/* chunk table overflow	*/
#define EOVDBS    138		/* dbspace table ovflow	*/
#define EOVLOG    139		/* logfile table ovflow	*/
#define EGBLSECT  140		/* global section disallowing access - VMS */
#define EOVPARTN  141		/* partition table ovfo	*/
#define EOVPPAGE  142		/* overflow partn page	*/
#define EDEADLOK  143		/* deadlock detected	*/
#define EKLOCKED  144		/* key value locked	*/
#define EDEADDEM  149		/* turbo demon has died */

/*
 * For system call errors
 *   iserrno = errno (system error code 1-99)
 *   iserrio = IO_call + IO_file
 *		IO_call  = what system call
 *		IO_file  = which file caused error
 */

#define IO_OPEN	  0x10		/* open()	*/
#define IO_CREA	  0x20		/* creat()	*/
#define IO_SEEK	  0x30		/* lseek()	*/
#define IO_READ	  0x40		/* read()	*/
#define IO_WRIT	  0x50		/* write()	*/
#define IO_LOCK	  0x60		/* locking()	*/
#define IO_IOCTL  0x70		/* ioctl()	*/

#define IO_IDX	  0x01		/* index file	*/
#define IO_DAT	  0x02		/* data file	*/
#define IO_AUD	  0x03		/* audit file	*/
#define IO_LOK	  0x04		/* lock file	*/
#define IO_SEM	  0x05		/* semaphore file */

extern int iserrno;		/* isam error return code	*/
extern int iserrio;		/* system call error code	*/
extern long isrecnum;		/* record number of last call	*/
extern char isstat1;		/* cobol status characters	*/
extern char isstat2;
extern char *isversnumber;	/* C-ISAM version number	*/
extern char *iscopyright;	/* RDS copyright		*/
extern char *isserial;		/* C-ISAM software serial number */
extern int  issingleuser;	/* set for single user access	*/
extern int  is_nerr;		/* highest C-ISAM error code	*/
extern char *is_errlist[];	/* C-ISAM error messages	*/
/*  error message usage:
 *	if (iserrno >= 100 && iserrno < is_nerr)
 *	    printf("ISAM error %d: %s\n", iserrno, is_errlist[iserrno-100]);
 */

struct audhead
    {
    char au_type[2];		/* audit record type aa,dd,rr,ww*/
    char au_time[4];		/* audit date-time		*/
    char au_procid[2];		/* process id number		*/
    char au_userid[2];		/* user id number		*/
    char au_recnum[4];		/* record number		*/
    };
#define AUDHEADSIZE  14		/* num of bytes in audit header	*/

#endif /* _ISAM_H */
