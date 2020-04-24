/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: hdladmpsap.c,v $
 * Revision 1.1.312.2  1996/02/18  18:20:59  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:43:47  marty]
 *
 * Revision 1.1.312.1  1995/12/08  16:07:18  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:17:12  root]
 * 
 * Revision 1.1.310.4  1994/09/06  12:26:17  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:23:56  keutel]
 * 
 * Revision 1.1.310.3  1994/06/21  14:47:51  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:43:34  marrek]
 * 
 * Revision 1.1.310.2  1994/05/10  15:57:30  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:43:46  marrek]
 * 
 * Revision 1.1.310.1  1994/02/22  16:11:15  marrek
 * 	February 1994 code drop
 * 	[1994/02/15  07:51:07  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:02:03  marrek]
 * 
 * Revision 1.1.308.2  1993/10/14  17:52:30  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:43:07  keutel]
 * 
 * Revision 1.1.308.1  1993/10/13  17:33:57  keutel
 * 	October 1993 code drop
 * 	[1993/10/09  17:42:31  keutel]
 * 
 * Revision 1.1.2.2  1993/08/11  12:43:46  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:31:25  marrek]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: hdladmpsap.c,v $ $Revision: 1.1.312.2 $ $Date: 1996/02/18 18:20:59 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton	***************************************************************/
/*								      */
/* TYPE         : MODUL                                               */
/*								      */
/* NAME         : hdladmpsap.c                                        */
/*								      */
/* AUTHOR       : R. Horn, D AP 11                                    */
/* DATE         : 15.12.92                                            */
/*								      */
/* COMPONENT    : DS                                                  */
/*								      */
/* DOC.-NR.     :                                                     */
/*								      */
/* PRD#/VERS.   :                                                     */
/*								      */
/* DESCRIPTION  :                                                     */
/*								      */
/* SYSTEM DEPENDENCIES:                                               */
/*								      */
/* HISTORY      :                                                     */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/*exon ***********************************************************************/
/*                                                                           */
/*  FUNCTION:  d2p_p01_str_to_bin_psap ()                                    */
/*             d2p_d01_bin_to_str_psap ()                                    */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*    The function d2p_p01_str_to_bin_psap() converts a PSAP-address, where  */
/*    the different address components (P-/S-/T-selector and NSAP(s)) are    */
/*    available in a string representation, into the internally used binary  */
/*    representation. The conversion is done by means of the macros which    */
/*    are defined in the macro file 'nsapmacros'.                            */
/*                                                                           */
/*    The function d2p_d01_bin_to_str_psap() converts a PSAP-address, avail- */
/*    able in the internally used binary representation, into a strings      */ 
/*    representation. The conversion is also done under consideration of     */
/*    of the macro informations defined in the macro file 'nsapmacros'.      */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*      d2p_d01_str_to_bin_psap ()                                           */
/*	psel	    = Pointer to the P-selector string                       */
/*	ssel	    = Pointer to the S-selector string                       */
/*	tsel	    = Pointer to the T-selector string                       */
/*		      Note: if the P-/S- or T-selector is not present, then  */
/*			    the corresponding pointer must be a pointer to   */
/*			    an empty string.                                 */
/*      nonsap      = # of NSAP-address strings.                             */
/*      nsap        = Pointer to an array which contains the pointer to the  */
/*                    different NSAP-address strings.                        */
/*      psap        = Pointer to a location where the converted PSAP-address */
/*                    should be stored.                                      */
/*                                                                           */
/*      d2p_d01_bin_to_str_psap ()                                           */
/*      psap        = Pointer to the binary representation of the PSAP-      */
/*                    address which must be converted.                       */
/*      psel        = Pointer to a memory location where the P-selector      */
/*                    can be stored as a string.                             */
/*      ssel        = Pointer to a memory location where the P-selector      */
/*                    can be stored as a string.                             */
/*      tsel        = Pointer to a memory location where the P-selector      */
/*                    can be stored as a string.                             */
/*                    Note: The size of a memory location reserved for a     */
/*                          selector must be at least (2 * max. selector     */
/*                          size) + 4                                        */
/*      mxnsap	    = # of memory locations described by the nsap parameter  */
/*                    where NSAP-addresses can be stored.                    */
/*      nsap        = Pointer to an array which contains the pointers to     */
/*                    the memory locations where the different NSAP-         */
/*                    addresses can be stored as strings.                    */
/*      nsapsize    = Pointer to an array which contains the sizes of the    */
/*                    memory locations specified by nsap. The size of the    */
/*                    locations depends from the macros in use.              */
/*                                                                           */
/*  OUTPUT-PARAMETER:                                                        */
/*  	d2p_p01_str_to_bin_psap ()                                           */
/*      psap        = PSAP-address in binary representation.                 */
/*                                                                           */
/*      d2p_d01_bin_to_str_psap ()                                           */
/*      psel        = P-selector in string representation.                   */
/*      ssel        = S-selector in string representation.                   */
/*      tsel        = T-selector in string representation.                   */
/*      nsap        = NSAP-address(es) in string representation.             */
/*                    Note: If a PSAP-address component is not available,    */
/*                          then an empty string is written to the           */
/*                          corresponding memory location.                   */
/*  RETURN-VALUE:                                                            */
/*  	d2p_p01_str_to_bin_psap ()                                           */
/*      > 0         = Size of the converted PSAP-address.                    */
/*      < 0         = An error is occurred during address conversion         */
/*                    (in this case, the returned negative value             */
/*                    corresponds to the problem).                           */
/*                                                                           */
/*      d2p_d01_bin_to_str_psap ()                                           */
/*      = 0         = PSAP-address conversion performed successfully.        */
/*      < 0         = An error is occurred during address conversion         */
/*                    (in this case, the returned negative value             */
/*                    corresponds to the problem).                           */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: R.Horn                                        DATE: 15.12.92     */
/*exoff **********************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <dce/osiaddr.h>
#include <dce/cmx_addr.h>


/* ----------------- DEFINITION OF CONSTANTS ------------------------------- */

