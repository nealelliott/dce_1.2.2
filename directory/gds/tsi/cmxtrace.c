/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cmxtrace.c,v $
 * Revision 1.1.5.2  1996/03/11  13:24:28  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:12:51  marty]
 *
 * Revision 1.1.5.1  1995/12/08  17:48:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:25:35  root]
 * 
 * Revision 1.1.2.5  1994/07/18  13:40:47  marrek
 * 	Changes for threadsafe libxti.
 * 	[1994/07/18  12:11:52  marrek]
 * 
 * Revision 1.1.2.4  1994/07/06  15:08:38  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:22:32  marrek]
 * 
 * Revision 1.1.2.3  1994/06/21  14:48:22  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:46:37  marrek]
 * 
 * Revision 1.1.2.2  1994/06/10  21:17:37  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:54:48  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/10  16:15:33  marrek
 * 	Bug fixes in April 1994 submission.
 * 	[1994/05/04  09:57:31  marrek]
 * 
 * 	Created for April Submission.
 * 	[1994/04/26  12:51:36  marrek]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: cmxtrace.c,v $ $Revision: 1.1.5.2 $ $Date: 1996/03/11 13:24:28 $";
#endif

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

/*****************************************************************************/
/*                                                                           */
/*        ICMX(L) TRACE FUNCTIONS                                            */
/*        =======================					     */
/*                                                                           */
/* This includes the special implementation for previous TUI-TRACEs          */
/* as of version CMX V3.0                                                    */
/*                                                                           */
/* Modifications:                                                            */
/*   kam     V31                                        18.03.92             */
/*                                                                           */
/*****************************************************************************/

/*
#ifdef THREADSAFE
# include <pthread.h>
#endif
*/

#include <gds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/uio.h>
#include <sys/utsname.h>
#include "cxioctl.h"
#include "cxmon.h"
#include "cmx.h"
#if defined(_AIX)
#include <sys/select.h>
#endif
#include <sys/signal.h>
#include "cmxextern.h"
#undef	T_MORE
#undef	T_CHECK
#include "xti.h"
#undef	T_MORE
#undef	T_CHECK
#define  CMXTRACE_XTI
#include "cmxtrace.h"
#include "sna_def.h"
#include "cmxxtr.h"

/*
static char sccsid[] = "@(#)cmxtrace.c	311.40 93/08/24 CMX (NC1)";
*/

/*****************************************************************************/
/*      D E C L A R A T I O N  O F  I N T E R N A L  D A T A                 */
/*****************************************************************************/

/*
 * These definitions are for the ICMX(L) TRACE
 */
#ifdef NOFIELDS
static int      t_trcdeb = 0;           /* Connection-specific debug for
					   attach, event, detach */
static int      t_trcsel = 0;           /* TRAC selector, no default */
#else
struct t_trcflag t_trcflg;              /* Global for t_opttrace */
#endif

static FILE     *t_trcfd = NULL;        /* TRACE stream */
       char     t_trcdir[T_TRCDIR_SIZE];/* Directory for TRACE file*/
static char     t_trclp[80];            /* Primary TRACE file */
static char     t_trcmp[80];            /* Alternative TRACE file */
static char     *t_trccurr = NULL;      /* Current TRACE file */
static long     t_trcbeg = 0;           /* TRACE start time */
static int      t_trccnt = 0;           /* Current size of the TRACE file in bytes */
static int	dce_serviceability = TRUE ;

/*
 * The following descriptors are global due to access in t_opttrace
 */
char    *t_trcbp = NULL;        /* TRACE buffer */
int     t_trcwrap = T_TRWRAP;   /* Max. size of TRACE file in bytes */
int     t_trcfac = T_TRFAC;     /* Buffer factor */
int     t_trclen = T_TRLEN;     /* Length of the logged data */
int     t_trcpid = -1;          /* Current process number */

static struct t_trcentry	t_trcel = { 0 };
static struct cx_timestamp	cmxltimestamp;
static struct udat              t_udat;         /* For extended timestamp    */
static void t_trchead (void);
static int  t_trcon (int dflt);
static int  t_trcopen (int *fd, char *template);
static void t_trcsw (void);
static int  zmin (int v1, int v2);

static struct {
	short	id;
	short	info;
	short	number;
} t_trcsig = { 0 };

/*
 * FWRITE* - Macros to output a TRACE entry
 * the length (int cnt) byte (char *buf)
 * with and without (previous) output of incoming signals and with and without
 * linking wrap-around of the TRACE file.
 */

/* The following macros are all redefined because of redirection of the */
/* CMX-transport interface tracing to DCE-serviceability */

/*
#define FWRITE_CMX(buf, cnt) \
	{ \
		if (t_trcflg.async) { \
			fwrite(&t_trcsig, T_LHEADER, 1, t_trcfd); \
			t_trcflg.async = 0; \
			t_trccnt += T_LHEADER; \
		} \
		fwrite((buf), (cnt), 1, t_trcfd); \
		t_trccnt += (cnt); \
		if (t_trccnt >= t_trcwrap) \
			t_trcsw(); \
	}
*/

#define FWRITE_CMX(buf, cnt) \
	{ \
		if (t_trcflg.async) { \
		        DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%b%b"), \
			GDS_S_CMX, svc_c_debug1, GDS_S_CMX_LOG, \
			T_LHEADER, &t_trcsig, (cnt), (buf))) ; \
			t_trcflg.async = 0; \
		} else {\
		DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%b%b"), GDS_S_CMX, \
		svc_c_debug1, GDS_S_CMX_LOG, (cnt), (buf), 0, NULL)) ; \
		} \
	}

#define FWRITE_WRAP_SIG(buf, cnt)	FWRITE_CMX(buf, cnt)

/*
#define FWRITE_NOWRAP_SIG(buf, cnt) \
	{ \
		if (t_trcflg.async) { \
			fwrite(&t_trcsig, T_LHEADER, 1, t_trcfd); \
			t_trcflg.async = 0; \
			t_trccnt += T_LHEADER; \
		} \
		fwrite((buf), (cnt), 1, t_trcfd); \
		t_trccnt += (cnt); \
	}
*/

#define FWRITE_NOWRAP_SIG(buf, cnt) \
	{ \
		if (t_trcflg.async) { \
		        DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%b%b"), \
			GDS_S_CMX, svc_c_debug1, GDS_S_CMX_LOG, \
			T_LHEADER, &t_trcsig, (cnt), (buf))) ; \
			t_trcflg.async = 0; \
		} else {\
		DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%b%b"), GDS_S_CMX, \
		svc_c_debug1, GDS_S_CMX_LOG, (cnt), (buf), 0, NULL)) ; \
		} \
	}

