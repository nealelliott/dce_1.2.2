/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d23sattach.c,v $
 * Revision 1.1.772.2  1996/02/18  18:19:49  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:42:34  marty]
 *
 * Revision 1.1.772.1  1995/12/08  16:03:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:15:16  root]
 * 
 * Revision 1.1.770.4  1994/09/06  12:25:32  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:16:01  keutel]
 * 
 * Revision 1.1.770.3  1994/05/10  15:56:38  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:40:38  marrek]
 * 
 * Revision 1.1.770.2  1994/03/23  15:18:59  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:31:57  keutel]
 * 
 * Revision 1.1.770.1  1994/02/22  17:40:49  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:53:53  marrek]
 * 
 * Revision 1.1.768.2  1993/10/14  17:35:55  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:33:54  keutel]
 * 
 * Revision 1.1.768.1  1993/10/13  17:33:13  keutel
 * 	October 1993 code drop
 * 	[1993/10/11  08:55:56  keutel]
 * 
 * Revision 1.1.6.2  1993/08/11  12:18:45  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:27:27  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  20:45:58  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:32:40  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:55:50  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:41:26  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d23sattach.c,v $ $Revision: 1.1.772.2 $ $Date: 1996/02/18 18:19:49 $";
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
/*   NAME:    d23_attsvr ()                                                  */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The function attaches the calling server process to the IPC-        */
/*       resources. Thus, the server process is known to the client          */
/*       processes.                                                          */
/*                                                                           */
/*   INPUT-PARAMETER:                                                        */
/*       atsv_pb     = Reference to the function parameter block which       */
/*                     must contain the following input parameter:           */
/*                             - IPC-version number                          */
/*                             - server-ID                                   */
/*                             - event mode                                  */
/*                                                                           */
/*   OUTPUT-PARAMETER:                                                       */
/*                     The function parameter block contains the output      */
/*		       parameter as follows:                                 */
/*                             - max. size of the IPC-send/receive buffer    */
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
#include <signal.h>
#include <unistd.h>
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

