/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d24asearch.c,v $
 * Revision 1.1.10.2  1996/02/18  19:42:49  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:23:39  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:23:46  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:53:31  root]
 * 
 * Revision 1.1.8.3  1994/06/21  14:44:15  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:26:41  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:07:21  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:14:57  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:50:54  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:41:22  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  10:45:45  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  12:33:39  marrek]
 * 
 * Revision 1.1.4.4  1993/02/02  10:09:59  marrek
 * 	Fixes for OT 6572
 * 	[1993/02/01  17:50:36  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:12:39  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:09:57  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  15:56:11  marrek
 * 	November 1992 code drop
 * 	[1992/11/17  12:49:26  marrek]
 * 
 * Revision 1.1.2.3  1992/06/30  20:54:49  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  20:33:51  melman]
 * 
 * Revision 1.1.2.2  1992/06/01  20:05:27  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:01:59  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d24asearch.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:42:49 $";
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
/* NAME         : d24asearch.c                                        */
/*								      */
/* AUTHOR       : Praveen Gupta, Siemens India, Delhi.                */
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
#include <sys/types.h>
#include <stdlib.h>

#include <d2asn1.h>
#include <d2asn1_int.h>

/******** LOCAL **********/

static signed32 d2a110_filter1_to_ids_filter ( char ** , ids_filter * ,
						Octet_string , signed32 * );
static signed32 d2a111_fitem_to_ids_fitem( D23_fi1_info ** , ids_filter_item * ,
					Octet_string , signed32 * );

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a069_apdu_search_arg_ids()                            */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*           This function transforms search_arg's from APDU format to*/
/*  the search arguments and common arguments of the ids interface.   */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      apdu        = APDU for the search arguments.                  */
/*      apdu_len    = APDU length for the search arguments.           */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      search_arg  = search arguments in IDS_interface.              */
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
/*  AUTHOR: Ernst Kraemer                                             */
/**********************************************************************/

signed32 d2a069_apdu_search_arg_ids(
  byte * apdu,
  signed32 * apdu_len,
  ids_search_arg * search_arg,
  ids_common_arg * com_arg,
  byte * err_apdu )
{
D23_srcharg  *srcharg = (D23_srcharg *)apdu;
byte *scan = apdu;
#ifndef DSA
signed32 version = ((D23_header *)apdu)->d23_versno;
#endif
const char function_name[] = "d2a069_apdu_search_arg_ids";

#ifndef DSA
    if                              /*  IPC version is not ok         */
      (!D2_CHECK_MINMAX(D23_APDUV22,D23_APDUV22,version))
    {                               /*  reset to 0 and return error   */
      d27_010_set_error_apdu((D23_rserror *)apdu,D2_CALL_ERR,D2_ILL_VERS,
			     D2_NOT_SPEC,apdu_len);
      D2_ASN1_ERR_RETURN_WITH_TRACE
    }                               /*                                */

    scan += D23_S_HDR;
#endif
    d2a080_mk_ids_comarg(&scan,com_arg);
    srcharg = (D23_srcharg *)scan;

    search_arg->subset_mode = srcharg->d23_Iobjsub;
    search_arg->alias = srcharg->d23_Iali;

    scan += sizeof(D23_srcharg);

    if (d2a121_dn_ids(&scan, &search_arg->base_object, err_apdu,
			apdu_len) == D2_ERROR) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}

    /*   for rq info  */
    if(d2a019_req_to_einfo_sel(&scan,&search_arg->selection,
	  err_apdu, apdu_len) == D2_ERROR)
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}

    /* set default values             */
    search_arg->filter.mode = FILTER_AND;
    search_arg->filter.value.fil_and.nb_filters = 0;
    if(d2a110_filter1_to_ids_filter((char **)&scan,
       &search_arg->filter,err_apdu,apdu_len)
       == D2_ERROR)
    {
      D2_ASN1_ERR_RETURN_WITH_TRACE
    }

return(D2_NOERROR);
}

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a110_filter1_to_ids_filter()                          */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      Transforms the filter informations  from new APDU format to   */
/*      THORN-IDS format ("ids_filter")                               */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      apdu        = pointer to the APDU containing filter infor-    */
/*                    mations                                         */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      apdu        = pointer to next tag in the APDU                 */
/*      filter      = filter-information in IDS format                */
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

