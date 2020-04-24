/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cmx3.c,v $
 * Revision 1.1.5.2  1996/03/11  13:24:06  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:12:34  marty]
 *
 * Revision 1.1.5.1  1995/12/08  17:47:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:24:59  root]
 * 
 * Revision 1.1.2.3  1994/07/06  15:08:24  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:21:29  marrek]
 * 
 * Revision 1.1.2.2  1994/06/10  21:16:27  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:54:26  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/10  16:15:05  marrek
 * 	Created for April Submission.
 * 	[1994/04/26  12:50:25  marrek]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: cmx3.c,v $ $Revision: 1.1.5.2 $ $Date: 1996/03/11 13:24:06 $";
#endif

/*
 * COPYRIGHT (C) 1990 BY SIEMENS AG  All rights reserved
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

/****************************************************************************/
/*                                                                          */
/*              ICMX(L) INTERFACE FOR COMPUTER INTERCONNECTION              */
/*              ==============================================              */
/*                                                                          */
/* This code is a customization of ICMX(S3) as of CMX V3.0, starting with   */
/* the appropriate implementation for ICMX(S3) as of CMX V2.5.              */
/*                                                                          */
/*                                                                          */
/* Author: bee                                            Date:   31.05.90  */
/* Modifications:                                                           */
/*        kam   V31                                               24.02.92  */
/*        mai   PCMX - Dummyfunctions                             09.12.92  */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/****************************************************************************/

#ifndef PCMX

#ifdef THREADSAFE
# include <pthread.h>
#endif

#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include "tnsx.h"
#include "cmx.h"
#include "cmxextern.h"
#include "cmxaddr.h"
#include "cxioctl.h"
#include "cmxl3.h"
#include "cmxtrace.h"

static char sccsid[] = "@(#)cmx3.c	311.26 93/08/24 CMX (NC1)";

/**************************************************************************/
/*      D E C L A R A T I O N    I N T E R N A L    D A T A               */
/**************************************************************************/

/*
 * Legal pids for t_redirect() are between SYSPID_MAX and PID_MAX.
 * If SYSPID_MAX is not defined, assume that pids 1, 2, 3 are for the system.
 */
#ifndef SYSPID_MAX
#define SYSPID_MAX	3
#endif

static struct cx_upar_t upar;   /* Structures for ioctl() calls */
static int      t_mypid;        /* Own pid */
static char     *hname;         /* For gateway, buffered space, loc. name */

static struct du_buf    t_du;   /* TIDU buffer */
static struct du_buf    t_xdu;  /* ETSDU buffer */
static char		xdata[T_EXP_SIZE];
static int		gatewayb = 0;
static char t_gatex[] = "$GATEX";
static char t_trsna[] = "$TRSNA";
static int		t_netopen(),t_getrout();

/*
 * Parameter structures for TNS.
 * Required to determine ROUTINGINFORMATION from toaddr during t_conrq().
 */
static Ts_head  t_head = { 
	TS_1VER01
};

static Ts_leaf_name	t_sname = { 0 };
static Ts_leaf_name	t_fname;
static char		t_vlist[NAMLENGTH];
static Ts_filter	t_filter = { TS_TRANS };

static Ts_P11		t_ts11 = {
	&t_head, TS_CMX_DIR_ID, &t_sname, TS_GENERAL,
	&t_filter, TS_LOCAL, NULL, NULL, t_vlist,
	sizeof (t_vlist), 1, &t_fname, NULL
};

static Ts_property	t_property[2] = {
	TS_ROUT, TS_ITEM, NULL, NULL, TS_EMPTYPROP
};

	Ts_route        envrout;        /* Routing information in CMXINIT */
static Ts_route		routinfo;
static Ts_P13 		t_ts13 = {
	&t_head, TS_CMX_DIR_ID, &t_fname, 0L,
	(char *) &routinfo, sizeof (Ts_route), &t_property[0],
	NULL, NULL, NULL
};

/*------------------------------------------------------------------------*
 * T-ATTACH 
 *------------------------------------------------------------------------*/

/*
 * t_attach - T-ATTACH.request
 */
