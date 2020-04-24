/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d23cattach.c,v $
 * Revision 1.1.769.2  1996/02/18  18:19:26  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:42:14  marty]
 *
 * Revision 1.1.769.1  1995/12/08  16:02:56  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/06/15  19:53 UTC  smythe
 * 	fix for CHFts15308
 * 
 * 	HP revision /main/smythe_CHFts15308/1  1995/06/14  20:28 UTC  smythe
 * 	fix for CHFts15308
 * 	[1995/12/08  15:14:54  root]
 * 
 * Revision 1.1.767.4  1994/09/06  12:25:14  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:14:38  keutel]
 * 
 * Revision 1.1.767.3  1994/05/10  15:56:19  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:39:33  marrek]
 * 
 * Revision 1.1.767.2  1994/03/23  15:18:50  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:31:31  keutel]
 * 
 * Revision 1.1.767.1  1994/02/22  19:21:27  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:51:25  marrek]
 * 
 * Revision 1.1.765.2  1993/10/14  17:35:52  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:33:45  keutel]
 * 
 * Revision 1.1.765.1  1993/10/13  17:33:07  keutel
 * 	October 1993 code drop
 * 	[1993/10/11  08:56:09  keutel]
 * 
 * Revision 1.1.6.2  1993/08/11  12:02:53  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:24:43  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  20:44:26  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:31:46  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:52:58  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:40:08  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d23cattach.c,v $ $Revision: 1.1.769.2 $ $Date: 1996/02/18 18:19:26 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*exon ***********************************************************************/
/*                                                                           */
/*   TYPE:    function                                                       */
/*                                                                           */
/*   NAME:    d23_attclt ()                                                  */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The function attaches the calling client process to the IPC-        */
/*       resources.                                                          */
/*                                                                           */
/*   INPUT-PARAMETER:                                                        */
/*       atcl_pb     = Reference to the function parameter block which       */
/*                     must contain the following input parameter:           */
/*                             - IPC-version number                          */
/*                             - event mode                                  */
/*                                                                           */
/*   OUTPUT-PARAMETER:                                                       */
/*                     None                                                  */
/*                                                                           */
/*   RETURN-VALUE:                                                           */
/*       D23_SUCES   = The function has been performed successfully.         */
/*       D23_ERROR   = A system error or an internal error is occurred       */
/*                     during execution of the function (in this case the    */
/*                     standard header contains detailed error information). */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                        DATE: 17.5.91     */
/*exoff **********************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <d23ipc.h>


/* ------------------- DECLARATION OF GLOBAL DATA -------------------------- */

extern D23_iprocinfo	d23_info ;	/* process specific IPC-information */

/* ------------------------------------------------------------------------- */

signed32 d23_attclt (D23_atclpb *atcl_pb) {

    signed32	i, evmode, istate ;
    char	fname[D27_LFILE_NAME] ;
    signed32	cmask ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		     svc_c_debug1, GDS_S_CATTACH_ENTRY, atcl_pb->d23_9evmode)) ;

    /* check version number parameter */
    if (!D2_CHECK_MINMAX (D23_MIN, D23_MAX, atcl_pb->d23_9hdinfo.d2_version)) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
	svc_c_sev_fatal, GDS_S_IPCVERSIL_ERR, atcl_pb->d23_9hdinfo.d2_version) ;
	return (d23_ipcerror (&atcl_pb->d23_9hdinfo, D23_VERSIL)) ;
    }

    /* check client-state */
    if (((istate = D23_GIAS(d23_info.d23_istate)) & D23_ICLTATT) == D23_ICLTATT) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		     svc_c_sev_fatal, GDS_S_IPCSTATE_ERR, d23_info.d23_istate) ;
	return (d23_ipcerror (&atcl_pb->d23_9hdinfo, D23_ATCLIL)) ;
    }

    /* check event mode parameter */
    if (((evmode = D23_GEVM(atcl_pb->d23_9evmode)) != D23_SYNC_EM &&
	  evmode != D23_PASYNC_EM && evmode != D23_FASYNC_EM) ||
	  ((istate & D23_ISVRATT) == D23_ISVRATT &&
				    evmode != D23_GIEVM(d23_info.d23_istate))) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		   svc_c_sev_fatal, GDS_S_IPCEVMOIL_ERR, atcl_pb->d23_9evmode) ;
	return (d23_ipcerror (&atcl_pb->d23_9hdinfo, D23_EVMOIL)) ;
    }

    if (istate == D23_INATTACH) {
	/* get IPC-resources  */
	if (d23_get_ipc () == D23_ERROR) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
#ifndef __hpux_CHFts15308
					 svc_c_sev_notice, GDS_S_IPCGETRES_ERR) ;
