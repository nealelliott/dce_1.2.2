/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ndscompil.c,v $
 * Revision 1.1.4.2  1996/02/18  18:20:29  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:43:08  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:06:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:16:35  root]
 * 
 * Revision 1.1.2.5  1994/09/06  12:25:57  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:17:41  keutel]
 * 
 * Revision 1.1.2.4  1994/07/06  15:08:00  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:18:41  marrek]
 * 
 * Revision 1.1.2.3  1994/06/21  14:47:44  marrek
 * 	June 1994 code submission.
 * 	[1994/06/17  12:59:56  marrek]
 * 
 * Revision 1.1.2.2  1994/06/09  18:47:06  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:20:16  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/10  16:14:56  marrek
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/05  06:38:17  marrek]
 * 
 * 	Bug fixes.
 * 	[1994/05/02  13:05:51  marrek]
 * 
 * 	Created for April 1994 submission.
 * 	[1994/04/29  10:31:07  marrek]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: ndscompil.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:20:29 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton **********************************************************************/
/*                                                                           */
/*   TYPE:    program                                                        */
/*                                                                           */
/*   NAME:    ndscomp                                                        */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The NDS-compiler compiles the information available in a NDS-       */
/*       configuration data source file (ASCII-file). The resulting binary   */
/*       output (called NDS-configuration data base) is always stored in a   */
/*       file NDSCONF.DIB in the actual directory.                           */
/*                                                                           */
/*   CALLING SYNTAX:                                                         */
/*       ndscomp [-h<workspace size>] [-n<no. net-addr>]                     */
/*               [-o<NDS-conf. data base file] [-V<1|2>] [-P...] [-w...]     */
/*               <NDS-conf. source file>                                     */
/*                                                                           */
/*   CALLING PARAMETER:                                                      */
/*       -h<heapsize(kB)>     = size of of the workspace internally used     */
/*                              during compilation.                          */
/*                              (default value = NDS_DFLT_HPSIZE)            */
/*       -n<no. net-addr>     = max. no. of single net-addresses or groups   */
/*                              of net-addresses occurring in the NDS-con-   */
/*                              figuration file.                             */
/*                              (default value = NDS_MX_NETADDR)             */
/*       -o<data base name>   = name of the binary configuration data base   */
/*                              file.                                        */
/*                              (default value = NDS_CONF_DIB)               */
/*       -V1                  = verbose mode (compilation information is     */
/*                              written to stdout).                          */
/*       -V2                  = extended verbose mode (including yacc        */
/*                              logging information).                        */
/*                              Note: The yacc logging information can't be  */
/*                              controlled by serviceability and is always   */
/*                              written to stdout.                           */
/*       -P, -w               = Serviceability switches.                     */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*    Author: R.Horn                                      Date: 27.5.93      */
/*datoff *********************************************************************/

#include <gds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <d27util.h>
#include <sys/types.h>
#include <dce/dcesvcmsg.h>
#include <dce/osiaddr.h>
#include <dce/cmx_addr.h>
#include <y.tab.h>
#include <ndscfg.h>

/* ******************* DEFINITION OF CONSTANTS ***************************** */

#define NDS_MX_IFNAME		32	/* max. size of an interface name */
#define NDS_UNUSED		-1

/* ******************* DECLARATION OF GLOBAL DATA ************************** */

extern int	yydebug, optind, opterr ;
extern char	*optarg ; 
signed32	svcprolog ;

/* ******************* DECLARATION OF LOCAL DATA *************************** */

				/* size of work space */
static signed32 mxheapsize = NDS_DFLT_HPSIZE ;
				/* max. # of net-addresses in config. file */
static signed32 mxnetaddr = NDS_MX_NETADDR ;
				/* name of configuration data base file */
static char	*nds_cfgdbfile = { NDS_CONF_DIB } ;

static Nds_shmgenarea	genarea ;
static Nds_shmrule	*naddrinfo, *naddrref ;
static Nds_optdesc	naoptinfo[NDS_MX_OPT + 1], *naoptref  ;
static Nds_shmifdesc	profinfo[NDS_MX_PROFILE], *profref ;
static signed32		noif, ifids, noprof, noopt ;
static signed32		ifid[NDS_MX_IF], comdev = NDS_UNUSED ;
static signed32		nonetaddr, noprofaddr, heapsize ;
static char		ifname[NDS_MX_IFNAME] ;
static void		*heapref, *heapstart ;
static OPTFLDTYPE	comopttypes, opttypes ;

/* ******************* PROTOTYPING OF FUNCTIONS **************************** */

signed32 nds_setoptions (Nds_optdesc *, signed32, Nds_options *, signed32) ;
void nds_set_subprofile (void) ;
void nds_store_all (void) ;
signed32 nds_store_info (void *, signed32) ;
char *nds_line (void) ;
int main (int argc, char *argv[]);

/* ------------------------------------------------------------------------- */