t_attach(name, opt)
char    *name;                /* actually: struct t_myname *name; */
t_opta	*opt;
{
	register struct cx_apar_t	*ap = (struct cx_apar_t *) &upar.upu;
	int	res;
	char	*cp;
        int     i,j=80;
	int 	ccid;
	short	adid;
static	short	wlist[] = {T_W1,T_W2,T_W3,T_W4,T_W5,T_W6};
static	short	llist[] = {T_L1,T_L2,T_L3,T_L4,T_L5,T_L6};

	/*
	 * Checking options, name already checked
	 */
	if (opt != NULL)
	{ 
		if ((opt->opta1.t_optnr != T_OPTA1 
			&& opt->opta2.t_optnr != T_OPTA2 
			&& opt->opta4.t_optnr != T_OPTA4
			&& opt->opta5.t_optnr != T_OPTA5
			&& opt->opta6.t_optnr != T_OPTA6)
		|| !(opt->opta1.t_apmode & (T_ACTIVE|T_PASSIVE|T_REDIRECT))
		|| (opt->opta1.t_apmode & ~(T_ACTIVE|T_PASSIVE|T_REDIRECT))
		|| opt->opta1.t_conlim == 0
		|| opt->opta1.t_conlim < T_NOLIMIT)
		{
			_t_error = T_WPARAMETER;
			return (T_ERROR);
		}
		if (opt->opta4.t_optnr == T_OPTA4
		&&  opt->opta4.t_mignea != T_NO
		&&  opt->opta4.t_mignea != T_YES)
		{
			_t_error = T_WPARAMETER;
			return (T_ERROR);
		}
	}

	/*
	 * Modify the environment inherited by fork(). (Close CMX-fd
	 * note pid)
	 */
	if (t_mypid == 0 || (t_mypid != 0 && t_mypid != t_pid)) {
		if (cmx_fd > -1) {
			close(cmx_fd);
			t_trc(T_FCLOSE, 0, cmx_fd);
			FD_CLR(cmx_fd, &t_fdset);
			FD_CLR(cmx_fd, &t_rfds);
			FD_CLR(cmx_fd, &t_wfds);
			FD_CLR(cmx_fd, &t_xfds);
			cmx_fd = -1;
		}
		if (t_du.state != NOBUF)
			free(t_du.sptr);
		if (t_xdu.state != NOBUF)
			free(t_du.sptr);
		t_du.state = t_xdu.state = NOBUF;
		t_mypid = t_pid;
	}

	/*
	 * Get a new CMX-fd. CLOSE_ON_EXEC is required because according
	 * to exec() the cmx-fd check failed.
	 */
	if (cmx_fd == -1) {
		if ((cmx_fd = t_netopen()) == T_ERROR)
			return (T_ERROR);
		FD_SET(cmx_fd, &t_fdset);
		FD_SET(cmx_fd, &t_rfds);
		FD_SET(cmx_fd, &t_wfds);
		FD_SET(cmx_fd, &t_xfds);
		if (fcntl(cmx_fd, F_SETFD, CLOSE_ON_EXEC) == T_ERROR) {
			_t_error = errno;
			return (T_ERROR);
		}
	}

	cp = ((struct sbka_sel *)((struct cx_myname *)((struct t_myname *) name)->t_mn)->cxn_sel)->ks_rest;
	if (strncmp(cp,t_gatex,(size_t) 6) == 0 || strncmp(cp,t_trsna,(size_t) 6) == 0)
		gatewayb = 1;
	/*
	 * Log on to CMX automaton via ICMX(S3)
	 */
	if (gatewayb == 1) {
		upar.up_subtype = CX_APAR0;
		ap->ap_type = CX_GWD;
		hname = name;
	}
	else	
		upar.up_subtype = CX_APAR;
	ap->ap_pusapname = name;
	ap->ap_lsapname = ((struct t_myname *) name)->t_mnlng;
	ap->ap_libversion = T_LIBV40;
	ap->ap_signal = t_cmxsig;
	if (opt != NULL) {
		ap->ap_mode = opt->opta2.t_apmode;
		ap->ap_nconcnt = opt->opta2.t_conlim;
		upar.up_uattid =
			((opt->opta2.t_optnr == T_OPTA2) || 
			 (opt->opta2.t_optnr == T_OPTA5) || 
			 (opt->opta2.t_optnr == T_OPTA6)) ? opt->opta2.t_uattid : 0;
		upar.up_mignea =
			opt->opta4.t_optnr == T_OPTA4 ? opt->opta4.t_mignea : 0;
	} else {
		ap->ap_mode = (T_ACTIVE|T_PASSIVE|T_REDIRECT);
		ap->ap_nconcnt = 1;
		upar.up_uattid = 0;
		upar.up_mignea = 0;
	}
       
	res = t_ioctl(cmx_fd, ST_SCMD(STINIRQ, CX_ASIZE), &upar);

	/*
	 * ATTACH reference for the CMX automaton and info, with which CCs and
	 * CCPs were logged on (successfully).
	 */
	 if (res == T_OK || res == T_NOTFIRST) {
		if (opt != NULL 
                && ((opt->opta2.t_optnr == T_OPTA2 ) ||
		    (opt->opta5.t_optnr == T_OPTA5 ) ||
		    (opt->opta6.t_optnr == T_OPTA6 ))) {
			opt->opta2.t_attid = upar.up_aattid;
     			opt->opta2.t_sptypes |= ap->ap_sptypes;
			t_addcclst((struct t_cclst *)&ap->ap_cclist);
		}
		if (opt != NULL
		&& ((opt->opta2.t_optnr == T_OPTA2) ||
		    (opt->opta5.t_optnr == T_OPTA5))
		&& (ap->ap_cclist.cl_num != CMX_ALLCCIDS)) {
			for ( i = 0; i < (int)ap->ap_cclist.cl_num; i++) {
				adid = CX_GCCADID(ap->ap_cclist.cl_id[i]);
				switch(adid){
				case CX_WANADID:
					ccid = CX_GINCC(ap->ap_cclist.cl_id[i]);
					opt->opta2.t_ccbits |= (int)wlist[ccid];
					break;
				case CX_LANADID:
					ccid = CX_GINCC(ap->ap_cclist.cl_id[i]);
					opt->opta2.t_ccbits |= (int)llist[ccid];
					break;
				case CX_LBS2ADID:
					opt->opta2.t_ccbits |= (int)T_LBS2;
					break;
				case CX_LOOPADID:
					opt->opta2.t_ccbits |= (int)T_LOOP;
					break;
				}
			}
		}
	}
	return (res);
}

/*
 * t_detach - T-ATTACH.release
 */
t_detach(name)
char    *name;                          /* Actually: struct t_myname *name */
{
	register struct cx_apar_t	*ap = (struct cx_apar_t *) &upar.upu;
	int	res;

	/*
	 * Logging off the CMX automaton via ICMX(S3)
	 */
	upar.up_subtype = CX_APAR;
	ap->ap_pusapname = name;
	ap->ap_lsapname = ((struct t_myname *) name)->t_mnlng;

	res = t_ioctl(cmx_fd, ST_SCMD(STTRMRQ, CX_ASIZE), &upar);

	/*
	 * Release TIDU and ETSDU buffer completely, because the caller has used
	 * dem t_detach() to specify that no more data is to be exchanged for
	 * this tref. The buffer is identified by the ATT-Id delivered by the
	 * CMX automaton; the Id is therefore only meaningful after successfully
	 * logging off.
	 */
	if (res != T_ERROR) {
		if (t_du.state != NOBUF && t_du.attid == upar.up_aattid) {
			if (t_du.state == FULL)
				free(t_du.sptr);
			t_du.state = NOBUF;
		}
		if (t_xdu.state != NOBUF && t_xdu.attid == upar.up_aattid) {
			if (t_xdu.state == FULL)
				free(t_xdu.sptr);
			t_xdu.state = NOBUF;
		}
	}
	return (res);
}

/*------------------------------------------------------------------------*
 * T-CONNECTION
 *------------------------------------------------------------------------*/

/*
 * t_conrq - T-CONNECTION.request
 */
