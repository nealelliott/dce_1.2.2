/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d23distcmd.c,v $
 * Revision 1.1.12.2  1996/02/18  18:19:31  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:42:18  marty]
 *
 * Revision 1.1.12.1  1995/12/08  16:03:04  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:14:58  root]
 * 
 * Revision 1.1.10.3  1994/09/06  12:25:19  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:14:55  keutel]
 * 
 * Revision 1.1.10.2  1994/05/10  15:56:22  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:39:47  marrek]
 * 
 * Revision 1.1.10.1  1994/02/22  19:21:31  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:51:49  marrek]
 * 
 * Revision 1.1.8.1  1993/11/03  13:17:06  keutel
 * 	CR 9300
 * 	[1993/11/03  13:16:34  keutel]
 * 
 * Revision 1.1.6.2  1993/08/11  12:05:03  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:25:08  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  20:44:31  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:31:53  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  17:31:31  marrek
 * 	November 1992 code drop
 * 	[1992/11/26  10:48:32  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:53:22  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:40:18  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d23distcmd.c,v $ $Revision: 1.1.12.2 $ $Date: 1996/02/18 18:19:31 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton **********************************************************************/
/*                                                                           */
/*   TYPE:    module                                                         */
/*                                                                           */
/*   NAME:    d23distcmd.c                                                   */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       This module contains all functions which are required to handle     */
/*       distributed IPC-commands. The following functions are available     */
/*                                                                           */
/*       * d23_write_dist_cmd - send a distributed ipc-command               */
/*       * d23_read_dist_cmd  - receive a distributed ipc-command            */
/*       * d23_conf_dist_cmd  - confirm a received distributed ipc-command   */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                          DATE: 17.5.91   */
/*datoff *********************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <signal.h>
#include <dce/d27util.h>
#include <d23ipc.h>


/* ---------------- DECLARATION OF GLOBAL DATA ----------------------------- */

extern D23_iprocinfo	d23_info ;	/* process specific IPC-information */

/* ---------------- DECLARATION OF LOCAL DATA ------------------------------ */

				/* initialization state of local variables */
static	boolean		d23_initstate = FALSE ;
				/* command confirmation states */
static	boolean		d23_confstate[D23_MXDISTCMD] ;
				/* last distributed command keys */
static	signed32	d23_distcmdkey[D23_MXDISTCMD] ;

/*exon ***********************************************************************/
/*                                                                           */
/*   TYPE:    function                                                       */
/*                                                                           */
/*   NAME:    d23_write_dist_cmd ()                                          */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The function writes the informations of a distributed command to    */
/*       the distributed command area of IPC, informs all server processes   */
/*       which belong to the requested server-ID (by sending a signal        */
/*       SIGUSR1) and waits (within the given time out interval) for the     */
/*       confirmation of the server processes.                               */
/*       If the sending process belongs also to the requested server-ID,     */
/*       then it will omitted.                                               */
/*                                                                           */
/*   INPUT-PARAMETER:                                                        */
/*       cmd         = Command code of the distributed IPC-command.          */
/*       sv_prid     = Server-ID of the server(s) or process-ID of a process */
/*                     to which the distributed command is directed.         */
/*       tmout       = Time out value of the distributed command             */
/*                     (1 <= tmout <= D23_DMAXTOUT).                         */
/*       par         = Reference to the parameter block of the command       */
/*                     (max. parameter block size is D23_DPARSIZE) or a      */
/*                     NULL-pointer, if there is no parameter block.         */
/*                                                                           */
/*       parsize     = Size of the parameter block (irrelevant, if parameter */
/*                     par is a NULL-pointer).                               */
/*                                                                           */
/*   OUTPUT-PARAMETER:                                                       */
/*                     None                                                  */
/*                                                                           */
/*   RETURN_VALUE:                                                           */
/*       D23_SUCES   = The function has been performed successfully.         */
/*       D23_ERROR   = A system error or an internal error is occurred       */
/*                     during execution of the function.                     */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                          DATE: 17.5.91   */
/*exoff **********************************************************************/

signed32 d23_write_dist_cmd (D23_dwritepb *dwr_pb) {

    signed32		i, cmdtstamp ;
    boolean		myprocfound = FALSE ;
    D23_distcmd		*distptr ;
    D23_ipcentry	*svrptr ;
    D23_reginfo		*regptr ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d%d%d%d"), GDS_S_IPC,
			svc_c_debug1, GDS_S_CSDWRITE_ENTRY, dwr_pb->d23_Jcmd,
			dwr_pb->d23_Jsv_prid, dwr_pb->d23_Jtmout,
			dwr_pb->d23_Jparref, dwr_pb->d23_Jparsize)) ;

    /* check version number parameter */
    if (!D2_CHECK_MINMAX (D23_MIN, D23_MAX, dwr_pb->d23_Jhdinfo.d2_version)) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
	svc_c_sev_fatal, GDS_S_IPCVERSIL_ERR, dwr_pb->d23_Jhdinfo.d2_version) ;
	return (d23_ipcerror (&dwr_pb->d23_Jhdinfo, D23_VERSIL)) ;
    }

    /* get IPC-resources */
    if (d23_get_ipc () == D23_ERROR) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					 svc_c_sev_fatal, GDS_S_IPCGETRES_ERR) ;
	return (d23_ipcerror (&dwr_pb->d23_Jhdinfo, D23_GIPCERR)) ;
    }

    if (!d23_initstate) {
	/* initialize local variables */
	for (i = 0; i < D23_MXDISTCMD; i++) {
	    d23_confstate[i] = FALSE ;
	    d23_distcmdkey[i] = 0L ;
	}
	d23_info.d23_ipid.d23_rpid = getpid () ;
	d23_initstate = TRUE ;
    }

    /* check timeout parameter resp. parameter block size */
    if (dwr_pb->d23_Jtmout < 1 || dwr_pb->d23_Jtmout > D23_DMAXTOUT) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		    svc_c_sev_fatal, GDS_S_IPCTMOUTIL_ERR, dwr_pb->d23_Jtmout) ;
	return (d23_ipcerror (&dwr_pb->d23_Jhdinfo, D23_DPARERR)) ;
    }
    if (dwr_pb->d23_Jparref != (byte *) NULL && (dwr_pb->d23_Jparsize <= 0 ||
					 dwr_pb->d23_Jparsize > D23_DPARSIZE)) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		  svc_c_sev_fatal, GDS_S_IPCPARSZIL_ERR, dwr_pb->d23_Jparsize) ;
	return (d23_ipcerror (&dwr_pb->d23_Jhdinfo, D23_DPARERR)) ;
    }

    /* lock shared memory */
    if (d23_lock (d23_info.d23_semid, D23_SHMSEMNO) == D23_ERROR) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSYNC_ERR) ;
	return (d23_ipcerror (&dwr_pb->d23_Jhdinfo, D23_LOSEMA)) ;
    }

    /* check distributed command state */
    for (i = 0, distptr = d23_info.d23_dstarea;
			   i < d23_info.d23_genarea->d23_mdno; i++, distptr++) {
	if (distptr->d23_dcmd == D23_UNUSED)
	    break ;
    }
    if (i == d23_info.d23_genarea->d23_mdno) {
	d23_unlock (d23_info.d23_semid, D23_SHMSEMNO) ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
				    svc_c_sev_notice, GDS_S_IPCLIMDIST_ERR, i) ;
	return (d23_ipcerror (&dwr_pb->d23_Jhdinfo, D23_DBUSY)) ;
    }

    /* initialize distributed ipc-command area */
    distptr->d23_dcpid = d23_info.d23_ipid.d23_rpid ;
    distptr->d23_dsv_prid = dwr_pb->d23_Jsv_prid ;
    cmdtstamp = time (0L) ;
    distptr->d23_dtstamp = ++(d23_info.d23_genarea->d23_gltstamp) ;
    distptr->d23_disvrno = distptr->d23_dconfno = 0 ;
    distptr->d23_dtout = dwr_pb->d23_Jtmout ;
    if (dwr_pb->d23_Jparref != (byte *) NULL) {
	if ((distptr->d23_dpar = d23_shmalloc (dwr_pb->d23_Jparsize)) ==
								    D23_ERROR) {
	    d23_unlock (d23_info.d23_semid, D23_SHMSEMNO) ;
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		 svc_c_sev_notice, GDS_S_IPCLIMHEAP_ERR, dwr_pb->d23_Jparsize) ;
	    return (d23_ipcerror (&dwr_pb->d23_Jhdinfo, D23_NOHPMEM)) ;
	}
	memcpy ((void *) ((byte *) d23_info.d23_hparea + distptr->d23_dpar),
		  (void *) dwr_pb->d23_Jparref, (size_t) dwr_pb->d23_Jparsize) ;
	distptr->d23_dparsize = dwr_pb->d23_Jparsize ;
    } else
	distptr->d23_dparsize = 0 ;
    distptr->d23_dcmd = dwr_pb->d23_Jcmd ;

    /* unlock shared memory */
    if (d23_unlock (d23_info.d23_semid, D23_SHMSEMNO) == D23_ERROR) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSYNC_ERR) ;
	return (d23_ipcerror (&dwr_pb->d23_Jhdinfo, D23_ULSEMA)) ;
    }

    /* distribute IPC-command */
    if (dwr_pb->d23_Jcmd != D23_DCABANDON) {
	for (i = 0, svrptr = d23_info.d23_svrarea ;
			    i < d23_info.d23_genarea->d23_msno; i++, svrptr++) {
	    if (D23_GES(svrptr->d23_estate) == D23_ATTACH &&
				     svrptr->d23_esid == dwr_pb->d23_Jsv_prid) {
		if (svrptr->d23_epid.d23_rpid == d23_info.d23_ipid.d23_rpid) {
		    myprocfound = TRUE ;
		    continue ;
		}
		if (kill (svrptr->d23_epid.d23_rpid, SIGUSR1) == 0)
		    distptr->d23_disvrno++ ;
	    }
	}
	for (i = 0, regptr = d23_info.d23_regarea;
			    i < d23_info.d23_genarea->d23_mrno; i++, regptr++) {
	    if (regptr->d23_pregtype == dwr_pb->d23_Jsv_prid) {
		if (regptr->d23_procid == d23_info.d23_ipid.d23_rpid) {
		    myprocfound = TRUE ;
		    continue ;
		}
		if (kill (regptr->d23_procid, SIGUSR1) == 0)
		    distptr->d23_disvrno++ ;
	    }
	}
    } else {
	if (kill (dwr_pb->d23_Jsv_prid, SIGUSR1) == 0)
	    distptr->d23_disvrno++ ;
    }

    if (distptr->d23_disvrno == 0) {
	/* error -> no recipient process available */
	distptr->d23_dcmd = D23_UNUSED ;
	if (dwr_pb->d23_Jparref != (byte *) NULL)
	    d23_shmfree (distptr->d23_dpar) ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		   svc_c_sev_fatal, GDS_S_IPCNOSERV_ERR, dwr_pb->d23_Jsv_prid) ;
	return (d23_ipcerror (&dwr_pb->d23_Jhdinfo, D23_NOSERV)) ;
    }

    /* wait for confirmation of the distributed IPC-command */
    while (distptr->d23_dconfno < distptr->d23_disvrno &&
				     time (0L) < cmdtstamp + distptr->d23_dtout)
	sleep (1) ;
	
    if (dwr_pb->d23_Jparref != (byte *) NULL) {
	if (d23_shmfree (distptr->d23_dpar) == D23_ERROR) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		     svc_c_sev_fatal, GDS_S_IPCHEAPREL_ERR, distptr->d23_dpar) ;
	    return (d23_ipcerror (&dwr_pb->d23_Jhdinfo, D23_HPMEM)) ;
	}
    }
    distptr->d23_dcmd = D23_UNUSED ;
    if (distptr->d23_dconfno < distptr->d23_disvrno) {
	/* error -> timeout */
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_IPC,
		   	svc_c_sev_fatal, GDS_S_IPCTMEX_ERR, dwr_pb->d23_Jcmd,
			dwr_pb->d23_Jsv_prid, dwr_pb->d23_Jtmout) ;
	return (d23_ipcerror (&dwr_pb->d23_Jhdinfo, D23_TMOUT)) ;
    }

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		     svc_c_debug1, GDS_S_CSDWRITE_EXIT, distptr->d23_dconfno)) ;

    return (D23_SUCES) ;
}