main (int argc, char *argv[]) {

    int			opt, level ;
    boolean		verbose ;
    error_status_t	svc_result ;

    /* set verbose flags default values */
    verbose = yydebug = FALSE ;

    /* initialize serviceability */
    svcprolog = svc_c_action_brief ;
    d27_001_svc_init (argc, argv, &svc_result) ;
    if (svc_result != svc_s_ok) {
	fprintf (stderr, "%s: Unable to initialize serviceability, status: 0x%lx\n",    
							  argv[0], svc_result) ;
	exit (1) ;
    }

    /* evaluate calling parameter */
    opterr = 0 ;
    while ((opt = getopt (argc, argv, "h:n:o:P:V:w:")) != -1) {
	switch (opt) {
	case 'h':
	    /* change work space size */
	    if ((mxheapsize = atoi (optarg) * 1024) < NDS_DFLT_HPSIZE) {
		dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_CMX,
			svc_c_sev_warning | svcprolog, GDS_S_NDS_WORKSPACE,
			mxheapsize, NDS_DFLT_HPSIZE) ;
	    }
	    break ;
	case 'n':
	    /* change no. of net-address (matching/mapping rule) entries */
	    if ((mxnetaddr = atoi (optarg)) < NDS_MX_NETADDR) {
		dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_CMX,
			svc_c_sev_warning | svcprolog, GDS_S_NDS_NETADDR,
			mxnetaddr, NDS_MX_NETADDR) ;
	    }
	    break ;
	case 'V':
	    if ((level = atoi (optarg)) == 1) {
		/* enable verbose mode */
		verbose = TRUE ;
	    } else {
		if (level == 2) {
		    /* enable extended verbose mode */
		    verbose = yydebug = TRUE ;
		} else {
		    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
			     svc_c_sev_fatal | svcprolog, GDS_S_NDS_USAGE_ERR) ;
		    exit (1) ;
		}
	    }
	    break ;
	case 'o':
	    /* change configuration data base file name */
	    nds_cfgdbfile = optarg ;
	    break ;
	case 'P':
	case 'w':
	    /* serviceability switches */
	    svcprolog = 0 ;
	    break ;
	default:
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
			     svc_c_sev_fatal | svcprolog, GDS_S_NDS_USAGE_ERR) ;
	    exit (1) ;
	    break ;
	}
    }
    if (optind != argc - 1) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_CMX,
			     svc_c_sev_fatal | svcprolog, GDS_S_NDS_USAGE_ERR) ;
	exit (1) ;
    }

    if (freopen (argv[optind], "r", stdin) == NULL) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%s%d"), GDS_S_CMX,
			svc_c_sev_fatal | svcprolog, GDS_S_NDS_OCONF_ERR,
			argv[optind], errno) ;
	exit (1) ;
    }

    if (!verbose) {
	if (freopen ("/dev/null", "w", stdout) == NULL) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_CMX,
		      svc_c_sev_fatal | svcprolog, GDS_S_NDS_REDIR_ERR, errno) ;
	    exit (1) ;
	}
    }
    /* parse NDS-configuration file */
    yyparse() ;

    exit (0) ;
}

/*daton **********************************************************************/
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       In the following all functions are defined which are used to        */
/*       evaluate the informations from NDS-configuration file (ASCII-file). */
/*       The informations are collected and saved temporary in a heap buf-   */
/*       fer. At the end the content of the heap buffer is stored in the     */
/*       NDS-data base (binary file).                                        */
/*                                                                           */
/*       The following functions are available:                              */
/*                                                                           */
/*          * nds_store_title    - store NDS-data base configuration-ID      */
/*          * nds_start_ifsection- evaluate interface section start info     */
/*          * nds_end_ifsection  - evaluate interface section end info       */
/*          * nds_start_if       - evaluate interface start info             */
/*          * nds_end_if         - evaluate interface end info               */
/*          * nds_start_prof     - evaluate profile start info               */
/*          * nds_end_prof       - evaluate profile end info                 */
/*          * nds_save_cominfo   - save common profile info                  */
/*          * nds_save_netaddr   - evaluate and save NSAP-address info       */
/*          * nds_save_option    - evaluate options info                     */
/*          * nds_setoptions     - save options info                         */
/*          * nds_store_info     - write info to temporary heap              */
/*          * nds_set_subprofile - set subprofile info                       */
/*          * nds_store_all      - write saved info to NDS-data base file    */
/*          * nds_lexdebug       - debugging function                        */
/*                                                                           */
/*datoff *********************************************************************/

/* ------------------ STORE CONFIGURATION TITLE ---------------------------- */

void nds_store_title (char *title) {	/* >  configuration title */

#ifdef DEBUG
    dce_fprintf (stdout, GDS_S_NDS_TITLE, title) ;
#endif
    sprintf (genarea.nds_conftitle, "@(#)%32s", title) ;
}

/* -------------- PERFORM 'BEGIN OF INTERFACE SECTION' ACTIONS ------------- */

void nds_start_ifsection () {

#ifdef DEBUG
    dce_fprintf (stdout, GDS_S_NDS_IFSSTART) ;
#endif
    /* start of interface section */
    noif = ifids = noprof = nonetaddr = 0 ;
    profref = profinfo ;

    /* allocate work space memory */
    if ((heapstart = heapref = malloc ((size_t) mxheapsize)) == (void *) NULL) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%s"), GDS_S_CMX,
			svc_c_sev_fatal | svcprolog, GDS_S_NDS_WSPACE_ERR,
			mxheapsize, nds_line()) ;
	exit (1) ;
    }
    memset ((void *) heapref, (int) NDS_FILLER, (size_t) mxheapsize) ;

    /* allocate NSAP-address description memory */
    if ((naddrinfo = naddrref =
	(Nds_shmrule *) malloc ((size_t) (sizeof (Nds_shmrule) * mxnetaddr))) ==
							 (Nds_shmrule *) NULL) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%s"), GDS_S_CMX,
			svc_c_sev_fatal | svcprolog, GDS_S_NDS_NETSPACE_ERR,
			mxnetaddr, nds_line()) ;
	exit (1) ;
    }
}

