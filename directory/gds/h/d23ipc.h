/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d23ipc.h,v $
 * Revision 1.1.753.2  1996/02/18  23:35:38  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:25:44  marty]
 *
 * Revision 1.1.753.1  1995/12/08  16:00:12  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:13:39  root]
 * 
 * Revision 1.1.751.4  1994/06/21  14:47:08  marrek
 * 	June 1994 code submission.
 * 	[1994/06/17  13:00:25  marrek]
 * 
 * Revision 1.1.751.3  1994/05/10  15:55:47  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:37:41  marrek]
 * 
 * Revision 1.1.751.2  1994/03/23  15:18:22  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:29:34  keutel]
 * 
 * Revision 1.1.751.1  1994/02/22  18:49:09  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:44:21  marrek]
 * 
 * Revision 1.1.749.3  1993/10/29  15:11:35  keutel
 * 	CR 9261
 * 	[1993/10/29  14:08:36  keutel]
 * 
 * Revision 1.1.749.2  1993/10/14  17:27:45  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:26:37  keutel]
 * 
 * Revision 1.1.749.1  1993/10/13  17:32:32  keutel
 * 	October 1993 code drop
 * 	[1993/10/09  18:26:22  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  12:35:53  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:39:17  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  20:40:15  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:29:19  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/02  04:01:13  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:41:46  zeliff]
 * 
 * $EndLog$
 */
#ifndef _D23IPC_H
#define _D23IPC_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char d23ipc_rcsid[] = "@(#)$RCSfile: d23ipc.h,v $ $Revision: 1.1.753.2 $ $Date: 1996/02/18 23:35:38 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*****************************************************************************/
/*                                                                           */
/*                     IPC - H E A D E R - F I L E                           */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                        DATE: 17.5.91     */
/*****************************************************************************/

#include <sys/types.h>
#include <gds.h>

/* --------------- IPC - interface (common constants) --------------------- */
					
					/* IPC-interface version # */
#define		D23_MIN		(233)	/* IPC-minimum version */
#define		D23_V02		(233)	/* IPC-version-number */
#define		D23_MAX		(233)	/* IPC-maximum version */

					/* IPC-interface function return values */
#define		D23_SUCES	0       /* IPC-function: okay */
#define		D23_ERROR	-1      /* IPC-function: failed */

					/* server-/registration-ID's */
#define		D23_DCASID 	1	/* server-ID: DUA-Cache */	
#define		D23_CSTSID 	2	/* server-ID: C-Stub */	
#define		D23_MONPROC	5	/* registration-ID: IPC-Monitor */
#define		D23_SSTSID	10	/* server-ID: basic S-Stub server-ID */
					/* D23_SSTSID + dir-ID = server-ID */
					/* of S-Stub */
#define         D23_DS0SID      (D23_SSTSID + D2MAXDIR_ID)
					/* server-ID: basic DSA server-ID */
					/* D23_DS0ID + dir-ID = server-ID */
					/* of DSA */
					/* max. server-ID */
#define         D23_MAXSVR     (D23_DS0SID+D2MAXDIR_ID)


/* --------------- IPC - interface function parameter blocks -------------- */

					/* Global Invoke-ID information block */
typedef struct {
	signed32	d23_invid ;	/* local Invoke-ID */
	signed32	d23_usrid ;	/* local user-ID */
	unsigned16	d23_contid ;	/* context-ID */
} D23_invinfo ;

typedef struct {			/* d23_attsvr()-parameter block */
	D2_pbhead	d23_0hdinfo ;	/* <-> Standard Header */
	signed16	d23_0svrid ;	/* --> Server ID */
	signed16	d23_0evmode ;	/* --> event mode */
	signed16	d23_0mslen ;	/* <-- Memorysegment Length */
} D23_atsvpb ;

					/* event modes */
#define D23_SYNC_EM		1	/* enable synchronous event */
					/* handling */
#define D23_PASYNC_EM		2	/* enable partially asynchronous */
					/* event handling */
#define D23_FASYNC_EM		3	/* enable fully asynchronous */
					/* event handling */

					/* set/get event mode macros */
#define D23_SEVM(x)		(x)
#define D23_GEVM(x)		((x) & 0x03)

					/* set/get server type macros */
#define D23_SSTY(x)		((x) << 2)
#define D23_GSTY(x)		(((x) >> 2) & 0x03)


typedef struct {			/* d23_detsvr()-parameter block */
	D2_pbhead    d23_1hdinfo ;	/* <-> Standard Header */
} D23_desvpb ;


typedef struct {			/* d23_waitin()-parameter block */
	D2_pbhead	d23_2hdinfo ;	/* <-> Standard Header */
	signed16	d23_2wtime ;	/* --> Wait-Time */
	signed16	d23_2assid ;	/* <-- Association-ID */
	D23_invinfo	d23_2invinfo ;	/* <-- Invoke info */
	signed16	d23_2oprid ;	/* <-- Operation-ID */
	signed16	d23_2imdat ;	/* <-- More Invoke-Data */
	byte		*d23_2rfidt ;	/* <-- Invoke-Data Reference */
	signed16	d23_2lnidt ;	/* <-- Invoke-Data Length */
} D23_wtinpb ;

#define		D23_CHECK 	1	/* Wait-Time: no wait (check only) */	
#define		D23_INFIN  	2	/* Wait-Time: infinite */	


