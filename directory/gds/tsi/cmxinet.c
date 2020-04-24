/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cmxinet.c,v $
 * Revision 1.1.5.2  1996/03/11  13:24:13  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:12:39  marty]
 *
 * Revision 1.1.5.1  1995/12/08  17:47:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:25:10  root]
 * 
 * Revision 1.1.2.4  1994/07/06  15:08:27  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:21:49  marrek]
 * 
 * Revision 1.1.2.3  1994/06/21  14:48:16  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:56:33  marrek]
 * 
 * Revision 1.1.2.2  1994/06/10  21:17:13  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:54:33  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/10  16:15:16  marrek
 * 	Created for April Submission.
 * 	[1994/04/26  12:50:46  marrek]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: cmxinet.c,v $ $Revision: 1.1.5.2 $ $Date: 1996/03/11 13:24:13 $";
#endif

/*
 * COPYRIGHT (C) 1990 BY SIEMENS AG  All rights reserved
 * COPYRIGHT (C) Siemens Informationssysteme AG 1993
 *               All Rights Reserved
 *  
 * This software is furnished under a license and may be used
 * only in accordance with the terms of that license and with the
 * inclusion of the above copyright notice. This software may not
 * be provided or otherwise made available to, or used by, any
 * other person. No title to or ownership of the software is
 * hereby transferred.
 */

/****************************************************************************/
/*                                                                          */
/*                INTERFACE ICMX(L) ON TOP OF THE TCP/IP                    */
/*                ======================================                    */
/*                                                                          */
/* This code is a mapping of ICMX(L) as defined by CMX V2.5 until V3.1 onto */
/* 4.3BSD sockets. Its roots are a similar implementation done for CMX V2.2.*/
/* This adaption adds major enhancements, higher robustness and the new     */
/* options available with CMX V2.5.                                         */
/*                                                                          */
/* Author: rit                                           Date:     6.03.89  */
/*         bee   V30                                     Date:    24.08.90  */
/*         kam   V31                                     	  05.03.92  */
/*                                                                          */
/* Defines:                                                                 */
/* SYSPID_MAX	highest pid occupied by a system process (init)		    */
/*		usually taken from limits.h				    */
/*                                                                          */
/****************************************************************************/

#ifdef THREADSAFE
# include <pthread.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/types.h>
#if defined(_AIX)
#include <sys/select.h>
#endif /* _AIX */
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#if defined(__OSF__)
#include <machine/endian.h>
#endif /* __OSF__ */
#if defined(HPUX)                                                               
#define t_error meng_ma_ned                                                     
#include "xti.h"                                                                
#else                                                                           
#define t_error(x) *meng_ma_ned                                                 
#include <sys/tiuser.h>                                                         
#endif                                                                          
#undef t_error /* now t_error is taken from cmx */
#undef T_MORE
#undef T_END
#undef T_IDLE
#undef T_CHECK
#undef T_ERROR
#include <memory.h>
#include <netdb.h>
#include <sys/uio.h>
#include "cmx.h"
#include "cmx_addr.h"
#include "cmxextern.h"
#include "cmxaddr.h"
#include "cmxtrace.h"
#define	CMXINET_INTERN
#include "cmxinet.h"
#include "cxioctl.h"
#include <ndsif.h>
#ifdef	ALLOC_TEST
#include "alloctrc.h"
#endif	/* ALLOC_TEST */

/*
static char sccsid[] = "@(#)cmxinet.c	311.55 93/08/24 CMX (NC1)";
*/
/**************************************************************************/
/*      D E C L A R A T I O N    O F    G L O B A L    D A T A            */
/**************************************************************************/
/*
 * only global for CMXINET.C and CMXINUP.C
 */
u_short         portnrsav;
struct tc_proc  tc_p = { 0 };                                                   


/**************************************************************************/
/*	D E C L A R A T I O N    O F    I N T E R N A L    D A T A	  */
/**************************************************************************/

/*
 * Legal pid for t_redirect() are between SYSPID_MAX and PID_MAX.
 * If SYSPID_MAX is not defined, assume at least pids 1, 2, 3 are system's.
 */
#ifndef SYSPID_MAX
#define SYSPID_MAX	3
#endif

/*
 * For waiting at t_redirect
 */
#define T_WAITMAX	 65535
#define T_SLEEP  	 8

static fd_set	rfds, wfds;

static struct du_buf	t_du;	/* TIDU buffer */
static struct timeval	t;
static int		res;
static int chck_selres (int *tref);
static int c1_hdlr  (struct tc_app *app, struct tc_con *conp, int *tref);
static int c2_hdlr  (struct tc_app *app, struct tc_con *conp);
static int c3_hdlr  (struct tc_app *app, struct tc_con *conp);
static int c4_hdlr  (struct tc_app *app, struct tc_con *conp, int res_pen);
static int c8_hdlr  (struct tc_con *conp);                                     
static int c11_hdlr (struct tc_app *app, struct tc_con *conp, int *tref);

/*
 * Prototyping
 */
int tc_attach (char *name, t_opta *opt);
int tc_detach (char *name);
int tc_conrq (int *, union t_address *, union t_address *, t_opt13 *);
int tc_conin (int *, union t_address *, union t_address *, t_opt1 *);
int tc_conrs (int *tref, t_opt13 *opt);
int tc_concf (int *tref, t_opt1 *opt);
int tc_redrq (int *tref, int *pid, t_opt12 *opt);
int tc_redin (int *tref, int *pid, t_opt23 *opt);
int tc_disrq (int *tref, t_opt2 *opt);
int tc_disin (int *tref, int *reason, t_opt2 *opt);
int tc_vdatarq (int *tref, struct t_data *vdata, int *vcnt, int *chain);
int tc_datarq ( int *tref, char *datap, int *datal, int *chain);
int tc_vdatain (int *tref, struct t_data *vdata, int *vcnt, int *chain);
int tc_datain (int *tref, char *datap, int *datal, int *chain);
int tc_datastop (int *tref);
int tc_datago (int *tref);
int tc_xdatrq (int *tref, char *datap, int *datal);
int tc_xdatin (int *tref, char *datap, int *datal);
int tc_xdatstop (int *tref);
int tc_xdatgo (int *tref);
int tc_event (int *tref, int cmode, t_opte *opt);
int tc_info(int *tref, t_opti *opt);


/*------------------------------------------------------------------------*
 * T-ATTACH 
 *------------------------------------------------------------------------*/

/*
 * t_attach - T-ATTACH.request
 */
int tc_attach (char *name, t_opta *opt)
{
	extern void t_addcclst(struct t_cclst *);
	struct tc_app	*app;
	int	myport, res, res2;
	int	apmode = (T_ACTIVE|T_PASSIVE|T_REDIRECT);
	Nds_osisockinfo ndsinfo ;
static	struct t_cclst lan1cclist ={1,CX_LAN1ID};

	/*
	 * Check name, options, and state
	 * reinitialise after fork()
	 */
	if (opt) {
		if ((opt->opta1.t_optnr != T_OPTA1 
			&& opt->opta2.t_optnr != T_OPTA2 
			&& opt->opta4.t_optnr != T_OPTA4 
			&& opt->opta5.t_optnr != T_OPTA5 
			&& opt->opta6.t_optnr != T_OPTA6)
		    || !(opt->opta1.t_apmode & (T_ACTIVE|T_PASSIVE|T_REDIRECT))
	 	    || (opt->opta1.t_apmode & ~(T_ACTIVE|T_PASSIVE|T_REDIRECT))
		    || opt->opta1.t_conlim < T_NOLIMIT
		    || opt->opta1.t_conlim == 0
		    || opt->opta1.t_conlim > TC_CON_MAX) {
			_t_error = T_WPARAMETER;
			return (T_ERROR);
		}
		apmode = opt->opta2.t_apmode;
	}
	if (tc_p.tc_flags.tc_mypid != t_pid) {
		if (tc_p.tc_flags.tc_mypid != 0) {
			/*
			 * This is child which did not exec().
			 * Release all data structures but don't touch
			 * socket-fd's if the application
			 * wants to get connections via REDIRECT !
			 * HAVE A CLOSER LOOK INTO THIS!!!
			 */
			while ((app = tc_p.tc_apptr) != NULLAP) {
				tc_app_free(app, !(apmode & T_REDIRECT), FALSE);
				tc_p.tc_nr_att--;
			}
		}
		if (t_du.state != NOBUF)
			free(t_du.sptr);
		tc_p.tc_flags.tc_mypid = t_pid;
		tc_p.tc_flags.tc_enabsig = (t_cmxsig ? TRUE : FALSE);
	}
	if ((((T_adrdesc *) name)->t_infotype == T_TSAPINFO) ||
	    (((T_adrdesc *) name)->t_infotype == T_NSAPINFO)) {
		ndsinfo.nds_osiaddr = (T_adrdesc *) name ;
		if (NDS_map_addrinfo (NDS_OSITOSOCK,
						(Nds_mapinfo *) &ndsinfo) < 0) {
			_t_error = ndsinfo.nds_errno ;
			return (T_ERROR);
		}
		myport = ntohs (ndsinfo.nds_sockaddr.sin_port) ;
	} else
		myport = tc_getmyport((struct t_myname *)name);
	if (myport < MIN_PORT || myport > MAX_PORT) {
		_t_error = T_WPARAMETER;
		return (T_ERROR);
	}
	if (tc_srch_app(myport) != NULLAP) {
		_t_error = T_WAPPLICATION;
		return (T_ERROR);
	}
	if (tc_p.tc_nr_att >= TC_APP_MAX) {
		_t_error = T_WAPP_LIMIT;
		return (T_ERROR);
	}

	/*
	 * Allocate and init a fresh application structure
	 */
	if ((app = (struct tc_app *) calloc(sizeof (struct tc_app), sizeof(char))) == NULLAP) {
		_t_error = T_ENOMEM;
		return (T_ERROR);
	}
	app->tc_portnr = myport;
	app->tc_nr_conn = 0;
	app->tc_conp = NULLCP;
	if (opt) {
		app->tc_uattid =
			((opt->opta2.t_optnr == T_OPTA2) || 
			 (opt->opta2.t_optnr == T_OPTA5) || 
			 (opt->opta2.t_optnr == T_OPTA6)) ? opt->opta2.t_uattid : 0;
		app->tc_apmode = opt->opta2.t_apmode;
		app->tc_conlim = opt->opta2.t_conlim;
	} else {
		app->tc_apmode = apmode;
		app->tc_conlim = 1;
		app->tc_uattid = 0;
	}
	memcpy(&app->tc_name, name, ((struct t_myname *) name)->t_mnlng);

	/*
	 * Set up sockets for incoming or redirected connections
	 * then link application structure in chain
	 */
	res = res2 = T_OK;
	if (app->tc_apmode & T_REDIRECT)
		res2 = tc_listen(T_REDIRECT, app);
		if ((res2 == T_ERROR) | (res2 == T_NOTPRIV)) {
			free(app);
			return (res2);
		}
	if (app->tc_apmode & T_PASSIVE)
		res = tc_listen(T_PASSIVE, app);
		if ((res == T_ERROR) | (res == T_NOTPRIV)) {
			close(app->tc_rconp->tc_socket_fd);
			FD_CLR(app->tc_rconp->tc_socket_fd,&t_fdset);
			FD_CLR(app->tc_rconp->tc_socket_fd,&t_rfds);
			t_trc(T_FCLOSE, 0, app->tc_rconp->tc_socket_fd);
			free(app->tc_rconp);
			free(app);
			return (res);
		}
	if (res == T_OK || res == T_NOTFIRST) {
		tc_app_ins(app);
		tc_p.tc_nr_att++;
		if (opt &&
			((opt->opta2.t_optnr == T_OPTA2) ||
			 (opt->opta5.t_optnr == T_OPTA5) ||
			 (opt->opta6.t_optnr == T_OPTA6))) {
			opt->opta2.t_sptypes |= T_LANINET;
			opt->opta2.t_attid = 0;
			opt->opta2.t_ccbits |= T_L1;
			/* add attachment in cc-list */
			t_addcclst((struct t_cclst *)&lan1cclist);
		}
	}

	if ((res == T_NOTFIRST) || (res2 == T_NOTFIRST))
		return (T_NOTFIRST);
	else
		return (T_OK);
}

