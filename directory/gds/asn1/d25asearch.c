/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d25asearch.c,v $
 * Revision 1.1.10.2  1996/02/18  19:43:06  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:23:57  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:24:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:53:44  root]
 * 
 * Revision 1.1.8.3  1994/06/21  14:44:28  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:27:31  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:08:30  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:16:38  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  18:37:05  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:45:26  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  10:59:58  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  12:36:43  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:13:14  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:10:49  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  15:59:43  marrek
 * 	November 1992 code drop
 * 	[1992/11/26  13:03:53  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:08:36  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:03:13  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d25asearch.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:43:06 $";
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
/* NAME         : d25asearch.c                                        */
/*								      */
/* AUTHOR       : Ernst Kraemer, DI AP 11                             */
/* DATE         : 26. 2. 1990                                         */
/*								      */
/* DESCRIPTION  : This source files contains subroutiones to          */
/*                transform Search-APDUs to IDS interface.            */
/*								      */
/* SYSTEM DEPENDENCIES:  COMMON                                       */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   0.1    | 90-02-26| Original                       | WS |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

#include <gds.h>
#include <stdlib.h>
#include <stdio.h>

#include <d2asn1.h>
#include <d2asn1_int.h>

/******** LOCAL *********/

static signed32 d2a078_ids_filter_to_filter1(ids_filter *,byte **,byte **,signed32 *,
					byte *,signed32 *);
static signed32 d2a079_ids_fitem_to_fitem(ids_filter_item *,byte **,byte **,signed32 *,
					byte *,signed32 *);
static signed32 d2a107_count_entry_infos(byte *);

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a070_ids_search_arg_apdu()                            */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*           This function transforms search_arg's from ids format to */
/*  the search arguments and common arguments of the APDU interface.  */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      search_arg = search arguments in IDS_interface.               */
/*      com_arg   = Common arguments of IDS_interface.                */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      apdu        = APDU for the read arguments.                    */
/*      apdu_len    = APDU length for the read arguments.             */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_ERROR        transformation fails                          */
/*      D2_NOERROR      transformation is successful                  */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: Ernst Kraemer                                             */
/**********************************************************************/

signed32 d2a070_ids_search_arg_apdu(
  byte ** apdu,
  byte ** beg_apdu,
  signed32 * apdu_len,
  ids_search_arg * search_arg,
  ids_common_arg * com_arg,
  byte          * err_apdu )
{
signed32 max_length = *apdu_len;
byte * scan = *apdu;
const char function_name[] = "d2a070_ids_search_arg_apdu";

     if (d2a123_mk_apdu_cmarg(&scan,com_arg, beg_apdu, apdu_len,
			      err_apdu,apdu_len) == D2_ERROR )
	{
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	}

     ((D23_srcharg *) scan)->d23_Itag = D23_ITAG;
     ((D23_srcharg *) scan)->d23_Iali = TRUE;
     ((D23_srcharg *) scan)->d23_Iobjsub = search_arg->subset_mode;
     if (search_arg->alias == FALSE)
       ((D23_srcharg *) scan)->d23_Iali = FALSE;
     scan      += sizeof(D23_srcharg);

    if (d2a122_dn_apdu(D23_NAMTAG, &scan, &search_arg->base_object,
	       beg_apdu, &max_length, err_apdu,apdu_len)==D2_ERROR)
	{ 
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	 };


/*   for rq info  */
	if(d2a028_einfo_sel_to_req_attr(&search_arg->selection,
	   &scan,beg_apdu,&max_length,err_apdu,apdu_len) == D2_ERROR)
		{
		D2_ASN1_ERR_RETURN_WITH_TRACE
		}
     
/*   for filter info  */
	if(d2a078_ids_filter_to_filter1(&search_arg->filter,
	   &scan,beg_apdu,&max_length,err_apdu,apdu_len) == D2_ERROR)
		{
		D2_ASN1_ERR_RETURN_WITH_TRACE
		}

((D23_eom *) scan)->d23_eomtag = D23_EOMTAG;
((D23_eom *) scan)->d23_eomfil = D2_ALIGN(D23_EOMTAG);
*apdu = scan + sizeof(D23_eom);

return(D2_NOERROR);
}

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a078_ids_filter_to_filter1()                          */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      Transforms the filter    attributes from THORN-IDS format to  */
/*      APDU format                                                   */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      filter      = filter    attributes in IDS format              */
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
/*  AUTHOR: Praveen Gupta                                             */
/**********************************************************************/

static signed32 d2a078_ids_filter_to_filter1(
  ids_filter          *filter,
  byte              **act_apdu,
  byte              **beg_apdu,
  signed32                 *max_apdu_len,
  byte              *err_apdu,
  signed32                 *err_len  )       /* PKG */
{
D23_ft1_info    *f_info;
signed32              i;
const char function_name[] = "d2a078_ids_filter_to_filter1";

if  (filter->mode == FILTER_AND && filter->value.fil_and.nb_filters == 0)
  {
  return (D2_NOERROR);
  }

/*
   check if enough memory
*/
if(d2a033_check_memsize(sizeof(D23_ft1_info),beg_apdu,act_apdu,max_apdu_len,
   (D23_rserror *)err_apdu,err_len) != D2_NOERROR)
{
    d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_TEMP_ERR,
			   D2_SYS_ERR,D21_ALLOC_ERROR,err_len);
    D2_ASN1_ERR_RETURN_WITH_TRACE
}
f_info = (D23_ft1_info *)(*act_apdu);
f_info->d23_fp1tag = D23_PATTAG;
switch(filter->mode)
{
    case FILTER_ITEM:   f_info->d23_fp1mod = D2_ITEM;
			*act_apdu = (byte *)(f_info + 1);
			if(d2a079_ids_fitem_to_fitem(&filter->value.fil_item,
			   act_apdu,beg_apdu,max_apdu_len,err_apdu,err_len)
			   == D2_ERROR)
				{
				D2_ASN1_ERR_RETURN_WITH_TRACE
				}
			break;
    case FILTER_AND :   f_info->d23_fp1mod = D2_AND;
			f_info->d23_fp1count = filter->value.fil_and.
							     nb_filters;
			*act_apdu = (byte *)(f_info + 1);
			for(i = 0, filter = filter->value.fil_and.filters;
			    i < f_info->d23_fp1count; i++, filter++)
			{
			  if(d2a078_ids_filter_to_filter1(filter,act_apdu,
			     beg_apdu,max_apdu_len,err_apdu,err_len)
			     == D2_ERROR)
			    	{
				D2_ASN1_ERR_RETURN_WITH_TRACE
				}
			}
			break;
    case FILTER_OR  :   f_info->d23_fp1mod = D2_OR;
			f_info->d23_fp1count = filter->value.fil_or.
							     nb_filters;
			*act_apdu = (byte *)(f_info + 1);
			for(i = 0, filter = filter->value.fil_or.filters;
			    i < f_info->d23_fp1count; i++, filter++)
			{
			  if(d2a078_ids_filter_to_filter1(filter,act_apdu,
			     beg_apdu,max_apdu_len,err_apdu,err_len)
			     == D2_ERROR)
			    	{
				D2_ASN1_ERR_RETURN_WITH_TRACE
				}
			}
			break;
    case FILTER_NOT :   f_info->d23_fp1mod = D2_NOT;
			*act_apdu = (byte *)(f_info + 1);
			filter = filter->value.fil_not;
			if(d2a078_ids_filter_to_filter1(filter,act_apdu,
			   beg_apdu,max_apdu_len,err_apdu,err_len)
			   == D2_ERROR)
			  	{
				D2_ASN1_ERR_RETURN_WITH_TRACE
				}
			break;
}

return(D2_NOERROR);
}

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a079_ids_fitem_to_fitem()                             */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      Transforms the filter items         from THORN-IDS format to  */
/*      APDU format                                                   */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      filter_item = filter item          in IDS format              */
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
/*  AUTHOR: E. Kraemer                                                */
/**********************************************************************/

