/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cmxtrace.h,v $
 * Revision 1.1.4.2  1996/03/11  13:24:30  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:12:53  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:48:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:25:37  root]
 * 
 * Revision 1.1.2.3  1994/07/18  13:40:48  marrek
 * 	Changes for threadsafe libxti.
 * 	[1994/07/18  12:11:58  marrek]
 * 
 * Revision 1.1.2.2  1994/06/10  21:17:39  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:54:50  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/10  16:15:36  marrek
 * 	Created for April Submission.
 * 	[1994/04/26  12:51:40  marrek]
 * 
 * $EndLog$
 */
#ifndef _CMXTRACE_H
#define _CMXTRACE_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char cmxtrace_rcsid[] = "@(#)$RCSfile: cmxtrace.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:24:30 $";
#endif

/* #ident " @(#)cmxtrace.h	311.25 93/08/24 CMX (NC1)" */
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

#include <sys/utsname.h>

/*
 *
 * Definition for the trace in the CMX library
 */

/*
 * DEBUG options
 */
#ifdef DEBUG
#define FPRINTF(stderr, text, arg) fprintf(stderr, text, arg)
#else
#define FPRINTF(stderr, text, arg)
#endif

/*
 * For the data stream TRACE of the station connection
 */
#ifndef NULL
#define NULL    0               /* as stdio.h */
#endif

#define LF	0x0a
#define CR	0x0d

/*
 * For the trace in the case of the station connection
 */
#define T_TRHEADER      35      /* Trace header */
#define T_TRENTLNG      150     /* Maximum length per entry*/
#define T_TRMAXENT      100     /* Maximum number of entries*/
#define T_TRCMXO        "/usr/tmp/CMXO" /* Output to system*/
#define T_TRCMXI        "/usr/tmp/CMXI" /* Entries in library*/
#define T_TRCMXE        "/usr/tmp/CMXE" /* for t_event's */

#define T_VERSION       "/opt/lib/cmx/version" /* File with version */
#define T_VER_SIZE	80

/*
 * For the trace of ICMX(L)
 */
#define T_TRCDIR	"/usr/tmp"
#define T_TRCDIR_SIZE   128             /* Max. size of the directory names*/
#define T_TRCMXL        "/CMXLaXXXXX"   /* Primary file*/
#define T_TRCMXM        "/CMXMaXXXXX"   /* Alternative file*/

struct t_trcflag {
	unsigned int    deb   : 2;      /* Special debug*/
#define T_NODEBUG	0
#define T_LIBDEBUG	1
#define T_TCPDEBUG	2
	unsigned int    sel   : 3;      /* trace selector, no default */
#define	T_TRCHECKEV	0
#define	T_TRVERBOSE	1
#define	T_TRCOMPACT	2
#define	T_TRDONOT	3
	unsigned int    async : 1;      /* Async. trace entry output*/
};

#define T_TRV		1
#define T_TRX		2
#define T_TRDFLT	0
#define T_TRNDFLT	1
#define T_TRFLSH	0
#define T_TRNFLSH	1
#define T_TRSTART	3

#define T_TRLEN         0       /* Standard length for data*/
#define T_TRFAC         8       /* Factor for buffering*/
#define T_TRWRAP        (512*BUFSIZ) /* Default for wrap around */

/*
 * Definitions of ICMX(L) function names
 */
#define T_FATTACH	0
#define T_FDETACH	1
#define T_FCONRQ	2
#define T_FCONIN	3
#define T_FCONRS	4
#define T_FCONCF	5
#define T_FDATARQ	6
#define T_FDATAIN	7
#define T_FDATAGO	8
#define T_FDATASTOP	9
#define T_FREDRQ	10
#define T_FREDIN	11
#define T_FXDATRQ	12
#define	T_FXDATIN	13
#define T_FXDATGO	14
#define T_FXDATSTOP	15
#define T_FDISRQ	16
#define T_FDISIN	17
#define T_FEVENT	18
#define T_FERROR	19
#define T_FINFO		20
#define T_FINFO3        66 /* as of V 3.1*/