/*exon ***********************************************************************/
/*                                                                           */
/*   TYPE:    function                                                       */
/*                                                                           */
/*   NAME:    d23_read_dist_cmd ()                                           */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The function reads the available information of a received          */
/*       distributed command.                                                */
/*                                                                           */
/*   INPUT-PARAMETER:                                                        */
/*       cmd         = Reference to a variable where the command code of the */
/*                     received command can be stored.                       */
/*       key         = Reference to a variable where the command key of the  */
/*                     received command can be stored (this key must be      */
/*                     used to confirm a distributed command -> see function */
/*                     d23_dconf_cmd()).                                     */
/*       par         = Reference to a variable where the reference to the    */
/*                     parameter block of the received command can be stored.*/
/*       parsize     = Reference to a variable where the size of the para-   */
/*                     meter block can be stored.                            */
/*                                                                           */
/*   OUTPUT-PARAMETER:                                                       */
/*       *cmd        = Command code of the command.                          */
/*       *key        = Command key (confirmation key) of the command.        */
/*       *par        = Reference to the parameter block of the command       */
/*                     (NULL-pointer, if there is no parameter block).       */
/*       *parsize    = Size of the parameter block (irrelevant, if parameter */
/*                     *par is a NULL-pointer)                               */
/*                                                                           */
/*   RETURN_VALUE:                                                           */
/*       D23_SUCES   = The function has been performed successfully.         */
/*       D23_ERROR   = A system error or an internal error is occurred       */
/*                     during execution of the function.                     */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                          DATE: 17.5.91   */
/*exoff **********************************************************************/

