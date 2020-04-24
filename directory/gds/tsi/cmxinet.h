/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cmxinet.h,v $
 * Revision 1.1.5.2  1996/03/11  13:24:15  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:12:40  marty]
 *
 * Revision 1.1.5.1  1995/12/08  17:47:36  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:25:13  root]
 * 
 * Revision 1.1.2.2  1994/06/10  21:17:15  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:54:34  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/10  16:15:18  marrek
 * 	Created for April Submission.
 * 	[1994/04/26  12:50:53  marrek]
 * 
 * $EndLog$
 */
#ifndef _CMXINET_H
#define _CMXINET_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char cmxinet_rcsid[] = "@(#)$RCSfile: cmxinet.h,v $ $Revision: 1.1.5.2 $ $Date: 1996/03/11 13:24:15 $";
#endif

/* #ident " @(#)cmxinet.h	311.18 93/08/24 CMX (NC1)" */
/*
 * COPYRIGHT (C) 1990 BY SIEMENS AG All rights reserved
 * COPYRIGHT (C) Siemens Nixdorf Informationssysteme AG 1992
 *               All Rights Reserved
 *  
 * This software is furnished under a license and may be used
 * only in accordance with the terms of that license and with the
 * inclusion of the above copyright notice. This software may not
 * be provided or otherwise made available to, or used by, any
 * other person. No title to or ownership of the software is
 * hereby transferred.
 */

/*
 * Header File for INTERNET-Module
 *
 */

/*
 * most of the definitions are only needed in cmxinet.c and cmxinup.c
 * to get it, you must define CMXINET_INTERN
 */
#ifdef	CMXINET_INTERN

#ifndef SIGIO
#define SIGIO 22
#endif

#define MIN(a,b) (((a) > (b)) ? (b) : (a))

#define FALSE	0
#define TRUE	1

#define tc_p		t_400cb
#define tc_mxtpdu	tc_mxtidu
#define tc_gethisaddr	t_400gha
#define tc_srch_app	t_400sa
#define	tc_get_apptr	t_400ga
#define tc_get_conptr	t_400gc

#define tc_attach	t_400attach
#define tc_detach	t_401detach
#define tc_conrq	t_402conrq
#define tc_conin	t_403conin
#define tc_conrs	t_404conrs
#define tc_concf	t_405concf
#define tc_datarq	t_406datarq
#define tc_datain	t_407datain
#define tc_datago	t_408datago
#define tc_datastop	t_409datastop
#define tc_redrq	t_410redrq
#define tc_redin	t_411redin
#define tc_xdatrq	t_412xdatrq
#define tc_xdatin	t_413xdatin
#define tc_xdatgo	t_414xdatgo
#define tc_xdatstop	t_415xdatstop
#define tc_disrq	t_416disrq
#define tc_disin	t_417disin
#define tc_event	t_418event
#define tc_error	t_419error
#define tc_info		t_420info
#define tc_vdatarq	t_421vdatarq
#define tc_vdatain	t_422vdatain

/*
 * Defines for blocking/non-blocking mode
 */
#define TC_BLOCK 	1	/* socket in blocking mode */
#define TC_NBLOCK	2	/* socket in non-blocking mode */
/*
 * Convergence protocol as of RFC 1006
 */
#define	TC_HDRLEN	6	/* length received from tc_event() */
#define	TC_VRSN		3	/* version */
#define	TC_RSVD		0	/* reserved */

#define	TC_CR_FIX	17	/* fixed length of li in CR/DR/DT/RR/AK TPDU */
#define	TC_DR_FIX	6
#define	TC_DT_FIX	2
#define	TC_RR_FIX	17
#define TC_AK_FIX	4

#define TC_DTPDU_PRE	7	/* header data of an data TPDU */

#define	TC_TP0		0x00	/* transport class */

#define TC_PAR_OFF	5	/* parameter offset in CR/CC/DR TPDU */
#define	TC_PAR_TPDULEN	0xc0	/* parameter types */
#define	TC_PAR_CALLING	0xc1
#define	TC_PAR_CALLED	0xc2

#define	LINTMPORT	0x0f
#define	LINTOPORT	0x0d

#endif /* CMXINET_INTERN */

