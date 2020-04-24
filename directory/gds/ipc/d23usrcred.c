/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d23usrcred.c,v $
 * Revision 1.1.10.2  1996/02/18  18:19:58  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:42:42  marty]
 *
 * Revision 1.1.10.1  1995/12/08  16:03:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:15:25  root]
 * 
 * Revision 1.1.8.4  1994/09/06  12:25:39  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:16:32  keutel]
 * 
 * Revision 1.1.8.3  1994/05/10  15:56:43  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:41:01  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:19:01  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:32:03  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:41:05  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:54:45  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  12:25:21  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:28:16  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  20:46:29  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:32:58  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:57:06  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:41:54  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d23usrcred.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:19:58 $";
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
/*   NAME:    d23usrcred.c                                                   */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       This module contains all functions which are required to handle     */
/*       the user credential area within the shared memory of a directory    */
/*       system installation. The following functions are available          */
/*                                                                           */
/*          * d23_add_usr_cred  - add user credentials to shared memory      */
/*          * d23_rem_usr_cred  - remove user credentials from shared memory */
/*          * d23_read_usr_cred - read user credentials from shared memory   */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                        DATE: 17.5.91     */
/*datoff *********************************************************************/

#include <gds.h>
#include <stdio.h>
#include <string.h>
#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <d23ipc.h>


/* --------------------- DECLARATION OF GLOBAL DATA ------------------------ */

extern D23_iprocinfo	d23_info ;	/* process specific IPC-information */

/*exon ***********************************************************************/
/*                                                                           */
/*   TYPE:    function                                                       */
/*                                                                           */
/*   NAME:    d23_add_usr_cred ()                                            */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The function adds the given user credentials (user name) to the     */
/*       shared memory.                                                      */
/*                                                                           */
/*   INPUT-PARAMETER:                                                        */
/*       contid      = Context-ID to which the given user credentials        */
/*                     belong. The following values are allowed:             */
/*                     D27_DAP_AC + D27_LOC_AC   (DAP + local access)        */
/*                     D27_DAP_AC + D27_REM_AC   (DAP + remote access)       */
/*                     D27_DSP_AC + D27_REM_AC   (DSP + remote access)       */
/*                                                                           */
/*       usrid       = User-ID                                               */
/*                                                                           */
/*       credref     = Reference to the user credential information          */
/*                                                                           */
/*       credsize    = Length of user credential information                 */
/*                                                                           */
/*   OUTPUT-PARAMETER:                                                       */
/*                     None                                                  */
/*                                                                           */
/*   RETURN-VALUE:                                                           */
/*       D23_SUCES   = The function has been performed successfully.         */
/*       D23_ERROR   = A system error or an internal error is occurred       */
/*                     during execution of the function.                     */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                        DATE: 17.5.91     */
/*exoff **********************************************************************/

