/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ndsxtimap.c,v $
 * Revision 1.1.4.2  1996/02/18  18:20:25  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:43:04  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:06:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:16:31  root]
 * 
 * Revision 1.1.2.6  1994/09/06  12:25:54  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:17:28  keutel]
 * 
 * Revision 1.1.2.5  1994/07/06  15:07:54  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:17:58  marrek]
 * 
 * Revision 1.1.2.4  1994/06/21  14:47:43  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:43:06  marrek]
 * 
 * Revision 1.1.2.3  1994/06/09  18:46:59  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:20:06  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:57:18  marrek
 * 	Bug fixes.
 * 	[1994/04/29  12:01:57  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:42:58  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:26:14  keutel
 * 	creation
 * 	[1994/03/21  15:54:18  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: ndsxtimap.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:20:25 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton **********************************************************************/
/*                                                                           */
/*   TYPE:    module                                                         */
/*                                                                           */
/*   NAME:    ndsxtimap.c                                                    */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       This module contains all the functions used to map global OSI TSAP- */
/*       address information into the local address formats defined by the   */
/*       supported XTI-transport provider. This module must be modified      */
/*       resp. rewritten if there are changes in existing local address      */
/*       formats or if new local address formats are introduced.             */
/*                                                                           */
/*       The following functions are available:                              */
/*                                                                           */
/*          * nds_map_ositoxti   - mapping entry call function               */
/*          * nds_map_to_netbios - NETBIOS-provider mapping function         */
/*          * nds_map_tsel       - map T-selector information                */
/*          * nds_map_tpi        - map transport protocol-ID information     */
/*          * nds_map_tpc        - map transport protocol class information  */
/*          * nds_map_regproc    - map region/processor no. information      */
/*          * nds_map_osinsap    - map OSI-NSAP-address information          */
/*          * nds_map_snpa       - map SNPA information                      */
/*          * nds_map_mac        - map ethernet(MAC)-address information     */
/*          * nds_map_mac_mig    - map ethernet(MAC)-address information     */
/*                                 (migration format)                        */
/*          * nds_map_x121       - map X.121-address information             */
/*          * nds_map_e163       - map E.163-address information             */
/*          * nds_map_e164       - map E.164-address information             */
/*          * nds_map_x21        - map X.21-address information              */
/*          * nds_map_pvc        - map PVC-channel information               */
/*          * nds_map_ccinfo     - map CC-routing information                */
/*          * nds_map_ip	 - map IP-address information                */
/*          * nds_map_port       - map port no. information                  */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                        DATE: 27.5.93     */
/*datoff *********************************************************************/

#include <gds.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <xti.h>
#include <netinet/in.h>
#include <cmxaddr.h>
#include <dce/cmx_addr.h>
#include <dce/osiaddr.h>
#include <ndsif.h>
#include <ndscfg.h>

/* ******************* DEFINITION OF CONSTANTS ***************************** */

#define EBCDICSPACE		0x40
#define SPACE			0x20
#define NDS_DEFAULT_PORT	102	/* default RFC1006 port no. */

					/* Address Unit (AU)-types */
#define NDS_PROP_NSAP_AU	0x30
#define NDS_OSI_NSAP_AU		0x40
#define NDS_NEA_NSAP_AU		0x41
#define NDS_SNPA_AU		0x70
#define NDS_SNPA_FS_AU		0x80
#define NDS_PORT_AU		0x81
#define NDS_SNPA_SS_AU		0x40
#define NDS_TSEL_AU		0x80
#define NDS_CCINFO_AU		0x90
#define NDS_CCINFO_S_AU		0x91
#define NDS_TPI_AU		0xD0
#define NDS_TPC_AU		0xD1
					/* SNPA-address subtypes */
#define NDS_X121_SNPA		0x01
#define NDS_X21_SNPA		0x02
#define NDS_E164_SNPA		0x03
#define NDS_MAC_SNPA		0x04
#define NDS_PVC_SNPA		0x05
#define NDS_X31_SNPA		0x06
#define NDS_X32_SNPA		0x07
#define NDS_E163_SNPA		0x08

#define NDS_IGNORE_OPT		'-'

					/* NETBIOS-TP address info */
#define NB_NAMELEN	16		/* definition should be in xti.h */
#define NB_UNIQUE	0		/*		"		 */
#define NB_GROUP	1		/*		"		 */

/* ******************* DECLARATION OF LOCAL DATA *************************** */

					/* profile-ID to address type-ID */
					/* mapping table */
signed32 nds_profid[] = {
			CX_RFC1006,  CX_LANBSBKA, CX_LANSBKA, CX_WANSBKA,
			CX_WAN3SBKA, CX_WANNEA,   CX_LOOPSBKA } ;

/* ******************* PROTOTYPING OF LOCAL FUNCTIONS ********************** */

static signed32 nds_map_to_netbios (T_adrdesc *, T_adrdesc *, Nds_options *,
							      struct netbuf *) ;
static signed32 nds_map_tpi (T_adrdesc *, Nds_options *, byte *) ;
static signed32 nds_map_tsel (T_adrdesc	*, byte	*) ;
static signed32 nds_map_tpc (Nds_options *, byte *) ;
static signed32 nds_map_regproc (T_adrdesc *, Nds_options *, byte *) ;
static signed32 nds_map_osinsap (T_adrdesc *, byte *) ;
static signed32 nds_map_snpa (T_adrdesc	*, Nds_options *, Nds_options *,
								       byte *) ;
static signed32 nds_map_mac (T_adrdesc *, Nds_options *, byte *) ;
static signed32 nds_map_mac_mig (T_adrdesc *, Nds_options *, byte *) ;
static signed32 nds_map_x121 (signed32,	T_adrdesc *, Nds_options *,
					 Nds_options *, Nds_opt_str *, byte *) ;
static signed32 nds_map_e163 (T_adrdesc	*, Nds_options *, Nds_options *,
							Nds_opt_str *, byte *) ;
static signed32 nds_map_e164 (signed32,	T_adrdesc *, Nds_options *,
					 Nds_options *, Nds_opt_str *, byte *) ;
static signed32 nds_map_x21 (signed32, T_adrdesc *, Nds_opt_str *, byte *) ;
static signed32 nds_map_pvc (T_adrdesc *, Nds_opt_short *, byte *) ;
static signed32 nds_map_ccinfo (Nds_options *, byte *) ;
static signed32 nds_map_ip (T_adrdesc *, Nds_options *, byte *) ;
static signed32 nds_map_port (T_adrdesc *, T_adrdesc *, Nds_options *, byte *) ;

/* -------------------------------------------------------------------------- */

signed32
nds_map_ositoxti (signed32	profid,		/* >  profile-ID */
		  struct netbuf	*from,		/* >< from TSAP-address */
		  struct netbuf	*to,		/* >< to TSAP-address */
		  Nds_options	*optref,	/* >  specific options info */
		  Nds_options	*coptref) {	/* >  common options info */

    T_adrdesc		*fromtsel, *fromnsap, *totsel, *tonsap ;
    struct t_partaddr	cmxaddr ;
    struct t_myname	cmxname ;
    struct cx_myname	*cmxfix ;
    union cx_unisel	*cmxsel ;
    struct sbka_addr	*psbka ;	
    signed32		res ;

    fromtsel = (T_adrdesc *) from->buf ;
    if (fromtsel->t_infotype != T_TSAPINFO) {
	/* no T-selector available */
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_CMX,
				     svc_c_sev_warning, GDS_S_NDS_TSEL_ERR, 0) ;
	return (NDS_TSELERR) ;
    }
    fromnsap = (T_adrdesc *) ((char *) fromtsel + fromtsel->t_nextoff) ;

    if (to != (struct netbuf *) NULL) {
	totsel = (T_adrdesc *) to->buf ;
	if (totsel->t_infotype != T_TSAPINFO) {
	    tonsap = totsel ;
	    totsel = (T_adrdesc *) NULL ;
	} else
	    tonsap = (T_adrdesc *) ((char *) totsel + totsel->t_nextoff) ;
    }

    /* generate XTI(CMX)-like local name */
    if (profid != NDS_NETBIOS_PROF) {
	memset ((void *) &cmxname, (int) 0, (size_t) sizeof (struct t_myname)) ;
	cmxname.t_mnmode = T_MNMODE ;
	cmxname.t_mnlng = CXN_PRESIZE + CXN_FIXSIZE + sizeof (union cx_unisel) ;
	cmxfix = (struct cx_myname *) cmxname.t_mn ;
	cmxfix->cxn_selsize = sizeof (union cx_unisel) ;
	cmxfix->cxn_abbr[0] = cmxfix->cxn_abbr[1] = 0 ;
	cmxsel = (union cx_unisel *) cmxfix->cxn_sel ;
	cmxsel->cxs.cx_type = nds_profid[profid] ;
    }
    switch (profid) {
    case NDS_OSITYPE_PROF:
    case NDS_LANSBKA_PROF:
    case NDS_WANSBKA_PROF:
    case NDS_WAN3SBKA_PROF:
    case NDS_LOOPSBKA_PROF:
    case NDS_RFC1006_PROF:
	if (fromtsel->t_infolng > SBKA_SELSIZE) {
	    /* invalid size of T-selector */
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_CMX,
		   svc_c_sev_warning, GDS_S_NDS_TSEL_ERR, fromtsel->t_infolng) ;
	    return (NDS_TSELERR) ;
	}
	memcpy ((void *) cmxsel->cxs.cxs_rest, (void *) (fromtsel + 1),
						 (size_t) fromtsel->t_infolng) ;
	cmxsel->cxs.cxs_lng = fromtsel->t_infolng ;
	from->len = cmxname.t_mnlng ;
	memcpy ((void *) from->buf, (void *) &cmxname, (size_t) from->len) ;
	break ;
    case NDS_WANNEA_PROF:
	if (fromtsel->t_infolng > NEA_SELSIZE) {
	    /* invalid size of T-selector */
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_CMX,
		   svc_c_sev_warning, GDS_S_NDS_TSEL_ERR, fromtsel->t_infolng) ;
	    return (NDS_TSELERR) ;
	}
	cmxsel->cxs_nea.ns_size = NEA_SELSIZE ;
	memset ((void *) cmxsel->cxs_nea.ns_stname, (int) EBCDICSPACE,
							 (size_t) NEA_SELSIZE) ;
	memcpy ((void *)cmxsel->cxs_nea.ns_stname, (void *) (fromtsel + 1),
						 (size_t) fromtsel->t_infolng) ;
	cmxsel->cxs_nea.ns_mig = MIG_NO ;
	from->len = cmxname.t_mnlng ;
	memcpy ((void *) from->buf, (void *) &cmxname, (size_t) from->len) ;
	break ;
    case NDS_NETBIOS_PROF:
	if (nds_map_to_netbios (fromtsel, fromnsap, optref, from) != NDS_NOERR)
	    return (NDS_NSAPERR) ;
	break ;
    default:
	/* NSAP-address is assigned to an unknown profile */
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
				     svc_c_sev_warning, GDS_S_NDS_MAPNSAP_ERR) ;
	return (NDS_NSAPERR) ;
	break ;
    }
    if (to == (struct netbuf *) NULL)
	return (NDS_NOERR) ;


    /* generate XTI(CMX)-like transport address */
    if (profid != NDS_NETBIOS_PROF) {
	memset ((void *) &cmxaddr, (int) 0, (size_t) sizeof (struct t_partaddr)) ;
	cmxaddr.t_pamode = T_PAMODE ;
	psbka = (struct sbka_addr *) cmxaddr.t_pa ;
	psbka->ka_type = SBKA_TYPE ;
	psbka->cx_type = nds_profid[profid] ;
    }
    switch (profid) {
    case NDS_OSITYPE_PROF:
	/* set MAC-address */
	if ((res = nds_map_mac (tonsap, optref, (byte *) psbka->ka_rest)) < 0)
	    return (NDS_NSAPERR) ;
	psbka->ka_size = (unsigned short) res ;

	/* set OSI-NSAP-address */
	if ((res = nds_map_osinsap (tonsap,
				 (byte *) &psbka->ka_rest[psbka->ka_size])) < 0)
	    return (NDS_NSAPERR) ;
	psbka->ka_size += (unsigned short) res ;

	/* set T-selector */
	psbka->ka_size += nds_map_tsel (totsel,
				     (byte *) &psbka->ka_rest[psbka->ka_size]) ;

	/* set CC-routing information */	
	psbka->ka_size += (unsigned short) nds_map_ccinfo (optref,
				     (byte *) &psbka->ka_rest[psbka->ka_size]) ;
	break ;
    case NDS_LANSBKA_PROF:
	/* set subet-ID, MAC-address (migration format) */
	if ((res = nds_map_mac_mig (tonsap, optref,
						 (byte *) psbka->ka_rest)) <= 0)
	    return (NDS_NSAPERR) ;
	psbka->ka_size = (unsigned short) res ;

	/* set T-selector */
	psbka->ka_size += nds_map_tsel (totsel,
				 (byte *) &psbka->ka_rest[psbka->ka_size]) ;

	/* set CC-routing information */	
	psbka->ka_size += (unsigned short) nds_map_ccinfo (optref,
				     (byte *) &psbka->ka_rest[psbka->ka_size]) ;
	break ;
    case NDS_WANSBKA_PROF:
	/* set SNPA-address */
	if ((res = nds_map_snpa (tonsap, optref, coptref,
						  (byte *) psbka->ka_rest)) < 0)
	    return (NDS_NSAPERR) ;
	psbka->ka_size = (unsigned short) res ;

	/* set T-selector */
	psbka->ka_size += nds_map_tsel (totsel,
				     (byte *) &psbka->ka_rest[psbka->ka_size]) ;

	/* set CC-routing information */	
	psbka->ka_size += (unsigned short) nds_map_ccinfo (optref,
				     (byte *) &psbka->ka_rest[psbka->ka_size]) ;

	/* set TPI information */	
	if ((res = nds_map_tpi (tonsap, optref,
				 (byte *) &psbka->ka_rest[psbka->ka_size])) < 0)
	    return (NDS_NSAPERR) ;
	psbka->ka_size += (unsigned short) res ;

	/* set TPC information */	
	psbka->ka_size += (unsigned short) nds_map_tpc (optref,
				     (byte *) &psbka->ka_rest[psbka->ka_size]) ;
	break ;
    case NDS_WAN3SBKA_PROF:
	/* set SNPA-address */
	if ((res = nds_map_snpa (tonsap, optref, coptref,
						  (byte *) psbka->ka_rest)) < 0)
	    return (NDS_NSAPERR) ;
	psbka->ka_size = (unsigned short) res ;

	/* set CC-routing information */	
	psbka->ka_size += (unsigned short) nds_map_ccinfo (optref,
				     (byte *) &psbka->ka_rest[psbka->ka_size]) ;
	break ;
    case NDS_WANNEA_PROF:
	/* set region-/processor no. */
	if ((res = nds_map_regproc (tonsap, optref,
						  (byte *) psbka->ka_rest)) < 0)
	    return (NDS_NSAPERR) ;
	psbka->ka_size = (unsigned short) res ;

	/* set SNPA-address */
	if ((res = nds_map_snpa (tonsap, optref, coptref,
				 (byte *) &psbka->ka_rest[psbka->ka_size])) < 0)
	psbka->ka_size += (unsigned short) res ;

	/* set T-selector */
	psbka->ka_size += nds_map_tsel (totsel,
				     (byte *) &psbka->ka_rest[psbka->ka_size]) ;

	/* set CC-routing information */	
	psbka->ka_size += (unsigned short) nds_map_ccinfo (optref,
				     (byte *) &psbka->ka_rest[psbka->ka_size]) ;
	break ;
    case NDS_LOOPSBKA_PROF:
	if (totsel == (T_adrdesc *) NULL || totsel->t_infolng > SBKA_SELSIZE) {
	    /* no T-selector or invalid size of T-selector */
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_CMX,
		       svc_c_sev_warning, GDS_S_NDS_TSEL_ERR,
		       (totsel == (T_adrdesc *) NULL) ? 0 : totsel->t_infolng) ;
	    return (NDS_TSELERR) ;
	}
	/* set T-selector */
	psbka->ka_size = nds_map_tsel (totsel, (byte *) psbka->ka_rest) ;
	break ;
    case NDS_RFC1006_PROF:
	/* set IP-address */
	if ((res = nds_map_ip (tonsap, optref, (byte *) psbka->ka_rest)) < 0)
	    return (NDS_NSAPERR) ;
	psbka->ka_size = (unsigned short) res ;

	/* set port no. */
	if ((res = nds_map_port (totsel, tonsap, optref,
				 (byte *) &psbka->ka_rest[psbka->ka_size])) < 0)
	    return (NDS_NSAPERR) ;
	psbka->ka_size += (unsigned short) res ;

	/* set T-selector */
	psbka->ka_size += nds_map_tsel (totsel,
				     (byte *) &psbka->ka_rest[psbka->ka_size]) ;
	break ;
    case NDS_NETBIOS_PROF:
	if (totsel == (T_adrdesc *) NULL) {
	    /* no T-selector -> can't build netbios name */
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_CMX,
				     svc_c_sev_warning, GDS_S_NDS_TSEL_ERR, 0) ;
	    return (NDS_TSELERR) ;
	}
	if (nds_map_to_netbios (totsel, tonsap, optref, to) != NDS_NOERR)
	    return (NDS_NSAPERR) ;
	break ;
    default:
	break ;
    }
    if (profid != NDS_NETBIOS_PROF) {
	cmxaddr.t_palng = (short) (sizeof (struct t_partaddr) - T_PASIZE +
		   sizeof (struct sbka_addr) - SBKA_ADDRSIZE + psbka->ka_size) ;
	to->len = cmxaddr.t_palng ;
	memcpy ((void *) to->buf, (void *) &cmxaddr, (size_t) to->len) ;
    }
    return (NDS_NOERR) ;
}