/*
 * t_detach - T-ATTACH.release
 */
int tc_detach (char	*name)
{
	struct tc_app	*app;
	int	myport;
	Nds_osisockinfo ndsinfo ;

	/*
	 * search application structure with myport from name and unlink it
	 */
	if ((((T_adrdesc *) name)->t_infotype == T_TSAPINFO) ||
	    (((T_adrdesc *) name)->t_infotype == T_NSAPINFO)) {
		ndsinfo.nds_osiaddr = (T_adrdesc *) name ;
		if (NDS_map_addrinfo (NDS_OSITOSOCK,
						(Nds_mapinfo *) &ndsinfo) < 0) {
			_t_error = ndsinfo.nds_errno ;
			return (T_ERROR);
		}
		myport = ntohs (ndsinfo.nds_sockaddr.sin_port) ;
	} else
		myport = tc_getmyport((struct t_myname *)name);
	if (myport < MIN_PORT || myport > MAX_PORT) {
		_t_error = T_WPARAMETER;
		return (T_ERROR);
	}
	if ((app = tc_srch_app(myport)) == NULLAP) {
		_t_error = T_WAPPLICATION;
		return (T_ERROR);
	}

	tc_app_free(app, TRUE, TRUE);
	tc_p.tc_nr_att--;
	if (t_du.state != NOBUF && t_du.attid == app->tc_ind) {
		if (t_du.state == FULL)
			free(t_du.sptr);
		t_du.state = NOBUF;
	}

	if (tc_p.tc_nr_att == 0 && tc_p.tc_flags.tc_enabsig) {
		if (t_cmxsig == SIGIO) 
			;
		else {
			if (t_sigio) {
				sigaction(SIGIO,&t_prevsigio,NULL);
				t_sigio = FALSE;
				t_cmxsig = 0;
				t_prevsigio.sa_handler = SIG_DFL;
			}
		}
		tc_p.tc_flags.tc_enabsig = FALSE;
	}

	return (T_OK);
}

/*------------------------------------------------------------------------*
 * T-CONNECTION
 *------------------------------------------------------------------------*/

/*
 * t_conrq - T-CONNECTION.request
 */
int tc_conrq   (int		*tref,
		union t_address *toaddr,
		union t_address *fromaddr,
		t_opt13		*opt)
{
	struct tc_app	*app;
	struct tc_con	*conp;
	int		myport, sd;
	struct sockaddr_in sinhim;
	Nds_osisockinfo ndsinfo ;

	if (opt && 
	    ((opt->optc1.t_optnr != T_OPTC1 && opt->optc3.t_optnr != T_OPTC3)
		|| opt->optc3.t_timeout < T_NOLIMIT
		|| opt->optc3.t_udatal < 0
		|| (opt->optc3.t_xdata != T_YES && opt->optc3.t_xdata != T_NO)
		|| opt->optc3.t_udatal > T_CON_SIZE)) {
		_t_error = T_WPARAMETER;
		return (T_ERROR);
	}

	/*
	 * allocate and partially init a fresh connection element
	 */
	if ((conp = (struct tc_con *)
		calloc(sizeof(struct tc_con), sizeof(char))) == NULLCP) {
		free(conp);
		_t_error = T_ENOMEM;
		return (T_ERROR);
	}
	FLC_INI(conp->tc_flcontr);

	/*
	 * Check fromaddr, options and state
	 */
	if ((((T_adrdesc *) fromaddr)->t_infotype == T_TSAPINFO) ||
	    (((T_adrdesc *) fromaddr)->t_infotype == T_NSAPINFO)) {
		ndsinfo.nds_osiaddr = (T_adrdesc *) fromaddr ;
		if (NDS_map_addrinfo (NDS_OSITOSOCK,
						(Nds_mapinfo *) &ndsinfo) < 0) {
			_t_error = ndsinfo.nds_errno ;
			free (conp);
			return (T_ERROR);
		}
		myport = ntohs (ndsinfo.nds_sockaddr.sin_port) ;
		conp->tc_tcallnglen = ndsinfo.nds_ositselln ;
		memcpy ((void *) conp->tc_tselng, (void *) ndsinfo.nds_ositsel,
					       (size_t) ndsinfo.nds_ositselln) ;
	} else
		myport = tc_getmyport(&fromaddr->tmyname);
	if (myport < MIN_PORT || myport > MAX_PORT) {
		_t_error = T_WPARAMETER;
		return (T_ERROR);
	}
	if ((app = tc_srch_app(myport)) == NULLAP) {
		_t_error = T_WAPPLICATION;
		return (T_ERROR);
	}
	if (!(app->tc_apmode & T_ACTIVE)) {
		_t_error = T_WSEQUENCE;
		return (T_ERROR);
	}
	if (app->tc_nr_conn == app->tc_conlim) {
		_t_error = T_WCONN_LIMIT;
		return (T_ERROR);
	}

	if ((((T_adrdesc *) toaddr)->t_infotype == T_TSAPINFO) ||
	    (((T_adrdesc *) toaddr)->t_infotype == T_NSAPINFO)) {
		ndsinfo.nds_osiaddr = (T_adrdesc *) toaddr ;
		if (NDS_map_addrinfo (NDS_OSITOSOCK,
						(Nds_mapinfo *) &ndsinfo) < 0) {
			_t_error = ndsinfo.nds_errno ;
			free (conp);
			return (T_ERROR);
		}
		conp->tc_portnr = ntohs (ndsinfo.nds_sockaddr.sin_port) ;
		conp->tc_tcalledlen = ndsinfo.nds_ositselln ;
		memcpy ((void *) conp->tc_tseled, (void *) ndsinfo.nds_ositsel,
					       (size_t) ndsinfo.nds_ositselln) ;
	} else {
		tc_getmytsel(app,conp);
		tc_gethisport(&toaddr->tpartaddr,conp);
	}
	
	if (conp->tc_portnr < MIN_PORT || conp->tc_portnr > MAX_PORT) {
		_t_error = T_WPARAMETER;
		free((char *)conp);
		return (T_ERROR);
	}
	conp->tc_state = T_DISCONIND;
	conp->tc_event = T_DISIN;
	conp->savindata = NULL;
	conp->savoutdata = NULL;
	conp->read_state = R_IDLE;

	/* save user data */

	if (opt && opt->optc1.t_udatal > 0) {
		conp->tc_optdatlng = opt->optc1.t_udatal;
		conp->tc_optdat = (char *)malloc(conp->tc_optdatlng);
		memcpy(conp->tc_optdat, opt->optc1.t_udatap, conp->tc_optdatlng);
	}
	else
		conp->tc_optdatlng = 0;

	/*
	 * establish an stream socket in INTERNET domain, connect to the peer,
	 * then send a TP0 connection request over
	 */
	 while ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1 && errno == EINTR)
                t_trc(T_FTCPSCK, 0, AF_INET, SOCK_STREAM, 0, sd, errno);        

	t_trc(T_FTCPSCK, 0, AF_INET, SOCK_STREAM, 0, sd, errno);
	if ((conp->tc_socket_fd = sd) == -1) {
		if (errno == EPROTONOSUPPORT || errno == ESOCKTNOSUPPORT
			|| errno == EPFNOSUPPORT || errno == EAFNOSUPPORT) {
			free((char *)conp);
			_t_error = T_NOCCP;
			return (T_ERROR);
		} else if (errno == EMFILE || errno == ENOSPC 
			|| errno == ENOSR || errno == ENOMEM
			|| errno == ENOBUFS || errno == EBADF) {
			free((char *)conp);
			_t_error = T_WCONN_LIMIT;
			return (T_ERROR);
		} else
			conp->tc_reason = T_RUNKNOWN;
		goto out;
	}
	FD_SET(sd, &t_fdset);
	FD_SET(sd, &t_wfds);
	if (tc_enabsig(conp)) {
		free((char *)conp);
		return (T_ERROR);
	}

	tc_setsock(sd,TC_NBLOCK); /* set socket options */

	if ((((T_adrdesc *) toaddr)->t_infotype == T_TSAPINFO) ||
	    (((T_adrdesc *) toaddr)->t_infotype == T_NSAPINFO)) {
		sinhim = ndsinfo.nds_sockaddr ;
	} else {
		memset((char *) &sinhim, 0, sizeof(sinhim));
		sinhim.sin_family = AF_INET;
		sinhim.sin_port = htons((u_short) conp->tc_portnr);
		memcpy ((char *) &sinhim.sin_addr,
			tc_gethisaddr(&toaddr->tpartaddr),
			sizeof (struct in_addr));
	}

	/*
	 * Now connect.
	 * With signals enabled, connect() often reports interruption (EINTR)
	 */
	while ((sd = connect(conp->tc_socket_fd, (struct sockaddr *) &sinhim,
		sizeof(sinhim))) == -1 && errno == EINTR)
		t_trc(T_FTCPCON, 0, conp->tc_socket_fd, &sinhim, sd, errno);

	t_trc(T_FTCPCON, 0, conp->tc_socket_fd, &sinhim, sd, errno);

	if (sd == -1 && errno != EINTR) {
		if ((errno == EINPROGRESS) || (errno == EAGAIN)) {
			conp->tc_state = T_OUTCONNECT;
			conp->tc_event = T_NOEVENT;
		}
		if (errno == EPROTONOSUPPORT || errno == ESOCKTNOSUPPORT
		    || errno == EPFNOSUPPORT || errno == EAFNOSUPPORT
		    || errno == EADDRNOTAVAIL|| errno == EIO) {
			_t_error = T_NOCCP;
			close(conp->tc_socket_fd);
			t_trc(T_FCLOSE, 0, conp->tc_socket_fd);
			FD_CLR(conp->tc_socket_fd,&t_fdset);
			FD_CLR(conp->tc_socket_fd,&t_rfds);
			FD_CLR(conp->tc_socket_fd,&t_wfds);
			free((char *)conp);
			return (T_ERROR);
		} else if (errno == ETIMEDOUT) {
			_t_error = T_ETIMEOUT;
			close(conp->tc_socket_fd);
			t_trc(T_FCLOSE, 0, conp->tc_socket_fd);
			FD_CLR(conp->tc_socket_fd,&t_fdset);
			FD_CLR(conp->tc_socket_fd,&t_rfds);
			FD_CLR(conp->tc_socket_fd,&t_wfds);
			free((char *)conp);
			return (T_ERROR);
		} else if (errno == ECONNREFUSED || errno == EADDRINUSE
		    || errno == ENETUNREACH || errno == ENETDOWN
		    || errno == EHOSTUNREACH || errno == EHOSTDOWN
		    || errno == EALREADY )
			conp->tc_reason = T_RSAPNOTATT;
		else
			conp->tc_reason = T_RUNKNOWN;
	} else {
		FD_CLR(conp->tc_socket_fd, &t_wfds);
		FD_SET(conp->tc_socket_fd, &t_rfds);
		if (tc_sndfrm(CONRQ, conp, app->tc_portnr, opt))
			conp->tc_reason = T_RLCONNLOST;
		else {
			conp->tc_state = T_OUTCONPEND;
			conp->tc_event = T_NOEVENT;
		}
	}