typedef struct {			/* d23_result()-parameter block */
	D2_pbhead	d23_3hdinfo ;	/* <-> Standard Header */
	signed16	d23_3assid ;	/* --> Association-ID */
	D23_invinfo	d23_3invinfo ;	/* --> Invoke info */
	signed16	d23_3rltid ;	/* --> Result-Type */
	signed16	d23_3rmdat ;	/* --> More Result-Data */
	byte		*d23_3rfrdt ;	/* --> Result-Data Reference */
	signed16	d23_3lnrdt ;	/* --> Result-Data Length */
} D23_rsrqpb ;

					/* result type values */
#define D23_RESULT		254	/* return result */
#define D23_RERROR		100	/* basic return error type value */
					/* return error type = D23_RERROR + */
					/* error value */
#define D23_MXRERR		49	/* max. error value */
#define D23_RREJECT		150	/* basic return reject type value */
					/* return reject type = D23_RREJECT + */
					/* reject value */
#define D23_MXRREJ		49	/* max. reject value */
#define D23_ABORT		255	/* abort */

typedef struct {			/* d23_bind()-parameter block */
	D2_pbhead	d23_4hdinfo ;	/* <-> Standard Header */
	signed16	d23_4svrid ;	/* --> Server-ID */
	signed16	d23_4assid ;	/* <-- Association-ID */
	byte		*d23_4refms ;	/* <-- IPC-buffer Reference */
	signed16	d23_4lenms ;	/* <-- IPC-buffer Length */
} D23_bindpb ;


typedef struct {			/* d23_unbind()-parameter block */
	D2_pbhead	d23_5hdinfo ;	/* <-> Standard Header */
	signed16	d23_5assid ;	/* --> Association-ID */
} D23_ubndpb ;


typedef struct {			/* d23_invoke()-parameter block */
	D2_pbhead	d23_6hdinfo ;	/* <-> Standard Header */
	signed16	d23_6assid ;	/* --> Association-ID */
	D23_invinfo	d23_6invinfo ;	/* <-> Invoke info */
	signed16	d23_6oprid ;	/* --> Operation-ID */
	signed16	d23_6imdat ;	/* --> More Invoke-Data */
	byte		*d23_6rfidt ;	/* --> Invoke-Data Reference */
	signed16	d23_6lnidt ;	/* --> Invoke-Data Length */
} D23_inrqpb ;

					/* set/get application context macros */
#define D23_SAC(x)		(x)
#define D23_GAC(x)		((x) & 0x03)

					/* set/get abstract syntax macros */
#define D23_SAS(x)		((x) << 2)
#define D23_GAS(x)		(((x) >> 2) & 0x03)

					/* set/get/change transfer syntax macros */
#define D23_STS(x)		((x) << 4)
#define D23_GTS(x)		(((x) >> 4) & 0x03)
#define D23_CTS(x,y)		x = (((x) & (~(0x03 << 4))) | D23_STS(y))

					/* set/get access mode macros */
#define D23_SACM(x)		((x) << 6)
#define D23_GACM(x)		(((x) >> 6) & 0x03)

					/* set/get remote-ID macros */
#define D23_SRMID(x)		((x) << 8)
#define D23_GRMID(x)		(((x) >> 8) & 0x3F)

					/* operation-ID values */
					/* see also header file 'd23apdu.h' */
#define D23_NOOPR		0	/* no operation */
#define D23_MXUOPID		99	/* max. user specific operation-ID */

					/* last/more invoke data values */
#define		D23_LASTFG 	1	/* last data fragment */	
#define		D23_MRDATA 	2	/* more data fragment */	


typedef struct {			/* d23_waitrs()-parameter block */
	D2_pbhead	d23_7hdinfo ;	/* <-> Standard Header */
	signed16	d23_7wtime ;	/* --> Wait-Time */
	signed16	d23_7assid ;	/* <-- Association-ID */
	D23_invinfo	d23_7invinfo ;	/* <-- Invoke info */
	signed16	d23_7rltid ;	/* <-- Result Type */
	signed16	d23_7rmdat ;	/* <-- More Result-Data */
	byte		*d23_7rfrdt ;	/* <-- Result-Data Reference */
	signed16	d23_7lnrdt ;	/* <-- Result-Data Length */
} D23_wtrspb ;


typedef struct {			/* d23_attclt()-parameter block */
	D2_pbhead	d23_9hdinfo ;	/* <-> Standard Header */
	signed16	d23_9evmode ;	/* --> event mode */
} D23_atclpb ;


typedef struct {			/* d23_detclt()-parameter block */
	D2_pbhead	d23_Ahdinfo ;	/* <-> Standard Header */
} D23_declpb ;


typedef struct {			/* d23_event()-parameter block */
	D2_pbhead	d23_Bhdinfo ;	/* <-> Standard Header */
	signed16	d23_Bevtype ;	/* <-- Event Type */
	signed16	d23_Bassid ;	/* <-- Association-ID */
	D23_invinfo	d23_Binvinfo ;	/* <-- Invoke info */
	signed16	d23_Bopid ;	/* <-- Operation-ID */
	signed16	d23_Bmdat ;	/* <-- More data/end of data */
	signed16	d23_Bln ;	/* <-- Data length */
	byte		*d23_Bref ;	/* <-- Data reference */
} D23_evntpb ;
					/* IPC-event types */
#define D23_NOEVNT		0	/* no event */
#define D23_INVEVNT		1	/* invoke indication event */
#define D23_INVCEVNT		2	/* invoke (continuation) ind. event */
#define D23_RESEVNT		3	/* result indication event */
#define D23_RESCEVNT		4	/* result (continuation) ind. event */
#define D23_RRQBFEVNT		5	/* return request IPC-buffer event */
#define D23_RRSBFEVNT		6	/* return result IPC-buffer event */
#define D23_ABOEVNT		7	/* abort indication event */


