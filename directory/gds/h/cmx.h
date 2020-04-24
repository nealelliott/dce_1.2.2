/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cmx.h,v $
 * Revision 1.1.749.2  1996/02/18  23:35:21  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:25:15  marty]
 *
 * Revision 1.1.749.1  1995/12/08  15:59:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:13:17  root]
 * 
 * Revision 1.1.747.2  1994/07/06  15:07:26  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:16:05  marrek]
 * 
 * Revision 1.1.747.1  1994/02/22  18:07:32  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:42:35  marrek]
 * 
 * Revision 1.1.745.2  1993/10/14  17:27:40  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:26:16  keutel]
 * 
 * Revision 1.1.745.1  1993/10/13  17:32:22  keutel
 * 	October 1993 code drop
 * 	[1993/10/09  18:10:35  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  12:12:12  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:36:56  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  20:38:56  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:28:36  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/07  11:11:58  marrek
 * 	November 1992 code drop
 * 	t_error function name collision solved
 * 	[1992/12/07  11:03:50  marrek]
 * 
 * Revision 1.1.4.2  1992/12/04  18:09:15  marrek
 * 	November 1992 code drop
 * 	new cmx.h
 * 	[1992/12/04  11:09:31  marrek]
 * 
 * Revision 1.1.2.2  1992/06/02  03:46:50  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:40:49  zeliff]
 * 
 * $EndLog$
 */
#ifndef _CMX_H
#define _CMX_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char cmx_rcsid[] = "@(#)$RCSfile: cmx.h,v $ $Revision: 1.1.749.2 $ $Date: 1996/02/18 23:35:21 $";
#endif

/* #ident "@(#)cmx.h	311.22 93/03/12 CMX (NC1)" */

/*
 * COPYRIGHT (C) Siemens Nixdorf Informationssysteme AG 1992
 * 		 All Rights Reserved
 *  
 * This software is furnished under a license and may be used
 * only in accordance with the terms of that license and with the
 * inclusion of the above copyright notice. This software may not
 * be provided or otherwise made available to, or used by, any
 * other person. No title to or ownership of the software is
 * hereby transferred.
 */

/*********************************************************************/
/*								     */
/*								     */
/*			HEADER FILE  (cmx.h)			     */
/*								     */
/*  for the programmer's interface to the transport system in SINIX  */
/*								     */
/*			    I C M X (L) 			     */
/*								     */
/*  Defines:							     */
/*  _HETERO	Structure declarations should be independent from    */
/*		alignment, i.e. should result in the same memory     */
/*		layout for different compilers. This is important    */
/*		for their usage with message interfaces, e.g.        */
/*		between BS2000 and SINIX systems.         	     */
/*								     */
/*********************************************************************/

/*
 * Aenderungsstand:
 * RK	(NC152)	02.07.91	Basic version CMX V3.1 (<- 4.14).
 * ka	(NC152)	24.02.92 	Anpassungen wegen BS2000-Angleichung
 */

/*********************************************************************/
/*	CONSTANTS 						     */
/*********************************************************************/

#define T_LIBV20	0xc201
#define T_LIBV21	T_LIBV20
#define T_LIBV25	0x025a
#define T_LIBV30	0x030a
#define T_LIBV30B	0x030b
#define T_LIBV40	0x0400

/*
 * Return values of function calls
 */
#define T_NOTFIRST	1	/* Return value for t_attach */
#define T_NOTPRIV 	2	/* Return value for t_attach */
#define T_OK		0	/* Function call successful */
#define T_ERROR 	-1	/* Function call errored */
#define T_DATASTOP	-2 	/* Return value for t_datarq */
#define T_XDATSTOP	-3	/* Return value for t_xdatrq */
#define T_SHORTAGE_DATASTOP -4  /* Return value for datarq      (BS2-TPR) */
#define T_SHORTAGE_XDATSTOP -5  /* Return value for xdatrq      (BS2-TPR) */
#define T_TRYLATER      -99     /* Return-value for datarq      (BS2-TPR) */
#define T_INVREF        -100    /* invalid tref / invalid evref (BS2-TPR) */
/*
 * Return values for t_event
 */
