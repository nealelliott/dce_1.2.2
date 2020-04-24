/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d25acomp.c,v $
 * Revision 1.1.10.2  1996/02/18  19:42:57  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:23:48  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:24:07  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:53:37  root]
 * 
 * Revision 1.1.8.3  1994/06/21  14:44:21  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:27:05  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:08:23  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:15:21  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:51:05  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:42:24  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  10:52:22  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  12:35:09  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:12:54  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:10:20  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  15:58:00  marrek
 * 	November 1992 code drop
 * 	[1992/11/17  12:50:54  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:06:15  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:02:30  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d25acomp.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:42:57 $";
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
/* NAME         : d25acomp.c                                           */
/*								      */
/* AUTHOR       : Schmid, DI AP 11                                    */
/* DATE         : 26. 2. 1990                                         */
/*								      */
/* DESCRIPTION  : This source files contains subroutiones to          */
/*                transform APDUs to IDS interface.           */
/*								      */
/* SYSTEM DEPENDENCIES:  COMMON                                       */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   0.1    | 90-02-09| Original                       | WS |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

#include <gds.h>

#include <d2asn1.h>
#include <d2asn1_int.h>

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a083_ids_compare_arg_apdu()                           */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function transforms compare_arg's from IDS interface to  */
/*       APDU format.                                                 */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      apdu        = APDU for the compare arguments.                 */
/*      apdu_len    = APDU length for the compare arguments.          */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      comp_arg  = compare arguments in IDS_interface.             */
/*      com_arg   = Common arguments of IDS_interface.              */
/*      err_apdu    = error APDU (memory error or syntax error)       */
/*      apdu_len    = length of error APDU                            */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_ERROR        transformation fails                          */
/*                      (err_apdu with length = *apdu_len is returned)*/
/*      D2_NOERROR      transformation is successful                  */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: W. Schmid                                                 */
/**********************************************************************/

signed32 d2a083_ids_compare_arg_apdu(
  byte           **apdu,
  byte           **beg_apdu,
  signed32              *apdu_len,
  ids_compare_arg  *comp_arg,
  ids_common_arg   *com_arg,
  byte            *err_apdu )
{
ids_att          ids_attr;
byte           *act_apdu = *apdu;
const char function_name[] = "d2a083_ids_compare_arg_apdu";

if (d2a123_mk_apdu_cmarg(&act_apdu,com_arg, beg_apdu, apdu_len,
	    err_apdu,apdu_len) == D2_ERROR )
	{
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	}

/*
   generate DN name (in APDU) from DUA dist. name
*/
if (d2a122_dn_apdu( D23_NAMTAG, &act_apdu, &comp_arg->object,
	    beg_apdu, apdu_len, err_apdu, apdu_len) == D2_ERROR)
  {
      D2_ASN1_ERR_RETURN_WITH_TRACE
   }

/*
   generate APDU from DUA attribute assertion
*/
ids_attr.type.l = comp_arg->purported.type.l;
ids_attr.type.v = comp_arg->purported.type.v;
ids_attr.nb_values = 1;
ids_attr.values = &comp_arg->purported.value;
if (d2a060_mk_att_apdu (&ids_attr, &act_apdu, beg_apdu,
			apdu_len, err_apdu, apdu_len) != D2_NOERROR)
{
    D2_ASN1_ERR_RETURN_WITH_TRACE
}

((D23_eom *) act_apdu)->d23_eomtag = D23_EOMTAG;
*apdu = act_apdu + D23_S_EOM;

return(D2_NOERROR);
}

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a085_apdu_compare_res_ids()                           */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function transforms compare result from APDU format to   */
/*      the compare result and common result of the ids interface.    */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      apdu        = APDU for the compare result .                   */
/*      apdu_len    = APDU length for the compare result .            */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      comp_res  = compare result in IDS_interface.                */
/*      com_res   = Common result of IDS_interface.                 */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_ERROR        transformation fails                          */
/*                      (err_apdu and err_len will be returned)       */
/*      D2_NOERROR      transformation is successful                  */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: W. Schmid                                                 */
/**********************************************************************/

signed32 d2a085_apdu_compare_res_ids(
  byte           *apdu,
  signed32              apdu_len,
  ids_compare_res *comp_res,
  ids_common_res  *com_res )
{
D23_ds_name     *ds_name;
D23_rscompinfo   *rscomp = (D23_rscompinfo *) apdu;
byte            err_apdu[D2A_ERR_APDU_SIZE];
signed32              err_len;
const char function_name[] = "d2a085_apdu_compare_res_ids";

comp_res->matched = (rscomp->d23_Lmatsgn == TRUE) ? TRUE : FALSE;

comp_res->from_entry = (rscomp->d23_Lfrmen == TRUE) ? TRUE : FALSE;
apdu = (byte *)rscomp  + sizeof(D23_rscompinfo);

/*
   check for DN in APDU
*/
ds_name = (D23_ds_name *) apdu;
if (ds_name->d23_dstag != D23_NAMTAG)
   comp_res->dname_defined = FALSE;
else
{   comp_res->dname_defined = TRUE;
    if (d2a121_dn_ids(&apdu, &(comp_res->dname),
		      err_apdu,&err_len) == D2_ERROR)
      {
	 D2_ASN1_ERR_RETURN_WITH_TRACE
       }
}

d2a081_mk_ids_comres(&apdu, com_res);

return(D2_NOERROR);
}
