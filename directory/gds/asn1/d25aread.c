/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d25aread.c,v $
 * Revision 1.1.10.2  1996/02/18  19:43:04  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:23:55  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:24:27  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:53:42  root]
 * 
 * Revision 1.1.8.3  1994/06/21  14:44:26  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:27:22  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:08:29  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:16:15  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:51:13  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:43:14  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  10:57:59  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  12:36:16  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:13:08  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:10:41  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  15:59:06  marrek
 * 	November 1992 code drop
 * 	[1992/11/17  12:51:59  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:08:15  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:02:59  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d25aread.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:43:04 $";
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
/* NAME         : d25aread.c                                        */
/*								      */
/* AUTHOR       : Praveen Gupta, Siemens India                        */
/* DATE         : 9. 1. 1990                                          */
/*								      */
/* DESCRIPTION  : This source files contains subroutiones to          */
/*                transform APDUs to (THORN)-IDS interface.           */
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
/*  FUNCTION: d2a007_ids_read_arg_apdu()                              */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*           This function transforms read_arg's from ids format to   */
/*  the read arguments and common arguments of the APDU interface.    */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      apdu        = APDU for the read arguments.                    */
/*      apdu_len    = APDU length for the read arguments.             */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      read_arg  = read arguments in IDS_interface.                  */
/*      com_arg   = Common arguments of IDS_interface.                */
/*      err_apdu    = error APDU (memory error or syntax error)       */
/*      apdu_len     = length of error APDU                           */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_ERROR        transformation fails                          */
/*                      (err_apdu and err_len will be returned)       */
/*      D2_NOERROR      transformation is successful                  */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: Praveen Gupta                                             */
/**********************************************************************/


signed32 d2a007_ids_read_arg_apdu(
  byte ** apdu,
  byte ** beg_apdu,
  signed32 * apdu_len,
  ids_read_arg * read_arg,
  ids_common_arg * com_arg,
  byte          * err_apdu )
{
signed32 max_length = *apdu_len;
byte * scan = *apdu;
const char function_name[] = "d2a007_ids_read_arg_apdu";

     if (d2a123_mk_apdu_cmarg(&scan, com_arg, beg_apdu, &max_length,
			      err_apdu,apdu_len) == D2_ERROR)
	{
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	}

    if (d2a122_dn_apdu(D23_NAMTAG, &scan, &read_arg->object,
	       beg_apdu, &max_length, err_apdu,apdu_len) == D2_ERROR)
	{
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	}

/*   for rq info  */
	if (d2a028_einfo_sel_to_req_attr(&read_arg->selection,
		&scan, beg_apdu, &max_length, err_apdu, apdu_len) == D2_ERROR)
	{
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	}
     
     ((D23_eom *) scan)->d23_eomtag = D23_EOMTAG;
     ((D23_eom *) scan)->d23_eomfil = D2_ALIGN(D23_EOMTAG);
     *apdu = (scan + sizeof(D23_eom));

return(D2_NOERROR);
}

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a009_apdu_read_res_ids()                              */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*           This function transforms read_result from APDU format to */
/*  the read result and common result of the ids interface.           */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      apdu        = APDU for the read result .                      */
/*      apdu_len    = APDU length for the read result .               */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      read_res  = read result in IDS_interface.                     */
/*      com_res   = Common result of IDS_interface.                   */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_ERROR        transformation fails                          */
/*                      (err_apdu and err_len will be returned)       */
/*      D2_NOERROR      transformation is successful                  */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: Praveen Gupta                                             */
/**********************************************************************/

signed32 d2a009_apdu_read_res_ids(
  byte * apdu,
  signed32 apdu_len,                            
  ids_read_res * read_res,
  ids_common_res * com_res )
{
byte         err_apdu[D2A_ERR_APDU_SIZE];
signed32           er_len;
D23_en_info * en_info = (D23_en_info *) apdu;
byte       * scan = apdu;
const char function_name[] = "d2a009_apdu_read_res_ids";

    /* Assumes APDU to be of Version-2.2 */

    if (en_info->d23_enmas == TRUE)
       read_res->entry_info.from_entry = TRUE;
    else
       read_res->entry_info.from_entry = FALSE;
    scan += sizeof(D23_en_info);
    if (d2a121_dn_ids(&scan, &(read_res->entry_info.dist_name),
		      err_apdu,&er_len) == D2_ERROR)
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}

    read_res->entry_info.nb_info = en_info->d23_enmaxat;

    /*   for rq info  */
    if (d2a030_at_info_to_entry_info(&scan,
			(signed32 *)&(read_res->entry_info.nb_info),
			 &(read_res->entry_info.infos), err_apdu, &er_len)
					 == D2_ERROR) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
    if (read_res->entry_info.nb_info)
	 read_res->entry_info.infos_defined = TRUE;
    else
	 read_res->entry_info.infos_defined = FALSE;

    d2a081_mk_ids_comres(&scan, com_res);

return(D2_NOERROR);
}

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a028_einfo_sel_to_req_attr()                          */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      Transforms the requested attributes from THORN-IDS format to  */
/*      APDU format                                                   */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      selection   = requested attributes in IDS format              */
/*      act_apdu    = pointer to the APDU where to write the          */
/*                    requested attribute info                        */
/*      beg_apdu    = start address of APDU                           */
/*      max_apdu_len= max. length of allocated APDU memory            */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      act_apdu    = pointer to next tag in the APDU where to write  */
/*                    the next block                                  */
/*      beg_apdu    = (new) start address of APDU 		      */
/*      max_apdu_len= (new) max. length of allocated APDU memory      */
/*      err_apdu    = error APDU (memory error)                       */
/*      err_len     = length of error APDU                            */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_ERROR        function fails                                */
/*      D2_NOERROR      function successful                           */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: W. Schmid                                                 */
/**********************************************************************/