out:

	/*
	 * Link newly established connect element (possibly just to report a
	 * disconnect indication). Set peer address and tref.
	 * Inhibit redirection, if t_conlim is reached by
	 * unlinking redirect element (thus closing 'redirect' socket)
	 */
	conp->tc_ucepid =
		(opt && opt->optc3.t_optnr == T_OPTC3) ? opt->optc3.t_ucepid : 0;
	memcpy((char *) &conp->tc_addr, (char *) toaddr, toaddr->tpartaddr.t_palng);
	tc_con_ins(app, conp);
	app->tc_nr_conn++;
	if ((app->tc_nr_conn == app->tc_conlim) && (app->tc_rconp != NULL))
		tc_rd_free(app);

	*tref = SET_TREF(app->tc_ind, conp->tc_ind);
	conp->tc_srcref = SET_SRCREF(app->tc_ind, conp->tc_ind);
	return (T_OK);
}

/*
 * t_conin - T-CONNECTION.indication
 */
int tc_conin   (int		*tref,
		union t_address *toaddr,
		union t_address *fromaddr,
		t_opt1		*opt)
{
	struct tc_app	*app;
	struct tc_con	*conp;
	struct tc_con 	a;
	Nds_osisockinfo	ndsinfo ;
	int	app_ind, con_ind;
	char		from[T_TSAPSIZE] ;

	app_ind = GET_APP_IND(tref);
	con_ind = GET_CON_IND(tref);
	if ((app = tc_get_apptr(app_ind)) == NULLAP
	     || app_ind >= TC_APP_MAX
	     || (conp = tc_get_conptr(app, con_ind)) == NULLCP
	     || con_ind >= TC_CON_MAX
	     || conp->tc_state != T_INCONIND
	     || conp->tc_event != T_CONIN) {
		_t_error = T_WSEQUENCE;
		return (T_ERROR);
	}

	if (opt && 
	    (opt->optc1.t_optnr != T_OPTC1
		|| opt->optc1.t_udatal < 0
		|| (opt->optc1.t_udatal > 0
			&& opt->optc1.t_udatal < conp->tc_udatal))) {
		_t_error = T_WPARAMETER;
		return (T_ERROR);
	}

	conp->tc_xdata = T_NO;
	conp->tc_state = T_INCONPEND;
	conp->tc_event = T_NOEVENT;

        /* get my T-selector to check against incoming T-selector */
	tc_getmytsel(app,conp); 

	if (tc_rcvconincf(CONIN, conp, opt)) {
		conp->tc_state  = T_DISCONIND;
		conp->tc_length = 0;
		conp->tc_event  = T_DISIN;
	}

	/*
	 * return fromaddr, toaddr
	 */
	if (conp->tc_portnr == app->tc_portnr) {
		 /* in case of incompability between old and new TSAP-ID */
		conp->tc_partner = RFCV25;
		conp->tc_portnr = portnrsav; 
		/* swap calling and called TSAP-ID */
		/* HELP = CALLED */
		a.tc_tcalledlen = conp->tc_tcalledlen;
		memcpy(a.tc_tseled,conp->tc_tseled,conp->tc_tcalledlen);
		/* CALLED = CALLING */
		conp->tc_tcalledlen = conp->tc_tcallnglen;
		memcpy(conp->tc_tseled,conp->tc_tselng,conp->tc_tcallnglen);
		/* CALLING = HELP */
		conp->tc_tcallnglen = a.tc_tcalledlen;
		memcpy(conp->tc_tselng,a.tc_tseled,a.tc_tcalledlen);
		}
	if ((((T_adrdesc *) &app->tc_name)->t_infotype == T_TSAPINFO) ||
	    (((T_adrdesc *) &app->tc_name)->t_infotype == T_NSAPINFO)) {
		ndsinfo.nds_osiaddr = (T_adrdesc *) from ;
		ndsinfo.nds_sockaddr.sin_addr.s_addr = conp->tc_inaddr.s_addr ;
		ndsinfo.nds_sockaddr.sin_port = htons (conp->tc_portnr) ;
		ndsinfo.nds_ositselln = conp->tc_tcallnglen ;
		memcpy ((void *) ndsinfo.nds_ositsel, (void *) conp->tc_tselng,
						 (size_t) conp->tc_tcallnglen) ;
		if (NDS_map_addrinfo (NDS_SOCKTOOSI, (Nds_mapinfo *) &ndsinfo) < 0) {
			_t_error = ndsinfo.nds_errno ;
			return (T_ERROR);
		}
		memcpy ((void *) fromaddr, (void *) ndsinfo.nds_osiaddr,
				     (size_t) ndsinfo.nds_osiaddr->t_translng) ;
		memcpy ((void *) toaddr, (void *) &app->tc_name,
			   (size_t) ((T_adrdesc *) &app->tc_name)->t_translng) ;
	} else {
		tc_sethisaddr(conp);                   /* build fromaddr */
		memcpy(fromaddr, &conp->tc_addr, sizeof(struct t_partaddr));
		memcpy(toaddr, &app->tc_name, sizeof(struct t_myname));
	}
	conp->tc_srcref = SET_SRCREF(app->tc_ind, conp->tc_ind);

	return (T_OK);
}

/*
 * t_conrs - T-CONNECTION.response
 */