typedef struct {			/* d23_retbuf()-parameter block */
	D2_pbhead	d23_Chdinfo ;	/* <-> Standard Header */
	signed16	d23_Cassid ;	/* --> Association-ID */
} D23_rbufpb ;


typedef struct {			/* d23_abort()-parameter block */
	D2_pbhead	d23_Fhdinfo ;	/* <-> Standard Header */
	signed16	d23_Fassid ;	/* --> Association-ID */
	D23_invinfo	d23_Finvinfo ;	/* --> Invoke info */
} D23_abortpb ;


typedef struct {			/* d23_abandon()-parameter block */
	D2_pbhead	d23_Ghdinfo ;	/* <-> Standard Header */
	D23_invinfo	d23_Ginvinfo ;	/* --> Invoke info */
} D23_abandonpb ;

typedef struct {			/* d23_dis_abandon()-parameter block */
	D2_pbhead	d23_Hhdinfo ;	/* <-> Standard Header */
	signed16	d23_Hassid ;	/* --> Association-ID */
} D23_disabandonpb ;

typedef struct {			/* d23_gipc_eminfo()-parameter block */
	D2_pbhead	d23_Ehdinfo ;	/* <-> Standard Header */
	signed16	d23_Enfds ;	/* <-- no. of file descriptors */
	int		*d23_Eipcfds ;	/* <-- IPC-event specific fd(s) */
} D23_ipcempb ;


typedef struct {			/* d23_getsvrinfo()-parameter block */
	D2_pbhead	d23_Dhdinfo ;	/* <-> Standard Header */
	signed16	d23_Dsvrid ;	/* --> Server-ID */
	signed16	d23_Dunloadf ;	/* --> process unload factor */
	signed16	d23_Dnototpr ;	/* <-- total no. of processes */
	signed16	d23_Dnobsypr ;	/* <-- no. of processes being busy */
} D23_gtsvpb ;

typedef struct {			/* d23_reg_process()-parameter block */
	D2_pbhead	d23_Ihdinfo ;	/* <-> Standard Header */
	signed32	d23_Iptype ;	/* --> process type */
	signed32	d23_Idirid ;	/* --> directory-ID */
	boolean		d23_Iotype ;	/* --> operation type */
} D23_regprocpb ;

typedef struct {			/* d23_write_dist_cmd()-parameter block */
	D2_pbhead	d23_Jhdinfo ;	/* <-> Standard Header */
	signed32	d23_Jcmd ;	/* --> distributed command */
	signed32	d23_Jsv_prid ;	/* --> destination server/process-ID */
	signed32	d23_Jtmout ;	/* --> timeout value (in seconds) */
	signed32	d23_Jparsize ;	/* --> size of parameter data */
	byte		*d23_Jparref ;	/* --> parameter data reference */
} D23_dwritepb ;

typedef struct {			/* d23_read_dist_cmd()-parameter block */
	D2_pbhead	d23_Khdinfo ;	/* <-> Standard Header */
	signed32	d23_Kcmd ;	/* <-- distributed command */
	signed32	d23_Kkey ;	/* <-- dist. command key */
	signed32	d23_Kparsize ;	/* <-- size of parameter data */
	byte		*d23_Kparref ;	/* <-- parameter data reference */
} D23_dreadpb ;

typedef struct {			/* d23_conf_dist_cmd()-parameter block */
	D2_pbhead	d23_Lhdinfo ;	/* <-> Standard Header */
	signed32	d23_Lkey ;	/* --> dist. command key */
} D23_dconfpb ;

typedef struct {			/* d23_add_usr_cred()-parameter block */
	D2_pbhead	d23_Mhdinfo ;	/* <-> Standard Header */
	unsigned16	d23_Mcontid ;	/* --> context-ID */
	signed32	d23_Musrid ;	/* --> user-ID */
	signed32	d23_Mcredsize ;	/* --> size of credential data */
	byte		*d23_Mcredref ;	/* --> credential data reference */
} D23_addcrdpb ;

typedef struct {			/* d23_rem_usr_cred()-parameter block */
	D2_pbhead	d23_Nhdinfo ;	/* <-> Standard Header */
	unsigned16	d23_Ncontid ;	/* --> distributed command */
	signed32	d23_Nusrid ;	/* --> dist. command key */
} D23_remcrdpb ;

typedef struct {			/* d23_read_usr_cred()-parameter block */
	D2_pbhead	d23_Ohdinfo ;	/* <-> Standard Header */
	unsigned16	d23_Ocontid ;	/* --> context-ID */
	signed32	d23_Ousrid ;	/* --> user-ID */
	signed32	d23_Ocredsize ;	/* <-- size of credential data */
	byte		*d23_Ocredref ;	/* --> credential data reference */
} D23_readcrdpb ;

typedef struct {			/* d23_set_pstate()-parameter block */
	D2_pbhead	d23_Phdinfo ;	/* <-> Standard Header */
	signed16	d23_Passid ;	/* --> assiciation-ID */
	char		*d23_Ppstate ;	/* --> process state */
} D23_pstatpb ;

/* --------------- IPC - error values ------------------------------------- */

	/* Note: If a system error (S) occurs, the system error value is */
	/* combined with the specific IPC-error value (e.g. the detecting of */
	/* a system error ENOENT during the attachment to the IPC-resources */
	/* results in a returned error value of (D23_NIPCERR + ENOENT)) */