#define T_DATTACH	21
#define T_DDETACH	22
#define T_DEVENT 	23

#define T_FVDATARQ	24
#define T_FVDATAIN	25
#define T_FGETLOC	26
#define T_FGETADDR	27
#define T_FGETNAME	28
#define T_FRETATT	29

#define T_FFIRST	30	/* CMX V2.2 */
#define T_FRETERR	31
#define T_FRETOK 	32
#define T_FDATARQS	33
#define T_FDATAINS	34
#define T_FXDATRQS	35
#define	T_FXDATINS	36
#define T_DRETERR	37
#define T_DRETOK 	38
#define T_FRETEVT	39

#define T_FTCPPKT	40
#define T_FTCPSCK	41
#define T_FTCPBIN	42
#define T_FTCPCON	43
#define T_FTCPLIS	44
#define T_FTCPACC	45
#define T_FTCPSEL	46
#define T_FTCPHDR	47
#define T_FTCPBDY	48
#define T_FTCPEOT	49

#define T_FSIGIO	50
#define T_FSIGTERM	51

#define T_FOPEN		52
#define T_FREAD		53
#define T_FWRITE	54
#define T_FIOCTL	55
#define T_FCLOSE	56

#define T_FTUIIN	57
#define T_FTUIOUT	58

#define T_FFCNTL	59

#define	T_FSTRING	60
#define	T_FSNAPIPE	61

#define T_FMKNOD	62
#define T_FUNLINK	63

#define T_FTCPGSOPT     64
#define T_FTCPSSOPT     65

#define T_FSETOPT       67
#define T_FCALLBACK     68
#define T_FCBRES        69

#define T_DLERRO	70
#define T_DLERRS	71

#define T_UNAME		72

/* 
 * defines necessary for CMX on top of XTI/TLI
 */
#define	T_XTI_ACCEPT		100
#define	T_XTI_ACCEPT_FIN	101
#define	T_XTI_ALLOC		102	/* not used */
#define	T_XTI_ALLOC_FIN		103	/* not used */
#define	T_XTI_BIND		104
#define	T_XTI_BIND_FIN		105
#define	T_XTI_CLOSE		106
#define	T_XTI_CLOSE_FIN		107
#define	T_XTI_CONNECT		108
#define	T_XTI_CONNECT_FIN	109
#define	T_XTI_FREE		110	/* not used */
#define	T_XTI_FREE_FIN		111	/* not used */
#define	T_XTI_GETINFO		112	/* not used */
#define	T_XTI_GETINFO_FIN	113	/* not used */
#define	T_XTI_GETSTATE		114
#define	T_XTI_GETSTATE_FIN	115
#define	T_XTI_LISTEN		116
#define	T_XTI_LISTEN_FIN	117
#define	T_XTI_LOOK		118
#define	T_XTI_LOOK_FIN		119
#define	T_XTI_OPEN		120
#define	T_XTI_OPEN_FIN		121
#define	T_XTI_RCV		122
#define	T_XTI_RCV_FIN		123
#define	T_XTI_RCVCONNECT	124
#define	T_XTI_RCVCONNECT_FIN	125
#define	T_XTI_RCVDIS		126
#define	T_XTI_RCVDIS_FIN	127
#define	T_XTI_RCVREL		128
#define	T_XTI_RCVREL_FIN	129
#define	T_XTI_SND		130
#define	T_XTI_SND_FIN		131
#define	T_XTI_SNDDIS		132
#define	T_XTI_SNDDIS_FIN	133
#define	T_XTI_SNDREL		134
#define	T_XTI_SNDREL_FIN	135
#define	T_XTI_SYNC		136
#define	T_XTI_SYNC_FIN		137
#define	T_XTI_UNBIND		138
#define	T_XTI_UNBIND_FIN	139

#define T_STRING                199

struct xti_ret	{
	int	retval;
	int	xti_errno;
	int	cmx_errno;
};

