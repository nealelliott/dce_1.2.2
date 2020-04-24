/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d23ipcstat.c,v $
 * Revision 1.1.871.2  1996/02/18  18:19:43  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:42:29  marty]
 *
 * Revision 1.1.871.1  1995/12/08  16:03:28  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:15:10  root]
 * 
 * Revision 1.1.869.6  1994/09/06  12:25:27  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:15:40  keutel]
 * 
 * Revision 1.1.869.5  1994/07/06  15:07:41  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:16:56  marrek]
 * 
 * Revision 1.1.869.4  1994/06/21  14:47:27  marrek
 * 	June 1994 code submission.
 * 	[1994/06/17  12:59:37  marrek]
 * 
 * Revision 1.1.869.3  1994/05/10  15:56:31  marrek
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/05  06:41:59  marrek]
 * 
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/02  13:23:38  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:40:21  marrek]
 * 
 * Revision 1.1.869.2  1994/03/23  15:18:56  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:31:51  keutel]
 * 
 * Revision 1.1.869.1  1994/02/22  17:40:40  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:53:08  marrek]
 * 
 * Revision 1.1.867.2  1993/10/14  17:35:54  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:33:51  keutel]
 * 
 * Revision 1.1.867.1  1993/10/13  17:33:11  keutel
 * 	October 1993 code drop
 * 	[1993/10/11  08:55:54  keutel]
 * 
 * Revision 1.1.7.2  1993/08/11  12:13:07  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:26:37  marrek]
 * 
 * Revision 1.1.5.3  1992/12/31  20:45:20  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:32:21  bbelch]
 * 
 * Revision 1.1.5.2  1992/12/17  23:13:40  tom
 * 	Bug 6411 - Add setlocale call for i18n.
 * 	[1992/12/17  23:08:54  tom]
 * 
 * Revision 1.1.2.2  1992/06/01  20:54:48  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:40:59  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d23ipcstat.c,v $ $Revision: 1.1.871.2 $ $Date: 1996/02/18 18:19:43 $";
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
/*   NAME:    d23ipcstat                                                     */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The program displays the actual (state) information contained       */
/*       in the shared memory area of a directory system installation.       */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                        DATE: 17.5.91     */
/*datoff *********************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <locale.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <dce/dcesvcmsg.h>
#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <d23ipc.h>


/* -------------------- DECLARATION OF GLOBAL DATA ------------------------- */

extern D23_iprocinfo	d23_info ;	/* process specific IPC-information */
					/* variable is used to suppress */
					/* standard prolog in serviceability */
extern signed32		d23_svcprolog ;
/*
 * Prototyping
 */
int main (signed32 argc, char *argv[]);

/* ------------------------------------------------------------------------- */