#define	D23_NOSERV	1000		/* no server process available */
#define	D23_LIMASS	1100		/* IPC-association limit exceeded */
#define	D23_LIMSVR	1200		/* server-limit exceeded */
#define	D23_SIGINT	1300		/* function interrupted by signal */
#define	D23_NOINST	1400		/* no DIRINFBAS environment variable */
#define D23_PENTERR	1500		/* no process registration entry */
#define	D23_IFFMND	1600		/* (S) can't create named FIFO */
#define	D23_IFFOPN	1700		/* (S) can't open name FIFO */
#define	D23_IFFWRT	1800		/* (S) can't write into named FIFO */
#define	D23_IFFRED	1900		/* (S) can't read from name FIFO */
#define	D23_IFFCLS	2000		/* (S) can't close named FIFO */
#define D23_LOSEMA	2100		/* (S) can't lock semaphore */
#define D23_ULSEMA	2200		/* (S) can't unlock semaphore */
#define D23_SIGERR	2300		/* (S) signal handler problem */

#define	D23_VERSIL	3000		/* invalid IPC-version # */
#define	D23_SVIDIL	3100		/* invalid server-ID */
#define	D23_ASIDIL	3200		/* invalid IPC-association-ID */
#define	D23_WTYPIL	3300		/* invalid wait time parameter */
#define	D23_MTYPIL	3400		/* invalid data-fragment parameter */
#define	D23_MREFIL	3500		/* invalid IPC-buffer reference parameter */
#define	D23_ILNERR	3600		/* invalid length of invoke-PDU */
#define	D23_RLNERR	3700		/* invalid length of result-PDU */
#define D23_EVMOIL	3800		/* invalid event mode parameter */
#define D23_RSTYIL	3900		/* invalid result type parameter */
#define D23_UNLDIL	4000		/* invalid unload factor parameter */
#define D23_SVMOIL	4100		/* invalid server type parameter */
#define D23_PPARERR	4200		/* process parameter error */
#define D23_IMLMERR	4300		/* implementation limit exceeded */
#define D23_CTXTIL	4400		/* invalid context-ID parameter */
#define D23_USIDIL	4500		/* invalid user-ID parameter */
#define D23_INVERR	4600		/* invalid invoke info parameter */

#define	D23_ATSVIL	5000		/* illegal d23_attsvr()-call */ 
#define	D23_DESVIL	5100		/* illegal d23_detsvr()-call */ 
#define	D23_WTINIL	5200		/* illegal d23_waitin()-call */ 
#define	D23_RSRQIL	5300		/* illegal d23_result()-call */ 
#define	D23_ATCLIL	5400		/* illegal d23_attclt()-call */ 
#define	D23_DECLIL	5500		/* illegal d23_detclt-call */ 
#define	D23_BINDIL	5600		/* illegal d23_bind()-call */
#define	D23_UBNDIL	5700		/* illegal d23_unbind()-call */ 
#define	D23_INRQIL	5800		/* illegal d23_invoke()-call */ 
#define	D23_WTRSIL	5900		/* illegal d23_waitrs()-call */ 
#define D23_EVNTIL	6000		/* illegal d23_event()-call */
#define D23_RBUFIL	6100		/* illegal d23_retbuf()-call */
#define D23_GTEMIL	6200		/* illegal d23_gipc_eminfo()-call */
#define D23_GTSVIL	6300		/* illegal d23_getsvrinfo()-call */
#define D23_DCWRIL	6400		/* illegal d23_write_dist_cmd()-call */
#define D23_DCRDIL	6500		/* illegal d23_read_dist_cmd()-call */
#define D23_DCCFIL	6600		/* illegal d23_conf_dist_cmd()-call */
#define D23_UCADIL	6700		/* illegal d23_add_usr_cred()-call */
#define D23_UCRDIL	6800		/* illegal d23_read_usr_cred()-call */
#define D23_UCRMIL	6900		/* illegal d23_rem_usr_cred()-call */
#define D23_ABOIL	7000		/* illegal d23_abort()-call */
#define D23_ABNDIL	7100		/* illegal d23_abandon()-call */
#define D23_SPSTIL	7200		/* illegal d23_set_pstate()-call */

#define D23_NOHPMEM	7300		/* can't allocate heap memory */
#define D23_HPMEM	7400		/* can't release heap memory */
#define D23_GIPCERR	7500		/* (S) can't get IPC-resources */
#define D23_RIPCERR	7600		/* (S) can't release IPC-resources */
#define D23_NIPCERR	7700		/* (S) no IPC-resources available */
#define D23_SMSGERR	7800		/* (S) can't send IPC-message */
#define D23_RMSGERR	7900		/* (S) can't receive IPC-message */
#define D23_ABOERR	8000		/* IPC-association aborted */
#define D23_INVIL	8100		/* invalid invoke-ID */
#define D23_OPIDIL	8200		/* invalid operation-ID */
#define D23_PROTERR	8300		/* IPC-protocol error */
#define D23_NSIZEIL	8400		/* 'DN size'-error */
#define D23_NOCRED	8500		/* 'no credential entry found'-error */
#define D23_TMOUT	8600		/* 'dist. command time out'-error */
#define D23_DBUSY	8700		/* 'dist. command busy'-error */
#define D23_DPARERR	8800		/* 'dist. command parameter'-error */
#define D23_NODCMD	8900		/* 'no dist. command'-error */
#define D23_DCONFILL	9000		/* 'illegal dist. command call'-error */
#define D23_ABANDONED	9100		/* 'abandoned'-error */
#define D23_ILLINVID	9200		/* 'illegal invode-ID'-error */
#define D23_TOOLATE	9300		/* 'too late'-error */
#define D23_CNTABND	9400		/* 'cannot abandon'-error */


/* ********* IPC - internally used constants resp. data structures ******** */