/*
 * Format of function specific trace entries
 *
 * In logon and connection setup, which rarely occurs, local names and
 * transport addresses are always listed.
 */
struct t_trcentry {
	/* Header */
	short	id;
	short	info;
	short	relsec;
	short	milsec;
	/* id-specific entries */
	union U {
		/*------------------------------*
		 * Start entry
		 *------------------------------*/
		struct U_first {
			int	len;
			long	abs;
		} U_first;

		struct U_first2 {
			int	len;
			long	abs;
			char 	version;
			int	fdsetsize;
			int  	msgsize;
		} U_first2;

		/*------------------------------*
		 * ICMX(L) calls
		 *------------------------------*/
		struct U_attach {
			char	*name;
			struct t_opta2 *opt;
			int	t_optnr;
			int	t_apmode;
			int	t_conlim;
			struct t_myname t_mn;
		} U_attach;

		struct U_retatt {
			int	t_optnr;
			int	t_uattid;
			int	t_attid;
			int	t_ccbits;
			int	t_sptypes;
			int	retval;
			struct t_cclst t_cc;
		} U_retatt;

		struct U_detach {
			char	*name;
			struct t_myname	t_mn;
		} U_detach;

		struct U_conrq {
			int	*tref;
			struct t_partaddr *toaddr;
			struct t_myname *fromaddr;
			struct t_optc3 *opt;
			int	t_optnr;
			char	*t_udatap;
			int	t_udatal;
			int	t_xdata;
			int	t_timeout;
			int	t_ucepid;
			struct t_partaddr t_pa;
			struct t_myname t_mn;
			char	udata[T_MSG_SIZE];
		} U_conrq;

		struct U_concf {
			int	tref;
			struct t_optc1 *opt;
			int	t_optnr;
			char	*t_udatap;
			int	t_udatal;
			int	t_xdata;
			int	t_timeout;
			int	udatal;
			char	udata[T_MSG_SIZE];
		} U_concf;

		struct U_conin {
			int	tref;
			struct t_myname *toaddr;
			struct t_partaddr *fromaddr;
			struct t_optc1 *opt;
			int	t_optnr;
			char	*t_udatap;
			int	t_udatal;
			int	t_xdata;
			int	t_timeout;
			struct t_partaddr t_pa;
			struct t_myname t_mn;
			int	udatal;
			char	udata[T_MSG_SIZE];
		} U_conin;

		struct U_conrs {
			int	tref;
			struct t_optc3 *opt;
			int	t_optnr;
			char	*t_udatap;
			int	t_udatal;
			int	t_xdata;
			int	t_timeout;
			int	t_ucepid;
			char	udata[T_MSG_SIZE];
		} U_conrs;

		struct U_disrq {
			int	tref;
			struct t_optc2 *opt;
			int	t_optnr;
			char	*t_udatap;
			int	t_udatal;
			char	udata[T_MSG_SIZE];
		} U_disrq;

		struct U_disin {
			int	tref;
			int	reason;
			struct t_optc2 *opt;
			int	t_optnr;
			char	*t_udatap;
			int	t_udatal;
			int	udatal;
			char	udata[T_MSG_SIZE];
		} U_disin;

		struct U_redin {
			int	tref;
			int	pid;
			struct t_optc3 *opt;
			int	t_optnr;
			char	*t_udatap;
			int	t_udatal;
			int	t_xdata;
			int	t_timeout;
			int	t_ucepid;
			int	udatal;
			char	udata[T_MSG_SIZE];
		} U_redin;

		struct U_redrq {
			int	tref;
			int	pid;
			struct t_optc1 *opt;
			int	t_optnr;
			char	*t_udatap;
			int	t_udatal;
			int	t_xdata;
			int	t_timeout;
			char	udata[T_MSG_SIZE];
		} U_redrq;

		struct U_datarq {
			int	tref;
			char	*datap;
			int	datal;
			int	chain;
			int	datacnt;
		} U_datarq;