/* ---------------- PERFORM 'END OF INTERFACE SECTION' ACTIONS ------------- */

void nds_end_ifsection () {

#ifdef DEBUG
    dce_fprintf (stdout, GDS_S_NDS_IFSEND, noif, ifids, noprof, nonetaddr) ;
#endif

    if (noif == 0) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%s"), GDS_S_CMX,
	       svc_c_sev_fatal | svcprolog, GDS_S_NDS_IFDESC_ERR, nds_line()) ;
	exit (1) ;
    }

    /* store all collected information from the interface section into */
    /* NDS-configuration data base */
    nds_store_all() ;
}

/* ---------------- PERFORM 'START OF INTERFACE' ACTIONS ------------------- */

void nds_start_if (signed32	iftype,		/* >  interface type */
		   char		*ifnamep) {	/* >  interface name */

#ifdef DEBUG
    dce_fprintf (stdout, GDS_S_NDS_IFSTART, ifnamep, iftype) ;
#endif
    if (noif == NDS_MX_IF) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%s"), GDS_S_CMX,
			svc_c_sev_fatal | svcprolog, GDS_S_NDS_IFMXDESC_ERR,
			NDS_MX_IF, nds_line()) ;
	exit (1) ;
    }

    /* set interface-ID */
    ifid[noif] = (iftype == IF_SOCKET) ? NDS_SOCKETINFO :
		 (iftype == IF_XTI) ?    NDS_XTIINFO : NDS_CMXSINFO ;
    ifids |= ifid[noif] ;

    /* save interface name */
    strcpy (ifname, ifnamep) ;

    noopt = 0 ;
    naoptref = naoptinfo ;
    opttypes = (OPTFLDTYPE) 0 ;
}

/* ------------------ PERFORM 'END OF INTERFACE' ACTIONS ------------------- */

void nds_end_if (char *ifnamep) {	/* >  interface name */

#ifdef DEBUG
    dce_fprintf (stdout, GDS_S_NDS_IFEND, ifnamep) ;
#endif
    if (strcmp (ifname, ifnamep)) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%s%s%s"), GDS_S_CMX,
			svc_c_sev_fatal | svcprolog, GDS_S_NDS_IFNM_ERR,
			ifname, ifnamep, nds_line()) ;
	exit (1) ;
    }
    noif++ ;
}

/* ------------------ PERFORM 'START OF PROFILE' ACTIONS ------------------- */

void nds_start_prof (signed32 profid) {	/* >  profile type */

    signed32	optsize ;

    if (noprof == NDS_MX_PROFILE) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%s"), GDS_S_CMX,
			svc_c_sev_fatal | svcprolog, GDS_S_NDS_MXPROF_ERR,
			NDS_MX_PROFILE, nds_line()) ;
	exit (1) ;
    }
    profref->nds_ifid = ifid[noif] ;
    profref->nds_provid = profid ;
    profref->nds_devname = NDS_UNUSED ;
    profref->nds_profopt = NDS_UNUSED ;
    noprofaddr = 0 ;

    /* save common profile options */
    if (noopt > 0) {
	profref->nds_profopt = heapsize ;
	optsize = nds_setoptions (naoptinfo, noopt, heapref,
							mxheapsize - heapsize) ;
	heapref = (void *) ((byte *) heapref + optsize) ;
	heapsize += optsize ;
	noopt = 0 ;
	naoptref = naoptinfo ;
	comopttypes = opttypes ;
	opttypes = (OPTFLDTYPE) 0 ;
    } else {
	if (noprof > 0 && (profref - 1)->nds_ifid == profref->nds_ifid)
	    profref->nds_profopt = (profref - 1)->nds_profopt ;
    }
#ifdef DEBUG
    dce_fprintf (stdout, GDS_S_NDS_PROFSTART, profid) ;
#endif
}

/* ------------------ PERFORM 'END OF PROFILE' ACTIONS --------------------- */

void nds_end_prof (signed32 profid) {	/* >  profile type */

    signed32		optsize ;

    if (noprofaddr == 0) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%s"), GDS_S_CMX,
		svc_c_sev_fatal | svcprolog, GDS_S_NDS_NONSAP_ERR, nds_line()) ;
	exit (1) ;
    }
    if (profid != profref->nds_provid) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d%s"), GDS_S_CMX,
		svc_c_sev_fatal | svcprolog, GDS_S_NDS_PROFID_ERR,
		profid, profref->nds_provid, nds_line()) ;
	exit (1) ;
    }
    if (noopt > 0) {
	nds_set_subprofile () ;

	/* save options */
	naddrref->nds_mprule = heapsize ;
	optsize = nds_setoptions (naoptinfo, noopt, heapref,
							mxheapsize - heapsize) ;
	heapref = (void *) ((byte *) heapref + optsize) ;
	heapsize += optsize ;
	noopt = 0 ;
	naoptref = naoptinfo ;
	opttypes = (OPTFLDTYPE) 0 ;
    }
    if (profref->nds_ifid == NDS_XTIINFO && profref->nds_devname == NDS_UNUSED) {
	if (comdev == NDS_UNUSED) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%s"), GDS_S_CMX,
		 svc_c_sev_fatal | svcprolog, GDS_S_NDS_DEVNM_ERR, nds_line()) ;
	    exit (1) ;
	}
	profref->nds_devname = comdev ;
    }
    profref++ ;
    noprof++ ;
    naddrref++ ;