signed32 d23_add_usr_cred (D23_addcrdpb *acr_pb) {

    signed32	i, idx ;
    signed16	contxt, acmode, remid ;
    unsigned16	icontid ;
    D23_usrcred	*usrcredtmp, *usrcred = (D23_usrcred *) NULL ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d%d%d"), GDS_S_IPC,
	svc_c_debug1, GDS_S_CSADDCRD_ENTRY, acr_pb->d23_Mcontid,
	acr_pb->d23_Musrid, acr_pb->d23_Mcredref, acr_pb->d23_Mcredsize)) ;

    /* check version number parameter */
    if (!D2_CHECK_MINMAX (D23_MIN, D23_MAX, acr_pb->d23_Mhdinfo.d2_version)) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
	svc_c_sev_fatal, GDS_S_IPCVERSIL_ERR, acr_pb->d23_Mhdinfo.d2_version) ;
	return (d23_ipcerror (&acr_pb->d23_Mhdinfo, D23_VERSIL)) ;
    }

    /* check IPC-specific process state */
    if ((D23_GIAS(d23_info.d23_istate) & D23_ISVRATT) != D23_ISVRATT) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		     svc_c_sev_fatal, GDS_S_IPCSTATE_ERR, d23_info.d23_istate) ;
	return (d23_ipcerror (&acr_pb->d23_Mhdinfo, D23_UCADIL)) ;
    }

    /* check context-ID parameter */
    remid = D23_GRMID(acr_pb->d23_Mcontid) ;
    if (((acmode = D23_GACM(acr_pb->d23_Mcontid)) != D27_LOC_AC &&
	acmode != D27_REM_AC) ||
	((contxt = D23_GAC(acr_pb->d23_Mcontid)) != D27_DAP_AC &&
	contxt != D27_DSP_AC) || (acmode == D27_LOC_AC &&
	contxt != D27_DAP_AC && remid != 0)|| (acmode == D27_REM_AC &&
	(remid <= D23_SSTSID || remid > D23_SSTSID + D2MAXDIR_ID))) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_IPC,
		svc_c_sev_fatal, GDS_S_IPCINVIL_ERR, D23_UNUSED, D23_UNUSED,
		acr_pb->d23_Mcontid) ;
	return (d23_ipcerror (&acr_pb->d23_Mhdinfo, D23_CTXTIL)) ;
    }

    /* check user-ID parameter */
    if (acr_pb->d23_Musrid < 0 || (acmode == D27_LOC_AC &&
			 acr_pb->d23_Musrid > d23_info.d23_genarea->d23_mcno)) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_IPC,
				svc_c_sev_fatal, GDS_S_IPCINVIL_ERR, D23_UNUSED,
				acr_pb->d23_Musrid, D23_UNUSED) ;
	return (d23_ipcerror (&acr_pb->d23_Mhdinfo, D23_USIDIL)) ;
    }

    /* check user name parameter (size) */
    if (acr_pb->d23_Mcredsize > D2_DNL_MAX) {
	return (d23_ipcerror (&acr_pb->d23_Mhdinfo, D23_NSIZEIL)) ;
    }

    /* lock shared memory */
    if (d23_lock (d23_info.d23_semid, D23_SHMSEMNO) == D23_ERROR) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSYNC_ERR) ;
	return (d23_ipcerror (&acr_pb->d23_Mhdinfo, D23_LOSEMA)) ;
    }

    /* search free user credential entry */
    usrcredtmp = d23_info.d23_crdarea ;
    icontid = D23_SACM(acmode) | D23_SAC(contxt) | D23_SRMID(remid) ;
    for (i = 0; i < d23_info.d23_genarea->d23_mcred; i++, usrcredtmp++) {
	if (usrcredtmp->d23_credusrid == D23_UNUSED) {
	    if (usrcred == (D23_usrcred *) NULL) {
		usrcred = usrcredtmp ;
		idx = i ;
	    }
	} else {
	    if (usrcredtmp->d23_credcontid == icontid &&
			      usrcredtmp->d23_credusrid == acr_pb->d23_Musrid) {
		/* reuse existing user credential entry */
		usrcred = usrcredtmp ;	
		idx = i ;
		/* release old user credentials */
		if (d23_shmfree (usrcred->d23_credoff) == D23_ERROR) {
		    d23_unlock (d23_info.d23_semid, D23_SHMSEMNO) ;
		    return (d23_ipcerror (&acr_pb->d23_Mhdinfo, D23_NOCRED)) ;
		}
		usrcred->d23_credusrid = D23_UNUSED ;
		d23_info.d23_genarea->d23_cred-- ;
		break ;
	    }
	}
    }
    if (usrcred == (D23_usrcred *) NULL) {
	/* no user credential entry available */
	d23_unlock (d23_info.d23_semid, D23_SHMSEMNO) ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
					svc_c_sev_fatal, GDS_S_IPCLIMCRED_ERR,
					d23_info.d23_genarea->d23_mcred) ;
	return (d23_ipcerror (&acr_pb->d23_Mhdinfo, D23_NOCRED)) ;
    }

    /* allocate memory for user name and write informations to the entry */
    if ((usrcred->d23_credoff = d23_shmalloc (acr_pb->d23_Mcredsize)) ==
								    D23_ERROR) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		svc_c_sev_notice, GDS_S_IPCLIMHEAP_ERR, acr_pb->d23_Mcredsize) ;
	d23_unlock (d23_info.d23_semid, D23_SHMSEMNO) ;
	return (d23_ipcerror (&acr_pb->d23_Mhdinfo, D23_NOHPMEM)) ;
    }
    memcpy ((void *) ((char *) d23_info.d23_hparea + usrcred->d23_credoff),
		(void *) acr_pb->d23_Mcredref, (size_t) acr_pb->d23_Mcredsize) ;
    usrcred->d23_credcontid = icontid ;
    usrcred->d23_credsvrid = d23_info.d23_isvrid ;
    usrcred->d23_credusrid = acr_pb->d23_Musrid ;
    usrcred->d23_credlen = acr_pb->d23_Mcredsize ;
    d23_info.d23_genarea->d23_cred++ ;

    /* unlock shared memory */
    if (d23_unlock (d23_info.d23_semid, D23_SHMSEMNO) == D23_ERROR) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSYNC_ERR) ;
    	return (d23_ipcerror (&acr_pb->d23_Mhdinfo, D23_ULSEMA)) ;
    }

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
				      svc_c_debug1, GDS_S_CSADDCRD_EXIT, idx)) ;
    return (D23_SUCES) ;
}

