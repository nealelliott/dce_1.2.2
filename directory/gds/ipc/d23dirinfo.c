/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: d23dirinfo.c,v $
 * Revision 1.1.4.2  1996/02/18  18:19:29  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:42:16  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:03:01  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:14:57  root]
 * 
 * Revision 1.1.2.5  1994/09/06  12:25:17  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:14:48  keutel]
 * 
 * Revision 1.1.2.4  1994/07/06  15:07:37  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:16:40  marrek]
 * 
 * Revision 1.1.2.3  1994/06/09  18:46:30  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:19:35  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:56:21  marrek
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/05  07:00:16  marrek]
 * 
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/02  13:23:24  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:39:41  marrek]
 * 
 * Revision 1.1.2.1  1994/02/22  19:21:29  marrek
 * 	Change rcsid string format.
 * 	[1994/02/09  08:12:51  marrek]
 * 
 * 	Creation for dce1.1.
 * 	[1994/02/02  10:54:18  marrek]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d23dirinfo.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:19:29 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton **********************************************************************/
/*                                                                           */
/*   TYPE:    program (utility)                                              */
/*                                                                           */
/*   NAME:    d23dirinfo                                                     */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The program reads all informations from the shared memory area      */
/*       of a specific directory system installation which are correspon-    */
/*       ding to the running processes of this installation. The infor-      */
/*       mations are written to the standard output (one line per running    */
/*       process) in the following format:                                   */
/*                                                                           */
/*             <process-type> <process-id> <directory-id> <ipc-id> <state>   */
/*                                                                           */
/*                                                                           */
/*       <process-type> = DUA-Cache | C-Stub | S-Stub | DSA | Monitor |      */
/*                        Dir-User                                           */
/*                                                                           */
/*        Note: The directory-identifier is only relevant, if the process    */
/*              type is equal to DSA resp. S-Stub.                           */
/*                                                                           */
/*        Exit-Values: 0 = Operation performed successfully.                 */
/*                     1 = No running process found.                         */
/*                     2 = Can't perform operation.                          */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                        DATE: 11.7.89     */
/*datoff *********************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <dce/dcesvcmsg.h>
#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <d23ipc.h>


/* -------------------- DECLARATION OF CONSTANTS --------------------------- */

#define	D20_CAPROC	"DUA-Cache"	/* process type names */
#define D20_CSPROC	"C-Stub"
#define D20_SSPROC	"S-Stub"
#define D20_DSPROC	"DSA"
#define D20_MOPROC	"Monitor"
#define D20_DIPROC	"Dir-User"
#define D20_UKNPROC	"???"
#define D20_NOVALUE	"-"

/* -------------------- DECLARATION OF GLOBAL DATA ------------------------- */

extern D23_iprocinfo    d23_info ;      /* process specific IPC-information */

					/* variable is used to suppress */
					/* standard prolog in serviceability */
extern signed32		d23_svcprolog ;

/*
 * Prototyping
 */
int main (signed32 argc, char *argv[]);

/* ------------------------------------------------------------------------- */