#ifdef DEBUG
    dce_fprintf (stdout, GDS_S_NDS_PROFEND, profid, noprofaddr) ;
#endif
}

/* ------------------ SAVE COMMON PROFILE OPTION INFO ---------------------- */

void nds_save_cominfo (signed32	infotype,	/* >  info type */
		       int	infoval1,	/* >  info value */
		       char	*infoval2) {	/* >  info value */

    Nds_shmifdesc	*profhref ;

    switch (infotype) {
    case NDS_PRIO_TOK:
#ifdef DEBUG
    dce_fprintf (stdout, GDS_S_NDS_PROFPRIO, infoval1) ;
#endif
	profref->nds_profprioid = (signed32) infoval1 ;

	for (profhref = profinfo; profhref < profref; profhref++)
	    if (profhref->nds_profprioid == profref->nds_profprioid) {
		dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%s"), GDS_S_CMX,
			svc_c_sev_fatal | svcprolog, GDS_S_NDS_PRIO_ERR,
			profref->nds_profprioid, nds_line()) ;
		exit (1) ;
	    }
	break ;
    case NDS_DEVNM_TOK:
#ifdef DEBUG
    dce_fprintf (stdout, GDS_S_NDS_PROFDEV, infoval2) ;
#endif
	if (profref->nds_provid == 0)
	    comdev = nds_store_info (infoval2, 0) ;
	else
	    profref->nds_devname = nds_store_info (infoval2, 0) ;
	break ;
    default:
	break ;
    }
}

/* ------------------ SAVE NSAP-ADDRESS INFORMATION ------------------------ */

void nds_save_netaddr (signed32	ntype,	      /* >  net-address type */
		       char	*netaddr) {   /* >  net-address value */

    char	tmpbuf[256] ;
    byte	*compref ;
    T_adrdesc	*nsapref = (T_adrdesc *) tmpbuf ;
    signed32	res, optsize, compsize ;

    if (nonetaddr == mxnetaddr) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%s"), GDS_S_CMX,
			svc_c_sev_fatal | svcprolog, GDS_S_NDS_MXNET_ERR,
			mxnetaddr, nds_line()) ;
	exit (1) ;
    }
    if (noopt > 0) {
	/* store address mapping options */
	nds_set_subprofile () ;

	naddrref->nds_mprule = heapsize ;
	optsize = nds_setoptions (naoptinfo, noopt, heapref,
							mxheapsize - heapsize) ;
	heapref = (void *) ((byte *) heapref + optsize) ;
	heapsize += optsize ;
	noopt = 0 ;
	naoptref = naoptinfo ;
	opttypes = (OPTFLDTYPE) 0 ;
    }
#ifdef DEBUG
    dce_fprintf (stdout, GDS_S_NDS_PROFGNSAP, netaddr) ;
#endif
    if (noprofaddr > 0)
	naddrref++ ;
    if (ntype == NDS_GROUP_ADDR) {
	/* compile regular expression describing NSAP-address group */
	if (nds_regcomp (netaddr, &compref, &compsize) != 0) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%s"), GDS_S_CMX,
		 svc_c_sev_fatal | svcprolog, GDS_S_NDS_CNSAP_ERR, nds_line()) ;
	    exit (1) ;
	}
#ifdef DEBUG
	dce_fprintf (stdout, GDS_S_NDS_PROFGNSAPC) ;
	d27_printmsg (stdout, compref, compsize) ;
#endif
	naddrref->nds_mtchtype = NDS_GROUP_ADDR | compsize ;
	naddrref->nds_mtchrule = nds_store_info (compref, compsize) ;
    } else {
	/* convert NSAP-address macro describing a single NSAP-address */
	if ((res = d2p_p01_str_to_bin_psap ("", "", "", 1, &netaddr,
							(byte *) tmpbuf)) < 0) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%s"), GDS_S_CMX,
			svc_c_sev_fatal | svcprolog, GDS_S_NDS_ENSAP_ERR,
			res, nds_line()) ;
	    exit (1) ;
	}
#ifdef DEBUG
	dce_fprintf (stdout, GDS_S_NDS_PROFSNSAP) ;
	d27_printmsg (stdout, (byte *)(nsapref + 1), (signed32) nsapref->t_infolng) ;
#endif
	naddrref->nds_mtchtype = NDS_SINGLE_ADDR | nsapref->t_infolng ;
	naddrref->nds_mtchrule = nds_store_info (nsapref + 1, nsapref->t_infolng) ;
    }
    naddrref->nds_ifref = sizeof (Nds_shmgenarea) + noprof * sizeof (Nds_shmifdesc) ;
    naddrref->nds_mprule = 0 ;
    nonetaddr++ ;	
    noprofaddr++ ;
}