static signed32 d2a079_ids_fitem_to_fitem(
  ids_filter_item     *filter_item,
  byte              **act_apdu,
  byte              **beg_apdu,
  signed32                 *max_apdu_len,
  byte              *err_apdu,
  signed32                 *err_len )
{
D23_fi1_info   * f_item;
D23_fs1_info   * f_substr;
ids_pattern    * ids_patt;
signed32              i;
signed32              syntax;
D23_av_value   * av_value;
const char function_name[] = "d2a079_ids_fitem_to_fitem";

/*
   check if enough memory
*/
if(d2a033_check_memsize(sizeof(D23_fi1_info),beg_apdu,act_apdu,max_apdu_len,
   (D23_rserror *)err_apdu,err_len) != D2_NOERROR)
{
    d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_TEMP_ERR,
			   D2_SYS_ERR,D21_ALLOC_ERROR,err_len);
    D2_ASN1_ERR_RETURN_WITH_TRACE
}
f_item = (D23_fi1_info *)(*act_apdu);
f_item->d23_fi1tag = D23_FIITAG;
*act_apdu += sizeof(D23_fi1_info);
switch(filter_item->mode)
  {
    case FILTER_ITEM_EQUAL: f_item->d23_fi1match = D2_EQUAL;
			    if (d2a124_copy_attyp_apdu(act_apdu,
				  &filter_item->value.equal.type,
			       beg_apdu,max_apdu_len,err_apdu,
			       err_len) == D2_ERROR)
					{
					D2_ASN1_ERR_RETURN_WITH_TRACE
					}
			    f_item->d23_fi1syntx = syntax =
			    recover_att_syntax(&filter_item->value.equal.type);
			    if (syntax == D2_ERROR)
					f_item->d23_fi1syntx = D2_ASN1;
			    av_value = (D23_av_value *) *act_apdu;
			    av_value->d23_avfil = 0;
			    *act_apdu += sizeof(D23_av_value);
			    if(d2a106_ids_atval_to_atval(
			       &filter_item->value.equal.value,syntax,
			       &av_value->d23_avlen,&av_value->d23_avoff,
			       &av_value->d23_avrep,
			       act_apdu,beg_apdu,max_apdu_len,err_apdu,
			       err_len) == D2_ERROR)
					{
					D2_ASN1_ERR_RETURN_WITH_TRACE
					}
			    break;
    case FILTER_ITEM_SUBSTRING: f_item->d23_fi1match = D2_SUBSTRINGS;
			    if (d2a124_copy_attyp_apdu(act_apdu,
				  &filter_item->value.substring.type,
				  beg_apdu,max_apdu_len,err_apdu,
				  err_len) == D2_ERROR)
				    	{
					D2_ASN1_ERR_RETURN_WITH_TRACE
					}
			    f_item->d23_fi1syntx = syntax =
			    recover_att_syntax(
                                        &filter_item->value.substring.type);
			    if (syntax == D2_ERROR)
					f_item->d23_fi1syntx = D2_ASN1;
			    for (i = 0,
				 ids_patt = filter_item->value.substring.
								 patterns;
				i < filter_item->value.substring.nb_pattern;
				i++, ids_patt++)
			    {
				if(d2a033_check_memsize(sizeof(D23_fs1_info),
				   beg_apdu,act_apdu,max_apdu_len,
				   (D23_rserror *)err_apdu,err_len)
				   != D2_NOERROR)
				{
				    d27_010_set_error_apdu(
					(D23_rserror *)err_apdu,D2_TEMP_ERR,
					D2_SYS_ERR,D21_ALLOC_ERROR,
					err_len);
				    D2_ASN1_ERR_RETURN_WITH_TRACE
				}
				f_substr = (D23_fs1_info *)*act_apdu;
				f_substr->d23_fs1tag = D23_FISTAG;
				f_substr->d23_fs1position = ids_patt->mode;
				f_substr->d23_fs1fil = 0;
				*act_apdu += sizeof(D23_fs1_info);
				if(d2a106_ids_atval_to_atval(
				   &ids_patt->value,
				   syntax,&f_substr->d23_fs1len,
				   &f_substr->d23_fs1off,
				   &f_substr->d23_fs1rep,act_apdu,beg_apdu,
				   max_apdu_len,err_apdu,err_len)
				   == D2_ERROR)
				     	{
					D2_ASN1_ERR_RETURN_WITH_TRACE
					}
			    }
			    break;
    case FILTER_ITEM_GREATER_EQUAL: f_item->d23_fi1match = D2_GTEQ;
			    if (d2a124_copy_attyp_apdu(act_apdu,
				    &filter_item->value.gequal.type,
			       beg_apdu,max_apdu_len,err_apdu,
			       err_len) == D2_ERROR)
					{
					D2_ASN1_ERR_RETURN_WITH_TRACE
					}
			    f_item->d23_fi1syntx = syntax =
			    recover_att_syntax(&filter_item->value.gequal.type);
			    if (syntax == D2_ERROR)
					f_item->d23_fi1syntx = D2_ASN1;
			    av_value = (D23_av_value *) *act_apdu;
			    av_value->d23_avfil = 0;
			    *act_apdu += sizeof(D23_av_value);
			    if(d2a106_ids_atval_to_atval(
			       &filter_item->value.gequal.value,syntax,
			       &av_value->d23_avlen,&av_value->d23_avoff,
			       &av_value->d23_avrep,
			       act_apdu,beg_apdu,max_apdu_len,err_apdu,
			       err_len) == D2_ERROR)
					{
					D2_ASN1_ERR_RETURN_WITH_TRACE
					}
			    break;
    case FILTER_ITEM_LESS_EQUAL: f_item->d23_fi1match = D2_LTEQ;
			    if (d2a124_copy_attyp_apdu(act_apdu,
				&filter_item->value.lequal.type,
				beg_apdu,max_apdu_len,err_apdu,
				err_len) == D2_ERROR)
				  	{
					D2_ASN1_ERR_RETURN_WITH_TRACE
					}
			    f_item->d23_fi1syntx = syntax =
			    recover_att_syntax(&filter_item->value.lequal.type);
			    if (syntax == D2_ERROR)
					f_item->d23_fi1syntx = D2_ASN1;
			    av_value = (D23_av_value *) *act_apdu;
			    av_value->d23_avfil = 0;
			    *act_apdu += sizeof(D23_av_value);
			    if(d2a106_ids_atval_to_atval(
			       &filter_item->value.lequal.value,syntax,
			       &av_value->d23_avlen,&av_value->d23_avoff,
			       &av_value->d23_avrep,
			       act_apdu,beg_apdu,max_apdu_len,err_apdu,
			       err_len) == D2_ERROR)
					{
					D2_ASN1_ERR_RETURN_WITH_TRACE
					}
			    break;
    case FILTER_ITEM_PRESENT: f_item->d23_fi1match = D2_PRESENT;
			    if (d2a124_copy_attyp_apdu(act_apdu,
				  &filter_item->value.present,
				  beg_apdu,max_apdu_len,err_apdu,
				  err_len) == D2_ERROR)
				    	{
					D2_ASN1_ERR_RETURN_WITH_TRACE
					}
			    break;
    case FILTER_ITEM_APPROXIMATE:f_item->d23_fi1match = D2_APPROX_MATCH;
			    if (d2a124_copy_attyp_apdu(act_apdu,
				&filter_item->value.approx_match.type,
				beg_apdu,max_apdu_len,err_apdu,
				err_len) == D2_ERROR)
				   	{
					D2_ASN1_ERR_RETURN_WITH_TRACE
					}
			    f_item->d23_fi1syntx = syntax =
			    recover_att_syntax(&filter_item->value.approx_match.type);
			    if (syntax == D2_ERROR)
					f_item->d23_fi1syntx = D2_ASN1;
			    av_value = (D23_av_value *) *act_apdu;
			    av_value->d23_avfil = 0;
			    *act_apdu += sizeof(D23_av_value);
			    if(d2a106_ids_atval_to_atval(
			       &filter_item->value.approx_match.value,syntax,
			       &av_value->d23_avlen,&av_value->d23_avoff,
			       &av_value->d23_avrep,
			       act_apdu,beg_apdu,max_apdu_len,err_apdu,
			       err_len) == D2_ERROR)
					{
					D2_ASN1_ERR_RETURN_WITH_TRACE
					}
  }