/*
#define FWRITE_WRAP_NOSIG(buf, cnt) \
	{ \
		fwrite((buf), (cnt), 1, t_trcfd); \
		t_trccnt += (cnt); \
		if (t_trccnt >= t_trcwrap) \
			t_trcsw(); \
	}
*/
#define FWRITE_WRAP_NOSIG(buf, cnt) \
	{ \
		DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%b%b"), GDS_S_CMX, \
		svc_c_debug1, GDS_S_CMX_LOG, (cnt), (buf), 0, NULL)) ; \
	}

/*
#define FWRITE_NOWRAP_NOSIG(buf, cnt) \
	{ \
		fwrite((buf), (cnt), 1, t_trcfd); \
		t_trccnt += (cnt); \
	}
*/
#define FWRITE_NOWRAP_NOSIG(buf, cnt) \
	{ \
		DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%b%b"), GDS_S_CMX, \
		svc_c_debug1, GDS_S_CMX_LOG, (cnt), (buf), 0, NULL)) ; \
	}
/*
 * Argumente typgerecht uebernehmen
 */
#define GET_ARG(Typ) \
	(Typ)va_arg(ap, Typ)

/*
 * Macro for transferring a pointer to a buffer and filling the
 * buffer, if the pointer is != ZERO
 */
#define XTI_PTR(Ptr,Buf,Str) \
	if ((trp->Ptr = GET_ARG(Str)) != NULL) \
		trp->Buf = *trp->Ptr

/*
 * Summary of the XTI result values
 */
#define XTI_RET(Str) \
	trp->Str.xret.retval = GET_ARG(int); \
	trp->Str.xret.xti_errno = GET_ARG(int); \
	trp->Str.xret.cmx_errno = GET_ARG(int) \

/*
 * Writing an area, if one is available and if it is between one
 * and BUFSIZ bytes in length.
 */
#define XTI_NBUF(Ptr,Nbuf,Fkt) \
	if (trp->Ptr != NULL && trp->Nbuf.len > 0) \
		Fkt(trp->Nbuf.buf, XMIN(BUFSIZ, trp->Nbuf.len))

/*
 * Length of the trace data
 * T_LHEADER is defined in cmxtrace.h.
 */
#define T_LFIRST 	(T_LHEADER + sizeof (trp->t_ffirst))
#define T_LFIRST2 	(T_LHEADER + sizeof (trp->t_ffirst2))

#define T_LRETERR 	(T_LHEADER + sizeof (trp->t_freterr))
#define T_LRETOK 	(T_LHEADER + sizeof (trp->t_fretok))

#define T_LATTACH 	(T_LHEADER + sizeof (trp->t_fattach))
#define T_LRETATT 	(T_LHEADER + sizeof (trp->t_fretatt))
#define T_LDETACH 	(T_LHEADER + sizeof (trp->t_fdetach))
#define T_LCONRQ 	(T_LHEADER + sizeof (trp->t_fconrq))
#define T_LCONCF 	(T_LHEADER + sizeof (trp->t_fconcf))
#define T_LCONIN 	(T_LHEADER + sizeof (trp->t_fconin))
#define T_LCONRS 	(T_LHEADER + sizeof (trp->t_fconrs))
#define T_LDISRQ 	(T_LHEADER + sizeof (trp->t_fdisrq))
#define T_LDISIN 	(T_LHEADER + sizeof (trp->t_fdisin))
#define T_LREDIN 	(T_LHEADER + sizeof (trp->t_fredin))
#define T_LREDRQ 	(T_LHEADER + sizeof (trp->t_fredrq))
#define T_LDATARQ 	(T_LHEADER + sizeof (trp->t_fdatarq))
#define T_LVDATARQ 	(T_LHEADER + sizeof (trp->t_fvdatarq))
#define T_LDATAIN 	(T_LHEADER + sizeof (trp->t_fdatain))
#define T_LVDATAIN 	(T_LHEADER + sizeof (trp->t_fvdatain))
#define T_LDATARQS 	(T_LHEADER + sizeof (trp->t_fdatarqs))
#define T_LDATAINS 	(T_LHEADER + sizeof (trp->t_fdatains))
#define T_LFLOW 	(T_LHEADER + sizeof (trp->t_fflow))
#define T_LXDATRQ 	(T_LHEADER + sizeof (trp->t_fxdatrq))
#define T_LXDATIN 	(T_LHEADER + sizeof (trp->t_fxdatin))
#define T_LXDATRQS 	(T_LHEADER + sizeof (trp->t_fxdatrqs))
#define T_LXDATINS 	(T_LHEADER + sizeof (trp->t_fxdatins))
#define T_LINFO 	(T_LHEADER + sizeof (trp->t_finfo))
#define T_LINFO3 	(T_LHEADER + sizeof (trp->t_finfo3))
#define T_LEVENT 	(T_LHEADER + sizeof (trp->t_fevent))
#define T_LDEVENT	(T_LHEADER + sizeof (trp->t_devent))
#define T_LERROR	(T_LHEADER)

#define T_LGETADDR 	(T_LHEADER + sizeof (trp->t_fgetaddr))
#define T_LGETNAME 	(T_LHEADER + sizeof (trp->t_fgetname))
#define T_LGETLOC 	(T_LHEADER + sizeof (trp->t_fgetloc))

#define T_LTCPSCK 	(T_LHEADER + sizeof (trp->t_ftcpsck))
#define T_LTCPSOPT 	(T_LHEADER + sizeof (trp->t_ftcpsopt))
#define T_LTCPABC 	(T_LHEADER + sizeof (trp->t_ftcpabc))
#define T_LTCPLIS 	(T_LHEADER + sizeof (trp->t_ftcplis))
#define T_LTCPSEL 	(T_LHEADER + sizeof (trp->t_ftcpsel))
#define T_LOPEN		(T_LHEADER + sizeof (trp->t_fopen))
#define T_LSVC		(T_LHEADER + sizeof (trp->t_fsvc))
#define T_LSETOPT	(T_LHEADER + sizeof (trp->t_fsetopt))
#define T_LCALLBACK	(T_LHEADER + sizeof (trp->t_fcallback))
#define T_LCBRES	(T_LHEADER + sizeof (trp->t_fcbres))
#define T_LSTRING	(T_LHEADER + sizeof (trp->t_fstring))
#define T_LSNAPIPE	(T_LHEADER + sizeof (trp->t_fsnapipe))
#define T_LDLERR	(T_LHEADER + sizeof (trp->t_fdlerr))
#define T_LUNAME	(T_LHEADER + sizeof (trp->t_funame))

#define T_LxACCEPT	(T_LHEADER + sizeof(trp->t_fXaccept))
#define T_LxFIN		(T_LHEADER + sizeof(trp->t_fXfin))
#define T_LxFD		(T_LHEADER + sizeof(trp->t_fXfd))
#define T_LxBIND	(T_LHEADER + sizeof(trp->t_fXbind))
#define T_LxBINDFIN	(T_LHEADER + sizeof(trp->t_fXbindfin))
#define T_LxCONNECT	(T_LHEADER + sizeof(trp->t_fXconnect))
#define T_LxCALL	(T_LHEADER + sizeof(trp->t_fXcall))
#define T_LxCALLFIN	(T_LHEADER + sizeof(trp->t_fXcallfin))
#define T_LxOPEN	(T_LHEADER + sizeof(trp->t_fXopen))
#define T_LxOPENFIN	(T_LHEADER + sizeof(trp->t_fXopenfin))
#define T_LxRCVREQ	(T_LHEADER + sizeof(trp->t_fXrcvreq))
#define T_LxRCVFIN	(T_LHEADER + sizeof(trp->t_fXrcvfin))
#define T_LxSNDREQ	(T_LHEADER + sizeof(trp->t_fXsndreq))
#define T_LxDIS		(T_LHEADER + sizeof(trp->t_fXdis))



/*
 * t_trc --Preparing an entry for ICMX(L) TRACE
 *
 * This is called with a variable number of arguments (depending on the id).
 * The id identifies the actual call, info is transferred transparently,
 * args are optional arguments; their number and meaning depend on the id.
 */
/* VARARGS2 */
/*
void t_trc(id, info, va_alist)
int	id, info;
*/
void t_trc(int id, int info, ...)
{
	extern int t_ioctl(int, int, struct cx_upar_t *);
	register struct t_trcentry	*trp;
	va_list				ap;
	register int			i;
	int	res;
	int	myerrno;
	char	strbuf [80];
	char	*hrfds,*hwfds,*hxfds;
	int	hsec;
	char	*hpptr;
	char 	*strng;
	size_t 	strcnt;

	if (t_trcflg.sel == T_TRDONOT)
		return;
	myerrno = errno;

	/*
	 * Reinitialise after exec() and fork().
	 * If a call occurs after fork () WITHOUT t_attach(),
	 * data is logged in the parent stream. getpid() can thus be avoided
	 * in any call, because t_attach() sets t_pid to getpid().
	 */
	if (t_trcpid == -1 ||                   /* after exec() OR */
	    (t_trcpid != t_pid &&               /* fork() with t_attach() AND */
			t_trcpid != getpid())) {        /* first after fork() */
		if (t_trcon(T_TRNDFLT) == T_ERROR) {
			t_trcoff(T_TRNFLSH);
			errno = myerrno;
			return;
		}
		t_trcpid = getpid();
	}

	/* verification of a valid trace file descriptor is not longer */
	/* necessary because all trace information is passed to DCE- */
	/* serviceability */
	/*
	if (!dce_serviceability && t_trcfd == NULL) {
		errno = myerrno;
		return;
	}
	*/

	va_start(ap, info);
	if (id == T_FSIGTERM || id == T_FSIGIO) {
		if (!t_trcflg.async) {  /* Signal entry does not wait */
			t_trcflg.async = 1;
			t_trcsig.id = id;
			t_trcsig.info = (id == T_FSIGTERM ? t_cmxsig : SIGIO);
			t_trcsig.number = (short) info;
		}
		va_end(ap);
		errno = myerrno;
		return;
	}
	memset(&t_trcel,0,sizeof(struct t_trcentry));
	trp = &t_trcel;
	trp->id = id;
	trp->info = (short) info;

	t_udat.udat_l = sizeof(struct cx_timestamp);
	t_udat.udat_offl = 0;
	t_udat.udat_spec = 0;
	t_udat.udat_p = (char *) &cmxltimestamp;
	errno = 0;
	if (cmx_fd == -1)
		res = -1;
	else
		res = t_ioctl(cmx_fd, CX_TIMESTAMP, (struct cx_upar_t *)&t_udat);
	if (res == -1 || errno != 0) {
		/*
		 * time() must be used again because
		 * 'gettimeofday()' with libcmx.so creates problems.
		 */
		trp->relsec = (short) (time((long*) NULL) - t_trcbeg);
		trp->milsec = (short) 0;
	}
	else {
		trp->relsec = (short) (cmxltimestamp.ts_sec - t_trcbeg);
		trp->milsec = cmxltimestamp.ts_msec;
	}

	switch (id) {

	case T_DATTACH:
	case T_DDETACH:
		/* t_trc(T_DATTACH, connection) */
		/* t_trc(T_DDETACH, connection) */
		if (!t_trcflg.deb)
			break;
		FWRITE_CMX(trp, T_LHEADER);
		break;

	case T_FATTACH: {
		/* t_trc(T_FATTACH, 0, name, opt) */
		t_opta *op;
		struct t_opta1 *opt;
		struct t_opta2 *op2;

		trp->t_fattach.name = (char *) va_arg(ap, char *);
		op = (t_opta *) va_arg(ap, t_opta *);

		opt = (struct t_opta1 *) op;
		op2 = (struct t_opta2 *) op;

		memcpy((char *) &(trp->t_fattach.t_mn), trp->t_fattach.name,
			sizeof (struct t_myname));
		if ((trp->t_fattach.opt = op2) != NULL) {
			trp->t_fattach.t_optnr = opt->t_optnr;
			trp->t_fattach.t_apmode = opt->t_apmode;
			trp->t_fattach.t_conlim = opt->t_conlim;
		}

		FWRITE_CMX(trp, T_LATTACH);
		break;
	}

	case T_FDETACH: {
		/* t_trc(T_FDETACH, 0, name) */

		trp->t_fdetach.name = (char *) va_arg(ap, char *);
		memcpy((char *) &(trp->t_fdetach.t_mn), trp->t_fdetach.name,
			sizeof (struct t_myname));

		FWRITE_CMX(trp, T_LDETACH);
		break;
	}
	case T_FCONRQ: {
		/* t_trc(T_FCONRQ, 0, tref, toaddr, fraddr, opt) */

		t_opt13 *op;
		struct t_optc1 *opt;
		struct t_optc3 *op3;

		trp->t_fconrq.tref = va_arg(ap, int *);
		trp->t_fconrq.toaddr = va_arg(ap, struct t_partaddr *);
		trp->t_fconrq.fromaddr = va_arg(ap, struct t_myname *);
		op = va_arg(ap, t_opt13 *);
		opt = (struct t_optc1 *) op;
		op3 = (struct t_optc3 *) op;
		if ((trp->t_fconrq.opt = op3) != NULL) {
			trp->t_fconrq.t_optnr = opt->t_optnr;
			trp->t_fconrq.t_udatap = opt->t_udatap;
			trp->t_fconrq.t_udatal = opt->t_udatal;
			trp->t_fconrq.t_xdata  = opt->t_xdata;
			trp->t_fconrq.t_timeout  = opt->t_timeout;
			if (opt->t_optnr == T_OPTC3)
				trp->t_fconrq.t_ucepid = op3->t_ucepid;
			memcpy(trp->t_fconrq.udata, opt->t_udatap,
				zmin(opt->t_udatal, T_MSG_SIZE));
		}
		memcpy((char *) &(trp->t_fconrq.t_pa),
			(char *) trp->t_fconrq.toaddr,
			sizeof (struct t_partaddr));
		memcpy((char *) &(trp->t_fconrq.t_mn),
			(char *) trp->t_fconrq.fromaddr,
			sizeof (struct t_myname));

		FWRITE_CMX(trp, T_LCONRQ);
		break;
	}
	case T_FCONCF: {
		/* t_trc(T_FCONCF, 0, tref, opt, udatal) */

		struct t_optc1 *opt;

		trp->t_fconcf.tref = *(va_arg(ap, int *));
		opt = va_arg(ap, struct t_optc1 *);
		trp->t_fconcf.udatal = va_arg(ap, int);
		if (((trp->t_fconcf.opt = opt) != NULL)
			&& (trp->info != T_TRSTART)) {
			trp->t_fconcf.t_optnr = opt->t_optnr;
			trp->t_fconcf.t_udatap = opt->t_udatap;
			trp->t_fconcf.t_udatal = opt->t_udatal;
			trp->t_fconcf.t_xdata = opt->t_xdata;
			trp->t_fconcf.t_timeout = opt->t_timeout;
			memcpy(trp->t_fconcf.udata, opt->t_udatap,
				zmin(opt->t_udatal, T_MSG_SIZE));
		}

		FWRITE_CMX(trp, T_LCONCF);
		break;
	}
	case T_FCONIN: {
		/* t_trc (T_FCONIN, 0, tref, toaddr, fromaddr, opt, udatal) */

		struct t_optc1 *opt;

		trp->t_fconin.tref = *(va_arg(ap, int *));
		trp->t_fconin.toaddr = (struct t_myname *) va_arg(ap, union t_address *);
		trp->t_fconin.fromaddr = (struct t_partaddr *) va_arg(ap, union t_address *);
		opt = va_arg(ap, struct t_optc1 *);
		trp->t_fconin.udatal = va_arg(ap, int);
		if (((trp->t_fconin.opt = opt) != NULL)
			&& (trp->info != T_TRSTART)) {
			trp->t_fconin.t_optnr = opt->t_optnr;
			trp->t_fconin.t_udatap = opt->t_udatap;
			trp->t_fconin.t_udatal = opt->t_udatal;
			trp->t_fconin.t_xdata  = opt->t_xdata;
			trp->t_fconin.t_timeout = opt->t_timeout;
			memcpy(trp->t_fconin.udata, opt->t_udatap,
				zmin(opt->t_udatal, T_MSG_SIZE));
		}
		if (trp->info != T_TRSTART) {
			memcpy((char *) &(trp->t_fconin.t_pa),
				trp->t_fconin.fromaddr, sizeof (struct t_partaddr));
			memcpy((char *) &(trp->t_fconin.t_mn),
				trp->t_fconin.toaddr, sizeof (struct t_myname));
		}

		FWRITE_CMX(trp, T_LCONIN);
		break;
	}
	case T_FCONRS: {
		/* t_trc (T_FCONRS, 0, tref, opt) */

		t_opt13 *op;
		struct t_optc1 *opt;
		struct t_optc3 *op3;

		trp->t_fconrs.tref = *(va_arg(ap, int *));
		op = va_arg(ap, t_opt13 *);
		opt = (struct t_optc1 *) op;
		op3 = (struct t_optc3 *) op;

		if ((trp->t_fconrs.opt = op3) != NULL) {
			trp->t_fconrs.t_optnr = opt->t_optnr;
			trp->t_fconrs.t_udatap = opt->t_udatap;
			trp->t_fconrs.t_udatal = opt->t_udatal;
			trp->t_fconrs.t_xdata = opt->t_xdata;
			trp->t_fconrs.t_timeout = opt->t_timeout;
			if (opt->t_optnr == T_OPTC3)
				trp->t_fconrs.t_ucepid = op3->t_ucepid;
			memcpy(trp->t_fconrs.udata, opt->t_udatap,
				zmin(opt->t_udatal, T_MSG_SIZE));
		}

		FWRITE_CMX(trp, T_LCONRS);
		break;
	}
	case T_FREDRQ: {
		/* t_trc (T_FREDRQ, 0, tref, pid, opt) */

		t_opt12 *op;
		struct t_optc2 *opt;
		struct t_optc1 *op1;

		trp->t_fredrq.tref = *(va_arg(ap, int *));
		trp->t_fredrq.pid = *(va_arg(ap, int *));
		op = va_arg(ap, t_opt12 *);
		opt = (struct t_optc2 *) op;
		op1 = (struct t_optc1 *) op;

		if ((trp->t_fredrq.opt = op1) != NULL) {
			trp->t_fredrq.t_optnr = opt->t_optnr;
			trp->t_fredrq.t_udatap = opt->t_udatap;
			trp->t_fredrq.t_udatal = opt->t_udatal;
			if (opt->t_optnr == T_OPTC1)
				trp->t_fredrq.t_timeout = op1->t_timeout;
			memcpy(trp->t_fredrq.udata, opt->t_udatap,
				zmin(opt->t_udatal, T_MSG_SIZE));
		}

		FWRITE_CMX(trp, T_LREDRQ);
		break;
	}
	case T_FREDIN: {
		/* t_trc (T_FREDIN, 0, tref, pid, opt, udatal) */

		t_opt23 *op;
		struct t_optc2 *opt;
		struct t_optc3 *op3;

		trp->t_fredin.tref = *(va_arg(ap, int *));
		trp->t_fredin.pid = *(va_arg(ap, int *));
		op = va_arg(ap, t_opt23 *);
		trp->t_fredin.udatal = va_arg(ap, int);
		opt = (struct t_optc2 *) op;
		op3 = (struct t_optc3 *) op;

		if (((trp->t_fredin.opt = op3) != NULL)
			&& (trp->info != T_TRSTART)) {
			trp->t_fredin.t_optnr = opt->t_optnr;
			trp->t_fredin.t_udatap = opt->t_udatap;
			trp->t_fredin.t_udatal = opt->t_udatal;
			if (opt->t_optnr == T_OPTC3) {
				trp->t_fredin.t_timeout = op3->t_timeout;
				trp->t_fredin.t_xdata = op3->t_xdata;
				trp->t_fredin.t_ucepid = op3->t_ucepid;
			}
			memcpy(trp->t_fredin.udata, opt->t_udatap,
				zmin(opt->t_udatal, T_MSG_SIZE));
		}

		FWRITE_CMX(trp, T_LREDIN);
		break;
	}
	case T_FDISRQ: {
		/* t_trc (T_FDISRQ, 0, tref, opt) */

		struct t_optc2 *opt;

		trp->t_fdisrq.tref = *(va_arg(ap, int *));
		opt = va_arg(ap, struct t_optc2 *);

		if ((trp->t_fdisrq.opt = opt) != NULL) {
			trp->t_fdisrq.t_optnr = opt->t_optnr;
			trp->t_fdisrq.t_udatap = opt->t_udatap;
			trp->t_fdisrq.t_udatal = opt->t_udatal;
			memcpy(trp->t_fdisrq.udata, opt->t_udatap,
				zmin(opt->t_udatal, T_MSG_SIZE));
		}

		FWRITE_CMX(trp, T_LDISRQ);
		break;
	}
	case T_FDISIN: {
		/* t_trc (T_FDISIN, 0, tref, reason, opt, udatal) */

		struct t_optc2 *opt;

		trp->t_fdisin.tref = *(va_arg(ap, int *));
		trp->t_fdisin.reason = *(va_arg(ap, int *));
		opt = va_arg(ap, struct t_optc2 *);
		trp->t_fdisin.udatal = va_arg(ap, int);

		if (((trp->t_fdisin.opt = opt) != NULL)
		   && (trp->info != T_TRSTART)) {
			trp->t_fdisin.t_optnr = opt->t_optnr;
			trp->t_fdisin.t_udatap = opt->t_udatap;
			trp->t_fdisin.t_udatal = opt->t_udatal;
			memcpy(trp->t_fdisin.udata, opt->t_udatap,
				zmin(opt->t_udatal, T_MSG_SIZE));
		}

		FWRITE_CMX(trp, T_LDISIN);
		break;
	}
	case T_FDATARQ:
		/* t_trc (T_FDATARQ, 0, tref, datap, datal, chain) */

		trp->t_fdatarq.tref = *(va_arg(ap, int *));
		trp->t_fdatarq.datap = va_arg(ap, char *);
		trp->t_fdatarq.datal = va_arg(ap, int);
		trp->t_fdatarq.chain = va_arg(ap, int);

		trp->t_fdatarq.datacnt = zmin(trp->t_fdatarq.datal, t_trclen);

		FWRITE_NOWRAP_SIG(trp, T_LDATARQ);

		if (trp->info == T_TRSTART)
			break;

		FWRITE_WRAP_NOSIG((char *) trp->t_fdatarq.datap, trp->t_fdatarq.datacnt);
		break;

	case T_FVDATARQ: {
		/* t_trc (T_FVDATARQ, 0, tref, vdata, vcnt, chain) */

		struct t_data	*vp;

		trp->t_fvdatarq.tref = *(va_arg(ap, int *));
		trp->t_fvdatarq.vdata = vp = va_arg(ap, struct t_data *);
		trp->t_fvdatarq.vcnt = va_arg(ap, int);
		trp->t_fvdatarq.chain = va_arg(ap, int);

		FWRITE_NOWRAP_SIG(trp, T_LVDATARQ);

		if (trp->info == T_TRSTART)
			break;

		for (i = 0; i < zmin(trp->t_fvdatarq.vcnt, T_VCNT); i++, vp++) {
			FWRITE_NOWRAP_NOSIG((char *) vp, sizeof (struct t_data));
			FWRITE_NOWRAP_NOSIG(vp->t_datap, zmin(vp->t_datal, t_trclen));
		}
		if (t_trccnt >= t_trcwrap)
			t_trcsw();
		break;
	}
	case T_FDATAIN:
		/* t_trc (T_FDATAIN, x, tref, datap, datal, chain) */

		trp->t_fdatain.tref = *(va_arg(ap, int *));
		trp->t_fdatain.datap = va_arg(ap, char *);
		trp->t_fdatain.datal = va_arg(ap, int);
		trp->t_fdatain.chain = va_arg(ap, int);

		trp->t_fdatain.datacnt = zmin(trp->t_fdatain.datal & 0xFFFF, t_trclen);

		FWRITE_NOWRAP_SIG(trp, T_LDATAIN);

		if (trp->info == T_TRSTART)
			break;

		if (trp->info == T_TRFLSH)
			FWRITE_NOWRAP_NOSIG(trp->t_fdatain.datap, trp->t_fdatain.datacnt);
		if (t_trccnt >= t_trcwrap)
			t_trcsw();
		break;

	case T_FVDATAIN: {
		/* t_trc (T_FVDATAIN, x, tref, vdata, vcnt, chain) */

		struct t_data	*vp;

		trp->t_fvdatain.tref = *(va_arg(ap, int *));
		trp->t_fvdatain.vdata = vp = va_arg(ap, struct t_data *);
		trp->t_fvdatain.vcnt = va_arg(ap, int);
		trp->t_fvdatain.chain = va_arg(ap, int);

		FWRITE_NOWRAP_SIG(trp, T_LVDATAIN);

		if (trp->info == T_TRSTART)
			break;

		for (i = 0; i < zmin(trp->t_fvdatain.vcnt, T_VCNT); i++, vp++) {
			FWRITE_NOWRAP_NOSIG((char *) vp, sizeof (struct t_data));
			if (trp->info != T_TRFLSH)
				continue;
			FWRITE_NOWRAP_NOSIG(vp->t_datap, zmin(vp->t_datal & 0xFFFF, t_trclen));
		}
		if (t_trccnt >= t_trcwrap)
			t_trcsw();
		break;
	}
	case T_FXDATRQ:
		/* t_trc (T_FXDATRQ, 0, tref, datap, datal) */

		trp->t_fxdatrq.tref = *(va_arg(ap, int *));
		trp->t_fxdatrq.datap = va_arg(ap, char *);
		trp->t_fxdatrq.datal = va_arg(ap, int);

		if (trp->info != T_TRSTART) {
			memcpy(trp->t_fxdatrq.data, trp->t_fxdatrq.datap,
				zmin(trp->t_fxdatrq.datal, T_EXP_SIZE));
		}

		FWRITE_CMX(trp, T_LXDATRQ);
		break;

	case T_FXDATIN:
		/* t_trc (T_FXDATIN, x, tref, datap, datal) */

		trp->t_fxdatin.tref = *(va_arg(ap, int *));
		trp->t_fxdatin.datap = va_arg(ap, char *);
		trp->t_fxdatin.datal = va_arg(ap, int);

		memcpy(trp->t_fxdatin.data, trp->t_fxdatin.datap,
			zmin(trp->t_fxdatin.datal, T_EXP_SIZE));

		FWRITE_CMX(trp, T_LXDATIN);
		break;

	case T_FDATAGO:
	case T_FDATASTOP:
	case T_FXDATSTOP:
	case T_FXDATGO:
		/* t_trc (T_FDATAGO, 0, tref) */
		/* t_trc (T_FDATASTOP, 0, tref) */
		/* t_trc (T_FXDATGO, 0, tref) */
		/* t_trc (T_FXDATSTOP, 0, tref) */

		trp->t_fflow.tref = *(va_arg(ap, int *));

		FWRITE_CMX(trp, T_LFLOW);
		break;

	case T_FINFO: {
		/* t_trc (T_FINFO, 0, tref, opt) */

		struct t_opti1 *opt;

		trp->t_finfo.tref = *(va_arg(ap, int *));
		opt = va_arg(ap, struct t_opti1 *);

		trp->t_finfo.opt = opt;
		trp->t_finfo.t_optnr = opt->t_optnr;
		trp->t_finfo.t_maxl = opt->t_maxl;

			FWRITE_CMX(trp, T_LINFO);
		break;
	}
	case T_FINFO3: {
		/* t_trc (T_FINFO3, 0, tref, opt) */

		struct t_opti3 *opt;

		trp->t_finfo3.tref = *(va_arg(ap, int *));
		opt = va_arg(ap, struct t_opti3 *);

		trp->t_finfo3.opt = opt;
		trp->t_finfo3.t_optnr = opt->t_optnr;
		trp->t_finfo3.t_maxl = opt->t_maxl;
		trp->t_finfo3.t_fdsetsize = opt->t_fdsetsize;
		trp->t_finfo3.t_openmax = opt->t_openmax;
		trp->t_finfo3.t_tsdu = opt->t_tsdu;
		trp->t_finfo3.t_etsdu = opt->t_etsdu;
		trp->t_finfo3.t_condata = opt->t_condata;
		trp->t_finfo3.t_concfdata = opt->t_concfdata;
		trp->t_finfo3.t_disdata = opt->t_disdata;
		trp->t_finfo3.t_adrsize = opt->t_adrsize;
		FWRITE_CMX(trp, T_LINFO3);
		break;
	}
	case T_FEVENT: {
		/* t_trc (T_FEVENT, 0, tref, cmode, opt) */

		struct t_opte1 *opt;

		trp->t_fevent.tref = (int) va_arg(ap, int *);
		trp->t_fevent.cmode = va_arg(ap, int);
		opt = va_arg(ap, struct t_opte1 *);

		if ((trp->t_fevent.opt = opt) != NULL) {
			trp->t_fevent.t_optnr = opt->t_optnr;
			trp->t_fevent.t_attid = opt->t_attid;
			trp->t_fevent.t_uattid = opt->t_uattid;
			trp->t_fevent.t_ucepid = opt->t_ucepid;
			trp->t_fevent.t_timeout = opt->t_timeout;
			trp->t_fevent.t_evdat = opt->t_evdat;
		}

		FWRITE_CMX(trp, T_LEVENT);
		break;
	}
	case T_FERROR:
		/* t_trc (T_FERROR, error) */

		FWRITE_CMX(trp, T_LERROR);
		break;

	case T_DEVENT:
		/* t_trc(T_DEVENT, connection, mode) */

		if (!t_trcflg.deb)
			break;
		trp->t_devent.cmode = va_arg(ap, int);

		FWRITE_CMX(trp, T_LDEVENT);
		break;

	case T_FSETOPT: {
		/* t_trc(T_FSETOPT, component, opt) */

		t_opts *op;
		struct t_opts1 *opt;

		trp->t_fsetopt.component = va_arg(ap, int);
		op = va_arg(ap, t_opts *);
		opt = (struct t_opts1 *) op;
		trp->t_fsetopt.opt = opt;
		trp->t_fsetopt.t_optnr = opt->t_optnr;
		trp->t_fsetopt.t_optname = opt->t_optname;

		if (trp->t_fsetopt.opt) {
			strncpy(trp->t_fsetopt.t_optvalue, opt->t_optvalue,
				(size_t)zmin((int)strlen(opt->t_optvalue), sizeof(trp->t_fsetopt.t_optvalue)));
		}

		FWRITE_CMX(trp, T_LSETOPT);
		break;
		}

	case T_FCALLBACK:
		/* t_trc(T_FCALLBACK, routine, opt) */

		trp->t_fcallback.routine = va_arg(ap, int);
		trp->t_fcallback.usr = va_arg(ap, void *);
		trp->t_fcallback.opt = NULL;

		FWRITE_CMX(trp, T_LCALLBACK);
		break;

	/*------------------------------------------------------------*
	 * Call results
	 *------------------------------------------------------------*/

	case T_DRETOK:
		if (!t_trcflg.deb)
			break;
		/* fall thru */

	case T_FRETOK:
		/*
		 * t_trc(T_FRETOK, info, ap[0], ...)
		 * info is the identification for the call T_F*
		 * ap[0] is a value dependent on the call, namely with
		 * T_FATTACH: T_OK | T_NOTFIRST
		 * T_FCONRQ: tref
		 * T_FEVENT: event, tref
		 * T_FDATARQ, T_FXDATRQ, T_FXDATRQ: T_DATASTOP, T_XDATSTOP
		 * T_FDATAIN, T_FXDATIN, T_FXDATIN: Rest length
		 */
		switch (info) {
		case T_FATTACH:
		case T_FDATARQ:
		case T_FVDATARQ:
		case T_FXDATRQ:
		case T_FDATAIN:
		case T_FVDATAIN:
		case T_FXDATIN:
		case T_FSETOPT:
			trp->t_fretok.retval = va_arg(ap, int);
			break;

		case T_FCONRQ:
			trp->t_fretok.tref = *(va_arg(ap, int *));
			break;

		case T_FEVENT:
			trp->t_fretok.tref = *(va_arg(ap, int *));
			trp->t_fretok.retval = va_arg(ap, int);
			break;
		}

		FWRITE_CMX(trp, T_LRETOK);
		break;

	case T_FRETATT: {
		/* t_trc(T_FRETATT, 0, ok_notf, opt) */

		struct t_opta2 *op2;
		struct t_opta6 *op6;

		trp->t_fretatt.retval = va_arg(ap, int);
		op2 = va_arg(ap, struct t_opta2 *);
		op6 = (struct t_opta6 *)op2;

		trp->t_fretatt.t_optnr = 0;
		if (op2 != NULL &&
			((op2->t_optnr == T_OPTA2) ||
			 (op2->t_optnr == T_OPTA5) ||
			 (op2->t_optnr == T_OPTA6))) {
			trp->t_fretatt.t_optnr = op2->t_optnr;
			trp->t_fretatt.t_attid = op2->t_attid;
			trp->t_fretatt.t_uattid = op2->t_uattid;
			trp->t_fretatt.t_ccbits = op2->t_ccbits;
			trp->t_fretatt.t_sptypes = op2->t_sptypes;
			if (op6->t_optnr == T_OPTA6) {
				trp->t_fretatt.t_ccbits = (int)op6->t_cclist;
				trp->t_fretatt.t_cc = *op6->t_cclist;
			}
		}

		FWRITE_CMX(trp, T_LRETATT);
		break;
	}
	case T_FRETEVT: {
		/* t_trc(T_FRETEVT, 0, tref, evt, opt), opt IST ungleich NULL */

		struct t_opte1 *opt;

		trp->t_fevent.tref = *(va_arg(ap, int *));
		trp->t_fevent.cmode = va_arg(ap, int);
		opt = va_arg(ap, struct t_opte1 *);

		trp->t_fevent.t_optnr = opt->t_optnr;
		trp->t_fevent.t_attid = opt->t_attid;
		trp->t_fevent.t_uattid = opt->t_uattid;
		trp->t_fevent.t_ucepid = opt->t_ucepid;
		trp->t_fevent.t_timeout = opt->t_timeout;
		trp->t_fevent.t_evdat = opt->t_evdat;

		FWRITE_CMX(trp, T_LEVENT);
		break;
	}
	case T_DRETERR:
		if (!t_trcflg.deb)
			break;
		/* fall thru */

	case T_FRETERR:
		/* t_trc(T_FRETURN, info) */
		trp->t_freterr.error = info;

		FWRITE_CMX(trp, T_LRETERR);
		break;

	/*------------------------------------------------------------*
	 * Name and address info
	 *------------------------------------------------------------*/

	case T_FGETLOC:
		/* t_trc(T_FGETLOC, 0, glob, opt, result) */

		trp->t_fgetloc.glob = va_arg(ap, char *);
		trp->t_fgetloc.lglob = strlen(trp->t_fgetloc.glob) & 0xff;
		trp->t_fgetloc.opt = va_arg(ap, char *);
		trp->t_fgetloc.loc = va_arg(ap, struct t_myname *);
		if (trp->t_fgetloc.loc != NULL)
			memcpy((char *) &(trp->t_fgetloc.t_mn),
				(char *) trp->t_fgetloc.loc, sizeof (struct t_myname));

		FWRITE_NOWRAP_SIG(trp, T_LGETLOC);
		FWRITE_WRAP_NOSIG(trp->t_fgetloc.glob, trp->t_fgetloc.lglob);
		break;

	case T_FGETADDR:
		/* t_trc(T_FGETADDR, 0, glob, opt, result) */

		trp->t_fgetaddr.glob = va_arg(ap, char *);
		trp->t_fgetaddr.lglob = strlen(trp->t_fgetaddr.glob) & 0xff;
		trp->t_fgetaddr.opt = va_arg(ap, char *);
		trp->t_fgetaddr.addr = va_arg(ap, struct t_partaddr *);
		if (trp->t_fgetaddr.addr != NULL)
			memcpy((char *) &(trp->t_fgetaddr.t_pa),
				(char *) trp->t_fgetaddr.addr, sizeof (struct t_partaddr));

		FWRITE_NOWRAP_SIG(trp, T_LGETADDR);
		FWRITE_WRAP_NOSIG(trp->t_fgetaddr.glob, trp->t_fgetaddr.lglob);
		break;

	case T_FGETNAME:
		/* t_trc(T_FGETNAME, 0, addr, opt, result) */

		trp->t_fgetname.addr = va_arg(ap, struct t_partaddr *);
		trp->t_fgetname.opt = va_arg(ap, char *);
		trp->t_fgetname.name = va_arg(ap, char *);
		memcpy((char *) &(trp->t_fgetname.t_pa),
			trp->t_fgetname.addr, sizeof (struct t_partaddr));
		if (trp->t_fgetname.name != NULL)
			trp->t_fgetname.lname = strlen(trp->t_fgetname.name) & 0xff;

		FWRITE_NOWRAP_SIG(trp, T_LGETNAME);
		if (trp->t_fgetname.name != NULL)
			FWRITE_NOWRAP_NOSIG(trp->t_fgetname.name, trp->t_fgetname.lname);
		if (t_trccnt >= t_trcwrap)
			t_trcsw();
		break;

	/*------------------------------------------------------------*
	 * Log and SVC trace for INET module(s) and others
	 *------------------------------------------------------------*/

	case T_FTCPEOT:
		/* t_trc(T_FTCPEOT, info) */
		if (!t_trcflg.deb)
			break;
		FWRITE_CMX(trp, T_LHEADER);
		break;

	case T_FTUIIN:
	case T_FTUIOUT:
		/* t_trc(T_FTUI*, len, buf) */
		trp->relsec = (short) info;
		if (trp->info > T_TRENTLNG)
			trp->info = info = T_TRENTLNG;
		goto output;

	case T_FTCPBDY:
	case T_FTCPPKT:
	case T_FTCPHDR:
		/* t_trc(T_FTCPPKT, len, buf) */
		/* t_trc(T_FTCPHDR, len, buf) */
		/* t_trc(T_FTCPBDY, info:len, buf, udatl) */
		if (!t_trcflg.deb)
			break;
		hpptr = va_arg(ap, char *);
		if (id == T_FTCPBDY)
			trp->relsec = (short) va_arg(ap, int);
output:
		FWRITE_NOWRAP_SIG(trp, T_LHEADER);
		FWRITE_WRAP_NOSIG((char *) hpptr, info & 0xff);
		break;

	case T_FTCPSSOPT:
	case T_FTCPGSOPT:
		/* t_trc(T_FTCPSSOPT, 0, socket, level, optname, optval, optlen, res, errno) */
		/* t_trc(T_FTCGSSOPT, 0, socket, level, optname, optval, optlen, res, errno) */
		if (t_trcflg.deb==T_NODEBUG)
			break;
		trp->t_ftcpsopt.socket = va_arg(ap, int);
		trp->t_ftcpsopt.level = va_arg(ap, int);
		trp->t_ftcpsopt.optname = va_arg(ap, int);
		trp->t_ftcpsopt.optval = va_arg(ap, int);
		trp->t_ftcpsopt.optlen = va_arg(ap, int);
		trp->t_ftcpsopt.res = va_arg(ap, int);
		trp->t_ftcpsopt.cmx_errno = va_arg(ap, int);

		FWRITE_CMX(trp, T_LTCPSOPT);

		break;

	case T_FTCPSCK:
		/* t_trc(T_FTCPSCK, 0, dom, type, prot, res, errno) */
		if (t_trcflg.deb==T_NODEBUG)
			break;
		trp->t_ftcpsck.dom = va_arg(ap, int);
		trp->t_ftcpsck.type = va_arg(ap, int);
		trp->t_ftcpsck.prot = va_arg(ap, int);
		trp->t_ftcpsck.res = va_arg(ap, int);
		trp->t_ftcpsck.cmx_errno = va_arg(ap, int);

		FWRITE_CMX(trp, T_LTCPSCK);

		break;

	case T_FTCPACC:
	case T_FTCPBIN:
	case T_FTCPCON:
		/* t_trc(T_FTCPACC, 0, sd, name, res, errno) */
		/* t_trc(T_FTCPBIN, 0, sd, name, res, errno) */
		/* t_trc(T_FTCPCON, 0, sd, name, res, errno) */
		if (!t_trcflg.deb)
			break;
		trp->t_ftcpabc.sd = va_arg(ap ,int);
		memcpy(trp->t_ftcpabc.name, (char *) va_arg(ap, char *),
			sizeof (struct sockaddr));
		trp->t_ftcpabc.res = va_arg(ap ,int);
		trp->t_ftcpabc.cmx_errno = va_arg(ap ,int);

		FWRITE_CMX(trp, T_LTCPABC);
		break;

	case T_FTCPLIS:
		/* t_trc(T_FTCPLIS, 0, sd, back, res, errno) */
		if (!t_trcflg.deb)
			break;
		trp->t_ftcplis.sd = va_arg(ap, int);
		trp->t_ftcplis.back = va_arg(ap, int);
		trp->t_ftcplis.res = va_arg(ap, int);
		trp->t_ftcplis.cmx_errno = va_arg(ap, int);

		FWRITE_CMX(trp, T_LTCPLIS);
		break;

	case T_FTCPSEL:
		/* t_trc(T_FTCPSEL, x, nsd, rfds, wfds, NULL, sec, res, errno) */
		if (!t_trcflg.deb)
			break;
		trp->t_ftcpsel.nsd = va_arg(ap, int);
		hrfds = va_arg(ap, char *);
		hwfds = va_arg(ap, char *);
		hxfds = va_arg(ap, char *);
		hsec = va_arg(ap, int);
		if (hrfds == (char *)  0)
			FD_ZERO(&trp->t_ftcpsel.rfds);
		else
			memcpy((char *) &trp->t_ftcpsel.rfds, (char *) hrfds,
				sizeof(fd_set));
		if (hwfds == (char *)  0)
			FD_ZERO(&trp->t_ftcpsel.wfds);
		else
			memcpy((char *) &trp->t_ftcpsel.wfds, (char *) hwfds,
				sizeof(fd_set));
		if (hxfds == (char *) 0)
			FD_ZERO(&trp->t_ftcpsel.xfds);
		else
			memcpy((char *) &trp->t_ftcpsel.xfds, (char *) hxfds,
				sizeof(fd_set));
		if (hsec)
			trp->t_ftcpsel.sec = ((struct timeval *) hsec)->tv_sec;
		else
			trp->t_ftcpsel.sec = -1;
		trp->t_ftcpsel.res = va_arg(ap, int);
		trp->t_ftcpsel.cmx_errno = va_arg(ap, int);

		FWRITE_CMX(trp, T_LTCPSEL);
		break;

	case T_FCBRES:
		/* t_trc(T_FCBRES, x, nsd, rfds, wfds, NULL, sec, res, errno) */
		if (!t_trcflg.deb)
			break;
		trp->t_fcbres.nsd = va_arg(ap, int);
		hrfds = va_arg(ap, char *);
		hwfds = va_arg(ap, char *);
		hxfds = va_arg(ap, char *);
		hsec = va_arg(ap, int);
		trp->t_fcbres.res = va_arg(ap, int);
		trp->t_fcbres.cmx_errno = va_arg(ap, int);
		if (hrfds == (char *)  0)
			FD_ZERO(&trp->t_fcbres.rfds);
		else
			memcpy((char *) &trp->t_fcbres.rfds, (char *) hrfds,
				sizeof(fd_set));
		if (hwfds == (char *)  0)
			FD_ZERO(&trp->t_fcbres.wfds);
		else
			memcpy((char *) &trp->t_fcbres.wfds, (char *) hwfds,
				sizeof(fd_set));
		if (hxfds == (char *) 0)
			FD_ZERO(&trp->t_fcbres.xfds);
		else
			memcpy((char *) &trp->t_fcbres.xfds, (char *) hxfds,
				sizeof(fd_set));
		if (hsec)
			trp->t_fcbres.sec = ((struct timeval *) hsec)->tv_sec;
		else
			trp->t_fcbres.sec = -1;

		FWRITE_CMX(trp, T_LCBRES);
		break;

	case T_FOPEN:
	case T_FMKNOD:
	case T_FUNLINK:
		/* t_trc (T_FOPEN,   0,    path, mode, res, errno); */
		/* t_trc (T_FMKNOD,  flag, path, mode, res, errno); */
		/* t_trc (T_FUNLINK, 0,    path, 0,    res, errno); */

		if (!t_trcflg.deb)
			break;

		memcpy(trp->t_fopen.path, va_arg(ap, char *), 16);
		trp->t_fopen.mode = va_arg(ap, int);
		trp->t_fopen.res = va_arg(ap, int);
		trp->t_fopen.cmx_errno = va_arg(ap, int);

		FWRITE_CMX(trp, T_LOPEN);
		break;

	case T_FREAD:
	case T_FWRITE:
	case T_FIOCTL:
	case T_FFCNTL:
	case T_FCLOSE:
		/* t_trc(T_FREAD, 0, fd, 0, len, res, errno) */
		/* t_trc(T_FWRITE, 0, fd, 0, len, res, errno) */
		/* t_trc(T_FIOCTL, 0, fd, cmd, arg, res, errno) */
		/* t_trc(T_FFCNTL, 0, fd, cmdflg, arg, res, errno) */
		/* t_trc(T_FCLOSE, 0, fd) */

		if (!t_trcflg.deb)
			break;
		trp->t_fsvc.fd = va_arg(ap, int);
		if (id != T_FCLOSE) {
			trp->t_fsvc.arg2 = va_arg(ap, int);
			trp->t_fsvc.arg3 = va_arg(ap, int);
			trp->t_fsvc.res = va_arg(ap, int);
			trp->t_fsvc.cmx_errno = va_arg(ap, int);
		}

		FWRITE_CMX(trp, T_LSVC);
		break;

	case T_DLERRO:
	case T_DLERRS:
	case T_STRING:
		if ((strng = va_arg(ap,char *)) == NULL)
		   strng = "(NULL)";
		strcnt = strlen(strng);
		if (strcnt > (size_t)119) strcnt = (size_t)119;
		trp->t_fdlerr.errarray[0] = '\0';
		strncat((char *)trp->t_fdlerr.errarray,strng,strcnt);
		FWRITE_CMX(trp, T_LDLERR);
		break;

	case T_FSTRING:
		/* t_trc(T_FSTRING, info_int, string_to_print) */
		if (!t_trcflg.deb)
			break;
		sprintf (strbuf, "%.78s\n", va_arg(ap, char *));

		FWRITE_NOWRAP_SIG(trp, T_LHEADER);
		FWRITE_WRAP_NOSIG(strbuf, strlen (strbuf));
		break;
	case T_FSNAPIPE:
		/* t_trc(T_FSNAPIPE, info_int, (pipe_buf *) pipe_buf) */

		if (!t_trcflg.deb)
			break;

		hpptr = va_arg(ap, char *);
		FWRITE_NOWRAP_SIG(trp, T_LHEADER);
		FWRITE_WRAP_NOSIG((void *)hpptr, ((pipe_buf *)hpptr)->len_ges);
		break;

	/*------------------------------------------------------------*
	 * CMX on top of XTI/TLI
	 *------------------------------------------------------------*/
	case T_XTI_ACCEPT:
		/* t_trc(T_XTI_ACCEPT, 0, fd, resfd, call) */
		if (!t_trcflg.deb)
			break;
		trp->t_fXaccept.fd = GET_ARG(int);
		trp->t_fXaccept.resfd = GET_ARG(int);

		XTI_PTR(t_fXaccept.callp, t_fXaccept.call, struct t_call *);
		FWRITE_NOWRAP_SIG(trp, T_LxACCEPT);

		XTI_NBUF(t_fXaccept.callp, t_fXaccept.call.addr, FWRITE_NOWRAP_NOSIG);
		XTI_NBUF(t_fXaccept.callp, t_fXaccept.call.opt, FWRITE_NOWRAP_NOSIG);
		XTI_NBUF(t_fXaccept.callp, t_fXaccept.call.udata, FWRITE_WRAP_NOSIG);
		break;

	case T_XTI_BIND:
		/* t_trc(T_XTI_BIND, 0, fd, req, ret); */
		if (!t_trcflg.deb)
			break;
		trp->t_fXbind.fd = GET_ARG(int);
		XTI_PTR(t_fXbind.reqp, t_fXbind.req, struct t_bind *);
		XTI_PTR(t_fXbind.retp, t_fXbind.ret, struct t_bind *);
		FWRITE_NOWRAP_SIG(trp, T_LxBIND);
		XTI_NBUF(t_fXbind.reqp, t_fXbind.req.addr, FWRITE_WRAP_NOSIG);
		break;

	case T_XTI_BIND_FIN:
		/* t_trc(T_XTI_BIND_FIN, 0, ret, retval, t_errno, errno); */
		if (!t_trcflg.deb)
			break;
		XTI_PTR(t_fXbindfin.retp, t_fXbindfin.ret, struct t_bind *);
		XTI_RET(t_fXbindfin);
		FWRITE_NOWRAP_SIG(trp, T_LxBINDFIN);
		XTI_NBUF(t_fXbindfin.retp, t_fXbindfin.ret.addr, FWRITE_WRAP_NOSIG);
		break;

	case T_XTI_CONNECT:
		/* t_trc(T_XTI_CONNECT, 0, fd, sndcall, rcvcall) */
		if (!t_trcflg.deb)
			break;
		trp->t_fXconnect.fd = GET_ARG(int);
		XTI_PTR(t_fXconnect.sndcallp, t_fXconnect.sndcall, struct t_call *);
		XTI_PTR(t_fXconnect.rcvcallp, t_fXconnect.rcvcall, struct t_call *);
		FWRITE_NOWRAP_SIG(trp, T_LxCONNECT);

		XTI_NBUF(t_fXconnect.sndcallp, t_fXconnect.sndcall.addr, FWRITE_NOWRAP_NOSIG);
		XTI_NBUF(t_fXconnect.sndcallp, t_fXconnect.sndcall.opt, FWRITE_NOWRAP_NOSIG);
		XTI_NBUF(t_fXconnect.sndcallp, t_fXconnect.sndcall.udata, FWRITE_WRAP_NOSIG);
		break;

	case T_XTI_CONNECT_FIN:
		/* t_trc(T_XTI_CONNECT_FIN, 0, rcvcall, retval, t_errno, errno) */
	case T_XTI_LISTEN_FIN:
		/* t_trc(T_XTI_LISTEN_FIN, 0, call, retval, t_errno, errno) */
	case T_XTI_RCVCONNECT_FIN:
		/* t_trc(T_XTI_RCVCONNECT_FIN, 0, call, retval, t_errno, errno) */
		if (!t_trcflg.deb)
			break;
		XTI_PTR(t_fXcallfin.callp, t_fXcallfin.call, struct t_call *);
		XTI_RET(t_fXcallfin);
		FWRITE_NOWRAP_SIG(trp, T_LxCALLFIN);

		XTI_NBUF(t_fXcallfin.callp, t_fXcallfin.call.addr, FWRITE_NOWRAP_NOSIG);
		XTI_NBUF(t_fXcallfin.callp, t_fXcallfin.call.opt, FWRITE_NOWRAP_NOSIG);
		XTI_NBUF(t_fXcallfin.callp, t_fXcallfin.call.udata, FWRITE_WRAP_NOSIG);
		break;

	case T_XTI_LISTEN:
		/* t_trc(T_XTI_LISTEN, 0, fd, call) */
	case T_XTI_RCVCONNECT:
		/* t_trc(T_XTI_RCVCONNECT, 0, fd, call) */
		if (!t_trcflg.deb)
			break;
		trp->t_fXcall.fd = GET_ARG(int);
		XTI_PTR(t_fXcall.callp, t_fXcall.call, struct t_call *);
		FWRITE_WRAP_SIG(trp, T_LxCALL);
		break;

	case T_XTI_CLOSE:
		/* t_trc(T_XTI_CLOSE, 0, fd) */
	case T_XTI_GETSTATE:
		/* t_trc(T_XTI_GETSTATE, 0, fd) */
	case T_XTI_LOOK:
		/* t_trc(T_XTI_LOOK, 0, fd) */
	case T_XTI_RCVREL:
		/* t_trc(T_XTI_RCVREL, 0, fd) */
	case T_XTI_SNDREL:
		/* t_trc(T_XTI_SNDREL, 0, fd) */
	case T_XTI_SYNC:
		/* t_trc(T_XTI_LSYNC, 0, fd) */
	case T_XTI_UNBIND:
		/* t_trc(T_XTI_UNBIND, 0, fd) */
		if (!t_trcflg.deb)
			break;
		trp->t_fXfd.fd = GET_ARG(int);
		FWRITE_WRAP_SIG(trp, T_LxFD);
		break;

	case T_XTI_ACCEPT_FIN:
		/* t_trc(T_XTI_ACCEPT_FIN, 0, retval, t_errno, errno) */
	case T_XTI_CLOSE_FIN:
		/* t_trc(T_XTI_CLOSE_FIN, 0, retval, t_errno, errno) */
	case T_XTI_GETSTATE_FIN:
		/* t_trc(T_XTI_GETSTATE_FIN, 0, retval, t_errno, errno) */
	case T_XTI_LOOK_FIN:
		/* t_trc(T_XTI_LOOK_FIN, 0, retval, t_errno, errno) */
	case T_XTI_RCVREL_FIN:
		/* t_trc(T_XTI_RCVREL_FIN, 0, retval, t_errno, errno) */
	case T_XTI_SND_FIN:
		/* t_trc(T_XTI_SND_FIN, 0, retval, t_errno, errno) */
	case T_XTI_SNDDIS_FIN:
		/* t_trc(T_XTI_SNDDIS_FIN, 0, retval, t_errno, errno) */
	case T_XTI_SNDREL_FIN:
		/* t_trc(T_XTI_SNDREL_FIN, 0, retval, t_errno, errno) */
	case T_XTI_SYNC_FIN:
		/* t_trc(T_XTI_SYNC_FIN, 0, retval, t_errno, errno) */
	case T_XTI_UNBIND_FIN:
		/* t_trc(T_XTI_UNBIND_FIN, 0, retval, t_errno, errno) */
		if (!t_trcflg.deb)
			break;
		XTI_RET(t_fXfin);
		FWRITE_WRAP_SIG(trp, T_LxFIN);
		break;

	case T_XTI_OPEN:
		/* t_trc(T_XTI_OPEN, 0, name, oflag, info) */
		if (!t_trcflg.deb)
			break;
		if ((trp->t_fXopen.namep = GET_ARG(char *)) != NULL)
			trp->t_fXopen.namel = strlen (trp->t_fXopen.namep) + 1;
		trp->t_fXopen.oflag = GET_ARG(int);
		trp->t_fXopen.infop = GET_ARG(struct t_info *);
		FWRITE_NOWRAP_SIG(trp, T_LxOPEN);

		if (trp->t_fXopen.namep != NULL)
			FWRITE_WRAP_NOSIG(trp->t_fXopen.namep, XMIN(BUFSIZ, trp->t_fXopen.namel));
		break;

	case T_XTI_OPEN_FIN:
		/* t_trc(T_XTI_OPEN_FIN, 0, info, retval, t_errno, errno) */
		if (!t_trcflg.deb)
			break;
		XTI_PTR(t_fXopenfin.infop, t_fXopenfin.info, struct t_info *);
		XTI_RET(t_fXopenfin);
		FWRITE_WRAP_SIG(trp, T_LxOPENFIN);
		break;

	case T_XTI_RCV:
		/* t_trc(T_XTI_RCV, 0, fd, buf, nbytes) */
		if (!t_trcflg.deb)
			break;
		trp->t_fXrcvreq.fd = GET_ARG(int);
		trp->t_fXrcvreq.bufp = GET_ARG(char *);
		trp->t_fXrcvreq.nbytes = GET_ARG(unsigned);
		FWRITE_WRAP_SIG(trp, T_LxRCVREQ);
		break;

	case T_XTI_RCV_FIN:
		/* t_trc(T_XTI_RCV_FIN, 0, buf, nbytes, *flags, retval, t_errno, errno) */
		if (!t_trcflg.deb)
			break;
		trp->t_fXrcvfin.bufp = GET_ARG(char *);
		trp->t_fXrcvfin.nbytes = GET_ARG(unsigned);
		trp->t_fXrcvfin.flags = GET_ARG(int);
		XTI_RET(t_fXrcvfin);
		trp->t_fXrcvfin.nbytest = XMIN(BUFSIZ, zmin(trp->t_fXrcvfin.xret.retval, t_trclen));
		FWRITE_NOWRAP_SIG(trp, T_LxRCVFIN);
		if (trp->t_fXrcvfin.bufp != 0 && trp->t_fXrcvfin.nbytest > 0)
			FWRITE_WRAP_NOSIG((char *) trp->t_fXrcvfin.bufp, trp->t_fXrcvfin.nbytest);
		break;

	case T_XTI_RCVDIS:
		/* t_trc(T_XTI_RCVDIS, 0, fd, discon) */
		if (!t_trcflg.deb)
			break;
		trp->t_fXdis.fd = GET_ARG(int);
		XTI_PTR(t_fXdis.disp, t_fXdis.dis, struct t_discon *);
		FWRITE_WRAP_SIG(trp, T_LxDIS);
		break;

	case T_XTI_RCVDIS_FIN:
		/* t_trc(T_XTI_RCVDIS_FIN, 0, discon, retval, t_errno, errno) */
		if (!t_trcflg.deb)
			break;
		XTI_PTR(t_fXdis.disp, t_fXdis.dis, struct t_discon *);
		XTI_RET(t_fXdis);
		FWRITE_NOWRAP_SIG(trp, T_LxDIS);
		XTI_NBUF(t_fXdis.disp, t_fXdis.dis.udata, FWRITE_WRAP_NOSIG);
		break;

	case T_XTI_SND:
		/* t_trc(T_XTI_SND, 0, fd, buf, nbytes, flags) */
		trp->t_fXsndreq.fd = GET_ARG(int);
		trp->t_fXsndreq.bufp = GET_ARG(char *);
		trp->t_fXsndreq.nbytes = GET_ARG(int);
		trp->t_fXsndreq.flags = GET_ARG(int);
		trp->t_fXsndreq.nbytest = XMIN(BUFSIZ, XMIN(trp->t_fXsndreq.nbytes, t_trclen));
		FWRITE_NOWRAP_SIG(trp, T_LxSNDREQ);
		if (trp->t_fXsndreq.bufp != 0 && trp->t_fXsndreq.nbytest > 0)
			FWRITE_WRAP_NOSIG((char *) trp->t_fXsndreq.bufp, trp->t_fXsndreq.nbytest);
		break;

	case T_XTI_SNDDIS: {
		/* t_trc(T_XTI_SNDDIS, 0, fd, call) */
		if (!t_trcflg.deb)
			break;
		trp->t_fXcall.fd = GET_ARG(int);
		XTI_PTR(t_fXcall.callp, t_fXcall.call, struct t_call *);
		FWRITE_NOWRAP_SIG(trp, T_LxCALL);
		XTI_NBUF(t_fXcall.callp, t_fXcall.call.udata, FWRITE_WRAP_NOSIG);
		break;
	}
	default:
		FPRINTF(stderr, "t_trc: bad id 0x%x\n", id);
		break;
	}
	if (dce_serviceability) {
	        DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_CMX,
					   svc_c_debug1, GDS_S_CMX_FENTRY, 0)) ;
	}
	va_end(ap);
	errno = myerrno;
	return;
}