#define T_NOEVENT	0	/* No event present */
#define T_DATAIN	1	/* Data reception indication */
#define T_DATAGO	2	/* Data transmittable again */
#define T_XDATIN	3	/* Data reception indication for expedited data */
#define T_XDATGO	4	/* Expedited data transmittable again */
#define T_CONIN 	5	/* Connection indication */
#define T_CONCF 	6	/* Connection confirmation */
#define T_DISIN 	7	/* Disconnection indication */
#define T_REDIN 	8	/* Redirection indication */
#define T_WAKEEVENT     9       /* WAKE-indication (BS2) */
#define T_SYS_EVENT    10       /* no CMX-Event (BS2) */

/*
 * Length definitions for user data with T-CONNECTION, T-DISCONNECTION
 * T-REDIRECT, T-EXPEDITED-DATA
 */
#define T_MSG_SIZE	256	/* Max. user data during T-CONNECTION (max) */
#define T_CON_SIZE	32	/* Max. user data during T-CONNECTION (OSI) */
#define T_DIS_SIZE_MAX	129	/* Max. user data during T-DISCONNECT (max) */
#define T_DIS_SIZE	64	/* Max. user data during T-DISCONNECT (OSI) */
#define T_RED_SIZE	32	/* Max. user data during T-REDIRECT */
#define T_EXP_SIZE	16	/* Max. length of expedited data */

/*
 * Values for parameters cmode, chain, vcnt
 */
#define T_WAIT		0	/* Wait for an event synchronously */
#define T_CHECK		1	/* Check for an event */
#define T_EVALUATE	2	/* Check for CMX event (BS2) */

#define T_END		0	/* No more data in current TSDU */
#define T_MORE		1	/* More data in current TSDU */

#define T_VCNT		3	/* Max. number of elements in buffer array */

#define T_EXCL          0       /* exclusive Eventpoint (BS2-TP) */
#define T_SHARE         1       /* shared Eventpoint (BS2-TP) */

/*
 * Return values of callback routine
 */
#define T_TSEVENT	1	/* Check for a TS-event */
#define T_USEREVENT	2	/* A user event occurred. Do not check
				   for TS-event */

/*
 * Values for parameter reason (reason of disconnect indication)
 * The explanation is also contained in the message catalogue cmxlib.cat
 * (see also cmxdec(1))
 *
 * 1. Generally used
 */
#define T_USER		0	/* Disconnect by partner (user),
				   possibly due to user error of partner */
/*
 * 2. Used with computer interconnection
 */
#define T_RTIMEOUT	1	/* Local disconnect by CMX due to inactivity
				   according to t_timeout */
#define T_RADMIN	2	/* Local disconnect by CMX due to CCP termination */
#define T_RCCPEND	3	/* Local disconnect by CMX due to CCP failure */

/*
 * For all reasons >= T_RCCP the disconnect is issued by the local CCP
 * ("local disconnect") or remote CCP ("remote disconnect");
 * all these reasons are of the format T_RCCP + ITRANS reason
 */
#define T_RCCP		256

/*
 * Reasons according to ISO 8073 (incremented by 256)
 */
#define T_RUNKNOWN	256	/* Remote disconnect, no reason specified
				   possibly due to user error of partner */
#define T_RSAPCONGEST	257	/* Remote disconnect due to TSAP congestion */
#define T_RSAPNOTATT	258	/* Remote disconnect due to not established TSAP */
#define T_RUNSAP	259	/* Remote disconnect due to unknown TSAP */
#define T_RPERMLOST	261	/* Remote disconnect by (network) administration */
#define T_RSYSERR	262	/* Error in network */

#define T_RCONGEST	385	/* Remote disconnect due to congestion during
				   connection establishment */
#define T_RCONNFAIL	386	/* Remote disconnect due to failure in
				   connection negotiation */
#define T_RDUPREF	387	/* Remote disconnect due to detection of
				   duplicate source reference for the same pair
				   of NSAPs */
#define T_RMISREF	388	/* Remote disconnect due to mismatched
				   references */
#define T_RPROTERR	389	/* Remote disconnect due to protocol error */
#define T_RREFOFLOW	391	/* Remote disconnect due to reference overflow */
#define T_RNOCONN	392	/* Remote disconnect due to connection refusal
				   on this network connection */
