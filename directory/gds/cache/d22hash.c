/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d22hash.c,v $
 * Revision 1.1.10.2  1996/02/18  19:43:52  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:24:52  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:27:42  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:54:30  root]
 * 
 * Revision 1.1.8.3  1994/09/06  12:24:59  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:13:51  keutel]
 * 
 * Revision 1.1.8.2  1994/05/10  15:51:18  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:17:18  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  16:03:27  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:53:59  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  07:41:02  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  12:14:39  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  18:41:17  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:13:49  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:30:18  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:37:36  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d22hash.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:43:52 $";
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
/* NAME         : d22hash.c                                           */
/*								      */
/* AUTHOR       : R. Horn, D AP 11                                    */
/* DATE         : 10.05.88                                            */
/*								      */
/* COMPONENT    : DS                                                  */
/*								      */
/* DOC.-NR.     : Directory-Design-Specification                      */
/*			.					      */
/*			.					      */
/*								      */
/* PRD#/VERS.   :                                                     */
/*								      */
/* DESCRIPTION  :                                                     */
/*								      */
/* SYSTEM DEPENDENCIES: COMMON                                        */
/*								      */
/* HISTORY      :                                                     */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/*exon ***********************************************************************/
/*                                                                           */
/*  FUNCTION:  d22_hash_index ()                                             */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*                                                                           */
/*      The function generates a hash index from a given  distinguished      */
/*      name. Both, the types and the values of the name components are      */
/*      considered therefore. The hash index is formed  by summing the       */
/*      values  of all name part types (attribute identifier), the values    */
/*      of all characters contained in the name part values and the values   */
/*      of the delimiter. The function considers also that the matching      */
/*      rule of a distinguished name is 'case ignore string'. Thus, all      */
/*      appearing upper case characters are converted to lower case for      */
/*      index generation. The position of the characters is evaluated, be-   */
/*      cause the generated hash index of e.g. the name part value 'xyz'     */
/*      should be different from that generated for the name value 'xzy'.    */
/*      The name part types are included, because the generated hash index   */
/*      of e.g. the name part value 'xyz' of name part type 'X' should be    */
/*      different from that generated for the name part value 'xyz' of name  */
/*      part type 'Y'.                                                       */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*      dn          =  Pointer to the distinguished name from which a        */
/*                     hash index should be generated.                       */
/*      max_index   =  The maximal allowable hash index (depends on the      */
/*                     size of the hash table).                              */
/*                                                                           */
/*  OUTPUT-PARAMETER:                                                        */
/*                     None                                                  */
/*                                                                           */
/*  RETURN-VALUE:                                                            */
/*                     Generated hash index.                                 */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: R.Horn                                        DATE: 16.11.87     */
/*exoff **********************************************************************/

#include <gds.h>
#include <dce/d2dir.h>
#include <d23apdu.h>
#include <d23ipc.h>
#include <d22cache.h>


/* ------------------------------------------------------------------------ */  

signed32 d22_hash_index (D2_name_string dn, signed32 max_index) {

    signed32		i = 0, index = 0 ;
    Octet_string 	name = (Octet_string) dn;

    while (*name) {
	if (*name == D2_T61_REP || *name == D2_PRTBL_REP ||
	    *name == D2_IA5_REP || *name == D2_NUM_REP) {
	    name++ ;
	    continue ;
	}
	if (*name <= D2_RG4 && *name >= D2_RG3)
	    index += (i++ % 2) ? (signed32) (*name++ + D2_ULDIFF) << 2 :
					 (signed32) (*name++ + D2_ULDIFF) << 1 ;
	else
	    index += (i++ % 2) ? (signed32) *name++ << 2 :
						       (signed32) *name++ << 1 ;
    }
    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
			svc_c_debug6, GDS_S_CACHE_HASHIDX, index % max_index)) ;
    return (index % max_index) ;
}