/* ------------------ SAVE SPECIFIC OPTION INFORMATION --------------------- */

void nds_save_option (OPTFLDTYPE opttype,	/* >  option type */
		      signed32	 optrep,	/* >  option representation */
		      char	 *optvalue) {	/* >  option value */

    signed32	i, iopt, octsize, tmplen ;
    void	*tmpref ;

#ifdef DEBUG
    dce_fprintf (stdout, GDS_S_NDS_PROFOPT, opttype, optvalue) ;
#endif
    if (noopt == NDS_MX_OPT) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%s"), GDS_S_CMX,
			svc_c_sev_fatal | svcprolog, GDS_S_NDS_CNSAP_ERR,
			NDS_MX_OPT, nds_line()) ;
	exit (1) ;
    }

    switch (optrep) {
    case NDS_EXP_ASCSTR:
	naoptref->nds_optlen =
			  NDS_ALIGN4(sizeof (Nds_opt_str) + strlen (optvalue)) ;
	if ((naoptref->nds_optinfo = tmpref =
	    (void *) malloc ((size_t) naoptref->nds_optlen)) == (void *) NULL) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%s"), GDS_S_CMX,
			svc_c_sev_fatal | svcprolog, GDS_S_NDS_OPTMEM_ERR,
			naoptref->nds_optlen, nds_line()) ;
	    exit (1) ;
	}
	memset (tmpref, (int) NDS_FILLER, (size_t) naoptref->nds_optlen) ;
	((Nds_opt_str*)tmpref)->nds_opt_rep = (byte) optrep ;
	strcpy (((Nds_opt_str*)tmpref)->nds_opt_strval, optvalue) ;
	break ;
    case NDS_EXP_OCTSTR:
	octsize = strlen (optvalue)/(size_t) 2 ;
	naoptref->nds_optlen = NDS_ALIGN4(sizeof (Nds_opt_oct) + octsize - 1) ;
	if ((naoptref->nds_optinfo = tmpref =
	    (void *) malloc ((size_t) naoptref->nds_optlen)) == (void *) NULL) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%s"), GDS_S_CMX,
			svc_c_sev_fatal | svcprolog, GDS_S_NDS_OPTMEM_ERR,
			naoptref->nds_optlen, nds_line()) ;
	    exit (1) ;
	}
	memset (tmpref, (int) NDS_FILLER, (size_t) naoptref->nds_optlen) ;
	((Nds_opt_oct*)tmpref)->nds_opt_rep = (byte) optrep ;
	((Nds_opt_oct*)tmpref)->nds_opt_len = octsize ;
	for (i = 0; i < octsize; i++) {
	    sscanf (&optvalue[2 * i], "%2x", &iopt) ;
	    ((Nds_opt_oct*)tmpref)->nds_opt_octval[i] = (byte) iopt ;
	}
	break ;
    case NDS_EXP_SHORT:
    case NDS_IMP_DIGSTR:
    case NDS_IMP_OCTSTR:
    case NDS_IMP_INT:
	naoptref->nds_optlen = NDS_ALIGN4(sizeof (Nds_opt_short)) ;
	if ((naoptref->nds_optinfo = tmpref =
	    (void *) malloc ((size_t) naoptref->nds_optlen)) == (void *) NULL) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%s"), GDS_S_CMX,
			svc_c_sev_fatal | svcprolog, GDS_S_NDS_OPTMEM_ERR,
			naoptref->nds_optlen, nds_line()) ;
	    exit (1) ;
	}
	memset (tmpref, (int) NDS_FILLER, (size_t) naoptref->nds_optlen) ;
	((Nds_opt_short*)tmpref)->nds_opt_rep = (byte) optrep ;
	((Nds_opt_short*)tmpref)->nds_opt_shortval = (signed16) atoi (optvalue) ;
	break ;
    case NDS_EXP_LONG:
	naoptref->nds_optlen = NDS_ALIGN4(sizeof (Nds_opt_long)) ;
	if ((naoptref->nds_optinfo = tmpref =
	    (void *) malloc ((size_t) naoptref->nds_optlen)) == (void *) NULL) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%s"), GDS_S_CMX,
			svc_c_sev_fatal | svcprolog, GDS_S_NDS_OPTMEM_ERR,
			naoptref->nds_optlen, nds_line()) ;
	    exit (1) ;
	}
	memset (tmpref, (int) NDS_FILLER, (size_t) naoptref->nds_optlen) ;
	((Nds_opt_long*)tmpref)->nds_opt_rep = (byte) optrep ;
	((Nds_opt_long*)tmpref)->nds_opt_longval = atol (optvalue) ;
	break ;
    case NDS_REC_ASCSTR:
    case NDS_REC_OCTSTR:
    case NDS_REC_SHORT:
    case NDS_REC_LONG:
	tmplen = (optrep == NDS_REC_ASCSTR) ?
		 NDS_ALIGN4(sizeof (Nds_opt_recstr) + strlen (optvalue)) :
		 (optrep == NDS_REC_OCTSTR) ?
		 NDS_ALIGN4(sizeof (Nds_opt_recoct) +
				(octsize = strlen (optvalue)/(size_t) 2) - 1) :
		 (optrep == NDS_REC_SHORT) ?
		 NDS_ALIGN4(sizeof (Nds_opt_recshort)) :
		 NDS_ALIGN4(sizeof (Nds_opt_reclong)) ;
	if (noopt == 0 || (naoptref - 1)->nds_opttype != opttype) {
	    if ((naoptref->nds_optinfo = tmpref =
			  (void *) malloc ((size_t) tmplen)) == (void *) NULL) {
		dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%s"), GDS_S_CMX,
			svc_c_sev_fatal | svcprolog, GDS_S_NDS_OPTMEM_ERR,
			tmplen, nds_line()) ;
		exit (1) ;
	    }
	    naoptref->nds_optlen = tmplen ;
	} else {
	    /* additional recurring option value */
	    naoptref-- ;
	    noopt-- ;
	    if ((naoptref->nds_optinfo =
		(void *) realloc ((void *) naoptref->nds_optinfo,
		  (size_t) (tmplen + naoptref->nds_optlen))) == (void *) NULL) {
		dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%s"), GDS_S_CMX,
			svc_c_sev_fatal | svcprolog, GDS_S_NDS_OPTMEM_ERR,
			tmplen + naoptref->nds_optlen, nds_line()) ;
		exit (1) ;
	    }
	    tmpref = (void *) ((char *) naoptref->nds_optinfo +
							 naoptref->nds_optlen) ;
	    naoptref->nds_optlen += tmplen ;
	}
	memset (tmpref, (int) NDS_FILLER, (size_t) tmplen) ;
	switch (optrep) {
	case NDS_REC_ASCSTR:
	    ((Nds_opt_recstr*)tmpref)->nds_opt_rep = (byte) optrep ;
	    ((Nds_opt_recstr*)tmpref)->nds_opt_roff = (byte) tmplen ;
	    strcpy (((Nds_opt_recstr*)tmpref)->nds_opt_restrval, optvalue) ;
	    break ;
	case NDS_REC_OCTSTR:
	    ((Nds_opt_recoct*)tmpref)->nds_opt_rep = (byte) optrep ;
	    ((Nds_opt_recoct*)tmpref)->nds_opt_roff = (byte) tmplen ;
	    ((Nds_opt_recoct*)tmpref)->nds_opt_len = (byte) octsize ;
	    for (i = 0; i < octsize; i++) {
		sscanf (&optvalue[2 * i], "%2x", &iopt) ;
		((Nds_opt_recoct*)tmpref)->nds_opt_reoctval[i] = (byte) iopt ;
	    }
	    break ;
	case NDS_REC_SHORT:
	    ((Nds_opt_recshort*)tmpref)->nds_opt_rep = (byte) optrep ;
	    ((Nds_opt_recshort*)tmpref)->nds_opt_roff = (byte) tmplen ;
	    ((Nds_opt_recshort*)tmpref)->nds_opt_reshortval =
						    (signed16) atoi (optvalue) ;
	    break ;
	case NDS_REC_LONG:
	    ((Nds_opt_reclong*)tmpref)->nds_opt_rep = (byte) optrep ;
	    ((Nds_opt_reclong*)tmpref)->nds_opt_roff = (byte) tmplen ;
	    ((Nds_opt_reclong*)tmpref)->nds_opt_relongval = atol (optvalue) ;
	    break ;
	default:
	    break ;
	}
	break ;
    default:
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_CMX,
			svc_c_sev_warning | svcprolog, GDS_S_NDS_UNKNOWNREP_ERR,
			opttype, optrep) ;
	exit (1) ;
    }
    opttypes |= opttype ;
    naoptref++->nds_opttype = opttype ;
    naoptref->nds_opttype = (OPTFLDTYPE) 0 ;
    noopt++ ;
}