int tc_conrs (int *tref, t_opt13 *opt)
{
	struct tc_app	*app;
	struct tc_con	*conp;
	int	app_ind, con_ind;

	app_ind = GET_APP_IND(tref);
	con_ind = GET_CON_IND(tref);
	if ((app = tc_get_apptr(app_ind)) == NULLAP
	     || app_ind >= TC_APP_MAX
	     || (conp = tc_get_conptr(app, con_ind)) == NULLCP
	     || con_ind >= TC_CON_MAX
	     || conp->tc_state != T_INCONPEND) {
		_t_error = T_WSEQUENCE;
		return (T_ERROR);
	}

	if (opt && 
	    ((opt->optc1.t_optnr != T_OPTC1 && opt->optc3.t_optnr != T_OPTC3)
		|| opt->optc3.t_udatal < 0
		|| opt->optc3.t_udatal > T_CON_SIZE
		|| opt->optc3.t_xdata != T_NO
		|| opt->optc3.t_timeout < T_NOLIMIT
		|| (opt->optc3.t_udatal > 0 && opt->optc3.t_udatap == NULL))) {
		_t_error = T_WPARAMETER;
		return (T_ERROR);
	}

	if (tc_sndfrm(CONRS, conp, app->tc_portnr, opt)) {
		conp->tc_event  = T_DISIN;
		conp->tc_length = 0;
		conp->tc_state  = T_DISCONIND;
		_t_error = T_COLLISION;
		return (T_ERROR);
	}
	conp->tc_state = T_DATTRFRDY;
	conp->tc_ucepid =
		(opt && opt->optc3.t_optnr == T_OPTC3) ? opt->optc3.t_ucepid : 0;

	return (T_OK);
}

/*
 * t_concf - T-CONNECTION.confirmation
 */
int tc_concf (int *tref, t_opt1 *opt)
{
	struct tc_app	*app;
	struct tc_con	*conp;
	int	app_ind, con_ind;

	app_ind = GET_APP_IND(tref);
	con_ind = GET_CON_IND(tref);
	if ((app = tc_get_apptr(app_ind)) == NULLAP
	     || app_ind >= TC_APP_MAX
	     || (conp = tc_get_conptr(app, con_ind)) == NULLCP
	     || con_ind >= TC_CON_MAX
	     || conp->tc_state != T_OUTCONCONF
	     || conp->tc_event != T_CONCF) {
		_t_error = T_WSEQUENCE;
		return (T_ERROR);
	}

	if (opt &&
	    (opt->optc1.t_optnr != T_OPTC1
		|| opt->optc1.t_udatal < 0
		|| (opt->optc1.t_udatal > 0
			&& opt->optc1.t_udatal < conp->tc_udatal))) {
		_t_error = T_WPARAMETER;
		return (T_ERROR);
	}

	if (tc_rcvconincf(CONCF, conp, opt)) {
		conp->tc_state  = T_DISCONIND;
		conp->tc_length = 0;
		conp->tc_event  = T_DISIN;
		_t_error = T_COLLISION;
		return (T_ERROR);
	}
	conp->tc_state = T_DATTRFRDY;
	conp->tc_event = T_NOEVENT;

	return (T_OK);
}

/*------------------------------------------------------------------------*
 * T-REDIRECT
 *------------------------------------------------------------------------*/

/*
 * t_redrq - T-REDIRECT.request
 */
int tc_redrq (int *tref, int *pid, t_opt12 *opt)
{
	struct tc_app	*app;
	struct tc_con	*conp;
	int	app_ind, con_ind;
	int	sd, res;
	struct sockaddr_in sinhim;
	int	tout = T_NO;
	unsigned int	unslept = T_NO;
static	fd_set 	conn_wfds;
static	struct timeval ti;
	int	conn_nfds = MIN(open_max,FD_SETSIZE);

	if (t_du.state != NOBUF && *tref == t_du.tref) {
		_t_error = T_WSEQUENCE;
		return (T_ERROR);
	}

	app_ind = GET_APP_IND(tref);
	con_ind = GET_CON_IND(tref);
	if ((app = tc_get_apptr(app_ind)) == NULLAP
	     || app_ind >= TC_APP_MAX
	     || (conp = tc_get_conptr(app, con_ind)) == NULLCP
	     || con_ind >= TC_CON_MAX
	     || conp->tc_state != T_DATTRFRDY
	     || !FLC_CHK(conp->tc_flcontr, T_SNDGO)
	     || conp->tc_event != T_NOEVENT) {
		_t_error = T_WSEQUENCE;
		return (T_ERROR);
	}

	if (*pid == tc_p.tc_flags.tc_mypid
	    || *pid <= SYSPID_MAX || *pid > PID_MAX) {
		_t_error = T_WPARAMETER;
		return (T_ERROR);
	}

	if (opt && 
	    ((opt->optc1.t_optnr != T_OPTC1 && opt->optc2.t_optnr != T_OPTC2)
		|| opt->optc1.t_udatal < 0
		|| opt->optc1.t_udatal > T_RED_SIZE
		|| (opt->optc1.t_optnr == T_OPTC1
			&& (tout = opt->optc1.t_timeout) < T_NOLIMIT))) {
		_t_error = T_WPARAMETER;
		return (T_ERROR);
	}

	/*
	 * Create stream socket in INTERNET domain, connect to peer process
	 * which is supposed to listen on port
	 * "(application pid modulo 0x8000) + MAX_PORT"
	 */
	sd = socket(AF_INET, SOCK_STREAM, 0);
	t_trc(T_FTCPSCK, 0, AF_INET, SOCK_STREAM, 0, sd, errno);
	if (sd == -1) {
		_t_error = errno;
		if (errno == EMFILE || errno == ENOBUFS)
			_t_error = T_WRED_LIMIT;
		return (T_ERROR);
	}

	tc_setsock(sd,TC_NBLOCK); /* set socket options */
	memset((char *) &sinhim, 0, sizeof(sinhim));
	sinhim.sin_family = AF_INET;
	sinhim.sin_port = htons((u_short) ((*pid % 0x8000) + MAX_PORT));
	sinhim.sin_addr.s_addr = htonl(LOCALHOST);

	if (tout == T_NOLIMIT)
		tout = T_WAITMAX;

	if (tout != T_NO)
		tout--, (void) sleep(1);

retry:
	res = connect(sd, (struct sockaddr *) &sinhim, sizeof(sinhim));
	t_trc(T_FTCPCON, 0, sd, &sinhim, res, errno);
	if (res) {
		if ((errno == EINPROGRESS) || (errno == EAGAIN)) {
		/* wait for termination of connect() */
			FD_ZERO(&conn_wfds);
			FD_SET(sd, &conn_wfds);
			ti.tv_sec = ((tout > 10) ? tout : 10);
			t_trc(T_FTCPSEL,0,conn_nfds,NULL,&conn_wfds,NULL,&ti);
			res = select(conn_nfds,NULL,&conn_wfds,NULL,&ti);
			t_trc(T_FTCPSEL,1,conn_nfds,NULL,&conn_wfds,NULL,&ti,res,errno);
			goto out;
		}
		if ((errno == ECONNREFUSED) || (errno == EIO)) {
			/*
			 * If connect() fails with ECONNREFUSED (I/O in
                         * blocking mode) or with EIO (nonblocking I/O),
                         * we assume that the receiving
			 * process is not (yet) attached. The
			 * timeout option tells, whether retries are requested.
			 * Waiting is achieved via sleep(). Returning from
			 * sleep() with unslept time forces one retry, with a
			 * failure reported as interrupted t_redrq().
			 */
			if (tout == T_NO)	/* don't wait, report error */
				_t_error = T_WPARAMETER;
			else {			/* some waiting */
				if (unslept)	/* ... interrupted */
					_t_error = EINTR;
				else {
					if (tout <= 0) /* ... all slept */
						_t_error = T_ETIMEOUT;

					else {
						/*
						 * because of errors in connect systemcall
						 * at retries, we close socket
						 * and open again
						 */
						close(sd);
						FD_CLR(sd,&t_fdset);
						FD_CLR(sd,&t_rfds);
						t_trc(T_FCLOSE, 0, sd);
						sd = socket(AF_INET, SOCK_STREAM, 0);
						t_trc(T_FTCPSCK, 0, AF_INET, SOCK_STREAM, 0, sd, errno);
						if (sd == -1) {
							_t_error = errno;
							if (errno == EMFILE || errno == ENOBUFS)
								_t_error = T_WRED_LIMIT;
							return (T_ERROR);
						}
						tc_setsock(sd,TC_NBLOCK);
						memset((char *) &sinhim, 0, sizeof(sinhim));
						sinhim.sin_family = AF_INET;
						sinhim.sin_port = htons((u_short) ((*pid % 0x8000) + MAX_PORT));
						sinhim.sin_addr.s_addr = htonl(LOCALHOST);
						/*
						 * Up to 2 sleep()'s catch roving sig's
						 */
						if (tout > 2)
							tout--, (void) sleep(1);
						if (tout > 1)
							tout--, (void) sleep(1);
						if ((tout - T_SLEEP) <= 0) 
							unslept = sleep(tout);
						else
							unslept = sleep(T_SLEEP);
						tout -= T_SLEEP;	
						if (tout == 0)
							tout -= 1; /* T_ETIMEOUT statt T_WPARAMETER */
						goto retry;
					}
				}
			}
		} else
			_t_error = errno;

		close(sd);
		FD_CLR(sd,&t_fdset);
		FD_CLR(sd,&t_rfds);
		t_trc(T_FCLOSE, 0, sd);
		return (T_ERROR);
	}
out:

	/*
	 * Send state and user data. Close socket and unlink connect
	 * element. Since this frees a connection, reestablish redirection
	 */
	if (tc_sndfrm(REDRQ, conp, sd, opt)) {
		if (errno == EPIPE)
			_t_error = T_WPARAMETER;
		else
			_t_error = errno;
		close(sd);
		FD_CLR(sd,&t_fdset);
		FD_CLR(sd,&t_rfds);
		t_trc(T_FCLOSE, 0, sd);
		return (T_ERROR);
	}

	close(sd);
	FD_CLR(sd,&t_fdset);
	FD_CLR(sd,&t_rfds);
	t_trc(T_FCLOSE, 0, sd);
	FD_CLR(conp->tc_socket_fd, &t_fdset);
	FD_CLR(conp->tc_socket_fd, &t_rfds);
	FD_CLR(conp->tc_socket_fd, &t_wfds);
	close(conp->tc_socket_fd);
	t_trc(T_FCLOSE, 0, conp->tc_socket_fd);
	tc_con_del(app, conp);
	tc_p.tc_flags.tc_nfound = 0;
	app->tc_nr_conn--;
	if ((app->tc_rconp == NULLCP) && (app->tc_apmode & T_REDIRECT)) 
		tc_listen(T_REDIRECT, app);

	return (T_OK);
}