#define D2P_HEXLABEL	'x'		/* label for HEX-input/output format */
#define D2P_EBCDICLABEL 't'		/* label for ASCII/EBCDIC   format */
#define D2P_HEXDEL	'\''		/* Delimiter for HEX-input/output */
#define D2P_ASCDEL	'\''		/* Delimiter for ASCII-input/output */
#define D2P_SPECFORM	"NA"		/* label for NSAP-address HEX-input/output */
#define D2P_TRDELALL	"\377+"		/* Sum of all normal NSAP-address delimiters */
#define D2P_TRDEL1	'='		/* Delimiters between a macro name */
					/* and macro value resp. a macro */
#define D2P_TRDEL2	'+'		/* Delimiter between NSAP-address parts */
#define D2P_TRDEL2S	"+"
#define D2P_TRDEL3      '!'             /* Delimiter between a macro name */
					/* and the parameter(s) of the macro */
#define D2P_MADELALL    "=!"            /* Sum of all delimiters between a macro*/
					/* name and the value/parameter(s) */
#define D2P_TRDEL4	'.'		/* Delimiter between the values of a */
#define D2P_TRDEL4S	"."		/* dotted input/output value representation */
#define D2P_SPACE	' '		/* Space character */
#define D2P_SUBDEL	'\377'		/* Special filler character */
#define D2P_MACROFILE	"/nsapmacros"	/* NSAP-macro file name */
#define D2P_MAXAP	13		/* Max. no. of NSAP-address parts */
#define D2P_MAXPAR	10		/* Max. no. of macro parameters */
#define D2P_MAXMACROS	20		/* Max. no. of macros which can be */
					/* defined by a user/administrator */
#define D2P_MAXMACROSZ	80		/* Max. size of a macro (name + value) */
#define D2P_NSAP_BUF	80		/* Max. size of an internally used */
					/* NSAP-address buffer */
#define D2P_NO_SEL	3		/* No. of selectors */
#define D2P_SEL_BUF	40		/* Max. size of an internally used */
					/* selector buffer */

/* ----------------- DECLARATION OF LOCAL DATA ----------------------------- */

static signed32		no_macros ;		/* No. of macro table entries */
						/* Input/Output macro table */
static char		*macrotab[2 * D2P_MAXMACROS] ;
#ifdef THREADSAFE
static pthread_once_t	macros_loaded = pthread_once_init ;
static pthread_mutex_t	recursive_opr ;  
#else
static signed32		macros_loaded = FALSE ;		/* Macros loaded-flag */
#endif

/* ------------ PROTOTYPING OF LOCAL FUNCTIONS ----------------------------- */

static signed32 d2p_p02_prepare_sel (char *, Psap_info *) ;
static signed32 d2p_p03_prepare_nsap (char *, Psap_info *) ;
static signed32 d2p_s01_subst_macro (char *, char *[], signed32, signed32,
							     char *, signed32) ;
static signed32 d2p_s02_get_macroname (char *, char *[], signed32, char *,
								     signed32) ;
static signed32 d2p_s03_get_macropar (signed32, char *, char *[], signed32,
							     char *, signed32) ;
static signed32 d2p_s04_get_macros (void) ;

/* ------------------------------------------------------------------------- */
/* - CONVERT PSAP-ADDRESS FROM STRING FORMAT TO THE INTERNALLY USED FORMAT - */
/* ------------------------------------------------------------------------- */