#define T_RINLNG	394	/* Remote disconnect due to invalid header
				   or parameter length */

/*
 * Other reasons
 */
#define T_RLCONGEST	448	/* Local disconnect due to congestion */
#define T_RLNOQOS	449	/* Local disconnect, QoS can no longer be
				   provided */
#define T_RILLPWD	451	/* Local disconnect due to invalid
				   (connection) password */
#define T_RNETACC	452	/* Local disconnect due to denial of
				   network access */
#define T_RLPROTERR	464	/* Local disconnect due to protocol error */
#define T_RLINTIDU	465	/* Local disconnect, received too long TIDU */
#define T_RLNORMFLOW	466	/* Local disconnect due to violation of flow
				   control for normal data */
#define T_RLEXFLOW	467	/* Local disconnect due to violation of flow
				   control for expedited data */
#define T_RLINSAPID	468	/* Local disconnect due to invalid TSAP id */
#define T_RLINCEPID	469	/* Local disconnect due to invalid TCEP id */
#define T_RLINPAR	470	/* Local disconnect due to invalid parameter */
#define T_RLNOPERM	480	/* Connection inhibited by local administration */
#define T_RLPERMLOST	481	/* Disconnect by local administration */
#define T_RLNOCONN      482     /* Connection can not be set up locally because
				   no network connection can be established */
#define T_RLCONNLOST	483	/* Local disconnect due to loss of network connection */
#define T_RLNORESP	484	/* Connection can not be set up because 
				   partner does not respond to CONRQ */
#define T_RLIDLETRAF	485	/* Local disconnect due to loss of connection
				   (Idle Traffic Inactivity Timeout) */
#define T_RLRESYNC	486	/* Local disconnect because resynchronization 
				   failed (more than 10 retries) */
#define T_RLEXLOST	487	/* Local disconnect because expedited channel
				   is inoperable */

/*****************************************************************************/
/*	ERROR INFORMATIONS 						     */
/*****************************************************************************/

/*
 * Error information returned by t_cmxerror() is structured as follows
 *	
 * Bit	15    12 11    8 7	       0
 *	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *	|   o	|   o	|	o	|
 *	+-+-|-+-+-+-|-+-+-+-+-+-|-+-+-+-+
 *	    |	    |		+--- ERROR VALUE (Bit 2**0  - 2**7)
 *	    |	    +--- ERROR CLASS (Bit 2**8  - 2**11)
 *	    +--- ERROR TYPE (Bit 2**12 - 2**15)
 *	
 */

/*
 * ERROR TYPES
 */
#define T_CMXTYPE	(0 << 12)	/* CMX error */
#define T_DSTEMP_ERR	(2 << 12)	/* Temporary TNSX error */
#define T_DSCALL_ERR	(3 << 12)	/* Call error to TNSX */
#define T_DSPERM_ERR	(4 << 12)	/* Permanent TNSX error */
#define T_DSWARNING	(5 << 12)	/* TNSX warning */
#define T_DSAPP_ERR	(6 << 12)	/* DUA error */
#define T_BX1           (7 << 12)       /* NEABX error */
#define T_BX2           (8 << 12)       /* NEABX error */
#define T_BX3           (9 << 12)       /* NEABX error */

/*
 * ERROR CLASSES for CMX and TNSX (for NEABX cf. neabx.h)
 */
#define T_CMXCLASS	(0 << 8)	/* CMX class */
#define T_DSNOT_SPEC	(2 << 8)	/* Not specified */
#define T_DSPAR_ERR	(3 << 8)	/* Parameter error */
#define T_DSILL_VERS	(4 << 8)	/* Invalid version */
#define T_DSSYS_ERR	(5 << 8)	/* System error (cf. errno.h) */
#define T_DSINT_ERR	(6 << 8)	/* Internal error */
#define T_DSMESSAGE	(7 << 8)	/* Info */

/*
 * ERROR VALUES for CMX (for TNSX cf. tnsx.h, for NEABX cf. neabx.h)
 *
 * There are three sets of CMX errors:
 * - general error 
 * - CMX errors related ontext name and address mapping
 * - additional errors for station interconnection with CCP-STA2
 * The values are as follows:
 */