t_conrq(tref, toaddr, fromaddr, opt)
int	*tref;
union t_address	*toaddr, *fromaddr;
t_opt13	*opt;
{
	register struct cx_cpar_t	*cp = (struct cx_cpar_t *) &upar.upu;
	int	res;
	static int	checkenv = 1;

	/*
	 * Checking parameters and options
	 */
	if (((fromaddr->tmyname.t_mnmode != T_MNMODE) &&
		(fromaddr->tmyname.t_mnmode != T_PAMODE)) ||
		(toaddr->tpartaddr.t_pamode != T_PAMODE)) {
		_t_error = T_WPARAMETER;
		return (T_ERROR);
	}
	if (opt != NULL &&
	     ((opt->optc3.t_optnr != T_OPTC1 && opt->optc3.t_optnr != T_OPTC3)
	   	|| opt->optc3.t_udatal < 0
	   	|| opt->optc3.t_udatal > T_MSG_SIZE
	   	|| opt->optc3.t_xdata & ~(T_YES | T_NO)
	   	|| opt->optc3.t_timeout < T_NOLIMIT
	   	|| (opt->optc3.t_udatal > 0 && opt->optc3.t_udatap == NULL))) {
		_t_error = T_WPARAMETER;
		return (T_ERROR);
	}

	/*
	 * Determining ROUTINGINFORMATION.
	 * ROUTINGINFORMATION is determined in the order 1) from the environment 2) from TNS
	 * via toaddr 3) default.
	 * 2) and 3) are defined in getcc.
	 * Determine which CC was used for setup.
	 * In the "old" CMX V2.0 format, only ONE of the CCs W1, W2, L1 (0, 1, 2)
	 * was possible.
	 * CMX V2.1 contained a mask of bits (larger CX_NCCBITS) with
	 * ORed bits for W1 (B5), W2 (B6), L1 (B7), L2 (B8),
	 * W[3-6] (B[9-12]).
	 * The CMX V2.5 format and the V3.0 format are formed by means of ">> 5".
	 * In CMX V3.1, bit lists no longer exist, only
	 * cc-id-arrays. The bit lists supplied must therefore be mapped with
	 * getcc.
	 */
	if (envrout)
	{
		if ( _t_error = t_getcc(NULL, &(cp->cp_cclist), envrout))
			return (T_ERROR);
	}
	else {
		checkenv = 0;  /* If no environment exists, do not check */
		if ( _t_error = t_getcc(toaddr, &(cp->cp_cclist), NULL))
			return (T_ERROR);
	}

	/*
	 * T-CONNECTION.request to CMX automaton via ICMX(S3)
	 */
	if (gatewayb == 1) {
		upar.up_subtype = CX_CPAR0;
		cp->cp_pumadd = (char *) fromaddr;
		cp->cp_lmadd = ((struct t_partaddr *) fromaddr)->t_palng;
		cp->cp_pusapname = (char *) hname;
		cp->cp_lsapname = ((struct t_myname *) hname)->t_mnlng;
	}
	else {
		upar.up_subtype = CX_CPAR;
		cp->cp_pusapname = (char *) fromaddr;
		cp->cp_lsapname = ((struct t_myname *) fromaddr)->t_mnlng;
	}
	cp->cp_purmadd = cp->cp_purpadd = cp->cp_puopt = (char *) NULL;
	cp->cp_lrmadd  = cp->cp_lrpadd  = cp->cp_lopt  = 0;
	cp->cp_pupadd = (char *) toaddr;
	cp->cp_lpadd = ((struct t_partaddr *) toaddr)->t_palng;
	if (opt != NULL) {
		upar.up_xdt = opt->optc3.t_xdata == T_YES ? CX_XDT : 0;
		upar.up_ucepid =
			opt->optc3.t_optnr == T_OPTC3 ? opt->optc3.t_ucepid : 0;
		cp->cp_pudat = opt->optc3.t_udatap;
		cp->cp_ldat = opt->optc3.t_udatal;
		cp->cp_maxinactsec = opt->optc3.t_timeout;
	} else
		upar.up_xdt = upar.up_ucepid =
			cp->cp_ldat = cp->cp_maxinactsec = 0;

	res = t_ioctl(cmx_fd, ST_SCMD(STCONRQ, CX_CSIZE), &upar);

	if (res != T_ERROR)
		*tref = upar.up_acepid;
	return (res);
}

/*
 * t_conin - T-CONNECTION.indication
 */
t_conin(tref, toaddr, fromaddr, opt)
int	*tref;
union t_address *toaddr, *fromaddr;
t_opt1	*opt;
{
	register struct cx_cpar_t	*cp = (struct cx_cpar_t *) &upar.upu;
	int	res;

	/*
	 * Checking options
	 */
	if (opt != NULL
	    && (opt->optc1.t_optnr != T_OPTC1 || opt->optc1.t_udatal < 0)) {
		_t_error = T_WPARAMETER;
		return (T_ERROR);
	}

	/*
	 * T-CONNECTION.indication from CMX automaton via ICMX(S3),
	 * supplies return
	 */
	if (gatewayb == 1) {
		upar.up_subtype = CX_CPAR0;
		cp->cp_purmadd = (char *) toaddr;
		cp->cp_lrmadd = sizeof (union t_address);
	}
	else
		upar.up_subtype = CX_CPAR;
	cp->cp_pusapname = (char *) toaddr;
	cp->cp_lsapname = sizeof (union t_address);
	upar.up_acepid = *tref;
	cp->cp_purpadd = (char *) fromaddr;
	cp->cp_lrpadd = sizeof (union t_address);
	if (opt != NULL) {
		cp->cp_purdat = opt->optc1.t_udatap;
		cp->cp_lrdat = opt->optc1.t_udatal;
	} else
		cp->cp_lrdat = 0;

	res = t_ioctl(cmx_fd, ST_SCMD(STCONIN, CX_CSIZE), &upar);

	if (opt != NULL && res == T_OK) {
		opt->optc1.t_udatal = cp->cp_lrdat;
		opt->optc1.t_xdata = upar.up_xdt == CX_XDT ? T_YES : T_NO;
		opt->optc1.t_timeout = T_NO;
	}
	return (res);
}

/*
 * t_conrs - T-CONNECTION.response
 */
t_conrs(tref, opt)
int	*tref;
t_opt13	*opt;
{
	register struct cx_cpar_t	*cp = (struct cx_cpar_t *) &upar.upu;

	/*
	 * Checking options
	 */
	if (opt != NULL &&
	     ((opt->optc3.t_optnr != T_OPTC1 && opt->optc3.t_optnr != T_OPTC3)
		|| opt->optc3.t_udatal < 0
	   	|| opt->optc3.t_udatal > T_MSG_SIZE
		|| opt->optc3.t_xdata & ~(T_YES | T_NO)
		|| opt->optc3.t_timeout < T_NOLIMIT
	        || (opt->optc3.t_udatal > 0 && opt->optc3.t_udatap == NULL))) {
		_t_error = T_WPARAMETER;
		return (T_ERROR);
	}

	/*
	 * T-CONNECTION.response to CMX automaton via ICMX(S3)
	 */
	upar.up_subtype = CX_CPAR;
	upar.up_acepid = *tref;
	if (opt != NULL) {
		upar.up_xdt = opt->optc3.t_xdata == T_YES ? CX_XDT : 0;
		upar.up_ucepid =
			opt->optc3.t_optnr == T_OPTC3 ? opt->optc3.t_ucepid : 0;
		cp->cp_pudat = opt->optc3.t_udatap;
		cp->cp_ldat = opt->optc3.t_udatal;
		cp->cp_maxinactsec = opt->optc3.t_timeout;
	} else
		upar.up_xdt = upar.up_ucepid =
			cp->cp_ldat = cp->cp_maxinactsec = 0;

	return (t_ioctl(cmx_fd, ST_SCMD(STCONRS, CX_CSIZE), &upar));
}

/*
 * t_concf - T-CONNECTION.confirmation
 */
t_concf(tref, opt)
int	*tref;
t_opt1	*opt;
{
	register struct cx_cpar_t	*cp = (struct cx_cpar_t *) &upar.upu;
	int	res;

	/*
	 * Checking options
	 */
	if (opt != NULL
	    && (opt->optc1.t_optnr != T_OPTC1 || opt->optc1.t_udatal < 0)) {
		_t_error = T_WPARAMETER;
		return (T_ERROR);
	}

	/*
	 * T-CONNECTION.confirmation from CMX automaton via ICMX(S3),
	 * supplies return
	 */
	upar.up_subtype = CX_CPAR;
	upar.up_acepid = *tref;
	if (opt != NULL) {
		cp->cp_purdat = opt->optc1.t_udatap;
		cp->cp_lrdat = opt->optc1.t_udatal;
	} else
		cp->cp_lrdat = 0;

	res = t_ioctl(cmx_fd, ST_SCMD(STCONCF, CX_CSIZE), &upar);

	if (opt != NULL && res == T_OK) {
		opt->optc1.t_udatal = cp->cp_lrdat;
		opt->optc1.t_xdata = upar.up_xdt == CX_XDT ? T_YES : T_NO;
		opt->optc1.t_timeout = T_NO;
	}
	return (res);
}

