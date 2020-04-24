/*
 *  @OSF_COPYRIGHT@
 *  COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 *  ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 *  src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d25alist.c,v $
 * Revision 1.1.10.2  1996/02/18  19:43:00  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:23:51  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:24:15  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:53:39  root]
 * 
 * Revision 1.1.8.3  1994/06/21  14:44:23  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:27:14  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:08:25  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:16:06  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:51:07  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:42:47  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  10:55:07  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  12:35:40  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  18:13:00  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:10:29  bbelch]
 * 
 * Revision 1.1.4.3  1992/11/27  15:58:22  marrek
 * 	November 1992 code drop
 * 	[1992/11/17  12:51:24  marrek]
 * 
 * Revision 1.1.4.2  1992/10/06  18:36:03  sommerfeld
 * 	Comment out #ident
 * 	[1992/09/30  21:03:45  sommerfeld]
 * 
 * Revision 1.1.2.3  1992/07/17  19:58:32  weisman
 * 	Checked in at OSF for SNI
 * 	[1992/07/17  19:26:16  weisman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d25alist.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:43:00 $";
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
/* NAME         : d25alist.c                                          */
/*								      */
/* AUTHOR       :  Praveen Gupta                                      */
/* DATE         :  26.02.90                                           */
/*								      */
/* DESCRIPTION  : This source files contains subroutiones to          */
/*                transform APDUs to IDS interface.                   */
/*                ( for list )                                        */
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

/******** LOCAL *********/

static signed32 d2a149_count_subord_info( byte * );

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a102_apdu_list_res_ids()                              */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*           This function transforms list_result from APDU format to */
/*  the list result and common result of the ids interface.           */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      apdu        = APDU for the list result .                      */
/*      apdu_len    = APDU length for the list result .               */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      list_res  = list result in IDS_interface.                     */
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

signed32 d2a102_apdu_list_res_ids(
  byte          * apdu,
  signed32              apdu_len,
  ids_list_res   * list_res,
  ids_common_res * com_res )
{
D23_ds_name *ds_name = (D23_ds_name *) apdu; /* pointer to distinguish name */
					       /* in the message block        */
byte err_apdu[D2A_ERR_APDU_SIZE];
signed32 er_len,i;
D23_subord_info *sord_info;
byte * scan = apdu;
ids_list_info * l_info;
ids_dist_name dn;
ids_rdname   * rdn;
const char function_name[] = "d2a102_apdu_list_res_ids";

  /* Invoke-APDU version checking is ignored ??? */

    list_res->uncorrelated = FALSE;
    l_info = &(list_res->info.list_info);
    if (ds_name->d23_dstag == D23_BONTAG)
      {
	 /*  value of DN cannot be BLANK              */
	  l_info->dname_defined = TRUE;
	  if (d2a121_dn_ids(&scan, &l_info->dname, err_apdu, &er_len)
				 == D2_ERROR)
	     { 
	       D2_ASN1_ERR_RETURN_WITH_TRACE
	      };
       }
     else
       l_info->dname_defined = FALSE;

    l_info->nb_subordinate = d2a149_count_subord_info(scan);
    if (l_info->nb_subordinate > 0)
      {
      if ((l_info->subordinates = D27_MALLOC(svc_c_sev_warning,
	       l_info->nb_subordinate * sizeof(ids_subordinate))) == NULL)
	{
	  d27_010_set_error_apdu((D23_rserror *)err_apdu,D2_TEMP_ERR,
		D2_SYS_ERR,D21_ALLOC_ERROR, &apdu_len);
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	}
       }
    for (i=0 ; i < l_info->nb_subordinate; i++)
	{
	   sord_info = (D23_subord_info *) scan;
	   scan += sizeof(D23_subord_info);
	   if (d2a121_dn_ids(&scan, &dn, err_apdu, &er_len)
				 == D2_ERROR)
	     { 
	       D2_ASN1_ERR_RETURN_WITH_TRACE
	      };
	   rdn = &((l_info->subordinates + i )->rdname);
	   rdn->nb_assertions = dn.rdnames->nb_assertions; /* No. of rdnmames */
	   rdn->assertions = dn.rdnames->assertions;       /* must be 1       */
	   free(dn.rdnames);
	   (l_info->subordinates + i)->alias_entry = sord_info->d23_soali;
	   (l_info->subordinates + i)->from_entry  = sord_info->d23_somas;
	 }
     l_info->part_oq_defined = FALSE;
     if(((D23_part_oq *)scan)->d23_oqtag == D23_OQTAG )
       {
	l_info->part_oq_defined = TRUE;
	if(d2a108_mk_ids_part_oq(&scan,&(l_info->part_oq),
				     err_apdu,&er_len) == D2_ERROR)
	   {
	   D2_ASN1_ERR_RETURN_WITH_TRACE
	   }
	}
d2a081_mk_ids_comres(&scan, com_res);

return(D2_NOERROR);
}

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a101_ids_list_arg_apdu()                               */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*           This function transforms list_arg's from ids format to   */
/*  the list arguments and common arguments of the APDU interface.    */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      apdu        = APDU for the list arguments.                    */
/*      apdu_len    = APDU length for the list arguments.             */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      list_arg  = list arguments in IDS_interface.                */
/*      com_arg   = Common arguments of IDS_interface.              */
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