/*
 * ERROR VALUES for name and address mapping
 */
#define T_NOTSPEC	0	/* Not specified */
#define T_DIRERR	1	/* TS directory unknown */
#define T_NAMERR	2	/* GLOBAL NAME not/already existing */
#define T_ILLNAM	3	/* GLOBAL NAME syntactically wrong */
#define T_PROPER	5	/* Property not/already existing */
#define T_TIMOUT	7	/* No response from tnsxd */
#define T_LEAFNO	10	/* More/fewer GLOBAL NAMEs than expected */
#define T_LENERR	16	/* Length error in path/property */
#define T_PROT		20	/* Error in tnsxd protocol */
#define T_NORQ		21	/* tnsxd currently not available */
#define T_LFILE		100	/* Length error in TS directory file */

/*
 * General ERROR VALUES
 */
#define T_NOERROR	0	/* No error */
#define T_ENOENT	2	/* All resources provided are busy */
#define T_EINTR		4	/* Function call interrupted by signal */
#define T_EIO		5	/* I/O error, CCP no longer ready */
#define T_EBADF 	9	/* Function call not allowed in this state */
#define T_ENOMEM	12	/* Insufficient memory */
#define T_EFAULT	14	/* Address not in allocated address space */
#define T_EBUSY 	16	/* Network special file already open (does
				   not appear at ICMX(L) */
#define T_EINVAL	22	/* Invalid argument */
#define T_ENOBUF	132	/* Temporary shortage of resources */

#define T_UNSPECIFIED	100	/* Unspecified error */
#define T_WSEQUENCE	101	/* Function call not allowed in this state */
#define T_WREQUEST	102	/* Invalid function call */
#define T_WPARAMETER	103	/* Invalid parameter */
#define T_WAPPLICATION	104	/* Application not/already attached */
#define T_WAPP_LIMIT	105	/* No further attaching of applications possible */
#define T_WCONN_LIMIT	106	/* Connection limit reached */
#define T_WTREF 	107	/* Invalid transport reference */
#define T_WTUI_MSG	108	/* Invalid TUI message (CCP-STA2)) */
#define T_COLLISION	109	/* Collision with connection setup/close-down/
				   redirection */
#define T_WPROC_LIMIT	110	/* Too many processes have attached to
				   applications */
#define T_NOCCP 	111	/* No CCP for this application/connection 
				   available */
#define T_ETIMEOUT	112	/* CCP does not respond in time, or expiration 
				   of timer for connection redirection */
#define T_WROUTINFO	113	/* Invalid CC selection (CC index)
				   during connection request */
#define T_CCP_END	114	/* CCP is no longer ready */
#define T_WRED_LIMIT	115	/* Too many connection redirections (temporary) */
#define T_WLIBVERSION	116	/* Library version incompatible */
#define T_CBRECURSIVE   117     /* Recursive call of t_event not allowed */
#define T_W_NDS_ACCESS  118     /* A NDS error occurred */



/*********************************************************************/
/*	NAMES AND ADDRESSES					     */
/*********************************************************************/

/*
 * Address formats supported only in CMX V1.0  (no TNSX)
 */
struct t_tdaddr {
	char t_admode;		/* = T_TRANSDATA (addressing mode) */
#define T_TRANSDATA	'0'
	char t_pname[8];	/* TRANSDATA processor/region number */
	char t_stname[8];	/* TRANSDATA station name */
	char t_term;		/* = \0 (structure end indicator) */
};

struct t_t70addr {
	char t_admode; 	 	/* = T_T70 (addressing mode) */
#define T_T70		'1'
	char t_netadr[21];	/* Network addressing information */
	char t_tsapid[21];	/* TSAP identifier */
	char t_protocolid[5];	/* X.25 'protocolid' */
};

#include <cmx_addr.h>

union t_address {
	struct t_myname		tmyname;
	struct t_partaddr	tpartaddr;
	struct t_tdaddr		tdaddr;
	struct t_t70addr	t70addr;
};


/*********************************************************************/
/*	STRUCTURE OF OPTIONS					     */
/*********************************************************************/

/*
 * Values for parameter t_apmode, t_conlimit, t_xdata, t_timeout
 */
