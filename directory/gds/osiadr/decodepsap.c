/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: decodepsap.c,v $
 * Revision 1.1.10.2  1996/02/18  18:20:50  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:43:35  marty]
 *
 * Revision 1.1.10.1  1995/12/08  16:07:01  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:17:00  root]
 * 
 * Revision 1.1.8.2  1994/09/06  12:26:11  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:23:04  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  16:11:06  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:00:55  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  12:36:21  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:29:57  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  20:47:50  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:34:05  bbelch]
 * 
 * Revision 1.1.4.2  1992/12/04  18:10:15  marrek
 * 	November 1992 code drop
 * 	Replace cmx.h by cmx_addr.h
 * 	Replace T_NO_NSAP by T_MAXNSAP
 * 	[1992/12/04  14:01:37  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:00:34  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:52:29  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: decodepsap.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:20:50 $";
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
/* NAME         : decodepsap.c                                        */
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


/* --------------- DECLARATION OF GLOBAL DATA ----------------------------- */

extern Sel_description	sel_info[] ;	/* selector description table */

/*exon ***********************************************************************/
/*                                                                           */
/*  FUNCTION:  d2p_decode_psap_addr ()                                       */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*    The function decodes a PSAP-address and returns the selector value(s)  */
/*    and also the available NSAP-address values.                            */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*      p_sel       =  Pointer to a structure which contains the pointer to  */
/*                     the location at which the decoded presentation (P)    */
/*                     selector value should be stored.                      */
/*      s_sel       =  Pointer to a structure which contains the pointer to  */
/*                     the location at which the decoded session (S)         */
/*                     selector value should be stored.                      */
/*      t_sel       =  Pointer to a structure which contains the pointer to  */
/*                     the location at which the decoded transport (T)       */
/*                     selector value should be stored.                      */
/*                     NOTE: The reference to the selector value inside of   */
/*                     the PSAP-address instead of the selector value it-    */
/*                     self is returned if the given memory reference is a   */
/*                     NULL-pointer. This will be only done if the requested */
/*                     syntax of the selector (see parameter sel_syntax) is  */
/*                     BINARY.                                               */
/*      sel_syntax  =  Pointer to a variable which contains the requested    */
/*                     syntax of the decoded selectors. The requested        */
/*                     syntax can be expressed always in a two bit field.    */
/*                     Bit 2**0 - 2**1 = requested syntax of the P-selector  */
/*                     Bit 2**2 - 2**3 = requested syntax of the S-selector  */
/*                     Bit 2**4 - 2**5 = requested syntax of the T-selector  */
/*                     The following syntaxes are supported:                 */
/*                                00 = ASCII  (string)                       */
/*                                01 = ASCHEX (string)                       */
/*                                02 = BINARY                                */
/*                     NOTE: If the requested syntax of a selector is ASCII  */
/*                     and the selector can't be represented in this syntax  */
/*                     then the used syntax is ASCHEX.                       */
/*                     If the requested syntax of a selector is ASCHEX and   */
/*                     the selector can be represented in a ASCII syntax     */
/*                     then ASCII syntax is used.                            */
/*      no_nsap     =  Pointer to a variable where the no. of decoded NSAP-  */
/*                     addresses should be stored.                           */
/*      nsap_info   =  Pointer to an array which contains pointer to the     */
/*                     location at which the decoded NSAP-address values     */
/*                     should be stored.                                     */
/*                     NOTE: The reference to the NSAP-address value inside  */
/*                     of the PSAP-address instead of the NSAP-address       */
/*                     value itself is returned if the given memory          */
/*                     reference is a NULL-pointer.                          */
/*                                                                           */
/*      psap_addr   =  Pointer to the PSAP-address which should be decoded.  */
/*                                                                           */
/*  OUTPUT-PARAMETER:                                                        */
/*      *p_sel      =  Pointer to the decoded P-selector value or a NULL-    */
/*                     pointer, if the selector isn't present. Size of the   */
/*                     decoded P-selector.                                   */
/*      *s_sel      =  Pointer to the decoded S-selector value or a NULL-    */
/*                     pointer, if the selector isn't present. Size of the   */
/*                     decoded P-selector.                                   */
/*      *t_sel      =  Pointer to the decoded T-selector value or a NULL-    */
/*                     pointer, if the selector isn't present. Size of the   */
/*                     decoded P-selector.                                   */
/*      *sel_syntax =  Syntax of the decoded selectors.                      */
/*      *no_nsap    =  No. of the decoded NSAP-addresses.                    */
/*      *nsap_info  =  Size of each decoded NSAP-address value and the       */
/*                     pointers to the locations, where the NSAP-values are  */
/*                     stored.                                               */
/*  RETURN-VALUE:                                                            */
/*      NO_ERR      = PSAP-address decoded without any error.                */
/*      < 0         = An error is occurred during decoding of the PSAP-      */
/*                    address (in this case, the returned negative value     */
/*                    corresponds to the type of error).                     */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: R.Horn                                        DATE: 30.03.90     */
/*exoff **********************************************************************/

signed32 d2p_decode_psap_addr (Psap_info *p_sel, Psap_info *s_sel,
			       Psap_info *t_sel, signed32 *sel_syntax,
			       signed32 *no_nsap, Psap_info *nsap_info,
							      byte *psap_addr) {

    Psap_info		*sel_ptr[MAX_NO_SEL] ;
    char		*p ;
    signed32		i, j, sel_size ;
    signed32		req_syntax = *sel_syntax ;
    struct t_adrdesc	*descptr ;

    descptr = (struct t_adrdesc *) psap_addr ;
    *sel_syntax = *no_nsap = 0 ;

    /* decode selector(s) from PSAP-address */
    sel_ptr[0] = p_sel ; sel_ptr[1] = s_sel ; sel_ptr[2] = t_sel ;
    for (i = 0; i < MAX_NO_SEL; i++) {
	if (descptr->t_infotype == sel_info[i].sel_type) {
	    /* verify the size of the selector */
	    if ((sel_size = descptr->t_infolng) > sel_info[i].sel_size)
		return (SEL_SIZE_ERR - i) ;

	    /* verify the request syntax */
	    if (((req_syntax >> (i * SEL_SYNTAXW)) & SEL_SYNTAXMSK) == SEL_BINARY) {
		if (sel_ptr[i]->psap_infoval != (char *) NULL) {
		    /* copy selector value to user area */
		    memcpy ((void *) sel_ptr[i]->psap_infoval,
				    (void *) (descptr + 1), (size_t) sel_size) ;
		    *sel_syntax |= (SEL_BINARY << (i * SEL_SYNTAXW)) ;
		} else
		    /* return only reference to the selector */
		    /* value inside of the PSAP-address */
		    sel_ptr[i]->psap_infoval = (char *) (descptr + 1) ;
		    sel_ptr[i]->psap_infolng = sel_size ;
	    } else {
	 	/* request selector syntax is ASCII-string or */
		/* ASCII-HEX-string */
		/* determine syntax of selector */
		for (j = 0, p = (char *) (descptr + 1); j < sel_size; j++, p++)
		    if (!isprint (*p)) break ;
		if (j == sel_size) {
		    /* syntax is ASCII-string */
		    strncpy (sel_ptr[i]->psap_infoval, (char *) (descptr + 1),
							    (size_t) sel_size) ;
		    *(sel_ptr[i]->psap_infoval + sel_size) = '\0' ;	
		    sel_ptr[i]->psap_infolng = sel_size ;
		    *sel_syntax |= (SEL_ASCII << (i * SEL_SYNTAXW)) ;
		} else {
		    /* syntax is ASCII-HEX-string */
		    decode_semi_octets (sel_ptr[i]->psap_infoval, sel_size * 2,
					 sel_size * 2, (byte *) (descptr + 1)) ;
		    *(sel_ptr[i]->psap_infoval + (sel_size * 2)) = '\0' ;	
		    sel_ptr[i]->psap_infolng = sel_size * 2 ;
		    *sel_syntax |= (SEL_ASCHEX << (i * SEL_SYNTAXW)) ;
		}
	    }	

	    descptr = (struct t_adrdesc *) ((char *) descptr +
							   descptr->t_nextoff) ;
	} else
	    sel_ptr[i]->psap_infoval = (char *) NULL ;
    }

    /* decode NSAP-address(es) from PSAP-address */
    while (*no_nsap < T_MAXNSAP) {
	if (descptr->t_infotype != T_NSAPINFO)
	    return (PSAP_FORMAT_ERR) ;
 	if (descptr->t_infolng > T_NSAPSIZE)
	    return (NSAP_SIZE_ERR) ;

	if (nsap_info->psap_infoval != (char *) NULL)
	    /* copy NSAP-address value to user area */
	    memcpy ((void *) nsap_info->psap_infoval, (void *) (descptr + 1),
						  (size_t) descptr->t_infolng) ;
	else
	    /* return only reference to the NSAP-address */
	    /* value inside of the PSAP-address */
	    nsap_info->psap_infoval = (char *) (descptr + 1) ;
	nsap_info->psap_infolng = descptr->t_infolng ;
	(*no_nsap)++ ;
	nsap_info++ ;
		
	if (descptr->t_translng > descptr->t_nextoff)
	    descptr = (struct t_adrdesc *) ((char *) descptr +
							   descptr->t_nextoff) ;
	else
	    break ;
    }

    return ((*no_nsap > 0 && *no_nsap <= T_MAXNSAP) ? NO_ERR : NO_NSAP_ERR) ;
}
