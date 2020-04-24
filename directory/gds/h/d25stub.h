/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d25stub.h,v $
 * Revision 1.2.754.2  1996/02/18  23:35:40  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:25:48  marty]
 *
 * Revision 1.2.754.1  1995/12/08  16:00:21  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:13:42  root]
 * 
 * Revision 1.2.752.2  1994/05/10  15:55:49  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:37:47  marrek]
 * 
 * Revision 1.2.752.1  1994/02/22  18:49:33  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:44:45  marrek]
 * 
 * Revision 1.2.750.2  1993/10/14  17:27:46  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:26:44  keutel]
 * 
 * Revision 1.2.750.1  1993/10/13  17:32:34  keutel
 * 	October 1993 code drop
 * 	[1993/10/09  18:26:28  keutel]
 * 
 * Revision 1.2.6.2  1993/08/10  12:37:58  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:39:47  marrek]
 * 
 * Revision 1.2.4.3  1993/02/02  15:40:12  marrek
 * 	Use 1L in D25_EXTFLG
 * 	[1993/02/02  11:11:04  marrek]
 * 
 * Revision 1.2.4.2  1992/12/31  20:40:39  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:29:28  bbelch]
 * 
 * Revision 1.2.2.2  1992/06/02  04:02:56  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:42:06  zeliff]
 * 
 * $EndLog$
 */
#ifndef _D25STUB_H
#define _D25STUB_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char d25stub_rcsid[] = "@(#)$RCSfile: d25stub.h,v $ $Revision: 1.2.754.2 $ $Date: 1996/02/18 23:35:40 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*****************************************************************************/
/*                                                                           */
/*                  S T U B - H E A D E R - F I L E                          */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                        DATE: 21.5.91     */
/*****************************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <d23ipc.h>
#include <ros.h>

/* ------------------- general constants ----------------------------------- */

						/* function return values */
#define D25_ERROR		-1
#define D25_SUCES		0

#define D25_UNUSED		-1		/* unused-flag */
						/* retry times (in seconds) */
						/* during initialization */
#define D25_RETRYTIME		30		/* retry 'get PSAP-address' */
#define D25_RETRYTIME1		10		/* retry 'bind DUA-cache' */
#define D25_DFROSASSOC		32		/* default # of ROS-associations */
#define D25_DFIPCASSOC		32		/* default # of IPC-associations */

/* ------------------- ROS-association table entry ------------------------- */

typedef struct {
	unsigned32	d25_rstate ;		/* entry state */
	signed32	d25_rassid ;		/* ROS-association-ID */
	signed16	d25_rrefcnt ;		/* ROS-association reference */
						/* counter */
	signed16	d25_rcontid ;		/* application context-ID */
	signed16	d25_rabsid ;		/* abstract syntax-ID */
	signed16	d25_rtransid ;		/* transfer syntax-ID */
	signed16	d25_rpsaplen ;		/* own PSAP-address length */
	byte		*d25_rpsapref ;		/* own PSAP-address reference */
	signed16	d25_rbndrqlen ;		/* bind request user data */
						/* length */
	byte		*d25_rbndrqref ;	/* bind request user data */
						/* reference */
	signed16	d25_rbndcflen ;		/* bind confirmation user */
						/* data length */
	byte		*d25_rbndcfref ;	/* bind confirmation user */
						/* data reference */
} D25_rosentry ;


#define D25_MXROSASSOC		128		/* max. # of ROS-associations */

						/* entry states */
#define D25_RFREE		1		/* entry is free */
#define D25_RBUSY		2		/* entry is occupied */

						/* set/get entry state macros */
#define D25_SRES(x)		(x)
#define D25_GRES(x)		((x) & 0x0F)

						/* initiator flag values */
#define D25_RINITIATOR		1		/* ROS-association initiated */
						/* by Stub */

						/* set/get initiator flag macros */