#define T_ACTIVE	1	/* Only active connection setup */
#define T_PASSIVE	2	/* Only passive connection setup */
#define T_REDIRECT	4	/* Only connection redirection */

#define T_YES		1	/* Option requests */
#define T_NO		0	/* Option not requested */
#define T_NOLIMIT	-1	/* No limitation */

struct t_opta1 {
	int	t_optnr;	/* Option number */
#define T_OPTA1 1
	int	t_apmode;	/* Application mode */
	int	t_conlim;	/* Max. number of connections */
};

struct t_opta2 {
	int	t_optnr;	/* Option number */
#define T_OPTA2 3
	int	t_apmode;	/* Application mode */
	int	t_conlim;	/* Max. number of connections */
	int	t_uattid;	/* User reference of application */
	int	t_attid;	/* CMX reference of application */
	int	t_ccbits;	/* Bit mask of CCs */
#define T_W1	0x000001
#define T_W2	0x000002
#define T_L1	0x000004
#define T_L2	0x000008
#define T_W3	0x000010
#define T_W4	0x000020
#define T_W5	0x000040
#define T_W6	0x000080
#define T_LBS2	0x000100
#define T_LOOP	0x000200
#define T_B1	0x000400
#define T_L3	0x000800
#define T_L4	0x001000
#define T_L5	0x002000
#define T_L6	0x004000
	int	t_sptypes;	/* Bit mask of address formats */
#define T_WANNEA   	0x0001
#define T_LANSBKA  	0x0002
#define T_STA      	0x0004
#define T_WAN3SBKA	0x0008
#define T_WANSBKA  	0x0010
#define T_STANEA   	0x0020
#define T_EMSNA    	0x0040
#define T_SDLCSBKA 	0x0080
#define T_LANINET  	0x0100
#define T_LBS2NEA  	0x0200
#define T_LOOPSBKA 	0x0400
#define T_TRSNASBKA    	0x0800 
#define T_LANGWSBKA 	0x1000
#define T_LANBSBKA 	0x2000
#define T_WANGWSBKA	0x4000
};

struct t_opta4 {
	int	t_optnr;	/* Option number */
#define T_OPTA4 4
	int	t_apmode;	/* Application mode */
	int	t_conlim;	/* Max. number of connections */
	int     t_mignea;       /* T_YES with NEABX migration, T_NO without */
};

/* the following structure t_opta5 is for BS2-TPR only */

struct t_opta5 {
	int	t_optnr;	/* Option number */
#define T_OPTA5 5
	int 	t_apmode;	/* Application mode */
	int	t_conlim;	/* max. number of connections */
	int 	t_uattid;	/* user reference of application */
	int	t_attid;	/* CMX reference of application */
	int	t_ccbits;	/* bit mask of CCs */ 
	int	t_sptypes;	/* bit mask of address formats */
	int 	t_evref;	/* system reference event point */
};

struct t_opta6 {
	int	t_optnr;	/* Option number */
#define T_OPTA6 6
	int	t_apmode;	/* Application mode */
	int	t_conlim;	/* Max. number of connections */
	int	t_uattid;	/* User reference of application */
	int	t_attid;	/* CMX reference of application */
	struct t_cclst	*t_cclist; /* Address of bit list with CCs affected */
	int	t_sptypes;	/* bit mask of address formats */
};

/*
 * Data structure to store a CC list.
 */
struct t_cclst
{
	unsigned short	cl_num;		/* number of elements in cl_id, or: */
#define	CMX_ALLCCIDS	0xffff
#define	CMX_NCC	16			/* Maximale Anzahl bedienbarer CCs */
	unsigned short	cl_id[CMX_NCC];	/* list of CC identifiers */
/*
 * Adaptor identifier values.
 */
#define CMX_WANADID	0x0000		/* Id. of WAN-CC adaptor */
#define CMX_LANADID	0x0001		/* Id. of LAN-CC adaptor */
#define CMX_LBS2ADID	0x0002		/* Id. of local BS2000 adaptor */
#define CMX_LOOPADID	0x0003		/* Id. of loop adaptor */
#define CMX_TPIADID	0x0004		/* Id. of TPI adaptor */
#define CMX_MB2ADID	0x0005		/* Id. of Multibus 2 adaptor */
/*
 * Pseudo adaptors.
 */
#define	CMX_XTIID	0x000d		/* ICMX(L) on top of XTI */
#define	CMX_LAN1ID	0x000e		/* ICMX(L) on top of TCP/IP */
#define	CMX_SNAID	0x000f		/* SNA communication */

};