#else
					 svc_c_sev_fatal, GDS_S_IPCGETRES_ERR) ;
#endif
	    return (d23_ipcerror (&atcl_pb->d23_9hdinfo, D23_GIPCERR)) ;
	}

	/* initialize client specific IPC-information */
	d23_info.d23_ipid.d23_vpid = D23_UNUSED ;

	d23_info.d23_clstref = (D23_ipcentry *) NULL ;
	d23_info.d23_enaevmsk = 0 ;
	d23_info.d23_pevfd = d23_info.d23_sevfd = D23_UNUSED ;
	for (i = 0; i < D23_MXEV; i++)
	    d23_info.d23_evinfo[i].d23_evfd = D23_UNUSED ;

	/* establish named FIFO's */
	if (evmode != D23_SYNC_EM) {
	    cmask = umask (0) ;
	    /* create process-ID specific event FIFO */
	    sprintf (fname, D23_FIFO_FILE, dcelocal_path, D27_CLIENT_DIR,
					      'p', d23_info.d23_ipid.d23_rpid) ;
	    if (mknod (fname, S_IFIFO|S_IRUSR|S_IWUSR|S_IWGRP|S_IWOTH, 0) < 0)
		if (errno != EEXIST) {
		    dce_svc_printf (DCE_SVC (gds_svc_handle, "%s%d%d"),
			GDS_S_IPC, svc_c_sev_fatal, GDS_S_MKNOD_ERR,
			fname, S_IFIFO|S_IRUSR|S_IWUSR|S_IWGRP|S_IWOTH, errno) ;
		    umask (cmask) ;
		    return (d23_ipcerror (&atcl_pb->d23_9hdinfo, D23_IFFMND)) ;
		}

	    /* open process-ID specific event FIFO */
	    if ((d23_info.d23_pevfd = open (fname, O_RDWR|O_NONBLOCK)) < 0) {
		dce_svc_printf (DCE_SVC (gds_svc_handle, "%s%d%d"),
			GDS_S_IPC, svc_c_sev_fatal, GDS_S_OPEN_ERR,
					      fname, O_RDWR|O_NONBLOCK, errno) ;
		umask (cmask) ;
		return (d23_ipcerror (&atcl_pb->d23_9hdinfo, D23_IFFOPN)) ;
	    }
	    umask (cmask) ;
	    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%s%d%d"), GDS_S_IPC,
		    		svc_c_debug3, GDS_S_OPEN_FIFO, fname,
				d23_info.d23_pevfd, O_RDWR|O_NONBLOCK)) ;
	}
    }

    /* set event type enable mask */
    if (evmode == D23_PASYNC_EM || evmode == D23_FASYNC_EM)
	d23_info.d23_enaevmsk |= (1 << D23_RESMSG) ;
    if (evmode == D23_FASYNC_EM)
	d23_info.d23_enaevmsk |= (1 << D23_RESCMSG) | (1 << D23_RRQBFMSG) ;

    /* change process specific IPC-state */
    d23_info.d23_istate |= (D23_SIAS(D23_ICLTATT) | D23_SIEVM(evmode)) ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					    svc_c_debug1, GDS_S_CATTACH_EXIT)) ;
    return (D23_SUCES) ;
}