#define	TC_TPDU_4096	0x0c	/* parameter values for TC_PAR_TPDULEN */
#define	TC_TPDU_2048	0x0b
#define	TC_TPDU_1024	0x0a
#define	TC_TPDU_512	0x09
#define	TC_TPDU_256	0x08
#define	TC_TPDU_128	0x07

#define MAP_TPDU(code)	\
	(code == TC_TPDU_4096 ? 4096 :	\
	code == TC_TPDU_2048 ? 2048 :	\
	code == TC_TPDU_512 ? 512 :	\
	code == TC_TPDU_256 ? 256 :	\
	code == TC_TPDU_128 ? 128 : 1024)

#define MAP_CREDIT(tpdu)	\
	(tpdu == TC_TPDU_4096 ? 1 :	\
	tpdu == TC_TPDU_2048 ? 2 :	\
	tpdu == TC_TPDU_512 ? 8 :	\
	tpdu == TC_TPDU_256 ? 16 :	\
	tpdu == TC_TPDU_128 ? 32 : 4)

#ifdef	CMXINET_INTERN
#define	TC_MORE		0x00	/* TP0: MORE */
#define	TC_END		0x80	/* TP0: END */

#define	CONRQ   	0xe0	/* send connection request */
#define CONIN		CONRQ	/* receive connection request */
#define CONCF   	0xd0	/* send connection confirmation */
#define CONRS   	CONCF	/* receive connection confirmation */

#define DISRQ   	0x80	/* send disconnect request */
#define DISIN   	DISRQ	/* receive disconnect request */

#define DATRQ		0xf0	/* send data request */
#define DATIN		DATRQ	/* receive data request */
#define ACKRQ		0x60	/* send acknowledgement request */
#define ACKIN		ACKRQ	/* receive acknowledgement request */

#define REDRQ		0x05	/* send redirect request */
#define REDIN   	REDRQ	/* receive redirect request */

#define MAP_CODE(c)	\
	(((c) == CONIN || (c) == CONCF || (c) == DISIN || (c) == REDIN \
		|| (c) == DATIN || (c) == ACKIN) ? 0 : 1)

/*
 * address and name related definitions
 */
#define LOCALHOST	0x7f000001
#define CX_INADDR_OFF 	10

#define MIN_PORT	1
#define MAX_PORT	32767
#define MAXLISTEN	32	/* backlog for incoming conns in listen() */

/*
 * get/set application and connection index from/into tref
 *   3                       2                   1                 0
 *   1                       0                   0                 0
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |       |       |       |       |       |       |       |       |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  | T_INET|               |           APP_IND |          CONN_IND |
 */
#define TC_CON_SHFT	10
#define TC_CON_MASK	((1 << TC_CON_SHFT) - 1)
#define TC_CON_MAX	(1 << TC_CON_SHFT)

#define TC_APP_SHFT	10
#define TC_APP_MASK	((1 << TC_APP_SHFT) - 1)
#define TC_APP_MAX	(1 << TC_APP_SHFT)

#define T_INET		3

#define GET_APP_IND(t)	((*t >> TC_CON_SHFT) & TC_APP_MASK)
#define GET_CON_IND(t)	(*t & TC_CON_MASK)

#define SET_TREF(app_ind, con_ind) \
	(((app_ind) << TC_CON_SHFT) | (con_ind) | (T_INET << 28))

#define SET_SRCREF(app_ind, con_ind) \
	(( u_short ) (0xffff & (((app_ind) << TC_CON_SHFT) | (con_ind))))
/*
 * definitions for select()
 */
#define NULLTP 		((struct timeval *) NULL)
#define READP   	1	/* to identify R/W/E event from select() */
#define WRITEP  	2
#define EXCEPTP 	4

/*
 * structure per process
 */
struct tc_proc
{
	int	tc_nr_att;		/* # attached applications in process */
	struct {			/* per process flags */
		unsigned int	tc_mypid : 16;
		unsigned int	tc_nfound : 8;
		unsigned int	tc_enabsig : 1;
	} tc_flags;		
	struct tc_app *tc_apptr;	/* list of application structures */
};

/*
 * structure per application
 */