static signed32 d2a110_filter1_to_ids_filter(
  char               **apdu,
  ids_filter         *filter,
  Octet_string       err_apdu,
  signed32                *err_len )
{
D23_ft1_info    *f_info = (D23_ft1_info *) *apdu;
ids_filter      *df_tmp;
signed32             i;
const char function_name[] = "d2a110_filter1_to_ids_filter";

if(f_info->d23_fp1tag == D23_PATTAG)
{
    switch(f_info->d23_fp1mod)
    {
	case D2_AND:    filter->mode = FILTER_AND;
			filter->value.fil_and.nb_filters =
					      f_info->d23_fp1count;
			if             /* no memory available           */
			  ((filter->value.fil_and.filters = 
			   D27_CALLOC(svc_c_sev_warning,
					filter->value.fil_and.nb_filters,
					   sizeof(ids_filter))) == NULL)
			{
			   d27_010_set_error_apdu((D23_rserror *) err_apdu,
				   D2_TEMP_ERR,D2_SYS_ERR,D21_ALLOC_ERROR,
				   err_len);
			   D2_ASN1_ERR_RETURN_WITH_TRACE
			}
			else
			{
			  for          /* all filters in apdu            */
			     (f_info += 1, i = 0,
			      df_tmp = filter->value.fil_and.filters;
			      i < filter->value.fil_and.nb_filters;
			      i++, df_tmp++)
			  {            /*  convert the components        */
			      if(d2a110_filter1_to_ids_filter(
				 (char **)&(f_info),df_tmp,err_apdu,err_len)
				 == D2_ERROR)
			      {
				D2_ASN1_ERR_RETURN_WITH_TRACE
			      }
			  }
			}
			break;
	case D2_OR:     filter->mode = FILTER_OR;
			filter->value.fil_or.nb_filters =
					     f_info->d23_fp1count;
			if             /* no memory available           */
			  ((filter->value.fil_or.filters =
			   D27_CALLOC(svc_c_sev_warning,
					filter->value.fil_or.nb_filters,
					   sizeof(ids_filter))) == NULL)
			{
			   d27_010_set_error_apdu((D23_rserror *) err_apdu,
				   D2_TEMP_ERR,D2_SYS_ERR,D21_ALLOC_ERROR,
				   err_len);
			   D2_ASN1_ERR_RETURN_WITH_TRACE
			}
			else
			{
			  for          /* all filters in apdu            */
			     (f_info += 1, i = 0,
			      df_tmp = filter->value.fil_or.filters;
			      i < filter->value.fil_or.nb_filters;
			      i++, df_tmp++)
			  {            /*  convert the components        */
			      if(d2a110_filter1_to_ids_filter(
				 (char **)&(f_info),df_tmp,err_apdu,err_len)
				 == D2_ERROR)
			      {
				D2_ASN1_ERR_RETURN_WITH_TRACE
			      }
			  }
			}
			break;
	case D2_NOT:    filter->mode = FILTER_NOT;
			if             /* no memory available           */
			  ((filter->value.fil_not = 
			   D27_CALLOC(svc_c_sev_warning,
					1,sizeof(ids_filter))) == NULL)
			{
			  d27_010_set_error_apdu((D23_rserror *) err_apdu,
				  D2_TEMP_ERR,D2_SYS_ERR,D21_ALLOC_ERROR,
				  err_len);
			  D2_ASN1_ERR_RETURN_WITH_TRACE
			}
			else
			{
			  f_info += 1;
				       /*  convert the negated filter    */
			  if(d2a110_filter1_to_ids_filter((char **)&(f_info),
			     filter->value.fil_not,err_apdu,err_len)
			     == D2_ERROR)
			  {
			    D2_ASN1_ERR_RETURN_WITH_TRACE
			  }
			}
			break;
	case D2_ITEM:   filter->mode = FILTER_ITEM;
			f_info += 1;
				       /*  convert the filter item       */
			if(d2a111_fitem_to_ids_fitem((D23_fi1_info **)&f_info,
			   &filter->value.fil_item,
			   err_apdu,err_len) == D2_ERROR)
			{
			  D2_ASN1_ERR_RETURN_WITH_TRACE
			}
			break;
  }
}

*apdu = (char *)f_info;

return(D2_NOERROR);
}

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a111_fitem_to_ids_fitem()                             */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      Transforms the filter item from new APDU format to            */
/*      IDS format ("ids_filter_item")                                */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      f_item      = pointer to the APDU containing the filter item  */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      f_item      = pointer to next tag in the APDU                 */
/*      item      = filter-item in IDS format                       */
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

