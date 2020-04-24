/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20distcmd.c,v $
 * Revision 1.1.10.2  1996/02/18  19:42:23  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:23:09  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:21:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:53:00  root]
 * 
 * Revision 1.1.8.5  1994/09/19  08:39:20  marrek
 * 	Fixes for OT12176 (wrong incore tables.)
 * 	[1994/09/19  08:32:04  marrek]
 * 
 * Revision 1.1.8.4  1994/07/08  18:03:29  keutel
 * 	OT 11156: call new function dce_svc_init
 * 	[1994/07/08  18:03:05  keutel]
 * 
 * Revision 1.1.8.3  1994/05/10  15:50:20  marrek
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/05  09:21:58  marrek]
 * 
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/02  13:23:46  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:14:42  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:07:04  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:14:07  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:35:56  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:38:37  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  11:04:41  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:42:07  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  18:11:09  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:07:52  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/17  23:12:17  tom
 * 	Bug 6411 - Add setlocale call for i18n.
 * 	[1992/12/17  23:07:35  tom]
 * 
 * Revision 1.1.4.2  1992/11/27  15:40:54  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  16:46:56  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:02:06  melman
 * 	New GDS merged drop
 * 	[1992/05/28  20:46:01  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20distcmd.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:42:23 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton **********************************************************************/
/*                                                                           */
/*   TYPE:    program (utility)                                              */
/*                                                                           */
/*   NAME:    d20distcmd                                                     */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The program handles a given distributed command to a class of       */
/*       directory system processes by calling the proper ipc-function.      */
/*                                                                           */
/*   CALLING SYNTAX:                                                         */
/*                                                                           */
/*       d20dircmd -c<command> -s<server-id1[/server-id2/.../server-idN]>    */
/*                 [-t<timeout>] [-p<parameter>]                             */
/*                                                                           */
/*                                                                           */
/*        Exit-Values: 0 = Operation performed successfully.                 */
/*                     1 = Calling parameter error.                          */
/*                     2 = Can't perform operation.                          */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                        DATE: 11.7.89     */
/*datoff *********************************************************************/

#include <gds.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#include <dce/dcesvcmsg.h>
#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <d23ipc.h>
#include <d20proto.h>


/* ---------------- DECLARATION OF CONSTANTS ------------------------------- */

#define D20_DCMD	'c'		/* "distributed command value"-switch */
#define D20_DSRVID	's'		/* "server-id value"-switch */
#define D20_DTOUT	't'		/* "timeout value"-switch */
#define D20_DPAR	'p'		/* "parameter value"-switch */
#define D20_MAXIDNO	50		/* max. # of given server-ID's */
#define D20_SPAR	2		/* # of scanf arguments */
#define D20_DERR1 	"%s: wrong calling syntax\nUsage: %s -c<command> -s<server-id1[/server-idN]> [-t<timeout>] [-p<parameter>]\n"
#define D20_DERR2	"%s: unknown switch\nUsage: %s -c<command> -s<server-id1[/server-idN]> [-t<timeout>] [-p<parameter>]\n"
#define D20_DERR3	"%s: error on distributed command operation\n"
#define D20_DERR4	"%s: unable to attach to IPC (%d/%d/%d)\n"

/* ------------------------------------------------------------------------- */

 int main (int argc, char *argv[]) {

    char		*p, *pname ;
    boolean 		wrong_srvid = TRUE ;
    D23_atclpb		atclpb ;
    D23_dwritepb	dwrpb ;
    signed32		i, j, srvid[D20_MAXIDNO] ;
    error_status_t	svc_result ;
    unsigned32 		flags;

    setlocale(LC_ALL, "");

/* call dce_svc_init - it will do SVC intialization 
   (esp. rpc_sm_enable_allocate) */
/* flags are not yet used in dce_svc_init - so we set them to 0 */

    flags = 0;
    dce_svc_init(flags,&svc_result);
    if (svc_result != svc_s_ok) {
	fprintf (stderr, "%s: Unable to initialize serviceability, status: 0x%lx\n",    
							  argv[0], svc_result) ;
	exit (1) ;
    }

    /* register the message table */
    dce_msg_define_msg_table(gds_msg_table,
			     sizeof(gds_msg_table)/sizeof(gds_msg_table[0]),
			     &svc_result);

    if (svc_result != msg_s_ok) {
	fprintf (stderr, "%s: Unable to initialize message table, status: 0x%lx\n",
							  argv[0], svc_result) ;
	exit (1) ;
    }

    /* register at serviceability */
    gds_svc_handle = dce_svc_register(gds_svc_table,(idl_char *)"gds",
								&svc_result) ;
    if (svc_result != svc_s_ok) {
	fprintf (stderr, "%s: Unable to register at serviceability, status: 0x%lx\n",    
							  argv[0], svc_result) ;
	exit (1) ;
    }

    /* initialize some default values */
    memset (srvid, 0, sizeof (srvid)) ;
    dwrpb.d23_Jhdinfo.d2_version = D23_V02 ;
    dwrpb.d23_Jcmd = 0 ;
    dwrpb.d23_Jtmout = D23_DMAXTOUT ;
    dwrpb.d23_Jparsize = 0 ;
    dwrpb.d23_Jparref = (byte *) NULL ;

    /* evaluate calling parameter */
    if ((pname = strrchr (*argv, (int) '/')) == NULL) {
	pname = *argv ;
    } else {
	pname++ ;
    }

    if (argc < 3) {
	fprintf (stderr, D20_DERR1, pname, pname) ;
	exit (1) ;
    }
    for (argv++, i = 1; i < argc; i++, argv++) {
	if (*(*argv)++ != '-') {
	    fprintf (stderr, D20_DERR1, pname, pname) ;
	    exit (1) ;
	}
	switch (*(*argv)++) {
	case D20_DCMD:
	    /* command code */
	    dwrpb.d23_Jcmd = atoi (*argv) ;
	    break ;
	case D20_DPAR:
	    /* command parameter */
	    dwrpb.d23_Jparsize = strlen (*argv) + 1 ;
	    dwrpb.d23_Jparref = (byte *) *argv ;
	    break ;
	case D20_DSRVID:
	    /* server-ID(s) */
	    for (p = *argv, j = 0; j < D20_MAXIDNO; j++) {
		if (sscanf (p, "%d%*c%s", &srvid[j], p) < D20_SPAR)
		    break ;
	    }
	    break ;
	case D20_DTOUT:
	    /* command timeout value */
	    dwrpb.d23_Jtmout = atoi (*argv) ;
	    break ;
	default:
	    fprintf (stderr, D20_DERR2, pname, pname) ;
	    exit (1) ;
	    break ;
	}
    }

    /* attach to IPC as a client and get IPC-resources */
    /* supply IPC-attach client parameter block */
    atclpb.d23_9hdinfo.d2_version = D23_V02 ;
    atclpb.d23_9evmode = D23_SEVM(D23_SYNC_EM) ;

    if (d23_attclt (&atclpb) == D23_ERROR) {
	fprintf (stderr, D20_DERR4, pname,
				atclpb.d23_9hdinfo.d2_retcode,
				atclpb.d23_9hdinfo.d2_errclass,
				atclpb.d23_9hdinfo.d2_errvalue) ;
	exit (1) ;
    }

    /* perform distributed command operation */
    for (i = 0; i < D20_MAXIDNO; i++) {
	if (srvid[i] > 0) {
	    fprintf (stdout, "%s: sending distributed command (%d) to server-id (%d)\n",
					pname, dwrpb.d23_Jcmd, srvid[i]) ;
	    dwrpb.d23_Jsv_prid = srvid[i] ;
	    if (d23_write_dist_cmd (&dwrpb) == D2_ERROR) {
		fprintf (stderr, D20_DERR3, pname) ;
		exit (2) ;
	    }
	    wrong_srvid = FALSE ;
	}
    }

    exit ((wrong_srvid) ? 1 : 0) ;
}
