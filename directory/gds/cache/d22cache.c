/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d22cache.c,v $
 * Revision 1.1.10.2  1996/02/18  19:43:27  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:24:23  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:26:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:54:12  root]
 * 
 * Revision 1.1.8.5  1994/09/06  12:24:39  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:12:30  keutel]
 * 
 * Revision 1.1.8.4  1994/07/06  15:06:46  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:12:26  marrek]
 * 
 * Revision 1.1.8.3  1994/05/10  15:50:53  marrek
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/05  06:42:25  marrek]
 * 
 * 	Bug fixes.
 * 	[1994/04/29  09:28:51  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:16:23  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:08:49  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:18:15  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:52:21  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:51:02  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  07:18:25  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  12:11:14  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  18:39:50  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:12:36  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/17  23:12:36  tom
 * 	Bug 6411 - Add setlocale call for i18n.
 * 	[1992/12/17  23:07:57  tom]
 * 
 * Revision 1.1.4.2  1992/11/27  17:06:23  marrek
 * 	November 1992 code drop
 * 	[1992/11/17  16:26:59  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:12:57  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:05:26  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d22cache.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:43:27 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton	***************************************************************/
/*								      */
/* TYPE         : MODUL                                               */
/*								      */
/* NAME         : d22cache.c                                          */
/*								      */
/* AUTHOR       : R. Horn, D AP 11                                    */
/* DATE         : 10.05.88                                            */
/*								      */
/* COMPONENT    : DS                                                  */
/*								      */
/* DOC.-NR.     : Directory-Design-Specification                      */
/*			.					      */
/*			.					      */
/*								      */
/* PRD#/VERS.   :                                                     */
/*								      */
/* DESCRIPTION  :                                                     */
/*								      */
/* SYSTEM DEPENDENCIES: COMMON                                        */
/*								      */
/* HISTORY      :                                                     */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/*exon ***********************************************************************/
/*                                                                           */
/*  FUNCTION:  main ()                                                       */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*      This is the main routine of the DUA-cachehandler process. The        */
/*      process can be called by special parameters for the configuration    */
/*      of a user specific DUA-Cache (note: ALL parameters are optional).    */
/*      If a configuration parameter doesn't appear as input parameter,      */
/*      then the corresponding internally defined default configuration      */
/*      parameter is valid.                                                  */
/*                                                                           */
/*  CALLING-PARAMETER:                                                       */
/*                                                                           */
/*      -h<nnn>     Max. no. of hash table entries.                          */
/*      -rm<nnn>    Max. no. of cacheable RESIDENT directory objects.        */
/*      -rd<nnn>    Max. no. of removable RESIDENT directory objects on      */
/*                  reaching the limit (see parameter -rm<nnn>).             */
/*      -pm<nnn>    Max. no. of cacheable PRIVILEGED directory objects.      */
/*      -pd<nnn>    Max. no. of removable PRIVILEGED directory objects on    */
/*                  reaching the limit (see parameter -pm<nnn>).             */
/*      -nm<nnn>    Max. no. of cacheable NORMAL directory objects.          */
/*      -nd<nnn>    Max. no. of removable NORMAL directory objects on        */
/*                  reaching the limit (see parameter -nm<nnn>).             */
/*      -i<nnn>     Max. no. of 1st level cache directory objects.           */
/*      -m<nnn>     Max. size of all free memory areas (in byte).            */
/*      -a<nnn>     Max. no. of free memory areas.                           */
/*      -b</../.>   Path name of the DUA-cache directory.                    */
/*      -f          Process test mode                                        */
/*      -P, -v, -w  Serviceability-switches.                                 */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: R.Horn                                        DATE: 17.11.87     */
/*exoff **********************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <locale.h>
#include <dce/dcesvcmsg.h>
#include <dce/d2dir.h>
#include <d23ipc.h>
#include <d23apdu.h>
#include <d22cache.h>

/*
 * Prototyping
 */
int main (int argc, char *argv[]);


/* -------------------- declaration of global data ------------------------- */

			/* error variable */
signed32	d22_errno ;

boolean d22_flgterm = FALSE ;
			/* process termination flag */
			/* set to TRUE, if the process receives a signal */
			/* SIGTERM */

boolean d22_inwork = FALSE ;
			/* process processing flag */
			/* set to TRUE, if a service request is in work */

boolean d22_dboperation = TRUE ;
			/* DUA-cache operation flag */
			/* set to FALSE, if access to the cache data base */
			/* information is impossible */

boolean d22_foreground = FALSE ;
			/* process test mode flag */
			/* set to TRUE, if the cache process shall not */
			/* run as a daemon */

signed16 d22_ipcmsgsize ;/* max. size of a IPC-message buffer */

			/* DUA-cache administration information table */
			/* this table must be global because it's referenced */
			/* by the 'distributed command'-signal routine */
D22_admin_tab	d22_admtab = { D22_NONE } ;

/* -------------------- declaration of local data -------------------------- */

			/* DUA-cache configuration information table */
static D22_config_tab	d22_conftab =
		{ D22_CMAX_ENTRIES, D22_IMAX_ENTRIES, D22_RMAX_ENTRIES,
		  D22_PMAX_ENTRIES, D22_NMAX_ENTRIES, D22_RDEL_ENTRIES,
		  D22_PDEL_ENTRIES, D22_NDEL_ENTRIES, D22_FMAX_MEMSIZE,
		  D22_AMAX_AREAS, 0, "" } ;


/* --------------------------- M A I N ------------------------------------- */

main (int argc, char *argv[]) {

#if !defined NOT_CHECK_KEY
    int 		x_l, x_sv ;
#endif
    signed32		result ;
    D23_atsvpb		att_par ;
    D23_wtinpb		wait_par ;
    D23_rsrqpb		res_par ;
    struct sigaction	siginfo ;
    char		tmpbuf[D27_LINST_NAME], daemon_dir[D27_LINST_NAME] ;
    error_status_t      svc_result ;

    setlocale(LC_ALL, "") ;

#if !defined NOT_CHECK_KEY
    /* --- check key disk --- */
    if (((x_l=_check_key("DIR-X-CL",DIR_VERSION))!=0) 
			    && ((x_sv=_check_key("DIR-X-SV",DIR_VERSION))!=0)) {
	exit (D27_CHK_KEY_ERR + (x_l > 0) ? x_l : x_sv) ;
    }
#endif

    /* --- evaluate calling parameter --- */
    d22_admtab.d22_conftab = &d22_conftab ;
    if ((result = d22_eval_param (argc, argv, &d22_admtab)) != D22_OK) {
	exit (result) ;
    }

    /* --- change actual directory --- */
    sprintf (daemon_dir, "%s%s%s", dcelocal_path, D27_CLIENT_DIR,
								D27_CACHE_DIR) ;
    if (chdir (daemon_dir) == -1) {
	exit (D27_CHDIR_ERR) ;
    }

    if (!d22_foreground) {
	/* --- create DUA-cachehandler process as child --- */
	if ((result = fork ()) < 0) {
	    exit (D27_FORK_ERR) ;
	}

	/* --- terminate initiator process --- */
	if (result > 0)
	    exit (0) ;

	/* --- close all unused files (inherited from fork ()) --- */
	d27_011_close_unused_files (TRUE) ;
    }

    /* initialize serviceability */
    d27_001_svc_init (argc, argv, &svc_result) ;
    if (svc_result != svc_s_ok) {
	fprintf (stderr, "%s: Unable to initialize serviceability, status: %#x\n",    
							  argv[0], svc_result) ;
	exit (1) ;
    }

    /* --- establish signal routines --- */
    siginfo.sa_handler = d22_terminate ;
    siginfo.sa_flags = 0 ;
    sigemptyset (&siginfo.sa_mask) ;
    if (SIGACTION (SIGTERM, &siginfo, (struct sigaction *) NULL) == -1) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
			    svc_c_sev_fatal, GDS_S_SIGHDL_ERR, SIGTERM, errno) ;
	exit (1) ;
    }

    siginfo.sa_handler = d22_hdldistcmd ;
    siginfo.sa_flags = 0 ;
    sigemptyset (&siginfo.sa_mask) ;
    if (SIGACTION (SIGUSR1, &siginfo, (struct sigaction *) NULL) == -1) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
			    svc_c_sev_fatal, GDS_S_SIGHDL_ERR, SIGUSR1, errno) ;
	exit (1) ;
    }

    /* --- initialize object identifier conversion table --- */
    sprintf (tmpbuf, "%s%s%s", dcelocal_path, D27_CLIENT_DIR, D27_CONF_DIR) ;
    if (d27_307_init_att (tmpbuf) == D2_ERROR) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_GENERAL,
					  svc_c_sev_fatal, GDS_S_ATTRFILE_ERR) ;
	exit (1) ;
    }

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d%d%d%d%d%d%d"), GDS_S_GENERAL,
		svc_c_debug1, GDS_S_CACHE_CONF1, d22_conftab.d22_hsh_max,
		d22_conftab.d22_res_max, d22_conftab.d22_rdel_no,
		d22_conftab.d22_priv_max, d22_conftab.d22_pdel_no,
		d22_conftab.d22_norm_max, d22_conftab.d22_ndel_no,
		d22_conftab.d22_int_max)) ;
    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d%s"),
	 	GDS_S_GENERAL, svc_c_debug1, GDS_S_CACHE_CONF2,
		d22_conftab.d22_mem_no, d22_conftab.d22_mem_max,
		d22_conftab.d22_base_path)) ;

    /* --- set file access mask -> no restrictions --- */
    umask ((mode_t) D22_UMASK) ;

    /* --- set new process group --- */
    setpgrp () ;

    /* --- attach DUA-cache server process at IPC level --- */
    att_par.d23_0hdinfo.d2_version = D23_V02 ;
    att_par.d23_0svrid = D23_DCASID ;
    att_par.d23_0evmode = D23_SEVM(D23_SYNC_EM) ;
    if (d23_attsvr (&att_par) == D23_ERROR) {
	exit (1) ;
    }

    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_GENERAL,
					   svc_c_sev_notice, GDS_S_ENTRYCACHE) ;

    /* initialize max. size of a IPC-message buffer */
    d22_ipcmsgsize = att_par.d23_0mslen ;

    /* --- enter service loop --- */
    for (;;) {
	/* generate parameter block for IPC-call 'wait' */
	wait_par.d23_2hdinfo.d2_version = D23_V02 ;
	wait_par.d23_2wtime = D23_INFIN ;

	/* --- wait for service request --- */
	d22_inwork = FALSE ;

	if (d22_flgterm) {
	    /* process termination requested */
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_GENERAL,
					    svc_c_sev_notice, GDS_S_EXITCACHE) ;
	    exit (0) ;
	}