signed32 d2p_p01_str_to_bin_psap (char *psel, char *ssel, char *tsel,
				    signed32 nonsap, char *nsap[], byte *psap) {

    Psap_info	ipsel ;
    Psap_info	issel ;
    Psap_info	itsel ;
    char	iselval[D2P_NO_SEL][D2P_SEL_BUF] ;
    Psap_info	insap[T_MAXNSAP] ;
    char	insapval[T_MAXNSAP][D2P_NSAP_BUF] ;
    signed32	i, result, psyntax, ssyntax, tsyntax ;

    /* read NSAP-address macro informations from macro file */
#ifdef THREADSAFE
    if (pthread_once (&macros_loaded,
			     (pthread_initroutine_t) d2p_s04_get_macros) == -1) 
	return (D2P_S046_ERR) ;
#else
    if (!macros_loaded) {
	if ((result = d2p_s04_get_macros ()) < 0)
	    return (result) ;
	macros_loaded = TRUE ;
    }
#endif

    /* prepare P-selector */
    ipsel.psap_infoval = iselval[0] ;
    psyntax = d2p_p02_prepare_sel (psel, &ipsel) ;

    /* prepare S-selector */
    issel.psap_infoval = iselval[1] ;
    ssyntax = d2p_p02_prepare_sel (ssel, &issel) ;

    /* prepare T-selector */
    itsel.psap_infoval = iselval[2] ;
    tsyntax = d2p_p02_prepare_sel (tsel, &itsel) ;

    /* prepare NSAP-addresses */
    for (i = 0; i < nonsap; i++) {
    	insap[i].psap_infoval = insapval[i] ;
    	if ((result = d2p_p03_prepare_nsap (nsap[i], &insap[i])) < 0)
    	    return (result) ;
    }

    /* encode PSAP-address */
    if ((result = d2p_encode_psap_addr ((ipsel.psap_infoval != (char *) NULL) ?
						&ipsel : (Psap_info *) NULL,
				      (issel.psap_infoval != (char *) NULL) ?
						&issel : (Psap_info *) NULL,
				      (itsel.psap_infoval != (char *) NULL) ?
						&itsel : (Psap_info *) NULL,
				      SEL_SETSYNTAX (psyntax, ssyntax, tsyntax),
						    nonsap, insap, psap)) < 0) {
	DBGPRINT ((DEV, "Can't encode PSAP-address (%d)\n", result)) ;
    }
    return (result) ;
}

/* ------------------------------------------------------------------------- */
/* ---- CONVERT PSAP-ADDRESS FROM INTERNALLY FORMAT INTO STRING FORMAT ----- */
/* ------------------------------------------------------------------------- */