/* ----------------------------------------------------------------------- */

static signed32
nds_map_to_netbios (T_adrdesc     *tsel,	/* >  T-selector */
		    T_adrdesc     *nsap,	/* >  NSAP-address */
		    Nds_options   *optref,	/* >  specific options info */
		    struct netbuf *xtiaddr) {	/* >< mapped NETBIOS-addr info */

    byte		nb_addr[NB_NAMELEN + 2], nbhost[MAX_DSP], *nbhostref ;
    Nds_opt_short	*nbunigrpopt, *nbhostopt ;
    signed32		nbunigrpid, nbhostlen, nsapdec = 0 ;

    memset ((void *) nb_addr, (int) SPACE, (size_t) sizeof (nb_addr)) ;

    (void) nds_get_option (NDS_UNI_GRP_OPT, optref, (void **) &nbunigrpopt) ;
    (void) nds_get_option (NDS_HOST_OPT, optref, (void **) &nbhostopt) ;

    if (nbunigrpopt->nds_opt_rep & NDS_IMP_MSK) {
	/* get netbios unique/group-id from NSAP-address */
	if (nds_get_imp_val (nsap, nbunigrpopt, (void *) &nbunigrpid,
					 	       (signed32 *) NULL) < 0) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
				      svc_c_sev_warning, GDS_S_NDS_NBGUID_ERR) ;
	    return (NDS_NSAPERR) ;
	}
	if (nbunigrpid != NB_UNIQUE && nbunigrpid != NB_GROUP) {
	    /* invalid unique/group-id value */
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_CMX,
				svc_c_sev_warning, GDS_S_NDS_NBGUIDV_ERR,
				nbunigrpid, NB_UNIQUE, NB_GROUP) ;
	    return (NDS_NSAPERR) ;
	}
	nsapdec = 1 ;
    } else
	nbunigrpid = (signed32) nbunigrpopt->nds_opt_shortval ;    
	
    if (nbhostopt->nds_opt_rep & NDS_IMP_MSK) {
	/* get netbios host name from NSAP-address */
	if (nds_get_imp_val ((nsapdec) ? (T_adrdesc *) NULL : nsap, nbhostopt,
					     (void *) nbhost, &nbhostlen) < 0) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
				      svc_c_sev_warning, GDS_S_NDS_NBNAME_ERR) ;
	    return (NDS_NSAPERR) ;
	}
	nbhostref = nbhost ;
    } else {
	/* get netbios host name from NDS-option */
	switch (nbhostopt->nds_opt_rep) {
	case NDS_EXP_ASCSTR:
	    nbhostref = (byte *) ((Nds_opt_str *) nbhostopt)->nds_opt_strval ;
	    nbhostlen = (signed32) strlen ((char *) nbhostref) ;
	    break ;
	case NDS_EXP_OCTSTR:
	    nbhostref = ((Nds_opt_oct *) nbhostopt)->nds_opt_octval ;
	    nbhostlen = ((Nds_opt_oct *) nbhostopt)->nds_opt_len ;
	    break ;
	default:
	    /* invalid representation type for host name */
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_CMX,
		      svc_c_sev_warning, GDS_S_NDS_NBNAMEV_ERR, 0, NB_NAMELEN) ;
	    return (NDS_NSAPERR) ;
	    break ;
	}
    }

    if (tsel->t_infolng + nbhostlen > NB_NAMELEN) {
	/* invalid size of netbios name */
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_CMX,
				svc_c_sev_warning, GDS_S_NDS_NBNAMEV_ERR,
				tsel->t_infolng + nbhostlen, NB_NAMELEN) ;
	return (NDS_NSAPERR) ;
    }

    /* set reserved field */
    nb_addr[NB_NAMELEN] = (byte) 0 ;

    /* set unique/group-id */
    nb_addr[NB_NAMELEN + 1] = (byte) nbunigrpid ;
    /* set netbios name (host name + service name) */
    memcpy ((void *) nb_addr, (void *) nbhostref, (size_t) nbhostlen) ;
    memcpy ((void *) &nb_addr[nbhostlen], (void *) (tsel + 1),
						     (size_t) tsel->t_infolng) ;

    xtiaddr->len = NB_NAMELEN + 2 ;
    memcpy ((void *) xtiaddr->buf, (void *) nb_addr, (size_t) xtiaddr->len) ;
    return (NDS_NOERR) ;
}