/* -------------- IPC - common constants ---------------------------------- */

					/* directory system ipc-key file */
#define D23_IPC_KFILE   "%s%s/gdsipcinit"
					/* DIR-X specific IPC-key file */
#define D23_FIFO_FILE   "%s%s/ipc/event.%c%d"

#define D23_FIFO_FILE1	"%s/ipc/event.%c%d"

#define D23_IPC_KEY	 'D'		/* DIR-X specific IPC-key */
#define D23_ACCMODE	0666		/* access mode to IPC-resources */
#define D23_MXSEMA	   1		/* max. # of semaphores in the */
					/* used semaphore set */
#define D23_SHMSEMNO	   0		/* # of common IPC-semaphore */

#define D23_UNUSED	  -1		/* unused flag */
#define D23_LINE	__LINE__

#define D23_MCHKTIME	3600		/* max. time interval (in sec) for */
					/* monitoring IPC-resources */
#define D23_DEFCHKTIME    60		/* default time interval (in sec) */


/* --------- IPC - internally used real/virtual process-ID format --------- */

typedef struct {
	pid_t		d23_rpid ;	/* real process-ID */
	signed16	d23_vpid ;	/* virtual process-ID */
} D23_rvpid ;

#define D23_MXVPID	D23_GDSTID(~0L)	/* max. virtual process-ID */


/* -------------- IPC - Shared memory general area format ------------------ */

#define D23_SHMVERS	    8		/* version # of shared memory layout */
#define D23_MXDISTCMD	   64		/* max. # of dist. command entries */
#define D23_MXREGINFO	  256		/* max. # of registration entries */
#define D23_MXSRVPROC  	  512		/* max. # of server entries */
#define D23_MXCLTPROC     512		/* max. # of client entries */
#define D23_MXSIZECBUF     16		/* max. communication buffer size(kB) */
#define D23_MXCRED	  512		/* max. # of user credential entries */
#define D23_MXHPBUF	 2048		/* max. # of buffers in the heap */
#define D23_MDEFREL	   16		/* relation max./default-values */

					/* shift operation delimiter */
					/* for heap buffer state */
#define D23_HPMSKLM	((unsigned32) 1 << (D23_FLDSIZE - 1))
					/* basic bit field width */
#define D23_FLDSIZE	(sizeof(signed32)*8)

typedef struct {
	signed32	d23_versno ;	/* version # (associated to this */
					/* shared memory layout) */
	signed32	d23_creatime ;	/* shared memory creation time stamp */
	signed16	d23_lvpid ;	/* last virtual process-ID used */
	signed32	d23_linvid ;	/* last invoke-ID used */
	signed32	d23_gltstamp ;	/* global dist. command time stamp */
	signed16	d23_mdno ;	/* max. # of dist. command entries */
	signed16	d23_dno ;	/* # of occupied dist. command entries*/
	signed32	d23_darea ;	/* offset to dist. command area */
	signed16	d23_mrno ;	/* max. # of registration entries */
	signed16	d23_rno ;	/* # of occupied registration entries */
	signed32	d23_rarea ;	/* offset to registration area */
	signed16	d23_msno ;	/* max. # of server entries */
	signed16	d23_sno ;	/* # of occupied server entries */	
	signed32	d23_sarea ;	/* offset to server area */
	signed16	d23_mcno ;	/* max. # of client entries */
	signed16	d23_cno ;	/* # of occupied client entries */
	signed32	d23_carea ;	/* offset to client area */
	signed16	d23_bno ;	/* # of communication buffers */
	signed16	d23_bsize ;	/* communication buffer size */
	signed32	d23_barea ;	/* offset to communication buffer area*/
	signed16	d23_mcred ;	/* max. # of user credential entries */
	signed16	d23_cred ;	/* # of occupied user cred. entries */
	signed32	d23_crarea ;	/* offset to user credential area */
	signed16	d23_mhpno ;	/* max. # of buffers within the heap */
					/* state of the heap buffer */
	unsigned32	d23_hpstate[D23_MXHPBUF / D23_FLDSIZE] ;
	signed32	d23_hparea ;	/* offset to heap area */
	signed16	d23_mext ;	/* # of ext. areas description area */
	signed32	d23_exarea ;	/* offset to ext. area descript. area */
} D23_ipcinfo ;


/* -------------- IPC - Shared memory distributed command entry format ---- */

					/* distributed IPC-command codes */
#define D23_CLLFILE	   1		/* close delta update log-file */
#define D23_OPLFILE	   2		/* reopen delta update log-file */
#define D23_DISDBOP	   3		/* disable data base operations */
#define D23_ENADBOP	   4		/* enable data base operations */
#define D23_RESTART	   5		/* restart process */
#define D23_DISLOG	   6		/* disable logging */
#define D23_ENALOG	   7		/* enable logging */
#define D23_CLDBFILES      8            /* close directory database files */
#define D23_DISPFMLOG      9            /* disable performance logging    */
#define D23_ENAPFMLOG     10            /* enable performance logging     */
#define D23_DCABANDON	  11		/* abandon request */
#define D23_DISDBMOD      12            /* disable database modifications */
#define D23_SVCLOGLEVEL   13            /* reset serviceability log levels*/
#define D23_DISSHLOG	  14		/* disable shadow update logging */
#define D23_ENASHLOG	  15		/* enable shadow update logging */

#define D23_DMINTOUT	   1		/* min. time out interval of a */
					/* distributed IPC-command */
#define D23_DMAXTOUT     600            /* max. time out interval of a */
					/* distributed IPC-command */
#define D23_DPARSIZE	 512		/* max. size of a parameter block */
					/* of a distributed IPC-command */