signed32 d2p_d01_bin_to_str_psap (byte *psap, char *psel, char *ssel,
		char *tsel, signed32 mxnsap, char *nsap[], signed32 *nsapsize) {

    Nsap_idpinfo	*nsap_idpptr ;
    Nsap_dspinfo	*nsap_dspptr ;
    Psap_info		selinfo[D2P_NO_SEL] ;
    Psap_info		insap[T_MAXNSAP] ;
    signed32		i, j, afi, s_syntax, nonsap ; 
    signed32		result, dsp_parts, fldno ;
    signed32		dsp_types[D2P_MAXAP];
    char		*p, idi[2 * T_NSAPSIZE] ;
    char		*dsp_valptr[D2P_MAXAP] ;
    char		dsp_values[D2P_MAXAP][2 * T_NSAPSIZE] ;
    char		tmpbuf[D2P_NSAP_BUF] ;

    /* read NSAP-address macro informations from macro file */
#ifdef THREADSAFE
    if (pthread_once (&macros_loaded,
			     (pthread_initroutine_t) d2p_s04_get_macros) == -1) 
	return (D2P_S046_ERR) ;
#else
    if (!macros_loaded) {
	if ((result = d2p_s04_get_macros ()) < 0)
	    return (result) ;
	macros_loaded = TRUE ;
    }
#endif

    *psel = *ssel = *tsel = D2_EOS ;
    for (i = 0; i < mxnsap; i++)
	*nsap[i] = D2_EOS ;

    selinfo[0].psap_infoval = psel + 2 ;
    selinfo[1].psap_infoval = ssel + 2 ;
    selinfo[2].psap_infoval = tsel + 2 ;
    for (i = 0; i < T_MAXNSAP; i++)
    	insap[i].psap_infoval = (char *) NULL ;

    for (i = 0; i < D2P_MAXAP; i++)
    	dsp_valptr[i] = dsp_values[i] ;

    s_syntax = SEL_SETSYNTAX (SEL_ASCII, SEL_ASCII, SEL_ASCII) ;
    if ((result = d2p_decode_psap_addr (selinfo, &selinfo[1], &selinfo[2],
					&s_syntax, &nonsap, insap, psap)) < 0) {
    	DBGPRINT ((DEV, "Can't decode PSAP-address (%d)\n", result)) ;
    	return (result) ;
    }
    if (nonsap > mxnsap)
	/* # of available NSAP-addresses exceeds # of memory locations */
	return (NO_NSAP_ERR) ;

    /* change selectors into special input/output format */
    for (j = 0; j < D2P_NO_SEL; j++) {
	if (j == 0) {
	    p = psel ; fldno = SEL_P_FLD ;
	} else {
	    if (j == 1) {
		p = ssel ; fldno = SEL_S_FLD ;
	    } else {
		p = tsel ; fldno = SEL_T_FLD ;
	    }
	}
	if (SEL_GETSYNTAX(s_syntax, fldno) == SEL_ASCHEX) {
	    *p = D2P_HEXLABEL ;
	    *(p+1) = D2P_HEXDEL ;
	    i = (signed32) strlen (p) ;
	    *(p+i) = D2P_HEXDEL ;
	    *(p+i+1) = D2_EOS ;
        }
	else if (SEL_GETSYNTAX(s_syntax, fldno) == SEL_ASCHEX) {
	    *p = D2P_EBCDICLABEL ;
	    *(p+1) = D2P_HEXDEL ;
	    i = (signed32) strlen (p) ;
	    *(p+i) = D2P_HEXDEL ;
	    *(p+i+1) = D2_EOS ;
	} else {
	    if (selinfo[j].psap_infoval == (char *) NULL)
		/* no selector available */
		*p = D2_EOS ;
	    else {
		if (selinfo[j].psap_infolng == 0) {
		    /* selector of zero size */
		    *p = D2P_HEXDEL ;
		    *(p+1) = D2_EOS ;
		} else {
		    strcpy (p, p + 2) ; i = (signed32) strlen (p) ;
		    if (*(p+i-1) == D2P_SPACE) {
			*(p+i) = D2P_HEXDEL ;
			*(p+i+1) = D2_EOS ;
		    }
		}
	    }	
	}
    }

    for (i = 0; i < nonsap; i++) {
	DBGPRINT ((DEV, "ENCODED NSAP-ADDRESS:  ")) ;
	for (j = 0, p = insap[i].psap_infoval; j < insap[i].psap_infolng; j++, p++) {
	    DBGPRINTC ((DEV, "%02x ", *p & 0xFF)) ;
	}
	DBGPRINTC ((DEV, "\n")) ;

	if ((result = d2p_decode_nsap_addr (DEC_DISPLAY, &afi, idi,
				&dsp_parts, dsp_valptr, dsp_types, 
				(byte *) insap[i].psap_infoval,
				insap[i].psap_infolng)) < 0) {
		DBGPRINT ((DEV, "return code (%d)\n\n", result)) ;
	    if (result == NSAP_SIZE_ERR) {
		DBGPRINT ((DEV, "Can't decode %d. NSAP-address (%d)\n", i+1, result)) ;
		return (result) ;
	    }
	    /* display NSAP-address in a hex-format */
	    p = nsap[i] ;
	    strcpy (p, D2P_SPECFORM) ;
	    strcat (p, D2P_TRDEL2S) ;
	    j = (signed32) strlen (p) ;
	    decode_semi_octets (p + j, insap[i].psap_infolng * 2,
		    insap[i].psap_infolng * 2, (byte *) insap[i].psap_infoval) ;
	    *(p + j + (insap[i].psap_infolng * 2)) = D2_EOS ;
	    continue ;
	}

	DBGPRINT ((DEV, "TOTAL NUMBER OF DSP PARTS: (%d) ",dsp_parts)) ;
	/* search NSAP-address format description entries */
	/* (IDP-/DSP-description) */
	nsap_idpptr = (Nsap_idpinfo *) NULL ;
	if (dsp_parts > 0)
		(void) d2p_srch_nsap_form (dsp_parts, &nsap_idpptr, afi, 
			(char *) NULL, &nsap_dspptr, idi, (void *) dsp_valptr) ;
        else 
		(void) d2p_srch_nsap_form (0, &nsap_idpptr, afi, 
				(char *) NULL, (Nsap_dspinfo **) NULL, idi, 
				(void *) dsp_valptr) ;

	p = tmpbuf ;
	sprintf (p, "%s", nsap_idpptr->idp_fname) ;
	if (strlen (idi) > (size_t) 0) {
	    /* print IDI information */
	    strcat (p, D2P_TRDEL2S) ;
	    strcat (p, idi) ;
	}
	if (dsp_parts > 0) {
	    /* print DSP information */
	    strcat (p, D2P_TRDEL2S) ;
	    strcat (p, nsap_dspptr->dsp_fname) ;
	    for (j = 0; j < dsp_parts; j++) {
		strcat (p, D2P_TRDEL2S) ;
		strcat (p, dsp_valptr[j]) ;
	    }
	}
	DBGPRINT ((DEV, "return code (%s)\n\n", p)) ;
	/* if available, get macro parameter(s) */
	if ((result = d2p_s03_get_macropar (GET_DSPSYN(nsap_idpptr->geninfo),
		    tmpbuf, macrotab, no_macros, nsap[i], *(nsapsize + i))) < 0)
	    return (result) ;
	if (result > 0)
	    continue ; 

	/* if available, get macro name(s) */
#ifdef THREADSAFE
	if (pthread_mutex_lock (&recursive_opr) == -1)
	    return (D2P_S047_ERR) ;
#endif
	result = d2p_s02_get_macroname (tmpbuf, macrotab, no_macros, nsap[i],
								  *(nsapsize)) ;
#ifdef THREADSAFE
	if (pthread_mutex_unlock (&recursive_opr) == -1)
	    return (D2P_S047_ERR) ;
#endif
	if (result < 0)
	    return (result) ; 
    }
    return (0) ;
}