/*------------------------------------------------------------------------*
 * T-REDIRECT
 *------------------------------------------------------------------------*/

/*
 * t_redrq - T-REDIRECT.request
 */
t_redrq(tref, pid, opt)
int	*tref, *pid;
t_opt12	*opt;
{
	register struct cx_cpar_t	*cp = (struct cx_cpar_t *) &upar.upu;

	/*
	 * Checking parameters, options, and state:
	 * Reject if current TIDU is still not empty, or user process
	 */
	if (*pid == t_mypid || *pid <= SYSPID_MAX || *pid > PID_MAX) {
		_t_error = T_WPARAMETER;
		return (T_ERROR);
	}
	if (opt != NULL &&
	     ((opt->optc1.t_optnr != T_OPTC1 && opt->optc1.t_optnr != T_OPTC2)
		|| opt->optc1.t_udatal < 0
		|| opt->optc1.t_udatal > T_RED_SIZE
		|| (opt->optc1.t_optnr == T_OPTC1
			&& opt->optc1.t_timeout < T_NOLIMIT))) {
		_t_error = T_WPARAMETER;
		return (T_ERROR);
	}
	if (t_du.state != NOBUF && *tref == t_du.tref || 
	    t_xdu.state != NOBUF && *tref == t_xdu.tref) {
		_t_error = T_WSEQUENCE;
		return (T_ERROR);
	}

	/*
	 * T-REDIRECTION.request to CMX automaton via ICMX(S3)
	 */
	upar.up_subtype = CX_CPAR;
	upar.up_acepid = *tref;
	upar.up_epid = *pid;
	if (opt != NULL) {
		cp->cp_pudat = opt->optc1.t_udatap;
		cp->cp_ldat = opt->optc1.t_udatal;
		upar.up_maxsec =
			opt->optc1.t_optnr == T_OPTC1 ? opt->optc1.t_timeout : 0;
	} else
		cp->cp_ldat = upar.up_maxsec = 0;

	return (t_ioctl(cmx_fd, ST_SCMD(STREDRQ, CX_CSIZE), &upar));
}

/*
 * t_redin - T-REDIRECT.indication
 */
t_redin(tref, pid, opt)
int	*tref, *pid;
t_opt23	*opt;
{
	register struct cx_cpar_t	*cp = (struct cx_cpar_t *) &upar.upu;
	int	res;

	/*
	 * Checking parameters
	 */
	if (opt != NULL &&
	    ((opt->optc3.t_optnr != T_OPTC2 && opt->optc3.t_optnr != T_OPTC3)
		|| opt->optc3.t_udatal < 0)) {
		_t_error = T_WPARAMETER;
		return (T_ERROR);
	}

	/*
	 * T-REDIRECT.indication from CMX automaton via ICMX(S3),
	 * to supply return parameters
	 */
	upar.up_subtype = CX_CPAR;
	upar.up_acepid = *tref;
	if (opt != NULL) {
		upar.up_ucepid =
			opt->optc3.t_optnr == T_OPTC3 ? opt->optc3.t_ucepid : 0;
		cp->cp_purdat = opt->optc2.t_udatap;
		cp->cp_lrdat = opt->optc2.t_udatal;
	} else
		upar.up_ucepid = cp->cp_lrdat = 0;

	res = t_ioctl(cmx_fd, ST_SCMD(STREDIN, CX_CSIZE), &upar);

	if (res == T_OK) {
		*pid = upar.up_epid;
		if (opt != NULL) {
			opt->optc2.t_udatal = cp->cp_lrdat;
			if (opt->optc3.t_optnr == T_OPTC3) {
				opt->optc3.t_timeout = T_NO;
				opt->optc3.t_xdata = T_NO;
			}
		}
	}
	return (res);
}

/*------------------------------------------------------------------------*
 * T-DISCONNECTION
 *------------------------------------------------------------------------*/

/*
 * t_disrq - T-DISCONNECT.request
 */
t_disrq(tref, opt)
int	*tref;
t_opt2	*opt;
{
	register struct cx_cpar_t	*cp = (struct cx_cpar_t *) &upar.upu;
	int	res;

	/*
	 * Release TIDU and ETSDU buffers, so that the user can use
	 * t_disrq() to specify that data exchange no longer takes place
	 * for this tref.
	 */
	if (t_du.state != NOBUF && *tref == t_du.tref) {
		if (t_du.state == FULL)
			free(t_du.sptr);
		t_du.state = NOBUF;
	}
	if (t_xdu.state != NOBUF && *tref == t_xdu.tref) {
		if (t_xdu.state == FULL)
			free(t_xdu.sptr);
		t_xdu.state = NOBUF;
	}

	/*
	 * Checking options
	 */
	if (opt != NULL &&
	    (opt->optc2.t_optnr != T_OPTC2
		|| opt->optc2.t_udatal < 0
		|| opt->optc2.t_udatal > T_DIS_SIZE_MAX)) {
		_t_error = T_WPARAMETER;
		return (T_ERROR);
	}

	/*
	 * T-DISCONNECT.request to CMX automaton via ICMX(S3)
	 */
	upar.up_subtype = CX_CPAR;
	upar.up_acepid = *tref;
	if (opt != NULL) {
		cp->cp_pudat = opt->optc2.t_udatap;
		cp->cp_ldat = opt->optc2.t_udatal;
	} else
		cp->cp_ldat = 0;

	res = t_ioctl(cmx_fd, ST_SCMD(STDISRQ, CX_CSIZE), &upar);
	return (res);
}

/*
 * t_disin - T-DISCONNECT.indication
 */
t_disin(tref, reason, opt)
int	*tref, *reason;
t_opt2	*opt;
{
	register struct cx_cpar_t	*cp = (struct cx_cpar_t *) &upar.upu;
	int	res;

	/*
	 * Checking options
	 */
	if (opt != NULL
	     && (opt->optc2.t_optnr != T_OPTC2 || opt->optc2.t_udatal < 0)) {
		_t_error = T_WPARAMETER;
		return (T_ERROR);
	}

	/*
	 * T-DISCONNECT.indication from CMX automaton via ICMX(S3)
	 * to supply return parameter
	 */
	upar.up_subtype = CX_CPAR;
	upar.up_acepid = *tref;
	if (opt != NULL) {
		cp->cp_purdat = opt->optc2.t_udatap;
		cp->cp_lrdat = opt->optc2.t_udatal;
	} else
		cp->cp_lrdat = 0;

	res = t_ioctl(cmx_fd, ST_SCMD(STDISIN, CX_CSIZE), &upar);

	if (res == T_OK) {
		*reason = cp->cp_reason;
		if (opt != NULL)
			opt->optc2.t_udatal = cp->cp_lrdat;
	}
	return (res);
}

/*------------------------------------------------------------------------*
 * T-DATA
 *------------------------------------------------------------------------*/