/* --------------- CONVERT OPTION INFO INTO INTERNAL FORMAT ---------------- */

signed32 nds_setoptions (Nds_optdesc *naoptref,	  /* >  option description */
			 signed32    noopt,	  /* >  no. of options */
			 Nds_options *optarea,	  /* >< options (int. format) */
			 signed32    areasize) {  /* >< heap area size */

    signed32  		presize, optno = 0 ;
    byte		*optdatref ;
    Nds_optdesc		*naoptref1, *naoptref2, naopttmp ;


    /* sort options */
    naoptref2 = naoptref ;
    while (naoptref2 < naoptref + noopt - 1) {
	naoptref1 = naoptref2 + 1 ;
	while (naoptref1 < naoptref + noopt) {
	   if ((naoptref2->nds_opttype - 1) & naoptref1->nds_opttype) {
		naopttmp = *naoptref2 ;
		*naoptref2 = *naoptref1 ;
		*naoptref1 = naopttmp ;
	    }	
	    naoptref1++ ;
	}
	naoptref2++ ;
    }

    /* convert options into internal option format */
    presize = (signed32) ((char *)&optarea->nds_opt_off[noopt + 1] -
							     (char *) optarea) ;
    presize = NDS_ALIGN4(presize) ;
    if ((areasize -= presize) < 0) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%s"), GDS_S_CMX,
	       svc_c_sev_fatal | svcprolog, GDS_S_NDS_UWSPACE_ERR, nds_line()) ;
	exit (1) ;
    }

    optdatref = (byte *) optarea + presize ;
    optarea->nds_opt_types = (OPTFLDTYPE) 0 ;

    for (optno = 0; optno < noopt; optno++, naoptref++) {
	if ((areasize = areasize - naoptref->nds_optlen) < 0) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%s"), GDS_S_CMX,
	       svc_c_sev_fatal | svcprolog, GDS_S_NDS_UWSPACE_ERR, nds_line()) ;
	    exit (1) ;
	}

	optarea->nds_opt_types |= naoptref->nds_opttype ;
	optarea->nds_opt_off[optno] = (OFFSETTYPE) (optdatref -
							     (byte *) optarea) ;
	memcpy (optdatref, naoptref->nds_optinfo, naoptref->nds_optlen) ;
	optdatref += naoptref->nds_optlen ;
    }
    optarea->nds_opt_off[optno] = (OFFSETTYPE) (optdatref - (byte *) optarea) ;