/*exon ***********************************************************************/
/*                                                                           */
/*   TYPE:    function                                                       */
/*                                                                           */
/*   NAME:    d23_rem_usr_cred ()                                            */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The function removes the user credentials which correspond to       */
/*       the given context-ID and user-ID from the shared memory.            */
/*                                                                           */
/*   INPUT-PARAMETER:                                                        */
/*   INPUT-PARAMETER:                                                        */
/*       contid      = Context-ID to which the given user credentials        */
/*                     belong. The following values are allowed:             */
/*                     D27_DAP_AC + D27_LOC_AC   (DAP + local access)        */
/*                     D27_DAP_AC + D27_REM_AC   (DAP + remote access)       */
/*                     D27_DSP_AC + D27_REM_AC   (DSP + remote access)       */
/*                                                                           */
/*       usrid       = User-ID                                               */
/*                                                                           */
/*   OUTPUT-PARAMETER:                                                       */
/*                     None                                                  */
/*                                                                           */
/*   RETURN-VALUE:                                                           */
/*       D23_SUCES   = The function has been performed successfully.         */
/*       D23_ERROR   = A system error or an internal error is occurred       */
/*                     during execution of the function.                     */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                        DATE: 17.5.91     */
/*exoff **********************************************************************/

signed32 d23_rem_usr_cred (D23_remcrdpb *rcr_pb) {

    signed32	i ;
    signed16	contxt, acmode, remid ;
    unsigned16	icontid ;
    D23_usrcred	*usrcred ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_IPC,
				svc_c_debug1, GDS_S_CSREMCRD_ENTRY,
				rcr_pb->d23_Ncontid, rcr_pb->d23_Nusrid)) ;

    /* check version number parameter */
    if (!D2_CHECK_MINMAX (D23_MIN, D23_MAX, rcr_pb->d23_Nhdinfo.d2_version)) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
	svc_c_sev_fatal, GDS_S_IPCVERSIL_ERR, rcr_pb->d23_Nhdinfo.d2_version) ;
	return (d23_ipcerror (&rcr_pb->d23_Nhdinfo, D23_VERSIL)) ;
    }

    /* check IPC-specific process state */
    if ((D23_GIAS(d23_info.d23_istate) & D23_ISVRATT) != D23_ISVRATT) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		     svc_c_sev_fatal, GDS_S_IPCSTATE_ERR, d23_info.d23_istate) ;
	return (d23_ipcerror (&rcr_pb->d23_Nhdinfo, D23_UCRMIL)) ;
    }

    /* check context-ID parameter */
    remid = D23_GRMID(rcr_pb->d23_Ncontid) ;
    if (((acmode = D23_GACM(rcr_pb->d23_Ncontid)) != D27_LOC_AC &&
	acmode != D27_REM_AC) ||
	((contxt = D23_GAC(rcr_pb->d23_Ncontid)) != D27_DAP_AC &&
	contxt != D27_DSP_AC) || (acmode == D27_LOC_AC &&
	contxt != D27_DAP_AC && remid != 0)|| (acmode == D27_REM_AC &&
	(remid <= D23_SSTSID || remid > D23_SSTSID + D2MAXDIR_ID))) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_IPC,
		svc_c_sev_fatal, GDS_S_IPCINVIL_ERR, D23_UNUSED, D23_UNUSED,
		rcr_pb->d23_Ncontid) ;
	return (d23_ipcerror (&rcr_pb->d23_Nhdinfo, D23_CTXTIL)) ;
    }

    /* check user-ID parameter */
    if (rcr_pb->d23_Nusrid < 0 || (acmode == D27_LOC_AC &&
			 rcr_pb->d23_Nusrid > d23_info.d23_genarea->d23_mcno)) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_IPC,
				svc_c_sev_fatal, GDS_S_IPCINVIL_ERR, D23_UNUSED,
				rcr_pb->d23_Nusrid, D23_UNUSED) ;
	return (d23_ipcerror (&rcr_pb->d23_Nhdinfo, D23_USIDIL)) ;
    }

    /* lock shared memory */
    if (d23_lock (d23_info.d23_semid, D23_SHMSEMNO) == D23_ERROR) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSYNC_ERR) ;
	return (d23_ipcerror (&rcr_pb->d23_Nhdinfo, D23_LOSEMA)) ;
    }

    /* search user credential entry */
    icontid = D23_SAC(contxt) | D23_SACM(acmode) | D23_SRMID(remid) ;
    usrcred = d23_info.d23_crdarea ;
    for (i = 0; i < d23_info.d23_genarea->d23_mcred; i++, usrcred++)
	if (usrcred->d23_credcontid == icontid &&
				   usrcred->d23_credusrid == rcr_pb->d23_Nusrid)
	    break ;
    if (i == d23_info.d23_genarea->d23_mcred) {
	/* user credential entry not found */
	d23_unlock (d23_info.d23_semid, D23_SHMSEMNO) ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
			      svc_c_sev_fatal, GDS_S_IPCSTATE_ERR, D23_UNUSED) ;
	return (d23_ipcerror (&rcr_pb->d23_Nhdinfo, D23_NOCRED)) ;
    }

    /* remove user credential entry */
    if (d23_shmfree (usrcred->d23_credoff) == D23_ERROR) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		  svc_c_sev_fatal, GDS_S_IPCHEAPREL_ERR, usrcred->d23_credoff) ;
    	return (d23_ipcerror (&rcr_pb->d23_Nhdinfo, D23_NOCRED)) ;
    }
    usrcred->d23_credusrid = D23_UNUSED ;
    d23_info.d23_genarea->d23_cred-- ;

    /* unlock shared memory */
    if (d23_unlock (d23_info.d23_semid, D23_SHMSEMNO) == D23_ERROR) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSYNC_ERR) ;
	return (d23_ipcerror (&rcr_pb->d23_Nhdinfo, D23_ULSEMA)) ;
    }

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
					svc_c_debug1, GDS_S_CSREMCRD_EXIT, i)) ;
    return (D23_SUCES) ;
}