#define D25_SRIF(x)		((x) << 4)
#define D25_GRIF(x)		(((x) >> 4) & 1)

/* ------------------- User-data description buffer ------------------------ */

typedef struct {
	signed32	d25_datlen ;			/* size of user data */
	signed32	d25_datoff ;			/* operation offset */
	byte		*d25_datref ;			/* reference to user data */
} D25_usrdat ;

/* ------------------- IPC-association table entry ------------------------- */

typedef struct {
	unsigned32	d25_istate ;		/* entry state/entry type */
	D25_rosentry	*d25_irosref ;		/* ROS-association table */
						/* entry reference */
	signed16	d25_iassid ;		/* IPC-association-ID */
	D23_invinfo	d25_invinfo ;		/* IPC-invoke info */
	signed16	d25_ibufsz ;		/* max. size of IPC-buffer */
	byte		*d25_ibufref ;		/* IPC-buffer reference */
	D25_usrdat	d25_iudat ;		/* user data description */
	R_int		d25_ioprid ;		/* operation-ID */
	signed16	d25_iresid ;		/* result-ID */
	R_int		d25_ierrejval ;		/* error- resp. reject value */
} D25_ipcentry ;

						/* operational states */
#define D25_IRIDLE		1
#define D25_IRASSOCIATION	2
#define D25_IR2ABORTED		3

	  /* states belonging to outgoing DAP/DSP only */

#define D25_IBIND_PENDING	4
#define D25_IINVOKE_PENDING	5
#define D25_IUNBIND_PENDING	6

	  /* states belonging to incoming DAP/DSP only */

#define D25_RBIND_PENDING	7
#define D25_RINVOKE_PENDING	8
#define D25_R1UNBIND_PENDING	9
#define D25_R2UNBIND_PENDING	10
#define D25_R1ABORTED		11

	/* state belonging to DUA-cache update only */
#define D25_ICINVOKE_PENDING	12

	/* global Stub state */
#define D25_IRINIT		13

#define D25_MXSSTATE		13	/* max. # of states */

					/* set/get/change operational state */
					/* macros */
#define D25_SIOS(x)		(x)
#define D25_GIOS(x)		((x) & 0x1F)
#define D25_CIOS(x,y)		x = (((x) & (~0x1F)) | D25_SIOS(y))

					/* entry type */
#define D25_DACTIVE		1	/* incoming DAP/DSP specific entry */
#define D25_DPASSIVE		2	/* outgoing DAP/DSP specific entry */
#define D25_CACTIVE		3	/* DUA-cache update specific entry */

					/* set/get entry type macros */
#define D25_SITY(x)		((x) << 5)
#define D25_GITY(x)		(((x) >> 5) & 0x03)

					/* entry sub-type */
#define D25_IPCPOPR		1	/* parallel IPC-operation */

					/* set/get entry sub-type macros */
#define D25_SPTY(x)		((x) << 7)
#define D25_GPTY(x)		(((x) >> 7) & 0x03)


/* ------------------- event parameter block ------------------------------- */

typedef struct {
	struct D25_stubinfo *d25_esinforef ;	/* --> reference to the Stub */
						/* information table */
	D25_ipcentry	    *d25_eipcref ;	/* <-- reference to the actual */
						/* IPC-association table */
						/* entry */
	D25_rosentry	    *d25_erosref ;	/* <-- reference to the actual */
						/* ROS-association table */
						/* entry */
	signed32	    d25_eevent ;	/* <-- received event */
	signed32	    d25_estate ;	/* <-- actual state */
} D25_eventpb ;


/* ------------------- action parameter block ------------------------------ */