#ifdef DEBUG
    dce_fprintf (stdout, GDS_S_NDS_PROFOPTBIN) ;
    d27_printmsg (stdout, (byte *)optarea, (signed32) optarea->nds_opt_off[optno]) ;
#endif
    return ((signed32) optarea->nds_opt_off[optno]) ;
}

/* --------------- COPY INFORMATION INTO WORK SPACE ------------------------ */

signed32 nds_store_info (void	   *inforef,	/* > info value */
			 signed32  infosize) {	/* > info size */

    signed32	oldheapsize, tmplen ;

    oldheapsize = heapsize ;
    if (infosize == 0)
	infosize = strlen (inforef) + 1 ;
    tmplen = NDS_ALIGN4(infosize) ;
    if (heapsize + tmplen > mxheapsize) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%s"), GDS_S_CMX,
	       svc_c_sev_fatal | svcprolog, GDS_S_NDS_UWSPACE_ERR, nds_line()) ;
	exit (1) ;
    }
    memcpy (heapref, inforef, (size_t) infosize) ;
    heapref = (void *) ((byte *) heapref + tmplen) ;
    heapsize += tmplen ;
    return (oldheapsize) ;
}

/* ------------------- VERIFY COMPLETENESS OF SUB-PROFILE INFO ------------- */

void nds_set_subprofile () {

    char	subprofval[4] ;
    signed32	subprof ;
    OPTFLDTYPE	alloptions = opttypes | comopttypes ;

#ifdef SNI_SINIX
    if (profref->nds_provid == NDS_WANSBKA_PROF ||
	profref->nds_provid == NDS_WAN3SBKA_PROF ||
	profref->nds_provid == NDS_WANNEA_PROF) {
#else
    if (profref->nds_provid == NDS_OSIWAN_PROF) {
#endif
	subprof = ((opttypes & NDS_E164_OPT) && (opttypes & NDS_X121_OPT)) ?
							NDS_X31_SUBPROF :
		  ((opttypes & NDS_X21_OPT) && (opttypes & NDS_X121_OPT)) ?
							NDS_X32_SUBPROF :
		  (opttypes & NDS_X121_OPT) ?		NDS_X121_SUBPROF :
		  (opttypes & NDS_E164_OPT) ?		NDS_E164_SUBPROF :
		  (opttypes & NDS_E163_OPT) ?		NDS_E163_SUBPROF :
		  (opttypes & NDS_X21_OPT) ?		NDS_X21_SUBPROF :
		  (opttypes & NDS_PVC_OPT) ?		NDS_PVC_SUBPROF : 0 ;
	switch (subprof) {
	case NDS_X31_SUBPROF:
	    if (!(alloptions & NDS_X121_IP_OPT) ||
	       (!(alloptions & NDS_X121_DCC_OPT) &&
		!(alloptions & NDS_X121_DNIC_OPT))) {
		dce_svc_printf (DCE_SVC (gds_svc_handle, "%s"), GDS_S_CMX,
			svc_c_sev_fatal | svcprolog, GDS_S_NDS_MISX121_ERR,
			nds_line()) ;
		exit (1) ;
	    }
	    if (!(alloptions & NDS_E164_IP_OPT) ||
	        !(alloptions & NDS_E164_CC_OPT) ||
		!(alloptions & NDS_E164_NDC_OPT)) {
		dce_svc_printf (DCE_SVC (gds_svc_handle, "%s"), GDS_S_CMX,
			svc_c_sev_fatal | svcprolog, GDS_S_NDS_MISE164_ERR,
			nds_line()) ;
		exit (1) ;
	    }
	    break ;
	case NDS_X32_SUBPROF:
	case NDS_X121_SUBPROF:
	    if (!(alloptions & NDS_X121_IP_OPT) ||
	       (!(alloptions & NDS_X121_DCC_OPT) &&
		!(alloptions & NDS_X121_DNIC_OPT))) {
		dce_svc_printf (DCE_SVC (gds_svc_handle, "%s"), GDS_S_CMX,
			svc_c_sev_fatal | svcprolog, GDS_S_NDS_MISX121_ERR,
			nds_line()) ;
		exit (1) ;
	    }
	    break ;
	case NDS_E163_SUBPROF:
	    if (!(alloptions & NDS_E163_IP_OPT) ||
	        !(alloptions & NDS_E163_CC_OPT) ||
	        !(alloptions & NDS_E163_NP_OPT) ||
		!(alloptions & NDS_E163_AC_OPT)) {
		dce_svc_printf (DCE_SVC (gds_svc_handle, "%s"), GDS_S_CMX,
			svc_c_sev_fatal | svcprolog, GDS_S_NDS_MISE163_ERR,
			nds_line()) ;
		exit (1) ;
	    }
	    break ;
	case NDS_E164_SUBPROF:
	    if (!(alloptions & NDS_E164_IP_OPT) ||
	        !(alloptions & NDS_E164_CC_OPT) ||
		!(alloptions & NDS_E164_NDC_OPT)) {
		dce_svc_printf (DCE_SVC (gds_svc_handle, "%s"), GDS_S_CMX,
			svc_c_sev_fatal | svcprolog, GDS_S_NDS_MISE164_ERR,
			nds_line()) ;
		exit (1) ;
	    }
	    break ;
	case NDS_X21_SUBPROF:
	case NDS_PVC_SUBPROF:
	    break ;
	default:
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%s"), GDS_S_CMX,
			svc_c_sev_fatal | svcprolog, GDS_S_NDS_MISPROF_ERR,
			nds_line()) ;
	    exit (1) ;
	    break ;
	}
	sprintf (subprofval, "%d", subprof) ;
	nds_save_option (NDS_SUBPROF_OPT, NDS_EXP_SHORT, subprofval) ;
    }
}