/*exon ***********************************************************************/
/*                                                                           */
/*   TYPE:    function                                                       */
/*                                                                           */
/*   NAME:    d23_read_usr_cred ()                                           */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The function reads the user credentials which correspond to the     */
/*       given context-ID and user-ID from the shared memory.                */
/*                                                                           */
/*   INPUT-PARAMETER:                                                        */
/*       contid      = Context-ID to which the given user credentials        */
/*                     belong. The following values are allowed:             */
/*                     D27_DAP_AC + D27_LOC_AC   (DAP + local access)        */
/*                     D27_DAP_AC + D27_REM_AC   (DAP + remote access)       */
/*                     D27_DSP_AC + D27_REM_AC   (DSP + remote access)       */
/*                                                                           */
/*       usrid       = User-ID                                               */
/*                                                                           */
/*       credinfo    = Reference to the memory area where the read user      */
/*                     credential information should be stored.              */
/*                                                                           */
/*       credlen     = Reference, where the size of the user credentials can */
/*                     can be stored.                                        */
/*                                                                           */
/*   OUTPUT-PARAMETER:                                                       */
/*       *credinfo   = User credential information.                          */
/*       *credlen    = Size of the user credential information.              */
/*                                                                           */
/*   RETURN-VALUE:                                                           */
/*       D23_SUCES   = The function has been performed successfully.         */
/*       D23_ERROR   = A system error or an internal error is occurred       */
/*                     during execution of the function.                     */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                        DATE: 17.5.91     */
/*exoff **********************************************************************/