/* ------------------------------------------------------------------------- */
/* -- CONVERT SELECTORS FROM SCREEN FORMAT INTO THE INTERNALLY USED FORMAT - */
/* ------------------------------------------------------------------------- */

static signed32 d2p_p02_prepare_sel (char *isel, Psap_info *osel) {
    signed32	len ;
    char	tmpbuf[D2P_SEL_BUF];

    len = (signed32) strlen (isel) ;
    if (len == 0) {
	osel->psap_infolng = 0 ;
	osel->psap_infoval = (char *) NULL ;
	return (SEL_ASCII) ;
    }
    if (len > 3 && *isel == D2P_HEXLABEL && *(isel + 1) == D2P_HEXDEL &&
					      *(isel + len - 1) == D2P_HEXDEL) {
	/* selector is given as a HEX(ASCII)-string */
	osel->psap_infolng = len - 3 ;
	strncpy (osel->psap_infoval, isel + 2, len - 3) ;
	*(osel->psap_infoval + len - 3) = D2_EOS ;
	return (SEL_ASCHEX) ;
    }
    if (len > 3 && *isel == D2P_EBCDICLABEL && *(isel + 1) == D2P_HEXDEL &&
					      *(isel + len - 1) == D2P_HEXDEL) {
	/* selector is given as a HEX(ASCII)-string */
	osel->psap_infolng = len - 3 ;
	strncpy (tmpbuf, isel + 2, len - 3) ;
	*(tmpbuf + len - 3) = D2_EOS ;
	asc_ebc(osel->psap_infoval, tmpbuf, len-3);
	*(osel->psap_infoval + len - 3) = D2_EOS ;
	return (SEL_ASCEBCDIC) ;
    }

    /* selector is given as a ASCII-string */
    if (*(isel + len - 1) == D2P_HEXDEL) {
	osel->psap_infolng = len - 1 ;
	strncpy (osel->psap_infoval, isel, len - 1) ;
	*(osel->psap_infoval + len - 1) = D2_EOS ;
    } else {
	osel->psap_infolng = len ;
	strcpy (osel->psap_infoval, isel) ;
    }
    return (SEL_ASCII) ;
}

/* ------------------------------------------------------------------------- */
/* - CONVERT NSAP-ADDRESS FROM SCREEN FORMAT TO THE INTERNALLY USED FORMAT - */
/* ------------------------------------------------------------------------- */