/*
 * t_redin - T-REDIRECT.indication
 */
int tc_redin (int *tref, int *pid, t_opt23 *opt)
{
	struct tc_app	*app;
	struct tc_con	*conp;
	int	app_ind, con_ind;

	app_ind = GET_APP_IND(tref);
	con_ind = GET_CON_IND(tref);
	if ((app = tc_get_apptr(app_ind)) == NULLAP
	     || app_ind >= TC_APP_MAX
	     || (conp = tc_get_conptr(app, con_ind)) == NULLCP
	     || con_ind >= TC_CON_MAX
	     || conp->tc_state != T_REDCONIND
	     || conp->tc_event != T_REDIN) {
		_t_error = T_WSEQUENCE;
		return (T_ERROR);
	}

	if (opt && 	
	    ((opt->optc2.t_optnr != T_OPTC2 && opt->optc3.t_optnr != T_OPTC3)
		|| opt->optc2.t_udatal < 0
		|| (opt->optc2.t_udatal > 0
			&& opt->optc2.t_udatal < conp->tc_udatal))) {
		_t_error = T_WPARAMETER;
		return (T_ERROR);
	}

	/*
	 * Fetch user data and state, also enable signalling now for my pid
	 */
	*pid = 0;
	if (tc_rcvredin(conp, pid, opt) || tc_enabsig(conp)) {
		FD_CLR(conp->tc_socket_fd, &t_fdset);
		FD_CLR(conp->tc_socket_fd, &t_rfds);
		close(conp->tc_socket_fd);
		t_trc(T_FCLOSE, 0, conp->tc_socket_fd);
		conp->tc_state  = T_DISCONIND;
		conp->tc_length = 0;
		conp->tc_event  = T_DISIN;
	} else {
		FD_SET(conp->tc_socket_fd, &t_fdset);
		FD_SET(conp->tc_socket_fd, &t_rfds);
		conp->tc_state = T_DATTRFRDY;
		conp->tc_event = T_NOEVENT;
	}
	conp->tc_ucepid =
		(opt && opt->optc3.t_optnr == T_OPTC3) ? opt->optc3.t_ucepid : 0;

	return (T_OK);
}

/*------------------------------------------------------------------------*
 * T-DISCONNECTION
 *------------------------------------------------------------------------*/

/*
 * t_disrq - T-DICONNECTION.request
 */
int tc_disrq (int *tref, t_opt2 *opt)
{
	struct tc_app	*app;
	struct tc_con	*conp;
	int	app_ind, con_ind;

	if (t_du.state != NOBUF && *tref == t_du.tref) {
		if (t_du.state == FULL)
			free(t_du.sptr);
		t_du.state = NOBUF;
	}

	app_ind = GET_APP_IND(tref);
	con_ind = GET_CON_IND(tref);
	if ((app = tc_get_apptr(app_ind)) == NULLAP
	     || app_ind >= TC_APP_MAX
	     || (conp = tc_get_conptr(app, con_ind)) == NULLCP
	     || con_ind >= TC_CON_MAX) {
		_t_error = T_WSEQUENCE;
		return (T_ERROR);
	}

	if (opt &&
	    (opt->optc2.t_optnr != T_OPTC2
		|| opt->optc2.t_udatal < 0
		|| opt->optc2.t_udatal > T_DIS_SIZE)) {
		_t_error = T_WPARAMETER;
		return (T_ERROR);
	}

	/*
	 * Send reason and user data. Close socket and unlink connect
	 * element. Since this frees a connection, reestablish redirection
	 */
	if (conp->tc_state != T_IDLE)
		(void) tc_sndfrm(DISRQ, conp, T_USER, opt);
	FD_CLR(conp->tc_socket_fd, &t_fdset);
	FD_CLR(conp->tc_socket_fd, &t_rfds);
	FD_CLR(conp->tc_socket_fd, &t_wfds);
	close(conp->tc_socket_fd);
	t_trc(T_FCLOSE, 0, conp->tc_socket_fd);
	tc_con_del(app, conp);
	tc_p.tc_flags.tc_nfound = 0;
	app->tc_nr_conn--;
	if ((app->tc_rconp == NULLCP) && (app->tc_apmode & T_REDIRECT))
		(void) tc_listen(T_REDIRECT, app);

	return (T_OK);
}

/*
 * t_disin - T-DICONNECTION.indication
 */
int tc_disin (int *tref, int *reason, t_opt2 *opt)
{
	struct tc_app	*app;
	struct tc_con	*conp;
	int	app_ind, con_ind;

	app_ind = GET_APP_IND(tref);
	con_ind = GET_CON_IND(tref);
	if ((app = tc_get_apptr(app_ind)) == NULLAP
	     || app_ind >= TC_APP_MAX
	     || (conp = tc_get_conptr(app, con_ind)) == NULLCP
	     || con_ind >= TC_CON_MAX
	     || conp->tc_event != T_DISIN) {
		_t_error = T_WSEQUENCE;
		return (T_ERROR);
	}

	if (opt &&
	    (opt->optc2.t_optnr != T_OPTC2
		|| opt->optc2.t_udatal < 0
		|| (opt->optc2.t_udatal > 0
                        && conp->tc_length > 0   /* No T_COLLISION */
			&& opt->optc2.t_udatal < conp->tc_udatal))) {
		_t_error = T_WPARAMETER;
		return (T_ERROR);
	}

	/*
	 * Fetch reason and user data. 
	 * If tc_socket_fd not -1, close socket and unlink connect element.
	 * Since this frees a connection, reestablish redirection.
	 */
	tc_rcvdisin(conp, reason, opt);
	if (conp->tc_socket_fd != -1)
	{
		FD_CLR(conp->tc_socket_fd, &t_fdset);
		FD_CLR(conp->tc_socket_fd, &t_rfds);
		FD_CLR(conp->tc_socket_fd, &t_wfds);
		close(conp->tc_socket_fd);
		t_trc(T_FCLOSE, 0, conp->tc_socket_fd);
	}
	tc_con_del(app, conp);
	tc_p.tc_flags.tc_nfound = 0;
	app->tc_nr_conn--;
	if ((app->tc_rconp == NULLCP) && (app->tc_apmode & T_REDIRECT))
		tc_listen(T_REDIRECT, app);

	return (T_OK);
}

/*------------------------------------------------------------------------*
 * T-DATA
 *------------------------------------------------------------------------*/

/*
 * t_vdatarq - T-DATA.request
 */
int tc_vdatarq (int *tref, struct t_data *vdata, int *vcnt, int *chain)
{
	struct tc_app	*app;
	struct tc_con	*conp;
	int	res;
	int	app_ind, con_ind;
	int	n = *vcnt;
	int	size = 0;

	app_ind = GET_APP_IND(tref);
	con_ind = GET_CON_IND(tref);
	if ((app = tc_get_apptr(app_ind)) == NULLAP
	     || app_ind >= TC_APP_MAX
	     || (conp = tc_get_conptr(app, con_ind)) == NULLCP
	     || con_ind >= TC_CON_MAX
	     || conp->tc_state != T_DATTRFRDY
	     || !FLC_CHK(conp->tc_flcontr, T_SNDGO)) {
		_t_error = T_WSEQUENCE;
		return (T_ERROR);
	}

	if ((*chain != T_MORE && *chain != T_END)
		|| n < 1 || n > T_VCNT || vdata == NULL) {
		_t_error = T_WPARAMETER;
		return (T_ERROR);
	}

	for (res = 0; res < n; res++) {
		size += vdata[res].t_datal;
		if (vdata[res].t_datap == NULL || vdata[res].t_datal < 1
		    || size > conp->tc_mxtidu) {
			_t_error = T_WPARAMETER;
			return (T_ERROR);
		}
	}

	res = tc_sndfrm(DATRQ, conp, vdata, n, size, *chain);
	if (res == T_ERROR) {
		conp->tc_state  = T_DISCONIND;
		conp->tc_length = 0;
		conp->tc_event  = T_DISIN;
		conp->tc_reason  = T_RLCONNLOST;
		_t_error = T_COLLISION;
		return (T_ERROR);
	}

	return (res);
}

/*
 * t_datarq - T-DATA.request
 */
int tc_datarq ( int *tref, char *datap, int *datal, int *chain)
{
	struct t_data	vdata;
	int	vcnt = 1;

	vdata.t_datap = datap;
	vdata.t_datal = *datal;
	return (tc_vdatarq(tref, &vdata, &vcnt, chain));
}