#if !defined THREADSAFE
	/* enable distributed command operation */
	siginfo.sa_handler = d22_hdldistcmd ;
	siginfo.sa_flags = 0 ;
	sigemptyset (&siginfo.sa_mask) ;
	if (SIGACTION (SIGUSR1, &siginfo, (struct sigaction *) NULL) == -1) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
			    svc_c_sev_fatal, GDS_S_SIGHDL_ERR, SIGUSR1, errno) ;
	    exit (1) ;
	}

	/* handle distributed command, if available */
	d22_hdldistcmd (SIGUSR1) ;
#endif

	if ((result = d23_waitin (&wait_par)) == D23_ERROR) {
	    exit (1) ;
	}
	if (wait_par.d23_2oprid == D23_NOOPR ||
					     wait_par.d23_2oprid == D23_ABORT) {
	    /* signal or abort request occurred */
#if defined THREADSAFE
	    if (wait_par.d23_2oprid == D23_NOOPR) {
		/* release current 'main' thread to allow signal handling */
		pthread_yield () ;
	    }
#endif
	    continue ;
	}

	d22_inwork = TRUE ;

#if !defined THREADSAFE
	/* disable distributed command operation */
	siginfo.sa_handler = SIG_IGN ;
	siginfo.sa_flags = 0 ;
	sigemptyset (&siginfo.sa_mask) ;
	if (SIGACTION (SIGUSR1, &siginfo, (struct sigaction *) NULL) == -1) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
			    svc_c_sev_fatal, GDS_S_SIGHDL_ERR, SIGUSR1, errno) ;
	    exit (1) ;
	}
