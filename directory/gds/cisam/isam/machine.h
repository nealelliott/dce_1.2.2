/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: machine.h,v $
 * Revision 1.1.12.2  1996/02/18  23:34:57  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:24:30  marty]
 *
 * Revision 1.1.12.1  1995/12/08  15:30:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:55:10  root]
 * 
 * Revision 1.1.10.1  1994/02/22  16:04:36  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:05:06  marrek]
 * 
 * Revision 1.1.8.1  1993/10/28  13:33:04  keutel
 * 	CR 9279
 * 	[1993/10/28  13:24:26  keutel]
 * 
 * Revision 1.1.6.2  1993/08/11  13:37:59  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:08:30  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:57:38  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:16:45  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  17:24:43  marrek
 * 	November 1992 code drop
 * 	[1992/11/26  15:13:39  marrek]
 * 
 * Revision 1.1.2.3  1992/06/01  23:40:41  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:42:10  zeliff]
 * 
 * Revision 1.1.2.2  1992/04/09  20:08:10  jim
 * 	Comment strings after #endif and #else statements.
 * 	[1992/04/09  19:34:04  jim]
 * 
 * Revision 1.1  1992/01/19  15:12:53  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _MACHINE_H
#define _MACHINE_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char machine_rcsid[] = "@(#)$RCSfile: machine.h,v $ $Revision: 1.1.12.2 $ $Date: 1996/02/18 23:34:57 $";
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
 *  Title:	machine.h
 *  Description:
 *		Machine specific definitions.
 *		Defines in this file should be turned ON and OFF
 *		simply by removing or inserting an uncommented
 *		version of the define before the commented define.
 *
 ***************************************************************************
 */


/*
 *      SINIX   - SIEMENS common for all SINIX versions
 *
 *      M16S30  - SIEMENS 9780 with NS32016 or MX-?00 sie universe
 *      M32S50  - Sequent with multiple NS32016, System V
 *      ISAM_LOCK	- SIEMENS with NS32332, System V
 *      WX200	- SIEMENS with Intel 386, System V
 *  	PCDOS   - SIEMENS PCD port for NETSTATION
 */

#ifdef M16S30
#define ONIXLOCK		/* ONIX locking available	*/
#define FLTTYPE "IEEEI"         /* IEEE Intel byte order        */
#define VER7TTY 		/* UNIX Version 7 tty driver	*/
#define DEFPRINT   "lpr"	/* printer file is "lpr"	*/
#define DEFEDIT    "ced"	/* name of editor		*/
#define SIEVERSION "2.1V"
#define SINIX			/* common for all SINIX systems */
#endif

#ifdef ISAM_LOCK
#define SYS5LOCK		/* System V fcntl locking	*/
#define SYS3TTY 		/* UNIX System III tty driver	*/
#define FLTTYPE "IEEEI"         /* IEEE Intel byte order        */
#define NOFTIME 		/* ftime() system call missing	*/
#define NET			/* Some type of network         */
#define DEFPRINT   "lpr"	/* printer file is "lpr"	*/
#define DEFEDIT    "ced"	/* name of editor		*/
#define SIEVERSION "2.1V"
#define SINIX			/* common for all SINIX systems */
#endif

#ifdef WX200
#define SYS5LOCK		/* System V fcntl locking	*/
#define SYS3TTY 		/* UNIX System III tty driver	*/
#define FLTTYPE "IEEEI"         /* IEEE Intel byte order        */
#define NOFTIME 		/* ftime() system call missing	*/
#define NET			/* Some type of network         */
#define DEFPRINT   "lp -s" 	/* System V			*/
#define DEFEDIT    "vi"		/* name of editor		*/
#define SIEVERSION "2.1"
#define SINIX			/* common for all SINIX systems */
#define  CHECKERROR  " licence/keyinformation missing "
#define INFDEV 	"INFDEV"	/* Informix Development */
#define INF4GL 	"INF4GL"	/* Informix 4GL Development */
#define INFTURBO "INFTURBO"	/* Informix Turbo engine */
#define INFC	"INFC"		/* Informix ESQL-C */
#define INFCOB	"INFCOB"	/* Informix ESQL-COBOL */
#define INFNET	"INFNET"	/* Informix NET */
#define INFRUN	"INFRUN" 	/* Informix Runtime */
#define VERONE     "1.1"
#define VERTWO     "2.1"
#define VERTHREE   "3.1"
#endif

#ifdef M32S50
#define SYS5LOCK		/* System V fcntl locking	*/
#define SYS3TTY 		/* UNIX System III tty driver	*/
#define FLTTYPE "IEEEI"         /* IEEE Intel byte order        */
#define NOFTIME 		/* ftime() system call missing	*/
#define SEQUENT   		/* SEQUENT Operating System	*/
#define NET			/* Some type of network         */
#define DEFPRINT   "lpr"	/* printer file is "lpr"	*/
#define DEFEDIT    "ced"	/* name of editor		*/
#define SIEVERSION "2.1V"
#define SINIX			/* common for all SINIX systems */
#endif