typedef struct {
	struct D25_stubinfo *d25_asinforef ;	/* --> reference to the Stub */
						/* information table */
	D25_ipcentry	    *d25_aipcref ;	/* <--> reference to the */
						/* actual IPC-association */
						/* table entry (NULL-pointer */
						/* if undefined) */
	D25_rosentry	    *d25_arosref ;	/* <--> reference to the */
						/* actual ROS-association */
						/* table entry (NULL-pointer */
						/* if undefined) */
	signed32	    d25_astate ;	/* --> actual state */
	signed32	    d25_aevent ;	/* --> actual event */
	signed32	    d25_aglopar ;	/* --> global action param. */
	union {
	    signed32	    d25_alocpar ;	/* <--> local action param. */
	    void	    *d25_alocptr ;
	} d25_lpar ;
} D25_actionpb ;

						/* function return values */
#define D25_ACONT		1		/* continue execution */
#define D25_ASKIP		2		/* skip next entry */
#define D25_ATERM		3		/* terminate execution */
#define D25_ANSUC		4		/* continue execution at */
						/* next successor state */
#define D25_AIEXCPT		5		/* continue execution at */
						/* exception entry */
#define D25_AREXCPT		6		/* continue execution at */
						/* exception entry */
#define D25_AERROR		7		/* fatal error */

						/* global action parameter */
						/* values (used by */
#define	D25_EQTERM		1		/* function d25_checkres()) */
#define D25_GTSKIP		2
#define D25_GTNSUC		3
#define D25_EQNSUC		4
#define D25_ACTNSUC		5
#define D25_ENDNSUC		6
#define D25_CONTSKIP		7
#define D25_POPRNSUC		8


/* ------------------- state/event table entry ----------------------------- */

#define D25_MXACTION		4		/* max. # of action */
						/* reference entries */

typedef struct {
	unsigned32	d25_stevinfo ;		/* state/event information */
	signed32	(*d25_action[D25_MXACTION])(D25_actionpb *) ;
						/* action references */
						/* (function entry points) */
} D25_steventry ;


						/* event values */
#define D25_IRNO_EV		0		/* no event */

						/* IPC-events */
#define D25_IBIND_EV		1		/* bind request */
#define D25_IUNBIND_EV		2		/* unbind request */
#define D25_IINVOKE_EV		3		/* invoke request */
#define D25_IMINVOKE_EV		4		/* invoke request */
						/* (more data flag set) */
#define D25_IRESULT_EV		5		/* return result */
#define D25_IMRESULT_EV		6		/* return result */
						/* (more data flag set) */
#define D25_IERROR_EV		7		/* return error */
#define D25_IREJECT_EV		8		/* return reject */
#define D25_IRETBUF_EV		9		/* return buffer */
#define D25_IABORT_EV		10		/* return abort */

						/* ROS-events */
#define D25_RBIND_EV		12		/* bind request */
#define D25_RCFBIND_EV		13		/* bind confirmation */
#define D25_RRJBIND_EV		14		/* bind confirmation (reject) */
#define D25_RUNBIND_EV		15		/* unbind request */
#define D25_RCFUBIND_EV		16		/* unbind confirmation */
#define D25_RINVOKE_EV		17		/* invoke request */
#define D25_RRESULT_EV		18		/* return result */
#define D25_RERROR_EV		19		/* return error */
#define D25_RUSRREJ_EV		20		/* return user reject */
#define D25_RPROVREJ_EV		21		/* return provider reject */
#define D25_RRETBUF_EV		22		/* return buffer */
#define D25_RABORT_EV		23		/* return abort */

#define D25_IEXCPT_EV		30		/* IPC-exception event */
#define D25_REXCPT_EV		31		/* ROS-exception event */

						/* set/get event macros */
#define D25_SEV(x)		(x)
#define D25_GEV(x)		((x) & 0x1F)

						/* set/get state macros */
#define D25_SST(x)		((x) << 5)
#define D25_GST(x)		(((x) >> 5) & 0x1F)

						/* set/get # of actions macros */
						/* (successor state 1) */
#define D25_S1NA(x)		((x) << 10)
#define D25_G1NA(x)		(((x) >> 10) & 0x7)

						/* set/get state macros */
						/* (successor state 1) */