return(D2_NOERROR);
}

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a072_apdu_search_res_ids()                            */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*           This function transforms search_result from APDU format  */
/*  to the read result and common result of the ids interface.        */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      apdu        = APDU for the search result .                    */
/*      apdu_len    = APDU length for the search result .             */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      search_res  = search result in IDS_interface.                 */
/*      com_res   = Common result of IDS_interface.                   */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_ERROR        transformation fails                          */
/*                      (err_apdu and err_len will be returned)       */
/*      D2_NOERROR      transformation is successful                  */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: Ernst Kraemer                                             */
/**********************************************************************/

signed32 d2a072_apdu_search_res_ids(
  byte * apdu,
  signed32 apdu_len,
  ids_search_res * search_res,
  ids_common_res * com_res )
{
byte err_apdu[D2A_ERR_APDU_SIZE];
signed32 er_len;
byte * scan = apdu;
ids_entry_info *entry_info;
D23_ds_name * ds_name = (D23_ds_name *) apdu;
const char function_name[] = "d2a072_apdu_search_res_ids";

    search_res->uncorrelated   = FALSE;
    if (ds_name->d23_dstag == D23_BONTAG)
    {
	search_res->info.search_info.dname_defined = TRUE;
	if (d2a121_dn_ids(&scan, &(search_res->info.search_info.dname),
		       err_apdu, &er_len) == D2_ERROR)
	 {
	   D2_ASN1_ERR_RETURN_WITH_TRACE
	  }
    }
    else
	search_res->info.search_info.dname_defined = FALSE;

    search_res->info.search_info.nb_info = d2a107_count_entry_infos(scan);
    if((search_res->info.search_info.infos = D27_CALLOC( svc_c_sev_warning,
       search_res->info.search_info.nb_info+1, sizeof(ids_entry_info))) == NULL)
    {
	d27_010_set_error_apdu((D23_rserror *)err_apdu,D2_TEMP_ERR,
			  D2_SYS_ERR,D21_ALLOC_ERROR,&er_len);
	D2_ASN1_ERR_RETURN_WITH_TRACE
    }
    entry_info = search_res->info.search_info.infos;
    while(((D23_en_info *)scan)->d23_entag == D23_ENTTAG)
    {
			/* entry-information found                  */
        entry_info->nb_info = ((D23_en_info *)scan)->d23_enmaxat;
	entry_info->from_entry = (Boolean)(((D23_en_info *)scan)->d23_enmas);
	scan += sizeof(D23_en_info);
	if (d2a121_dn_ids(&scan, &(entry_info->dist_name),
		       err_apdu,&er_len) == D2_ERROR)
	 {
	   D2_ASN1_ERR_RETURN_WITH_TRACE
	  }
			/* get attribute informations               */
	if(d2a030_at_info_to_entry_info(&scan,
				(signed32 *)&(entry_info->nb_info),
	   &(entry_info->infos),err_apdu,&er_len) == D2_ERROR)
	   {
	     D2_ASN1_ERR_RETURN_WITH_TRACE
	    }
	if(entry_info->nb_info > 0)
	  entry_info->infos_defined = TRUE;
	else
	  entry_info->infos_defined = FALSE;
	entry_info++;
    }
    search_res->info.search_info.part_oq_defined = FALSE;
    if(((D23_part_oq *)scan)->d23_oqtag == D23_OQTAG )
    {
	search_res->info.search_info.part_oq_defined = TRUE;
	if(d2a108_mk_ids_part_oq(&scan,&(search_res->info.search_info.
			      part_oq),err_apdu,&er_len) == D2_ERROR)
	 {
	   D2_ASN1_ERR_RETURN_WITH_TRACE
	  }
    }
    d2a081_mk_ids_comres(&scan, com_res);

return(D2_NOERROR);
}

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a107_count_entry_infos()                              */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*           This function counts the entry informations contained    */
/*  in a search- or list-result-APDU.                                 */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      apdu        = pointer to the first entry-information.         */
/*      apdu_len    = APDU length for the search result .             */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      Number of entry-informations.                                 */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: Ernst Kraemer                                             */
/**********************************************************************/