		struct U_vdatarq {
			int	tref;
			struct t_data	*vdata;
			short	vcnt;
			short	chain;
		} U_vdatarq;

		struct U_datarqs {
			int	tref;
			char	*datap;
			int	datal;
			int	chain;
		} U_datarqs;

		struct U_datain {
			int	tref;
			char	*datap;
			int	datal;
			int	chain;
			int	datacnt;
		} U_datain;

		struct U_vdatain {
			int	tref;
			struct t_data	*vdata;
			short	vcnt;
			short	chain;
		} U_vdatain;

		struct U_datains {
			int	tref;
			char	*datap;
			int	datal;
			int	chain;
		} U_datains;

		struct U_xdatrq {
			int	tref;
			char	*datap;
			int	datal;
			char	data[T_EXP_SIZE];
		} U_xdatrq;

		struct U_xdatrqs {
			int	tref;
			char	*datap;
			int	datal;
		} U_xdatrqs;

		struct U_xdatin {
			int	tref;
			char	*datap;
			int	datal;
			char	data[T_EXP_SIZE];
		} U_xdatin;

		struct U_xdatins {
			int	tref;
			char	*datap;
			int	datal;
		} U_xdatins;

		struct U_flow {
			int	tref;
		} U_flow;

		struct U_event {
			int	tref;
			int	cmode;
			struct t_opte1	*opt;
			int	t_optnr;
			int	t_attid;
			int	t_uattid;
			int	t_ucepid;
			int	t_timeout;
			int	t_evdat;
		} U_event;

		struct U_info {
			int	tref;
			struct t_opti1 *opt;
			int	t_optnr;
			int	t_maxl;
		} U_info;

		struct U_info3 {
			int	tref;
			struct t_opti3 *opt;
			int	t_optnr;
			int	t_maxl;
			int 	t_fdsetsize;
			int 	t_openmax;
			int 	t_tsdu;
			int 	t_etsdu;
			int 	t_condata;
			int 	t_concfdata;
			int 	t_disdata;
			int 	t_adrsize;
		} U_info3;

		struct U_gaddr {
			char	*glob;
			char	*opt;
			short	lglob;
			struct t_partaddr *addr;
			struct t_partaddr t_pa;
		} U_gaddr;

		struct U_gname {
			struct t_partaddr *addr;
			char	*opt;
			char	*name;
			short	lname;
			struct t_partaddr t_pa;
		} U_gname;

		struct U_gloc {
			char	*glob;
			char	*opt;
			short	lglob;
			struct t_myname *loc;
			struct t_myname t_mn;
		} U_gloc;

		struct U_setopt {
			int	component;
			struct  t_opts1	*opt;
			int	t_optnr;
			int	t_optname;
			char    t_optvalue[T_TRCDIR_SIZE];
		} U_setopt;

		struct U_callback {
			int	routine;
			void	*usr;
			void	*opt;
		} U_callback;

		/*------------------------------*
		 * Return values ICMX(L) calls
		 *------------------------------*/
		struct U_reterr {
			int	error;
		} U_reterr;

		struct U_retok {
			int	tref;
			int	retval;
		} U_retok;

		struct U_devent {
			int	cmode;
		} U_devent;

		struct U_dlerr {
			int	umode;
			char	errarray[120];
		} U_dlerr;

		struct U_uname {
			struct utsname	unameres;
			char   version[T_VER_SIZE];
		} U_uname;

		/*---------------------------------------------*
		 * SVC trace, INET module and callback routine
		 *---------------------------------------------*/
		struct U_tcpsck {
			int	dom;
			int	type;
			int	prot;
			int	res;
			int	cmx_errno;
		} U_tcpsck;

		struct U_tcpsopt {
			int	socket;
			int	level;
			int	optname;
			int	optval;
			int	optlen;
			int	res;
			int	cmx_errno;
		} U_tcpsopt;

		struct U_tcplis {
			int	sd;
			int	back;
			int	res;
			int	cmx_errno;
		} U_tcplis;