#ifdef PCDOS
#define MSNET			/* locking for pc's 		*/
#define PCANSI			/* ANSI compatible terminal	*/
#define PCTERMCAP		/* You can specify $TERMCAP 	*/
#define PCMEMOMAP
#define FLTTYPE "IEEEI"         /* IEEE Intel byte order        */
#define NETSTATION		/* Only frontend on DOS machine */
#define PC_PROTECT
#define SMALLSYS		/* define this if it is "small system"	*/
#define DEFPRINT "lpt1"
#define DEFEDIT "edlin"
#define SIEVERSION "2.1DOS"
#define SINIX
#endif /* PCDOS */
/* common #defines for all SIEMENS machines */

#ifdef SINIX
#define NODESIZE   1024		/* ISAM nodesize		*/
#define ISALLOC 		/* dynamic buffer allocation	*/
#define ISBUFHASH		/* do buffer hashing for lookup	*/
#define ISXACTION		/* do logical transactions	*/
#define ISXBATCH		/* do extended batching 	*/
#define EIGHTBIT 		/* eight-bit version of informix */
#define PBUFSIZ	   8192		/* Pipe buffer size back/front  */
#define NOVALIDATE		/* no serial number validation  */
#define SCCSID			/* SCCS ID will be in objects	*/
#endif /* SINIX */

/***********************************************************************
 *
 * EIGHT BIT VERSIONS 
 *
 * use this define if the version is to allow all 8 bits for identifer
 * and data characters
 *
 ***********************************************************************/

/* #define EIGHTBIT */

/************************************************************************
 *
 * Define one of the following to select the desired locking mechanism.
 */

/* #define CREATLOCK */

/* #define MSNET		-- locking for pc's 		*/
/* #define ONIXLOCK		-- onix locking() system call	*/
/* #define CREATLOCK		-- create ".lok" file		*/
/* #define DRVRLOCK		-- use special lock driver	*/
/* #define MEMLOCK 		-- keep lock info in memory	*/
/* #define NOLOCK  		-- no locking, single user	*/
/* #define RDSLOCK 		-- rds locking for kernel	*/
/* #define SEMLOCK 		-- System V semaphore locking	*/
/* #define SHMLOCK		-- System V shared memory lock	*/
/* #define SHMBUFF		-- System V shared memory buffers*/
/* #define SYS5LOCK		-- System V fcntl locking	*/
/* #define ISKERNEL		-- kernel installation flag	*/

/************************************************************************
 *
 *  Informix-TURBO Defines
 *
 * For Shared Memory without assembly language routines
 * define MACROSEMA for shared memory semaphores.
 */

/* #define MACROSEMA		-- C-macro semaphores		*/

/*
 * Pick one of the following page sizes:
 *	1024, 2048, 4096, 8192
 */

#define PAGESIZE 2048

/*
 * SHMBASE is the base address of shared memory
 *  most machines use 0x300000 (3 megabytes)
 *  others need 0x80000 (1/2 megabyte)
 */

/* #define SHMBASE 0x80000 */

/************************************************************************
 *
 * Define Terminal I/O Specific Flags.
 */


/* #define VER7TTY 		-- UNIX Version 7 tty driver	*/
/* #define SYS3TTY 		-- UNIX System III tty driver	*/
/* #define PCDOSTTY		-- PCDOS special terminal (include PCDOS)
				   LEAVE OUT for generic MSDOS port*/

/************************************************************************
 *
 * Define the default printer file name.
 */


/* #define DEFPRINT "lpr"	-- Most common one		*/   
/* #define DEFPRINT "lp -s" 	-- System V			*/
/* #define DEFPRINT "lpt1"	-- PCDOS			*/
/* #define DEFPRINT "print"	-- PC/IX			*/
/* #define DEFPRINT "$printer"  -- VMS                          */

/************************************************************************
 *
 * Define the default editor file name.
 */


/* #define DEFEDIT "vi"		-- Most common one		*/   
/* #define DEFEDIT "e" 		-- PC/IX e (INed)		*/
/* #define DEFEDIT "edit/tpu"   -- VMS                          */
/* #define DEFEDIT "edline"	-- pcdos			*/

/************************************************************************
 *
 * Define Operating System Specific Flags.
 */

/* #define BERK4_2		-- Berkeley UNIX 4.2		*/
/* #define SEQUENT   		-- SEQUENT Operating System	*/
/* #define ZILOG   		-- ZEUS Operating System	*/
/* #define FORTUNE		-- FORTUNE Operating System	*/
/* #define CROMIX   		-- CROMIX Operating System	*/
/* #define PCDOS   		-- MS-DOS Operating System	*/
/* #define UNOS    		-- UNOS Operating System	*/
/* #define BBN			-- BBN Machine has 10 bits/byte	*/
/* #define PCIX			-- PC/IX Operating System	*/
/* #define VMS			-- VAX/VMS			*/