signed32 d2a028_einfo_sel_to_req_attr(
  ids_entry_info_sel  *selection,
  byte              **act_apdu,
  byte              **beg_apdu,
  signed32                 *max_apdu_len,
  byte               *err_apdu,
  signed32                  *err_len  )       /* PKG */
{
D23_rq_info     *rq_info;
ids_att_type    *ids_types;
byte           * scan;
signed32              i;
signed32              no_types;
const char function_name[] = "d2a028_einfo_sel_to_req_attr";

no_types = (selection->is_all_att == TRUE) ? 0 : selection->nb_types;

/*
    check if enough memory
*/
if (no_types != 0)
  if (d2a033_check_memsize (no_types*D2_OBJ_IDL_MAX, beg_apdu, act_apdu,
		   max_apdu_len,(D23_rserror *)err_apdu, err_len) == D2_ERROR)
      {   
	  D2_ASN1_ERR_RETURN_WITH_TRACE
      }
scan   = *act_apdu;
rq_info = (D23_rq_info *) scan;

/*
   set pointer to next element to be generated in the APDU
*/
scan += sizeof(D23_rq_info);

rq_info->d23_rqtag = D23_REQTAG;
rq_info->d23_rqreq = (selection->info_type_mode == ATT_TYPE_ONLY) ? D2_R_TYP
						      : D2_R_VAL_TYP;
if (selection->is_all_att == TRUE)
{   rq_info->d23_rqnbr = D2_ALL_ATT_REQ;
    *act_apdu = scan;
    return(D2_NOERROR);
}
else
{
    for (rq_info->d23_rqnbr = no_types, i = 0, ids_types = selection->types;
	 i < no_types; ids_types++, i++)
	  {
	    if (d2a124_copy_attyp_apdu(&scan,
			ids_types, beg_apdu, max_apdu_len,
			err_apdu, err_len) == D2_ERROR)
	     	{ 
		D2_ASN1_ERR_RETURN_WITH_TRACE
		}
	    }
}

*act_apdu = scan;

return(D2_NOERROR);
}
