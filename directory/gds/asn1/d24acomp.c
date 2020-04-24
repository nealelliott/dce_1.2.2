/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d24acomp.c,v $
 * Revision 1.1.10.2  1996/02/18  19:42:40  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:23:30  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:23:21  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:53:25  root]
 * 
 * Revision 1.1.8.3  1994/06/21  14:44:09  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:26:12  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:07:15  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:14:33  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:50:37  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:40:05  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  10:19:20  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  12:32:07  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:12:20  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:09:28  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  15:54:18  marrek
 * 	November 1992 code drop
 * 	[1992/11/17  12:47:56  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:04:34  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:01:17  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d24acomp.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:42:40 $";
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
/* NAME         : d24acomp.c                                           */
/*								      */
/* AUTHOR       : Schmid, DI AP 11                                    */
/* DATE         : 26. 2. 1990                                         */
/*								      */
/* DESCRIPTION  : This source files contains subroutiones to          */
/*                transform APDUs to IDS interface.                   */
/*								      */
/* SYSTEM DEPENDENCIES:  COMMON                                       */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   0.1    | 90-02-09| Original                       | WS |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <stdlib.h>

#include <d2asn1.h>
#include <d2asn1_int.h>

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a082_apdu_compare_arg_ids()                           */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function transforms compare_arg's from APDU format to    */
/*      the compare arguments and common arguments of the ids         */
/*      interface.                                                    */
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

signed32 d2a082_apdu_compare_arg_ids(
  byte           *apdu,
  signed32             *apdu_len,
  ids_compare_arg *comp_arg,
  ids_common_arg  *com_arg,
  byte           *err_apdu )
{
byte           *p_mess = apdu;
signed32              syntax;
dua_att_value   att_val;
#ifndef DSA
signed16 version = ((D23_header *)apdu)->d23_versno;
#endif
const char function_name[] = "d2a082_apdu_compare_arg_ids";

#ifndef DSA

    p_mess += sizeof(D23_incompare);

    if                              /*  IPC version is not ok         */
      (!D2_CHECK_MINMAX(D23_APDUV22,D23_APDUV22,version))
    {                               /*  reset to 0 and return error   */
      d27_010_set_error_apdu((D23_rserror *)err_apdu,D2_CALL_ERR,D2_ILL_VERS,
			     D2_NOT_SPEC,apdu_len);
      D2_ASN1_ERR_RETURN_WITH_TRACE
    }                               /*                                */
#endif
d2a080_mk_ids_comarg(&p_mess, com_arg);

/*
   generate DUA-DN from APDU
*/
if (d2a121_dn_ids(&p_mess, &comp_arg->object, err_apdu,apdu_len)
					   == D2_ERROR) 
  {
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }

/*
   generate "ids_assertion" from attribute in APDU:
*/
if (d2a120_copy_att_type(&p_mess, &(comp_arg->purported.type), err_apdu,
	     apdu_len ) == D2_ERROR)
  {
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }

/*
   get attribute syntax
*/
syntax = ((D23_av_info *)p_mess)->d23_asyntx;
p_mess += D23_S_AV;

/*
   copy attribute value from APDU
*/
if (d2a031_cp_attr(((D23_av_value *)p_mess)->d23_avrep,
		   ((D23_av_value *)p_mess)->d23_avlen,
       (byte *) (p_mess + D23_S_ATV),&att_val, err_apdu, apdu_len)
		   != D2_NOERROR)
  {   
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }

dua_att_value_ids( &att_val, &comp_arg->purported.value);
if (syntax != D2_DISTNAME)
  dua_att_value_free( &att_val);

return(D2_NOERROR);
}

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a084_ids_compare_res_apdu()                           */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function transforms compare result's from ids format to  */
/*      the compare result and common result of the APDU interface.   */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      apdu        = APDU for the compare result .                   */
/*      apdu_len    = APDU length for the compare result .            */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      comp_res  = compare result in IDS_interface.                */
/*      com_res   = Common result of IDS_interface.                 */
/*      err_apdu    = error APDU (memory error or syntax error)       */
/*      apdu_len    = length of error APDU                            */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_ERROR        transformation fails                          */
/*                      (err_apdu and err_len will be returned)       */
/*      D2_NOERROR      transformation is successful                  */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: W. Schmid                                                 */
/**********************************************************************/


signed32 d2a084_ids_compare_res_apdu(
  byte          **cur_apdu,
  byte          **apdu,
  signed32             *apdu_len,
  ids_compare_res *comp_res,
  ids_common_res  *com_res,
  byte           *err_apdu )
{
D23_rscompinfo   *incomp;
byte           *act_apdu;
signed32        max_len;
const char function_name[] = "d2a084_ids_compare_res_apdu";

#ifndef DSA
if ((*cur_apdu = *apdu  = D27_MALLOC(svc_c_sev_warning,D2A_APDU_MEM))==NULL)
{   d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_TEMP_ERR,
			   D2_SYS_ERR, D21_ALLOC_ERROR, apdu_len);
    D2_ASN1_ERR_RETURN_WITH_TRACE
}
#endif
max_len = (*apdu_len > D2A_APDU_MEM)? *apdu_len : D2A_APDU_MEM;
act_apdu = *cur_apdu;
incomp = (D23_rscompinfo *) act_apdu;
incomp->d23_Ltag = D23_LTAG;
incomp->d23_Lmatsgn = comp_res->matched;
incomp->d23_Lfrmen = comp_res->from_entry;
act_apdu += sizeof(D23_rscompinfo);

/*
   generate ENTTAG
*/

if (comp_res->dname_defined == TRUE)
{
   if (d2a122_dn_apdu( D23_NAMTAG, &act_apdu, &comp_res->dname,
		apdu, &max_len, err_apdu,apdu_len) == D2_ERROR)
    {   free(*apdu);
	D2_ASN1_ERR_RETURN_WITH_TRACE
    }
}

d2a130_mk_apdu_comres (com_res, &act_apdu);
((D23_eom *) act_apdu)->d23_eomtag = D23_EOMTAG;
*cur_apdu = act_apdu + sizeof(D23_eom);
*apdu_len = *cur_apdu - *apdu;

return(D2_NOERROR);
}