/* ----------------------------------------------------------------------- */

static signed32
nds_map_tsel (T_adrdesc	*tsel,		/* >  T-selector */
	      byte	*addrref) {	/* >< mapped T-selector info */

    /* set T-selector AU-type */
    *addrref++ = NDS_TSEL_AU ;
    if (tsel == (T_adrdesc *) NULL) {
	/* set T-selector size */
	*addrref = 0 ;
	return (2) ;
    }

    /* set T-selector size */
    *addrref++ = tsel->t_infolng ;
    /* set T-selector value */
    memcpy ((void *) addrref, (void *) (tsel + 1), (size_t) tsel->t_infolng) ;
    return (2 + tsel->t_infolng) ;
}

/* ----------------------------------------------------------------------- */

static signed32
nds_map_tpi (T_adrdesc     *nsap,		/* >  NSAP-address */
	     Nds_options   *optref,		/* >  specific options info */
	     byte 	   *addrref) {		/* >< mapped TPI info */

    Nds_opt_oct *tpiopt ;
    signed32	tpivallen ;
    char	tpival[MAX_DSP], *tpivalref ;

    if (nds_get_option (NDS_TPI_OPT, optref, (void **) &tpiopt) == 0) 
	/* no transport protocol identifier available */
	return (0) ;
    if (tpiopt->nds_opt_rep & NDS_IMP_MSK) {
	/* get transport protocol identifier from NSAP-address */
	if (nds_get_imp_val (nsap, (Nds_opt_short *) tpiopt, (void *) tpival,
							      &tpivallen) < 0) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
				        svc_c_sev_warning, GDS_S_NDS_TPID_ERR) ;
	    return (NDS_NSAPERR) ;
	}
	tpivalref = tpival ;
    } else {
	tpivalref = (char *) tpiopt->nds_opt_octval ;
	tpivallen = tpiopt->nds_opt_len ;
    }

    /* set TPI AU-type */
    *addrref++ = NDS_TPI_AU ;
    /* set TPI length */
    *addrref++ = (byte) tpivallen ;
    /* set TPI value */
    memcpy ((void *) addrref, (void *) tpivalref, (size_t) tpivallen) ;
    return (2 + tpivallen) ;
}