/*
 * t_vdatain - T-DATA.indication
 */
int tc_vdatain (int *tref, struct t_data *vdata, int *vcnt, int *chain)
{
	struct tc_app	*app;
	struct tc_con	*conp;
	int	app_ind, con_ind, res, i;
	struct t_data	aux[T_VCNT + 1], *paux;
	int	n = *vcnt;
	int	size = 0;

	app_ind = GET_APP_IND(tref);
	con_ind = GET_CON_IND(tref);
	if ((app = tc_get_apptr(app_ind)) == NULLAP
	     || app_ind >= TC_APP_MAX
	     || (conp = tc_get_conptr(app, con_ind)) == NULLCP
	     || con_ind >= TC_CON_MAX
	     || (conp->tc_state != T_DATTRFRDY
	     	&& conp->tc_state != T_LOCFLOWCON)
	     || conp->tc_event != T_DATAIN) {
		_t_error = T_WSEQUENCE;
		return (T_ERROR);
	}

	if (n < 1 || n > T_VCNT || vdata == NULL) {
		_t_error = T_WPARAMETER;
		return (T_ERROR);
	}
	for (res = 0; res < n; res++) {
		if (vdata[res].t_datap == NULL || vdata[res].t_datal < 1) {
			_t_error = T_WPARAMETER;
			return (T_ERROR);
		}
		size += vdata[res].t_datal;
	}

	/*
	 * Fetch data into buffer(s).
	 */
	if (t_du.state == EVENT) {
		/*
		 * first t_vdatain() since T_DATAIN.
		 * Either scatter entire TIDU directly to caller or scatter
		 * its begin and keep the rest.
		 */
		if (size >= t_du.cnt) {
			size = t_du.cnt;
			for (i = 0, res = 0; i < n; i++, paux++) {
				if (vdata[i].t_datal > size)
					vdata[i].t_datal = size;
				if (size > 0)
					res++;
				size -= vdata[i].t_datal;
			}

			/* struct t_data and iovec look similar */
			if (tc_rcvdatin(conp, (struct iovec *)vdata, res, t_du.cnt)) {
				conp->tc_reason = T_RLCONNLOST;
				goto bad;
			}
			t_du.state = NOBUF;
			*chain = t_du.chain = conp->tc_chain;
			conp->tc_event = T_NOEVENT;
			FLC_SET(conp->tc_flcontr, T_RCVGO);
			return (T_OK);
		} else {
			t_du.sptr = (char *) malloc((unsigned) t_du.cnt);
			if (t_du.sptr == NULL) {
				_t_error = T_ENOMEM;
				return (T_ERROR);
			}
			aux[0].t_datap = t_du.sptr;
			aux[0].t_datal = t_du.cnt;

			/* struct t_data and iovec look similar */
			if (tc_rcvdatin(conp, (struct iovec *)aux, 1, t_du.cnt)) {
				conp->tc_reason = T_RLCONNLOST;
				free(t_du.sptr);
				goto bad;
			}

			/*
			 * Scatter beginnig of TIDU into caller's buffers.
			 * Every buffer becomes full!
			 */
			t_du.state = FULL;
			t_du.aptr = t_du.sptr;
			for (res = 0; res < n; res++) {
				memcpy(vdata[res].t_datap, t_du.aptr,
					vdata[res].t_datal);
				t_du.aptr += vdata[res].t_datal;
				t_du.cnt -= vdata[res].t_datal;
			}
			*chain = t_du.chain = conp->tc_chain;
			FLC_SET(conp->tc_flcontr, T_RCVIND);
			return (t_du.cnt);
		}
	} else {
		/*
		 * Not first t_vdatain() since last T_DATAIN.
		 * There may be a TIDU already available
		 */
		if (t_du.state != FULL || *tref != t_du.tref) {
			_t_error = T_WSEQUENCE;
			return (T_ERROR);
		}
		*chain = t_du.chain;
		if (size >= t_du.cnt) {
			/*
			 * Scatter rest of TIDU into caller's buffers.
			 * None of the buffers must become full.
			 */
			for (res = 0; res < n; res++) {
				if (vdata[res].t_datal > t_du.cnt)
					vdata[res].t_datal = t_du.cnt;
				if (t_du.cnt) {
					memcpy(vdata[res].t_datap, t_du.aptr,
						vdata[res].t_datal);
					t_du.aptr += vdata[res].t_datal;
					t_du.cnt -= vdata[res].t_datal;
				}
			}
			t_du.state = NOBUF;
			free(t_du.sptr);
			conp->tc_event = T_NOEVENT;
			FLC_SET(conp->tc_flcontr, T_RCVGO);
			return (T_OK);
		} else {
			/*
			 * Scatter next junk of TIDU to caller.
			 * All buffers become full!
			 */
			for (res = 0; res < n; res++) {
				memcpy(vdata[res].t_datap, t_du.aptr,
					vdata[res].t_datal);
				t_du.aptr += vdata[res].t_datal;
				t_du.cnt -= vdata[res].t_datal;
			}
			FLC_SET(conp->tc_flcontr, T_RCVIND);
			return (t_du.cnt);
		}
	}
bad:
	FLC_SET(conp->tc_flcontr, T_RCVGO);
	conp->tc_length = 0;
	conp->tc_state  = T_DISCONIND;
	conp->tc_event  = T_DISIN;
	t_du.state = NOBUF;
	_t_error = T_COLLISION;
	return (T_ERROR);
}

/*
 * t_datain - T-DATA.indication
 */
int tc_datain (int *tref, char *datap, int *datal, int *chain)
{
	struct t_data	vdata;
	int	vcnt = 1;
	int	res;

	vdata.t_datap = datap;
	vdata.t_datal = *datal;

	res = tc_vdatain(tref, &vdata, &vcnt, chain);
	if (res != T_ERROR)
		*datal = vdata.t_datal;
	return (res);
}

/*
 * t_datastop - T-FLOWCONTROL.request
 */
int tc_datastop (int *tref)
{
	struct tc_app	*app;
	struct tc_con	*conp;
	int	app_ind, con_ind;

	app_ind = GET_APP_IND(tref);
	con_ind = GET_CON_IND(tref);
	if ((app = tc_get_apptr(app_ind)) == NULLAP
	     || app_ind >= TC_APP_MAX
	     || (conp = tc_get_conptr(app, con_ind)) == NULLCP
	     || con_ind >= TC_CON_MAX
	     || conp->tc_state != T_DATTRFRDY) {
		_t_error = T_WSEQUENCE;
		return (T_ERROR);
	}

	if (t_du.state != NOBUF && *tref == t_du.tref) {
		_t_error = T_WSEQUENCE;
		return (T_ERROR);
	}

	FLC_SET(conp->tc_flcontr, T_RCVSTOP);
	FD_CLR(conp->tc_socket_fd, &t_rfds);

	return (T_OK);
}

/*
 * t_datago - T-FLOWCONTROL.release
 */
int tc_datago (int *tref)
{
	struct tc_app	*app;
	struct tc_con	*conp;
	int	app_ind, con_ind;

	app_ind = GET_APP_IND(tref);
	con_ind = GET_CON_IND(tref);
	if ((app = tc_get_apptr(app_ind)) == NULLAP
	     || app_ind >= TC_APP_MAX
	     || (conp = tc_get_conptr(app, con_ind)) == NULLCP
	     || con_ind >= TC_CON_MAX
	     || conp->tc_state != T_DATTRFRDY) {
		_t_error = T_WSEQUENCE;
		return (T_ERROR);
	}

	FLC_SET(conp->tc_flcontr, T_RCVEVT);
	FD_SET(conp->tc_socket_fd, &t_rfds);

	return (T_OK);
}

/*------------------------------------------------------------------------*
 * T-EXPEDITED-DATA
 *------------------------------------------------------------------------*/

/*
 * t_xdatrq - T-EXPEDITED-DATA.request
 */
int tc_xdatrq (int *tref, char *datap, int *datal)
/* ARGSUSED */
{
	_t_error = T_WSEQUENCE;
	return (T_ERROR);
}

/*
 * t_xdatin - T-EXPEDITED-DATA.indication
 */
int tc_xdatin (int *tref, char *datap, int *datal)
/* ARGSUSED */
{
	_t_error = T_WSEQUENCE;
	return (T_ERROR);
}

/*
 * t_xdatstop - T-EXPEDITED-FLOWCONTROL.request
 */
int tc_xdatstop (int *tref)
/* ARGSUSED */
{
	_t_error = T_WSEQUENCE;
	return (T_ERROR);
}

/*
 * t_xdatgo - T-EXPEDITED-FLOWCONTROL.release
 */
int tc_xdatgo (int *tref)
/* ARGSUSED */
{
	_t_error = T_WSEQUENCE;
	return (T_ERROR);
}

/*------------------------------------------------------------------------*
 * T-EVENT, T-INFO
 *------------------------------------------------------------------------*/

/*
 * t_event -  T_EVENT
 */