static signed32 d2p_p03_prepare_nsap (char *insap, Psap_info *onsap) {
    char		*aparts[D2P_MAXAP], *p ;
    char		buf[D2P_NSAP_BUF] ;
    signed32		i, afi, nsapsize, dsp_syntax, result, no_aparts = 0 ;
    signed32		res ;
    Nsap_idpinfo	*nsap_idpptr = (Nsap_idpinfo *) NULL ;

    /* substitute all macros which are available in the NSAP-address */
#ifdef THREADSAFE
    if (pthread_mutex_lock (&recursive_opr) == -1)
	return (D2P_S047_ERR) ;
#endif
    result = d2p_s01_subst_macro (insap, macrotab, no_macros, 1, buf,
								 D2P_NSAP_BUF) ;
#ifdef THREADSAFE
    if (pthread_mutex_unlock (&recursive_opr) == -1)
	return (D2P_S047_ERR) ;
#endif
    if (result < 0)
	return (result) ; 

    /* split NSAP-address information into specific parts */
    DBGPRINT ((DEV, "NSAP-address: %s\n", buf)) ;

    aparts[no_aparts++] = strtok (buf, D2P_TRDEL2S) ;
    while (no_aparts < D2P_MAXAP && (aparts[no_aparts] =
			  strtok ((char *) NULL, D2P_TRDEL2S)) != (char *) NULL)
    {
	no_aparts++ ;
    }
    if (no_aparts < 2) {
	DBGPRINT ((DEV, "Wrong no. of NSAP-address parts (< 2)\n")) ;
	return (D2P_P032_ERR) ;
    }

    /* evaluate all parts of the NSAP-address */
    if (strcmp (aparts[0], D2P_SPECFORM) == 0) {
	/* special format -> NSAP-address is given as an octet string */
	if (no_aparts != 2) {
	    DBGPRINT ((DEV, "Wrong no. of NSAP-address parts (!= 2)\n")) ;
	    return (D2P_P032_ERR) ;
	}

	/* verify size and format of given address */
	if ((nsapsize = (signed32) strlen (aparts[1])) < 2 ||
							  (nsapsize % 2) != 0) {
	    DBGPRINT ((DEV, "Wrong size of NSAP-address (< MIN)\n")) ;
		return (D2P_P033_ERR) ;
	}

	/* verify AFI */
	sscanf (aparts[1], "%2d", &afi) ;
	/* search NSAP-address format description entry (IDP-description) */
	/* 0 as a parameter to function, at this level it doesn't matter  */
	/* because number of parts are not known and function will return */
	/* before evaluating the opmode 				  */
	if ((res = d2p_srch_nsap_form (0, &nsap_idpptr, afi, (char *) NULL,
		  (Nsap_dspinfo **) NULL, (char *) NULL, (char **) NULL)) < 0) {
	    DBGPRINT ((DEV, "Unknown NSAP-address type (AFI)\n")) ;
	    return (res) ;
	}
	if (nsapsize < (2 + nsap_idpptr->idi_size) || nsapsize >
			  (2 + nsap_idpptr->idi_size + nsap_idpptr->dsp_size)) {
	    DBGPRINT ((DEV, "Wrong size of NSAP-address (< MIN or > MAX)\n")) ;
	    return (D2P_P033_ERR) ;
	}
	if ((dsp_syntax = GET_DSPSYN(nsap_idpptr->geninfo)) == DEC_DSP_SYNTAX) {
	    for (i = 0, p = aparts[1]; *p; p++)
		if (!isdigit (*p)) {
		    if (*(p+1) != '\0' || (*p != ALIGN_VAL[0] &&
						 *p != tolower(ALIGN_VAL[0]))) {
			DBGPRINT ((DEV, "NSAP-address contains invalid character (> %c)\n", *p)) ;
			return (D2P_P034_ERR) ;
		    }
		}
	} else {
	    if (dsp_syntax == BIN_DSP_SYNTAX) {
		for (i = 0, p = aparts[1]; *p; p++)
		    if (!isxdigit (*p)) {
			DBGPRINT ((DEV,
			"NSAP-address contains invalid character (> %c)\n", *p)) ;
			return (D2P_P034_ERR) ;
		    }
	    } else {
		DBGPRINT ((DEV, "Unknown DSP-syntax\n")) ;
		return (D2P_P035_ERR) ;
	    }
	}

	/* convert NSAP-address from an ASCII- to an Octet-string */
	encode_semi_octets (aparts[1], nsapsize, (byte *) onsap->psap_infoval) ;
	onsap->psap_infolng = nsapsize/2 ;
	return (NO_ERR) ;	
    } else {
	/* Changes made in the function parameters */
	if ((res = d2p_srch_nsap_form (0, &nsap_idpptr, (signed32) NULL,
					aparts[0], (Nsap_dspinfo **) NULL,
					  (char *) NULL, (char **) NULL)) < 0) {
	    DBGPRINT ((DEV, "Unknown NSAP-address type (AFI)\n")) ;
	    return (res) ;
	}
    /* Search for the NSAP-address format description entry (IDP-description */
    DBGPRINT ((DEV, "AFI1= %d AFI2 = %d pa = %s pn = %d\n", nsap_idpptr->afi0, \
		nsap_idpptr->afi1,aparts[1],no_aparts)) ;
    if (nsap_idpptr->idi_size > 0)
	onsap->psap_infolng = d2p_encode_nsap_addr ((*aparts[1] != LZERO) ?
				nsap_idpptr->afi0 : nsap_idpptr->afi1,
				aparts[1], (no_aparts > 2) ?
		  no_aparts - 3 : 0, &aparts[3], (byte *) onsap->psap_infoval) ;
    else
	onsap->psap_infolng = d2p_encode_nsap_addr (nsap_idpptr->afi0,
				(char *) NULL, no_aparts - 2, &aparts[2],
				(byte *) onsap->psap_infoval) ;
    if (onsap->psap_infolng < 0)
	    DBGPRINT ((DEV, "Can't encode NSAP-address (%d)\n", \
							onsap->psap_infolng)) ;
	return (onsap->psap_infolng) ;
    } 
}	

/* ------------------------------------------------------------------------- */
/* -------- SUBSTITUTE NORMAL MACROS AND MACROS WITH PARAMETER ------------- */
/* ------------------------ IN A NSAP-ADDRESS ------------------------------ */
/* ------------------------------------------------------------------------- */

