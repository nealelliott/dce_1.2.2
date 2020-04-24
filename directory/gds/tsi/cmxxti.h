/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cmxxti.h,v $
 * Revision 1.1.5.2  1996/03/11  13:24:37  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:12:59  marty]
 *
 * Revision 1.1.5.1  1995/12/08  17:49:06  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:25:48  root]
 * 
 * Revision 1.1.2.2  1994/06/10  21:17:50  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:54:58  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/10  16:15:46  marrek
 * 	Created for April Submission.
 * 	[1994/04/26  12:52:07  marrek]
 * 
 * $EndLog$
 */
#ifndef _CMXXTI_H
#define _CMXXTI_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char cmxxti_rcsid[] = "@(#)$RCSfile: cmxxti.h,v $ $Revision: 1.1.5.2 $ $Date: 1996/03/11 13:24:37 $";
#endif

/* #ident	"@(#)cmxxti.h	311.10 93/08/24 CMX (NC1)" */

/*
 * COPYRIGHT (C) 1990 BY SIEMENS AG All rights reserved
 * COPYRIGHT (C) Siemens Nixdorf Informationssysteme AG 1993
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
 * Header file for CMX-XTI/TLI-Module
 *
 */
#include "cmxxso.h"

#ifndef SIGIO
#define SIGIO 22
#endif

typedef int	bool;
#define FALSE	0
#define TRUE	1
typedef unsigned int	ui32;


/*
 * get/set application and connection index from/into tref
 *   3                       2                   1                 0
 *   1                       0                   0                 0
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |       |       |       |       |       |       |       |       |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  | T_XTI |               |           APP_IND |          CONN_IND |
 */
#define TX_CON_SHFT	10
#define TX_CON_MASK	((1 << TX_CON_SHFT) - 1)
#define TX_CON_MAX	(1 << TX_CON_SHFT)

#define TX_APP_SHFT	10
#define TX_APP_MASK	((1 << TX_APP_SHFT) - 1)
#define TX_APP_MAX	(1 << TX_APP_SHFT)

#define GET_APP_IND(t)	((*t >> TX_CON_SHFT) & TX_APP_MASK)
#define GET_CON_IND(t)	(*t & TX_CON_MASK)

#define SET_TREF(app_ind, con_ind) \
	(((app_ind) << TX_CON_SHFT) | (con_ind) | (T_XTI << 28))



/*
 * definitions for redirection from DIRX connections
 */
typedef struct {
   struct t_myname	myname;
   int			tsaplen;
   struct t_myname	tsap;
} tx_dirxbuf;

/*
 * Address and user data length
 *
 * Problem: If the buffers are too small XTI functions perform the actions
 * and only give one indication of this.Therefore the function call
 * cannot be repeated with corrected values.
 * The constants here define the maximum lengths used in CMX via
 * XTI/TLI.
 * TBUFOVFLW behavior:
 * With t-lists and t-rcvconnect the connection is once again closed:
 * With t-disin the mistake is ignored. Better possible solution:
 * With Reason = T_USER another reason could be given.
 */
/* see also ML_ADDR defined in cmxxso.h */
#define ML_UDATA	T_MSG_SIZE
/*
 * some TP's need an Option buffer to return a sequence number or to
 * avoid error messages
 */
#define ML_OPT		256

typedef struct liststr_tag
{
   struct liststr_tag  *nxt;
   struct t_call	call;
   char			addr [ML_ADDR];
   char			udata[ML_UDATA];
} tx_liststr;

/*
 * structure per connection
 *
 * udata userdata for t_concf, t_redin and t_disin
 *	The buffer is created on demand. Size: ML_UDATA.
 *      User data for t_conin, t_redin, and t-disin
 *      already accepted in t_event.
 *      At this stage with TBUFOVFLW
 *      (i.e. the buffer was too small, data was thrown away, function
 *      however was successfully successfully) accepted with T_DISIN,
 *      otherwise the connection is closed down.
 *
 *      With T_CONCF, T_REDIN, T_DISIN und with self generated T_DISINs
 *      data is always correctly inserted; released with t_concf, t_redin,
 *      t_rcvdis, and by removing the connection block.
 *	udata.buf == NULL indicates no buffer there.
 *	udata.len = length of userdata, = 0 when udata.buf == NULL
 */