struct t_optc1 {
	int	t_optnr;	/* Option number */
#define T_OPTC1	1
	char	*t_udatap;	/* Data buffer */
	int	t_udatal;	/* Length of data buffer */
	int	t_xdata;	/* Expedited option */
	int	t_timeout;	/* Inactivity timer and redirect timer */
};

struct t_optc2 {
	int	t_optnr;	/* Option number */
#define T_OPTC2	2
	char	*t_udatap;	/* Data buffer */
	int	t_udatal;	/* Length of data buffer */
};

struct t_optc3 {
	int	t_optnr;	/* Option number */
#define T_OPTC3	7
	char	*t_udatap;	/* Data buffer */
	int	t_udatal;	/* Length of data buffer */
	int	t_xdata;	/* Expedited option */
	int	t_timeout;	/* Inactivity timer */
	int	t_ucepid;	/* User reference of connection */
};

struct t_opte1 {
	int	t_optnr;	/* Option number */
#define T_OPTE1 1
	int	t_attid;	/* CMX referencce of application */
	int	t_uattid;	/* User reference of application */
	int	t_ucepid;	/* User reference of connection */
	int	t_timeout;	/* Timer for T_WAIT */
	int	t_evdat;	/* Event-specific information */
};

/* the following structure t_opte2 is for BS2-TPR only */

struct t_opte2 {
	int	t_optnr;	/* Option number */
#define T_OPTE2 2
	int	t_attid;	/* CMX reference of application */
	int	t_uattid;	/* User reference of application */
	int	t_ucepid;	/* User reference of connection */
	int	t_timeout;	/* Timer for T_WAIT */
	int	t_evdat;	/* Event-specific information */
	int	t_evref;	/* System reference event point */
	int	t_evinf[10];	/* BS2000 event information words */
};

/* the following structure t_optg1 is for BS2-TPR only */

struct t_optg1 {
	int	t_optnr;	/* Option number */
#define T_OPTG1 1
	int	t_evref;	/* system reference event point */
  	int	t_buf[200]; 	/* workarea */
};

struct t_opti1 {
	int	t_optnr;	/* Option number */
#define T_OPTI1 1
	int	t_maxl;		/* Length of TIDU */
};

/* the following structure t_opti2 is for BS2-TPR only */

struct t_opti2 {
	int	t_optnr;	/* Option number */
#define T_OPTI2 2
	int	t_evref;	/* system reference event point */
  	int	t_buffer[180]; 	/* buffer for Name Service output */
};

struct t_opti3 {
	int	t_optnr;	/* Option number */
#define T_OPTI3 3
	int	t_maxl; 	/* TIDU Size */
  	int	t_fdsetsize; 	/* CMX specific setsize */
	int	t_openmax;	/* system specific maximum number */
				/* of filedescriptors per process */
	int 	t_tsdu;		/* size of TSDU */
	int 	t_etsdu;	/* size of ETSDU */
	int 	t_condata;	/* maximum size of userdata at conrq */
	int 	t_concfdata;	/* maximum size of userdata at concf */
	int 	t_disdata;	/* maximum size of userdata at disrq */
	int 	t_adrsize;	/* maximum size of address */
};

/* Defines for function t_setopt */
#define	T_LIB 1

struct t_opts1 {
	int	t_optnr;	/* Option number */
#define T_OPTS1	1
	int	t_optname;	/* Option name */
#define T_DEBUG 1
	char	*t_optvalue;	/* Pointer to option string */
};

struct t_data {
	char	*t_datap;	/* Data buffer */
	int	t_datal;	/* Length of data buffer */
};


/*
 * Address formats supported as of CMX V2.0 in association with TNSX
 * LOCAL NAME, TRANSPORT ADDRESS
 * _HETERO for use in environments with different byte order
 */