signed32 d23_read_dist_cmd (D23_dreadpb *drd_pb) {

    signed32		i, j ;
    D23_distcmd		*distptr ;
    D23_ipcentry	*svrptr ;
    D23_reginfo		*regptr ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_debug1, GDS_S_CSDREAD_ENTRY)) ;

    /* check version number parameter */
    if (!D2_CHECK_MINMAX (D23_MIN, D23_MAX, drd_pb->d23_Khdinfo.d2_version)) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
	svc_c_sev_fatal, GDS_S_IPCVERSIL_ERR, drd_pb->d23_Khdinfo.d2_version) ;
	return (d23_ipcerror (&drd_pb->d23_Khdinfo, D23_VERSIL)) ;
    }

    /* get IPC-resources */
    if (d23_get_ipc () == D23_ERROR) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					 svc_c_sev_fatal, GDS_S_IPCGETRES_ERR) ;
	return (d23_ipcerror (&drd_pb->d23_Khdinfo, D23_GIPCERR)) ;
    }

    if (!d23_initstate) {
	/* initialize local variables */
	for (i = 0; i < D23_MXDISTCMD; i++) {
	    d23_confstate[i] = FALSE ;
	    d23_distcmdkey[i] = 0L ;
	}
	d23_info.d23_ipid.d23_rpid = getpid () ;
	d23_initstate = TRUE ;
    }

    /* lock shared memory */
    if (d23_lock (d23_info.d23_semid, D23_SHMSEMNO) == D23_ERROR) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSYNC_ERR) ;
    	return (d23_ipcerror (&drd_pb->d23_Khdinfo, D23_LOSEMA)) ;
    }

    /* verify distributed commands state */
    for (j = 0, distptr = d23_info.d23_dstarea;
			   j < d23_info.d23_genarea->d23_mdno; j++, distptr++) {
	if (distptr->d23_dcmd == D23_UNUSED)
	    continue ;

	if (distptr->d23_dtstamp == d23_distcmdkey[j])
	    continue ;

	/* verify server-ID/process-ID of called process */
	for (i = 0, svrptr = d23_info.d23_svrarea;
			      i < d23_info.d23_genarea->d23_msno; i++, svrptr++)
	    if (D23_GES(svrptr->d23_estate) == D23_ATTACH &&
		svrptr->d23_epid.d23_rpid == d23_info.d23_ipid.d23_rpid &&
		distptr->d23_dcpid != d23_info.d23_ipid.d23_rpid &&
		((distptr->d23_dcmd != D23_DCABANDON &&
				   svrptr->d23_esid == distptr->d23_dsv_prid) ||
		 (distptr->d23_dcmd == D23_DCABANDON &&
			  d23_info.d23_ipid.d23_rpid == distptr->d23_dsv_prid)))
		goto bloop ;

	for (i = 0, regptr = d23_info.d23_regarea ;
			      i < d23_info.d23_genarea->d23_mrno; i++, regptr++)
	    if (regptr->d23_procid == d23_info.d23_ipid.d23_rpid &&
		((distptr->d23_dcmd != D23_DCABANDON &&
			       regptr->d23_pregtype == distptr->d23_dsv_prid) ||
		 (distptr->d23_dcmd == D23_DCABANDON &&
			  d23_info.d23_ipid.d23_rpid == distptr->d23_dsv_prid)))
		goto bloop ;
    }