static signed32 d2a107_count_entry_infos(
  byte * apdu )
{
signed32 count = 0, i;
signed16 off;
unsigned16 tag;
char comment[30];

    tag = ((D23_en_info *)apdu)->d23_entag;
    while(tag == D23_ENTTAG)
    {
	count++;            /* entry found, count                   */
	apdu += sizeof(D23_en_info);
			    /* skip distinguished name now          */
	off = ((D23_ds_name *)apdu)->d23_dsoff;
	apdu += sizeof(D23_ds_name) + off;
	tag = ((D23_at_type *)apdu)->d23_atttag;
	while(tag == D23_ATTTAG)
	{
			    /* Attribute information, must be skipped */
	    off = ((D23_at_type *)apdu)->d23_atoff;
	    apdu += sizeof(D23_at_type) + off;
	    if (((D23_av_info *)apdu)->d23_avtag == D23_AVTAG)
	      {
	      i = ((D23_av_info *)apdu)->d23_avnum;
	      apdu += D23_S_AV;
	      while (i > 0 && (unsigned16)(((D23_av_value *)apdu)->d23_avlen)
								< D23_MINTAG)
	        {
			    /* Attribute value, must be skipped       */
		off = ((D23_av_value *)apdu)->d23_avoff;
		apdu += sizeof(D23_av_value) + off;
		i -=1;
	        }
	      }
	    tag = ((D23_at_type *)apdu)->d23_atttag;
	}
	tag = ((D23_en_info *)apdu)->d23_entag;
    }

sprintf(comment,"%d infos counted",count);

return(count);
}
