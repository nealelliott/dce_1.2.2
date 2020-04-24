/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d25stub.c,v $
 * Revision 1.1.12.2  1996/02/18  18:23:28  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:46:25  marty]
 *
 * Revision 1.1.12.1  1995/12/08  16:15:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:21:03  root]
 * 
 * Revision 1.1.10.5  1994/09/06  12:26:33  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:25:02  keutel]
 * 
 * Revision 1.1.10.4  1994/07/06  15:08:18  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:20:59  marrek]
 * 
 * Revision 1.1.10.3  1994/05/10  16:03:32  marrek
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/05  06:42:38  marrek]
 * 
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/02  13:23:16  marrek]
 * 
 * 	Bug fixes.
 * 	[1994/04/29  09:30:35  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  13:12:54  marrek]
 * 
 * Revision 1.1.10.2  1994/03/23  15:47:26  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:33:47  keutel]
 * 
 * Revision 1.1.10.1  1994/02/22  17:55:31  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:52:46  marrek]
 * 
 * Revision 1.1.8.1  1993/10/14  19:19:06  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  19:16:15  keutel]
 * 
 * 	October 1993 code drop
 * 	[1993/10/09  17:58:12  keutel]
 * 
 * Revision 1.1.6.2  1993/08/11  08:32:24  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  12:20:42  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  22:04:31  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:48:20  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/17  23:13:52  tom
 * 	Bug 6411 - Add setlocale call for i18n.
 * 	[1992/12/17  23:09:12  tom]
 * 
 * Revision 1.1.4.2  1992/11/27  18:29:24  marrek
 * 	November 1992 code drop
 * 	[1992/11/23  15:09:28  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:10:21  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:33:47  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d25stub.c,v $ $Revision: 1.1.12.2 $ $Date: 1996/02/18 18:23:28 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton **********************************************************************/
/*                                                                           */
/*   TYPE:    program                                                        */
/*                                                                           */
/*   NAME:    d25stub                                                        */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The Stub process provides within the architecture of DIR-X the      */
/*       multiplexing of the outgoing DSP-informations (occurring on the     */
/*       IPC-associations which are established FROM DSA-processes to the    */
/*       Stub) onto ROS-associations and also of the incoming DAP/DSP-       */
/*       informations (occurring on ROS-associations) onto IPC-associations  */
/*       which are established by the Stub TO DSA-processes.                 */
/*                                                                           */
/*       This multiplexing must be done because the used transport system    */
/*       protocol stack doesn't allow that a transport connection (and this  */
/*       means also a ROS-association) is controlled by several processes.   */
/*                                                                           */
/*   CALLING PARAMETER:                                                      */
/*      -f         = Causes execution of the Stub in the foreground. Be-     */
/*                   cause the Stub is normally running in the background    */
/*                   as a daemon, this flag should be used if there is the   */
/*                   need to run the Stub under the control of a debugger.   */
/*      -d<nn>     = Identifier of the directory (1 <= <nn> <= D2_MAXDIR_ID) */
/*                   to which the Stub belongs. If this flag is not supplied */
/*                   then the identifier 1 is assumed.                       */
/*      -R<nnn>    = Max. # of ROS-associations which are handled by the     */
/*                   Stub (1 <= <nnn> <= D25_MXROSASSOC). If this flag is    */
/*                   not supplied then D25_DFROSASSOC is assumed.            */
/*      -I<nnn>    = Max. # of IPC-associations which are handled by the     */
/*                   Stub (1 <= <nnn> <= D23_MXCLTPROC). If this flag is     */
/*                   not supplied then D25_DFIPCASSOC is assumed.            */
/*      -P, -v, -w = Serviceability-switches.                                */
#ifdef DIRV30
/*       -T        = Enable ASN.1-ADM transfer syntax test mode              */
#endif /* DIRV30 */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*    Author: R.Horn                                      Date: 3.6.91       */
/*datoff *********************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <locale.h>
#include <ros.h>
#include <dce/dcesvcmsg.h>
#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <d23ipc.h>
#include <d25stub.h>

/*
 * Prototyping
 */
int main (int argc, char *argv[]);

/* ******************* DECLARATION OF GLOBAL DATA ************************** */

extern char	*optarg ;	/* reference to to actual option argument */
extern int	optind ;	/* index of the next option argument to be */
				/* processed */
extern int	opterr ;	/* 'print option error message'-flag */

extern signed16	d25_mxtpbuf ;	/* max. # of TP-routines send/receive buffer */
extern D25_tpinfo *tpinfref ;	/* reference to the TP-routines send/receive */
				/* buffer description table */

/* ******************* DECLARATION OF LOCAL DATA *************************** */

/* ------------------ Stub information table ------------------------------- */

static D25_stubinfo	stubinfo ;

/* ------------------ application context (AC) description table ----------- */

static R_apcary apcary[] = {
/* DAP-AC */
	{ (R_int) 0, (R_unsign) 4, R_SFUDX, R_PKERN, R_ACSE | R_ROSE,
	  (R_int) 0, (R_unsign) 2 }
#ifndef CSTUB
/* DSP-AC */
      , { (R_int) 4, (R_unsign) 4, R_SFUDX, R_PKERN, R_ACSE | R_ROSE,
	  (R_int) 2, (R_unsign) 2 }
#endif
 	} ;

/* ------------------ abstract syntax-ID table ----------------------------- */

static R_int aidsary[] =
	{ D27_ACSE_AS, D27_DAP_AS
#ifndef CSTUB
	, D27_ACSE_AS, D27_DSP_AS
#endif
	} ;

/* ------------------ abstract syntax (AS) description table --------------- */

static R_absary absary[] = {
/* ACSE-AS */
	{ (R_int)  8, (R_unsign) 5, (R_int) D27_ACSE_AS, (R_unsign) 10,
						      (R_int) 0, (R_unsign) 1 },
/* DAP-AS */
	{ (R_int) 13, (R_unsign) 4, (R_int) D27_DAP_AS, (R_unsign) 21,
#ifdef DIRV30
#ifndef CSTUB
						      (R_int) 1, (R_unsign) 4 }
#else
						      (R_int) 1, (R_unsign) 3 }
#endif /* !CSTUB */
#else
						      (R_int) 1, (R_unsign) 2 }