struct tc_app
{
	struct t_myname tc_name;	/* LOCAL NAME of this application */
	int	tc_ind;			/* application index */
	int	tc_portnr;		/* port for passive connections */
	int	tc_apmode;		/* modes of connection establishment */
	int	tc_conlim;		/* max # of connections */
	int	tc_uattid;		/* users' ref. of the application */
	int	tc_nr_conn;		/* # of established connections */
	struct tc_con *tc_conp;		/* list of connection structures */
	struct tc_con *tc_pconp;	/* to receive passive conns. */
	struct tc_con *tc_rconp;	/* to receiving redirected conns. */
	struct tc_app *tc_prv;		/* ptr to previous tc_app */
	struct tc_app *tc_nxt;		/* ptr to next tc_app */
#define NULLAP (struct tc_app *)NULL
};

/*
 * structure per connection
 */
struct tc_con
{
	struct t_partaddr tc_addr;	/* peer's TRANSPORT ADDRESS */
	struct in_addr	tc_inaddr;	/* peer's INTERNET address */
	int	tc_ind;			/* connection index */
	u_short	tc_portnr;		/* peer's port */
	u_short	tc_tcalledlen;		/* peer's TSEL Length (called)   */
	char	tc_tseled[32];		/* peer's TSEL (called TSAP-ID)  */
	u_short	tc_tcallnglen;		/* peer's TSEL Length (calling)  */
	char	tc_tselng[32];		/* peer's TSEL (calling TSAP-ID) */
	int	tc_ucepid;		/* users' reference of the connection */
	int	tc_socket_fd;		/* socket fd for this CMX connection */
	int	tc_xdata;		/* xdatas: not implemented yet */
	int	tc_mxtidu;		/* TPDU size as number (not code) */
	int	tc_state;		/* connection state */
#define T_IDLE		1	/* idle */
#define T_OUTCONNECT	19	/* outgoing connection pending [t_conrq()] */
#define T_ACCEPT	18	/* wait for CR-TPDU [t_event()] */
#define T_OUTCONPEND	2	/* outgoing connection pending [t_event()] */
#define T_OUTCONCONF	21	/* outgoing connection confirmed [T_CONCF] */
#define T_INCONPEND	3	/* outgoing connection pending [t_conin()] */
#define T_INCONIND	30	/* outgoing connection pending [T_CONIN] */
#define T_DATTRFRDY	4	/* data transfer ready */
#define T_REDCONIND	40	/* redirected connection indicated [T_REDIN] */
#define T_DISCONIND	41	/* disconnect indication indicated [T_DISIN] */
#define T_LOCFLOWCON	5	/* local flow control for write activ */

	int tc_flcontr;			/* flow control state at user level */
					/* send states */
#define T_SNDGO		0x01		/* ground */
#define T_SNDSTOP	0x02		/* T_DATASTOP */
					/* receive states */
#define T_RCVGO		0x10		/* ground */
#define T_RCVIND	0x20		/* T_DATAIN received */
#define T_RCVEVT	0x40		/* T_NOEVENT received */
#define T_RCVSTOP	0x80		/* tc_datastop() called */

	struct {			/* flow control at protocol level */
		unsigned int	enab   : 1;	/* 1: is enabled */
		unsigned int	credit : 6;	/* credit */
		unsigned int	tpdunr : 7;	/* tpdu-# to send (mod 128) */
		unsigned int	yrtunr : 7;	/* next tpdu-# exp. (mod 128) */
		unsigned int	lowe   : 7;	/* lower window edge */
		unsigned int	upwe   : 7;	/* upper window edge */
	} tc_flow;		

/*
 * Type of partner-CMX; necessary for migration
 */
	int	tc_partner;		/* Version of Partner RFC1006 */

#define RFCV25		0x25	/* partner is CMX V2.5 */
#define RFCV30		0x30	/* partner is CMX V3.0 */
#define RFCV40		0x40	/* partner is CMX V4.0 */