main (signed32 argc, char *argv[]) {

    signed32		i, j, svrid, proc_no = 0 ;
    signed32		printed_pids[D23_MXREGINFO + D23_MXSRVPROC + D23_MXCLTPROC] ;
    signed32		printed_idx = 0 ;
    D23_ipcentry	*svrptr ;
    D23_ipcentry	*cltptr ;
    D23_reginfo		*regptr ;
    error_status_t	svc_result ;
    char		*pname, dirid[10], ipcid[10] ;

    setlocale(LC_ALL, "");

    /* initialize serviceability */
    d23_svcprolog = svc_c_action_brief ;
    d27_001_svc_init (argc, argv, &svc_result) ;
    if (svc_result != svc_s_ok) {
	fprintf (stderr, "%s: Unable to initialize serviceability, status: 0x%lx\n",    
							  argv[0], svc_result) ;
	exit (1) ;
    }

    /* evaluate program name */
    if ((pname = strrchr (argv[0], (int) '/')) == (char *) NULL)
	pname = argv[0] ;
    else
	pname++ ;

    /* get IPC-resources (shared memory) */
    if (d23_get_ipc () == D23_ERROR) {
	exit (2) ;
    }

    /* write header line */
    dce_fprintf (stdout, GDS_S_IPCINFO_HEAD) ;

    /* get process informations from the IPC-registration area */
    for (i = 0, regptr = d23_info.d23_regarea;
			    i < d23_info.d23_genarea->d23_mrno; i++, regptr++) {
	if ((svrid = regptr->d23_pregtype) != D23_UNUSED &&
					    kill (regptr->d23_procid, 0) == 0) {
	    for (j = 0; j < printed_idx; j++)
		if (printed_pids[j] == regptr->d23_procid)
		    /* process specific information already printed */
		    break ;
	    if (j < printed_idx)
	        continue ;
	    printed_pids[printed_idx++] = regptr->d23_procid ;

	    sprintf (dirid,"%d", regptr->d23_pdirid) ;
	    sprintf (ipcid,"%d", regptr->d23_pregtype) ;
	    dce_fprintf (stdout, GDS_S_IPCINFO_LFORMAT, 
			(svrid == D23_MONPROC) ? D20_MOPROC :
			(svrid == D23_DCASID) ? D20_CAPROC :
			(svrid == D23_CSTSID) ? D20_CSPROC :
			(svrid >  D23_SSTSID &&
			 svrid <= D23_SSTSID + D2MAXDIR_ID) ? D20_SSPROC :
			(svrid >  D23_DS0SID &&
			 svrid <= D23_MAXSVR) ? D20_DSPROC : D20_UKNPROC,
			regptr->d23_procid, (strcmp(dirid, "0") == 0) ?
			D20_NOVALUE : dirid, ipcid, regptr->d23_pstate) ;
			proc_no++ ;
	}
    }

    /* get process informations from the IPC-server area */
    for (i = 0, svrptr = d23_info.d23_svrarea;
			    i < d23_info.d23_genarea->d23_msno; i++, svrptr++) {
	if (D23_GES(svrptr->d23_estate) == D23_ATTACH &&
	    kill (svrptr->d23_epid.d23_rpid, 0) == 0 &&
	    (svrid = svrptr->d23_esid) >= D23_DCASID && svrid <= D23_MAXSVR) {
		
	    for (j = 0; j < printed_idx; j++)
		if (printed_pids[j] == svrptr->d23_epid.d23_rpid)
		    /* process specific information already printed */
		    break ;
	    if (j < printed_idx)
	        continue ;
	    printed_pids[printed_idx++] = svrptr->d23_epid.d23_rpid ;

	    sprintf (dirid,"%d",
		(svrid > D23_SSTSID && svrid <= D23_SSTSID + D2MAXDIR_ID) ?
						       svrid - D23_SSTSID :
		(svrid > D23_DS0SID && svrid <= D23_DS0SID + D2MAXDIR_ID) ?
						       svrid - D23_DS0SID : 0) ;
	    sprintf (ipcid,"%d", svrptr->d23_esid) ;
	    dce_fprintf (stdout, GDS_S_IPCINFO_LFORMAT, 
			(svrid == D23_DCASID) ? D20_CAPROC :
			(svrid == D23_CSTSID) ? D20_CSPROC :
			(svrid >  D23_SSTSID &&
			 svrid <= D23_SSTSID + D2MAXDIR_ID) ? D20_SSPROC :
			(svrid >  D23_DS0SID &&
			 svrid <= D23_MAXSVR) ? D20_DSPROC : D20_UKNPROC,
			svrptr->d23_epid.d23_rpid, (strcmp(dirid, "0") == 0) ?
			D20_NOVALUE : dirid, ipcid, svrptr->d23_ucstate) ;
			proc_no++ ;
	}
    }

    /* get directory system user information from the IPC-client area */
    for (i = 0, cltptr = d23_info.d23_cltarea ;
			    i < d23_info.d23_genarea->d23_mcno; i++, cltptr++) {
	if (D23_GES(cltptr->d23_estate) == D23_ATTACH &&
			 	     kill (cltptr->d23_epid.d23_rpid, 0) == 0) {
	    for (j = 0; j < printed_idx; j++)
		if (printed_pids[j] == cltptr->d23_epid.d23_rpid)
		    /* process specific information already printed */
		    break ;
	    if (j < printed_idx)
	        continue ;
	    printed_pids[printed_idx++] = cltptr->d23_epid.d23_rpid ;

	    sprintf (ipcid,"%d", cltptr->d23_esid) ;
	    dce_fprintf (stdout, GDS_S_IPCINFO_LFORMAT, D20_DIPROC,
				cltptr->d23_epid.d23_rpid,
				D20_NOVALUE, ipcid, cltptr->d23_ucstate) ;
	}
    }

    exit ((proc_no > 0) ? 0 : 1) ;
}