#endif /* DIRV30 */
#ifndef CSTUB
/* DSP-AS */
      , { (R_int) 17, (R_unsign) 4, (R_int) D27_DSP_AS, (R_unsign) 21 ,
#ifdef DIRV30
						   (R_int) 1, (R_unsign) 3 }
#else
						   (R_int) 1, (R_unsign) 2 }
#endif /* DIRV30 */
#endif /* !CSTUB */
	} ;

/* ------------------ transfer syntax-ID table ----------------------------- */

static R_int tidsary[] =
	{ D27_ASN1_TS
#ifndef DIRV30
	, D27_ASN1_ADM_TS, D27_ASN1_TS
#else
	, D27_PRIV_TS, D27_ASN1_ADM_TS, D27_ASN1_TS
#ifndef CSTUB
	, D27_PRIV_MIG_TS
#endif /* !CSTUB */
#endif /* DIRV30 */
	} ;

/* ------------------ transfer syntax (TS) description table --------------- */

static R_trsary trsary[] = {
/* ACSE-, DAP-, DSP-TS (basic encoding rules -> ASN1) */
	{ (R_int) 21, (R_unsign) 3, (R_int) D27_ASN1_TS },
/* DAP-, DSP-TS (ASN1-administration encoding rules) */
       	{ (R_int) 24, (R_unsign) 9, (R_int) D27_ASN1_ADM_TS }
#ifdef DIRV30
/* DAP-, DSP-TS (private encoding rules) */
      ,	{ (R_int) 33, (R_unsign) 9, (R_int) D27_PRIV_TS }
#ifndef CSTUB
      ,	{ (R_int) 42, (R_unsign) 8, (R_int) D27_PRIV_MIG_TS }
#endif /* !CSTUB */
#endif /* DIRV30 */
	} ;

/* ------------------ object identifier description table ------------------ */

static R_unsign objary[] = {
	 2, 5, 3, 1,		/* DAP-AC obj-id */
	 2, 5, 3, 2,		/* DSP-AC obj-id */
	 2, 2, 1, 0, 1,		/* ACSE-AS obj-id */
	 2, 5, 9, 1,		/* DAP-AS obj-id */
	 2, 5, 9, 2,		/* DSP-AS obj-id */
	 2, 1, 1,		/* ACSE-, DAP-, DSP-TS obj-id (ASN1) */
         1, 3, 12, 2, 1007, 1,	/* DAP-, DSP-TS obj-id (ASN1-administration) */
	 3, 200, 1 
#ifdef DIRV30
#ifdef BIGENDIAN 
       , 1, 3, 12, 2, 1007, 1,	/* DAP-, DSP-TS obj-id (private encoding) */
	 3, 200, 3 		/*                  (big endian machines) */
#else
       , 1, 3, 12, 2, 1007, 1,	/* DAP-, DSP-TS obj-id (private encoding) */
	 3, 200, 2		/*               (little endian machines) */
#endif /* BIGENDIAN */
#ifndef CSTUB
       , 1, 3, 12, 2, 1007, 1,	/* DAP-obj-id (private encoding (migration)) */
	 9, 1
#endif /* !CSTUB */
#endif /* DIRV30 */
 } ;