/*
 * t_datarq - T-DATA.request
 */
t_datarq(tref, datap, datal, chain)
int	*tref, *datal, *chain;
char	*datap;
{
	register struct cx_dpar_t	*dp = (struct cx_dpar_t *) &upar.upu;

	/*
	 * Checking parameters
	 */
	if ((*chain != T_MORE && *chain != T_END)
		|| *datal < 1 || datap == NULL) {
		_t_error = T_WPARAMETER;
		return (T_ERROR);
	}

	/*
	 * T-DATA.request to CMX automaton via ICMX(S3)
	 */
	upar.up_subtype = CX_DPAR;
	upar.up_acepid = *tref;
	upar.up_more = *chain == T_MORE ? CX_MORE : 0;
	upar.up_n = 1;
	upar.up_dbl = 0;
	dp->dbp[0].dp_pu = datap;
	dp->dbp[0].dp_l = *datal;

	return(t_ioctl(cmx_fd, ST_SCMD(STDATRQ, CX_USIZE + CX_DBSIZE), &upar));
}

/*
 * t_vdatarq - T-DATA.request
 */
t_vdatarq(tref, vdata, vcnt, chain)
int	*tref, *vcnt, *chain;
struct t_data	*vdata;
{
	register struct cx_dpar_t	*dp = (struct cx_dpar_t *) &upar.upu;
	register int	n = *vcnt;
	int	res;

	/*
	 * Checking parameters and state
	*/
	if ((*chain != T_MORE && *chain != T_END)
		|| n < 1 || n > T_VCNT || vdata == NULL) {
		_t_error = T_WPARAMETER;
		return (T_ERROR);
	}
	for (res = 0; res < n; res++) {
		if (vdata[res].t_datap == NULL || vdata[res].t_datal < 1) {
			_t_error = T_WPARAMETER;
			return (T_ERROR);
		} else {
			dp->dbp[res].dp_pu = vdata[res].t_datap;
			dp->dbp[res].dp_l = vdata[res].t_datal;
		}
	}

	/*
	 * T-DATA.request to CMX automaton via ICMX(S3)
	 */
	upar.up_subtype = CX_DPAR;
	upar.up_acepid = *tref;
	upar.up_more = *chain == T_MORE ? CX_MORE : 0;
	upar.up_n = n;
	upar.up_dbl = 0;

	return (t_ioctl(cmx_fd,
		ST_SCMD(STDATRQ, (CX_USIZE + n * CX_DBSIZE)), &upar));
}

/*
 * t_datain - T-DATA.indication
 */
t_datain(tref, datap, datal, chain)
int	*tref, *datal, *chain;
char	*datap;
{
	register struct cx_dpar_t	*dp = (struct cx_dpar_t *) &upar.upu;
	int	res;

	/*
	 * Checking parameters
	 */
	if (*datal < 1 || datap == NULL) {
		_t_error = T_WPARAMETER;
		return (T_ERROR);
	}

	/*
	 * Transfer T-DATA.indication from CMX automaton.
	 * CMX automaton entries are buffered according to the TIDU length.
	 * Calls are answered from a buffered TIDU, until it is empty.
	 */
	if (t_du.state == EVENT) {
		/*
		 * First t_datain() since T_DATAIN.
		 * The TIDU may be either transferred directly to the caller,
		 * otherwise, the beginning of the TIDU is transferred and the rest buffered.
		 */
		upar.up_subtype = CX_DPAR;
		upar.up_acepid = *tref;
		upar.up_dbl = 0;
		if (*datal >= t_du.cnt) {
			upar.up_n = 1;
			dp->dbp[0].dp_pu = datap;
			dp->dbp[0].dp_l = *datal;

			res = t_ioctl(cmx_fd,
				ST_SCMD(STDATIN, (CX_USIZE + CX_DBSIZE)), &upar);

			if (res == T_OK) {
				t_du.state = NOBUF;
				*datal = t_du.cnt;
				*chain = t_du.chain;
			}
			return (res);
		} else {
			t_du.sptr = (char *) malloc((unsigned) t_du.cnt);
			if (t_du.sptr == NULL) {
				_t_error = ENOMEM;
				return (T_ERROR);
			}
			upar.up_n = 1;
			dp->dbp[0].dp_pu = t_du.sptr;
			dp->dbp[0].dp_l = t_du.cnt;

			res = t_ioctl(cmx_fd,
				ST_SCMD(STDATIN, CX_USIZE + CX_DBSIZE), &upar);

			if (res == T_ERROR) {
				free(t_du.sptr);
				return (T_ERROR);
			}
			t_du.state = FULL;
			memcpy(datap, t_du.sptr, *datal);
			t_du.aptr = t_du.sptr + *datal;
			t_du.cnt -= *datal;
			*chain = t_du.chain;
			return (t_du.cnt);
		}
	} else {
		/*
		 * Not the first t_datain() since the last T_DATAIN.
		 * TIDU is already in the buffer. The next part (possibly the
		 * rest) is to be transferred to the caller.
		 */
		if (t_du.state != FULL || *tref != t_du.tref) {
			_t_error = T_WSEQUENCE;
			return (T_ERROR);
		}
		*chain = t_du.chain;
		if (*datal >= t_du.cnt) {
			memcpy(datap, t_du.aptr, t_du.cnt);
			*datal = t_du.cnt;
			t_du.state = NOBUF;
			free(t_du.sptr);
			return (T_OK);
		} else {
			memcpy(datap, t_du.aptr, *datal);
			t_du.cnt -= *datal;
			t_du.aptr += *datal;
			return (t_du.cnt);
		}
	}
}

/*
 * t_vdatain - T-DATA.indication
 */
t_vdatain(tref, vdata, vcnt, chain)
int	*tref, *vcnt, *chain;
struct t_data	*vdata;
{
	register struct cx_dpar_t	*dp = (struct cx_dpar_t *) &upar.upu;
	register int	n = *vcnt;
	int	res;
	int	size = 0;