#define D25_S1SU(x)		((x) << 13)
#define D25_G1SU(x)		(((x) >> 13) & 0x1F)
						/* set/get # of actions macros */
						/* (successor state 2) */
#define D25_S2NA(x)		((x) << 18)
#define D25_G2NA(x)		(((x) >> 18) & 0x07)
						/* set/get state macros */
						/* (successor state 2) */
#define D25_S2SU(x)		((x) << 21)
#define D25_G2SU(x)		(((x) >> 21) & 0x1F)

						/* set/get action parameter macros */
#define D25_SAPAR(x)		((x) << 26)
#define D25_GAPAR(x)		(((x) >> 26) & 0x0F)

						/* continuation flag values */
#define D25_CONTFLG		((unsigned const) 1L << 30)

						/* extention entry flag */
#define D25_EXTFLG		((unsigned const) 1L << 31)


/* ------------------- STUB information table ------------------------------ */

typedef struct D25_stubinfo {
	signed32	d25_aplid ;		/* application-ID */
	pid_t		d25_pid ;		/* process-ID */
	signed16	d25_dirid ;		/* directory-ID */
	signed16	d25_spsaplen ;		/* length of the own PSAP- */
						/* address */
	byte		*d25_spsapref ;		/* reference to the own */
						/* PSAP-address */
	signed16	d25_imaxentries ;	/* max. # of IPC-association */
						/* table entries */
	signed16	d25_iactentries ;	/* actual # of IPC-association */
						/* table entries */
	D25_ipcentry	*d25_itabref ;		/* reference to the IPC- */
						/* association table */
	byte		*d25_imnref ;		/* lowest used IPC-buffer */
						/* reference */
	byte		*d25_imxref ;		/* highest used IPC-buffer */
						/* reference */
	signed16	d25_imbufsz ;		/* max. IPC-buffer size */
	signed16	d25_rmaxentries ;	/* max. # of ROS-association */
						/* table entries */
	signed16	d25_ractentries ;	/* actual # of ROS-association */
						/* table entries */
	D25_rosentry	*d25_rtabref ;		/* reference to the ROS- */
						/* association table */
	signed16	d25_nstentries ;	/* # of state/event table */
						/* entries */
	D25_steventry	*d25_stevref ;		/* reference to the state/ */
						/* event table */
						/* references to state */
						/* specific state/event table */
						/* entries */
	D25_steventry	*d25_stsubref[D25_MXSSTATE] ;
	signed16	d25_nacentries ;	/* # of application context */
						/* description table entries */
	R_apcary	*d25_actabref ;		/* reference to the application */
						/* context description table */
	R_int		*d25_asitabref ;	/* reference to the abstract */
						/* syntax-ID table */
	signed16	d25_nasentries ;	/* # of abstract syntax */
						/* description table entries */
	R_absary	*d25_astabref ;		/* reference to the abstract */
						/* syntax description table */
	R_int		*d25_tsitabref ;	/* reference to the transfer */
						/* syntax-ID table */
	signed16	d25_ntsentries ;	/* # of transfer syntax */
						/* description table entries */
	R_trsary	*d25_tstabref ;		/* reference to the transfer */
						/* syntax description table */
	R_unsign	*d25_oitabref ;		/* reference to the object-ID */
						/* description table */
	char		*d25_dirpathref ;	/* reference to the instal- */
						/* lation path name */
} D25_stubinfo ;


/* ---------- TP-routines send/receive buffer description entry ------------ */

typedef struct {
	signed16	d25_tpstate ;		/* entry state */
	D25_usrdat	*d25_tpref ;		/* send/receive data descr. */
} D25_tpinfo ;

						/* entry state values */
#define D25_TPREAD	1			/* send buffer */
#define D25_TPWRITE	2			/* receive buffer */