/*****************************************************************
 *  
 *  MODELS
 *
 * use this define if the machine will have dual large/medium models
 * for esqlc.
 * this will affect the code produced by i4gl in terms of the libraries
 * it links in and the compiler switches.
 * MOST LIKELY, this will be an xenix port or other 286 things
 *
 *******************************************************************/

/* #define MODELS		--dual large/medium models 	*/


/***********************************************************************
 * *******IMPORTANT********
 * Define whether or not machine is a "small system", with 64k addressable
 *  	data space.  This shrinks the number of isam buffers from 16 to
 *	8, and the node size from 1024 to 512
 * 	note -- xenix systems tend to be smallsys machines
 *	NOTE:  medium model compilation does not increase data space.
 *	       large model (if done) will do so
 *      However, define SMALLSYS even for large models, because
 *      a large model shipment will invariably be shipped in addition to
 *	a medium model, and BOTH models must run the same isam files,
 *      so if SMALLSYS is defined in medium, it must be defined in large.
 */

 /* #define SMALLSYS		-- define this if it is "small system"	*/


/************************************************************************
 *
 * Define Index Buffer Node Size (probably should be power of two)
 *
 * For VMS systems, NODESIZE must be a multiple of 512
 */

/*
#ifdef SMALLSYS
#define NODESIZE 512
#else
#define NODESIZE 1024
#endif
*/

/* #define NODESIZE 512		-- 512 byte blocks		*/
/* #define NODESIZE 1024	-- 1024 byte blocks		*/
/* #define NODESIZE 2048	-- 2048 byte blocks		*/
/* #define NODESIZE 4096	-- 4096 byte blocks		*/


/************************************************************************
 *
 * Define Miscellaneous System Specific Flags.
 */
#ifndef SINIX
#ifdef PCDOS
#ifndef CVTFLOAT
#define NOFLOAT
#endif
#endif
#endif /* SINIX */

#ifdef BERK4_2
#define SOCKETS
#endif
/*
#ifdef SOCKETS
#define NET
#endif
#ifdef TLI
#define NET
#endif
*/

/* #define ISALLOC 		-- dynamic buffer allocation	*/
/* #define ISBUFHASH		-- do buffer hashing for lookup	*/
/* #define ISBUFDATA		-- do data record buffering	*/
/* #define ISXACTION		-- do logical transactions	*/
/* #define ISPCLOSE		-- do pseudo closes	 	*/
/* #define ISXBATCH		-- do extended batching 	*/
/* #define NOFTIME 		-- ftime() system call missing	*/
/* #define NOAUDIT 		-- no audit trail code		*/
/* #define NOFLOAT 		-- no float or double types	*/
/* #define NOCASTS 		-- C-compiler has no casts	*/
/* #define NOLONGMULT		-- use software long multiply	*/
/* #define NOATOF		-- no atof() in C library (very rare) */
/* #define NOVALIDATE		-- no serial number validation	*/
/* #define CUSTCOLL		-- enable custom collating table*/
/* #define COBOL   		-- Special COBOL features	*/
/* #define SCCSID		-- SCCS ID will be in objects	*/
/* #define SOCKETS		-- BSD4.2 style network sockets */
/* #define TLI			-- Sys 5.2 TLI network interface */
/* #define NET			-- Some type of network         */
/* #define NFS			-- Machine supports nfs	        */

/******************************************************************
 * 	When defined, the following means that isql, ised, apsql,
 *	arsql, e3, and fm will be seperate programs, called
 *	by exec's from isql.
 *	This define will most likely be needed only on "small" machines
 *	(xenix?) which cannot handle massive programs over 300k bytes 
 *	easily.
 */
/* #define VEZPIECES		-- isql is broken up into pieces */


/************************************************************************
 *
 * The following defines relate to the closing
 * of files on an exec() system call.
 * If either of the first two are available, they should be used.
 * The first is used on systems which derive from Version 7.
 * The second is used on systems from System III.
 * The third is usable on any system, but does not insure closing
 * of C-ISAM files across an exec() call.
 */

#ifdef  VER7TTY
#ifdef  BERK4_2
#define CLOSEXEC(fd) fcntl((fd),_F_SETFD,1)	/* Berkeley 4.2	*/
#define _F_SETFD     2
#else
#define CLOSEXEC(fd) ioctl((fd),_FIOCLEX,0)	/* Version 7	*/
#define _FIOCLEX     (('f'<<8)|1)
#endif
#endif  /* VER7TTY */