	/*
	 * Checking parameters and state
	 */
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
	 * Transfer T-DATA.indication from CMX automaton.
	 * CMX automaton entries are buffered according to TIDU length.
	 * Calls are answered from a buffered TIDU until this is empty.
	 */
	if (t_du.state == EVENT) {
		/*
		 * First t_vdatain() since T_DATAIN.
		 * The TIDU may either be transferred directly to the caller,
		 * otherwise, the beginning is transferred and the rest bufferred.
		 */
		upar.up_subtype = CX_DPAR;
		upar.up_acepid = *tref;
		upar.up_dbl = 0;
		if (size >= t_du.cnt) {
			upar.up_n = n;
			for (res = 0; res < n; res++) {
				dp->dbp[res].dp_pu = vdata[res].t_datap;
				dp->dbp[res].dp_l = vdata[res].t_datal;
			}

			res = t_ioctl(cmx_fd,
				ST_SCMD(STDATIN, (CX_USIZE + n * CX_DBSIZE)), &upar);

			if (res != T_OK)
				return (T_ERROR);

			t_du.state = NOBUF;
			*chain = t_du.chain;
			for (res = 0; res < n; res++) {
				if (vdata[res].t_datal > t_du.cnt)
					vdata[res].t_datal = t_du.cnt;
				if (t_du.cnt)
					t_du.cnt -= vdata[res].t_datal;
			}
			return (T_OK);
		} else {
			t_du.sptr = (char *) malloc((unsigned) t_du.cnt);
			if (t_du.sptr == NULL) {
				_t_error = ENOMEM;
				return (T_ERROR);
			}
			upar.up_n = 1;
			dp->dbp[0].dp_pu = t_du.sptr;
			dp->dbp[0].dp_l = t_du.cnt;

			res = t_ioctl(cmx_fd,
				ST_SCMD(STDATIN, CX_USIZE + CX_DBSIZE), &upar);

			if (res != T_OK) {
				free(t_du.sptr);
				return (T_ERROR);
			}

			/*
			 * "Scatter" beginning of the TIDU in the caller's buffer.
			 * All available buffer areas are full!
			 */
			t_du.state = FULL;
			t_du.aptr = t_du.sptr;
			for (res = 0; res < n; res++) {
				memcpy(vdata[res].t_datap, t_du.aptr,
					vdata[res].t_datal);
				t_du.aptr += vdata[res].t_datal;
				t_du.cnt -= vdata[res].t_datal;
			}
			*chain = t_du.chain;
			return (t_du.cnt);
		}
	} else {
		/*
		 * Not first t_vdatain() since last T_DATAIN.
		 * TIDU may be available
		 */
		if (t_du.state != FULL || *tref != t_du.tref) {
			_t_error = T_WSEQUENCE;
			return (T_ERROR);
		}
		*chain = t_du.chain;
		if (size >= t_du.cnt) {
			/*
			 * "Scatter" rest of TIDU in caller's buffer.
			 * It is not necessary to fill any of the available
			 * puffer areas! Release TIDU buffer.
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
			return (T_OK);
		} else {
			/*
			 * Transfer (next) part of the TIDU to the caller.
			 * All available puffer areas are full!
			 */
			for (res = 0; res < n; res++) {
				memcpy(vdata[res].t_datap, t_du.aptr,
					vdata[res].t_datal);
				t_du.aptr += vdata[res].t_datal;
				t_du.cnt -= vdata[res].t_datal;
			}
			return (t_du.cnt);
		}
	}
}

/*
 * t_datago - T-FLOWCONTROL.release
 */
t_datago(tref)
int	*tref;
{
	/*
	 * Forward T-FLOWCONTROL.release to CMX automaton
	 */
	upar.up_subtype = CX_UPAR;
	upar.up_acepid = *tref;

	return (t_ioctl(cmx_fd, ST_SCMD(STDATGO, CX_USIZE), &upar));
}

/*
 * t_datastop - T-FLOWCONTROL.request
 */
t_datastop(tref)
int	*tref;
{
	/*
	 * Forward T-FLOWCONTROL.request to CMX automaton:
	 * Reject, if sno TIDU is not yet empty
	 */
	if ((t_du.state != NOBUF && *tref == t_du.tref) ||
	    (t_xdu.state != NOBUF && *tref == t_xdu.tref)) {
		_t_error = T_WSEQUENCE;
		return (T_ERROR);
	}
	upar.up_subtype = CX_UPAR;
	upar.up_acepid = *tref;

	return (t_ioctl(cmx_fd, ST_SCMD(STDATSTOP, CX_USIZE), &upar));
}

/*------------------------------------------------------------------------*
 * T-EXPEDITED-DATA
 *------------------------------------------------------------------------*/

/*
 * t_xdatrq - T-EXPEDITED-DATA.request
 */
t_xdatrq(tref, datap, datal)
int	*tref;
char	*datap;
int	*datal;
{
	register struct cx_dpar_t	*dp = (struct cx_dpar_t *) &upar.upu;

	/*
	 * Checking parameters
	 */
	if (*datal < 1 || *datal > T_EXP_SIZE || datap == NULL) {
		_t_error = T_WPARAMETER;
		return (T_ERROR);
	}

	/*
	 * T-EXPEDITED-DATA.request to CMX automaton via ICMX(S3)
	 */
	upar.up_subtype = CX_DPAR;
	upar.up_acepid = *tref;
	upar.up_xdt = CX_XDT;
	upar.up_n = 1;
	upar.up_dbl = 0;
	dp->dbp[0].dp_pu = datap;
	dp->dbp[0].dp_l = *datal;

	return (t_ioctl(cmx_fd, ST_SCMD(STXDTRQ, CX_USIZE + CX_DBSIZE), &upar));
}

/*
 * t_xdatin - T-EXPEDITED-DATA.indication
 */
t_xdatin(tref, datap, datal)
int	*tref;
char	*datap;
int	*datal;
{
	register struct cx_dpar_t	*dp = (struct cx_dpar_t *) &upar.upu;
	int	res;

	/*
	 * Checking parameters
	 */
	if (*datal < 1 || datap == NULL) {
		_t_error = T_WPARAMETER;
		return (T_ERROR);
	}

	/*
	 * T-EXPEDITED-DATA.indication from CMX automaton via ICMX(S3)
	 * as for T-DATA, buffering takes place according to ETSDU length.
	 */
	if (t_xdu.state == EVENT) {
		/*
		 * First t_xdatin() since T_XDATIN.
		 * ETSDU may be transferred directly to the caller
		 */
		upar.up_subtype = CX_DPAR;
		upar.up_acepid = *tref;
		upar.up_xdt = CX_XDT;
		upar.up_n = 1;
		upar.up_dbl = 0;
		if (*datal >= t_xdu.cnt) {
			dp->dbp[0].dp_pu = datap;
			dp->dbp[0].dp_l = *datal;

			res = t_ioctl(cmx_fd,
				ST_SCMD(STXDTIN, (CX_USIZE + CX_DBSIZE)), &upar);

			if (res == T_OK) {
				t_xdu.state = NOBUF;
				*datal = t_xdu.cnt;
			}
			return (res);
		} else {
			t_xdu.sptr = xdata;
			dp->dbp[0].dp_pu = t_xdu.sptr;
			dp->dbp[0].dp_l = t_xdu.cnt;

			res = t_ioctl(cmx_fd,
				ST_SCMD(STXDTIN, (CX_USIZE + CX_DBSIZE)), &upar);

			if (res == T_ERROR)
				return (T_ERROR);
			t_xdu.state = FULL;
			memcpy(datap, t_xdu.sptr, *datal);
			t_xdu.aptr = t_xdu.sptr + *datal;
			t_xdu.cnt -= *datal;
			return (t_xdu.cnt);
		}
	} else {
		/*
		 * Not first t_xdatin() since last T_XDATIN.
		 * ETSDU already available; transfer next part (rest) to
		 * caller
		 */
		if (t_xdu.state != FULL || *tref != t_xdu.tref) {
			_t_error = T_WSEQUENCE;
			return (T_ERROR);
		}
		if (*datal >= t_xdu.cnt) {
			memcpy(datap, t_xdu.aptr, t_xdu.cnt);
			*datal = t_xdu.cnt;
			t_xdu.state = NOBUF;
			return (T_OK);
		} else {
			memcpy(datap, t_xdu.aptr, *datal);
			t_xdu.cnt -= *datal;
			t_xdu.aptr += *datal;
			return (t_xdu.cnt);
		}
	}
}

