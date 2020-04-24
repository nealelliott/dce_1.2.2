/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ndsusrmap.c,v $
 * Revision 1.1.185.2  1996/02/18  18:20:42  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:43:24  marty]
 *
 * Revision 1.1.185.1  1995/12/08  16:06:43  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:16:50  root]
 * 
 * Revision 1.1.183.4  1994/09/06  12:26:06  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:22:25  keutel]
 * 
 * Revision 1.1.183.3  1994/06/09  18:47:14  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:20:27  devsrc]
 * 
 * Revision 1.1.183.2  1994/05/10  15:57:28  marrek
 * 	April 1994 submission.
 * 	[1994/04/29  10:53:04  marrek]
 * 
 * Revision 1.1.183.1  1994/02/22  16:10:49  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:59:30  marrek]
 * 
 * Revision 1.1.181.2  1993/10/14  17:52:21  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:42:27  keutel]
 * 
 * Revision 1.1.181.1  1993/10/13  17:33:42  keutel
 * 	October 1993 code drop
 * 	[1993/10/09  16:51:17  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: ndsusrmap.c,v $ $Revision: 1.1.185.2 $ $Date: 1996/02/18 18:20:42 $";
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
/*   NAME:    nds_map_addrinfo ()                                            */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The function provides the mapping of OSI TSAP-addresses from a      */
/*       standardized format (described by the structure 'T_adrdesc') to     */
/*       formats which fulfil the requirements of different transport        */
/*       service interfaces resp. provider (e.g. CMX(s), XTI, SOCKET) and    */
/*       vice versa. Since in some cases OSI TSAP-address information must   */
/*       be combined with local informations the mapping is done by means    */
/*       of a NDS configuration data base. The use of a data base makes      */
/*       the mapping configurable and thus very flexible.                    */
/*                                                                           */
/*   INPUT-PARAMETER:                                                        */
/*       mp_type     = function mapping type                                 */
/*                     NDS_OSITOXTI:                                         */
/*                     map one (two) OSI TSAP-address(es) (belonging to a    */
/*                     local TSAP (and to a remote TSAP)) into the local     */
/*                     format defined by the corresponding XTI-provider.     */
/*                                                                           */
/*                     NDS_XTITOOSI:                                         */
/*                     map one (two) TSAP-address(es) (available in the      */
/*                     local format of a specific XTI-provider and belonging */
/*                     to a local TSAP (and to a remote TSAP)) into an       */
/*                     OSI TSAP-address format.                              */
/*                                                                           */
/*                     NDS_CMXSEL:                                           */
/*                     assign the information of one OSI TSAP-address        */
/*                     (mainly the OSI NSAP-address(es)) to the correspon-   */
/*                     ding interfaces (CMX(S), XTI, SOCKET).                */
/*                     select from two OSI TSAP-addresses a pair of NSAP-    */
/*                     addresses belonging to the same transport provider    */
/*                     (selection can be done on first match or on provider  */
/*                     priority).                                            */
/*                                                                           */
/*                     NDS_OSITOSOCK:                                        */
/*                     map OSI TSAP-address into the local format defined    */
/*                     by the socket-interface.                              */
/*                                                                           */
/*                     NDS_SOCKTOOSI:                                        */
/*                     map TSAP-address (available in the local format       */
/*                     defined by the socket-interface) into an OSI TSAP-    */
/*                     address.                                              */
/*                                                                           */
/*                     NDS_XTISEL:                                           */
/*                     determine no. of XTI-service provider supported       */
/*                     by an OSI TSAP-address.                               */
/*                                                                           */
/*                     NDS_IPTOOSI:                                          */
/*                     map IP-address and T-selector into an OSI TSAP-       */
/*                     address.                                              */
/*                                                                           */
/*       mp_info     = mainly OSI TSAP-address(es)                           */
/*                     (dependent on function mapping type)                  */
/*                                                                           */
/*   OUTPUT-PARAMETER:                                                       */
/*       mp_info     = mainly mapped TSAP-address information                */
/*                     (dependent on function mapping type)                  */
/*                                                                           */
/*   RETURN-VALUE:                                                           */
/*       NDS_NOERR   = The function has been performed successfully.         */
/*       NDS_ERR     = A system error or an internal error is occurred       */
/*                     during execution of the function (in this case the    */
/*                     return parameter nds_errno contains more information).*/
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                        DATE: 27.5.93     */
/*exoff **********************************************************************/

#include <gds.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <xti.h>
#include <dce/cmx_addr.h>
#include <ndsif.h>
#include <ndscfg.h>

/* ******************* DEFINITION OF CONSTANTS ***************************** */

#define IDX_MODE	1
#define PROVNM_MODE	2
#define ALL_MODE	3

/* ******************* DECLARATION OF GLOBAL DATA ************************** */

extern Nds_shmgenarea *nds_shm_base ;		/* reference to NDS-con- */
						/* figuration data base */

/* ******************* DECLARATION OF LOCAL DATA *************************** */

						/* interface-ID table */
static int ifid[] = { NDS_CMXSINFO, NDS_SOCKETINFO, NDS_XTIINFO } ;

/* ------------------------------------------------------------------------- */

int nds_map_addrinfo (const int   mp_type,
		      Nds_mapinfo *mp_info) {

    T_adrdesc		*osi1ref, *osi2ref, *sel1ref, *sel1href,
			*sel2ref, *sel2href, *ifbsref[NDS_MX_IF],
			*ifbshref[NDS_MX_IF] ;
    Nds_shmifdesc	*ifdesc = (Nds_shmifdesc *) NULL ;
    Nds_shmruledesc	*rdref ;
    Nds_shmrule		*rubsref, *rumxref ;
    int			retval = NDS_NOERR ;
    signed32		i, j, op_mode, ifmsk, nsapmcnt,
			provid[NDS_MX_PROVIDER] ;

    switch (mp_type) {
    case NDS_OSITOXTI:
        DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%b"), GDS_S_CMX,
		   		svc_c_debug3, GDS_S_NDS_FRTSAP_ENTRY, mp_type,
				mp_info->nds_mp1.nds_osi1addr->t_translng,
				(byte *) mp_info->nds_mp1.nds_osi1addr)) ;
	if (mp_info->nds_mp1.nds_osi2addr != (T_adrdesc *) NULL) {
            DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%b"), GDS_S_CMX,
		   		svc_c_debug3, GDS_S_NDS_TOTSAP_ENTRY, mp_type,
				mp_info->nds_mp1.nds_osi2addr->t_translng,
				(byte *) mp_info->nds_mp1.nds_osi2addr)) ;
	}
	/* verify availability of NDS-configuration data base */
	if ((mp_info->nds_mp1.nds_errno = nds_shmattach ()) < 0)
	    return (NDS_ERR) ;

	/* verify first OSI-TSAP-address */
	if ((mp_info->nds_mp1.nds_errno =
		      nds_check_tsap (mp_info->nds_mp1.nds_osi1addr, TRUE)) < 0)
	    return (NDS_ERR) ;
	/* verify address output buffer size */
	if (mp_info->nds_mp1.nds_xti1addr.maxlen < T_TSAPSIZE) {
	    mp_info->nds_mp1.nds_errno = NDS_BUFERR ;
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_CMX,
			svc_c_sev_warning, GDS_S_NDS_BUF_ERR,
			mp_info->nds_mp1.nds_xti1addr.maxlen, T_TSAPSIZE) ;
	    return (NDS_ERR) ;
	}

	if (mp_info->nds_mp1.nds_osi2addr == (T_adrdesc *) NULL) {
	    /* only one OSI-address is supplied -> address mapping is done as */
	    /* described by parameters 'nds_xtiprovnm' and 'nds_xtiprovidx' */

	    /* verify service provider parameter */
	    if (mp_info->nds_mp1.nds_xtiprovidx < 0 ||
	        mp_info->nds_mp1.nds_xtiprovidx > nds_shm_base->nds_noifdesc ||
	       (mp_info->nds_mp1.nds_xtiprovidx > 0 &&
			     mp_info->nds_mp1.nds_xtiprovnm != (char *) NULL)) {
		mp_info->nds_mp1.nds_errno = NDS_PROVERR ;
		dce_svc_printf (DCE_SVC (gds_svc_handle, "%s%d"), GDS_S_CMX,
			svc_c_sev_warning, GDS_S_NDS_PROV_ERR,
			(mp_info->nds_mp1.nds_xtiprovnm != (char *) NULL) ?
			mp_info->nds_mp1.nds_xtiprovnm : "",
			mp_info->nds_mp1.nds_xtiprovidx) ;
		return (NDS_ERR) ;
	    }

	    /* initialize access to shared memory information */
	    if (!nds_init_match_nsap (NDS_OSITOXTI, &rdref)) {
		mp_info->nds_mp1.nds_errno = NDS_NORULESERR ;
		return (NDS_ERR) ;
	    }
	    op_mode = (mp_info->nds_mp1.nds_xtiprovidx > 0) ? IDX_MODE :
		      (mp_info->nds_mp1.nds_xtiprovnm != (char *) NULL) ?
							PROVNM_MODE : ALL_MODE ;

	    if (op_mode != IDX_MODE) {
		/* operation mode not supported */
		mp_info->nds_mp1.nds_errno = NDS_PROVERR ;
		dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_CMX,
			     svc_c_sev_warning, GDS_S_NDS_OPMODE_ERR, op_mode) ;
		return (NDS_ERR) ;
	    }

	    /* search all NSAP-addresses which are associated to the same */
	    /* XTI-service provider */
	    osi1ref = (T_adrdesc *) ((char *) mp_info->nds_mp1.nds_osi1addr +
				     mp_info->nds_mp1.nds_osi1addr->t_nextoff) ;
	    sel1ref = sel1href = (T_adrdesc *) mp_info->nds_mp1.nds_xti1addr.buf ;
	    nsapmcnt = 0 ;
	    memset ((void *) provid, (int) -1, (size_t) sizeof (provid)) ;

	    rumxref = (Nds_shmrule *) SHMREF(rdref->nds_mrules) +
							   rdref->nds_nomrules ;
	    do {
		rubsref = (Nds_shmrule *) SHMREF(rdref->nds_mrules) ;
		do {
		    if (nds_match_nsap (osi1ref, NDS_MATCH, &ifdesc, &rubsref,
							(Nds_options *) NULL)) {
			if (ifdesc->nds_ifid != NDS_XTIINFO) {
			    /* NSAP-address doesn't match to any XTI-provider */
			    rubsref++ ;
			    continue ;
			}
			/* each provider should be referenced only one time */
			for (j = 0; j < nsapmcnt; j++)
			    if (ifdesc->nds_provid == provid[j])
				break ;
			if (j == nsapmcnt)
			    provid[nsapmcnt++] = ifdesc->nds_provid ;
			if (mp_info->nds_mp1.nds_xtiprovidx != nsapmcnt) {
			    /* NSAP-address doesn't match to requested index */
			    rubsref++ ;
			    continue ;
			}
			/* copy T-selector to XTI-address result area */
			memcpy ((void *) sel1ref,
				(void *) mp_info->nds_mp1.nds_osi1addr,
			    (size_t) mp_info->nds_mp1.nds_osi1addr->t_nextoff) ;
			sel1ref = (T_adrdesc *) ((char *) sel1ref +
				     mp_info->nds_mp1.nds_osi1addr->t_nextoff) ;

			/* copy NSAP-address to XTI-address result area */
			memcpy ((void *) sel1ref, (void *) osi1ref,
						  (size_t) osi1ref->t_nextoff) ;
			sel1ref = (T_adrdesc *) ((char *) sel1ref +
							   osi1ref->t_nextoff) ;
			break ;
		    }
		} while (rubsref < rumxref) ;
	    } while (mp_info->nds_mp1.nds_xtiprovidx != nsapmcnt &&
		     (osi1ref = (T_adrdesc *) ((osi1ref->t_translng >
		      osi1ref->t_nextoff) ? ((char *) osi1ref +
			   osi1ref->t_nextoff) : NULL)) != (T_adrdesc *) NULL) ;
	    if (mp_info->nds_mp1.nds_xtiprovidx != nsapmcnt) {
		/* no NSAP-address assigned to requested index */
	        mp_info->nds_mp1.nds_errno = NDS_NOSELERR ;
		dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
					 svc_c_sev_warning, GDS_S_NDS_SEL_ERR) ;
	        return (NDS_ERR) ;
	    }

	    /* adjust transfer size values */
	    while (sel1href < sel1ref) {
		sel1href->t_translng = (short) ((char *) sel1ref -
							    (char *) sel1href) ;
		sel1href = (T_adrdesc *) ((char *) sel1href +
							  sel1href->t_nextoff) ;
	    }
	    /* map XTI-address information into XTI-service provider specific */
	    /* format */
	    if ((mp_info->nds_mp1.nds_errno =
		nds_map_ositoxti (ifdesc->nds_provid,
			     &mp_info->nds_mp1.nds_xti1addr,
			     (struct netbuf *) NULL,
			     (Nds_options *) SHMREF(rubsref->nds_mprule),
			     (Nds_options *) SHMREF(ifdesc->nds_profopt))) < 0)
		return (NDS_ERR) ;
	} else {
	    /* both OSI-addresses are supplied -> select from both the */
	    /* NSAP-address(es) associated to the same XTI-service provider */
	    /* verify second OSI-TSAP-address */
	    if ((mp_info->nds_mp1.nds_errno =
		     nds_check_tsap (mp_info->nds_mp1.nds_osi2addr, FALSE)) < 0)
		return (NDS_ERR) ;
	    /* verify address output buffer size */
	    if (mp_info->nds_mp1.nds_xti2addr.maxlen < T_TSAPSIZE) {
		mp_info->nds_mp1.nds_errno = NDS_BUFERR ;
		dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_CMX,
			svc_c_sev_warning, GDS_S_NDS_BUF_ERR,
			mp_info->nds_mp1.nds_xti2addr.maxlen, T_TSAPSIZE) ;
		return (NDS_ERR) ;
	    }

	    /* verify mapping rule parameter */
	    if (mp_info->nds_mp1.nds_mprule != NDS_FMATCH &&
				     mp_info->nds_mp1.nds_mprule != NDS_HPRIO) {
		mp_info->nds_mp1.nds_errno = NDS_RULEPARERR ;
		dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_CMX,
					svc_c_sev_warning, GDS_S_NDS_MAP_ERR,
					mp_info->nds_mp1.nds_mprule) ;
		return (NDS_ERR) ;
	    }
	    /* initialize access to shared memory information */
	    if (!nds_init_match_nsap (NDS_OSITOXTI, &rdref)) {
		mp_info->nds_mp1.nds_errno = NDS_NORULESERR ;
		return (NDS_ERR) ;
	    }
	    osi1ref = mp_info->nds_mp1.nds_osi1addr ;
	    sel1ref = (T_adrdesc *) mp_info->nds_mp1.nds_xti1addr.buf ;
	    if (osi1ref->t_infotype == T_TSAPINFO) {
	        /* copy T-selector to mapped address information area */
	        memcpy ((void *) sel1ref, (void *) osi1ref,
						  (size_t) osi1ref->t_nextoff) ;

	        sel1ref = (T_adrdesc *) ((char *) sel1ref + sel1ref->t_nextoff) ;
	        osi1ref = (T_adrdesc *) ((char *) osi1ref + osi1ref->t_nextoff) ;
	    }
	    
	    osi2ref = mp_info->nds_mp1.nds_osi2addr ;
	    sel2ref = (T_adrdesc *) mp_info->nds_mp1.nds_xti2addr.buf ;
	    if (osi2ref->t_infotype == T_TSAPINFO) {
	        /* copy T-selector to mapped address information area */
	        memcpy ((void *) sel2ref, (void *) osi2ref,
						  (size_t) osi2ref->t_nextoff) ;

	        sel2ref = (T_adrdesc *) ((char *) sel2ref + sel2ref->t_nextoff) ;
	        osi2ref = (T_adrdesc *) ((char *) osi2ref + osi2ref->t_nextoff) ;
	    }

	    /* select OSI-NSAP-addresses */
	    if (!nds_select_nsap (osi1ref, osi2ref, &sel1ref, &sel2ref,
			      mp_info->nds_mp1.nds_mprule, &ifdesc, &rubsref)) {
		mp_info->nds_mp1.nds_errno = NDS_NOSELERR ;
		dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
					 svc_c_sev_warning, GDS_S_NDS_SEL_ERR) ;
		return (NDS_ERR) ;
	    }

	    if (ifdesc->nds_ifid != NDS_XTIINFO) {
		/* selected NSAP-addresses are not associated to a XTI- */
		/* service provider */
		mp_info->nds_mp1.nds_errno = NDS_NOSELERR ;
		dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
					 svc_c_sev_warning, GDS_S_NDS_SEL_ERR) ;
		return (NDS_ERR) ;
	    }

	    /* adjust transfer size values */
	    sel1href = (T_adrdesc *) mp_info->nds_mp1.nds_xti1addr.buf ;
	    while (sel1href < sel1ref) {
		sel1href->t_translng = (short) ((char *) sel1ref -
							    (char *) sel1href) ;
		sel1href = (T_adrdesc *) ((char *) sel1href +
							  sel1href->t_nextoff) ;
	    }
	    sel2href = (T_adrdesc *) mp_info->nds_mp1.nds_xti2addr.buf ;
	    while (sel2href < sel2ref) {
		sel2href->t_translng = (short) ((char *) sel2ref -
							    (char *) sel2href) ;
		sel2href = (T_adrdesc *) ((char *) sel2href +
							  sel2href->t_nextoff) ;
	    }
	    /* map XTI-address information into XTI-service provider specific */
	    /* format */
	    if ((mp_info->nds_mp1.nds_errno =
		nds_map_ositoxti (ifdesc->nds_provid,
			      &mp_info->nds_mp1.nds_xti1addr,
			      &mp_info->nds_mp1.nds_xti2addr,
			      (Nds_options *) SHMREF(rubsref->nds_mprule),
			      (Nds_options *) SHMREF(ifdesc->nds_profopt))) < 0)
		return (NDS_ERR) ;
	}
	/* supply XTI-service provider name */
	mp_info->nds_mp1.nds_xtiprovnm = (char *) SHMREF (ifdesc->nds_devname) ;

        DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%b"), GDS_S_CMX,
		   		svc_c_debug3, GDS_S_NDS_PROVNM_EXIT, mp_type,
				strlen (mp_info->nds_mp1.nds_xtiprovnm),
				(byte *) mp_info->nds_mp1.nds_xtiprovnm)) ;
        DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%b"), GDS_S_CMX,
		   		svc_c_debug3, GDS_S_NDS_FRTSAP_EXIT, mp_type,
				mp_info->nds_mp1.nds_xti1addr.len,
				(byte *) mp_info->nds_mp1.nds_xti1addr.buf)) ;
	if (mp_info->nds_mp1.nds_osi2addr != (T_adrdesc *) NULL) {
            DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%b"), GDS_S_CMX,
		   		svc_c_debug3, GDS_S_NDS_TOTSAP_EXIT, mp_type,
				mp_info->nds_mp1.nds_xti2addr.len,
				(byte *) mp_info->nds_mp1.nds_xti2addr.buf)) ;
	}
	break ;
    case NDS_XTITOOSI:
	/* no address mapping is done in this version */
	memcpy ((void *) mp_info->nds_mp1.nds_osi1addr,
		(void *) mp_info->nds_mp1.nds_xti1addr.buf,
		(size_t) mp_info->nds_mp1.nds_xti1addr.len) ;
	break ;
    case NDS_CMXSEL:
        DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%b"), GDS_S_CMX,
		   		svc_c_debug3, GDS_S_NDS_FRTSAP_ENTRY, mp_type,
				mp_info->nds_mp2.nds_osi1addr->t_translng,
				(byte *) mp_info->nds_mp2.nds_osi1addr)) ;
	if (mp_info->nds_mp2.nds_osi2addr != (T_adrdesc *) NULL) {
            DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%b"), GDS_S_CMX,
		   		svc_c_debug3, GDS_S_NDS_TOTSAP_ENTRY, mp_type,
				mp_info->nds_mp2.nds_osi2addr->t_translng,
				(byte *) mp_info->nds_mp2.nds_osi2addr)) ;
	}
	/* verify availability of NDS-configuration data base */
	if ((mp_info->nds_mp2.nds_errno = nds_shmattach ()) < 0)
	    return (NDS_ERR) ;

	/* verify first OSI-TSAP-address */
	if ((mp_info->nds_mp2.nds_errno =
		       nds_check_tsap (mp_info->nds_mp2.nds_osi1addr, TRUE)) < 0)
	    return (NDS_ERR) ;

	if (mp_info->nds_mp2.nds_osi2addr == (T_adrdesc *) NULL) {
	    /* only one OSI-address is supplied -> generate list of */
	    /* interface specific OSI-address information */

	    /* initialize access to shared memory information */
	    if (!nds_init_match_nsap (NDS_CMXSEL, &rdref)) {
		mp_info->nds_mp2.nds_errno = NDS_NORULESERR ;
		return (NDS_ERR) ;
	    }
	    /* assign NSAP-addresses to the different interfaces */
	    mp_info->nds_mp2.nds_iftypes = 0 ;
	    sel1ref = sel1href = mp_info->nds_mp2.nds_sel1addr ;
	    osi1ref = (T_adrdesc *) ((char *) mp_info->nds_mp2.nds_osi1addr +
				     mp_info->nds_mp2.nds_osi1addr->t_nextoff) ;
	    rumxref = (Nds_shmrule *) SHMREF(rdref->nds_mrules) +
							   rdref->nds_nomrules ;
	    for (i = 0; i < NDS_MX_IF; i++)
		ifbsref[i] = ifbshref[i] = (T_adrdesc *) ((char *) sel1ref +
							     (T_PSAPSIZE * i)) ;
	    do {
		rubsref = (Nds_shmrule *) SHMREF(rdref->nds_mrules) ;
	        ifmsk = 0 ;
		do {
		    if (nds_match_nsap (osi1ref, NDS_MATCH, &ifdesc, &rubsref,
							(Nds_options *) NULL)) {
			rubsref++ ;
			if (ifmsk & ifdesc->nds_ifid)
			    /* NSAP-address already assigned to interface */
			    continue ;
			ifmsk |= ifdesc->nds_ifid ;

			for (i = 0; i < NDS_MX_IF; i++)
			    if (ifid[i] == ifdesc->nds_ifid)
				break ;
			if (ifbsref[i] == ifbshref[i]) {
			    /* set interface-identifier */
			    mp_info->nds_mp2.nds_iftypes |= ifdesc->nds_ifid ;

			    /* copy T-selector to the list of interface */
			    /* specific OSI-address information */
			    memcpy ((void *) ifbshref[i],
				    (void *) mp_info->nds_mp2.nds_osi1addr,
			    (size_t) mp_info->nds_mp2.nds_osi1addr->t_nextoff) ;
			    ifbshref[i] = (T_adrdesc *) ((char *) ifbshref[i] +
				     mp_info->nds_mp2.nds_osi1addr->t_nextoff) ;
			}
			/* copy NSAP-address to the list of interface specific */
			/* OSI-address information */
		 	memcpy ((void *) ifbshref[i], (void *) osi1ref,
						  (size_t) osi1ref->t_nextoff) ;
			ifbshref[i] = (T_adrdesc *) ((char *) ifbshref[i] +
							   osi1ref->t_nextoff) ;
		    }
		} while (rubsref < rumxref) ;
	    } while ((osi1ref = (T_adrdesc *)
			((osi1ref->t_translng > osi1ref->t_nextoff) ?
			((char *) osi1ref + osi1ref->t_nextoff) : NULL)) !=
							   (T_adrdesc *) NULL) ;
	    if (mp_info->nds_mp2.nds_iftypes == 0) {
		/* no NSAP-address can be assigned to any interface */
		mp_info->nds_mp2.nds_errno = NDS_NOSELERR ;
		dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
					 svc_c_sev_warning, GDS_S_NDS_SEL_ERR) ;
		return (NDS_ERR) ;
	    }
	    /* adjust transfer size values */
	    sel1ref = mp_info->nds_mp2.nds_sel1addr ;
	    for (i = 0; i < NDS_MX_IF; i++) {
		if (ifbsref[i] != ifbshref[i]) {
		    /* interface specific information available */
		    if (sel1ref != ifbsref[i])
			memcpy ((void *) sel1ref, (void *) ifbsref[i],
			(size_t) ((char *) ifbshref[i] - (char *) ifbsref[i])) ;
		    sel1href = (T_adrdesc *) ((char *) sel1ref +
				 ((char *) ifbshref[i] - (char *) ifbsref[i])) ;
		    while (sel1ref < sel1href) {
			sel1ref->t_translng = (short) ((char *) sel1href -
							     (char *) sel1ref) ;
			sel1ref = (T_adrdesc *) ((char *) sel1ref +
							   sel1ref->t_nextoff) ;
		    }
		}
	    }
            DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%b"), GDS_S_CMX,
		   		svc_c_debug3, GDS_S_NDS_FRTSAP_EXIT, mp_type,
				(signed32) ((char *) sel1href -
				(char *) mp_info->nds_mp2.nds_sel1addr),
				(byte *) mp_info->nds_mp2.nds_sel1addr)) ;
	} else {
	    /* both OSI-addresses are supplied -> select from both the */
	    /* NSAP-address(es) belonging to the same interface */
	    /* verify second OSI-TSAP-address */
	    if ((mp_info->nds_mp2.nds_errno =
		     nds_check_tsap (mp_info->nds_mp2.nds_osi2addr, FALSE)) < 0)
		return (NDS_ERR) ;

	    /* verify mapping rule parameter */
	    if (mp_info->nds_mp2.nds_mprule != NDS_FMATCH &&
				     mp_info->nds_mp2.nds_mprule != NDS_HPRIO) {
		mp_info->nds_mp2.nds_errno = NDS_RULEPARERR ;
		dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_CMX,
					svc_c_sev_warning, GDS_S_NDS_MAP_ERR,
					mp_info->nds_mp2.nds_mprule) ;
		return (NDS_ERR) ;
	    }
	    /* initialize access to shared memory information */
	    if (!nds_init_match_nsap (NDS_CMXSEL, &rdref)) {
		mp_info->nds_mp2.nds_errno = NDS_NORULESERR ;
		return (NDS_ERR) ;
	    }
	    osi1ref = (T_adrdesc *) ((char *) mp_info->nds_mp2.nds_osi1addr +
				     mp_info->nds_mp2.nds_osi1addr->t_nextoff) ;
	    sel1ref = (T_adrdesc *) ((char *) mp_info->nds_mp2.nds_sel1addr +
				     mp_info->nds_mp2.nds_osi1addr->t_nextoff) ;
	    if (mp_info->nds_mp2.nds_osi2addr->t_infotype == T_TSAPINFO) {
		osi2ref = (T_adrdesc *) ((char *) mp_info->nds_mp2.nds_osi2addr +
				     mp_info->nds_mp2.nds_osi2addr->t_nextoff) ;
		sel2ref = (T_adrdesc *) ((char *) mp_info->nds_mp2.nds_sel2addr +
				     mp_info->nds_mp2.nds_osi2addr->t_nextoff) ;
	    } else {
		osi2ref = mp_info->nds_mp2.nds_osi2addr ;
		sel2ref = mp_info->nds_mp2.nds_sel2addr ;
	    }
	    /* select OSI-NSAP-addresses */
	    if (!nds_select_nsap (osi1ref, osi2ref, &sel1ref, &sel2ref,
			      mp_info->nds_mp2.nds_mprule, &ifdesc, &rubsref)) {
		mp_info->nds_mp2.nds_errno = NDS_NOSELERR ;
		return (NDS_ERR) ;
	    }

	    /* NSAP-address selection possible -> copy T-selectors to */
	    /* the mapped address information and adjust transfer size values */
	    sel1href = mp_info->nds_mp2.nds_sel1addr ;
	    memcpy ((void *) sel1href, (void *) mp_info->nds_mp2.nds_osi1addr,
			    (size_t) mp_info->nds_mp2.nds_osi1addr->t_nextoff) ;
	    while (sel1href < sel1ref) {
		sel1href->t_translng = (short) ((char *) sel1ref -
							    (char *) sel1href) ;
		sel1href = (T_adrdesc *) ((char *) sel1href +
							  sel1href->t_nextoff) ;
	    }
	    sel2href = mp_info->nds_mp2.nds_sel2addr ;
	    if (mp_info->nds_mp2.nds_osi2addr->t_infotype == T_TSAPINFO) {
		memcpy ((void *) sel2href, (void *) mp_info->nds_mp2.nds_osi2addr,
			    (size_t) mp_info->nds_mp2.nds_osi2addr->t_nextoff) ;
	    }	    
	    while (sel2href < sel2ref) {
		sel2href->t_translng = (short) ((char *) sel2ref -
							    (char *) sel2href) ;
		sel2href = (T_adrdesc *) ((char *) sel2href +
							  sel2href->t_nextoff) ;
	    }
	    /* supply interface-identifier */
	    mp_info->nds_mp2.nds_iftypes = ifdesc->nds_ifid ;

            DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%b"), GDS_S_CMX,
		   		svc_c_debug3, GDS_S_NDS_FRTSAP_EXIT, mp_type,
				mp_info->nds_mp2.nds_sel1addr->t_translng,
				(byte *) mp_info->nds_mp2.nds_sel1addr)) ;
            DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%b"), GDS_S_CMX,
		   		svc_c_debug3, GDS_S_NDS_TOTSAP_EXIT, mp_type,
				mp_info->nds_mp2.nds_sel2addr->t_translng,
				(byte *) mp_info->nds_mp2.nds_sel2addr)) ;
	}
	break ;
    case NDS_OSITOSOCK:
	osi1ref = sel1href = mp_info->nds_mp3.nds_osiaddr ;
        DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%b"), GDS_S_CMX,
		   		svc_c_debug3, GDS_S_NDS_TSAP_ENTRY, mp_type,
				osi1ref->t_translng, (byte *) osi1ref)) ;

	/* verify availability of NDS-configuration data base */
	if ((mp_info->nds_mp3.nds_errno = nds_shmattach ()) < 0)
	    return (NDS_ERR) ;

	/* verify OSI-TSAP-address */
	if ((mp_info->nds_mp3.nds_errno = nds_check_tsap (osi1ref, FALSE)) < 0)
	    return (NDS_ERR) ;

	if (osi1ref->t_infotype == T_TSAPINFO) {
	    /* skip T-selector */
	    osi1ref = (T_adrdesc *) ((char *) mp_info->nds_mp3.nds_osiaddr +
				      mp_info->nds_mp3.nds_osiaddr->t_nextoff) ;
	}

	/* verify NSAP-address */
	if (osi1ref->t_translng != osi1ref->t_nextoff) {
	    /* ambiguous NSAP-address */
	    mp_info->nds_mp3.nds_errno = NDS_NSAPERR ;
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_CMX,
				 svc_c_sev_warning, GDS_S_NDS_NSAP_ERR1, 2, 1) ;
	    return (NDS_ERR) ;
	}
	/* initialize access to shared memory information */
	if (!nds_init_match_nsap (NDS_OSITOSOCK, &rdref)) {
	    mp_info->nds_mp3.nds_errno = NDS_NORULESERR ;
	    return (NDS_ERR) ;
	}
	/* search socket interface description entry */
	rumxref = (Nds_shmrule *) SHMREF(rdref->nds_mrules) +
							   rdref->nds_nomrules ;
	rubsref = (Nds_shmrule *) SHMREF(rdref->nds_mrules) ;
	do {
	    if (nds_match_nsap (osi1ref, NDS_MATCH, &ifdesc, &rubsref,
							(Nds_options *) NULL)) {
		if (ifdesc->nds_ifid == NDS_SOCKETINFO)
		    break ;
	        rubsref++ ;
	    }
	} while (rubsref < rumxref) ;

	if (rubsref == rumxref) {
	    /* can't assign NSAP-address to socket interface */
	    mp_info->nds_mp3.nds_errno = NDS_NOSELERR ;
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
					 svc_c_sev_warning, GDS_S_NDS_SEL_ERR) ;
	    return (NDS_ERR) ;
	}
	/* supply T-selector */
	if (sel1href->t_infotype == T_TSAPINFO) {
	    mp_info->nds_mp3.nds_ositselln = sel1href->t_infolng ;
	    memcpy ((void *) mp_info->nds_mp3.nds_ositsel,
		    (void *) (sel1href + 1), (size_t) sel1href->t_infolng) ;
	} else
	    mp_info->nds_mp3.nds_ositselln = 0 ;

	/* supply internet-address and port no. */
	if ((mp_info->nds_mp3.nds_errno =
	    nds_map_ositosock (mp_info->nds_mp3.nds_osiaddr,
		   	      &mp_info->nds_mp3.nds_sockaddr,
			      (Nds_options *) SHMREF(rubsref->nds_mprule))) < 0)
	    return (NDS_ERR) ;

        DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%b"), GDS_S_CMX,
		   		svc_c_debug3, GDS_S_NDS_TSEL_EXIT, mp_type,
				mp_info->nds_mp3.nds_ositselln,
				(byte *) mp_info->nds_mp3.nds_ositsel)) ;
        DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%b"), GDS_S_CMX,
		   		svc_c_debug3, GDS_S_NDS_IP_EXIT, mp_type,
				sizeof (struct sockaddr_in),
				(byte *) &mp_info->nds_mp3.nds_sockaddr)) ;
	break ;
    case NDS_SOCKTOOSI:
	/* generate OSI-address */
	/* supply T-selector */
	osi1ref = mp_info->nds_mp3.nds_osiaddr ;
	osi1ref->t_infotype = T_TSAPINFO ;
	osi1ref->t_infovers = T_TSELVERS ;
	osi1ref->t_infolng = mp_info->nds_mp3.nds_ositselln ;
	osi1ref->t_translng =
	osi1ref->t_nextoff = sizeof (T_adrdesc) + T_ALIGN4(osi1ref->t_infolng) ;
	memcpy ((void *) (osi1ref + 1), (void *) mp_info->nds_mp3.nds_ositsel,
						  (size_t) osi1ref->t_infolng) ;

	/* supply NSAP-address */
	/* SNPA/NSAP-mapping is not supported in this version */
	break ;
    case NDS_XTISEL:
        DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%b"), GDS_S_CMX,
		   		svc_c_debug3, GDS_S_NDS_FRTSAP_ENTRY, mp_type,
				mp_info->nds_mp4.nds_osiaddr->t_translng,
				(byte *) mp_info->nds_mp4.nds_osiaddr)) ;

	/* verify availability of NDS-configuration data base */
	if ((mp_info->nds_mp4.nds_errno = nds_shmattach ()) < 0)
	    return (NDS_ERR) ;

	/* verify OSI-TSAP-address */
	if ((mp_info->nds_mp4.nds_errno =
		       nds_check_tsap (mp_info->nds_mp4.nds_osiaddr, TRUE)) < 0)
	    return (NDS_ERR) ;

	/* initialize access to shared memory information */
	if (!nds_init_match_nsap (NDS_XTISEL, &rdref)) {
	    mp_info->nds_mp4.nds_errno = NDS_NORULESERR ;
	    return (NDS_ERR) ;
	}

	/* determine # of NSAP-addresses which are associated to a XTI- */
	/* service provider */
	osi1ref = (T_adrdesc *) ((char *) mp_info->nds_mp4.nds_osiaddr +
				      mp_info->nds_mp4.nds_osiaddr->t_nextoff) ;
	rumxref = (Nds_shmrule *) SHMREF(rdref->nds_mrules) +
							   rdref->nds_nomrules ;
	mp_info->nds_mp4.nds_noxtiprov = 0 ;
	nsapmcnt = 0 ;    
	memset ((void *) provid, (int) -1, (size_t) sizeof (provid)) ;
	do {
	    rubsref = (Nds_shmrule *) SHMREF(rdref->nds_mrules) ;
	    do {
		if (nds_match_nsap (osi1ref, NDS_MATCH, &ifdesc, &rubsref,
							(Nds_options *) NULL)) {
		    rubsref++ ;
		    if (ifdesc->nds_ifid != NDS_XTIINFO)
			continue ;
		    /* each provider should be referenced only one time */
		    for (j = 0; j < nsapmcnt; j++)
			if (ifdesc->nds_provid == provid[j])
			    break ;
		    if (j == nsapmcnt)
		        provid[nsapmcnt++] = ifdesc->nds_provid ;
		}
	    } while (rubsref < rumxref) ;
	} while ((osi1ref = (T_adrdesc *)
		((osi1ref->t_translng > osi1ref->t_nextoff) ?
		((char *) osi1ref + osi1ref->t_nextoff) : NULL)) !=
							   (T_adrdesc *) NULL) ;
	if (nsapmcnt == 0) {
	    /* no NSAP-address assigned to any XTI-service provider */
	    mp_info->nds_mp4.nds_errno = NDS_NOSELERR ;
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
					 svc_c_sev_warning, GDS_S_NDS_SEL_ERR) ;
	    return (NDS_ERR) ;
	}
	mp_info->nds_mp4.nds_noxtiprov += nsapmcnt ;
        DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_CMX,
		   		svc_c_debug3, GDS_S_NDS_PROVSEL_EXIT, mp_type,
				mp_info->nds_mp4.nds_noxtiprov)) ;
	break ;
    case NDS_IPTOOSI:
    default:
	/* unsupported resp. unknown mapping type */
	retval = NDS_FATAL ;
	break ;
    }
    return (retval) ;
}
