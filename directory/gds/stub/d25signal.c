/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d25signal.c,v $
 * Revision 1.1.12.2  1996/02/18  18:23:25  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:46:22  marty]
 *
 * Revision 1.1.12.1  1995/12/08  16:15:31  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:20:59  root]
 * 
 * Revision 1.1.10.3  1994/09/06  12:26:32  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:24:58  keutel]
 * 
 * Revision 1.1.10.2  1994/05/10  16:03:31  marrek
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/05  07:29:30  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  13:12:50  marrek]
 * 
 * Revision 1.1.10.1  1994/02/22  17:55:27  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:52:24  marrek]
 * 
 * Revision 1.1.8.1  1993/10/14  19:19:04  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  19:16:11  keutel]
 * 
 * 	October 1993 code drop
 * 	[1993/10/09  17:58:09  keutel]
 * 
 * Revision 1.1.6.2  1993/08/11  08:30:22  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  12:20:17  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  22:04:24  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:48:11  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:10:00  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:33:31  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d25signal.c,v $ $Revision: 1.1.12.2 $ $Date: 1996/02/18 18:23:25 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*exon ***********************************************************************/
/*                                                                           */
/*   TYPE:    function                                                       */
/*                                                                           */
/*   NAME:    d25_signal ()                                                  */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The function (signal routine) handles all distributed commands      */
/*       which are received by the Stub.                                     */
/*                                                                           */
/*   INPUT-PARAMETER:                                                        */
/*       signo       = signal no.                                            */
/*                                                                           */
/*   OUTPUT-PARAMETER:                                                       */
/*                     none                                                  */
/*                                                                           */
/*   RETURN-VALUE:                                                           */
/*                     none                                                  */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                        DATE: 3.6.91      */
/*exoff **********************************************************************/

#include <gds.h>
#include <stdio.h>
#include <signal.h>
#include <ros.h>
#include <stdlib.h>
#include <cmx.h>
#include <dce/dcesvcmsg.h>
#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <d23ipc.h>
#include <d25stub.h>


/* ------------------------------------------------------------------------- */

void d25_signal (int signo) {

    boolean		confirm = TRUE ;
    D23_dreadpb 	drdpb ;
    D23_dconfpb 	dcfpb ;
    error_status_t	result ;

    /* get distributed command */
    drdpb.d23_Khdinfo.d2_version = D23_V02 ;
    if (d23_read_dist_cmd (&drdpb) == D23_ERROR) {
	return ;
    }

    /* evaluate distributed command */
    switch (drdpb.d23_Kcmd) {
    case D23_DISLOG:
	/* disable DIR-X logging */
	d27_003_reset_routing ((char *) drdpb.d23_Kparref, &result) ;

	/* disable transport interface logging */
	d25_tstrace (D23_DISLOG) ;
	break;
    case D23_ENALOG:
	/* enable DIR-X logging */
	d27_003_reset_routing ((char *) drdpb.d23_Kparref, &result) ;

	/* enable transport interface logging */
	d25_tstrace (D23_ENALOG) ;
	break;
    case D23_SVCLOGLEVEL:
	/* change logging level */
	dce_svc_debug_set_levels (drdpb.d23_Kparref, &result) ;
	if (result != svc_s_ok) {
	    fprintf (stderr,
	     "C/S-Stub: can't change logging level (status = %#x)\n", result) ;
	}
	break ;
    default:
	/* unknown distributed command */
	DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
			    svc_c_debug1, GDS_S_ILL_DIST_CMD, drdpb.d23_Kcmd)) ;
	confirm = FALSE ;
	break ;
    }

    if (confirm ) {
	/* confirm received distributed command */
	dcfpb.d23_Lhdinfo.d2_version = D23_V02 ;
	dcfpb.d23_Lkey = drdpb.d23_Kkey ;
	if (d23_conf_dist_cmd (&dcfpb) == D23_ERROR) {
	    return ;
	}
    }
}

void d25_tstrace (signed32 mode) {

    struct t_opts1	tstrc ;
    char		*trcpar ;

    /* enable/disable transport interface logging */
    if ((trcpar = getenv (TSTRCPAR)) != (char *) NULL) {
	tstrc.t_optnr = T_OPTS1 ;
	tstrc.t_optname = T_DEBUG ;
	tstrc.t_optvalue = (mode == D23_ENALOG) ? trcpar : "" ;
	r_setopt (T_LIB, (t_opts *) &tstrc) ;
    }
}