/* ------------------ state/event table ------------------------------------ */

#ifdef CSTUB
#include "d24stev.c"
#else
#include "d25stev.c"
#endif

/* ************************ M  A  I  N ************************************* */

main (int argc, char *argv[]) {

    int			opt ;
    signed32		result, i, state ;
    signed16		dirid = 1 ;
    signed16		mxrosass = D25_DFROSASSOC ;
    signed16		mxipcass = D25_DFIPCASSOC ;
    boolean		foreground = FALSE ;
    error_status_t	svc_result ;
    struct sigaction	siginfo ;
    D25_steventry	*stevptr ;
    D25_ipcentry	*ipcptr, bipc ;
    D25_rosentry	*rosptr ;
    D25_tpinfo		*tpiptr ;
    D25_eventpb		bevtpb ;
    char		daemon_dir[D27_LFILE_NAME] ;
    char		dir_path[D27_LFILE_NAME] ;

    setlocale(LC_ALL, "") ;

    /* evaluate calling parameter */
    opterr = 0 ;
    while ((opt = getopt (argc, argv, "d:fI:P:R:Tv:w:")) != -1) {
	switch (opt) {
	case 'd':
	    /* directory-ID */
	    if ((dirid = atoi (optarg)) < 1 || dirid > D2MAXDIR_ID) {
		exit (D27_INV_DIR_ID) ;
	    }
	    break ;
	case 'f':
	    /* run Stub process in the foreground */
	    foreground = TRUE ;
	    break ;
	case 'I':
	    /* max. # of IPC-associations */
	    if ((mxipcass = atoi (optarg)) < 1 || mxipcass > D23_MXCLTPROC) {
		exit (D27_INV_MXIASSOC) ;
	    }
	    break ;
	case 'R':
	    /* max. # of ROS-associations */
	    if ((mxrosass = atoi (optarg)) < 1 || mxrosass > D25_MXROSASSOC) {
		exit (D27_INV_MXRASSOC) ;
	    }
	    break ;
#ifdef DIRV30
	case 'T':
	    /* enable ASN.1-ADM transfer syntax test mode */
	    {
		R_int tmp = tidsary[1] ;
		tidsary[1] = tidsary[2] ;
		tidsary[2] = tmp ;
	    }
	    break ;
#endif /* DIRV30 */
	case 'P':
	case 'v':
	case 'w':
	    /* serviceability switches (ignored by stub code) */
	    break ;
	default:
	    exit (D27_INV_PAR) ;
	    break ;
	}
    }

    /* change actual directory */
#ifdef CSTUB
    sprintf (daemon_dir, "%s%s%s", dcelocal_path, D27_CLIENT_DIR, D27_CSTUB_DIR) ;
#else
    sprintf (daemon_dir, "%s%s%s%s", dcelocal_path, D27_SERVER_DIR, D27_ADM_DIR,
								D27_SSTUB_DIR) ;
#endif
    if (chdir (daemon_dir) == -1) {
	exit (D27_CHDIR_ERR) ;
    }

    if (!foreground) {
	/* fork daemon process */
	if ((result = fork()) < 0) {
	    exit (D27_FORK_ERR) ;
	}
	if (result != 0)
	    exit (0) ;

	/* set new process group */
	setpgrp () ;
    }

    /* close unused files */
    d27_011_close_unused_files (TRUE) ;

    /* initialize Stub information table with general values */
#ifdef CSTUB
    sprintf (dir_path, "%s%s", dcelocal_path, D27_CLIENT_DIR) ;
#else
    sprintf (dir_path, "%s%s%s", dcelocal_path, D27_SERVER_DIR, D27_ADM_DIR) ;
#endif
    stubinfo.d25_dirid = dirid ;
    stubinfo.d25_pid = getpid() ;
    stubinfo.d25_dirpathref = dir_path ;
    stubinfo.d25_imnref = (byte *) NULL ;
    stubinfo.d25_imxref = (byte *) NULL ;
    stubinfo.d25_nstentries = sizeof (stevtab)/sizeof (D25_steventry) ;
    stubinfo.d25_stevref = stevtab ;
    stubinfo.d25_nacentries = sizeof (apcary)/sizeof (R_apcary) ;
    stubinfo.d25_actabref = apcary ;
    stubinfo.d25_asitabref = aidsary ;
    stubinfo.d25_nasentries = sizeof (absary)/sizeof (R_absary) ;
    stubinfo.d25_astabref = absary ;
    stubinfo.d25_tsitabref = tidsary ;
    stubinfo.d25_ntsentries = sizeof (trsary)/sizeof (R_trsary) ;
    stubinfo.d25_tstabref = trsary ;
    stubinfo.d25_oitabref = objary ;


    /* initialize state specific state/event-table references */
    for (state = 1; state <= D25_MXSSTATE; state++) {
	for (i = 0, stevptr = stubinfo.d25_stevref; i < stubinfo.d25_nstentries;
								 i++, stevptr++)
	    if (D25_GST(stevptr->d25_stevinfo) == state)
		break ;
	stubinfo.d25_stsubref[state-1] = (i < stubinfo.d25_nstentries) ?
					   stevptr : (D25_steventry *) NULL ;
    }

    /* initialize serviceability */
    d27_001_svc_init (argc, argv, &svc_result) ;
    if (svc_result != svc_s_ok) {
	fprintf (stderr, "%s: Unable to initialize serviceability, status: 0x%lx\n",
							  argv[0], svc_result) ;
	exit (1) ;
    }
    /* initialize transport interface trace facilities */
    d25_tstrace (D23_ENALOG) ;

    /* initialize ROS-association table */
    stubinfo.d25_rmaxentries = mxrosass ;
    stubinfo.d25_ractentries = 0 ;
    if ((stubinfo.d25_rtabref = (D25_rosentry *) calloc (mxrosass + 1,
			  sizeof (D25_rosentry))) == (D25_rosentry *) NULL) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
				svc_c_sev_fatal, GDS_S_NOMEMORY,
				(mxrosass + 1) * sizeof (D25_ipcentry)) ;
	exit (1) ;
    }
    for (i = 0, rosptr = stubinfo.d25_rtabref; i < mxrosass; i++, rosptr++)
	rosptr->d25_rstate = D25_SRES(D25_RFREE) ;

    /* initialize IPC-association table */
    stubinfo.d25_imaxentries = mxipcass ;
    stubinfo.d25_iactentries = 0 ;
    if ((stubinfo.d25_itabref = (D25_ipcentry *) calloc (mxipcass + 1,
			  sizeof (D25_ipcentry))) == (D25_ipcentry *) NULL) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
				svc_c_sev_fatal, GDS_S_NOMEMORY,
				(mxipcass + 1) * sizeof (D25_ipcentry)) ;
	exit (1) ;
    }
    for (i = 0, ipcptr = stubinfo.d25_itabref; i < mxipcass; i++, ipcptr++)
	ipcptr->d25_istate = D25_SIOS(D25_IRIDLE) ;

    /* initialize TP-routines send/receive buffer description table */
    d25_mxtpbuf = mxrosass ;
    if ((tpinfref = (D25_tpinfo *) calloc (d25_mxtpbuf, sizeof (D25_tpinfo))) ==
						       (D25_tpinfo *) NULL) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
					svc_c_sev_fatal, GDS_S_NOMEMORY,
					d25_mxtpbuf * sizeof (D25_tpinfo)) ;
	exit (1) ;
    }
    for (i = 0, tpiptr = tpinfref; i < d25_mxtpbuf; i++, tpiptr++)
	tpiptr->d25_tpstate = D25_UNUSED ;

    /* activate signal handling (disributed command handling) */
    siginfo.sa_handler = d25_signal ;
    siginfo.sa_flags = 0 ;
    sigemptyset (&siginfo.sa_mask) ;
    if (SIGACTION (SIGUSR1, &siginfo, (struct sigaction *) NULL) == -1) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
			    svc_c_sev_fatal, GDS_S_SIGHDL_ERR, SIGUSR1, errno) ;
	exit (1) ;
    }

    /* initialize basic event parameter block */
    bevtpb.d25_esinforef = &stubinfo ;
    bevtpb.d25_eipcref = &bipc ;
    bevtpb.d25_erosref = (D25_rosentry *) NULL ;
    bevtpb.d25_estate = D25_IRINIT ;
    bevtpb.d25_eevent = 0 ;
    bipc.d25_iassid = D25_UNUSED ;

    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
			svc_c_sev_notice, GDS_S_ENTRYSTUB, stubinfo.d25_dirid) ;

    /* execute additional initialization actions */
    if (d25_exec_actions (&bevtpb) == D25_ERROR) {
	exit (1) ;
    }

    /* Stub is ready to execute incoming IPC- resp. ROS-events */
    /* Note: this function should normally never come back */
    if (d25_evmgt (&stubinfo) == D25_ERROR) {
	exit (1) ;
    }
    exit (0) ;
}