#ifdef  SYS3TTY
#define CLOSEXEC(fd) fcntl((fd),_F_SETFD,1)	/* System III	*/
#define _F_SETFD     2
#endif  /* SYS3TTY */

#ifndef CLOSEXEC
#define CLOSEXEC(fd)				/* All others	*/
#endif  /* CLOSEXEC */


/************************************************************************
 *
 * Define miscellaneous system-specific values and resources.
 */

#ifdef	BBN			/* BBN Machine has 10 bits/byte	*/
				/* !! be sure to edit isam.h !!	*/
#define BYTEMASK  0x3FF		/* mask for one byte		*/
#define BYTESHFT  10		/* shift for one byte		*/
#else
#define BYTEMASK  0xFF		/* mask for one byte		*/
#define BYTESHFT  8		/* shift for one byte		*/
#endif	/* BBN */

/************************************************************************
 *
 * Define routines as macros for certain machine architectures
 */

#ifndef VMS
#if mc68020 || mc68k32 || u370 || UTS
#define ldint(p)	(*(short *)(p))
#define stint(i,p)	(*(short *)(p)=(i))
#define ldlong(p)	(*(long *)(p))
#define stlong(l,p)	(*(long *)(p)=(l))
#endif

#ifndef SINIX
#if mc68020 || mc68k32 || u370 || vax || UTS
#define ldfloat(p)	(*(float *)(p))
#define stfloat(f,p)	(*(float *)(p)=(f))
#define lddbl(p)	(*(double *)(p))
#define stdbl(d,p)	(*(double *)(p)=(d))
#endif
#else /* SINIX */
#define ldfloat(p)	(*((float *) (p)))
#define stfloat(f,p)	((*((float *) (p))) = ((float) (f)))
#define lddbl(p)	(*((double *) (p)))
#define stdbl(d,p)	((*((double *) (p))) = ((double) (d)))
#endif /* SINIX */
#endif /* VMS */

#ifdef VMS
#define ldfloat(p)	(*(float *)(p))
#define stfloat(f,p)	(*(float *)(p)=(f))
#define lddbl(p)	(*(double *)(p))
#define stdbl(d,p)	(*(double *)(p)=(d))
#endif /* VMS */

#ifdef	PCIX
#define ldint	ldshort
#define stint 	stshort
#define locking	lockf
#endif	/* PCIX */


#define MAXINT		2147483647L	/* maximum integer size */
#define MAXSMINT	32767		/* maximum smallint size */
#define INTNULL		0x80000000
#define SMINTNULL	-32768
#define CHARNULL	0


#ifdef NOSTRUCTASG
#ifndef CASTCP	/* SINIX: if rdsmac.h is not #included. TP 18.08.88 */
#define CASTCP  (char *)
#endif /* CASTCP */
#define structasg(from, to)		{ bycopy(CASTCP from, CASTCP to,\
					    sizeof(*to)); }
#else
#define structasg(from, to)		{ *to = *from; }
#endif /* NOSTRUCTASG */

#ifdef SINIX
/* macros beginning with 'r' as functions in ../rds/rbytstr.c */
#define rstrlen(s)		stleng(s)
#define rstrcpy(f,t)		stcopy(f,t)
#define rstrcat(s1,s2)		stcat(s2,s1)
#define rbytefill(d,n,c)	byfill(d,n,c)
#define rbytecpy(f,t,n)		bycopy(f,t,n)
#define rbytecmp(b1,b2,n)	bycmpr(b1,b2,n)
#define rstrcmp(s1,s2,rel)	(*(rel) = stcmpr(s1,s2))
#define rcmpbyte(b1,b2,n,rel)	(*(rel) = bycmpr(b1,b2,n))

#define stleng(s)		strlen(s)
#define stcopy(f,t)		strcpy(t,f)
#define stcat(s1,s2)		strcat(s2,s1)
#define stcmpr(s1,s2)		strcmp(s1,s2)
#endif /* SINIX */

#ifdef VMS
#define unlink delete
#endif /* VMS */

/*	Machine Floating Point Type
 *      This is only used by network versions. It should identify the
 *	floating point representation on this machine.  Machines
 *	with the same representation (including byte order) should
 *	have the same value defined for FLTTYPE.  Machines with
 *	different representation MUST have different values for
 *      FLTTYPE or be defined as "".  There are a few standard 
 *      types.
 *
 *      IEEEM  - IEEE Motorola byte order 
 *      IEEEI  - IEEE Intel byte order
 */
#ifdef sun
#ifdef mc68000
#define FLTTYPE "IEEEM"
#endif /* mc68000 */
#endif /* sun */

#ifdef APOLLO
#define FLTTYPE "APOLLO"
#endif /* APOLLO */

#ifndef FLTTYPE
#define FLTTYPE ""
#endif

#endif /* _MACHINE_H */