/* ----------------------------------------------------------------------- */

static signed32
nds_map_tpc (Nds_options *optref,	/* >  specific options info */
	     byte	 *addrref) {	/* >< mapped TPC info */

    Nds_opt_short *tpcopt ;

    if (nds_get_option (NDS_TPC_OPT, optref, (void **) &tpcopt) == 0)
	/* no transport protocol class information available */
	return (0) ;

    /* set TPC AU-type */
    *addrref++ = NDS_TPC_AU ;
    /* set TPC length */
    *addrref++ = 1 ;
    /* set preferred/alternative TPC values */
    *addrref = (byte) tpcopt->nds_opt_shortval ;

    return (3) ;
}

/* ----------------------------------------------------------------------- */

static signed32
nds_map_regproc (T_adrdesc	*nsap,		/* >  NSAP-address */
		 Nds_options	*optref,	/* >  specific options info */
		 byte		*addrref) {	/* >< mapped region-/proc.- */
						/*    no. info */

    Nds_opt_short *regopt, *procopt ;
    signed32	  regno, procno, nsapdec = 0 ;

    (void) nds_get_option (NDS_REG_OPT, optref, (void **) &regopt) ;
    (void) nds_get_option (NDS_PROC_OPT, optref, (void **) &procopt) ;

    if (regopt->nds_opt_rep & NDS_IMP_MSK) {
	/* get region number from NSAP-address */
	if (nds_get_imp_val (nsap, regopt, (void *) &regno,
						       (signed32 *) NULL) < 0) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
				      svc_c_sev_warning, GDS_S_NDS_REGION_ERR) ;
	    return (NDS_NSAPERR) ;
	}
	if (regno > 255) {
	    /* invalid region number value */
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_CMX,
		      svc_c_sev_warning, GDS_S_NDS_REGIONV_ERR, regno, 0, 255) ;
	    return (NDS_NSAPERR) ;
	}
	nsapdec = 1 ;
    } else
	regno = (signed32) regopt->nds_opt_shortval ;
	
    if (procopt->nds_opt_rep & NDS_IMP_MSK) {
	/* get processor number from NSAP-address */
	if (nds_get_imp_val ((nsapdec) ? (T_adrdesc *) NULL : nsap, procopt,
				     (void *) &procno, (signed32 *) NULL) < 0) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
				        svc_c_sev_warning, GDS_S_NDS_PROC_ERR) ;
	    return (NDS_NSAPERR) ;
	}
	if (procno > 255) {
	    /* invalid processor number value */
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_CMX,
		       svc_c_sev_warning, GDS_S_NDS_PROCV_ERR, procno, 0, 255) ;
	    return (NDS_NSAPERR) ;
	}
    } else 
	procno = (signed32) procopt->nds_opt_shortval ;

    /* set REGION/PROCESSOR AU-type */
    *addrref++ = NDS_NEA_NSAP_AU ;
    /* set REGION/PROCESSOR length */
    *addrref++ = 2 ;
    /* set REGION value */
    *addrref++ = (byte) regno ;
    /* set PROCESSOR value */
    *addrref = (byte) procno ;

    return (4) ;
}

/* ----------------------------------------------------------------------- */

static signed32
nds_map_osinsap (T_adrdesc	*nsap,		/* >  NSAP-address */
		 byte		*addrref) {	/* >< mapped NSAP-addr info */

    /* set OSI-NSAP AU-type */
    *addrref++ = NDS_OSI_NSAP_AU ;
    /* set OSI-NSAP length */
    *addrref++ = (byte) nsap->t_infolng ;
    /* set OSI-NSAP value */
    memcpy ((void *) addrref, (void *) (nsap + 1), (size_t) nsap->t_infolng) ;

    return (2 + nsap->t_infolng) ;
}

/* ----------------------------------------------------------------------- */

static signed32
nds_map_snpa (T_adrdesc	  *nsap,	/* >  NSAP-address */
	      Nds_options *optref,	/* >  specific options info */
	      Nds_options *coptref,	/* >  common options info */
	      byte	  *addrref) {	/* >< mapped SNPA-addr info */

    Nds_opt_str	  *x121ref, *x21ref, *e163ref, *e164ref ;
    Nds_opt_short *pvcref ;
    signed32	  cnt, stage = 0 ;
    byte	  *snpalen, *msalen ;

    /* set SNPA AU-type */
    *addrref++ = NDS_SNPA_AU ;
    /* set SNPA length */
    *(snpalen = addrref++) = 0 ;

    if (nds_get_option (NDS_E164_OPT, optref, (void **) &e164ref) > 0) {
	if (nds_get_option (NDS_X121_OPT, optref, (void **) &x121ref) > 0) {
	    /* E.164/X.121 SNPA-address */
	    /* set X.31 MSA-type */
	    *addrref++ = NDS_X31_SNPA ;
	    /* set MSA-length */
	    *(msalen = addrref++) = 0 ;
	    stage = NDS_SNPA_FS_AU ;	
	}
	if ((cnt = nds_map_e164 (stage, nsap, optref, coptref, e164ref,
								  addrref)) < 0)
	    return (NDS_NSAPERR) ;
	if (stage == NDS_SNPA_FS_AU) {
	    *msalen += (byte) cnt ;
	    stage = NDS_SNPA_SS_AU ;
	    if ((cnt = nds_map_x121 (stage, nsap, optref, coptref, x121ref,
								  addrref)) < 0)
		return (NDS_NSAPERR) ;
	    *msalen += (byte) cnt ;
	    *snpalen += (byte) (*msalen + 2) ;
	} else
	    *snpalen += (byte) cnt ;
	return (2 + *snpalen) ;
    }

    if (nds_get_option (NDS_X21_OPT, optref, (void **) &x21ref) > 0) {
	if (nds_get_option (NDS_X121_OPT, optref, (void **) &x121ref) > 0) {
	    /* X.121/X.21 SNPA-address */
	    /* set X.32 MSA-type */
	    *addrref++ = NDS_X32_SNPA ;
	    /* set MSA-length */
	    *(msalen = addrref++) = 0 ;
	    stage = NDS_SNPA_FS_AU ;	
	}
	if ((cnt = nds_map_x21 (stage, nsap, x21ref, addrref)) < 0)
	    return (NDS_NSAPERR) ;
	if (stage == NDS_SNPA_FS_AU) {
	    *msalen += (byte) cnt ;
	    stage = NDS_SNPA_SS_AU ;
	    if ((cnt = nds_map_x121 (stage, nsap, optref, coptref, x121ref,
								  addrref)) < 0)
		return (NDS_NSAPERR) ;
	    *msalen += (byte) cnt ;
	    *snpalen += (byte) (*msalen + 2) ;
	} else
	    *snpalen += (byte) cnt ;
	return (2 + *snpalen) ;
    }

    if (nds_get_option (NDS_X121_OPT, optref, (void **) &x121ref) > 0) {
	/* X.121 SNPA-address */
	if ((cnt = nds_map_x121 (stage, nsap, optref, coptref, x121ref,
								  addrref)) < 0)
	    return (NDS_NSAPERR) ;
	*snpalen += (byte) cnt ;
	return (2 + *snpalen) ;
    }

    if (nds_get_option (NDS_E163_OPT, optref, (void **) &e163ref) > 0) {
	/* E.163 SNPA-address */
	if ((cnt = nds_map_e163 (nsap, optref, coptref, e163ref, addrref)) < 0)
	    return (NDS_NSAPERR) ;
	*snpalen += (byte) cnt ;
	return (2 + *snpalen) ;
    }

    if (nds_get_option (NDS_PVC_OPT, optref, (void **) &pvcref) > 0) {
	/* PVC SNPA-address */
	if ((cnt = nds_map_pvc (nsap, pvcref, addrref)) < 0)
	    return (NDS_NSAPERR) ;
	*snpalen += (byte) cnt ;
	return (2 + *snpalen) ;
    }

    return (0) ;
}