typedef struct {
	signed32	d23_dcmd ;		/* distributed IPC-command code */
	pid_t		d23_dcpid ;		/* real process-ID of initiator */
	pid_t		d23_dsv_prid ;		/* server-/process-ID of the recipient(s) */
	signed32	d23_dtstamp ;		/* command time stamp (for internal use) */
	signed32	d23_disvrno ;		/* # of informed recipients */
	signed32	d23_dconfno ;		/* # of received confirmations */
	signed32	d23_dtout ;		/* timeout value */
	signed32	d23_dparsize ;		/* size of parameter data */
	signed32	d23_dpar ;		/* offset to parameter block */
} D23_distcmd ;


/* -------------- IPC - Shared memory registration entry format ----------- */

					/* user controlled process state values */
#define D23_NOSTATE	"-"		/* process state (default value) */
#define D23_WSTATE	'W'		/* process waiting state (W1-W99) */
#define D23_RSTATE	'R'		/* process running state (R1-R99) */

typedef struct {
	signed16	d23_pregtype ;	/* registration-ID of process */
	signed16	d23_pdirid ;	/* directory-ID to which the process */
					/* belongs (= 0, if irrelevant) */
	signed32	d23_procid ;	/* real process-ID */
	char		d23_pstate[4] ;	/* state of the process */
} D23_reginfo ;


/* -------------- IPC - Shared memory client-/server entry format --------- */

  					/* entry states */
#define D23_NOCLT		0	/* client entry is free */
#define D23_NOSVR	D23_NOCLT	/* server entry is free */
#define D23_ATTACH		1	/* client-/server entry is occupied */

					/* set/get entry state macros */
#define D23_SES(x)		(x)
#define D23_GES(x)		((x) & 0x03)

					/* operational states */
#define D23_ASSOC		1	/* IPC-association */
#define D23_RBINVPEND		2	/* invoke pending + */
					/* IPC-buffer pending */
#define D23_INVPEND		3	/* invoke pending */
#define D23_REQPEND		4	/* request pending */
#define D23_RBRESPEND		5	/* result pending + */
					/* IPC-buffer pending */
#define D23_RESPEND		6	/* result pending */

					/* set/get/change operational state macros */
#define D23_SEOS(x)		((x) << 2)
#define D23_GEOS(x)		(((x) >> 2) & 0x0F)
#define D23_CEOS(x,y)		x = (((x) & (~(0x0F << 2))) | D23_SEOS(y))

					/* event modes */
					/* see function d23_attsvr() */
					/* set/get own event mode macros */
#define D23_SOEVM(x)		((x) << 6)
#define D23_GOEVM(x)		(((x) >> 6) & 0x03)

					/* set/get/change partner event mode macros */
#define D23_SPEVM(x)		((x) << 8)
#define D23_GPEVM(x)		(((x) >> 8) & 0x03)
#define D23_CPEVM(x,y)		x = (((x) & (~(0x03 << 8))) | D23_SPEVM(y))

					/* entry types (sever entries only) */
#define D23_PRIMARY		1	/* primary server entry */
#define D23_SECONDARY		2	/* secondary server entry */

					/* set/get entry type macros */
#define D23_SSET(x)		((x) << 10)
#define D23_GSET(x)		(((x) >> 10) & 0x03)

					/* server types (server entries only) */
#define D23_SPROCSVR		1	/* single process server */
#define D23_MPROCSVR		2	/* multiple process server */

					/* set/get server type macros */
#define D23_SSEST(x)		((x) << 12)
#define D23_GSEST(x)		(((x) >> 12) & 0x03)

					/* unload request values (server */
					/* entries only) */
#define D23_UNLDRQ		1	/* unload requested */

					/* set/get unload request macros */
#define D23_SSEUR(x)		((x) << 14)
#define D23_GSEUR(x)		(((x) >> 14) & 0x03)

					/* abandon request */
#define D23_ABNDRQ		1

					/* set/get/clear abandon request macros */
#define D23_SABND(x)		((x) << 16)
#define D23_GABND(x)		(((x) >> 16) & 0x01)
#define D23_CABND(x)		x &= (~(D23_ABNDRQ << 16))

					/* disable abandon request */
#define D23_DABNDRQ		1

					/* set/get/clear abandon request macros */
#define D23_SDABND(x)		((x) << 17)
#define D23_GDABND(x)		(((x) >> 17) & 0x01)
#define D23_CDABND(x)		x &= (~(D23_ABNDRQ << 17))

typedef struct {
	unsigned32	d23_estate ;	/* entry state information */
	char		d23_ucstate[4] ;/* user controlled state information */
	D23_rvpid	d23_epid ;	/* own client-/server real/virtual */
					/* process-ID */
	signed16	d23_esid ;	/* server-ID */
	signed16	d23_cassid ;	/* association-ID of corres- */
					/* ponding server-/client entry */
	signed16	d23_oprid ;	/* operation-ID in progress */
	D23_invinfo	d23_invinfo ;	/* invoke info of operation in progress */
	D23_rvpid	d23_cpid ;	/* real/virtual process-ID of corre- */
					/* sponding server-/client process */
} D23_ipcentry ;


/* -------------- IPC - Shared memory user credential entry format -------- */

typedef struct {
	unsigned16	d23_credcontid ;/* context-ID */
	signed32	d23_credusrid ;	/* user-ID */
	signed32	d23_credsvrid ;	/* server-ID */
	signed32	d23_credoff ;	/* offset to user credentials */
					/* within shared memory area */
	signed32	d23_credlen ;	/* length of user credential */
} D23_usrcred ;