static signed32 d2p_s01_subst_macro (char *istring, char *macros[],
	    signed32 max_macro, signed32 level, char *obuf, signed32 obufsize) {
    static char		*obufptr ;
    static signed32	osize ;

    signed32		act_macro, macro_nmsz, result, no_par = 0 ;
    char		*macropar[D2P_MAXPAR], ibuf[D2P_NSAP_BUF],
			macroval[D2P_MAXMACROSZ],
			*po = ibuf, *pi ;

    obufptr = obuf ;
    osize = obufsize ;
    while (*istring != D2_EOS) {
	for (act_macro = 0; act_macro < max_macro; act_macro += 2) {
	    /* search for macro name */
	    macro_nmsz = (signed32) strlen (macros[act_macro]) ;
	    if (strncmp (macros[act_macro], istring, macro_nmsz) == 0)
		break ;
	}
	if (act_macro < max_macro) {
	    istring += macro_nmsz ;
	    if (*(istring-1) == D2P_TRDEL3) {
		if (level > 1) {
		    DBGPRINT ((DEV, "Macro with parameter occurs at a wrong level\n")) ;
		    return (D2P_S012_ERR) ;
		}

		/* substitute special macro parameter name delimiter */
		strcpy (macroval, macros[act_macro+1]) ;
				
		/* macro name with parameter found */
		/* evaluate macro parameter */
		pi = istring ;
		do {
		    *po++ = *pi ;
		    if (*pi == D2P_TRDEL1)
			*po++ = D2P_SUBDEL ;
		} while (*(pi++)) ;
		while (no_par < D2P_MAXPAR &&
			(macropar[no_par] = strtok ((no_par == 0) ?
		        ibuf : (char *) NULL, D2P_TRDELALL)) != (char *) NULL)
		    no_par++ ;
		if (no_par == D2P_MAXPAR) {
		    DBGPRINT ((DEV, "Max. no. of macro parameter exceeded (> %d)\n", \
								D2P_MAXPAR)) ;
		    return (D2P_S013_ERR) ;
		}
 		if ((no_par % 2) != 0) {
		    DBGPRINT ((DEV, "Wrong macro parameter format\n")) ;
		    return (D2P_S014_ERR) ;
		}

		/* substitute macro parameter */
		if ((result = d2p_s01_subst_macro (macroval, macropar,
					  no_par, ++level, obufptr, osize)) < 0)
		    return (result) ;
		break ;
	    } else {
		/* macro name found -> substitute macro name */
		if ((result = d2p_s01_subst_macro (macros[act_macro+1], macros,
				       max_macro, ++level, obufptr, osize)) < 0)
		    return (result) ;
	    }
	} else {
	    if (osize-- <= 0) {
		DBGPRINT ((DEV, "Macro substitution exceeds available buffer size\n")) ;
		return (D2P_S015_ERR) ;
	    }
	    *(obufptr++) = *(istring++) ;
	}
    }
    *obufptr = D2_EOS ;
    return (NO_ERR) ;
}

/* ------------------------------------------------------------------------- */
/* ---------- CONVERT NSAP-ADDRESS INTO A NORMAL MACRO FORMAT -------------- */
/* ------------------------------------------------------------------------- */

static signed32 d2p_s02_get_macroname (char *istring, char *macros[],
			    signed32 max_macro, char *obuf, signed32 obufsize) {
    static char		*obufptr ;
    static signed32	osize ;

    signed32		act_macro ;
    char		ibuf[D2P_NSAP_BUF] ;

    obufptr = obuf ;
    osize = obufsize ;
    while (*istring != D2_EOS) {
	for (act_macro = 0; act_macro < max_macro; act_macro += 2) {
	    /* search for macro value */
	    if (strncmp (macros[act_macro+1], istring,
					     strlen (macros[act_macro+1])) == 0)
		break ;
	}
	if (act_macro < max_macro) {
	    /* macro value found -> get macro name */
	    istring += (signed32) strlen (macros[act_macro+1]) ;
	    strcpy (ibuf, macros[act_macro]) ;
	    strcat (ibuf, istring) ;
	    return (d2p_s02_get_macroname (ibuf, macros, max_macro,
							obufptr, osize)) ;
	} else {
	    if (osize-- <= 0) {
		DBGPRINT ((DEV, "Macro value substitution exceeds available buffer size\n")) ;
		return (D2P_S022_ERR) ;
	    }
	    *(obufptr++) = *(istring++) ;
	}
    }
    *obufptr = D2_EOS ;
    return (NO_ERR) ;
}

/* ------------------------------------------------------------------------- */
/* ------- CONVERT NSAP-ADDRESS INTO A 'MACRO WITH PARAMETER' FORMAT ------- */
/* ------------------------------------------------------------------------- */

