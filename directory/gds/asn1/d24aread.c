/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d24aread.c,v $
 * Revision 1.1.10.2  1996/02/18  19:42:46  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:23:37  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:23:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:53:29  root]
 * 
 * Revision 1.1.8.3  1994/06/21  14:44:13  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:26:33  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:07:19  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:14:49  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:50:49  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:40:59  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  10:43:43  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  12:33:12  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:12:34  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:09:49  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  15:55:37  marrek
 * 	November 1992 code drop
 * 	[1992/11/17  12:49:00  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:05:11  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:01:49  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d24aread.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:42:46 $";
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
/* NAME         : d24aread.c                                        */
/*								      */
/* AUTHOR       : Praveen Gupta, Siemens India                        */
/* DATE         : 9. 1. 1990                                          */
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
#include <stdlib.h>

#include <d2asn1.h>
#include <d2asn1_int.h>

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a006_apdu_read_arg_ids()                              */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*           This function transforms read_arg's from APDU format to  */
/*  the read arguments and common arguments of the ids interface.     */
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

signed32 d2a006_apdu_read_arg_ids(
  byte * apdu,
  signed32 * apdu_len,
  ids_read_arg * read_arg,
  ids_common_arg * com_arg,
  byte * err_apdu )
{
byte * scan = apdu;    /* Ignore Siemens specific hdr */
#ifndef DSA
signed32 version = ((D23_header *)apdu)->d23_versno;
#endif
const char function_name[] = "d2a006_apdu_read_arg_ids";

#ifndef DSA

    if                              /*  IPC version is not ok         */
      (!D2_CHECK_MINMAX(D23_APDUV22,D23_APDUV22,version))
    {                               /*  reset to 0 and return error   */
      d27_010_set_error_apdu((D23_rserror *)apdu,D2_CALL_ERR,D2_ILL_VERS,
			     D2_NOT_SPEC,apdu_len);
      D2_ASN1_ERR_RETURN_WITH_TRACE
    }                               /*                                */
    scan += D23_S_INR; /* Ignore Siemens specific hdr */
#endif

  d2a080_mk_ids_comarg(&scan,com_arg);
  if (d2a121_dn_ids(&scan, &read_arg->object, err_apdu,apdu_len)
		      == D2_ERROR)
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}

  /*   for rq info  */
  if (d2a019_req_to_einfo_sel(&scan,
	    &read_arg->selection, err_apdu, apdu_len) == D2_ERROR)
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}

return(D2_NOERROR);
}

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a008_ids_read_res_apdu()                              */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*           This function transforms read_arg's from ids format to   */
/*  the read arguments and common arguments of the APDU interface.    */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      apdu        = APDU for the read result .                      */
/*      apdu_len    = APDU length for the read result .               */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      read_res  = read result in IDS_interface.                     */
/*      com_res   = Common result of IDS_interface.                   */
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

signed32 d2a008_ids_read_res_apdu(
  byte ** cur_apdu,
  byte ** apdu,
  signed32 * apdu_len,
  ids_read_res * read_res,
  ids_common_res * com_res,
  byte * err_apdu )
{
signed32 max_length;
byte * scan; 
D23_en_info * en_info;
const char function_name[] = "d2a008_ids_read_res_apdu";

#ifndef DSA
    if ((*cur_apdu = *apdu = D27_MALLOC(svc_c_sev_warning,D2A_APDU_MEM))==NULL)
	{
	  d27_010_set_error_apdu((D23_rserror *)err_apdu,D2_TEMP_ERR,
		D2_SYS_ERR,D21_ALLOC_ERROR, apdu_len);
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	}
#endif
    scan = *cur_apdu;
    max_length = (*apdu_len > D2A_APDU_MEM) ? *apdu_len : D2A_APDU_MEM;

    en_info = (D23_en_info *) scan;
    en_info->d23_enmas = read_res->entry_info.from_entry;
    en_info->d23_enmaxat = read_res->entry_info.nb_info;
    en_info->d23_entag = D23_ENTTAG;

    scan += sizeof(D23_en_info);
    if (d2a122_dn_apdu(D23_NAMTAG, &scan,
	    &read_res->entry_info.dist_name, apdu, &max_length,
			 err_apdu, apdu_len) == D2_ERROR)
	{
	  free(*apdu);
	  *apdu = NULL;
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	}

     /* Common results are avoided as the mapping of comm_res is not known */

    if (read_res->entry_info.infos_defined)
      {
	/*   for entry info  */
	if (d2a029_entry_info_to_at_info( read_res->entry_info.nb_info, 
	       read_res->entry_info.infos, &scan, apdu, &max_length,
			err_apdu,apdu_len)
							== D2_ERROR)
	{
	  free(*apdu);
	  *apdu = NULL;
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	}
     }
     d2a130_mk_apdu_comres(com_res, &scan);
     ((D23_eom *) scan)->d23_eomtag = D23_EOMTAG;
     ((D23_eom *) scan)->d23_eomfil = D2_ALIGN(D23_EOMTAG);
     *cur_apdu = scan + sizeof(D23_eom);
     *apdu_len = *cur_apdu - *apdu;

return(D2_NOERROR);
}

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a019_req_to_einfo_sel()                               */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      Transforms the requested attributes from APDU format to       */
/*      THORN-IDS format ("ids_entry_info_sel")                       */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      apdu        = pointer to the APDU containing requested        */
/*                    attributes                                      */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      apdu        = pointer to next tag in the APDU                 */
/*      selection   = generated list of attribute info in IDS format  */
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

signed32 d2a019_req_to_einfo_sel(
  byte               **apdu,
  ids_entry_info_sel  *selection,
  Octet_string         err_apdu,
  signed32                 *err_len )
{
D23_rq_info     *rq_info = (D23_rq_info *) *apdu;
signed32              i;
const char function_name[] = "d2a019_req_to_einfo_sel";

if (rq_info->d23_rqtag != D23_REQTAG)
{   
    D2_ASN1_ERR_RETURN_WITH_TRACE
}
*apdu += sizeof(D23_rq_info);

selection->info_type_mode = (rq_info->d23_rqreq == D2_R_TYP) ? 
			    ATT_TYPE_ONLY : ATT_TYPE_AND_VALUE;
if (rq_info->d23_rqnbr == D2_ALL_ATT_REQ)
{   selection->is_all_att = TRUE;
    selection->nb_types = 0;
}
else
{   selection->is_all_att = FALSE;
    selection->nb_types = rq_info->d23_rqnbr;
    if (selection->nb_types != 0)
    {   if ((selection->types =
     D27_MALLOC(svc_c_sev_warning,selection->nb_types * sizeof(ids_att_type)))
		     == NULL )
	{   
	    d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_TEMP_ERR,
			   D2_SYS_ERR, D21_ALLOC_ERROR,err_len);
	    D2_ASN1_ERR_RETURN_WITH_TRACE
	}

	for (i = 0; i < selection->nb_types; i++)
	{
	   if (d2a120_copy_att_type(apdu, selection->types + i, err_apdu,
		       err_len) == D2_ERROR)
		{
		D2_ASN1_ERR_RETURN_WITH_TRACE
		}
	}
    }
    else
    { selection->types = (ids_att_type *) 0;}
}

return(D2_NOERROR);
}