signed32 d23_read_usr_cred (D23_readcrdpb *rcr_pb) {

    signed32	i ;
    signed16	contxt, acmode, remid ;
    unsigned16	icontid ;
    D23_usrcred	*usrcred ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_IPC,
		svc_c_debug1, GDS_S_CSREADCRD_ENTRY, rcr_pb->d23_Ocontid,
		rcr_pb->d23_Ousrid, rcr_pb->d23_Ocredref)) ;

    /* check version number parameter */
    if (!D2_CHECK_MINMAX (D23_MIN, D23_MAX, rcr_pb->d23_Ohdinfo.d2_version)) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
	svc_c_sev_fatal, GDS_S_IPCVERSIL_ERR, rcr_pb->d23_Ohdinfo.d2_version) ;
	return (d23_ipcerror (&rcr_pb->d23_Ohdinfo, D23_VERSIL)) ;
    }

    /* check IPC-specific process state */
    if ((D23_GIAS(d23_info.d23_istate) & D23_ISVRATT) != D23_ISVRATT) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
		     svc_c_sev_fatal, GDS_S_IPCSTATE_ERR, d23_info.d23_istate) ;
	return (d23_ipcerror (&rcr_pb->d23_Ohdinfo, D23_UCRDIL)) ;
    }

    /* check context-ID parameter */
    remid = D23_GRMID(rcr_pb->d23_Ocontid) ;
    if (((acmode = D23_GACM(rcr_pb->d23_Ocontid)) != D27_LOC_AC &&
	acmode != D27_REM_AC) ||
	((contxt = D23_GAC(rcr_pb->d23_Ocontid)) != D27_DAP_AC &&
	contxt != D27_DSP_AC) || (acmode == D27_LOC_AC &&
	contxt != D27_DAP_AC && remid != 0)|| (acmode == D27_REM_AC &&
	(remid <= D23_SSTSID || remid > D23_SSTSID + D2MAXDIR_ID))) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_IPC,
		svc_c_sev_fatal, GDS_S_IPCINVIL_ERR, D23_UNUSED, D23_UNUSED,
		rcr_pb->d23_Ocontid) ;
	return (d23_ipcerror (&rcr_pb->d23_Ohdinfo, D23_CTXTIL)) ;
    }

    /* check user-ID parameter */
    if (rcr_pb->d23_Ousrid < 0 || (acmode == D27_LOC_AC &&
			 rcr_pb->d23_Ousrid > d23_info.d23_genarea->d23_mcno)) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_IPC,
				svc_c_sev_fatal, GDS_S_IPCINVIL_ERR, D23_UNUSED,
				rcr_pb->d23_Ousrid, D23_UNUSED) ;
	return (d23_ipcerror (&rcr_pb->d23_Ohdinfo, D23_USIDIL)) ;
    }

    /* lock shared memory */
    if (d23_lock (d23_info.d23_semid, D23_SHMSEMNO) == D23_ERROR) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSYNC_ERR) ;
	return (d23_ipcerror (&rcr_pb->d23_Ohdinfo, D23_LOSEMA)) ;
    }

    /* search user credential entry */
    icontid = D23_SAC(contxt) | D23_SACM(acmode) | D23_SRMID(remid) ;
    usrcred = d23_info.d23_crdarea ;
    for (i = 0; i < d23_info.d23_genarea->d23_mcred; i++, usrcred++)
	if (usrcred->d23_credcontid == icontid &&
				   usrcred->d23_credusrid == rcr_pb->d23_Ousrid)
	    break ;
    if (i == d23_info.d23_genarea->d23_mcred) {
	/* user credential entry not found */
	d23_unlock (d23_info.d23_semid, D23_SHMSEMNO) ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
			      svc_c_sev_fatal, GDS_S_IPCSTATE_ERR, D23_UNUSED) ;
	return (d23_ipcerror (&rcr_pb->d23_Ohdinfo, D23_NOCRED)) ;
    }

    /* read user credential entry */
    memcpy ((void *) rcr_pb->d23_Ocredref,
	    (void *) ((char *) d23_info.d23_hparea + usrcred->d23_credoff),
	    (size_t) usrcred->d23_credlen) ;
    rcr_pb->d23_Ocredsize = usrcred->d23_credlen ;

    /* unlock shared memory */
    if (d23_unlock (d23_info.d23_semid, D23_SHMSEMNO) == D23_ERROR) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					   svc_c_sev_fatal, GDS_S_IPCSYNC_ERR) ;
	return (d23_ipcerror (&rcr_pb->d23_Ohdinfo, D23_ULSEMA)) ;
    }

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_IPC,
		svc_c_debug1, GDS_S_CSREADCRD_EXIT, rcr_pb->d23_Ocredsize, i)) ;
    return (D23_SUCES) ;
}