/*
 * t_xdatstop - T-EXPEDITED-FLOWCONTROL.request
 * Lock priority data display
 */
t_xdatstop(tref)
int	*tref;
{
	/*
	 * Checking parameters and state:
	 * Reject, if sno TIDU is not yet empty
	 */
	if (t_xdu.state != NOBUF && *tref == t_xdu.tref) {
		_t_error = T_WSEQUENCE;
		return (T_ERROR);
	}

	/*
	 * T-FLOWCONTROL.request to CMX automaton via ICMX(S3)
	 */
	upar.up_subtype = CX_UPAR;
	upar.up_acepid = *tref;

	return (t_ioctl(cmx_fd, ST_SCMD(STXDTSTOP, CX_USIZE), &upar));
}

/*
 * t_xdatgo - T-EXPEDITED-FLOWCONTROL.release
 * Release priority data display
 */
t_xdatgo(tref)
int	*tref;
{
	/*
	 * T-FLOWCONTROL.release to CMX automaton via ICMX(S3)
	 */
	upar.up_subtype = CX_UPAR;
	upar.up_acepid = *tref;

	return (t_ioctl(cmx_fd, ST_SCMD(STXDTGO, CX_USIZE), &upar));
}

/*------------------------------------------------------------------------*
 * T-EVENT, T-INFO, T-ERROR
 *------------------------------------------------------------------------*/

/*
 * t_event - T-EVENT
 * Query on transport events
 */
t_event(tref, cmode, opt)
int	*tref, cmode;
t_opte	*opt;
{
	int	cmd, size;

	/*
	 * Checking parameters and state:
	 * Reject if current TIDU is not yet empty
	 */
	if (t_du.state == FULL || t_xdu.state == FULL) {
		_t_error = T_WSEQUENCE;
		return (T_ERROR);
	}
	if (opt != NULL &&
	    ((opt->opte1.t_optnr != T_OPTE1 && opt->opte2.t_optnr != T_OPTE2)
            || opt->opte1.t_timeout < T_NOLIMIT)) {
		_t_error = T_WPARAMETER;
		return (T_ERROR);
	}
	if (cmode == T_WAIT) {
		if (opt != NULL) {
			if (opt->opte1.t_timeout == T_NOLIMIT)
				cmd = STWEVENT;
			else if (opt->opte1.t_timeout == T_NO)
				cmd = STEVENT;
			else {		/* positiver Wert! */
				cmd = STVEVENT;
				upar.up_maxsec = opt->opte1.t_timeout;
			}
		} else          /* per default continuous wait */
			cmd = STWEVENT;
	} else		/* T_CHECK */
		cmd = STEVENT;
	size = cmd == STVEVENT ? CX_USIZE : 0;
	cmd = ST_SCMD(cmd, size);

	/*
	 * Event query to CMX automaton via ICMX(S3). This is ALWAYS
	 * checking. Experience to date, however, makes it necessary
	 * to check EINTR and then to repeat.
	 *
	 * The following is the problem:
	 * T_DATAGO and T_XDATGO are consumed by "retrieving" with ioctl(ST*EVENT)
	 * and are reset to the CMX automatons. If the t_event handling by
	 * the library is aborted, BEFORE this event is delivered as the result
	 * of the application t_event call, this event is lost!
	 * For the next ioctl(ST*EVENT), it is not redisplayed!
	 *
	 * To improve the robustness, event transfer of a variable takes
	 * place (not in the return value), but not for T_NOEVENT.
	 */
	do {
		upar.up_re = T_NOEVENT;
		ioctl(cmx_fd, cmd, &upar);
		t_trc(T_FIOCTL, 0, cmx_fd, cmd, &upar, upar.up_re, errno);
	} while (upar.up_re == (u_short) T_ERROR && errno == EINTR && cmode == T_CHECK);

	if (upar.up_re == (u_short) T_ERROR) {
		_t_error = errno;
		return (T_ERROR);
	}

	if (upar.up_re == T_NOEVENT)
		return (T_NOEVENT);

	/*
	 * A productive event is available.
	 * For T_DATAIN/T_XDATIN transfer parameters to local variables
	 * since t_datain()/t_xdatin() now only picks up the clean TIDU/ETSDU.
	 */
	if (upar.up_re == T_DATAIN) {
		t_du.attid = upar.up_aattid;
		t_du.tref = upar.up_acepid;
		t_du.cnt = upar.up_l;
		t_du.chain = upar.up_more == CX_MORE ? T_MORE : T_END;
		t_du.state = EVENT;
	} else if (upar.up_re == T_XDATIN) {
		t_xdu.attid = upar.up_aattid;
		t_xdu.tref = upar.up_acepid;
		t_xdu.cnt = upar.up_l;
		t_xdu.state = EVENT;
	}
	if (opt != NULL) {
		opt->opte1.t_attid = upar.up_aattid;
		opt->opte1.t_uattid = upar.up_uattid;
		opt->opte1.t_ucepid = upar.up_ucepid;
		opt->opte1.t_evdat = upar.up_l;
	}
	*tref = upar.up_acepid;
	return (upar.up_re);
}

/*
 * t_info - T-INFO
 * Query on transport system parameters
 */
t_info(tref, opt)
int	*tref;
t_opti	*opt;
{
	register struct cx_ipar_t	*ip = (struct cx_ipar_t *) &upar.upu;
	int	res;

	/*
	 * Checking parameters
	 */
	if (opt == NULL || ( opt->opti1.t_optnr != T_OPTI1
                &&opt->opti3.t_optnr != T_OPTI3 )) {
		_t_error = T_WPARAMETER;
		return (T_ERROR);
	}

	/*
	 * Forward T-INFO to CMX automaton
	 */
	upar.up_subtype = CX_IPAR;
	upar.up_acepid = *tref;

	res = t_ioctl(cmx_fd, ST_SCMD(STINFO, CX_ISIZE), &upar);

	if (res == T_OK) {
		opt->opti1.t_maxl = (int) ip->ip_idumaxl;
		if (opt->opti3.t_optnr == T_OPTI3) {
			opt->opti3.t_fdsetsize = FD_SETSIZE;
			opt->opti3.t_openmax = open_max;
			opt->opti3.t_tsdu = (int) ip->ip_sdumaxl;
			opt->opti3.t_etsdu = (int) ip->ip_xsdumaxl;
			opt->opti3.t_condata = (int) ip->ip_conmaxl;
			opt->opti3.t_concfdata = (int) ip->ip_confmaxl;
			opt->opti3.t_disdata = (int) ip->ip_dismaxl;
		}
	}
	return (res);
}

/*
 * t_error - T-ERROR
 */
t_cmxerror(void)
{
	return (_t_error);
}