main (signed32 argc, char *argv[]) {

    signed32		i, maxidx ;
    error_status_t	svc_result ;
    D23_distcmd		*distptr ;
    D23_ipcentry	*svrptr, *cltptr ;
    D23_usrcred		*credptr ;
    D23_reginfo		*regptr ;
    D23_ipcinfo		*ipcinfo ;
    D23_extinfo		*extptr ;
    struct msqid_ds	msginfo ;

    setlocale(LC_ALL, "");

    /* initialize serviceability */
    d23_svcprolog = svc_c_action_brief ;
    d27_001_svc_init (argc, argv, &svc_result) ;
    if (svc_result != svc_s_ok) {
	fprintf (stderr, "%s: Unable to initialize serviceability, status: 0x%lx\n",    
							  argv[0], svc_result) ;
	exit (1) ;
    }

    /* get IPC-resources (shared memory) */
    if (d23_get_ipc () == D23_ERROR) {
	exit (1) ;
    }
    ipcinfo = d23_info.d23_genarea ;

    /* get message queue information */
    if (msgctl (d23_info.d23_msgid, IPC_STAT, &msginfo) == -1) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
			   svc_c_sev_fatal, GDS_S_MSGCTL_ERR, IPC_STAT, errno) ;
	exit (1) ;
    }

    /* get general IPC-information from shared memory */
    dce_fprintf (stdout, GDS_S_IPCSTAT_GHEAD) ;
    dce_fprintf (stdout, GDS_S_IPCSTAT_GVERS, ipcinfo->d23_versno) ;
    dce_fprintf (stdout, GDS_S_IPCSTAT_GCTIME,
		        (char *) asctime (localtime (&ipcinfo->d23_creatime))) ;
    dce_fprintf (stdout, GDS_S_IPCSTAT_GINVID, ipcinfo->d23_linvid) ;
    dce_fprintf (stdout, GDS_S_IPCSTAT_GVPID, ipcinfo->d23_lvpid) ;
    dce_fprintf (stdout, GDS_S_IPCSTAT_GMXDIST, ipcinfo->d23_mdno) ;
    dce_fprintf (stdout, GDS_S_IPCSTAT_GNODIST, ipcinfo->d23_dno) ;
    dce_fprintf (stdout, GDS_S_IPCSTAT_GMXREG, ipcinfo->d23_mrno) ;
    dce_fprintf (stdout, GDS_S_IPCSTAT_GNOREG, ipcinfo->d23_rno) ;
    dce_fprintf (stdout, GDS_S_IPCSTAT_GMXSVR, ipcinfo->d23_msno) ;
    dce_fprintf (stdout, GDS_S_IPCSTAT_GNOSVR, ipcinfo->d23_sno) ;
    dce_fprintf (stdout, GDS_S_IPCSTAT_GMXCLT, ipcinfo->d23_mcno) ;
    dce_fprintf (stdout, GDS_S_IPCSTAT_GNOCLT, ipcinfo->d23_cno) ;
    dce_fprintf (stdout, GDS_S_IPCSTAT_GMXBUF, ipcinfo->d23_bno) ;
    dce_fprintf (stdout, GDS_S_IPCSTAT_GBUFSZ, ipcinfo->d23_bsize/1024) ;
    dce_fprintf (stdout, GDS_S_IPCSTAT_GMXCRD, ipcinfo->d23_mcred) ;
    dce_fprintf (stdout, GDS_S_IPCSTAT_GNOCRD, ipcinfo->d23_cred) ;
    dce_fprintf (stdout, GDS_S_IPCSTAT_GMXHPB, ipcinfo->d23_mhpno) ;
    dce_fprintf (stdout, GDS_S_IPCSTAT_GHPBSTAT) ;
    for (maxidx = D23_MXHPBUF / D23_FLDSIZE;
		maxidx > 1 && ipcinfo->d23_hpstate[maxidx - 1] == 0; maxidx--) ;
	
    for (i = 1; i <= maxidx; i++) {
	dce_fprintf (stdout, GDS_S_IPCSTAT_GHPBSTAT1,
						  ipcinfo->d23_hpstate[i - 1]) ;
	if ((i % 6) == 0)
	    dce_fprintf (stdout, GDS_S_IPCSTAT_GHPBSTAT2) ;
    }
    if (i < D23_MXHPBUF / D23_FLDSIZE)
	dce_fprintf (stdout, GDS_S_IPCSTAT_GHPBSTAT3) ;
    if (ipcinfo->d23_mext > 0) {
	dce_fprintf (stdout, GDS_S_IPCSTAT_GNOEXT, ipcinfo->d23_mext) ;
    }
    dce_fprintf (stdout, GDS_S_IPCSTAT_GNOQUEUE, msginfo.msg_qnum) ;
    dce_fprintf (stdout, GDS_S_IPCSTAT_GDISTOFF, ipcinfo->d23_darea) ;
    dce_fprintf (stdout, GDS_S_IPCSTAT_GREGOFF, ipcinfo->d23_rarea) ;
    dce_fprintf (stdout, GDS_S_IPCSTAT_GSVROFF, ipcinfo->d23_sarea) ;
    dce_fprintf (stdout, GDS_S_IPCSTAT_GCLTOFF, ipcinfo->d23_carea) ;
    dce_fprintf (stdout, GDS_S_IPCSTAT_GBUFOFF, ipcinfo->d23_barea) ;
    dce_fprintf (stdout, GDS_S_IPCSTAT_GCRDOFF, ipcinfo->d23_crarea) ;
    dce_fprintf (stdout, GDS_S_IPCSTAT_GHPBOFF, ipcinfo->d23_hparea) ;
    if (ipcinfo->d23_mext > 0) {
	dce_fprintf (stdout, GDS_S_IPCSTAT_GEXTDOFF, ipcinfo->d23_exarea) ;
	for (i = 0, extptr = d23_info.d23_extarea; i < ipcinfo->d23_mext;
								i++, extptr++) {
	    dce_fprintf (stdout, GDS_S_IPCSTAT_GEXTOFF, i + 1,
							   extptr->d23_extoff) ;
 	}
	extptr-- ;
	dce_fprintf (stdout, GDS_S_IPCSTAT_GSHMSZ, (extptr->d23_extoff +
						    extptr->d23_extsize)/1024) ;
    } else {
	dce_fprintf (stdout, GDS_S_IPCSTAT_GSHMSZ, (ipcinfo->d23_hparea +
			  (ipcinfo->d23_mhpno * sizeof (D23_hpbufinfo)))/1024) ;
    }

    /* get distributed command information */
    dce_fprintf (stdout, GDS_S_IPCSTAT_DHEAD) ;
    for (i = 0, distptr = d23_info.d23_dstarea;
					i < ipcinfo->d23_mdno; i++, distptr++) {
	if (distptr->d23_dcmd != D23_UNUSED) {
	    dce_fprintf (stdout, GDS_S_IPCSTAT_DCMD, distptr->d23_dcmd) ;
	    dce_fprintf (stdout, GDS_S_IPCSTAT_DRPID, distptr->d23_dcpid) ;
	    dce_fprintf (stdout, GDS_S_IPCSTAT_DRSID, distptr->d23_dsv_prid) ;
	    dce_fprintf (stdout, GDS_S_IPCSTAT_DTSTMP, distptr->d23_dtstamp) ;
	    dce_fprintf (stdout, GDS_S_IPCSTAT_DNORQSVR, distptr->d23_disvrno) ;
	    dce_fprintf (stdout, GDS_S_IPCSTAT_DNORSSVR, distptr->d23_dconfno) ;
	    dce_fprintf (stdout, GDS_S_IPCSTAT_DTMOUT, distptr->d23_dtout) ;
	    if (distptr->d23_dparsize > 0) {
		dce_fprintf (stdout, GDS_S_IPCSTAT_DPAR, distptr->d23_dparsize) ;
		d27_printmsg (stdout, (byte *) ((char *) d23_info.d23_hparea +
				    distptr->d23_dpar), distptr->d23_dparsize) ;
	    }
	}
    }

    /* get IPC-registration information */
    dce_fprintf (stdout, GDS_S_IPCSTAT_RHEAD) ;
    for (i = 0, regptr = d23_info.d23_regarea;
					 i < ipcinfo->d23_mrno; i++, regptr++) {
	if (regptr->d23_pregtype != D23_UNUSED) {
	    dce_fprintf (stdout, GDS_S_IPCSTAT_RINFO, i + 1,
				regptr->d23_pregtype, regptr->d23_pdirid,
				regptr->d23_procid, regptr->d23_pstate) ;
	}
    }

    /* get server specific information from shared memory */
    dce_fprintf (stdout, GDS_S_IPCSTAT_SHEAD) ;
    for (i = 0, svrptr = d23_info.d23_svrarea;
					 i < ipcinfo->d23_msno; i++, svrptr++) {
	if (D23_GES(svrptr->d23_estate) == D23_ATTACH) {
	    dce_fprintf (stdout, GDS_S_IPCSTAT_SSTAT, i+1, svrptr->d23_estate) ;
	    switch (D23_GEOS(svrptr->d23_estate)) {
	    case D23_INVPEND:
		dce_fprintf (stdout, GDS_S_IPCSTAT_CSSTAT2) ;
		break ;
	    case D23_RBINVPEND:
		dce_fprintf (stdout, GDS_S_IPCSTAT_CSSTAT3) ;
		break ;
	    case D23_REQPEND:	
		dce_fprintf (stdout, GDS_S_IPCSTAT_CSSTAT4) ;
		break ;
	    case D23_RESPEND:
		dce_fprintf (stdout, GDS_S_IPCSTAT_CSSTAT5) ;
		break ;
	    case D23_RBRESPEND:
		dce_fprintf (stdout, GDS_S_IPCSTAT_CSSTAT6) ;
		break ;
	    default:
		dce_fprintf (stdout, GDS_S_IPCSTAT_CSSTAT1) ;
		break ;
	    }
	    dce_fprintf (stdout, GDS_S_IPCSTAT_SSUBSTAT, svrptr->d23_ucstate) ;
	    dce_fprintf (stdout, GDS_S_IPCSTAT_SPID, svrptr->d23_epid.d23_rpid,
						    svrptr->d23_epid.d23_vpid) ;
	    dce_fprintf (stdout, GDS_S_IPCSTAT_SSID, svrptr->d23_esid) ;
	    if (svrptr->d23_cassid != D23_UNUSED)
		dce_fprintf (stdout, GDS_S_IPCSTAT_SASSID, svrptr->d23_cassid) ;
	    if (svrptr->d23_cpid.d23_rpid != D23_UNUSED)
		dce_fprintf (stdout, GDS_S_IPCSTAT_SCPID,
		         svrptr->d23_cpid.d23_rpid, svrptr->d23_cpid.d23_vpid) ;
	    if (svrptr->d23_invinfo.d23_invid != D23_UNUSED)
		dce_fprintf (stdout, GDS_S_IPCSTAT_CSINVID,
					svrptr->d23_invinfo.d23_invid,
					svrptr->d23_invinfo.d23_usrid,
					svrptr->d23_invinfo.d23_contid) ;
	}
    }

    /* get client specific information from shared memory */
    dce_fprintf (stdout, GDS_S_IPCSTAT_CHEAD) ;
    for (i = 0, cltptr = d23_info.d23_cltarea;
					 i < ipcinfo->d23_mcno; i++, cltptr++) {
	if (D23_GES(cltptr->d23_estate) == D23_ATTACH) {
	    dce_fprintf (stdout, GDS_S_IPCSTAT_CSTAT, i+1, cltptr->d23_estate) ;
	    switch (D23_GEOS(cltptr->d23_estate)) {
	    case D23_INVPEND:
		dce_fprintf (stdout, GDS_S_IPCSTAT_CSSTAT2) ;
		break ;
	    case D23_RBINVPEND:
		dce_fprintf (stdout, GDS_S_IPCSTAT_CSSTAT3) ;
		break ;
	    case D23_REQPEND:	
		dce_fprintf (stdout, GDS_S_IPCSTAT_CSSTAT4) ;
		break ;
	    case D23_RESPEND:
		dce_fprintf (stdout, GDS_S_IPCSTAT_CSSTAT5) ;
		break ;
	    case D23_RBRESPEND:
		dce_fprintf (stdout, GDS_S_IPCSTAT_CSSTAT6) ;
		break ;
	    default:
		dce_fprintf (stdout, GDS_S_IPCSTAT_CSSTAT1) ;
		break ;
	    }
	    dce_fprintf (stdout, GDS_S_IPCSTAT_CSUBSTAT, cltptr->d23_ucstate) ;
	    dce_fprintf (stdout, GDS_S_IPCSTAT_CPID, cltptr->d23_epid.d23_rpid,
						    cltptr->d23_epid.d23_vpid) ;
	    dce_fprintf (stdout, GDS_S_IPCSTAT_CSID, cltptr->d23_esid) ;
	    if (cltptr->d23_cassid != D23_UNUSED)
		dce_fprintf (stdout, GDS_S_IPCSTAT_CASSID, cltptr->d23_cassid) ;
	    if (cltptr->d23_cpid.d23_rpid != D23_UNUSED)
		dce_fprintf (stdout, GDS_S_IPCSTAT_CSPID,
		         cltptr->d23_cpid.d23_rpid, cltptr->d23_cpid.d23_vpid) ;
	    if (cltptr->d23_invinfo.d23_invid != D23_UNUSED)
		dce_fprintf (stdout, GDS_S_IPCSTAT_CSINVID,
					cltptr->d23_invinfo.d23_invid,
					cltptr->d23_invinfo.d23_usrid,
					cltptr->d23_invinfo.d23_contid) ;
	}
    }

    /* get user credential information from shared memory */
    dce_fprintf (stdout, GDS_S_IPCSTAT_CRHEAD) ;
    for (i = 0, credptr = d23_info.d23_crdarea;
				       i < ipcinfo->d23_mcred; i++, credptr++) {
	if (credptr->d23_credusrid != D23_UNUSED) {
	    dce_fprintf (stdout, GDS_S_IPCSTAT_CRUSRID,
						       credptr->d23_credusrid) ;
	    dce_fprintf (stdout, GDS_S_IPCSTAT_CRCONID,
						      credptr->d23_credcontid) ;
	    dce_fprintf (stdout, GDS_S_IPCSTAT_CRSID, credptr->d23_credsvrid) ;
	    dce_fprintf (stdout, GDS_S_IPCSTAT_CRNAME) ;
	    d27_printmsg (stdout, (byte *) ((char *) d23_info.d23_hparea +
				  credptr->d23_credoff), credptr->d23_credlen) ;
	}
    }
    /* get ext. area description area information from shared memory */
    if (ipcinfo->d23_mext > 0) {
	dce_fprintf (stdout, GDS_S_IPCSTAT_EXTHEAD) ;
	for (i = 0, extptr = d23_info.d23_extarea; i < ipcinfo->d23_mext;
								i++, extptr++) {
	    dce_fprintf (stdout, GDS_S_IPCSTAT_EXTSTAT, i + 1,
							 extptr->d23_extstate) ;
	    dce_fprintf (stdout, GDS_S_IPCSTAT_EXTID, extptr->d23_extid) ;
	    dce_fprintf (stdout, GDS_S_IPCSTAT_EXTMNO, extptr->d23_mextno) ;
	    dce_fprintf (stdout, GDS_S_IPCSTAT_EXTANO, extptr->d23_aextno) ;
	    dce_fprintf (stdout, GDS_S_IPCSTAT_EXTSZ, extptr->d23_extsize) ;
	    dce_fprintf (stdout, GDS_S_IPCSTAT_EXTPRIV, extptr->d23_private) ;
	    dce_fprintf (stdout, GDS_S_IPCSTAT_EXTDATA) ;
	    d27_printmsg (stdout, (byte *) ((char *) d23_info.d23_genarea +
				     extptr->d23_extoff), extptr->d23_extsize) ;
	}
    }
    exit (0) ;
}