signed32 d2a101_ids_list_arg_apdu(
  byte ** apdu,
  byte ** beg_apdu,
  signed32 * apdu_len,
  ids_list_arg * list_arg,
  ids_common_arg * com_arg,
  byte          * err_apdu )
{
signed32 max_length = *apdu_len;
byte * scan = *apdu;
const char function_name[] = "d2a101_ids_list_arg_apdu";

if (d2a123_mk_apdu_cmarg(&scan,com_arg, beg_apdu, apdu_len,
	    err_apdu, apdu_len) == D2_ERROR )
	{
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	}

if (d2a122_dn_apdu( D23_NAMTAG, &scan, &list_arg->object,
			beg_apdu, &max_length, err_apdu, apdu_len) == D2_ERROR)
	 { 
	   D2_ASN1_ERR_RETURN_WITH_TRACE
	  };

((D23_eom *) scan)->d23_eomtag = D23_EOMTAG;
((D23_eom *) scan)->d23_eomfil = D2_ALIGN(D23_EOMTAG);
*apdu = scan + sizeof(D23_eom);

return(D2_NOERROR);
}
/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a108_mk_ids_part_oq()				      */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_ERROR        transformation fails                          */
/*                      (err_apdu and err_len will be returned)       */
/*      D2_NOERROR      transformation is successful                  */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: Praveen Gupta                                             */
/**********************************************************************/

signed32 d2a108_mk_ids_part_oq(
  byte ** apdu,
  ids_part_oq * part_oq,
  byte * err_apdu,
  signed32 * er_len )
{
signed32 i,j;
byte * scan = *apdu;
D23_part_oq  * apdu_poq = (D23_part_oq *)scan;
const char function_name[] = "d2a108_mk_ids_part_oq";

    part_oq->un_crit_ext = FALSE;
    part_oq->limit_pb_defined = FALSE;
    if (apdu_poq->d23_oq_limit != D2_NO_LIMIT_EX)
     {  part_oq->limit_pb = apdu_poq->d23_oq_limit;
	part_oq->limit_pb_defined = TRUE;
      }
    scan += sizeof(D23_part_oq);
    for (i=0;((D23_rf_info *)scan)->d23_rftag == D23_REFTAG; i++)
      {
	scan += sizeof(D23_rf_info);

	scan += ((D23_ds_name *)scan)->d23_dsoff + sizeof(D23_ds_name);

	for (j=0;((D23_acc_info *)scan)->d23_acctag == D23_ACCTAG; j++)
	 {
	   scan += D23_S_ACC;
	   scan += ((D23_ds_name *)scan)->d23_dsoff + sizeof(D23_ds_name);
	   scan += ((D23_av_value *)scan)->d23_avoff + D23_S_ATV;
	  }
	}
    scan = *apdu + sizeof(D23_part_oq);
    part_oq->nb_unexplored = i;
    if (part_oq->nb_unexplored == 0)
    {
      part_oq->unexplored_defined = FALSE;
      return (D2_NOERROR);
     }
    else
      part_oq->unexplored_defined = TRUE;
    if ((part_oq->unexplored = 
	 D27_MALLOC(svc_c_sev_warning,i * sizeof(ids_cont_reference))) == NULL)
	{
	  d27_010_set_error_apdu((D23_rserror *)err_apdu,D2_TEMP_ERR,
		D2_SYS_ERR,D21_ALLOC_ERROR, er_len);
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	  }
    for (i=0; i < part_oq->nb_unexplored; i++)
    {
     if (d2a039_put_referral(part_oq->unexplored+i, &scan, err_apdu, er_len)
	   == D2_ERROR)
       { 
	 D2_ASN1_ERR_RETURN_WITH_TRACE
	}
    }

*apdu = scan;

return (D2_NOERROR);
}

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d2a149_count_subord_info			*/
/*									*/
/* Description	: This function counts subordinate infos.		*/
/*									*/
/* Return Value	: count							*/
/*									*/
/* Date         : 1991							*/
/* Author       : Praveen Gupta						*/
/*									*/
/*----------------------------------------------------------------------*/

static signed32 d2a149_count_subord_info(
  byte *scan)
{
signed32 i;
    
for (i=0; ((D23_subord_info *)scan)->d23_sotag == D23_SOITAG; i++)
	{
	scan += D23_S_SUBORD;
	scan += D23_S_NM + ((D23_ds_name *)scan)->d23_dsoff;
	}
return (i);
}