/* ----------------------------------------------------------------------- */

static signed32
nds_map_mac (T_adrdesc	 *nsap,		/* >  NSAP-address */
	     Nds_options *optref,	/* >  specific options info */
	     byte	 *addrref) {	/* >< mapped MAC-addr info */

    Nds_opt_oct	  *macopt ;
    signed32	  macvallen ;
    char	  macval[MAX_DSP], *macvalref ;

    if (nds_get_option (NDS_MAC_OPT, optref, (void **) &macopt) > 0) {
        if (macopt->nds_opt_rep & NDS_IMP_MSK) {
	    /* get MAC-address from NSAP-address */
	    if (nds_get_imp_val (nsap, (Nds_opt_short *) macopt,
					     (void *) macval, &macvallen) < 0) {
		dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
					 svc_c_sev_warning, GDS_S_NDS_MAC_ERR) ;
		return (NDS_NSAPERR) ;
	    }
	    if (macvallen != 6) {
	        /* invalid size of MAC-address */
		dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_CMX,
			  svc_c_sev_warning, GDS_S_NDS_MACV_ERR, macvallen, 6) ;
	        return (NDS_NSAPERR) ;
	    }
	    macvalref = macval ;
        } else {
	    macvalref = (char *) macopt->nds_opt_octval ;
	    macvallen = macopt->nds_opt_len ;
	}

	/* set SNPA AU-type */
	*addrref++ = NDS_SNPA_AU ;
	/* set SNPA length */
	*addrref++ = (byte) (2 + macvallen) ;
	/* set MAC-SNPA type */
	*addrref++ = NDS_MAC_SNPA ;
	/* set MAC-address length */
	*addrref++ = (byte) macvallen ;
	/* set MAC-address value */
	memcpy ((void *) addrref, (void *) macvalref, (size_t) macvallen) ;

	return (4 + macvallen) ;
    } else
	return (0) ;
}

/* ----------------------------------------------------------------------- */

static signed32
nds_map_mac_mig (T_adrdesc   *nsap,		/* >  NSAP-address */
		 Nds_options *optref,		/* >  specific options info */
		 byte	     *addrref) {	/* >< mapped MAC-address info */
						/*    (migration format) */

    Nds_opt_oct	  *macopt ;
    Nds_opt_short *snidopt ;
    signed32	  macvallen, snidval ;

    /* set SNPA AU-type */
    *addrref++ = NDS_PROP_NSAP_AU ;
    /* set SNPA length */
    *addrref++ = (byte) 15 ;
    /* set IDP length */
    *addrref++ = (byte) 3 ;
    /* set IDP */
    *addrref++ = (byte) 0x49 ;
    *addrref++ = (byte) 0 ;
    *addrref++ = (byte) 0 ;
    /* set DSP type */
    *addrref++ = (byte) 0 ;
    /* set DSP length */
    *addrref++ = (byte) 9 ;

    /* set subnet-ID */
    if (nds_get_option (NDS_SNID_OPT, optref, (void **) &snidopt) > 0) {
	if (snidopt->nds_opt_rep & NDS_IMP_MSK) {
	    /* get subnet-ID from NSAP-address */
	    if (nds_get_imp_val (nsap, snidopt, (void *) &snidval,
						       (signed32 *) NULL) < 0) {
		dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
				       svc_c_sev_warning, GDS_S_NDS_SUBID_ERR) ;
		return (NDS_NSAPERR) ;
	    }
	} else
	    snidval = (signed32) snidopt->nds_opt_shortval ;
    } else
	snidval = 1 ;
    *((unsigned short *) addrref) = htons (snidval) ;
    addrref += sizeof (unsigned short) ;

    /* set MAC-address value */
    if (nds_get_option (NDS_MAC_OPT, optref, (void **) &macopt) > 0) {
        if (macopt->nds_opt_rep & NDS_IMP_MSK) {
	    /* get MAC-address from NSAP-address */
	    if (nds_get_imp_val (nsap, (Nds_opt_short *) macopt,
					    (void *) addrref, &macvallen) < 0) {
		dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
					 svc_c_sev_warning, GDS_S_NDS_MAC_ERR) ;
		return (NDS_NSAPERR) ;
	    }
	    if (macvallen != 6) {
	        /* invalid size of MAC-address */
		dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_CMX,
			  svc_c_sev_warning, GDS_S_NDS_MACV_ERR, macvallen, 6) ;
	        return (NDS_NSAPERR) ;
	    }
        } else {
	    memcpy ((void *) addrref, (void *) macopt->nds_opt_octval,
						 (size_t) macopt->nds_opt_len) ;
	}
	addrref += macvallen ;
    } else
	return (0) ;

    /* set N-selector */
    *addrref = (byte) 0xFE ;

    return (17) ;
}

/* ----------------------------------------------------------------------- */

