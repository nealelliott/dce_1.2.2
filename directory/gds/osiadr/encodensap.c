/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: encodensap.c,v $
 * Revision 1.1.710.2  1996/02/18  18:20:51  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:43:37  marty]
 *
 * Revision 1.1.710.1  1995/12/08  16:07:04  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:17:02  root]
 * 
 * Revision 1.1.708.2  1994/09/06  12:26:12  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:23:15  keutel]
 * 
 * Revision 1.1.708.1  1994/02/22  16:11:08  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:01:10  marrek]
 * 
 * Revision 1.1.706.2  1993/10/14  17:52:27  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:42:46  keutel]
 * 
 * Revision 1.1.706.1  1993/10/13  17:33:52  keutel
 * 	October 1993 code drop
 * 	[1993/10/09  17:42:13  keutel]
 * 
 * Revision 1.1.6.2  1993/08/11  12:38:29  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:30:25  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  20:47:58  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:34:13  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/09  16:18:14  marrek
 * 	November 1992 code drop
 * 	insert casts, remove encode_semi_octets ()
 * 	[1992/12/09  14:34:30  marrek]
 * 
 * Revision 1.1.4.2  1992/12/04  18:10:59  marrek
 * 	November 1992 code drop
 * 	Replace cmx.h by cmx_addr.h
 * 	[1992/12/04  14:02:57  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:00:46  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:52:37  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: encodensap.c,v $ $Revision: 1.1.710.2 $ $Date: 1996/02/18 18:20:51 $";
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
/* NAME         : encodensap.c                                        */
/*								      */
/* AUTHOR       : R. Horn, D AP 11                                    */
/* DATE         : 30.03.90                                            */
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

#include <gds.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <dce/cmx_addr.h> 
#include <dce/osiaddr.h>


/*exon ***********************************************************************/
/*                                                                           */
/*  FUNCTION:  d2p_encode_nsap_addr ()                                       */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*    The function encodes a NSAP-address from an authority and format       */
/*    identifier (AFI) and an initial domain identifier (IDI -> optional),   */
/*    whereby both are parts of the initial domain part (IDP), and some      */
/*    other informations which are considered as components of the domain    */
/*    specific part (DSP -> optional). All calling parameter must meet the   */
/*    requirements which are defined in the NSAP-address description table.  */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*      afi         =  Authority and format identifier (AFI)                 */
/*      idi_valptr  =  Pointer to the initial domain identifier (IDI)        */
/*                     (string) or a NULL-pointer, if there is no IDI        */
/*                     (depends on the AFI value).                           */
/*      dspparts    =  No. of components within the domain specific          */
/*                     part (DSP) or 0, if there is no DSP.                  */
/*      dsp_valptr  =  Pointer to an array, which contains itself the        */
/*                     pointer to the different component values (strings)   */
/*                     of the DSP or a NULL-pointer, if there is no DSP.     */
/*      nsap_addr   =  Pointer to the area, where the encoded NSAP-          */
/*                     address should be stored.                             */
/*                                                                           */
/*  OUTPUT-PARAMETER:                                                        */
/*      *nsap_addr  =  Encoded NSAP-address (valid only, if the function     */
/*                     return-value is > 0).                                 */
/*                                                                           */
/*  RETURN-VALUE:                                                            */
/*      > 0         =  Size of the encoded NSAP-address.                     */
/*      < 0         =  An error is occurred during encoding of the NSAP-     */
/*                     address (in this case, the returned negative value    */
/*                     corresponds to the type of error).                    */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: R.Horn                                        DATE: 30.03.90     */
/*exoff **********************************************************************/


signed32 d2p_encode_nsap_addr (signed32 afi, char *idi_valptr,
		       signed32 dspparts, char *dsp_valptr[], byte *nsap_addr) {

    char		*p, tbuf[(T_NSAPSIZE * 2) + 1] ;
    char		lzero[(T_NSAPSIZE * 2) - 2], lone[MAX_IDI] ;
    char		tmpbuf[MAX_DSP], rngbuf[MAX_DSP] ;
    char		rangetype, unittype, pstr[4] ;
    char		lrngchar, hrngchar, lrngstr[3], hrngstr[3] ;
    signed32		i, k, l, idi_size, dsp_partsize, res, dsp_syntax ;
    Nsap_idpinfo	*nsap_idpptr = (Nsap_idpinfo *) NULL ;
    Nsap_dspinfo	*nsap_dspptr ;

    /* search NSAP-address format description entry (IDP-description) */
    /* opmode can be anything at this level because this call is only */
    /* to find idp entry */
    if ((res = d2p_srch_nsap_form ((signed16) dspparts, &nsap_idpptr, afi, 
				(char *) NULL, (Nsap_dspinfo **) NULL,
					     (char *) NULL, (void *) NULL)) < 0)
	return (res) ;

    /* *** encode AFI and IDI *** */
    dsp_syntax = GET_DSPSYN(nsap_idpptr->geninfo) ;
    if (nsap_idpptr->idi_size > 0) {
	/* verify the size of the IDI */
	if (idi_valptr == (char *) NULL ||
	   (idi_size = (signed32) strlen (idi_valptr)) > nsap_idpptr->idi_size ||
								  idi_size == 0)
	    return (IDI_SIZE_ERR) ;

	/* verify the syntax of the IDI */
	for (i = 0, p = idi_valptr; i < idi_size; i++, p++)
	    if (!isdigit (*p))
		return (IDI_SYNTAX_ERR) ;

	/* encode AFI and IDI into NSAP-address */	
	memset (lzero, LZERO, MAX_IDI) ;
	memset (lone, LONE, MAX_IDI) ;
	sprintf (tbuf, "%2d%.*s%s", afi, (int) (nsap_idpptr->idi_size - idi_size),
		(nsap_idpptr->afi0 == afi) ? lzero : lone, idi_valptr) ;
	if (dsp_syntax != DEC_DSP_SYNTAX &&
				     (strlen (tbuf) % (size_t) 2) != (size_t) 0)
	    strcat (tbuf, ALIGN_VAL) ;
    } else {
	if (idi_valptr != (char *) NULL)
	    return (IDI_SIZE_ERR) ;
	sprintf (tbuf, "%2d", afi) ;
    }

    if (dspparts > 0) {
	/* *** encode DSP *** */
	/* search NSAP-address format description entry (DSP-description) */
	if ((res = d2p_srch_nsap_form ((signed16) dspparts, &nsap_idpptr,
				 (signed32) NULL, (char *) NULL, &nsap_dspptr,
					  idi_valptr, (void *) dsp_valptr)) < 0)
	    return (res) ;

	memset (lzero, LZERO, (T_NSAPSIZE * 2) - 2) ;
	/* decode DSP value(s) from NSAP-address */
	dsp_syntax = GET_DSPSYN(nsap_idpptr->geninfo) ;
	if (dsp_syntax == ISO_DSP_SYNTAX) 
	    /* ISO 646 character DSP-syntax */
	    /* not supported */
	    return (UNSUPPORTED_SYNTAX_ERR) ;
	else if (dsp_syntax == NAT_DSP_SYNTAX)
	    /* National character DSP-syntax */
	    /* not supported */
	    return (UNSUPPORTED_SYNTAX_ERR) ;
	else if ((dsp_syntax != BIN_DSP_SYNTAX) && (dsp_syntax != DEC_DSP_SYNTAX))
	    /* unknown DSP-syntax */
	    return (UNKNOWN_SYNTAX_ERR) ;
	
	for (i = 0; i < dspparts; i++) {

	    DBGPRINT((DEV, "<partno> = %d <dspstr> = %s <encodensap.c>\n", i,
							       dsp_valptr[i])) ;
	    if (GET_DSP_OPTIONAL(nsap_dspptr->dspflag[i]) && 
						       dsp_valptr[i][0] == '\0')
		break ;
	    else if (dsp_valptr[i][0] == '\0')
		return (DSP_SYNTAX_ERR) ;
	    rangetype = GET_DSP_RANGE(nsap_dspptr->dspflag[i]) ;
	    unittype  = GET_DSP_UNIT(nsap_dspptr->dspflag[i]) ;

	    switch (unittype) {
	    case DSP_UNIT_DEC1:
		if (!isdigitstr (dsp_valptr[i])) 
		     return (DSP_SYNTAX_ERR) ;
	    	if (((dsp_partsize = (signed32) strlen (dsp_valptr[i])) > 
	 	    nsap_dspptr->dsp_partsize[i]) || 
		    (dsp_partsize < nsap_dspptr->dsp_partsize_min[i]) || 
							      dsp_partsize == 0)
		    return (DSP_SIZE_ERR) ;
		if (rangetype == DSP_RANGE) {
		    lrngchar = nsap_dspptr->dsp_lowval[i][0] ;
		    hrngchar = nsap_dspptr->dsp_highval[i][0] ;
		    l = (signed32) strlen (dsp_valptr[i]) ;
		    for (k = 0; k < l; k++) 
			if ((dsp_valptr[i][k]<lrngchar) || 
						    (dsp_valptr[i][k]>hrngchar))
			    return (DSP_OUTOF_RANGE) ;
		}
		strcpy (tmpbuf, dsp_valptr[i]) ;
		break ;

	    case DSP_UNIT_DEC2:
		if (dsp_syntax != BIN_DSP_SYNTAX) 
		    return (DSP_UNIT_MISMATCH) ;
		if (!isdigitstr (dsp_valptr[i])) 
		    return (DSP_SYNTAX_ERR) ;
	    	if ((dsp_partsize = (signed32) strlen (dsp_valptr[i])) > 
	 	    nsap_dspptr->dsp_partsize[i] || 
		    (dsp_partsize < nsap_dspptr->dsp_partsize_min[i]) || 
							      dsp_partsize == 0)
		    return (DSP_SIZE_ERR) ;
		if ((dsp_partsize % 2) != 0)
	    	    return (DSP_SYNTAX_ERR) ;
		if (rangetype == DSP_RANGE) {
		    lrngchar = nsap_dspptr->dsp_lowval[i][0] ;
		    hrngchar = nsap_dspptr->dsp_highval[i][0] ;
		    l = (signed32) strlen (dsp_valptr[i]) ;
		    for (k = 0; k < l; k++) 
			if ((dsp_valptr[i][k]<lrngchar) || 
						    (dsp_valptr[i][k]>hrngchar))
			    return (DSP_OUTOF_RANGE) ;
		}
		strcpy (tmpbuf, dsp_valptr[i]) ;
		break ;

	    case DSP_UNIT_HEX1:
	  	if (dsp_syntax != BIN_DSP_SYNTAX)
		    return (DSP_UNIT_MISMATCH) ;
		if (!ishexstr (dsp_valptr[i])) 
		    return (DSP_SYNTAX_ERR) ;
	    	if ((dsp_partsize = (signed32) strlen (dsp_valptr[i])) > 
	 	    nsap_dspptr->dsp_partsize[i] || 
		    dsp_partsize < nsap_dspptr->dsp_partsize_min[i] || 
							      dsp_partsize == 0)
		    return (DSP_SIZE_ERR) ;
		if (rangetype == DSP_RANGE) {
		    lrngchar = toupper (nsap_dspptr->dsp_lowval[i][0]) ;
		    hrngchar = toupper (nsap_dspptr->dsp_highval[i][0]) ;
		    l = (signed32) strlen (dsp_valptr[i]) ;
		    for (k = 0; k < l; k++) 
			if (toupper (dsp_valptr[i][k]) < lrngchar || 
					 toupper (dsp_valptr[i][k]) > hrngchar)
			    return (DSP_OUTOF_RANGE) ;
		}
		strcpy (tmpbuf, dsp_valptr[i]) ;
		break ;

	    case DSP_UNIT_HEX2:
		if (dsp_syntax != BIN_DSP_SYNTAX) 
		    return (DSP_UNIT_MISMATCH) ;
		if (!ishexstr (dsp_valptr[i])) 
		    return (DSP_SYNTAX_ERR) ;
	    	if ((dsp_partsize = (signed32) strlen (dsp_valptr[i])) > 
	 	    nsap_dspptr->dsp_partsize[i] || 
		    (dsp_partsize < nsap_dspptr->dsp_partsize_min[i]) || 
							      dsp_partsize == 0)
		    return (DSP_SIZE_ERR) ;
		if ((dsp_partsize % 2) != 0)
	    	    return (DSP_SYNTAX_ERR) ;
		if (rangetype == DSP_RANGE) {
		    strcpy (lrngstr, nsap_dspptr->dsp_lowval[i]) ;
		    strcpy (hrngstr, nsap_dspptr->dsp_highval[i]) ;
		    l = (signed32) strlen (dsp_valptr[i]) ;
		    for (k = 0; k < l; k += 2) 
			if (strncmp ((char *)&(dsp_valptr[i]), lrngstr, 2) < 0 ||
			      strncmp ((char *)&(dsp_valptr[i]), hrngstr, 2) > 0)
			    return (DSP_OUTOF_RANGE) ;
		}
		strcpy (tmpbuf, dsp_valptr[i]) ;
		break ;
				
	    case DSP_UNIT_ASCII1:
	    	if ((dsp_partsize = 3 * (signed32) strlen (dsp_valptr[i])-6) >
			      nsap_dspptr->dsp_partsize[i] || dsp_partsize == 0)
		    return (DSP_SIZE_ERR) ;
		if ((res = asci_to_hexdec (dsp_valptr[i], tmpbuf, DSP_UNIT_ASCII1,
					 nsap_dspptr->dsp_partsize_min[i])) < 0)
		    return (res) ;
		break ;

	    case DSP_UNIT_ASCII2:
		if (dsp_syntax != BIN_DSP_SYNTAX) 
		    return (DSP_UNIT_MISMATCH) ;
	    	if ((dsp_partsize = 2 * (signed32) strlen (dsp_valptr[i])-4) >
		    nsap_dspptr->dsp_partsize[i] || 
		    (dsp_partsize < nsap_dspptr->dsp_partsize_min[i]) || 
							      dsp_partsize == 0)
		    return (DSP_SIZE_ERR) ;
		if ((res = asci_to_hexdec (dsp_valptr[i], tmpbuf, DSP_UNIT_ASCII2,
					 nsap_dspptr->dsp_partsize_min[i])) < 0)
		    return (res) ;
		break ;

	    case DSP_UNIT_DOT1:
		if ((res = dotdec_to_hexdec (dsp_valptr[i], tmpbuf,
				DSP_UNIT_DOT1, rangetype,
				nsap_dspptr->dsp_lowval[i],
				nsap_dspptr->dsp_highval[i],
				nsap_dspptr->dsp_partsize_min[i]))<0)
		    return (res) ;
		break ;

	    case DSP_UNIT_DOT2:
	  	if (dsp_syntax != BIN_DSP_SYNTAX) 
		    return (DSP_UNIT_MISMATCH) ;
		if ((res = dotdec_to_hexdec (dsp_valptr[i], tmpbuf,
				DSP_UNIT_DOT2, rangetype,
				nsap_dspptr->dsp_lowval[i],
				nsap_dspptr->dsp_highval[i],
				nsap_dspptr->dsp_partsize_min[i]))<0)
		    return (res) ;
		break ;

	    case DSP_UNIT_SLASH11:
		if ((res = slashdec_to_hexdec (dsp_valptr[i], tmpbuf,
				DSP_UNIT_SLASH11, rangetype,
				nsap_dspptr->dsp_lowval[i],
				nsap_dspptr->dsp_highval[i],
				nsap_dspptr->dsp_partsize_min[i]))<0)
		    return (res) ;
		break ;

	    case DSP_UNIT_SLASH12:
		if ((res = slashdec_to_hexdec (dsp_valptr[i], tmpbuf,
				DSP_UNIT_SLASH12, rangetype,
			        nsap_dspptr->dsp_lowval[i],
				nsap_dspptr->dsp_highval[i],
				nsap_dspptr->dsp_partsize_min[i]))<0)
		    return (res) ;
		break ;

	    case DSP_UNIT_SLASH21:
	  	if (dsp_syntax != BIN_DSP_SYNTAX) 
		    return (DSP_UNIT_MISMATCH) ;
		if ((res = slashdec_to_hexdec (dsp_valptr[i], tmpbuf,
				DSP_UNIT_SLASH21, rangetype,
				nsap_dspptr->dsp_lowval[i],
				nsap_dspptr->dsp_highval[i],
				nsap_dspptr->dsp_partsize_min[i]))<0)
		    return (res) ;
		break ;

	    case DSP_UNIT_SLASH22:
		if (dsp_syntax != BIN_DSP_SYNTAX) 
		    return (DSP_UNIT_MISMATCH) ;
		if ((res = slashdec_to_hexdec (dsp_valptr[i], tmpbuf,
				DSP_UNIT_SLASH22, rangetype,
				nsap_dspptr->dsp_lowval[i],
				nsap_dspptr->dsp_highval[i],
				nsap_dspptr->dsp_partsize_min[i])) < 0)
		    return (res) ;
		break ;

	    default :
	       	return (U_DSP_PART_SYNTAX) ;
   	     } /* END OF SWITCH */
		

  	    if (GET_DSP_VAR_ID(nsap_dspptr->dspflag[i])) {
		if (((res = encode_var_part_prefix (&tbuf[strlen (tbuf)],
				nsap_dspptr->dspflag[i], dsp_syntax,
				(signed32) strlen (tmpbuf),
					  nsap_dspptr->dsp_partsize[i])) < 0)) {
		    if (i != (dspparts-1)) 
			 return (res) ;
		} else if ((res + (signed32) strlen (tmpbuf)) >
						   nsap_dspptr->dsp_partsize[i])
		    return (DSP_SIZE_ERR) ;
		  else if (dsp_syntax == BIN_DSP_SYNTAX && 
				   ((res + (signed32) strlen (tmpbuf)) % 2) != 0)
		    return (DSP_BINARY_VAR_ERR) ;
	    } else {
		strcpy (rngbuf, tmpbuf) ;
	 	if (strlen (tmpbuf) < (size_t) nsap_dspptr->dsp_partsize[i]){
		    if (GET_PADCHAR(nsap_dspptr->dspflag[i]) != '-') {
			if (unittype == DSP_UNIT_ASCII1) {
	    		    sprintf (pstr, "%03d", 
				(int)GET_PADCHAR(nsap_dspptr->dspflag[i])) ;
    	   		    if (padunit (tmpbuf, pstr,
				   nsap_dspptr->dsp_partsize[i],
				   GET_DSP_JUST(nsap_dspptr->dspflag[i])) < 0) 
				return (DSP_UNIT_PAD_ERR) ;
			}
			else if (unittype == DSP_UNIT_ASCII2) {
	    		    sprintf (pstr, "%02x", 
				(int)GET_PADCHAR(nsap_dspptr->dspflag[i])) ;
    	   		    if (padunit (tmpbuf, pstr,
				   nsap_dspptr->dsp_partsize[i],
				   GET_DSP_JUST(nsap_dspptr->dspflag[i])) < 0) 
				return (DSP_UNIT_PAD_ERR) ;
			}
			else
			    pad (tmpbuf, GET_PADCHAR(nsap_dspptr->dspflag[i]),
				nsap_dspptr->dsp_partsize[i],
				GET_DSP_JUST(nsap_dspptr->dspflag[i])) ;
		    }
		    pad (rngbuf, '0', nsap_dspptr->dsp_partsize[i], DSP_RIGHT_JUST) ;
		}
		else
		    if (strlen (tmpbuf) != (size_t) nsap_dspptr->dsp_partsize[i])
			return (DSP_SIZE_ERR) ;

		/* This makes the range string comparisons, it must not */
		/* be moved because string comparisons must be made after */
		/* padding the characters */
		if ((unittype == DSP_UNIT_DEC1 ||
		     unittype == DSP_UNIT_DEC2 || unittype == DSP_UNIT_HEX1 ||
		     unittype == DSP_UNIT_HEX2) && 
		     rangetype == DSP_RANGE_STRING && 
		     (strcmp (nsap_dspptr->dsp_lowval[i], rngbuf) > 0 ||
			      strcmp (nsap_dspptr->dsp_highval[i], rngbuf) < 0))
		    return (DSP_OUTOF_RANGE) ;
	    }
	    strcpy (&tbuf[strlen (tbuf)], tmpbuf) ;
	}

	if ((strlen (tbuf) % (size_t) 2) != (size_t) 0) {
	    if (dsp_syntax != BIN_DSP_SYNTAX)
		strcat (tbuf, ALIGN_VAL) ;
	    else
		return (DSP_SYSTEM_ERR) ;
	}
    }
    /* initialize NSAP-address area */
    memset ((void *) nsap_addr, (int) 0, (size_t) T_NSAPSIZE) ;
	
   strcpy ((char *) nsap_addr, tbuf) ;
   return (encode_semi_octets (tbuf, (signed32) strlen (tbuf), nsap_addr)) ; 
}
/*****************************************************************************/
/* Encode the prefix stored in the variable part of DSP	                     */
/*****************************************************************************/

signed32 encode_var_part_prefix (char *str, signed32 dspflag, char dspsyntax,
					     signed32 length, signed32 partsz) {

    signed32	varsize, unitype ;
    char	dspsz[MAX_DSP_VAR_PREFIX] ;
	
    memset ((void *) dspsz, (int) '\0', (size_t) MAX_DSP_VAR_PREFIX) ;
    varsize = GET_DSP_VAR_SIZE(dspflag) ;
    unitype = GET_DSP_UNIT(dspflag) ;
    if (varsize != DSP_VAR_SIZE)
	return (-1) ;
    switch (unitype) {

    case DSP_UNIT_DEC1:
	if (dspsyntax == BIN_DSP_SYNTAX) {
	    if (partsz <= MAX_BIN_VAR_LEN) {
   		sprintf (dspsz, "%.1d", length) ;
		if (strlen (dspsz) != (size_t) 1)
		    return (DSP_WRONG_VAR_SZ) ; 
	    } else {
   		sprintf (dspsz, "%.2d", length) ;
		if (strlen (dspsz) != (size_t) 2)
		    return (DSP_WRONG_VAR_SZ) ; 
	    }
	} else { 
	    if (partsz <= MAX_DEC_VAR_LEN) {
   	        sprintf (dspsz, "%.1d", length) ;
		if (strlen (dspsz) != (size_t) 1)
		    return (DSP_WRONG_VAR_SZ) ; 
	    } else {
		sprintf (dspsz, "%.2d", length) ;
		if (strlen (dspsz) != (size_t) 2)
		    return (DSP_WRONG_VAR_SZ) ; 
	    }
	}
	break ;

    case DSP_UNIT_DEC2:
	if ((length % 2) != 0)
	    return (DSP_WRONG_VAR_LENGTH) ;
	if (dspsyntax == BIN_DSP_SYNTAX) {
   	    sprintf (dspsz, "%.2d", length/2) ;
	    if (strlen (dspsz) != (size_t) 2)
		return (DSP_WRONG_VAR_SZ) ; 
	} else
	    return (DSP_WRONG_UNIT) ; 
	break ;

    case DSP_UNIT_HEX1:
	if (dspsyntax == BIN_DSP_SYNTAX) {
	    if (partsz <= MAX_BIN_VAR_LEN) {
   		sprintf (dspsz, "%.1d", length) ;
		if (strlen (dspsz) != (size_t) 1)
		    return (DSP_WRONG_VAR_SZ) ; 
	    } else {
   	 	sprintf (dspsz, "%.2d", length) ;
		if (strlen (dspsz) != (size_t) 2)
		    return (DSP_WRONG_VAR_SZ) ; 
	    }
	} else
	    return (DSP_WRONG_UNIT) ; 
	break ;

    case DSP_UNIT_HEX2:
	if ((length % 2) != 0)
	    return (DSP_WRONG_VAR_LENGTH) ;
	if (dspsyntax == BIN_DSP_SYNTAX) {
   	    sprintf (dspsz, "%.2d", length/2) ;
	    if (strlen (dspsz) != (size_t) 2)
		return (DSP_WRONG_VAR_SZ) ; 
	} else
	    return (DSP_WRONG_UNIT) ; 
	break ;

    case DSP_UNIT_ASCII1:
	if ((length % 3) != 0)
	    return (DSP_WRONG_VAR_LENGTH) ;
	if (dspsyntax == BIN_DSP_SYNTAX) {
	    if (partsz <= (3*MAX_BIN_VAR_LEN)) {
   		sprintf (dspsz, "%.1d", length/3) ;
		if (strlen (dspsz) != (size_t) 1)
		    return (DSP_WRONG_VAR_SZ) ; 
	    } else {
		sprintf (dspsz, "%.2d", length/3) ;
		if (strlen (dspsz) != (size_t) 2)
		    return (DSP_WRONG_VAR_SZ) ; 
	    }
	} else { 
	    if (partsz <= (3*MAX_DEC_VAR_LEN)) {
   		sprintf (dspsz, "%.1d", length/3) ;
		if (strlen (dspsz) != (size_t) 1)
		    return (DSP_WRONG_VAR_SZ) ; 
	    } else {
		sprintf (dspsz, "%.2d", length/3) ;
		if (strlen (dspsz) != (size_t) 2)
		    return (DSP_WRONG_VAR_SZ) ; 
	    }
	}
	break ;

    case DSP_UNIT_ASCII2:
	if ((length % 2) != 0)
	    return (DSP_WRONG_VAR_LENGTH) ;
	if (dspsyntax == BIN_DSP_SYNTAX) {
   	    sprintf (dspsz, "%.2d", length/2) ;
	    if (strlen (dspsz) != (size_t) 2)
		return (DSP_WRONG_VAR_SZ) ; 
	} else
	    return (DSP_WRONG_UNIT) ;
	break ;

    case DSP_UNIT_DOT1:
 	if ((length % 3) != 0)
	    return (DSP_WRONG_VAR_LENGTH) ;
	if (dspsyntax == BIN_DSP_SYNTAX) {
	    if (partsz <= (3*MAX_BIN_VAR_LEN)) {
   		sprintf (dspsz, "%.1d", length/3) ;
		if (strlen (dspsz) != (size_t) 1)
		    return (DSP_WRONG_VAR_SZ) ; 
	    } else {
		sprintf (dspsz, "%.2d", length/3) ;
		if (strlen (dspsz) != (size_t) 2)
		    return (DSP_WRONG_VAR_SZ) ; 
	    }
	} else { 
	    if (partsz <= (3*MAX_DEC_VAR_LEN)) {
		sprintf (dspsz, "%.1d", length/3) ;
		if (strlen (dspsz) != (size_t) 1)
		    return (DSP_WRONG_VAR_SZ) ; 
	    } else {
		sprintf (dspsz, "%.2d", length/3) ;
		if (strlen (dspsz) != (size_t) 2)
		    return (DSP_WRONG_VAR_SZ) ; 
	    }
	}
	break ;

    case DSP_UNIT_DOT2:
	if ((length % 2) != 0)
	    return (DSP_WRONG_VAR_LENGTH) ;
	if (dspsyntax == BIN_DSP_SYNTAX) {
   	    sprintf (dspsz, "%.2d", length/2) ;
	    if (strlen (dspsz) != (size_t) 2)
		return (DSP_WRONG_VAR_SZ) ; 
	}
	break ;

    case DSP_UNIT_SLASH11:
	if ((length % 3) != 0)
	    return (DSP_WRONG_VAR_LENGTH) ;
	if (dspsyntax == BIN_DSP_SYNTAX) {
	    if (partsz <= (3*MAX_BIN_VAR_LEN)) {
   		sprintf (dspsz, "%.1d", length/3) ;
		if (strlen (dspsz) != (size_t) 1)
		    return (DSP_WRONG_VAR_SZ) ; 
	    } else {
		sprintf (dspsz, "%.2d", length/3) ;
		if (strlen (dspsz) != (size_t) 2)
		    return (DSP_WRONG_VAR_SZ) ; 
	    }
	} else { 
	    if (partsz <= (3*MAX_DEC_VAR_LEN)) {
   		sprintf (dspsz, "%.1d", length/3) ;
		if (strlen (dspsz) != (size_t) 1)
		    return (DSP_WRONG_VAR_SZ) ; 
	    } else {
		sprintf (dspsz, "%.2d", length/3) ;
		if (strlen (dspsz) != (size_t) 2)
		    return (DSP_WRONG_VAR_SZ) ; 
	    }
	}
	break ;

    case DSP_UNIT_SLASH12:
	if ((length % 5) != 0)
	    return (DSP_WRONG_VAR_LENGTH) ;
	if (dspsyntax == BIN_DSP_SYNTAX) {
	    if (partsz <= (5*MAX_BIN_VAR_LEN)) {
		sprintf (dspsz, "%.1d", length/5) ;
		if (strlen (dspsz) != (size_t) 1)
		    return (DSP_WRONG_VAR_SZ) ; 
	    } else {
   		sprintf (dspsz, "%.2d", length/5) ;
		if (strlen (dspsz) != (size_t) 2)
		    return (DSP_WRONG_VAR_SZ) ; 
	    }
	} else { 
	    if (partsz <= (5*MAX_DEC_VAR_LEN)) {
   		sprintf (dspsz, "%.1d", length/5) ;
		if (strlen (dspsz) != (size_t) 1)
		    return (DSP_WRONG_VAR_SZ) ; 
	    } else {
   		sprintf (dspsz, "%.2d", length/5) ;
		if (strlen (dspsz) != (size_t) 2)
		    return (DSP_WRONG_VAR_SZ) ; 
	    }
	}
	break ;

    case DSP_UNIT_SLASH21:
	if ((length % 2) != 0)
	    return (DSP_WRONG_VAR_LENGTH) ;
	if (dspsyntax == BIN_DSP_SYNTAX) {
   	    sprintf (dspsz, "%.2d", length/2) ;
	    if (strlen (dspsz) != (size_t) 2)
		return (DSP_WRONG_VAR_SZ) ; 
	}
	break ;

    case DSP_UNIT_SLASH22:
	if ((length % 4) != 0)
	    return (DSP_WRONG_VAR_LENGTH) ;
	if (dspsyntax == BIN_DSP_SYNTAX) {
   	    sprintf (dspsz, "%.2d", length/4) ;
	    if (strlen (dspsz) != (size_t) 2)
		return (DSP_WRONG_VAR_SZ) ; 
	}
	break ;

    default:
	return (-1) ;
    }
    strcpy (str, dspsz) ;
    return ((signed32) strlen (dspsz)) ;
}
