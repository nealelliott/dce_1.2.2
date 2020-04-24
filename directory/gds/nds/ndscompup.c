/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ndscompup.c,v $
 * Revision 1.1.178.2  1996/02/18  18:20:32  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:43:10  marty]
 *
 * Revision 1.1.178.1  1995/12/08  16:06:26  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:16:37  root]
 * 
 * Revision 1.1.176.3  1994/09/06  12:25:59  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:17:50  keutel]
 * 
 * Revision 1.1.176.2  1994/06/09  18:47:07  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:20:17  devsrc]
 * 
 * Revision 1.1.176.1  1994/02/22  17:41:34  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:58:23  marrek]
 * 
 * Revision 1.1.174.2  1993/10/14  17:36:03  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:34:45  keutel]
 * 
 * Revision 1.1.174.1  1993/10/13  17:33:28  keutel
 * 	October 1993 code drop
 * 	[1993/10/09  16:51:00  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: ndscompup.c,v $ $Revision: 1.1.178.2 $ $Date: 1996/02/18 18:20:32 $";
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
/*   NAME:    ndscompup.c                                                    */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       This module contains all functions used to evaluate the infor-      */
/*       mations from NDS-configuration file (ASCII-file), to save them      */
/*       into a temporary heap and to store at the end this informations     */
/*       in the NDS-data base (binary file).                                 */
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
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                        DATE: 27.5.93     */
/*datoff *********************************************************************/

#include <gds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dce/osiaddr.h>
#include <dce/cmx_addr.h>
#include <y.tab.h>
#include <ndscfg.h>

/* ******************* DEFINITION OF CONSTANTS ***************************** */

#define NDS_MX_IFNAME		32	/* max. size of an interface name */

/* ******************* DECLARATION OF GLOBAL DATA ************************** */

extern char	*nds_cfgdbfile ;
extern signed32	mxheapsize, mxnetaddr ;

/* ******************* DECLARATION OF LOCAL DATA *************************** */

static Nds_shmgenarea	genarea ;
static Nds_shmrule	*naddrinfo, *naddrref ;
static Nds_optdesc	naoptinfo[NDS_MX_OPT + 1], *naoptref  ;
static Nds_shmifdesc	profinfo[NDS_MX_PROFILE], *profref ;
static signed32		noif, ifids, noprof, noopt ;
static signed32		ifid[NDS_MX_IF], comdev ;
static signed32		nonetaddr, noprofaddr, heapsize ;
static char		ifname[NDS_MX_IFNAME] ;
static void		*heapref, *heapstart ;
static OPTFLDTYPE	comopttypes, opttypes ;

/* ******************* PROTOTYPING OF LOCAL FUNCTIONS ********************** */

signed32 nds_setoptions (Nds_optdesc *, signed32, Nds_options *, signed32) ;
void nds_set_subprofile (void) ;
void nds_store_all (void) ;
signed32 nds_store_info (void *, signed32) ;

/* ------------------ STORE CONFIGURATION TITLE ---------------------------- */

void nds_store_title (char *title) {	/* >  configuration title */

#ifdef DEBUG
    fprintf (stdout, "TITLE: %s\n", title) ;
#endif
    sprintf (genarea.nds_conftitle, "@(#)%32s", title) ;
}

/* -------------- PERFORM 'BEGIN OF INTERFACE SECTION' ACTIONS ------------- */

void nds_start_ifsection () {

#ifdef DEBUG
    fprintf (stdout, "start of interface section\n") ;
#endif
    /* start of interface section */
    noif = ifids = noprof = nonetaddr = 0 ;
    profref = profinfo ;

    /* allocate work space memory */
    if ((heapstart = heapref = malloc ((size_t) mxheapsize)) == (void *) NULL)
	yyerror ("parser: can't allocate work space memory") ;
    memset ((void *) heapref, (int) NDS_FILLER, (size_t) mxheapsize) ;

    /* allocate NSAP-address description memory */
    if ((naddrinfo = naddrref =
	(Nds_shmrule *) malloc ((size_t) (sizeof (Nds_shmrule) * mxnetaddr))) ==
							   (Nds_shmrule *) NULL)
	yyerror ("parser: can't allocate memory for NSAP-address descriptions") ;
}

/* ---------------- PERFORM 'END OF INTERFACE SECTION' ACTIONS ------------- */

void nds_end_ifsection () {

#ifdef DEBUG
    fprintf (stdout, "end of interface section\n") ;
    fprintf (stdout, "no. of interfaces: %d ; interface-mask: %#x\n",
								  noif, ifids) ;
    fprintf (stdout, "no. of profiles: %d ; no. of NSAP-addresses: %d\n",
							    noprof, nonetaddr) ;
#endif
    if (noif == 0)
	yyerror ("parser: no interface description found") ;

    /* store all collected information from the interface section into */
    /* NDS-configuration data base */
    nds_store_all() ;
}

/* ---------------- PERFORM 'START OF INTERFACE' ACTIONS ------------------- */

void nds_start_if (signed32	iftype,		/* >  interface type */
		   char		*ifnamep) {	/* >  interface name */

#ifdef DEBUG
    fprintf (stdout, "start of interface '%s' (ID = %d)\n", ifnamep, iftype) ;
#endif
    if (noif == NDS_MX_IF)
	yyerror ("parser: max. no. of interface descriptions exceeded") ;

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
    fprintf (stdout, "end of interface '%s'\n", ifnamep) ;
#endif
    if (strcmp (ifname, ifnamep))
	yyerror ("parser: inconsistent use of interface name") ;
    noif++ ;
}

/* ------------------ PERFORM 'START OF PROFILE' ACTIONS ------------------- */

void nds_start_prof (signed32 profid) {	/* >  profile type */

    signed32	optsize ;

    if (noprof == NDS_MX_PROFILE)
	yyerror ("parser: max. no. of profile descriptions exceeded") ;
    profref->nds_ifid = ifid[noif] ;
    profref->nds_provid = profid ;
    profref->nds_profopt = 0 ;
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
    fprintf (stdout, "start of profile '%d'\n", profid) ;
#endif
}

/* ------------------ PERFORM 'END OF PROFILE' ACTIONS --------------------- */

void nds_end_prof (signed32 profid) {	/* >  profile type */

    signed32		optsize ;

    if (noprofaddr == 0)
	yyerror ("parser: no NSAP-address found") ;
    if (profid != profref->nds_provid)
	yyerror ("parser: inconsistent use of profile-ID") ;
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
    if (profref->nds_ifid == NDS_XTIINFO && profref->nds_devname == 0) {
	if (comdev == 0)
	    yyerror ("parser: syntax error (profile device missing)") ;
	profref->nds_devname = comdev ;
    }
    profref++ ;
    noprof++ ;
    naddrref++ ;
#ifdef DEBUG
    fprintf (stdout, "no. of profile specific NSAP-addresses: %d\n", noprofaddr) ;
    fprintf (stdout, "end of profile '%d'\n", profid) ;
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
    fprintf (stdout, "PROFILE-PRIORITY: %d\n", infoval1) ;
#endif
	profref->nds_profprioid = (signed32) infoval1 ;

	for (profhref = profinfo; profhref < profref; profhref++)
	    if (profhref->nds_profprioid == profref->nds_profprioid)
		yyerror ("parser: profile priority already exists") ;
	break ;
    case NDS_DEVNM_TOK:
#ifdef DEBUG
    fprintf (stdout, "PROFILE-DEVICE: %s\n", infoval2) ;
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

    if (nonetaddr == mxnetaddr)
	yyerror ("parser: max. no. of NSAP-address descriptions exceeded") ;
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
    fprintf (stdout, "NSAP-ADDRESS(GROUP): %s\n", netaddr) ;
#endif
    if (noprofaddr > 0)
	naddrref++ ;
    if (ntype == NDS_GROUP_ADDR) {
	/* compile regular expression describing NSAP-address group */
	if (nds_regcomp (netaddr, &compref, &compsize) != 0)
	    yyerror ("parser: can't compile NSAP-address expression") ;
#ifdef DEBUG
	fprintf (stdout, "NSAP-ADDRESS(GROUP): compiled RE representation\n") ;
	nds_hexdump (stdout, compref, compsize) ;
#endif
	naddrref->nds_mtchtype = NDS_GROUP_ADDR | compsize ;
	naddrref->nds_mtchrule = nds_store_info (compref, compsize) ;
    } else {
	/* convert NSAP-address macro describing a single NSAP-address */
	if ((res = d2p_p01_str_to_bin_psap ("", "", "", 1, &netaddr,
							(byte *) tmpbuf)) < 0) {
	    sprintf (tmpbuf, "parser: can't encode NSAP-address (error = %d)", res) ;
	    yyerror (tmpbuf) ;
	}
#ifdef DEBUG
	fprintf (stdout, "NSAP-ADDRESS: binary representation\n") ;
	nds_hexdump (stdout, (byte *)(nsapref + 1), (signed32) nsapref->t_infolng) ;
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
    fprintf (stdout, "OPTION: type = %#04x ; value = %s\n", opttype, optvalue) ;
#endif
    if (noopt == NDS_MX_OPT)
	yyerror ("parser: max. no. of NSAP-address mapping options exceeded") ;

    switch (optrep) {
    case NDS_EXP_ASCSTR:
	naoptref->nds_optlen =
			  NDS_ALIGN4(sizeof (Nds_opt_str) + strlen (optvalue)) ;
	if ((naoptref->nds_optinfo = tmpref =
	      (void *) malloc ((size_t) naoptref->nds_optlen)) == (void *) NULL)
	    yyerror ("parser: can't allocate memory") ;
	memset (tmpref, (int) NDS_FILLER, (size_t) naoptref->nds_optlen) ;
	((Nds_opt_str*)tmpref)->nds_opt_rep = (byte) optrep ;
	strcpy (((Nds_opt_str*)tmpref)->nds_opt_strval, optvalue) ;
	break ;
    case NDS_EXP_OCTSTR:
	octsize = strlen (optvalue)/2 ;
	naoptref->nds_optlen = NDS_ALIGN4(sizeof (Nds_opt_oct) + octsize - 1) ;
	if ((naoptref->nds_optinfo = tmpref =
	      (void *) malloc ((size_t) naoptref->nds_optlen)) == (void *) NULL)
	    yyerror ("parser: can't allocate memory") ;
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
	      (void *) malloc ((size_t) naoptref->nds_optlen)) == (void *) NULL)
	    yyerror ("parser: can't allocate memory") ;
	memset (tmpref, (int) NDS_FILLER, (size_t) naoptref->nds_optlen) ;
	((Nds_opt_short*)tmpref)->nds_opt_rep = (byte) optrep ;
	((Nds_opt_short*)tmpref)->nds_opt_shortval = (signed16) atoi (optvalue) ;
	break ;
    case NDS_EXP_LONG:
	naoptref->nds_optlen = NDS_ALIGN4(sizeof (Nds_opt_long)) ;
	if ((naoptref->nds_optinfo = tmpref =
	      (void *) malloc ((size_t) naoptref->nds_optlen)) == (void *) NULL)
	    yyerror ("parser: can't allocate memory") ;
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
					(octsize = strlen (optvalue)/2) - 1) :
		 (optrep == NDS_REC_SHORT) ?
		 NDS_ALIGN4(sizeof (Nds_opt_recshort)) :
		 NDS_ALIGN4(sizeof (Nds_opt_reclong)) ;
	if (noopt == 0 || (naoptref - 1)->nds_opttype != opttype) {
	    if ((naoptref->nds_optinfo = tmpref =
			    (void *) malloc ((size_t) tmplen)) == (void *) NULL)
		yyerror ("parser: can't allocate memory") ;
	    naoptref->nds_optlen = tmplen ;
	} else {
	    /* additional recurring option value */
	    naoptref-- ;
	    noopt-- ;
	    if ((naoptref->nds_optinfo =
		(void *) realloc ((void *) naoptref->nds_optinfo,
		    (size_t) (tmplen + naoptref->nds_optlen))) == (void *) NULL)
		yyerror ("parser: can't allocate memory") ;
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
#ifdef DEBUG
    fprintf (stderr, "%s(%d): ERROR -> unknown representation\n", __FILE__, __LINE__) ;
#endif	
	break ;
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
    if ((areasize -= presize) < 0)
	yyerror ("parser: unsufficient work space memory") ;

    optdatref = (byte *) optarea + presize ;
    optarea->nds_opt_types = (OPTFLDTYPE) 0 ;

    for (optno = 0; optno < noopt; optno++, naoptref++) {
	if ((areasize = areasize - naoptref->nds_optlen) < 0)
	    yyerror ("parser: unsufficient work space memory") ;

	optarea->nds_opt_types |= naoptref->nds_opttype ;
	optarea->nds_opt_off[optno] = (OFFSETTYPE) (optdatref -
							     (byte *) optarea) ;
	memcpy (optdatref, naoptref->nds_optinfo, naoptref->nds_optlen) ;
	optdatref += naoptref->nds_optlen ;
    }
    optarea->nds_opt_off[optno] = (OFFSETTYPE) (optdatref - (byte *) optarea) ;
#ifdef DEBUG
    fprintf (stdout, "OPTIONS: binary representation\n") ;
    nds_hexdump (stdout, (byte *)optarea, (signed32) optarea->nds_opt_off[optno]) ;
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
    if (heapsize + tmplen > mxheapsize)
	yyerror ("parser: unsufficient work space memory") ;
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
		!(alloptions & NDS_X121_DNIC_OPT)))
		yyerror ("parser: missing X.121-information (e.g. DCC/DNIC)") ;
	    if (!(alloptions & NDS_E164_IP_OPT) ||
	        !(alloptions & NDS_E164_CC_OPT) ||
		!(alloptions & NDS_E164_NDC_OPT))
		yyerror ("parser: missing E.164-information (e.g. CC/NDC)") ;
	    break ;
	case NDS_X32_SUBPROF:
	case NDS_X121_SUBPROF:
	    if (!(alloptions & NDS_X121_IP_OPT) ||
	       (!(alloptions & NDS_X121_DCC_OPT) &&
		!(alloptions & NDS_X121_DNIC_OPT)))
		yyerror ("parser: missing X.121-information (e.g. DCC/DNIC)") ;
	    break ;
	case NDS_E163_SUBPROF:
	    if (!(alloptions & NDS_E163_IP_OPT) ||
	        !(alloptions & NDS_E163_CC_OPT) ||
	        !(alloptions & NDS_E163_NP_OPT) ||
		!(alloptions & NDS_E163_AC_OPT))
		yyerror ("parser: missing E.163-information (e.g. CC/NP)") ;
	    break ;
	case NDS_E164_SUBPROF:
	    if (!(alloptions & NDS_E164_IP_OPT) ||
	        !(alloptions & NDS_E164_CC_OPT) ||
		!(alloptions & NDS_E164_NDC_OPT))
		yyerror ("parser: missing E.164-information (e.g. CC/NDC)") ;
	    break ;
	case NDS_X21_SUBPROF:
	case NDS_PVC_SUBPROF:
	    break ;
	default:
	    yyerror ("parser: missing (sub-)profile information") ;
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
	perror ("opening configuration data base file failed") ;
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
	perror ("write to configuration data base file failed") ;
	exit (1) ;
    }

    profref = profinfo ;
    for (i = 0; i < noprof; i++, profref++) {
	if (profref->nds_devname)
	    profref->nds_devname += genarea.nds_heap ;	
	if (profref->nds_profopt)
	    profref->nds_profopt += genarea.nds_heap ;	
        if (fwrite (profref, sizeof (Nds_shmifdesc), 1, fd) == 0) {
	    perror ("write to configuration data base file failed") ;
	    exit (1) ;
        }
    }

    ruledesc.nds_mtype = NDS_CMXSEL|NDS_XTISEL|NDS_OSITOXTI|NDS_OSITOSOCK ;
    ruledesc.nds_nomrules = nonetaddr ;
    ruledesc.nds_mrules = genarea.nds_rulearea ;
    if (fwrite (&ruledesc, sizeof (Nds_shmruledesc), 1, fd) == 0) {
	perror ("write to configuration data base file failed") ;
	exit (1) ;
    }

    naddrref = naddrinfo ;
    for (i = 0; i < nonetaddr; i++, naddrref++) {
	naddrref->nds_mtchrule += genarea.nds_heap ;
	if (naddrref->nds_mprule)
	    naddrref->nds_mprule += genarea.nds_heap ;
	if (fwrite (naddrref, sizeof (Nds_shmrule), 1, fd) == 0) {
	    perror ("write to configuration data base file failed") ;
	    exit (1) ;
	}
    }

    if (fwrite (heapstart, heapsize, 1, fd) == 0) {
	perror ("write to configuration data base file failed") ;
	exit (1) ;
    }
}

/* ------------------------------------------------------------------------- */

#ifdef DEBUGLEX
signed32 nds_lexdebug (signed32 type, signed32 val, signed32 line) {

    switch (type) {
    case DBGRETVAL:
	fprintf (stdout, "DBG: lex returns %d at line %d\n", val, line) ;
	fflush (stdout) ;
	return (val) ;
	break ;
    default:
	break ;
    }
}
#endif
