/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: encodepsap.c,v $
 * Revision 1.1.10.2  1996/02/18  18:20:54  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:43:41  marty]
 *
 * Revision 1.1.10.1  1995/12/08  16:07:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:17:06  root]
 * 
 * Revision 1.1.8.2  1994/09/06  12:26:14  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:23:30  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  16:11:10  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:01:28  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  12:40:18  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:30:44  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  21:29:44  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:34:20  bbelch]
 * 
 * Revision 1.1.4.2  1992/12/04  18:11:27  marrek
 * 	November 1992 code drop
 * 	Replace cmx.h by cmx_addr.h
 * 	Replace T_NO_NSAP by T_MAXNSAP
 * 	[1992/12/04  14:03:44  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:00:56  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:52:46  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: encodepsap.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:20:54 $";
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
/* NAME         : encodepsap.c                                        */
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
#include <ctype.h>
#include <string.h>
#include <dce/cmx_addr.h>
#include <dce/osiaddr.h>


/* --------------- DECLARATION OF GLOBAL DATA ----------------------------- */

extern Sel_description	sel_info[] ;	/* selector description table */

/*exon ***********************************************************************/
/*                                                                           */
/*  FUNCTION:  d2p_encode_psap_addr ()                                       */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*    The function encodes a PSAP-address from a presentation-selector, a    */
/*    session-selector and a transport-selector, whereby each of them can    */
/*    be given optionally, and one or several (up to MAX_NSAP) NSAP-         */
/*    addresses.                                                             */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*      p_sel       =  Pointer to a structure (which contains the size of    */
/*		       the presentation (P) selector and a pointer to the    */
/*                     selector value) or a NULL-pointer, if this selector   */
/*                     isn't present.                                        */
/*      s_sel       =  Pointer to a structure (which contains the size of    */
/*		       the session (S) selector and a pointer to the         */
/*                     selector value) or a NULL-pointer, if this selector   */
/*                     isn't present.                                        */
/*      t_sel       =  Pointer to a structure (which contains the size of    */
/*		       the transport (S) selector and a pointer to the       */
/*                     selector value) or a NULL-pointer, if this selector   */
/*                     isn't present.                                        */
/*                     NOTE: The size of a selector value must only be       */
/*                     supplied if the syntax of the selector (see parameter */
/*                     sel_syntax) is of type BINARY. In case of the         */
/*                     syntaxes ASCII and ASCHEX the function expects a      */
/*                     string value from which the size can be determined    */
/*                     automatically.                                        */
/*                                                                           */
/*      sel_syntax  =  Syntax of the selectors, whereby the syntax of a      */
/*                     selector is represented always through two bits.      */
/*                        Bit 2**0 - 2**1 = syntax of the P-selector         */
/*                        Bit 2**2 - 2**3 = syntax of the S-selector         */
/*                        Bit 2**4 - 2**5 = syntax of the T-selector         */
/*                     The following syntaxes are supported:                 */
/*                                00 = ASCII  (string)                       */
/*                                01 = ASCHEX (string)                       */
/*                                02 = BINARY                                */
/*                     NOTE: The syntax field of a selector isn't evaluated  */
/*                     if the corresponding selector isn't present.          */
/*      no_nsap     =  No. of the supplied NSAP-address (1 - MAX-NSAP).      */
/*      nsap_info   =  Pointer to an array, whereby each element of the      */
/*                     array contains the size of a NSAP-address and a       */
/*                     pointer to the NSAP-value. The no. of array elements  */
/*                     must be at least 'no_nsap'.                           */
/*      psap_addr   =  Pointer to the area, where the encoded PSAP-address   */
/*                     should be stored.                                     */
/*                                                                           */
/*  OUTPUT-PARAMETER:                                                        */
/*      *psap_addr  =  Encoded PSAP-address (valid only, if the function     */
/*                     return-value is > 0).                                 */
/*                                                                           */
/*  RETURN-VALUE:                                                            */
/*      > 0         =  Size of the encoded PSAP-address.                     */
/*      < 0         =  An error is occurred during encoding of the PSAP-     */
/*                     address (in this case, the returned negative value    */
/*                     corresponds to the type of error).                    */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: R.Horn                                        DATE: 30.03.90     */
/*exoff **********************************************************************/

signed32 d2p_encode_psap_addr (Psap_info *p_sel, Psap_info *s_sel,
			       Psap_info *t_sel, signed32 sel_syntax,
			       signed32 no_nsap, Psap_info *nsap_info,
							      byte *psap_addr) {

    Psap_info		*sel_ptr[MAX_NO_SEL] ;
    signed32		i, j, sel_size ;
    char			*p ;
    struct t_adrdesc	*descptr, *desc1ptr ;

    descptr = (struct t_adrdesc *) psap_addr ;

    /* verify no. of NSAP-addresses */
    if (no_nsap < 1 || no_nsap > T_MAXNSAP)
	return (NO_NSAP_ERR) ;

    sel_ptr[0] = p_sel ; sel_ptr[1] = s_sel ; sel_ptr[2] = t_sel ;
    for (i = 0; i < MAX_NO_SEL + no_nsap; i++) {
	if (i < MAX_NO_SEL) {
	    /* encode selector(s) into PSAP-address */
	    if (sel_ptr[i] != (Psap_info *) NULL) {
		/* initialize selector area */
		memset ((void *) (descptr + 1), (int) 0,
						(size_t) sel_info[i].sel_size) ;

		/* determine syntax of selector */
		switch ((sel_syntax >> (i * SEL_SYNTAXW)) & SEL_SYNTAXMSK) {
		case SEL_ASCII:
		    /* verify the size of the selector value */
		    if ((sel_size = strlen (sel_ptr[i]->psap_infoval)) >
							   sel_info[i].sel_size)
			return (SEL_SIZE_ERR - i) ;

		    /* verify the syntax of the selector value */
		    for (j = 0, p = sel_ptr[i]->psap_infoval; j < sel_size;
								       j++, p++)
			if (!isprint (*p))
			    return (SEL_SYNTAX_ERR - i) ;

		    /* encode selector value into PSAP-address */
		    strncpy ((char *) (descptr + 1), sel_ptr[i]->psap_infoval,
							    (size_t) sel_size) ;
		    break ;
		case SEL_ASCHEX:
		    /* verify the size of the selector value */
		    if ((sel_size = strlen (sel_ptr[i]->psap_infoval)) >
			      (sel_info[i].sel_size * 2) || (sel_size % 2) != 0)
			return (SEL_SIZE_ERR - i) ;

		    /* verify the syntax of the selector value */
		    for (j = 0, p = sel_ptr[i]->psap_infoval; j < sel_size;
								       j++, p++)
			if (!isxdigit (*p))
			    return (SEL_SYNTAX_ERR - i) ;

		    /* encode selector value into PSAP-address */
		    sel_size = encode_semi_octets (sel_ptr[i]->psap_infoval,
					     sel_size, (byte *) (descptr + 1)) ;
		    break ;
		case SEL_BINARY:
		    /* verify the size of the selector value */
		    if ((sel_size = sel_ptr[i]->psap_infolng) >
							   sel_info[i].sel_size)
			return (SEL_SIZE_ERR - i) ;

		    /* encode selector value into PSAP-address */
		    memcpy ((void *) (descptr + 1),
			 (void *) sel_ptr[i]->psap_infoval, (size_t) sel_size) ;
		    break ;
		default:
		    return (UNKNOWN_SELSYNTAX_ERR) ;
		}
		/* write selector header into PSAP-address */
		descptr->t_infotype = sel_info[i].sel_type ;
		descptr->t_infovers = sel_info[i].sel_vers ;
		descptr->t_infolng = sel_size ;
		descptr->t_translng = descptr->t_nextoff =
			 sizeof (struct t_adrdesc) + ((sel_size - 1) & ~3) + 4 ;
	    } else {
		continue ;
	    }
	} else {
	    /* verify the size of the NSAP-address value */
	    if (nsap_info->psap_infolng > T_NSAPSIZE)
		return (NSAP_SIZE_ERR) ;
	    /* initialize NSAP area and copy NSAP-address value into PSAP-address */
            memset ((void *) (descptr + 1), (int) 0, (size_t) T_NSAPSIZE) ;
	    memcpy ((void *) (descptr + 1), (void *) nsap_info->psap_infoval,
					     (size_t) nsap_info->psap_infolng) ;
	    /* write NSAP-address header into PSAP-address */
	    descptr->t_infotype = T_NSAPINFO ;
	    descptr->t_infovers = T_NSAPVERS ;
	    descptr->t_infolng = nsap_info->psap_infolng ;
	    descptr->t_translng = descptr->t_nextoff =
	    sizeof (struct t_adrdesc) + ((nsap_info->psap_infolng - 1) & ~3) + 4 ;
	    nsap_info++ ;
	}
	/* adjust PSAP-address header */
	desc1ptr = (struct t_adrdesc *) psap_addr ;
	while (desc1ptr < descptr) {
	    desc1ptr->t_translng += descptr->t_translng ;
	    desc1ptr = (struct t_adrdesc *) ((char *) desc1ptr +
							  desc1ptr->t_nextoff) ;
	}
	descptr = (struct t_adrdesc *) ((char *) descptr + descptr->t_nextoff) ;
    }
    return ((signed32) ((byte *) descptr - psap_addr)) ;
}