/* -------------- IPC-Shared memory heap entry format --------------------- */

#define D23_HPBFSZ	  64		/* heap buffer size */

typedef struct {
	signed16	d23_hpbufno ;		/* # of continuous buffers */
	byte		d23_hpbuf[D23_HPBFSZ] ;	/* buffer area */
} D23_hpbufinfo ;

/* ------ IPC-Shared extension area(s) description area entry format ------ */

#define D23_INITIALIZED	1		/* ext. area is already initialized */

typedef struct {
	unsigned32	d23_extstate ;	/* state of extension area */
	signed32	d23_extid ;	/* identifier of extension area */
	signed32	d23_mextno ;	/* max. # of entries in ext. area */
	signed32	d23_aextno ;	/* # of occupied ext. area entries */
	signed32	d23_extsize ;	/* size of extension area */
	signed32	d23_extoff ;	/* offset to extension area */
	signed32	d23_private ;	/* reserved for private use */
} D23_extinfo ;

/* -------------- IPC - message format ------------------------------------ */

					/* IPC-message types */
#define D23_INVMSG	D23_INVEVNT	/* invoke message */
#define D23_INVCMSG	D23_INVCEVNT	/* continuation message */
#define D23_RESMSG	D23_RESEVNT	/* result message */	
#define D23_RESCMSG	D23_RESCEVNT	/* result continuation message */	
#define D23_RRQBFMSG	D23_RRQBFEVNT	/* return request buffer message */
#define D23_RRSBFMSG	D23_RRSBFEVNT	/* return result buffer message */
#define D23_MXEVNT		6	/* max. # of IPC-message types */		
					/* IPC-message type specific macros */
					/* set/get IPC-destination-ID macros */
#define D23_SDSTID(x)		(x)
#define D23_GDSTID(x)		((x) & 0xFFFF)

  					/* set/get IPC-message type macros */
#define D23_SITYPE(x)		((x) << 16)
#define D23_GITYPE(x)		(((x) >> 16) & 0x07)

					/* set/get calling/called IPC- */
					/* association-ID macros */
#define D23_SCALASS(x)		(x)
#define D23_GCALASS(x)		((x) & 0xFFFF)
#define D23_SCLDASS(x)		((x) << 16)
#define D23_GCLDASS(x)		(((x) >> 16) & 0xFFFF)

					/* set/get operation-ID/result type */
					/* macros */
#define D23_SIOI(x)		(x)
#define D23_GIOI(x)		((x) & 0x3FF)

  					/* operation flag values */
#define D23_END			1	/* end of data flag */
#define D23_MORE		2	/* more data flag */

					/* set/get operation flag macros */
#define D23_SIOF(x)		((x) << 10)
#define D23_GIOF(x)		(((x) >> 10) & 0x03)

					/* set/get event mode macros */
#define D23_SIOEVM(x)		((x) << 12)
#define D23_GIOEVM(x)		(((x) >> 12) & 0x03)

					/* size of an IPC-message */
#define D23_MSGBUFSZ	(sizeof (D23_ipcmsg) - sizeof (signed32))

typedef struct {
	signed32	d23_mtype ;	/* message type */
	D23_rvpid	d23_callpid ;	/* real/virtual process-ID of */
					/* calling process */
	unsigned32	d23_iassid ;	/* IPC-association-ID */
	D23_invinfo	d23_invinfo ;	/* invoke info */
	signed16	d23_msgsz ;	/* size of user message */
	unsigned16	d23_opid ;	/* operation flag/operation-ID */
} D23_ipcmsg ;


/* -------------- IPC - process specific information ---------------------- */

#define D23_MXEV		4	/* max. # of event fd's */
					/* initialization state */
#define D23_INIT		1
					/* set/get init state macros */
#define D23_SIS(x)		(x)
#define D23_GIS(x)		((x) & 0x01)

					/* attach states */
#define D23_INATTACH		0	/* process not attached to IPC */
#define D23_ICLTATT		1	/* process attached as a client */
#define D23_ISVRATT		2	/* process attached as a server */

					/* set/get attach state macros */
#define D23_SIAS(x)		((x) << 1)
#define D23_GIAS(x)		(((x) >> 1) & 0x03)

					/* event modes */
					/* see function d23_attsvr() */
					/* set/get/clear event mode macros */
#define D23_SIEVM(x)		((x) << 3)
#define D23_GIEVM(x)		(((x) >> 3) & 0x03)
#define D23_CIEVM		(~(0x03 << 3))

					/* server type values */
					/* see function d23_attsvr() */
					/* set/get/clear server type macros */
#define D23_SISTY(x)		((x) << 5)
#define D23_GISTY(x)		(((x) >> 5) & 0x03)
#define D23_CISTY		(~(0x03 << 5))

					/* destination-ID type values */
#define D23_DSTPID		1	/* destination-ID is a process-ID */
#define D23_DSTSID		2	/* destination-ID is a server-ID */

					/* set/get destination-ID type macros */
#define D23_SDSTTY(x)		((x) << 16)
#define D23_GDSTTY(x)		(((x) >> 16) & 0x03)