#define T_COM	0		/* Computer interconnection */
#define T_TUI	1		/* Station interconnection (via TUI) */
#define T_SNA	2		/* Interconnection across SNA (via TRANSIT) */
#define T_INET	3		/* Interconnection across INTERNET (via sockets) */
#define T_XTI	4		/* Interconnection across XTI-calls */


typedef union {
	struct t_opta1 opta1;
	struct t_opta2 opta2;
	struct t_opta4 opta4;
	struct t_opta5 opta5;
	struct t_opta6 opta6;
} t_opta;

typedef union {
	struct t_optc1 optc1;
	struct t_optc2 optc2;
} t_opt12;

typedef union {
	struct t_optc2 optc2;
	struct t_optc3 optc3;
} t_opt23;

typedef union {
	struct t_optc1 optc1;
	struct t_optc3 optc3;
} t_opt13;

typedef union {
	struct t_optc1 optc1;
} t_opt1;

typedef union {
	struct t_optc2 optc2;
} t_opt2;

typedef union {
	struct t_optg1 optg1;
} t_optg;


typedef union {
	struct t_opte1 opte1;
	struct t_opte2 opte2;
} t_opte;

typedef union {
	struct t_opti1 opti1;
	struct t_opti2 opti2;
	struct t_opti3 opti3;
} t_opti;

typedef union {
	struct t_opts1 opts1;
} t_opts;

/*
 * Prototypes of ICMX(L) functions
 */

int	t_attach(struct t_myname *name, t_opta *opt);
int	t_detach(struct t_myname *name);
int	t_event(int *tref, int cmode, t_opte *opt);
int	t_conrq(int *tref, union t_address *toaddr, union t_address *fromaddr, t_opt13 *opt);
int	t_conin(int *tref, union t_address *toaddr, union t_address *fromaddr, t_opt1 *opt);
int	t_conrs(int *tref, t_opt13 *opt);
int	t_concf(int *tref, t_opt1 *opt);
int	t_disrq(int *tref, t_opt2 *opt);
int	t_disin(int *tref, int *reason, t_opt2 *opt);
int	t_redrq(int *tref, int *pid, t_opt12 *opt);
int	t_redin(int *tref, int *pid, t_opt23 *opt);
int	t_datarq(int *tref, char *datap, int *datal,int *chain);
int	t_datain(int *tref, char *datap, int *datal,int *chain);
int	t_datastop(int *tref);
int	t_datago(int *tref);
int	t_vdatain(int *tref, struct t_data *vdata, int *vcnt, int *chain);
int	t_vdatarq(int *tref, struct t_data *vdata, int *vcnt, int *chain);
int	t_xdatrq(int *tref, char *datap, int *datal);
int	t_xdatin(int *tref, char *datap, int *datal);
int	t_xdatstop(int *tref);
int	t_xdatgo(int *tref);
int	t_info(int *tref, t_opti *opt);
int	t_cmxerror(void);

struct	t_partaddr *t_getaddr(char *glob, t_optg *opt);
char	*t_getname(struct t_partaddr *addr, t_optg *opt);
struct	t_myname *t_getloc(char *glob, t_optg *opt);

#if defined(_AIX)
# include <sys/select.h>
#endif /* _AIX */
#include <sys/types.h>
#include <sys/time.h>
/* the following procedures are not for use in BS2-TPR-programs */
int 	(*t_callback(int(*)(int,fd_set *,fd_set *,fd_set *,struct timeval *,void *),
	                    void *, void *))
	(int,fd_set *,fd_set *,fd_set *,struct timeval *,void *);
char	*t_strerror(int code);
char	*t_strreason(int reason);
void	t_perror(char *s, int code);
void	t_preason(char *s, int reason);
int	t_setopt(int component, t_opts *opt);
int     t_addr2str(char *string, int str_length, union t_address *address, int addr_length);

/* the following procedures are only for use in BS2-TPR-programs */
int 	t_close_event(int *evref);
int	t_open_event(int *evref, int *uevid, int *opt);

/* the following procedures are only for use in BS2-programs */
int 	t_wake(int *pid, int *evref);
int 	t_get_error(int *evref);
#endif /* _CMX_H */