#endif

	/* --- dispatch service request --- */
	if (d22_dboperation) {
	    switch ((int) wait_par.d23_2oprid) {
	    case D23_READ:
	    case D23_READ + D2_ADM_OPID:
		/* --- perform service request DS_READ --- */
		result = d22_cread (&d22_admtab, &wait_par, &res_par) ;
		break ;
	    case D23_LIST:
		/* --- perform service request DS_LIST --- */
		result = d22_clist (&d22_admtab, &wait_par, &res_par) ;
		break ;
	    case D23_SEARCH:
	    case D23_SEARCH + D2_ADM_OPID:
		/* --- perform service request DS_SEARCH --- */
		result = d22_csearch (&d22_admtab, &wait_par, &res_par) ;
		break ;
	    case D23_COMPARE:
		/* --- perform service request DS_COMPARE --- */
		result = d22_ccompare (&d22_admtab, &wait_par, &res_par) ;
		break ;
	    case D23_ADDOBJT:
	    case D23_ADDOBJT + D2_ADM_OPID:
		/* --- perform service request DS_ADD_OBJECT --- */
		result = d22_cadd (&d22_admtab, &wait_par, &res_par) ;
		break ;
	    case D23_REMOBJT:
	    case D23_REMOBJT + D2_ADM_OPID:
		/* --- perform service request DS_REMOVE_OBJECT --- */
		result = d22_crmove (&d22_admtab, &wait_par, &res_par) ;
		break ;
	    default:
		/* --- invalid service request --- */
		d22_errno = D22_REQUNKNOWN_ERR ;
		dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
		    		svc_c_sev_warning, GDS_S_CACHE_INVRQ_ERR,
				wait_par.d23_2oprid) ;
		result = D22_ERROR ;
		break ;
	    }
	    if (result == D22_SPEC_ERROR)
		/* special error occurred which is */
		/* handled by d22_c... ()-function itself */
		continue ;
	} else {
	    /* -- access to the cache information data base impossible -- */
	    d22_errno = D22_IBUSY ;
	    result = D22_ERROR ;
	}

	if (result == D22_ERROR) {
	    /* generate parameter block for IPC-call 'return error' */
	    d22_errorhdl (d22_errno, (char *) wait_par.d23_2rfidt) ;
	    res_par.d23_3hdinfo.d2_version = D23_V02 ;
	    res_par.d23_3assid = wait_par.d23_2assid ;
	    res_par.d23_3invinfo = wait_par.d23_2invinfo ;
	    res_par.d23_3rltid = D23_RERROR ;
	    res_par.d23_3rmdat = D23_LASTFG ;
	    res_par.d23_3rfrdt = wait_par.d23_2rfidt ;
	    res_par.d23_3lnrdt = 
	    (((D23_rserror *) res_par.d23_3rfrdt)->d23_Zretcod != D2_CALL_ERR ||
	    ((D23_rserror *) res_par.d23_3rfrdt)->d23_Zerrcls != D2_PAR_ERR ||
	    ((D23_rserror *) res_par.d23_3rfrdt)->d23_Zerrval != D2_NFOUND) ?
	    D23_S_RSERR + D23_S_EOM : D23_S_RSERR + D23_S_NM + D23_S_EOM ;
	} else {
	    /* complete parameter block for IPC-call 'return result' */
	    res_par.d23_3hdinfo.d2_version = D23_V02 ;
	    res_par.d23_3assid = wait_par.d23_2assid ;
	    res_par.d23_3invinfo = wait_par.d23_2invinfo ;
	    res_par.d23_3rltid = D23_RESULT ;
	}

	/* return result message or return error message */
	if (d23_result (&res_par) == D23_ERROR) {
	    if (res_par.d23_3hdinfo.d2_errvalue == D23_ABOERR)
		continue ;
	    exit (1) ;
	}
    }
}