typedef struct {
	unsigned32	d23_istate ;	/* IPC-state */
	D23_rvpid	d23_ipid ;	/* real/virtual process-ID */
	signed32	d23_shmid ;	/* shared memory-ID */
	signed32	d23_semid ;	/* semaphore-ID */
	signed32	d23_msgid ;	/* message-ID */
	D23_ipcinfo	*d23_genarea ;	/* reference to the general SHM-area */
	D23_distcmd	*d23_dstarea ;	/* reference to dist. command area */
	D23_reginfo	*d23_regarea ;	/* reference to registration area */
	D23_ipcentry	*d23_svrarea ;	/* reference to the server area */
	D23_ipcentry	*d23_cltarea ;	/* reference to the client area */
	byte		*d23_bufarea ;	/* reference to com. buffer area */
	D23_usrcred	*d23_crdarea ;	/* reference to user credential area */
	D23_hpbufinfo	*d23_hparea ;	/* reference to heap area */
	D23_extinfo	*d23_extarea ;	/* reference to ext. area desc. area */
	D23_ipcentry	*d23_primref ;	/* reference to the primary server */
					/* process entry */
	signed16	d23_prassid ;	/* association-ID of primary server */
	D23_ipcentry	*d23_slstref ;	/* reference to server entry in progress */
	D23_ipcentry	*d23_clstref ;	/* reference to client entry in progress */
					/* process entry */
	unsigned16	d23_enaevmsk ;	/* enabled event type mask */
	signed16	d23_isvrid ;	/* server-ID */
	int		d23_pevfd ;	/* file descriptor of own pro- */
					/* cess specific named FIFO */
	int		d23_sevfd ;	/* file descriptor of own ser- */
					/* ver-ID specific named FIFO */
	struct {
	    int		d23_evfd ;	/* file descriptor of a named */
					/* FIFO of a corresponding */
					/* server/client */
	    signed32	d23_evid ;	/* destination-ID of a corresponding */
					/* server/client */
	    signed32	d23_evrc ;	/* event-ID reference counter */
	} d23_evinfo[D23_MXEV] ;
} D23_iprocinfo ;


/* -------------- IPC - interface function prototyping --------------------- */

					/* attach server application */
extern signed32 d23_attsvr (D23_atsvpb *) ;
					/* detach server application */
extern signed32 d23_detsvr (D23_desvpb *) ;
					/* wait for invoke request */
extern signed32 d23_waitin (D23_wtinpb *) ;
					/* return result request */
extern signed32 d23_result (D23_rsrqpb *) ;
					/* attach client application */
extern signed32 d23_attclt (D23_atclpb *) ;
					/* detach client application */
extern signed32 d23_detclt (D23_declpb *) ;
					/* bind request */
extern signed32 d23_bind (D23_bindpb *) ;
					/* unbind request */
extern signed32 d23_unbind (D23_ubndpb *) ;
					/* invoke request */
extern signed32 d23_invoke (D23_inrqpb *) ;
					/* wait for result request */
extern signed32 d23_waitrs (D23_wtrspb *) ;
					/* return IPC-buffer */
extern signed32 d23_retbuf (D23_rbufpb *) ;
					/* look for IPC-event */
extern signed32 d23_event (D23_evntpb *) ;
					/* abort IPC-association */
extern signed32 d23_abort (D23_abortpb *) ;
					/* abandon operation */
extern signed32 d23_abandon (D23_abandonpb *) ;
					/* disable abandon operation */
extern signed32 d23_disable_abandon (D23_disabandonpb *) ;
					/* get IPC-event mode info */
extern signed32 d23_gipc_eminfo (D23_ipcempb *) ;
					/* get server information */
extern signed32 d23_getsvrinfo (D23_gtsvpb *) ;
					/* add user credential */
extern signed32 d23_add_usr_cred (D23_addcrdpb *) ;
					/* remove user credential */
extern signed32 d23_rem_usr_cred (D23_remcrdpb *) ;
					/* read user credential */
extern signed32 d23_read_usr_cred (D23_readcrdpb *) ;
					/* write distributed command */
extern signed32 d23_write_dist_cmd (D23_dwritepb *) ;
					/* read distributed command */
extern signed32 d23_read_dist_cmd (D23_dreadpb *) ;
					/* confirm distributed command */
extern signed32 d23_conf_dist_cmd (D23_dconfpb *) ;
					/* register process */
extern signed32 d23_reg_process (D23_regprocpb *) ;
					/* set process state */
extern signed32 d23_set_pstate (D23_pstatpb *) ;

/* --------- prototyping of internal IPC-functions ------------------------- */

					/* lock shared memory */
extern signed32 d23_lock (signed32, signed32) ;
					/* unlock shared memory */
extern signed32 d23_unlock (signed32, signed32) ;
					/* handle IPC-error */
extern signed32 d23_ipcerror (D2_pbhead *, signed32) ;
					/* handle system-error */
extern signed32 d23_syserror (D2_pbhead *, signed32, signed32) ;
					/* get IPC-resources */
extern signed32 d23_get_ipc (void) ;
					/* send IPC-message */
extern signed32 d23_msgsnd (signed32, D23_ipcmsg *, signed32, signed32) ;
					/* receive IPC-message */
extern signed32 d23_msgrcv (signed32, D23_ipcmsg *, signed32, signed32,
							signed32, D2_pbhead *) ;
					/* send attention byte through FIFO */
extern signed32 d23_attsnd (signed32, signed32, D2_pbhead *) ;
					/* get virtual process-ID */
extern signed32 d23_getvpid (void) ;
					/* allocate SHM-heap memory */
extern signed32 d23_shmalloc (signed32) ;
					/* release SHM-heap memory */
extern signed32 d23_shmfree (signed32) ;
					/* get internal IPC-event */
extern signed32 d23_ievent (D23_evntpb *, signed32, signed32) ;
					/* set process state */
extern signed32 d23_set_pstate (D23_pstatpb *) ;
#if defined THREADSAFE
					/* RPC-server init routine */
extern signed32 d23_init_svcserver (unsigned_char_t *) ;
#endif

#endif /* _D23IPC_H */
