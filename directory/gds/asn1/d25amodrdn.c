/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d25amodrdn.c,v $
 * Revision 1.1.10.2  1996/02/18  19:43:02  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:23:54  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:24:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:53:41  root]
 * 
 * Revision 1.1.8.3  1994/06/21  14:44:25  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:27:19  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:08:28  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:16:12  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:51:10  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:43:05  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  10:57:04  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  12:36:05  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:13:05  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:10:36  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  15:58:53  marrek
 * 	November 1992 code drop
 * 	[1992/11/17  12:51:48  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:06:50  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:02:54  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d25amodrdn.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:43:02 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton	***************************************************************/
/*								      */
/* TYP		: C-FILE					      */
/*								      */
/* NAME         : d25amodrdn.c                                        */
/*								      */
/* AUTHOR       :  Praveen Gupta                                      */
/* DATE         :  28.08.90                                           */
/*								      */
/* DESCRIPTION  : This source files contains subroutiones to          */
/*                transform IDS to APDU interface.                   */
/*                ( for modifyRDN entry )                             */
/*								      */
/* SYSTEM DEPENDENCIES:  COMMON                                       */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   0.1    | 90-03-01| Original                       | as |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

#include <gds.h>

#include <d2asn1.h>
#include <d2asn1_int.h>

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a126_ids_modrdn_arg_apdu()                            */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*        This function transforms modifyRDN_arg's from ids format to */
/*        the modifyRDN arguments and common arguments of the APDU    */
/*        interface.                                                  */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      modrdn_arg  = modRDN arguments in IDS_interface.              */
/*      com_arg   = Common arguments of IDS_interface.                */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      apdu        = APDU for the modify arguments.                  */
/*      apdu_len    = APDU length for the modify arguments.           */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_ERROR        transformation fails                          */
/*                      (err_apdu and apdu_len will be returned)       */
/*      D2_NOERROR      transformation is successful                  */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: Praveen Gupta                                             */
/**********************************************************************/

signed32 d2a126_ids_modrdn_arg_apdu(
  byte               **apdu,
  byte               **beg_apdu,
  signed32                  *apdu_len,
  ids_modify_rdn_arg   *modrdn_arg,
  ids_common_arg       *com_arg,
  byte               *err_apdu )
{
byte            *  act_apdu= *apdu;
ids_dist_name       dn;
const char function_name[] = "d2a126_ids_modrdn_arg_apdu";

if (d2a123_mk_apdu_cmarg(&act_apdu,com_arg, beg_apdu, apdu_len,
	    err_apdu, apdu_len) == D2_ERROR )
	{
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	}

((D23_repinfo *)act_apdu)->d23_reptag = D23_REPTAG;

if (modrdn_arg->delet == TRUE)
    ((D23_repinfo *)act_apdu)->d23_delordn = TRUE;
else
    ((D23_repinfo *)act_apdu)->d23_delordn = FALSE;

act_apdu += D23_S_REP;


/*
   generate DN from DUA dist. name
*/
if (d2a122_dn_apdu(D23_NAMTAG,&act_apdu, &modrdn_arg->object,
	   beg_apdu, apdu_len, err_apdu, apdu_len) == D2_ERROR)
    {
    D2_ASN1_ERR_RETURN_WITH_TRACE
    }

dn.nb_rdnames = 1;
dn.rdnames = &modrdn_arg->new_rdn;

/*
   generate NEW-RDN
*/
if (d2a122_dn_apdu(D23_NAMTAG, &act_apdu, &dn,
	   beg_apdu, apdu_len, err_apdu, apdu_len) == D2_ERROR)
    {
    D2_ASN1_ERR_RETURN_WITH_TRACE
    }

((D23_eom *) act_apdu)->d23_eomtag = D23_EOMTAG;
*apdu = (act_apdu + sizeof(D23_eom));

return(D2_NOERROR);
}