signed32 d23_attsvr (D23_atsvpb *atsv_pb) {

    signed32		i, assid, evmode, cmask, svrtype = 0, istate ;
    D23_ipcentry	*cltptr, *svrptr, *svrfptr = (D23_ipcentry *) NULL ;
    char		fname[D27_LFILE_NAME] ;
 
    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_IPC,
			svc_c_debug1, GDS_S_SATTACH_ENTRY, atsv_pb->d23_0svrid,
			atsv_pb->d23_0evmode)) ;

    /* check version number parameter */
    if (!D2_CHECK_MINMAX (D23_MIN, D23_MAX, atsv_pb->d23_0hdinfo.d2_version)) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
	svc_c_sev_fatal, GDS_S_IPCVERSIL_ERR, atsv_pb->d23_0hdinfo.d2_version) ;
	return (d23_ipcerror (&atsv_pb->d23_0hdinfo, D23_VERSIL)) ;
    }

    /* check server-state */
    if (((istate = D23_GIAS(d23_info.d23_istate)) & D23_ISVRATT) == D23_ISVRATT) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		     svc_c_sev_fatal, GDS_S_IPCSTATE_ERR, d23_info.d23_istate) ;
	return (d23_ipcerror (&atsv_pb->d23_0hdinfo, D23_ATSVIL)) ;
    }

    /* check server-ID parameter */
    if (atsv_pb->d23_0svrid < 1 || atsv_pb->d23_0svrid > D23_MAXSVR) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		    svc_c_sev_fatal, GDS_S_IPCSVIDIL_ERR, atsv_pb->d23_0svrid) ;
	return (d23_ipcerror (&atsv_pb->d23_0hdinfo, D23_SVIDIL)) ;
    }

    /* check event mode parameter */
    if (((evmode = D23_GEVM(atsv_pb->d23_0evmode)) != D23_SYNC_EM &&
	  evmode != D23_PASYNC_EM && evmode != D23_FASYNC_EM) ||
	  ((istate & D23_ICLTATT) == D23_ICLTATT &&
				    evmode != D23_GIEVM(d23_info.d23_istate))) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		   svc_c_sev_fatal, GDS_S_IPCEVMOIL_ERR, atsv_pb->d23_0evmode) ;
	return (d23_ipcerror (&atsv_pb->d23_0hdinfo, D23_EVMOIL)) ;
    }

    /* check server process mode parameter */
    if (evmode != D23_SYNC_EM &&
       ((svrtype = D23_GSTY(atsv_pb->d23_0evmode)) != D23_SPROCSVR &&
						     svrtype != D23_MPROCSVR)) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		   svc_c_sev_fatal, GDS_S_IPCSVMOIL_ERR, atsv_pb->d23_0evmode) ;
	return (d23_ipcerror (&atsv_pb->d23_0hdinfo, D23_SVMOIL)) ;
    }

    if (istate == D23_INATTACH) {
	/* get IPC-resources */
	if (d23_get_ipc () == D23_ERROR) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					 svc_c_sev_fatal, GDS_S_IPCGETRES_ERR) ;
	    return (d23_ipcerror (&atsv_pb->d23_0hdinfo, D23_GIPCERR)) ;
	}
	/* initialize server specific IPC-information */
	d23_info.d23_ipid.d23_vpid = d23_getvpid () ;

	d23_info.d23_slstref = (D23_ipcentry *) NULL ;
	d23_info.d23_enaevmsk = 0 ;
	d23_info.d23_pevfd = d23_info.d23_sevfd = D23_UNUSED ;
	for (i = 0; i < D23_MXEV; i++)
	    d23_info.d23_evinfo[i].d23_evfd = D23_UNUSED ;
    }

    /* get new virtual process-ID */
    if (d23_info.d23_ipid.d23_vpid == D23_UNUSED)
	d23_info.d23_ipid.d23_vpid = d23_getvpid () ;

    /* establish named FIFO's */
    if (evmode != D23_SYNC_EM) {
	cmask = umask (0) ;
	if (svrtype == D23_MPROCSVR) {
	    /* create server-ID specific event FIFO */
	    sprintf (fname, D23_FIFO_FILE, dcelocal_path, D27_CLIENT_DIR,
						     's', atsv_pb->d23_0svrid) ;
	    if (mknod (fname, S_IFIFO|S_IRUSR|S_IWUSR|S_IWGRP|S_IWOTH, 0) < 0) {
		if (errno != EEXIST) {
		    dce_svc_printf (DCE_SVC (gds_svc_handle, "%s%d%d"),
			GDS_S_IPC, svc_c_sev_fatal, GDS_S_MKNOD_ERR,
			fname, S_IFIFO|S_IRUSR|S_IWUSR|S_IWGRP|S_IWOTH, errno) ;
		    umask (cmask) ;
		    return (d23_ipcerror (&atsv_pb->d23_0hdinfo, D23_IFFMND)) ;
		}
	    }

	    /* open server-ID specific event FIFO */
	    if ((d23_info.d23_sevfd = open (fname, O_RDWR|O_NONBLOCK)) < 0) {
		dce_svc_printf (DCE_SVC (gds_svc_handle, "%s%d%d"),
			GDS_S_IPC, svc_c_sev_fatal, GDS_S_OPEN_ERR,
					      fname, O_RDWR|O_NONBLOCK, errno) ;
		umask (cmask) ;
		return (d23_ipcerror (&atsv_pb->d23_0hdinfo, D23_IFFOPN)) ;
	    }
	    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%s%d%d"), GDS_S_IPC,
		    		svc_c_debug3, GDS_S_OPEN_FIFO, fname,
				d23_info.d23_sevfd, O_RDWR|O_NONBLOCK)) ;
	}
	if ((istate & D23_ICLTATT) != D23_ICLTATT) {
	    /* create process-ID specific named FIFO */
	    sprintf (fname, D23_FIFO_FILE, dcelocal_path, D27_CLIENT_DIR,
				 	      'p', d23_info.d23_ipid.d23_rpid) ;
	    if (mknod (fname, S_IFIFO|S_IRUSR|S_IWUSR|S_IWGRP|S_IWOTH, 0) < 0) {
		if (errno != EEXIST) {
		    dce_svc_printf (DCE_SVC (gds_svc_handle, "%s%d%d"),
			GDS_S_IPC, svc_c_sev_fatal, GDS_S_MKNOD_ERR,
			fname, S_IFIFO|S_IRUSR|S_IWUSR|S_IWGRP|S_IWOTH, errno) ;
		    umask (cmask) ;
		    return (d23_ipcerror (&atsv_pb->d23_0hdinfo, D23_IFFMND)) ;
		}
	    }

	    /* open process-ID specific named FIFO */
	    if ((d23_info.d23_pevfd = open (fname, O_RDWR|O_NONBLOCK)) < 0) {
		dce_svc_printf (DCE_SVC (gds_svc_handle, "%s%d%d"),
			GDS_S_IPC, svc_c_sev_fatal, GDS_S_OPEN_ERR,
					      fname, O_RDWR|O_NONBLOCK, errno) ;
		umask (cmask) ;
		if (d23_info.d23_sevfd != D23_UNUSED)
		    close (d23_info.d23_sevfd) ;
		return (d23_ipcerror (&atsv_pb->d23_0hdinfo, D23_IFFOPN)) ;
	    }
	    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%s%d%d"), GDS_S_IPC,
				svc_c_debug3, GDS_S_OPEN_FIFO, fname,
				d23_info.d23_pevfd, O_RDWR|O_NONBLOCK)) ;
	}
	umask (cmask) ;
    }

    /* lock shared memory area */
    if (d23_lock (d23_info.d23_semid, D23_SHMSEMNO) == D23_ERROR) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSYNC_ERR) ;
	if (d23_info.d23_pevfd != D23_UNUSED)
	    close (d23_info.d23_pevfd) ;
	if (d23_info.d23_sevfd != D23_UNUSED)
	    close (d23_info.d23_sevfd) ;
	return (d23_ipcerror (&atsv_pb->d23_0hdinfo, D23_LOSEMA)) ;
    }

    /* search for a free server entry (if the server process is already */
    /* attached through an earlier process incarnation (see distributed */
    /* command RESTART), then the old server entry is reused) */
    for (i = 0, svrptr = d23_info.d23_svrarea;
			    i < d23_info.d23_genarea->d23_msno; i++, svrptr++) {
	if (D23_GES(svrptr->d23_estate) == D23_NOSVR) {
	    if (svrfptr == (D23_ipcentry *) NULL) {
		/* save pointer to free server entry */
		svrfptr = svrptr ;
		assid = i ;
	    }
	} else {
	    if (D23_GSET(svrptr->d23_estate) == D23_PRIMARY &&
				      svrptr->d23_esid == atsv_pb->d23_0svrid) {
		/* server is already attached */
		if (D23_GOEVM(svrptr->d23_estate) != evmode) {
		    d23_unlock (d23_info.d23_semid, D23_SHMSEMNO) ;
		    if (d23_info.d23_pevfd != D23_UNUSED)
			close (d23_info.d23_pevfd) ;
		    if (d23_info.d23_sevfd != D23_UNUSED)
			close (d23_info.d23_sevfd) ;
		    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"),
				GDS_S_IPC, svc_c_sev_fatal,
				GDS_S_IPCEVMOIL_ERR, atsv_pb->d23_0evmode) ;
		    return (d23_ipcerror (&atsv_pb->d23_0hdinfo, D23_EVMOIL)) ;
		}
		if (svrptr->d23_epid.d23_rpid == d23_info.d23_ipid.d23_rpid) {
		    /* process is already attached -> reuse entry */
		    svrfptr = svrptr ;
		    assid = i ;
		    d23_info.d23_genarea->d23_sno-- ;
		} else {
		    if (svrtype == D23_SPROCSVR &&
				     kill (svrptr->d23_epid.d23_rpid, 0) == 0) {
			/* single process server type specified, but another */
			/* process (using the same server-ID) already runs */
			if (d23_info.d23_pevfd != D23_UNUSED)
			    close (d23_info.d23_pevfd) ;
			if (d23_info.d23_sevfd != D23_UNUSED)
			    close (d23_info.d23_sevfd) ;
			d23_unlock (d23_info.d23_semid, D23_SHMSEMNO) ;
		        dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"),
				GDS_S_IPC, svc_c_sev_fatal,
				GDS_S_IPCSVMOIL_ERR, atsv_pb->d23_0evmode) ;
			return (d23_ipcerror (&atsv_pb->d23_0hdinfo, D23_SVMOIL)) ;
		    }
		}
	    }
	}
    }

    if (svrfptr == (D23_ipcentry *) NULL) {
	if (d23_info.d23_pevfd != D23_UNUSED)
	    close (d23_info.d23_pevfd) ;
	if (d23_info.d23_sevfd != D23_UNUSED)
	    close (d23_info.d23_sevfd) ;
	d23_unlock (d23_info.d23_semid, D23_SHMSEMNO) ;
        dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"),
			GDS_S_IPC, svc_c_sev_fatal, GDS_S_IPCSLIMSVR_ERR,
			d23_info.d23_genarea->d23_msno) ;
	return (d23_ipcerror (&atsv_pb->d23_0hdinfo, D23_LIMSVR)) ;
    }

    /* occupy free server entry */
    strcpy (svrfptr->d23_ucstate, D23_NOSTATE) ;
    svrfptr->d23_epid = d23_info.d23_ipid ;
    svrfptr->d23_esid = atsv_pb->d23_0svrid ;
    svrfptr->d23_cassid = D23_UNUSED ;
    svrfptr->d23_invinfo.d23_invid = D23_UNUSED ;
    svrfptr->d23_cpid.d23_rpid = svrfptr->d23_cpid.d23_vpid = D23_UNUSED ;
    svrfptr->d23_estate = D23_SES(D23_ATTACH) | D23_SEOS(D23_ASSOC) |
			  D23_SOEVM(evmode) | D23_SSET(D23_PRIMARY) |
			  D23_SSEST(svrtype) ;
    d23_info.d23_genarea->d23_sno++ ;
	
    if (svrtype == D23_SPROCSVR) {
	/* adjust server information within existing client entries */
	for (i = 0, cltptr = d23_info.d23_cltarea;
			    i < d23_info.d23_genarea->d23_mcno; i++, cltptr++) {
	    if (D23_GES(cltptr->d23_estate) == D23_ATTACH &&
		cltptr->d23_esid == svrfptr->d23_esid &&
		D23_GEOS(cltptr->d23_estate) == D23_ASSOC &&
				       kill (cltptr->d23_epid.d23_rpid, 0) == 0)
		cltptr->d23_cpid = svrfptr->d23_epid ;
	} 
    }

    /* unlock shared memory area */
    if (d23_unlock (d23_info.d23_semid, D23_SHMSEMNO) == D23_ERROR) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSYNC_ERR) ;
	svrfptr->d23_estate = D23_SES(D23_NOSVR) ;
	d23_info.d23_genarea->d23_sno-- ;
	if (d23_info.d23_pevfd != D23_UNUSED)
	    close (d23_info.d23_pevfd) ;
	if (d23_info.d23_sevfd != D23_UNUSED)
	    close (d23_info.d23_sevfd) ;
	return (d23_ipcerror (&atsv_pb->d23_0hdinfo, D23_ULSEMA)) ;
    }

    /* set event type enable mask */
    if (evmode == D23_PASYNC_EM || evmode == D23_FASYNC_EM)
	d23_info.d23_enaevmsk |= (1 << D23_INVMSG) ;
    if (evmode == D23_FASYNC_EM)
	d23_info.d23_enaevmsk |= (1 << D23_INVCMSG) | (1 << D23_RRSBFMSG) ;

    /* change process specific IPC-state */
    d23_info.d23_istate |= (D23_SIAS(D23_ISVRATT) | D23_SIEVM(evmode) |
		       D23_SISTY(svrtype)) ;
    d23_info.d23_primref = svrfptr ;
    d23_info.d23_prassid = assid ;
    d23_info.d23_isvrid = atsv_pb->d23_0svrid ;

    /* supply result parameter */
    atsv_pb->d23_0mslen = d23_info.d23_genarea->d23_bsize ;
	
    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_IPC,
		svc_c_debug1, GDS_S_SATTACH_EXIT, assid, atsv_pb->d23_0mslen)) ;
    return (D23_SUCES) ;
}