int tc_event (int *tref, int cmode, t_opte *opt)
{
	int	evt = T_NOEVENT;
	struct tc_app	*app;
	struct tc_con	*conp;
	struct timeval	*timeout = &t;
	int	wf, rf, work;
	int	nfds = MIN(open_max,FD_SETSIZE);

	if (t_du.state != NOBUF) {
		_t_error = T_WSEQUENCE;
		return (T_ERROR);
	}
	if (opt &&
	    ((opt->opte1.t_optnr != T_OPTE1 
		&& opt->opte2.t_optnr != T_OPTE2)
		|| opt->opte1.t_timeout < T_NOLIMIT)){
		_t_error = T_WPARAMETER;
		return (T_ERROR);
	}

	/*
	 * t_event() has always cmode=T_CHECK and not wait
	 */
		t.tv_sec = 0;
		t.tv_usec = 0;

	/*
	 * Loop through all connect elements and report an unhandled event
	 * THIS SHOULD BE DONE IN FASHION ALWAYS SHOWING THE OLDEST EVENT FIRST!
	 */
	if ((app = tc_p.tc_apptr) == NULLAP)
		abort();

	do {
		if ((conp = app->tc_conp) != NULLCP) {
			do {
				switch (conp->tc_state) {
				case T_OUTCONCONF:
				case T_INCONIND:
				case T_REDCONIND:
				case T_DISCONIND:
					if (!evt) {
						*tref = SET_TREF(app->tc_ind,
						   	 conp->tc_ind);
						evt = conp->tc_event;
						app->tc_conp = conp;
					}
					break;

				case T_DATTRFRDY:
					if (conp->tc_event == T_DATAIN
					     && !FLC_CHK(conp->tc_flcontr, T_RCVSTOP)
					     && !evt) {
						*tref = SET_TREF(app->tc_ind,
							conp->tc_ind);
						FLC_SET(conp->tc_flcontr, T_RCVIND);
						evt = conp->tc_event;
						app->tc_conp = conp;
					}
				}
				conp = conp->tc_nxt;
			} while (app->tc_conp != conp);
		}
		app = app->tc_nxt;
	} while (tc_p.tc_apptr != app);

	if (evt)
		goto out;

	/*
	 * No event currently in state tables pending.
	 * Check the fd's in the mask for events not yet processed
	 */
	while (tc_p.tc_flags.tc_nfound > 0) {
		evt = chck_selres(tref);
		if ((evt != T_NOEVENT) && (cmode == T_CHECK))
			goto out;
	}

	/*
	 * Still no event, have to T_WAIT for one.
	 * Loop through all connect elements and prepare fd mask for select()
	 */
	FD_ZERO(&rfds);
	rf = 0;
	FD_ZERO(&wfds);
	wf = 0;
	do {
		if ((conp = app->tc_conp) != NULLCP) {
			do {
				switch (conp->tc_state) {
				case T_OUTCONNECT:
					FD_SET(conp->tc_socket_fd, &wfds);
					wf++;
					break;

				case T_OUTCONPEND:
				case T_ACCEPT:
					FD_SET(conp->tc_socket_fd, &rfds);
					rf++;
					break;

				case T_DATTRFRDY:
					if (!FLC_CHK(conp->tc_flcontr, T_RCVSTOP)) {
						FD_SET(conp->tc_socket_fd, &rfds);
						rf++;
					}
					break;

				case T_LOCFLOWCON:
					FD_SET(conp->tc_socket_fd, &rfds);
					rf++;
					FD_SET(conp->tc_socket_fd, &wfds);
					wf++;
					break;
				}
				conp = conp->tc_nxt;
			} while (app->tc_conp != conp);
		}
		if ((conp = app->tc_pconp) != NULLCP) {
			FD_SET(conp->tc_socket_fd, &rfds);
			rf++;
		}
		if ((conp = app->tc_rconp) != NULLCP) {
			FD_SET(conp->tc_socket_fd, &rfds);
			rf++;
		}
		app = app->tc_nxt;
	} while (tc_p.tc_apptr != app);

	/*
	 * Do the select().
	 * T_CHECK returns, when no fd was selected or interrupted.
	 * If select() return -1, 'tc_p.tc_flags.tc_nfound'
	 * (unsigned int tc_nfound : 8 ;  field bit ) contains 255 
	 * (instead -1)
	 */
	t_trc(T_FTCPSEL, 0, nfds, &rfds, &wfds, NULL, timeout, 0, 0);

	work=select(nfds, rf ? &rfds : NULL, wf ? &wfds : NULL, NULL, timeout);

	t_trc(T_FTCPSEL, 1, nfds, &rfds, &wfds, NULL, timeout,
		work, errno);

	if (work > 0)
		tc_p.tc_flags.tc_nfound = work;
	else {
		tc_p.tc_flags.tc_nfound = 0;
		evt = T_NOEVENT;
		goto out;
	}
	while (tc_p.tc_flags.tc_nfound > 0) {
		evt = chck_selres(tref);
		if (evt != T_NOEVENT)
			goto out;
	}

	/*
	 * All done. Return event and possibly tref and options 
	 */
out:
	if (evt != T_NOEVENT && evt != T_ERROR && evt != T_RETRY) {
		app = tc_get_apptr(GET_APP_IND(tref));
		conp = tc_get_conptr(app, GET_CON_IND(tref));

		if (opt && opt->opte1.t_optnr == T_OPTE1) {
			opt->opte1.t_attid = app->tc_ind;
			opt->opte1.t_uattid = app->tc_uattid;
			opt->opte1.t_ucepid = conp->tc_ucepid;
			if (evt == T_DATAIN)
				opt->opte1.t_evdat = conp->tc_udatal;
			else
				opt->opte1.t_evdat = 0;
		}

		if (evt == T_DATAIN) {
			t_du.attid = app->tc_ind;
			t_du.tref = *tref;
			t_du.cnt = conp->read_size;
			t_du.state = EVENT;
		}
	}

	return (evt);
}

/*
 * Check what remains to be reported from previous select().
 * Handle 1 read/write condition pending and adjust params for next call
 */
static int chck_selres(int	*tref)
{
	struct tc_app	*app;
	struct tc_con	*conp;
	int	res_pen;
	register int	i;
	static int	curfd = 0;
	int	nfds = MIN(open_max,FD_SETSIZE);

	/*
	 * Determine which fd has something
	 * THIS SHOULD BE DONE IN A ROUND-ROBIN FASHION TO FAIRLY PROCESS
	 * MULTIPLE FDs
	 */
	res_pen = 0;
	for (i = 0; i < nfds; i++) {
		if (++curfd >= nfds)
			curfd = 0;
		if (FD_ISSET(curfd, &wfds)) {
			res_pen = WRITEP;
			FD_CLR(curfd, &wfds);
			break;
		}
		if (FD_ISSET(curfd, &rfds)) {
			res_pen = READP;
			FD_CLR(curfd, &rfds);
			break;
		}
	}

	if (res_pen == 0) {
		FD_ZERO(&rfds), FD_ZERO(&wfds);
		tc_p.tc_flags.tc_nfound = 0;
		return (T_NOEVENT);
	}
	tc_p.tc_flags.tc_nfound--;

	/*
	 * get app and conp holding curfd to construct tref
	 */
	if ((app = tc_p.tc_apptr) != NULLAP) {
		do {
			if ((conp = app->tc_conp) != NULLCP) {
				do {
					if (conp->tc_socket_fd == curfd) {
						*tref = SET_TREF(app->tc_ind, conp->tc_ind);
						goto out;
					}
					conp = conp->tc_nxt;
				} while (app->tc_conp != conp);
			}
			if (((conp = app->tc_pconp) != NULLCP) && (conp->tc_socket_fd == curfd))
				goto out;
			if (((conp = app->tc_rconp) != NULLCP) && (conp->tc_socket_fd == curfd))
				goto out;
			app = app->tc_nxt;
		} while (tc_p.tc_apptr != app);
	}

	return (T_NOEVENT);

	/*
	 * handle pending event in application app connection conp
	 * and return event
	 */
out:
	switch (conp->tc_state) {
	case T_IDLE:
		return (c1_hdlr(app, conp, tref));

	case T_ACCEPT:
		return (c11_hdlr(app, conp, tref));

	case T_OUTCONNECT:
		return (c3_hdlr(app, conp));

	case T_OUTCONPEND:
		return (c2_hdlr(app, conp));

	case T_DATTRFRDY:
		return (c4_hdlr(app, conp, res_pen));

	case T_LOCFLOWCON:
		if (res_pen == READP)
			return (c4_hdlr(app, conp, res_pen));
		else
			return (c8_hdlr(conp));

	default:
		abort();
		return (T_NOEVENT);
	}
}

/*
 * Handle events arriving in IDLE state, i.e.
 * incoming and/or redirected connections
 */
