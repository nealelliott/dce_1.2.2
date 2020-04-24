/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cmposiaddr.c,v $
 * Revision 1.1.10.2  1996/02/18  18:20:44  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:43:26  marty]
 *
 * Revision 1.1.10.1  1995/12/08  16:06:47  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:16:53  root]
 * 
 * Revision 1.1.8.2  1994/09/06  12:26:07  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:22:32  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  16:10:55  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:00:08  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  12:32:10  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:29:18  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  20:47:35  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:33:49  bbelch]
 * 
 * Revision 1.1.4.2  1992/12/04  18:09:52  marrek
 * 	November 1992 code drop
 * 	Replace cmx.h by cmx_addr.h
 * 	[1992/12/04  13:59:45  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:59:51  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:52:00  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: cmposiaddr.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:20:44 $";
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
/* NAME         : cmposiaddr.c                                        */
/*								      */
/* AUTHOR       : R. Horn, D AP 11                                    */
/* DATE         : 05.04.90                                            */
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
/*  FUNCTION:  d2p_psap_addr_match ()                                        */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*   The function verifies whether the presented PSAP-address matches to the */
/*   stored PSAP-address. Following the matching rules of a PSAP-address     */
/*   the two PSAP-addresses matches if all selectors of the presented PSAP-  */
/*   address are equal to all selectors of the stored PSAP-address and if    */
/*   the NSAP-address(es) of the presented PSAP-address is (are) a subset    */
/*   of the NSAP-address(es) of the stored PSAP-address.                     */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*      psap1       =  Pointer to the presented PSAP-address.                */
/*      psap2       =  Pointer to the stored PSAP-address.                   */
/*                                                                           */
/*  OUTPUT-PARAMETER:                                                        */
/*                    None                                                   */
/*                                                                           */
/*  RETURN-VALUE:                                                            */
/*      TRUE        = The two PSAP-addresses match.                          */
/*      FALSE       = The two PSAP-addresses don't match.                    */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: R.Horn                                        DATE: 05.04.90     */
/*exoff **********************************************************************/

#include <gds.h>
#include <string.h>
#include <dce/cmx_addr.h>
#include <dce/osiaddr.h>


boolean d2p_psap_addr_match (byte *psap1, byte *psap2) {

    struct t_adrdesc	*p1 = (struct t_adrdesc *) psap1 ;
    struct t_adrdesc	*p2 = (struct t_adrdesc *) psap2 ;
    char		p_type = p1->t_infotype ;

    /* verify whether the presented PSAP-address is a PSAP-address */
    if (p_type != T_PSAPINFO && p_type != T_SSAPINFO && p_type != T_TSAPINFO &&
							   p_type != T_NSAPINFO)
	return (FALSE) ;

    for (;;) {
	if (p1->t_infotype != p2->t_infotype)
	    return (FALSE) ;

	if (p1->t_infolng != p2->t_infolng) {
	    if (p1->t_infotype != T_NSAPINFO)
		return (FALSE) ;
	} else {
	    if (memcmp ((void *) (p1 + 1), (void *) (p2 + 1),
						 (size_t) p1->t_infolng) == 0) {
		if (p1->t_translng == p1->t_nextoff)
		    return (TRUE) ;
		p1 = (struct t_adrdesc *) ((char *) p1 + p1->t_nextoff) ;
	    }
	}
	if (p2->t_translng == p2->t_nextoff)
	    return (FALSE) ;
	p2 = (struct t_adrdesc *) ((char *) p2 + p2->t_nextoff) ;
    }
}