static signed32
nds_map_x121 (signed32	  stage,	/* >  single/multi-stage flag */
	      T_adrdesc	  *nsap,	/* >  NSAP-address */
	      Nds_options *optref,	/* >  specific options info */
	      Nds_options *coptref,	/* >  common options info */
	      Nds_opt_str *x121opt,	/* >  X.121-addr option info */
	      byte	  *addrref) {	/* >< mapped X.121-addr info */

    Nds_options *optsrc ;
    Nds_opt_str	*intpopt, *dcc_dnicopt ;
    char	*intpval, *dcc_dnicval ;
    char	x121val[MAX_DSP], tmpval[MAX_DSP] ;
    signed32	i, octval, x121vallen, dcc_dnicvallen ;

    if (x121opt->nds_opt_rep & NDS_IMP_MSK) {
	/* get X.121 international prefix */
	optsrc = optref ;
	if (nds_get_option (NDS_X121_IP_OPT, optsrc, (void **) &intpopt) == 0) {
	    /* information not available -> try to get information from */
	    /* common option section */
	    optsrc = coptref ;
	    if (nds_get_option (NDS_X121_IP_OPT, optsrc,
						     (void **) &intpopt) == 0) {
		dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
				      svc_c_sev_warning, GDS_S_NDS_X121IP_ERR) ;
		return (NDS_NSAPERR) ;
	    }
	}
	intpval = intpopt->nds_opt_strval ;

	/* get X.121 DCC/DNIC */
	if (nds_get_option (NDS_X121_DCC_OPT, optsrc,
						   (void **) &dcc_dnicopt) == 0)
	    if (nds_get_option (NDS_X121_DNIC_OPT, optsrc,
						  (void *) &dcc_dnicopt) == 0) {
		dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
				     svc_c_sev_warning, GDS_S_NDS_X121DCC_ERR) ;
		return (NDS_NSAPERR) ;
	    }
	dcc_dnicval = dcc_dnicopt->nds_opt_strval ;
	dcc_dnicvallen = (signed32) strlen (dcc_dnicval) ;

	/* get X.121-address from NSAP-address */
	if (nds_get_imp_val (nsap, (Nds_opt_short *) x121opt, (void *) tmpval,
							     &x121vallen) < 0) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
					svc_c_sev_warning, GDS_S_NDS_X121_ERR) ;
	    return (NDS_NSAPERR) ;
	}
	if (x121vallen > 14 || x121vallen < dcc_dnicvallen + 1) {
	    /* invalid size of X.121-address */
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_CMX,
					svc_c_sev_warning, GDS_S_NDS_X121V_ERR,
					x121vallen, dcc_dnicvallen + 1, 14) ;
	    return (NDS_NSAPERR) ;
	}

	if (*dcc_dnicval != NDS_IGNORE_OPT &&
			   strncmp (tmpval, dcc_dnicval, dcc_dnicvallen) == 0) {
	    /* same country - remove DCC/DNIC */
	    strcpy (x121val, &tmpval[dcc_dnicvallen]) ;
	} else {
	    if (*intpval != NDS_IGNORE_OPT) {
		/* different country - add international prefix */
		strcpy (x121val, intpval) ;
		strcat (x121val, tmpval) ;
	    } else
		strcpy (x121val, tmpval) ;
	}
    } else
	strcpy (x121val, x121opt->nds_opt_strval) ;

    x121vallen = strlen (x121val) ;
    if ((x121vallen % 2) != 0) {
	strcat (x121val, "F") ;
	x121vallen++ ;
    }

    /* set X.121 SNPA-type */
    *addrref++ = (byte) (stage | NDS_X121_SNPA) ;
    /* set X.121 length */
    x121vallen /= 2 ;
    *addrref++ = (byte) x121vallen ;
    /* set X.121-address value */
    for (i = 0; i < x121vallen; i++) {
	sscanf (&x121val[i * 2], "%2x", &octval) ;
	*addrref++ = (byte) octval ;
    }
    return (2 + x121vallen) ;
}

/* ----------------------------------------------------------------------- */

static signed32
nds_map_e163 (T_adrdesc	  *nsap,	/* >  NSAP-address */
	      Nds_options *optref,	/* >  specific options info */
	      Nds_options *coptref,	/* >  common options info */
	      Nds_opt_str *e163opt,	/* >  E.163-addr option info */
	      byte	  *addrref) {	/* >< mapped E.163-addr info */

    Nds_options *optsrc ;
    Nds_opt_str	*intpopt, *ccopt, *npopt, *acopt ;
    char	*intpval, *ccval, *npval, *acval ;
    char	e163val[MAX_DSP], tmpval[MAX_DSP] ;
    signed32	i, octval, e163vallen, ccvallen, acvallen ;

    if (e163opt->nds_opt_rep & NDS_IMP_MSK) {
	/* get E.163 international prefix */
	optsrc = optref ;
	if (nds_get_option (NDS_E163_IP_OPT, optsrc, (void **) &intpopt) == 0) {
	    /* information not available -> try to get information from */
	    /* common option section */
	    optsrc = coptref ;
	    if (nds_get_option (NDS_E163_IP_OPT, optsrc,
						     (void **) &intpopt) == 0) {
		dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
				     svc_c_sev_warning, GDS_S_NDS_E163IP_ERR) ;
		return (NDS_NSAPERR) ;
	    }
	}
	intpval = intpopt->nds_opt_strval ;

	/* get E.163 country code */
	if (nds_get_option (NDS_E163_CC_OPT, optsrc, (void **) &ccopt) == 0) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
				      svc_c_sev_warning, GDS_S_NDS_E163CC_ERR) ;
	    return (NDS_NSAPERR) ;
	}
	ccval = ccopt->nds_opt_strval ;
	ccvallen = strlen (ccval) ;

	/* get E.163 national prefix */
	if (nds_get_option (NDS_E163_NP_OPT, optsrc, (void **) &npopt) == 0) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
				      svc_c_sev_warning, GDS_S_NDS_E163NP_ERR) ;
	    return (NDS_NSAPERR) ;
	}
	npval = npopt->nds_opt_strval ;

	/* get E.163 area code */
	if (nds_get_option (NDS_E163_AC_OPT, optsrc, (void **) &acopt) == 0) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
				      svc_c_sev_warning, GDS_S_NDS_E163AC_ERR) ;
	    return (NDS_NSAPERR) ;
	}
	acval = acopt->nds_opt_strval ;
	acvallen = strlen (acval) ;

	/* get E.163-address from NSAP-address */
	if (nds_get_imp_val (nsap, (Nds_opt_short *) e163opt, (void *) tmpval,
							     &e163vallen) < 0) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
				        svc_c_sev_warning, GDS_S_NDS_E163_ERR) ;
	    return (NDS_NSAPERR) ;
	}
	if (e163vallen > 12 || e163vallen < ccvallen + acvallen + 1) {
	    /* invalid size of E.163-address */
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_CMX,
				svc_c_sev_warning, GDS_S_NDS_E163V_ERR,
				e163vallen, ccvallen + acvallen + 1, 12) ;
	    return (NDS_NSAPERR) ;
	}

	if (*ccval != NDS_IGNORE_OPT &&
				       strncmp (tmpval, ccval, ccvallen) == 0) {
	    /* same country - remove country code */
	    if (*acval != NDS_IGNORE_OPT &&
			    strncmp (&tmpval[ccvallen], acval, acvallen) == 0) {
		/* same area - remove area code */
		strcpy (e163val, &tmpval[ccvallen + acvallen]) ;
	    } else {
		/* different area - add national prefix */
		if (*npval != NDS_IGNORE_OPT) {
		    strcpy (e163val, npval) ;
		    strcat (e163val, &tmpval[ccvallen]) ;
		} else
		    strcpy (e163val, &tmpval[ccvallen]) ;
	    }
	} else {
	    /* different country - add international prefix */
	    if (*intpval != NDS_IGNORE_OPT) {
		strcpy (e163val, intpval) ;
		strcat (e163val, tmpval) ;
	    } else
		strcpy (e163val, tmpval) ;
	}
    } else
	strcpy (e163val, e163opt->nds_opt_strval) ;

    e163vallen = strlen (e163val) ;
    if ((e163vallen % 2) != 0) {
	strcat (e163val, "F") ;
	e163vallen++ ;
    }

    /* set E.163 SNPA-type */
    *addrref++ = NDS_E163_SNPA ;
    /* set E.163 length */
    e163vallen /= 2 ;
    *addrref++ = (byte) e163vallen ;
    /* set E.163-address value */
    for (i = 0; i < e163vallen; i++) {
	sscanf (&e163val[i * 2], "%2x", &octval) ;
	*addrref++ = (byte) octval ;
    }
    return (2 + e163vallen) ;
}