bloop:
    if (j == d23_info.d23_genarea->d23_mdno) {
	/* no distributed command available */
	if (d23_unlock (d23_info.d23_semid, D23_SHMSEMNO) == D23_ERROR) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSYNC_ERR) ;
	    return (d23_ipcerror (&drd_pb->d23_Khdinfo, D23_ULSEMA)) ;
	}
	DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_debug1, GDS_S_CSDREAD_EXIT1)) ;
	return (d23_ipcerror (&drd_pb->d23_Khdinfo, D23_NODCMD)) ;
    }

    /* return distributed command informations */
    drd_pb->d23_Kcmd = distptr->d23_dcmd ;
    drd_pb->d23_Kkey = d23_distcmdkey[j] = distptr->d23_dtstamp ;
    if (distptr->d23_dparsize > 0) {
	drd_pb->d23_Kparref = (byte *) d23_info.d23_hparea + distptr->d23_dpar ;
	drd_pb->d23_Kparsize = distptr->d23_dparsize ;
    } else
	drd_pb->d23_Kparref = (byte *) NULL ;

    d23_confstate[j] = TRUE ;

    /* unlock shared memory */
    if (d23_unlock (d23_info.d23_semid, D23_SHMSEMNO) == D23_ERROR) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSYNC_ERR) ;
	return (d23_ipcerror (&drd_pb->d23_Khdinfo, D23_ULSEMA)) ;
    }

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d%d%d"), GDS_S_IPC,
		svc_c_debug1, GDS_S_CSDREAD_EXIT, drd_pb->d23_Kcmd,
		drd_pb->d23_Kkey, drd_pb->d23_Kparref, drd_pb->d23_Kparsize)) ;
    return (D23_SUCES) ;
}