static int zmin (int	v1,
		 int	v2)
{
	int	ret;

	if ((ret = (v1 > v2) ? v2 : v1) < 0)
		return (0);
	return (ret);
}


/*
 * t_trcon -- activate ICMX(L) TRACE
 *
 * TRACE is controlled using t_opttrace function
 *
 */
static int t_trcon	(int	dflt)
{
	extern int t_getopt(int);
	t_trccnt = 0;

	t_getopt(dflt); /* get trace options */

	FPRINTF(stderr, "t_trcfac = %d, ", t_trcfac);
	FPRINTF(stderr, "t_trcsel = %d, ", t_trcflg.sel);
	FPRINTF(stderr, "t_trclen = %d, ", t_trclen);
	FPRINTF(stderr, "t_trcwrap = %d\n", t_trcwrap);

	/* if DCE-serviceability is enabled the previous trace file handling */
	/* is not longer required */
	if (dce_serviceability) {
		t_trchead();
		return (T_OK) ;
	}

	if (t_trcflg.sel) {
		int	fd = -1;

		if (t_trcflg.sel == T_TRCOMPACT)
			t_trclen = 0;
		/*
		 * prepare current (and alt.) TRACE stream
		 * (for later wrap around)
		 */
		if (strlen(t_trcdir) == 0) { /* default directory */
			t_trcdir[0] = '\0';
			strncat(t_trcdir, T_TRCDIR, strlen(T_TRCDIR));
		}

		t_trclp[0] = '\0';
		strcpy(t_trclp,t_trcdir);
		strncat(t_trclp + strlen(t_trclp), T_TRCMXL,strlen(T_TRCMXL)-5);
		sprintf(t_trclp + strlen(t_trclp),"%05ld",getpid());
		FPRINTF(stderr, "t_trcon: t_trclp = %s\n", t_trclp);

		t_trccurr = t_trclp;
		if (t_trcopen(&fd, t_trccurr) == T_ERROR)
			return (T_ERROR);
		t_trcfd = fdopen(fd, "a");

		t_trcmp[0] = '\0';
		strcpy(t_trcmp,t_trcdir);
		strncat(t_trcmp + strlen(t_trcmp), T_TRCMXM,strlen(T_TRCMXM)-5);
		sprintf(t_trcmp + strlen(t_trcmp),"%05ld",getpid());

		FPRINTF(stderr, "t_trcon: t_trcmp = %s\n", t_trcmp);

		switch (t_trcfac) {
		case 0:
			if (setvbuf(t_trcfd, NULL, _IONBF, 0)) {
				FPRINTF(stderr, "t_trcon: setvbuf failed\n", 0);
				return (T_ERROR);
			}
			break;

		default:
			if ((t_trcbp = (char *) malloc((unsigned) (t_trcfac * BUFSIZ)))
				!= NULL) {
				if (setvbuf(t_trcfd, t_trcbp, _IOFBF,
					t_trcfac * BUFSIZ)) {
					FPRINTF(stderr, "t_trcon: setvbuf failed\n", 0);
					return (T_ERROR);
				}
			} else {
				FPRINTF(stderr, "t_trcon: malloc failed\n", 0);
				return (T_ERROR);
			}
			break;
		}

		/*
		 * First entry
		 * Contains the version of CMX library, to enable
		 * evaluation irrelevant of version.
		 */

		t_trchead();

		return (T_OK);
	}
	return (T_ERROR);
}