static signed32 d2a111_fitem_to_ids_fitem(
  D23_fi1_info       **f_item,
  ids_filter_item    *item,
  Octet_string       err_apdu,
  signed32                *err_len )
{
D23_fs1_info    *f_substr;
dua_att_value   dua_at_val;
byte           * scan = (byte *)*f_item + D23_S_FI1;
ids_pattern     *pattern;
D23_av_value    * av_value;
signed32             syntax;
const char function_name[] = "d2a111_fitem_to_ids_fitem";

switch((*f_item)->d23_fi1match)
{
  case D2_EQUAL:  item->mode = FILTER_ITEM_EQUAL;
		  if (d2a120_copy_att_type(&scan, &(item->value.equal.type),
			err_apdu, err_len) == D2_ERROR)
		  {   
		      D2_ASN1_ERR_RETURN_WITH_TRACE
		  }

		  syntax = (*f_item)->d23_fi1syntx;
		  av_value = (D23_av_value *) scan;
		  if(d2a031_cp_attr(av_value->d23_avrep,av_value->d23_avlen,
		     (byte *)(av_value +1),&dua_at_val,
		     err_apdu, err_len) != D2_NOERROR)
		  {   
		      D2_ASN1_ERR_RETURN_WITH_TRACE
		  }

		  if (dua_att_value_ids(&dua_at_val,&item->value.equal.value) == D2_ERROR)
		  {   
		      D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		  if (syntax != D2_DISTNAME)
		  	dua_att_value_free(&dua_at_val);
		  *f_item = (D23_fi1_info *)((String)(av_value + 1) +
					     av_value->d23_avoff);
		  break;
  case D2_SUBSTRINGS:   item->mode = FILTER_ITEM_SUBSTRING;
		  if (d2a120_copy_att_type(&scan, &(item->value.substring.type),
			err_apdu, err_len)  == D2_ERROR)
		  {   
		      D2_ASN1_ERR_RETURN_WITH_TRACE
		  }

		  syntax = (*f_item)->d23_fi1syntx;
		  for(f_substr = (D23_fs1_info *)scan,
		      item->value.substring.nb_pattern = 0;
		      f_substr->d23_fs1tag == D23_FISTAG;
		      item->value.substring.nb_pattern++,
		      f_substr = (D23_fs1_info *)((String)(f_substr + 1) +
				 f_substr->d23_fs1off))
		  {
				/* just count                         */
		  }
		  if((item->value.substring.patterns =
		     D27_CALLOC(svc_c_sev_warning,
					item->value.substring.nb_pattern,
		     sizeof(ids_pattern))) == NULL)
		  {
		     d27_010_set_error_apdu((D23_rserror *)err_apdu,
			     D2_TEMP_ERR,D2_SYS_ERR,D21_ALLOC_ERROR,
			      err_len);
		     D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		  for(f_substr = (D23_fs1_info *)scan,
		      pattern = item->value.substring.patterns;
		      f_substr->d23_fs1tag == D23_FISTAG;
		      pattern++,
		      f_substr = (D23_fs1_info *)((String)(f_substr + 1) +
				 f_substr->d23_fs1off))
		    {
		    pattern->mode = f_substr->d23_fs1position;
	            if(d2a031_cp_attr(f_substr->d23_fs1rep,f_substr->d23_fs1len,
			 (byte *)(f_substr + 1),&dua_at_val,
			 err_apdu, err_len) != D2_NOERROR)
		      {   
			  D2_ASN1_ERR_RETURN_WITH_TRACE
		      }

		    if (dua_att_value_ids(&dua_at_val,&pattern->value) == D2_ERROR)
		      {
		       D2_ASN1_ERR_RETURN_WITH_TRACE
		       }
		    if (syntax != D2_DISTNAME)
		      	dua_att_value_free(&dua_at_val);
		     }
		  *f_item = (D23_fi1_info *)f_substr;
	          break;
  case D2_APPROX_MATCH: item->mode = FILTER_ITEM_APPROXIMATE;
		  if (d2a120_copy_att_type(&scan, &(item->value.approx_match.type),
			err_apdu, err_len)  == D2_ERROR)
		  {   
		      D2_ASN1_ERR_RETURN_WITH_TRACE
		  }

		  syntax = (*f_item)->d23_fi1syntx;
		  av_value = (D23_av_value *) scan;
		  if(d2a031_cp_attr(av_value->d23_avrep,av_value->d23_avlen,
		     (byte *)(av_value + 1),&dua_at_val,
		     err_apdu, err_len) != D2_NOERROR)
		  {   
		      D2_ASN1_ERR_RETURN_WITH_TRACE
		  }

		  if (dua_att_value_ids(&dua_at_val,
				   &item->value.approx_match.value) == D2_ERROR)
		  {   
		      D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
		  if (syntax != D2_DISTNAME)
		     dua_att_value_free(&dua_at_val);
		  *f_item = (D23_fi1_info *)((String)(av_value + 1) +
					     av_value->d23_avoff);

		  break;
  case D2_PRESENT:item->mode = FILTER_ITEM_PRESENT;
		  if (d2a120_copy_att_type(&scan, &(item->value.equal.type),
			err_apdu, err_len)  == D2_ERROR)
		  {   
		      D2_ASN1_ERR_RETURN_WITH_TRACE
		  }

		  *f_item = (D23_fi1_info *)scan;
}

return(D2_NOERROR);
}

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a071_ids_search_res_apdu()                            */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*           This function transforms search_arg's from ids format to */
/*  the search results and common results   of the APDU interface.    */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      search_res  = search result in IDS_interface.               */
/*      com_res   = Common result of IDS_interface.                 */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      apdu        = APDU for the search result .                    */
/*      apdu_len    = APDU length for the search result .             */
/*      err_apdu    = error APDU (memory error or syntax error)       */
/*      apdu_len     = length of error APDU                           */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_ERROR        transformation fails                          */
/*                      (err_apdu and err_len will be returned)       */
/*      D2_NOERROR      transformation is successful                  */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: Ernst Kraemer                                             */
/**********************************************************************/

signed32 d2a071_ids_search_res_apdu(
  byte ** cur_apdu,
  byte ** apdu,
  signed32 * apdu_len,
  ids_search_res * search_res,
  ids_common_res * com_res,
  byte * err_apdu )
{
signed32 max_length;
signed32 i;
byte * scan;
D23_en_info * en_info;
ids_cont_reference  *cont_reference;
ids_entry_info *entry_info;
D23_part_oq * apdu_poq;
signed32 len;
const char function_name[] = "d2a071_ids_search_res_apdu";

#ifndef DSA
    if ((*cur_apdu = *apdu = D27_MALLOC(svc_c_sev_warning,D2A_APDU_MEM))==NULL)
    {
	  d27_010_set_error_apdu((D23_rserror *)err_apdu,D2_TEMP_ERR,
		D2_SYS_ERR,D21_ALLOC_ERROR,apdu_len);
	  D2_ASN1_ERR_RETURN_WITH_TRACE
    }
#endif
    scan = *cur_apdu;
    max_length = (*apdu_len > D2A_APDU_MEM) ? *apdu_len : D2A_APDU_MEM;

    if(search_res->uncorrelated == TRUE)
    {                   /* uncorrelated search-results are coming in    */
	  d27_010_set_error_apdu((D23_rserror *)err_apdu,D2_PERM_ERR,
		D2_INT_ERR,D2_NOT_SPEC, apdu_len);
	  free(*apdu);
	  D2_ASN1_ERR_RETURN_WITH_TRACE
    }
    if(search_res->info.search_info.dname_defined == TRUE)
    {
	if(d2a122_dn_apdu(D23_BONTAG, &scan,
	    &(search_res->info.search_info.dname),
	    apdu,&max_length, err_apdu,apdu_len) == D2_ERROR)
	{
	  free(*apdu);
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	}
    }

   /* Common results are avoided as the mapping of comm_res is not known */
				/* set pointer to entry information     */

    for(i = 0, entry_info = search_res->info.search_info.infos;
	i < search_res->info.search_info.nb_info; i++, entry_info++)
    {
	    /* check memory for entry-information structure           */
	if(d2a033_check_memsize(sizeof(D23_en_info),apdu,&scan,&max_length,
	   (D23_rserror *)err_apdu,apdu_len) != D2_NOERROR)
	{
	    free(*apdu);
	    d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_TEMP_ERR,
			D2_SYS_ERR,D21_ALLOC_ERROR,apdu_len);
	    D2_ASN1_ERR_RETURN_WITH_TRACE
	}
		/* write components of entry information structure  */
	en_info = (D23_en_info *)scan;
	en_info->d23_entag = D23_ENTTAG;
	en_info->d23_enmas = entry_info->from_entry;
	en_info->d23_enmaxat = entry_info->nb_info;
	scan += sizeof(D23_en_info);
		/* write distinguished name after entry information   */
	if(d2a122_dn_apdu(D23_NAMTAG,&scan, &entry_info->dist_name,
	   apdu,&max_length,err_apdu,apdu_len) == D2_ERROR)
	{
	  free(*apdu);
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	}
	if(entry_info->infos_defined)
	{
		/* write attribute informations                       */
	    if(d2a029_entry_info_to_at_info(entry_info->nb_info,
	       entry_info->infos,&scan,apdu,&max_length,
						err_apdu,apdu_len)
	       == D2_ERROR)
	    {
	       free(*apdu);
	       D2_ASN1_ERR_RETURN_WITH_TRACE
	    }
	}
    }
    if(search_res->info.search_info.part_oq_defined == TRUE )
    {
	/* check memory for partial outcome qualifier structure           */
	if(d2a033_check_memsize(sizeof(D23_part_oq),apdu,&scan,&max_length,
	   (D23_rserror *)err_apdu,apdu_len) != D2_NOERROR)
	{
	    free(*apdu);
	    d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_TEMP_ERR,
			D2_SYS_ERR,D21_ALLOC_ERROR,apdu_len);
	    D2_ASN1_ERR_RETURN_WITH_TRACE
	}

	len = scan - *apdu;
	apdu_poq = (D23_part_oq *)scan;
	apdu_poq->d23_oqtag = D23_OQTAG;
	apdu_poq->d23_oq_limit = D2_NO_LIMIT_EX;
	if (search_res->info.search_info.part_oq.limit_pb_defined == TRUE)
	   apdu_poq->d23_oq_limit = search_res->info.search_info.part_oq.limit_pb;
	apdu_poq->d23_oq_uncrit = FALSE;
	if (search_res->info.search_info.part_oq.un_crit_ext != FALSE)
	   apdu_poq->d23_oq_uncrit = search_res->info.search_info.part_oq.un_crit_ext;
	scan += D23_S_POQ;
        if(search_res->info.search_info.part_oq.unexplored_defined == TRUE )
	 for(i = 0, cont_reference = search_res->info.search_info.
	      part_oq.unexplored;
	      i < search_res->info.search_info.part_oq.nb_unexplored;
	      i++, cont_reference++)
	{
	    if(d2a053_put_ref(cont_reference,&scan,apdu,&max_length,
	       err_apdu,apdu_len) == D2_ERROR)
	    {
	       free(*apdu);
	       D2_ASN1_ERR_RETURN_WITH_TRACE
	    }
	}
    }
    /* check memory for entry-information structure           */
    if(d2a033_check_memsize(sizeof(D23_com_res) + sizeof(D23_eom),apdu,&scan,
       &max_length,(D23_rserror *)err_apdu,apdu_len) != D2_NOERROR)
    {
	free(*apdu);
	d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_TEMP_ERR,
		    D2_SYS_ERR,D21_ALLOC_ERROR,apdu_len);
	D2_ASN1_ERR_RETURN_WITH_TRACE
    }

    d2a130_mk_apdu_comres (com_res, &scan);
    ((D23_eom *) scan)->d23_eomtag = D23_EOMTAG;
    ((D23_eom *) scan)->d23_eomfil = D2_ALIGN(D23_EOMTAG);
    *cur_apdu = scan + sizeof(D23_eom);
    *apdu_len = *cur_apdu - *apdu;

    if (search_res->info.search_info.part_oq_defined == TRUE)
      ((D23_eom *) scan)->d23_eom_oqoff = *apdu_len - len;
    else
      ((D23_eom *) scan)->d23_eom_oqoff = 0 ;

return(D2_NOERROR);
}