/*exon ***********************************************************************/
/*                                                                           */
/*   TYPE:    function                                                       */
/*                                                                           */
/*   NAME:    d23_conf_dist_cmd ()                                           */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The function writes the confirmation which belongs to a received    */
/*       distributed command to the shared memory.                           */
/*                                                                           */
/*   INPUT-PARAMETER:                                                        */
/*       key         = Command key of the distributed ipc-command            */
/*                     (see function d23_dread_cmd()).                       */
/*                                                                           */
/*   OUTPUT-PARAMETER:                                                       */
/*                     None                                                  */
/*                                                                           */
/*   RETURN_VALUE:                                                           */
/*       D23_SUCES   = The function has been performed successfully.         */
/*       D23_ERROR   = An internal error is occurred during execution of     */
/*                     the function.                                         */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                          DATE: 17.5.91   */
/*exoff **********************************************************************/

signed32 d23_conf_dist_cmd (D23_dconfpb *dcf_pb) {

    signed32		i ;
    D23_distcmd		*distptr ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
			 svc_c_debug1, GDS_S_CSDCONF_ENTRY, dcf_pb->d23_Lkey)) ;

    /* check version number parameter */
    if (!D2_CHECK_MINMAX (D23_MIN, D23_MAX, dcf_pb->d23_Lhdinfo.d2_version)) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
	svc_c_sev_fatal, GDS_S_IPCVERSIL_ERR, dcf_pb->d23_Lhdinfo.d2_version) ;
	return (d23_ipcerror (&dcf_pb->d23_Lhdinfo, D23_VERSIL)) ;
    }

    /* get IPC-resources */
    if (d23_get_ipc () == D23_ERROR) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					 svc_c_sev_fatal, GDS_S_IPCGETRES_ERR) ;
	return (d23_ipcerror (&dcf_pb->d23_Lhdinfo, D23_GIPCERR)) ;
    }

    /* check distributed command state */
    for (i = 0, distptr = d23_info.d23_dstarea;
			   i < d23_info.d23_genarea->d23_mdno; i++, distptr++) {
	if (distptr->d23_dcmd != D23_UNUSED &&
		   distptr->d23_dtstamp == dcf_pb->d23_Lkey && d23_confstate[i])
	    break ;
    }
    if (i == d23_info.d23_genarea->d23_mdno) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
			      svc_c_sev_fatal, GDS_S_IPCSTATE_ERR, D23_UNUSED) ;
	return (d23_ipcerror (&dcf_pb->d23_Lhdinfo, D23_DCONFILL)) ;
    }

    /* lock shared memory */
    if (d23_lock (d23_info.d23_semid, D23_SHMSEMNO) == D23_ERROR) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSYNC_ERR) ;
	return (d23_ipcerror (&dcf_pb->d23_Lhdinfo, D23_LOSEMA)) ;
    }

    /* confirm distibuted command in shared memory */
    distptr->d23_dconfno++ ;
    /* change process specific confirmation state */
    d23_confstate[i] = FALSE ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_IPC, svc_c_debug1,
	      GDS_S_CSDCONF_EXIT, distptr->d23_disvrno, distptr->d23_dconfno)) ;

    /* unlock shared memory */
    if (d23_unlock (d23_info.d23_semid, D23_SHMSEMNO) == D23_ERROR) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSYNC_ERR) ;
	return (d23_ipcerror (&dcf_pb->d23_Lhdinfo, D23_ULSEMA)) ;
    }

    return (D23_SUCES) ;
}