typedef struct tx_con_tag
{
	int              fd;    /* fd for this XTI/TLI transport endpoint*/
	int              ind;   /* connection index                      */
	ui32             state; /* connection state                      */
/*
 * The numeric order is important, don't touch
 */
#define SX_NONEX	0
#define	SX_OPEN		1
#define	SX_BOUND	2
#define	SX_OUTC_C	4       /* conrq initiated, wait remote conrs	*/
#define SX_OUTC_I	5       /* local T_CONCF or got T_CONNECT,	*/
				/* -> signal T_CONCF			*/
#define	SX_INCO_I	6	/* T_CONIN signalled, wait for t_conin	*/
#define	SX_DISI_I	7	/* -> signal T_DISIN			*/
#define	SX_REDIN_I	8	/* wait for t_redin			*/
#define SX_INRS_C	9	/* wait for t_conrs			*/


#define	SX_DATAXFER    10
#define	SX_DATAINP     11	/* t_datain in progress			*/

	int		 flwctrl;/* flow control state at user level	*/
#define FX_SNDNSTOP	0x01	/* T_DATASTOP				*/
#define FX_SNDXSTOP	0x02	/* T_XDATSTOP				*/
#define FX_SNDSTOP	(FX_SNDNSTOP | FX_SNDXSTOP)
#define	FX_RCVNSTOP	0x10	/* after t_datastop()			*/
#define FX_RCVXSTOP	0x20	/* after t_xdatstop()			*/
#define	FX_RCVSTOP	(FX_RCVNSTOP | FX_RCVXSTOP)
	struct t_info	 info;
	struct netbuf	 udata;	/* user data				*/
	int		 event;	/* actual event			*/
	int              elast; /* last event signaled to user  */
	int              reason;/* disconnect reason            */
	int		 mltidu;/* TPDU size as number		*/
	int              ucepid;/* users' reference of connection */
	tx_liststr      *listq; /* list element for t_accept    */
				/* ---------------------------- */
	int		 dcnt;	/* # of bytes to snd		*/
	int		 dflag;	/* chain for puffered data	*/
	char		*dbuf;	/* points to next byte to snd	*/
	char            *dmem;  /* memory area for outgoing data*/
/*
 * Pointer to next tx_con
 */
	struct tx_con_tag *nxt;	/* ptr to next tx_con		*/
	struct tx_con_tag *prv;	/* ptr to previous tx_con	*/
} tx_con;
#define NULLCP (tx_con *)NULL

/*
 * structure per application
 */
typedef struct tx_app_tag
{
	int		ind;	/* application index			*/
	int		tmn;	/* tmyname-id				*/
	int		apmode;	/* modes of connection establishment	*/
	int		conlim;	/* max actual possible new connections	*/
	int		uattid;	/* users' ref. of the application	*/
	int		tpflag;	/* TP-specific flags (see cmxxso.h)	*/
	tx_reason      *maprea;	/* Reason mapping table			*/
	tx_con	       *conp;	/* list of connection structures	*/
	tx_con	       *pconp;	/* one listening con. T_PASSIV		*/
	char           *tp;     /* Transport provider                   */
	int		atyp;	/* ATYP_TNS or ATYP_DIRX		*/
#define ATYP_TNS	(0)
#define ATYP_DIRX	(1)
	union cx_unisel	tsel;	/* local TSEL				*/
	struct netbuf	addr;	/* bind-address for TLI or XTI		*/
	struct tx_app_tag *nxt;	/* ptr to next tx_app			*/
	struct tx_app_tag *prv;	/* ptr to previous tx_app		*/
} tx_app;
#define NULLAP (tx_app *)NULL

/*
 * the user expects in case of an t_conin the original t_myname.
 * Internaly we have an application per TSEL, to deliver the original
 * address and also attach-id, tx_tmn was created.
 */
typedef struct tx_tmn_tag
{
   int			 ind;
   int			 cnt;
   struct t_myname	 myname;
   struct tx_tmn_tag 	*nxt;	/* ptr to next tx_tmn			*/
   struct tx_tmn_tag 	*prv;	/* ptr to previous tx_tmn		*/
} tx_tmn;
#define	NO_TMN	-2

/*
 * structure for pending t_datain
 *
 * dcnt == 0 means the buffer is empty.
 */