		struct U_tcpabc {
			int	sd;
			char	name[16];
			int	res;
			int	cmx_errno;
		} U_tcpabc;

		struct U_tcpsel {
			int	nsd;
			fd_set	rfds;
			fd_set	wfds;
			fd_set	xfds;
			int	sec;
			int	res;
			int	cmx_errno;
		} U_tcpsel;

		struct U_fcbres {
			int	nsd;
			fd_set	rfds;
			fd_set	wfds;
			fd_set	xfds;
			int	sec;
			int	res;
			int	cmx_errno;
		} U_fcbres;

		struct U_open {
			char	path[16];
			int	mode;
			int	res;
			int	cmx_errno;
		} U_open;

		struct U_svc {
			int	fd;
			int	arg2;
			int	arg3;
			int	res;
			int	cmx_errno;
		} U_svc;

		struct U_string {
			char	*str;
		} U_string;

		struct U_snapipe {
			unsigned short len_ges;
			char  fkt_aufruf;
			char  p_fill;
			int   proz_id;
			int   transref;
			int   param;
			int   param1;
			int   param2;
			int   param3;
			int   param4;
		} U_snapipe;

#ifdef	CMXTRACE_XTI
#define	XMIN(A, B)	((A) > (B) ? (B) : (A))	/* less then 0 already checked */

/* <xti-function without returning parameters other than the function value */
/* <t_accept, <t_close, <t_getstate, <t_look, <t_rcvrel, <t_snd */
/* <t_snddis, <t_sndrel, <t_sync, <t_unbind */
		struct	{
			struct xti_ret	xret;
		} U_Xfin;

/* >xti-function only using a fd */
/* >t_close, >t_getstate, >t_look, >t_rcvrel, >t_sndrel, >t_sync, >t_unbind */
		struct	{
			int	fd;
		} U_Xfd;

/* >t_accept:	U_Xaccept, addr.buf, opt.buf, udata.buf */
		struct	{
			int		 fd;
			int		 resfd;
			struct t_call	*callp;
			struct t_call	 call;
		} U_Xaccept;

/* >t_bind: U_Xbind, req.addr.buf */
		struct	{
			int		fd;
			struct t_bind	*reqp;
			struct t_bind	 req;
			struct t_bind	*retp;
			struct t_bind	 ret;
		} U_Xbind;

/* <t_bind: U_Xbindfin, ret.addr.buf */
		struct	{
			struct t_bind	*retp;
			struct t_bind	 ret;
			struct xti_ret  xret;
		} U_Xbindfin;

/* >t_connect: U_Xconnect, sndcall.addr.buf, .opt.buf, .udata.buf */
		struct	{
			int		fd;
			struct t_call	*sndcallp;
			struct t_call	 sndcall;
			struct t_call	*rcvcallp;
			struct t_call	 rcvcall;
		} U_Xconnect;

/* >t_listen: U_Xcall */
/* >t_rcvconnect: U_Xcall */
/* >t_snddis: U_Xcall, call.udata.buf */
		struct	{
			int		fd;
			struct t_call	*callp;
			struct t_call	 call;
		} U_Xcall;

/* <t_connect: U_Xcallfin, rcvcall.addr.buf, .opt.buf, .udata.buf */
/* <t_listen: U_Xcallfin, call.addr.buf, .opt.buf, .udata.buf */
/* <t_rcvconnect: U_Xcallfin, call.addr.buf, .opt.buf, .udata.buf */
		struct	{
			struct t_call	*callp;
			struct t_call	 call;
			struct xti_ret   xret;
		} U_Xcallfin;

/* >t_open: U_Xopen, name */
		struct	{
			char		*namep;
			int		 oflag;
			struct t_info	*infop;
			short		 namel;
		} U_Xopen;

/* <t_open: U_Xopenfin */
		struct	{
			struct t_info	*infop;
			struct t_info	 info;
			struct xti_ret	 xret;
		} U_Xopenfin;

/* >t_rcv: U_Xrcvreq */
		struct	{
			int	 fd;
			char	*bufp;
			unsigned nbytes;
		} U_Xrcvreq;

/* <t_rcv: U_Xrcvfin, data */
		struct	{
			char		*bufp;
			unsigned	 nbytes;
			int		 flags;
			struct xti_ret	 xret;
			short		 nbytest;
		} U_Xrcvfin;

/* >t_snd: U_Xsndreq, data */
		struct	{
			int		 fd;
			char		*bufp;
			unsigned	 nbytes;
			int		 flags;
			short		 nbytest;
		} U_Xsndreq;