/* ----------------------------------------------------------------------- */

static signed32
nds_map_e164 (signed32	  stage,	/* >  single/multi-stage flag */
	      T_adrdesc	  *nsap,	/* >  NSAP-address */
	      Nds_options *optref,	/* >  specific options info */
	      Nds_options *coptref,	/* >  common options info */
	      Nds_opt_str *e164opt,	/* >  E.164-addr option info */
	      byte	  *addrref) {	/* >< mapped E.164-addr info */

    Nds_options *optsrc ;
    Nds_opt_str	*intpopt, *ccopt, *ndcopt ;
    char	*intpval, *ccval, *ndcval ;
    char	e164val[MAX_DSP], tmpval[MAX_DSP] ;
    signed32	e164vallen, ccvallen, ndcvallen ;

    if (e164opt->nds_opt_rep & NDS_IMP_MSK) {
	/* get E.164 international prefix */
	optsrc = optref ;
	if (nds_get_option (NDS_E164_IP_OPT, optsrc, (void **) &intpopt) == 0) {
	    /* information not available -> try to get information from */
	    /* common option section */
	    optsrc = coptref ;
	    if (nds_get_option (NDS_E164_IP_OPT, optsrc,
						     (void **) &intpopt) == 0) {
		dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
				      svc_c_sev_warning, GDS_S_NDS_E164IP_ERR) ;
		return (NDS_NSAPERR) ;
	    }
	}
	intpval = intpopt->nds_opt_strval ;

	/* get E.164 CC */
	if (nds_get_option (NDS_E164_CC_OPT, optsrc, (void **) &ccopt) == 0) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
				      svc_c_sev_warning, GDS_S_NDS_E164CC_ERR) ;
	    return (NDS_NSAPERR) ;
	}
	ccval = ccopt->nds_opt_strval ;
	ccvallen = strlen (ccval) ;

	/* get E.164 NDC */
	if (nds_get_option (NDS_E164_NDC_OPT, optsrc, (void **) &ndcopt) == 0) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
				     svc_c_sev_warning, GDS_S_NDS_E164NDC_ERR) ;
	    return (NDS_NSAPERR) ;
	}
	ndcval = ndcopt->nds_opt_strval ;
	ndcvallen = strlen (ndcval) ;

	/* get E.164-address from NSAP-address */
	if (nds_get_imp_val (nsap, (Nds_opt_short *) e164opt, (void *) tmpval,
							     &e164vallen) < 0) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
					svc_c_sev_warning, GDS_S_NDS_E164_ERR) ;
	    return (NDS_NSAPERR) ;
	}
	if (e164vallen > 15 || e164vallen < ccvallen + ndcvallen + 1) {
	    /* invalid size of E.164-address */
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_CMX,
				svc_c_sev_warning, GDS_S_NDS_E164V_ERR,
				e164vallen, ccvallen + ndcvallen + 1, 15) ;
	    return (NDS_NSAPERR) ;
	}

	if (*ccval != NDS_IGNORE_OPT &&
				       strncmp (tmpval, ccval, ccvallen) == 0) {
	    /* same country - remove country code */
	    if (*ndcval != NDS_IGNORE_OPT &&
			    strncmp (&tmpval[ccvallen], ndcval, ndcvallen) == 0)
		/* same national destination - remove national destination code */
		strcpy (e164val, &tmpval[ccvallen + ndcvallen]) ;
	    else
		strcpy (e164val, &tmpval[ccvallen]) ;
	} else {
	    /* different country - add international prefix */
	    if (*intpval != NDS_IGNORE_OPT) {
		strcpy (e164val, intpval) ;
		strcat (e164val, tmpval) ;
	    } else
		strcpy (e164val, tmpval) ;
	}
    } else
	strcpy (e164val, e164opt->nds_opt_strval) ;

    /* set E.164 AU-type */
    *addrref++ = (byte) (stage | NDS_E164_SNPA) ;
    /* set E.164 length */
    e164vallen = strlen (e164val) ;
    *addrref++ = (byte) e164vallen ;
    /* set X.121-address value */
    strcpy ((char *) addrref, e164val) ;

    return (2 + e164vallen) ;
}

/* ----------------------------------------------------------------------- */

static signed32
nds_map_x21 (signed32	 stage,		/* >  single/multi-stage flag */
	     T_adrdesc   *nsap,		/* >  NSAP-address */
	     Nds_opt_str *x21opt,	/* >  X.21-addr option info */
	     byte	 *addrref) {	/* >< mapped X.21-addr info */

    char	x21val[MAX_DSP] ;
    signed32	x21vallen ;

    if (x21opt->nds_opt_rep & NDS_IMP_MSK) {
	/* get X.21-address from NSAP-address */
	if (nds_get_imp_val (nsap, (Nds_opt_short *) x21opt, (void *) x21val,
							      &x21vallen) < 0) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
					 svc_c_sev_warning, GDS_S_NDS_X21_ERR) ;
	    return (NDS_NSAPERR) ;
	}
	if (x21vallen > 17) {
	    /* invalid size of X.21-address */
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_CMX,
		      svc_c_sev_warning, GDS_S_NDS_X21V_ERR, x21vallen, 1, 17) ;
	    return (NDS_NSAPERR) ;
	}
    } else {
	strcpy (x21val, x21opt->nds_opt_strval) ;
	x21vallen = strlen (x21val) ;
    }

    /* set X.21 SNPA-type */
    *addrref++ = (byte) (stage | NDS_X21_SNPA) ;
    /* set X.21 length */
    *addrref++ = (byte) x21vallen ;
    /* set X.21-address value */
    strcpy ((char *) addrref, x21val) ;

    return (2 + x21vallen) ;
}

/* ----------------------------------------------------------------------- */

static signed32
nds_map_pvc (T_adrdesc     *nsap,	/* >  NSAP-address */
	     Nds_opt_short *pvcopt,	/* >  PVC-channel option info */
	     byte	   *addrref) {	/* >< mapped PVC-channel info */

    signed32	pvcval ;

    /* set PVC SNPA-type */
    *addrref++ = NDS_PVC_SNPA ;
    /* set PVC-value length */
    *addrref++ = 2 ;
    /* set PVC-value */
    if (pvcopt->nds_opt_rep & NDS_IMP_MSK) {
	/* get PVC-number from NSAP-address */
	if (nds_get_imp_val (nsap, pvcopt, (void *) &pvcval,
						       (signed32 *) NULL) < 0) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
					 svc_c_sev_warning, GDS_S_NDS_PVC_ERR) ;
	    return (NDS_NSAPERR) ;
	}
	if (pvcval > 4096) {
	    /* invalid PVC-number value */
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d%d"), GDS_S_CMX,
		       svc_c_sev_warning, GDS_S_NDS_PVCV_ERR, pvcval, 0, 4096) ;
	    return (NDS_NSAPERR) ;
	}
    } else
	pvcval = (signed32) pvcopt->nds_opt_shortval ;
    *((unsigned short *) addrref) = htons (pvcval) ;

    return (4) ;
}

/* ----------------------------------------------------------------------- */

