/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d22convdn.c,v $
 * Revision 1.1.10.2  1996/02/18  19:43:39  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:24:36  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:27:01  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:54:21  root]
 * 
 * Revision 1.1.8.3  1994/09/06  12:24:49  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:13:07  keutel]
 * 
 * Revision 1.1.8.2  1994/05/10  15:51:06  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:16:52  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  16:03:10  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:52:26  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  07:28:02  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  12:12:54  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  18:40:32  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:13:11  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:14:15  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:06:09  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d22convdn.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:43:39 $";
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
/* NAME         : d22convdn.c                                         */
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
/*  FUNCTION:  d22_convert_to_dn ()                                          */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*      The function takes the naming attribute informations (naming attri-  */
/*      bute type and naming attribute value) from a given naming infor-     */
/*      mation block and converts this information into a distinguished      */
/*      name string.                                                         */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*      dnptr       = Pointer to an area where the converted distinuished    */
/*                    should be stored.                                      */
/*      namingptr   = Pointer to the naming attribute information block.     */
/*      infosize    = Size of the attribute information block.               */
/*                                                                           */
/*  OUTPUT-PARAMETER:                                                        */
/*      dnptr       = Converted distinguished name in string-format.         */
/*                                                                           */
/*  RETURN-VALUE:                                                            */
/*                    None                                                   */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: R.Horn                                        DATE: 16.2.88      */
/*exoff **********************************************************************/

#include <gds.h>
#include <stdio.h>
#include <string.h>
#include <dce/d2dir.h>
#include <d23apdu.h>
#include <d23ipc.h>
#include <d22cache.h>

/* ------------------------------------------------------------------------- */

void d22_convert_to_dn (char *dnptr, D22_attr_entry *namingptr,
							    signed16 infosize) {

    D22_attr_entry	*maxnmptr = (D22_attr_entry *) ((char *) namingptr +
								     infosize) ;
    D2_obj_id		objid ;
    char		objidstr[D2_NP_ID_LEN], *p = dnptr ;
    signed16		del, rdnflag = D22_RDN_FLAG ;

    /* initialize distinguished name area */
    *p = '\0' ;
    /* convert naming attributes into a distinguished name string */
    while (namingptr < maxnmptr &&
		   (namingptr->d22_aetype & D22_NAME_AFLAG) == D22_NAME_AFLAG) {
    	objid.d2_typ_len = namingptr->d22_aidsize ;
	objid.d2_type = namingptr->d22_aident ;
	del = namingptr->d22_aidsyn ;
	del = (del == D2_T61) ? D2_T61_REP :
	      (del == D2_IA5) ? D2_IA5_REP :
	      (del == D2_NUMERIC) ? D2_NUM_REP : D2_PRTBL_REP ;
	d27_301_oid_str (&objid, objidstr) ;
	if (p == dnptr)
	    (void) sprintf (p, D2_RDNS_FORMAT, (int) strlen (objidstr), objidstr,
		del, namingptr->d22_asizes.d22_vlen, (char *) (namingptr + 1)) ;
	else
	    (void) sprintf (p, D2_RDN_FORMAT,
		((namingptr->d22_aetype & D22_RDN_FLAG) != rdnflag) ?
		D2_RDN_SEP : D2_AVA_SEP, (int) strlen (objidstr), objidstr,
		del, namingptr->d22_asizes.d22_vlen, (char *) (namingptr + 1)) ;
	p += strlen (p) ;
	rdnflag = namingptr->d22_aetype & D22_RDN_FLAG ;
	namingptr = (D22_attr_entry *) ((char *) namingptr +
		     sizeof (D22_attr_entry) + namingptr->d22_asizes.d22_voff) ;
    }
}