/* -------------- STORE CONFIGURATION INFO INTO DATA BASE ------------------ */

void nds_store_all () {

    Nds_shmruledesc	ruledesc ;
    signed32		i ;
    FILE		*fd ;
        
    if ((fd = fopen (nds_cfgdbfile, "w")) == (FILE *) NULL) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%s%d"), GDS_S_CMX,
			svc_c_sev_fatal | svcprolog, GDS_S_NDS_OCONFO_ERR,
			nds_cfgdbfile, errno) ;
	exit (1) ;
    }

    genarea.nds_versno = NDS_SHMVERS ;
    genarea.nds_shmstate = 0 ;
    genarea.nds_noifdesc = noprof ;
    genarea.nds_ifdescarea = sizeof (Nds_shmgenarea) ;
    genarea.nds_noruledesc = 1 ;
    genarea.nds_ruledescarea = genarea.nds_ifdescarea + noprof *
							sizeof (Nds_shmifdesc) ;
    genarea.nds_rulearea = genarea.nds_ruledescarea + sizeof (Nds_shmruledesc) ;
    genarea.nds_heap = genarea.nds_rulearea + nonetaddr * sizeof (Nds_shmrule) ;
    if (fwrite (&genarea, sizeof (Nds_shmgenarea), 1, fd) == 0) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%s%d"), GDS_S_CMX,
			svc_c_sev_fatal | svcprolog, GDS_S_NDS_WCONFO_ERR,
			nds_cfgdbfile, errno) ;
	exit (1) ;
    }

    profref = profinfo ;
    for (i = 0; i < noprof; i++, profref++) {
	if (profref->nds_devname != NDS_UNUSED) {
	    profref->nds_devname += genarea.nds_heap ;	
	} else {
	    profref->nds_devname = 0 ;	
	}
	if (profref->nds_profopt != NDS_UNUSED) {
	    profref->nds_profopt += genarea.nds_heap ;	
	} else {
	    profref->nds_profopt = 0 ;	
	}
        if (fwrite (profref, sizeof (Nds_shmifdesc), 1, fd) == 0) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%s%d"), GDS_S_CMX,
			svc_c_sev_fatal | svcprolog, GDS_S_NDS_WCONFO_ERR,
			nds_cfgdbfile, errno) ;
	    exit (1) ;
        }
    }

    ruledesc.nds_mtype = NDS_CMXSEL|NDS_XTISEL|NDS_OSITOXTI|NDS_OSITOSOCK ;
    ruledesc.nds_nomrules = nonetaddr ;
    ruledesc.nds_mrules = genarea.nds_rulearea ;
    if (fwrite (&ruledesc, sizeof (Nds_shmruledesc), 1, fd) == 0) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%s%d"), GDS_S_CMX,
			svc_c_sev_fatal | svcprolog, GDS_S_NDS_WCONFO_ERR,
			nds_cfgdbfile, errno) ;
	exit (1) ;
    }

    naddrref = naddrinfo ;
    for (i = 0; i < nonetaddr; i++, naddrref++) {
	naddrref->nds_mtchrule += genarea.nds_heap ;
	if (naddrref->nds_mprule)
	    naddrref->nds_mprule += genarea.nds_heap ;
	if (fwrite (naddrref, sizeof (Nds_shmrule), 1, fd) == 0) {
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%s%d"), GDS_S_CMX,
			svc_c_sev_fatal | svcprolog, GDS_S_NDS_WCONFO_ERR,
			nds_cfgdbfile, errno) ;
	    exit (1) ;
	}
    }

    if (fwrite (heapstart, heapsize, 1, fd) == 0) {
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%s%d"), GDS_S_CMX,
			svc_c_sev_fatal | svcprolog, GDS_S_NDS_WCONFO_ERR,
			nds_cfgdbfile, errno) ;
	exit (1) ;
    }
}