	short	tc_event;		/* actual event */
	short	tc_evtind;		/* actual event indicated */
	int	tc_reason;		/* disconnect-reason */
	int	tc_optdatlng;		/* length of user data at t_conrq */
	char	*tc_optdat;		/* user data at t_conrq */
	int	tc_length;		/* # of bytes to read */
	int	tc_udatal;		/* # of bytes user data left to read */
	int	tc_chain;		/* MORE/END indicator for TIDU */
	u_short	tc_dstref;		/* Destination reference */
	u_short	tc_srcref;		/* Source reference */
/*
 * Data structures for local flow control
 */
	char	*savoutdata;		/* Memory area for (write) */
	struct iovec	fc_write[1];	/* I/O vector of write */
	char	*savindata;		/* Memory area for (read) */
	struct iovec	fc_read[1];	/* I/O vector of read */
	int	read_state;		/* State of TPDU read */
#define	R_IDLE	1			/* Nothing read */
#define R_INHEADER	2		/* Part of header read */
#define R_ENDHEADER	3		/* Header completely read */
#define	R_INDATA	4		/* Part of TPDU read */
#define R_ENDDATA	1		/* TPDU completely read = R_IDLE */
	int    	tpdu_code;		/* type of TPDU read */
	int	read_size;		/* size of DT-TPDU */
/*
 * Pointer to previous/next tc_con
 */
	struct tc_con	*tc_prv;	/* ptr to previous tc_con */
	struct tc_con	*tc_nxt;	/* ptr to next tc_con */
#define NULLCP (struct tc_con *)NULL
};

struct du_buf {
	int	state;			/* state of TIDU buffer */
#define NOBUF	0			/* TIDU buffer not allocated */
#define EVENT	1			/* TIDU indicated */
#define FULL	2			/* TIDU is in TIDU buffer */
	int	attid;			/* id of CMX application */
	int	tref;			/* dto. tref of connection */
	int	cnt;			/* length of data in TIDU buffer */
	char	*sptr;			/* begin of TIDU buffer */
	char	*aptr;			/* current read pointer */
	int	chain;			/* end-of-TSDU indicator (chain) */
};

/*
 * macros for flow control
 */
#define SMASK	0x0f	/* mask for 'send data' automaton */
#define EMASK	0xf0	/* mask for 'receive data' automaton */

#define FLC_INI(flcbit) \
	{ \
		FLC_SET(flcbit, T_SNDGO); \
		FLC_SET(flcbit, T_RCVGO); \
	}

#define FLC_CHK(flcbit, STATE) (flcbit & STATE)

#define FLC_SET(flcbit, STATE) \
	{ \
		flcbit &= ((STATE & SMASK) ? EMASK : SMASK); \
		flcbit |= STATE; \
	}


/*
 * Prototypes for functions in cmxinup.c called by cmxinet.c
 */
extern u_short         portnrsav;
extern struct tc_proc  tc_p;

extern int tc_listen (int mode, struct tc_app *app);
extern int tc_enabsig (struct tc_con *conp);
extern int tc_read_data (struct tc_con *conp, int amount);
extern int tc_writev (struct tc_con *conp, int sd, struct iovec *iov,
		      int cnt, int n);
extern int tc_sndfrm (int code, struct tc_con *conp, ...);
extern int tc_rcvhdr (struct tc_con *conp, int *code);
extern int tc_rcvconincf (int code, struct tc_con *conp, t_opt1 *opt);
extern int tc_rcvdisin (struct tc_con *conp, int *hreasptr, t_opt2 *opt);
extern int tc_rcvdatin (struct tc_con *conp, struct iovec *piov,
			unsigned int hvcnt, unsigned int hcnt);
extern int tc_rcvredin (struct tc_con *conp, int *hpid, t_opt23 *opt);
extern int tc_rcvackin (struct tc_con *conp);
extern int tc_getmyport (struct t_myname *name);
extern void tc_getmytsel (struct tc_app	*appl, struct tc_con *conp);
extern void tc_gethisport (struct t_partaddr *paddr, struct tc_con *hconp);
extern char *tc_gethisaddr (struct t_partaddr *paddr);
extern void tc_sethisaddr (struct tc_con *conp);
extern void tc_rd_free (struct tc_app *app);
extern void tc_con_ins (struct tc_app *app, struct tc_con *insptr);
extern void tc_con_del (struct tc_app *app, struct tc_con *delptr);
extern struct tc_con *tc_get_conptr (struct tc_app *app, int ind);
extern struct tc_app *tc_srch_app (int port);
extern struct tc_app *tc_get_apptr (int ind);
extern void tc_app_ins (struct tc_app *insptr);
extern void tc_app_free (struct tc_app *aptr, int closeit, int discon);
extern void tc_setsock (int fd,int mode);

#endif /* CMXINET_INTERN */
#endif /* _CMXINET_H */