static signed32 d2p_s03_get_macropar (signed32 dspsyntax, char *istring,
	    char *macros[], signed32 max_macro, char *obuf, signed32 obufsize) {

    signed32	act_macro, i, osize,
		ino_par = 0, pno_par, par_pos ;
    char	*p, *p1 ;
    char	ibuf[D2P_NSAP_BUF], *ibufptr[D2P_MAXAP] ;
    char	pbuf[D2P_NSAP_BUF], *pbufptr[D2P_MAXAP] ;
    char	hbuf[D2P_NSAP_BUF] ;

    /* extract all address parts from the given NSAP-address */
    strcpy (ibuf, istring) ;
    while ((ibufptr[ino_par] = strtok ((ino_par == 0) ?
				   ibuf : NULL, D2P_TRDEL2S)) != (char *) NULL)
	ino_par++ ;

    for (act_macro = 0; act_macro < max_macro; act_macro += 2) {
	/* search in the macro table for a corresponding macro */
	/* with parameter */
	p = macros[act_macro] + (signed32) strlen (macros[act_macro]) - 1 ;
	if (*p != D2P_TRDEL3)
	    continue ;
	/* extract all address parts from the actual macro value */
	strcpy (pbuf, macros[act_macro+1]) ;
	pno_par = par_pos = 0 ;
	while ((pbufptr[pno_par] = strtok ((pno_par == 0) ?
				   pbuf : NULL, D2P_TRDEL2S)) != (char *) NULL)
	    pno_par++ ;
	if (ino_par != pno_par)
	    continue ;
	/* compare all address parts of NSAP-address and macro value */
	for (i = 0; i < ino_par; i++) {
	    /* search for address part(s) which are macro parameter */
	    p1 = pbufptr[i] + (signed32) strlen (pbufptr[i]) - 1 ;
	    if (*p1 == D2P_TRDEL1 ) {
		/* macro parameter found -> set position flag */
		par_pos |= (1 << i) ;
		continue ;
	    }
	    if (strcmp (ibufptr[i], pbufptr[i]) != 0)
		break ;
	}
	if (i == ino_par)
	    break ;
    }
    if (act_macro >= max_macro)
	return (NO_ERR) ;

    /* macro with parameter(s) found -> build up NSAP-address in the */
    /* 'macro with parameter' format */
    if ((osize = (signed32) strlen (macros[act_macro])) >= obufsize)
	return (D2P_S032_ERR) ;
    strcpy (obuf, macros[act_macro]) ;
    obufsize -= osize ;
    for (i = 0; i < ino_par; i++, par_pos >>= 1) {
	if ((par_pos & 1) != 0) {
	    strcpy(hbuf, ibufptr[i]);
	    if ((osize = (signed32) strlen (pbufptr[i]) +
			 (signed32) strlen (hbuf) +
				     (sizeof (D2P_TRDEL2S) - 1)) >= obufsize)
		return (D2P_S032_ERR) ;
	    strcat (obuf, pbufptr[i]) ;
	    strcat (obuf, hbuf) ;
	    obufsize -= osize ;
	    if (par_pos > 1) {
		strcat (obuf, D2P_TRDEL2S) ;
		obufsize -= (sizeof (D2P_TRDEL2S) - 1) ;
	    }
	}
    }
    return (ino_par) ;
}

/* ------------------------------------------------------------------------- */
/* -------------- READ NSAP-ADDRESS MACROS FROM MACRO FILE ----------------- */
/* ------------------------------------------------------------------------- */

static signed32 d2p_s04_get_macros () {
    FILE	*fd ;
    char	*p, *p1, *p2 ;
    signed32	len, i ;
    struct stat fstatus ;
    char	tmpbuf[D27_LFILE_NAME+1000] ;

#ifdef THREADSAFE
    /* initialize mutex for recursive macro handling functions */
    /* using global variables */
    if (pthread_mutex_init (&recursive_opr, pthread_mutexattr_default) == -1)
	return (D2P_S047_ERR) ;
#endif

    /* open macro file */
    sprintf (tmpbuf, "%s%s%s%s", dcelocal_path, D27_CLIENT_DIR,
				     D27_ADM_DIR, D2P_MACROFILE) ;
    if ((fd = fopen (tmpbuf, "r")) == (FILE *) NULL) {
	if (errno != ENOENT) {
	    DBGPRINT ((DEV, "Can't open macro file '%s' (%d)\n", tmpbuf, errno)) ;
	    return (D2P_S043_ERR) ;
	} 
	/* no macro file available */
	return (NO_ERR) ;
    }

    /* get size of macro file */
    if (fstat (fileno (fd), &fstatus) < 0) {
	DBGPRINT ((DEV, "Can't get file state of macro file (%d)\n", errno)) ;
	return (D2P_S044_ERR) ;
    }

    if ((p = malloc (fstatus.st_size + D2P_MAXMACROS)) == (char *) NULL) {
	DBGPRINT ((DEV, "Can't get system memory for holding macro information\n")) ;
	return (D2P_S045_ERR) ;
    }

    /* read all macros from file */
    while (fgets (p, D2P_MAXMACROSZ, fd) == p &&
					      no_macros < (2 * D2P_MAXMACROS)) {
        /* skip comment line */
        while (*p && (*p == ' ' || *p == '\t'))
            p++;
        if (*p == '#')
            continue;

	/* verify macro syntax */
	if ((len = (signed32) strlen(p)) < 4 || *(p + len - 1) != '\n' ||
			    (p1 = strpbrk (p, D2P_MADELALL)) == (char *) NULL) {
	    DBGPRINT ((DEV, "WARNING: Wrong format of %d. macro -> macro ignored\n", \
							(no_macros/2) + 1)) ;
	    continue ;
	}
	p2 = p + len ;
	while (--p2 > p1)
	    *p2 = *(p2-1) ;
	*(p1+1) = D2_EOS ;
	macrotab[no_macros++] = p ;
	macrotab[no_macros++] = p1 + 2 ;
	p += len + 1 ;
    }

    DBGPRINT ((DEV, "CONTENT OF THE MACRO-TABLE: (no_macros == %d)\n",no_macros)) ;
    for (i = 0; i < no_macros; i += 2) {
	DBGPRINTC ((DEV, "%d. MACRO\n\tNAME:  %.60s\n\tVALUE: %.60s\n", \
					(i/2)+1, macrotab[i], macrotab[i+1])) ;
    }
    DBGPRINTC ((DEV, "END OF THE MACRO-TABLE\n")) ;

    fclose (fd) ;
    return (NO_ERR) ;
}