		struct	{
			int		 fd;
			struct t_discon	*disp;
			struct t_discon	 dis;
			struct xti_ret	 xret;
		} U_Xdis;
#endif /* CMXTRACE_XTI */
	} U;
};

#define T_LHEADER	(sizeof (short) + sizeof (short) + sizeof (long))

/*
 * convenient abbreviations
 */
#define t_fattach	U.U_attach
#define t_fretatt	U.U_retatt
#define t_fdetach	U.U_detach

#define t_fconrq	U.U_conrq
#define t_fconcf	U.U_concf
#define t_fconin	U.U_conin
#define t_fconrs	U.U_conrs

#define t_fdisrq	U.U_disrq
#define t_fdisin	U.U_disin
#define t_fredin	U.U_redin
#define t_fredrq	U.U_redrq

#define t_fdatarq	U.U_datarq
#define t_fvdatarq	U.U_vdatarq
#define t_fdatarqs	U.U_datarqs

#define t_fdatain	U.U_datain
#define t_fvdatain	U.U_vdatain
#define t_fdatains	U.U_datains

#define t_fflow		U.U_flow

#define t_fxdatrq	U.U_xdatrq
#define t_fxdatrqs	U.U_xdatrqs
#define t_fxdatin	U.U_xdatin
#define t_fxdatins	U.U_xdatins

#define t_fevent	U.U_event
#define t_devent	U.U_devent
#define t_finfo		U.U_info
#define t_finfo3	U.U_info3

#define t_fgetaddr	U.U_gaddr
#define t_fgetname	U.U_gname
#define t_fgetloc	U.U_gloc

#define t_ffirst	U.U_first	
#define t_ffirst2	U.U_first2	
#define t_freterr	U.U_reterr
#define t_fretok	U.U_retok	

#define t_ftcpsck	U.U_tcpsck
#define t_ftcpsopt  	U.U_tcpsopt
#define t_ftcpabc	U.U_tcpabc
#define t_ftcplis	U.U_tcplis
#define t_ftcpsel	U.U_tcpsel

#define t_fopen		U.U_open
#define t_fsvc		U.U_svc

#define t_fsetopt	U.U_setopt
#define t_fcallback	U.U_callback
#define t_fcbres	U.U_fcbres

#define	t_fstring	U.U_string
#define	t_fsnapipe	U.U_snapipe

#define	t_fdlerr	U.U_dlerr

#define t_funame	U.U_uname


#define	t_fXaccept	U.U_Xaccept
#define	t_fXfin		U.U_Xfin
#define	t_fXfd		U.U_Xfd
#define	t_fXbind	U.U_Xbind
#define	t_fXbindfin	U.U_Xbindfin
#define	t_fXconnect	U.U_Xconnect
#define	t_fXcall	U.U_Xcall
#define	t_fXcallfin	U.U_Xcallfin
#define	t_fXopen	U.U_Xopen
#define	t_fXopenfin	U.U_Xopenfin
#define	t_fXrcvreq	U.U_Xrcvreq
#define	t_fXrcvfin	U.U_Xrcvfin
#define	t_fXsndreq	U.U_Xsndreq
#define	t_fXdis		U.U_Xdis

void t_trcoff	(int	flsh);	/* global wg. t_setopt */
void t_trc	(int, int, ...);
#endif /* _CMXTRACE_H */