/*
 * t_trcoff -- deactivate ICMX(L) TRACE
 *
 * Close TRACE file abd release buffer if possible
 */
void t_trcoff	(int flsh)
{
    if (!dce_serviceability) {
	if (flsh == T_TRNFLSH && t_trcfd != NULL)
		close(fileno(t_trcfd));		/* no flush */
	fclose(t_trcfd);
	t_trcfd = NULL;
	if (t_trcbp) {
		free(t_trcbp);
		t_trcbp = NULL;
	}
    }
	t_trcflg.sel = T_TRDONOT;               /* nothing works */
	t_trcpid = -1;
}

/*
 * trcopen -- open TRACE file
 * If does not exist, configure with "-rw-------" rights;
 * continue previous file.
 */
static int t_trcopen	(int	*fd,
			 char	*template)
{
	if (*fd == -1) {
		if ((*fd = open(template, 2)) == T_ERROR) {
			if (errno == ENOENT) {
				if ((*fd = creat(template, 0600)) == T_ERROR) {
					_t_error = errno;
					return (T_ERROR);
				}
			} else {
				_t_error = errno;
				return (T_ERROR);
			}
		}
		fcntl(*fd, F_SETFD, 1);
		if (lseek(*fd, 0L, 2) == T_ERROR) {
			_t_error = errno;
			return (T_ERROR);
		}
	}
	return (T_OK);
}