static struct sockaddr_in sinhim;
static int c1_hdlr(struct tc_app *app, struct tc_con *conp, int *tref)
{
	int	mode, k, fda;

	mode = (conp == app->tc_pconp) ? T_PASSIVE : 
		(conp == app->tc_rconp) ? T_REDIRECT : 0;
	if (mode == 0)
		return (T_NOEVENT);

	/*
	 * Allocate and init a connnect element, accept connection on
	 * passive or redirect socket. T_NOEVENT, if this fails.
	 */
	if ((conp = (struct tc_con *)
	    calloc(sizeof(struct tc_con), sizeof(char))) == NULLCP)
		return (T_NOEVENT);
	FLC_INI(conp->tc_flcontr);
	conp->savindata = NULL;
	conp->savoutdata = NULL;
	conp->read_state = R_IDLE;

	sinhim.sin_family = AF_INET;
	k = sizeof(sinhim);
	if (mode == T_PASSIVE) {
		fda = accept(app->tc_pconp->tc_socket_fd,
			(struct sockaddr *) &sinhim, &k);
		t_trc(T_FTCPACC, 0, app->tc_pconp->tc_socket_fd,
			&sinhim, fda, errno);
	} else {
		fda = accept(app->tc_rconp->tc_socket_fd,
			(struct sockaddr *) &sinhim, &k);
		t_trc(T_FTCPACC, 0, app->tc_rconp->tc_socket_fd,
			&sinhim, fda, errno);
	}
	 
	if (fda < 0) {
		tc_p.tc_flags.tc_nfound = 0;
		switch (errno) {
		case EIO:    /* I/O Interrupt */
		case EPIPE:  /* Broken pipe */
			return (T_CCP_END);
		case EWOULDBLOCK:  /* would block */
/*		case EAGAIN:          equal to EWOULDBLOCK */
			return (T_RETRY);
		default:
			return (T_NOEVENT);
		}
	}
	 
	/*
	 * If t_conlim connections already established, reject another
	 * incoming connection (here a redirected connections is always
	 * acceptable, since it is inhibited elsewhere)
	 */
	conp->tc_socket_fd = fda;

	if ((app->tc_nr_conn == app->tc_conlim) && (app->tc_pconp != NULL)) {
		(void) tc_sndfrm(DISRQ, conp, T_USER, NULL);
		close(conp->tc_socket_fd);
		FD_CLR(conp->tc_socket_fd,&t_fdset);
		FD_CLR(conp->tc_socket_fd, &t_wfds);
		FD_CLR(conp->tc_socket_fd, &t_rfds);
		t_trc(T_FCLOSE, 0, conp->tc_socket_fd);
		free((char *)conp);
		tc_p.tc_flags.tc_nfound = 0;
		return (T_NOEVENT);
	}

	conp->tc_state=T_ACCEPT;
  	FD_SET(conp->tc_socket_fd, &t_fdset);
	FD_SET(conp->tc_socket_fd, &t_rfds);
	tc_con_ins(app, conp);
	app->tc_nr_conn++;
	*tref = SET_TREF(app->tc_ind, conp->tc_ind);

	return (T_RETRY);
}

static int c11_hdlr(struct tc_app *app, struct tc_con *conp, int *tref)
{
	int code,k;

	tc_setsock(conp->tc_socket_fd,TC_BLOCK);

	/*
	 * read and interpret passive-/redirect-frame
	 */
	res = tc_rcvhdr(conp, &code);
	if (res == T_RETRY)
		return (T_RETRY);

	conp->read_state = R_ENDDATA;

	if (res == T_ERROR) {
		close(conp->tc_socket_fd);
		FD_CLR(conp->tc_socket_fd,&t_fdset);
		FD_CLR(conp->tc_socket_fd,&t_rfds);
		t_trc(T_FCLOSE, 0, conp->tc_socket_fd);
		tc_con_del(app, conp);
		tc_p.tc_flags.tc_nfound = 0;
		return (T_NOEVENT);
	}

	switch (code) {
	case CONIN:
		conp->tc_inaddr.s_addr = sinhim.sin_addr.s_addr;
		conp->tc_event = T_CONIN;
		conp->tc_state = T_INCONIND;
		k = T_CONIN;
		break;

	case REDIN:
		conp->tc_event = T_REDIN;
		conp->tc_state = T_REDCONIND;
		k = T_REDIN;
		break;

	default:
		close(conp->tc_socket_fd);
		FD_CLR(conp->tc_socket_fd,&t_fdset);
		FD_CLR(conp->tc_socket_fd,&t_rfds);
		t_trc(T_FCLOSE, 0, conp->tc_socket_fd);
		tc_con_del(app, conp);
		app->tc_nr_conn--;
		tc_p.tc_flags.tc_nfound = 0;
		return (T_NOEVENT);
	}

	/*
	 * Link new connect element and set tref.
	 * If t_conlim is exhausted, inhibit here further redirected
	 * connections by unlinking the connect element (implicitely
	 * closing the socket)
	 */

	if ((app->tc_nr_conn == app->tc_conlim) && (app->tc_rconp != NULL))
		tc_rd_free(app);

	return (k);
}

/*
 * Handle events arriving in OUTGOING_CONNECT state,
 * i.e. waiting for response of connect
 */
static int c3_hdlr(struct tc_app *app, struct tc_con *conp)
/* ARGSUSED */
{
	FD_CLR(conp->tc_socket_fd, &t_wfds);
	FD_SET(conp->tc_socket_fd, &t_rfds);
	if (tc_sndfrm(CONRQ, conp, app->tc_portnr, NULL)) {
		conp->tc_length = 0;
		conp->tc_event = T_DISIN;
		conp->tc_state = T_DISCONIND;
		conp->tc_reason = T_RSAPNOTATT;
		return (T_DISIN);
	}
	else {
		conp->tc_event = T_NOEVENT;
		conp->tc_state = T_OUTCONPEND;
		return(T_RETRY);
	}
}

/*
 * Handle events arriving in OUTGOING_CONNECTION_PENDING state,
 * i.e. confirmations or disconnections
 */
static int c2_hdlr(struct tc_app *app, struct tc_con *conp)
/* ARGSUSED */
{
	int	code, res;

	res = tc_rcvhdr(conp, &code);
	if (res == T_RETRY)
		return (T_RETRY);

	conp->read_state = R_ENDDATA;

	if (res == T_OK) {
		if (code == CONCF) {
			conp->tc_event = T_CONCF;
			conp->tc_state = T_OUTCONCONF;
			return (T_CONCF);
		}
		if (code == DISIN) {
			conp->tc_event = T_DISIN;
			conp->tc_state = T_DISCONIND;
			return (T_DISIN);
		}
	}
	conp->tc_length = 0;
	conp->tc_event = T_DISIN;
	conp->tc_state = T_DISCONIND;
	return (T_DISIN);
}

/*
 * Handle events arriving in DATA_TRANSFER_READY state,
 * i.e. data indications or disconnections (redirections and incoming
 * connections are are handeled seperately) also window updates
 */
static int c4_hdlr(struct tc_app *app, struct tc_con *conp, int	res_pen)
/* ARGSUSED */
{
	int	code, res;

	switch (res_pen) {
	case READP:
		res = tc_rcvhdr(conp, &code);
		if (res == T_RETRY)
			return (T_RETRY);

		if (res == T_OK) {
			if (code == DATIN) {
				if (tc_read_data(conp,conp->tc_udatal+1)==T_OK){
					conp->tc_event = T_DATAIN;
					conp->read_state = R_ENDDATA;
					FLC_SET(conp->tc_flcontr, T_RCVIND);
					return (T_DATAIN);
				}
				else {
				/* not all data read */
					conp->read_state = R_INDATA;
					FLC_SET(conp->tc_flcontr, T_RCVIND);
					return(T_RETRY);
				}
			}
			if (code == DISIN) {
				conp->tc_event = T_DISIN;
				conp->tc_state = T_DISCONIND;
				return (T_DISIN);
			}
			if (code == ACKIN) {
				if (tc_rcvackin(conp))
					break;
				if (FLC_CHK(conp->tc_flcontr, T_SNDSTOP)) {
					FLC_SET(conp->tc_flcontr, T_SNDGO);
					conp->read_state = R_ENDDATA;
					return (T_DATAGO);
				}
				return (T_NOEVENT);
			}
		}
		break;

	case WRITEP:
		if (FLC_CHK(conp->tc_flcontr, T_SNDSTOP)) {
			FLC_SET(conp->tc_flcontr, T_SNDGO);
			FD_CLR(conp->tc_socket_fd, &t_wfds);
			return (T_DATAGO);
		}
		return (T_NOEVENT);
		/* NOTREACHED */
	}
	conp->tc_length = 0;
	conp->tc_event = T_DISIN;
	conp->tc_state = T_DISCONIND;
	return (T_DISIN);
}

/* 
 * Handle local flow control
 */
static int c8_hdlr(struct tc_con *conp)
{
int	n;

	n = conp->fc_write->iov_len; /* length of rest-data */
	if (tc_writev(conp, conp->tc_socket_fd, conp->fc_write, 1, n) == n) {
		/* all data transferred */
		conp->tc_state = T_DATTRFRDY;
		/* no longer wait for write-events */
		FD_CLR(conp->tc_socket_fd, &t_wfds);
		return(T_DATAGO);
	} else
		/* more data to transfer */
		return(T_RETRY);

	/* CANNOT HAPPEN */
}
		
/*
 * t_info - T-INFO
 */
int tc_info(int *tref, t_opti *opt)
{
	struct tc_app	*app;
	struct tc_con	*conp;
	int	app_ind, con_ind;

	if (!opt || (opt->opti1.t_optnr != T_OPTI1
		&& opt->opti3.t_optnr != T_OPTI3)) {
		_t_error = T_WPARAMETER;
		return (T_ERROR);
	}

	app_ind = GET_APP_IND(tref);
	con_ind = GET_CON_IND(tref);
	if ((app = tc_get_apptr(app_ind)) == NULLAP
	     || app_ind >= TC_APP_MAX
	     || (conp = tc_get_conptr(app, con_ind)) == NULLCP
	     || con_ind >= TC_CON_MAX
	     || conp->tc_state != T_DATTRFRDY) {
		_t_error = T_WSEQUENCE;
		return (T_ERROR);
	}
	opt->opti1.t_maxl = conp->tc_mxtidu;
	if (opt->opti3.t_optnr == T_OPTI3) {
		opt->opti3.t_fdsetsize = FD_SETSIZE;
		opt->opti3.t_openmax = open_max;
		opt->opti3.t_tsdu = -1;
		opt->opti3.t_etsdu = -2;
		opt->opti3.t_condata = T_CON_SIZE;
		opt->opti3.t_concfdata =  T_CON_SIZE;
		opt->opti3.t_disdata = T_DIS_SIZE;
	}
	return (T_OK);
}