/*------------------------------------------------------------------------*
 * HELP FUNCTIONS
 *------------------------------------------------------------------------*/

/*
 *  Function t_ioctl - request to CMX automaton
 */
int t_ioctl(fd, cmd, addr)
int	fd, cmd;
struct cx_upar_t	*addr;
{
	register int	res;

	do {
		res = ioctl(fd, cmd, addr);
		if (cmd != CX_TIMESTAMP)
			t_trc(T_FIOCTL, 0, fd, cmd, addr, res, errno);
	} while (res == T_ERROR && errno == EINTR);

	if (res == T_ERROR) {
		_t_error = errno;
		return (T_ERROR);
	} else
		return (res);
}

/*
 * t_netopen - opens a network special file of the CMX automatons
 * These have names in the form "/dev/cmx/net[0-1][0-9][0-9][0-9]".
 * The 4 character suffix is also the minor device number
 */
static int
t_netopen()
{
	static char	*netfile = NULL;

	register int	fd, cxfd;
	struct cx_prenetopen	arg;
	int	res;

	/*
	 * Version check according to the following protocol:
	 * The CMX library transfer its version to the CMX automatons.
	 * If the CMX automaton has an earlier (smaller) version, it supplies
	 * it so that the CMX library can adapt itself accordingly if
	 * necessary (or reject the cooperation).
	 * If the CMX automaton has a more recent (larger) version, AND if it can
	 * cooperate with this CMX library, it supplies this version.
	 * If it cannot cooperate, it returns an error.
	 * The current library can be adapted to V2.5A.
	 */
	if (netfile == NULL) {
		if (((fd = open(CX_NETCFILE, O_RDWR)) == T_ERROR)
		    && (errno == ENOENT)) {
			netfile = CX_NETFILE;
		} else {
			netfile = CX_NETCFILE;
			close(fd);
		}
	}

	cxfd = open(CX_FILE, O_RDONLY);
	t_trc(T_FOPEN, 0, CX_FILE, O_RDONLY, cxfd, errno);
	if (cxfd == T_ERROR) {
		_t_error = errno;
		return (T_ERROR);
	}
	arg.cxo_version = T_LIBV40;

	res = t_ioctl(cxfd, CX_PRENETOPEN, (struct cx_upar_t *) &arg);

	if (res == T_ERROR)
		goto out;
	if (arg.cxo_version != T_LIBV25 
	 && arg.cxo_version != T_LIBV30
	 && arg.cxo_version != T_LIBV30B 
	 && arg.cxo_version != T_LIBV40)
	{
		_t_error = T_WLIBVERSION;
		goto out;
	}

	/*
	 * Construct the name of the network file to open it.
	 * If it is already occupied (EBUSY), repeat until certain that it is
	 * free (ENOENT).
	 */
	for (;;) {
		sprintf(&netfile[strlen(netfile) - 4],"%04d",arg.cxo_mindev);

		fd = open(netfile, O_RDWR);
		t_trc(T_FOPEN, 0, netfile, O_RDONLY, fd, errno);
		if (fd == T_ERROR) {
			if (errno != EBUSY) {
				_t_error = errno;
				break;
			} else {
				res = t_ioctl(cxfd, CX_PRENETOPEN,
					(struct cx_upar_t *) &arg);
				if (res == T_ERROR)
					break;
				continue;
			}
		} else {
			close(cxfd);
			t_trc(T_FCLOSE, 0, cxfd);
			return (fd);
		}
	}
out:
	close(cxfd);
	t_trc(T_FCLOSE, 0, cxfd);
	return (T_ERROR);
}

#else  /* ifndef PCMX */
/****************************************************************************/
/*                                                                          */
/* Dummy module for PCMX.                                                    */
/* T_ERROR with t_error T_WREQUEST                                          */
/* is returned for all library calls.                                                        */
/*                                                                          */
/****************************************************************************/
#include "tnsx.h"
#include "cmx.h"
#include "cxioctl.h"

extern	int		_t_error;
	Ts_route	envrout; 	/* routing information in CMXINIT */

/*
 * Prototyping
 */
int	t_ioctl(int, int, struct cx_upar_t *);
int	t_100attach(struct t_myname *name, t_opta *opt);
int	t_101detach(struct t_myname *name);
int	t_102conrq(int *tref, union t_address *toaddr, union t_address *fromaddr, t_opt13 *opt);
int	t_103conin(int *tref, union t_address *toaddr, union t_address *fromaddr, t_opt1 *opt);
int	t_104conrs(int *tref, t_opt13 *opt);
int	t_105concf(int *tref, t_opt1 *opt);
int	t_106datarq(int *tref, char *datap, int *datal, int *chain);
int	t_107datain(int *tref, char *datap, int *datal, int *chain);
int	t_108datago(int *tref);
int	t_109datastop(int *tref);
int	t_110redrq(int *tref, int *pid, t_opt12 *opt);
int	t_111redin(int *tref, int *pid, t_opt23 *opt);
int	t_112xdatrq(int *tref, char *datap, int *datal);
int	t_113xdatin(int *tref, char *datap, int *datal);
int	t_114xdatgo(int *tref);
int	t_115xdatstop(int *tref);
int	t_116disrq(int *tref, t_opt2 *opt);
int	t_117disin(int *tref, int *reason, t_opt2 *opt);
int	t_118event(int *tref, int cmode, t_opte *opt);
int	t_119error(void);
int	t_120info(int *tref, t_opti *opt);
int	t_121vdatarq(int *tref, struct t_data *vdata, int *vcnt, int *chain);
int	t_122vdatain(int *tref, struct t_data *vdata, int *vcnt, int *chain);

int t_ioctl(int fd, int cmd, struct cx_upar_t *addr)
{ return(-1); }

int	t_100attach(struct t_myname *name, t_opta *opt)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_101detach(struct t_myname *name)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_102conrq(int *tref, union t_address *toaddr, union t_address *fromaddr, t_opt13 *opt)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_103conin(int *tref, union t_address *toaddr, union t_address *fromaddr, t_opt1 *opt)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_104conrs(int *tref, t_opt13 *opt)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_105concf(int *tref, t_opt1 *opt)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_106datarq(int *tref, char *datap, int *datal, int *chain)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_107datain(int *tref, char *datap, int *datal, int *chain)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_108datago(int *tref)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_109datastop(int *tref)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_110redrq(int *tref, int *pid, t_opt12 *opt)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_111redin(int *tref, int *pid, t_opt23 *opt)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_112xdatrq(int *tref, char *datap, int *datal)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_113xdatin(int *tref, char *datap, int *datal)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_114xdatgo(int *tref)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_115xdatstop(int *tref)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_116disrq(int *tref, t_opt2 *opt)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_117disin(int *tref, int *reason, t_opt2 *opt)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_118event(int *tref, int cmode, t_opte *opt)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_119error(void)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_120info(int *tref, t_opti *opt)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_121vdatarq(int *tref, struct t_data *vdata, int *vcnt, int *chain)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_122vdatain(int *tref, struct t_data *vdata, int *vcnt, int *chain)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}
#endif /* ifndef PCMX */