/*
 * t_trcsw -- switch to TRACE file
 * (re)configure alternative TRACE file here for "-rw-------" rights
 */
static void t_trcsw	(void)
{

	if (t_trccurr == t_trclp)
		t_trccurr = t_trcmp;
	else
		t_trccurr = t_trclp;
	close(creat(t_trccurr, 0600));

	if (freopen(t_trccurr, "w", t_trcfd) == NULL) {
		t_trcoff(T_TRNFLSH);
		return;
	}

	switch (t_trcfac) {
	case 0:
		if (setvbuf(t_trcfd, NULL, _IONBF, 0)) {
			FPRINTF(stderr, "t_trcon: setvbuf failed\n", 0);
			t_trcoff(T_TRNFLSH);
			return;
		}
		break;

	default:
		if (setvbuf(t_trcfd, t_trcbp, _IOFBF, t_trcfac * BUFSIZ)) {
			FPRINTF(stderr, "t_trcon: setvbuf failed\n", 0);
			t_trcoff(T_TRNFLSH);
			return;
		}
		break;
	}

	t_trchead();
	t_trccnt = 0;
}

/*
 * t_trchead -- enter header and write to file
 */
static void t_trchead(void)
{
#define SINIX (const char*)"SINIX"
#define IRIX  (const char*)"IRIX"
	struct t_trcentry	*trp = &t_trcel;
	struct utsname unameres;	/* result of uname */
	int	versfile;

	trp->id = T_LIBV40;
	trp->info = t_trcflg.sel;
	trp->t_ffirst2.abs = t_trcbeg = time((long *) NULL);
	trp->t_ffirst2.len = t_trclen;
	trp->t_ffirst2.fdsetsize = FD_SETSIZE;
	trp->t_ffirst2.msgsize = T_MSG_SIZE;
/*
 * Enter CMX version letters in trace head
 */

	trp->t_ffirst2.version = '?';  /* default */
	if (uname((struct utsname *)&unameres) >= 0) {
		if (!strncmp((const char*)unameres.sysname, SINIX, (size_t)5))
			trp->t_ffirst2.version = unameres.sysname[6];
		if (!strncmp((const char*)unameres.sysname, IRIX, (size_t)4))
			trp->t_ffirst2.version = 'U';
		if (dce_serviceability) {
			DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%b%b"),
			GDS_S_CMX, svc_c_debug1, GDS_S_CMX_LOG, T_LFIRST2,
								trp, 0, NULL)) ;
	        	DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"),
			    GDS_S_CMX, svc_c_debug1, GDS_S_CMX_FENTRY, 0)) ;
		} else
			fwrite(trp, T_LFIRST2, 1, t_trcfd);
/*
 * uname-struktur -> Header 
 */
		trp->id = T_UNAME;
		trp->t_funame.unameres = unameres;
/*
 * CMX-Version -> header
 */
		if ((versfile = open(T_VERSION, O_RDONLY)) > 0) {
			read(versfile,trp->t_funame.version,(size_t)T_VER_SIZE);			close(versfile);
		}
	
		if (dce_serviceability) {
			DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%b%b"),
			GDS_S_CMX, svc_c_debug1, GDS_S_CMX_LOG, T_LUNAME,
								trp, 0, NULL)) ;
	        	DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"),
			    GDS_S_CMX, svc_c_debug1, GDS_S_CMX_FENTRY, 0)) ;
		} else
			fwrite(trp, T_LUNAME, 1, t_trcfd);
	}
	else {
	    if (dce_serviceability) {
		DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%b%b"), GDS_S_CMX,
			svc_c_debug1, GDS_S_CMX_LOG, T_LFIRST2, trp, 0, NULL)) ;
	        DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_CMX,
					   svc_c_debug1, GDS_S_CMX_FENTRY, 0)) ;
	    } else
		fwrite(trp, T_LFIRST2, 1, t_trcfd);
	}
	return;
}