/* ------------ transport interface logging constants ---------------------- */

			/* environment variable which describes transport */
			/* interface logging facilities */
#ifdef CSTUB
#define TSTRCPAR	"CSTUB_TSTRACE"
#else
#define TSTRCPAR	"SSTUB_TSTRACE"
#endif

/* ---------------------- function prototyping ----------------------------- */

extern signed32 d25_initasn1 (D25_actionpb *) ;
extern signed32 d25_apdutoasn1 (D25_actionpb *) ;
extern signed32 d25_asn1toapdu (D25_actionpb *) ;
extern signed32 d25_cvfname (signed32 (*)(D25_actionpb *)) ;
extern signed32 d25_evmgt (D25_stubinfo *) ;
extern signed32 d25_ipcsvratt (D25_actionpb *) ;
extern signed32 d25_ipsvrdet (D25_actionpb *) ;
extern signed32 d25_ipccltatt (D25_actionpb *) ;
extern signed32 d25_ipcltdet (D25_actionpb *) ;
extern signed32 d25_ipcbind (D25_actionpb *) ;
extern signed32 d25_ipcunbind (D25_actionpb *) ;
extern signed32 d25_ipcinvoke (D25_actionpb *) ;
extern signed32 d25_ipcwaitin (D25_actionpb *) ;
extern signed32 d25_ipcresult (D25_actionpb *) ;
extern signed32 d25_ipwaitrs (D25_actionpb *) ;
extern signed32 d25_ipretbuf (D25_actionpb *) ;
extern signed32 d25_ipcabort (D25_actionpb *) ;
extern signed32 d25_regproc (D25_actionpb *) ;
extern signed32 d25_unregproc (D25_actionpb *) ;
extern signed32 d25_ipcevent (D25_eventpb *) ;
extern signed32 d25_rosevent (D25_eventpb *, R_evtpb *) ;
extern void d25_signal (int) ;
extern signed32 d25_getpsap (D25_actionpb *) ;
extern signed32 d25_getres (D25_actionpb *) ;
extern signed32 d25_relres (D25_actionpb *) ;
extern signed32 d25_checkres (D25_actionpb *) ;
extern signed32 d25_relbuf (D25_actionpb *) ;
extern signed32 d25_rosattach (D25_actionpb *) ;
extern signed32 d25_rosdetach (D25_actionpb *) ;
extern signed32 d25_rosbinrq (D25_actionpb *) ;
extern signed32 d25_robinrs (D25_actionpb *) ;
extern signed32 d25_rosubnrq (D25_actionpb *) ;
extern signed32 d25_roubnrs (D25_actionpb *) ;
extern signed32 d25_rosinvrq (D25_actionpb *) ;
extern signed32 d25_rosresrq (D25_actionpb *) ;
extern signed32 d25_roerrrq (D25_actionpb *) ;
extern signed32 d25_rorjurq (D25_actionpb *) ;
extern signed32 d25_rosaborq (D25_actionpb *) ;
extern signed32 d25_rosprbf (D25_actionpb *) ;
extern signed32 d25_rostrrq (D25_actionpb *) ;
extern void d25_tstrace (signed32) ;
extern signed32 d25_exec_actions (D25_eventpb *) ;
#ifdef CSTUB
extern signed32 d25_initasn1 (D25_actionpb *) ;
extern signed32 d25_apdutoasn1 (D25_actionpb *) ;
extern signed32 d25_asn1toapdu (D25_actionpb *) ;
extern signed32 d25_101_save_invoke_data (D25_actionpb *) ;
extern signed32 d25_111_remove_cup_job (D25_actionpb *) ;
extern signed32 d25_121_save_result_data (D25_actionpb *) ; 
extern signed32 d25_141_restore_env (D25_actionpb *) ;
extern signed32 d25_151_continue_update (D25_actionpb *) ;
#endif

#endif /* _D25STUB_H */