typedef struct
{
   int	        dcnt; 		/* # of bytes left to deliver		*/
   int		dflag;		/* chain for puffered data		*/
   ui32		tref;
   char	       *dbuf;		/* points to next byte to deliver	*/
   char        *dmem;           /* memory area for incoming data        */
} tx_datain;

/*
 * structure per process
 */
typedef struct
{
   ui32		 mypid;
   ui32		 nr_att;	/* # attached applications in process	*/
   tx_app 	*apptr;		/* list of application structures	*/
   ui32		 nr_tmn;	/* # tmyname elements in use		*/
   tx_tmn	*tmnptr;	/* list of tmyname structures		*/
   tx_datain     datain;        /* Receive buffer                                */
} tx_proc;


#ifdef CMXXTI_C
#ifndef __TURBOC__
#define t_attach	t_500attach
#define t_detach	t_501detach
#define t_conrq		t_502conrq
#define t_conin		t_503conin
#define t_conrs		t_504conrs
#define t_concf		t_505concf
#define t_datarq	t_506datarq
#define t_datain	t_507datain
#define t_datago	t_508datago
#define t_datastop	t_509datastop
#define t_redrq		t_510redrq
#define t_redin		t_511redin
#define t_xdatrq	t_512xdatrq
#define t_xdatin	t_513xdatin
#define t_xdatgo	t_514xdatgo
#define t_xdatstop	t_515xdatstop
#define t_disrq		t_516disrq
#define t_disin		t_517disin
#define t_event		t_518event
/* #define t_error		t_519error */
#define t_info		t_520info
#define t_vdatarq	t_521vdatarq
#define t_vdatain	t_522vdatain
#endif	/* __TURBOC__ */
#endif  /* CMXXTI_C */

/* cmxxad.c, see also in cmxxso.h */ 
int tx_amsel	(union cx_unisel	*pv_tsel,
		 struct netbuf		*pv_addr,
		 char  		       **pv_tp,
		 int	 		 pv_pid,
		 ui8			 pv_mode);
void tx_tpinfo	(int			*pv_flag,
		 tx_reason	       **pv_rtab);
int tx_amaddr	(unsigned short		 pv_type,
		 struct	netbuf		*pv_buf,
		 struct t_partaddr	*pv_paddr,
		 int			 pv_pid);
void tx_bui_paddr(unsigned short	 pv_type,
		  struct t_partaddr	*pv_fromaddr,
		  struct netbuf		*pv_buf);

/* cmxxup.c xti-functions*/
int tx_accept	(tx_app		*pv_app,
		 tx_con		*pv_con);
int tx_bind	(tx_app		*pv_app,
		 tx_con		*pv_con,
		 unsigned	 pv_qlen);
int tx_close	(int		 pv_fd);
int tx_connect	(tx_con		*pv_con,
		 struct t_call	*pv_sndcall);
int tx_getstate	(int		 pv_fd);
int tx_listen	(tx_app		*pv_app);
int tx_look	(int		 pv_fd);
int tx_maperror	(int		 pv_terrno,
		 int		 pv_errno);
int tx_open	(tx_app		*pv_app,
		 tx_con		*pv_con);
int tx_rcv	(tx_con		*pv_con,
		 tx_datain	*pv_data);
int tx_rcvconnect(tx_con	*pv_con);
int tx_rcvdis	(tx_con		*pv_con);
int tx_rcvdisl	(tx_app		*pv_app);
int tx_rcvrel	(tx_con		*pv_con);
int tx_snd	(tx_con		*pv_con,
		 char		*pv_buf,
		 unsigned	 pv_nbytes,
		 int		 pv_flags);
int tx_snddis	(tx_con		*pv_con,
		 struct t_call	*pv_call);
int tx_snddisl	(tx_app		*pv_app,
		 struct t_call	*pv_call);
int tx_unbind	(tx_con		*pv_con);
int tx_sync	(int		 pv_fd);

/* cmxxrd.c redirection */
int tx_redrq	(tx_con		*pv_conp,
		 struct netbuf	*pv_ladr,
		 int		 pv_pid,
		 int		 pv_timeout,
		 char	        *pv_udatap,
		 int		 pv_udatal);
int tx_redopen	(void);
void tx_redclose(void);
int tx_redevent (struct netbuf	*pv_ladr);
int tx_redin    (tx_con		*pv_conp,
		 int		 pv_ack);
#endif /* _CMXXTI_H */