static signed32
nds_map_ccinfo (Nds_options *optref,	/* >  specific options info */
		byte	    *addrref) {	/* >< mapped CC-info */

    Nds_opt_oct		*lanccref ;
    Nds_opt_recoct	*wanccref ;
    signed32		i, ccinfolen ;
    byte		*cclenref ;

    if (nds_get_option (NDS_LANCC_OPT, optref, (void **) &lanccref) != 0) {
	ccinfolen = lanccref->nds_opt_len/2 ;

	/* set CC-info AU-type */
	*addrref++ = NDS_CCINFO_AU ;
	/* set CC-info length */
	*addrref++ = (byte) (ccinfolen * 4) ;
	for (i = 0; i < ccinfolen; i++) {
	    /* set CC-info sub-AU-type */
	    *addrref++ = NDS_CCINFO_S_AU ;
	    /* set CC-info sub-length */
	    *addrref++ = 2 ;
	    /* set CC-info (CC-number) */
	    *addrref++ = lanccref->nds_opt_octval[i * 2] ;
	    *addrref++ = lanccref->nds_opt_octval[(i * 2) + 1] ;
	}
	return (2 + (4 * ccinfolen)) ;
    } else {
	if ((ccinfolen = nds_get_option (NDS_WANCC_OPT, optref,
						     (void **) &wanccref)) == 0)
	    /* no CC-informationen available */
	    return (0) ;

	/* set CC-info AU-type */
	*addrref++ = NDS_CCINFO_AU ;
	/* set CC-info length */
	*(cclenref = addrref++) = 0 ;
	for (i = 0; i < ccinfolen;) {
	    /* set CC-info sub-AU-type */
	    *addrref++ = NDS_CCINFO_S_AU ;
	    /* set CC-info sub-length */
	    *addrref++ = (byte) wanccref->nds_opt_len ;
	    /* set CC-info (CC-number and line number(s)) */
	    memcpy ((void *) addrref, (void *) wanccref->nds_opt_reoctval,
					       (size_t) wanccref->nds_opt_len) ;
	    *cclenref += (byte) (2 + wanccref->nds_opt_len) ;
	    addrref += wanccref->nds_opt_len ;
	    i += wanccref->nds_opt_roff ;
	    wanccref = (Nds_opt_recoct *) ((char *) wanccref +
						       wanccref->nds_opt_roff) ;
	}
	return (2 + *cclenref) ;
    }
}

/* ----------------------------------------------------------------------- */

static signed32
nds_map_ip (T_adrdesc     *nsap,	/* >  NSAP-address */
	    Nds_options   *optref,	/* >  options info */
	    byte	  *addrref) {	/* >< mapped IP-address info */

    Nds_opt_oct		*ipopt ;
    char		ipval[MAX_DSP] ;
    signed32		ipvallen ;

    /* set SNPA AU-type */
    *addrref++ = NDS_PROP_NSAP_AU ;
    /* set SNPA length */
    *addrref++ = (byte) 13 ;
    /* set IDP length */
    *addrref++ = (byte) 3 ;
    /* set IDP */
    *addrref++ = (byte) 0x49 ;
    *addrref++ = (byte) 0 ;
    *addrref++ = (byte) 0 ;
    /* set DSP type */
    *addrref++ = (byte) 0 ;
    /* set DSP length */
    *addrref++ = (byte) 7 ;
    /* set subnet-ID */
    *addrref++ = (byte) 0 ;
    *addrref++ = (byte) 1 ;

    if (nds_get_option (NDS_IP_OPT, optref, (void **) &ipopt) == 0) {
	/* no IP-address information available */
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
				     svc_c_sev_warning, GDS_S_NDS_MAPNSAP_ERR) ;
	return (NDS_NSAPERR) ;
    }

    if (ipopt->nds_opt_rep & NDS_IMP_MSK) {
	/* get IP-address from NSAP-address */
	if (nds_get_imp_val (nsap, (Nds_opt_short *) ipopt, ipval,
								 &ipvallen) < 0)
	    return (NDS_NSAPERR) ;
	if (ipvallen != 4) {
	    /* invalid size of IP-address */
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
				     svc_c_sev_warning, GDS_S_NDS_MAPNSAP_ERR) ;
	    return (NDS_NSAPERR) ;
	}

	memcpy ((void *) addrref, (void *) ipval, (size_t) ipvallen) ;
    } else
	memcpy ((void *) addrref, (void *) ipopt->nds_opt_octval,
						  (size_t) ipopt->nds_opt_len) ;
    addrref += (byte) ipvallen ;

    /* set N-selector */
    *addrref++ = (byte) 0xFE ;

    return (15) ;
}

/* ----------------------------------------------------------------------- */

static signed32
nds_map_port (T_adrdesc	   *tsel,		/* >  T-selector */
	      T_adrdesc	   *nsap,		/* >  NSAP-address */
	      Nds_options   *optref,		/* >  specific options info */
	      byte 	   *addrref) {		/* >< mapped port info */

    Nds_opt_recshort	*portopt ;
    Nds_opt_recoct	*tselopt ;
    char		*tselval ;
    signed32		i, tsellen, portval, portinfolen ;
    signed16		port ;

    /* set port AU-type */
    *addrref++ = NDS_PORT_AU ;
    /* set port length */
    *addrref++ = (byte) 2 ;

    if ((portinfolen = nds_get_option (NDS_PORT_OPT, optref,
						     (void **) &portopt)) > 0) {
	if (portopt->nds_opt_rep & NDS_IMP_MSK) {
	    /* get port no. from NSAP-address */
	    if (nds_get_imp_val (nsap, (Nds_opt_short *) portopt, &portval,
							 (signed32 *) NULL) < 0)
		return (NDS_NSAPERR) ;
	    if (portval > 32768) {
		/* invalid port no. */
		dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
				     svc_c_sev_warning, GDS_S_NDS_MAPNSAP_ERR) ;
		return (NDS_NSAPERR) ;
	    }
	    port =  htons ((short) portval) ;
	    *addrref++ = *(byte *) &port ;
	    *addrref = *(byte *) (&port + 1) ;
	} else {
	    if (tsel != (T_adrdesc *) NULL) {
		/* get port no. related to T-selector */
		i = 0 ;
		while (i < portinfolen) {
		    tselopt = (Nds_opt_recoct *) ((char *) portopt +
							portopt->nds_opt_roff) ;
		    switch (tselopt->nds_opt_rep & NDS_EXPREC_MSK) {
		    case NDS_REC_ASCSTR:
			tselval = ((Nds_opt_recstr *) tselopt)->nds_opt_restrval ;
			tsellen = strlen (tselval) ;
			break ;
		    case NDS_REC_OCTSTR:
			tselval = (char *) tselopt->nds_opt_reoctval ;
			tsellen = tselopt->nds_opt_len ;
			break ;
		    default:
			/* invalid representation of T-selector */
			dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
				     svc_c_sev_warning, GDS_S_NDS_MAPNSAP_ERR) ;
			return (NDS_NSAPERR) ;
			break ;
		    }
		    if (tsellen == tsel->t_infolng &&
			memcmp ((void *) tselval, (void *) (tsel + 1),
						       (size_t) tsellen) == 0) {
			port = htons (portopt->nds_opt_reshortval) ;
			*addrref++ = *(byte *) &port ;
			*addrref = *(byte *) (&port + 1) ;
			break ;
		    }
		    i += portopt->nds_opt_roff + tselopt->nds_opt_roff ;
		    portopt = (Nds_opt_recshort *) ((char *) tselopt +
							tselopt->nds_opt_roff) ;
		}
		if (i >= portinfolen) {
		    /* no corresponding T-selector found */
		    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
				     svc_c_sev_warning, GDS_S_NDS_MAPNSAP_ERR) ;
		    return (NDS_NSAPERR) ;
		}
	    } else {
		/* no port information available -> use default port */
		port =  htons ((short) NDS_DEFAULT_PORT) ;
		*addrref++ = *(byte *) &port ;
		*addrref = *(byte *) (&port + 1) ;
	    }
	}
    } else {
	    /* no port information available -> use default port */
	    port =  htons ((short) NDS_DEFAULT_PORT) ;
	    *addrref++ = *(byte *) &port ;
	    *addrref = *(byte *) (&port + 1) ;
    }

    return (4) ;
}
