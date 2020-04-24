/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d24acharg.c,v $
 * Revision 1.1.10.2  1996/02/18  19:42:39  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:23:29  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:23:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:53:24  root]
 * 
 * Revision 1.1.8.3  1994/06/21  14:44:08  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:26:09  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:07:13  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:14:30  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:50:35  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:39:56  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  10:18:20  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  12:31:55  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:12:18  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:09:24  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  15:54:02  marrek
 * 	November 1992 code drop
 * 	[1992/11/17  12:47:43  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:04:27  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:01:11  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d24acharg.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:42:39 $";
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
/* NAME         : d24acharg.c                                         */
/*								      */
/* AUTHOR       : Praveen Gupta, Siemens India, Delhi                 */
/* DATE         : 26. 2. 1990                                         */
/*								      */
/* DESCRIPTION  : This source files contains subroutiones to          */
/*                transform ChainingArg-APDU to IDS interface.        */
/*								      */
/* SYSTEM DEPENDENCIES:  COMMON                                       */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   0.1    | 90-02-26| Original                       | WS |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

#include <gds.h>
#include <time.h>
#include <stdlib.h>

#include <d2asn1.h>
#include <d2asn1_int.h>

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a150_apdu_charg_ids()                                 */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*           This function transforms ChainingArg's from APDU format to*/
/*  the ChainingArgs of the ids interface.                            */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      apdu        = APDU for the Chaining arguments.                */
/*      apdu_len    = APDU length for the Chaining arguments.         */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      ch_arg  = chaining arguments in IDS_interface.                */
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

#define MAX_UTC_LEN 24

signed32 d2a150_apdu_charg_ids(
  byte ** apdu,
  signed32 * apdu_len,
  ids_ch_arg * ch_arg,
  byte * err_apdu )
{
const char function_name[] = "d2a150_apdu_charg_ids";
  byte * scan  = *apdu + sizeof(D23_header);
  D23_chain_arg  * d23_ch_arg = (D23_chain_arg *) scan;
     signed32         rt_val;
    time_t       clock;
    struct timeb tmb;

    ch_arg->alias_deref = (d23_ch_arg->d23_al_deref == FALSE)
			    ? FALSE : TRUE;
    ch_arg->aliased_rdn = (d23_ch_arg->d23_al_RDN == D2_NOT_DEFINED)
			    ? 0 : d23_ch_arg->d23_al_RDN;
    ch_arg->entry_only = (d23_ch_arg->d23_entry_only == FALSE)
			    ? FALSE : TRUE;
    ch_arg->rt_cross_refs = (d23_ch_arg->d23_ret_cref == FALSE)
			    ? FALSE : TRUE;
    ch_arg->ref_type = d23_ch_arg->d23_ref_type;

 /* domain info is not supported at the moment */
    ch_arg->domain_info.l = IDS_UNDEFINED;

    ch_arg->time_limit.nb = IDS_UNDEFINED;
    if (d23_ch_arg->d23_chtmlt != D2_T_UNLIMITED)
      {
	clock = time(0L);
	tmb.time = clock + d23_ch_arg->d23_chtmlt;
	tmb.timezone = tmb.dstflag = tmb.millitm = 0;
	if ((ch_arg->time_limit.contents = 
			D27_MALLOC(svc_c_sev_warning,MAX_UTC_LEN))== NULL)
	 {
	   d27_010_set_error_apdu((D23_rserror *)err_apdu,D2_TEMP_ERR,
				  D2_SYS_ERR, D21_ALLOC_ERROR, apdu_len);
	   D2_ASN1_ERR_RETURN_WITH_TRACE
	  }
	ch_arg->time_limit.nb = asn1_utc_time((char *)ch_arg->time_limit.contents,&tmb);
       }
    ch_arg->op_prog.name_res_ph = d23_ch_arg->d23_op_prog.d23_opnm_rs;
    ch_arg->op_prog.next_rdn_defined = (d23_ch_arg->d23_op_prog.d23_opnm_rs
					 == D2_PROCEEDING)? TRUE:FALSE;
    ch_arg->op_prog.next_rdn = d23_ch_arg->d23_op_prog.d23_op_nxtrdn;
    scan += D23_S_CHARG;
    rt_val = d2a152_tr_info_ids(&scan, apdu_len, d23_ch_arg->d23_max_tr,
				&ch_arg->trace_infos, err_apdu);
    if (rt_val == D2_ERROR) 
      {
      D2_ASN1_ERR_RETURN_WITH_TRACE
      }

    if (((D23_ds_name *)scan)->d23_dstag == D23_RQTAG)
      if (((D23_ds_name *)scan)->d23_dsoff != 0)
       { 
	if (d2a121_dn_ids(&scan, &ch_arg->originator, err_apdu,
				apdu_len) == D2_ERROR) 
	    { 
	    D2_ASN1_ERR_RETURN_WITH_TRACE
	    }
        }
      else
	{
/* mac - core dumping when calling i_dist_name_free. Crashes when trying to
         free ch_arg->originator.rdnames if nb_rdnames is set to 0. 
         therefore set this to IDS_UNDEFINED for the moment.

	  ch_arg->originator.nb_rdnames = 0;
 */
	  ch_arg->originator.nb_rdnames = IDS_UNDEFINED;
	  scan += D23_S_NM;
	}
    else
     ch_arg->originator.nb_rdnames = IDS_UNDEFINED;

    if (((D23_ds_name *)scan)->d23_dstag == D23_BONTAG)
      if (((D23_ds_name *)scan)->d23_dsoff != 0)
       {
          if (d2a121_dn_ids(&scan, &ch_arg->target, err_apdu,apdu_len)
		      == D2_ERROR)
	    {
	    D2_ASN1_ERR_RETURN_WITH_TRACE
	    }
        }
      else
	{
	  ch_arg->target.nb_rdnames = 0;
	  scan += D23_S_NM;
	}
    else
     ch_arg->target.nb_rdnames = IDS_UNDEFINED;

   *apdu = scan;
   return(D2_NOERROR);

  }

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a151_apdu_chres_ids()                                 */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*           This function transforms ChainingRes's from APDU format to*/
/*  the ChainingRes  of the ids interface.                            */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      ch_res  = chaining results in IDS_interface.                  */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      apdu        = APDU for the Chaining Results.                  */
/*      apdu_len    = APDU length for the Chaining Results.           */
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

signed32 d2a151_ids_chres_apdu(
  byte ** act_apdu,
  byte ** apdu,
  signed32 * apdu_len,
  ids_ch_res * ch_res,
  byte * err_apdu )
{
const char function_name[] = "d2a151_ids_chres_apdu";
  byte * scan = *act_apdu;
  signed32 i,max_len = *apdu_len;
  ids_cross_ref * cr_ref;

    ((D23_chain_res *)scan)->d23_chrtag = D23_CHRTAG;
    if (ch_res->nb_cross_ref != IDS_UNDEFINED)
     {
       ((D23_chain_res *)scan)->d23_max_cref = ch_res->nb_cross_ref;
       scan += D23_S_CHRES;
       for (i=0,cr_ref=ch_res->cross_refs; i<ch_res->nb_cross_ref; i++,cr_ref++)
	{
	    ((D23_cross_ref *)scan)->d23_creftag = D23_CREFTAG;
	    scan += D23_S_CREF;
	    if (d2a122_dn_apdu( D23_NAMTAG, &scan, &cr_ref->ctx_prefix,
		   apdu, &max_len, err_apdu,apdu_len) == D2_ERROR)
	      {
		D2_ASN1_ERR_RETURN_WITH_TRACE
	       }
	    ((D23_acc_info *)scan)->d23_acctag = D23_ACCTAG;
	    scan += D23_S_ACC;

	    /* DSA-name APDU */
	    if (d2a122_dn_apdu( D23_NAMTAG, &scan, &cr_ref->acc_pt.name,
		   apdu, &max_len, err_apdu,apdu_len) == D2_ERROR)
	     {
	       D2_ASN1_ERR_RETURN_WITH_TRACE
	      }

	    /* DSA-ADDRESS APDU */
	    if (d2a061_get_psap( (dua_psap *)&(cr_ref->acc_pt.addr),
		     TRUE, &scan, apdu, &max_len,err_apdu,apdu_len)
								== D2_ERROR)
	     {
		  D2_ASN1_ERR_RETURN_WITH_TRACE
	      }
	 }
      }
      else
      {
       ((D23_chain_res *)scan)->d23_max_cref = 0;
       scan += D23_S_CHRES;
      }
    /*
       Domain info is not supported
    */
   *act_apdu = scan;
   return(D2_NOERROR);
 }


/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a152_tr_info_ids()                                    */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function transforms Trace Infos from APDU format to      */
/*      the Trace Infos  of the ids interface.                        */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      apdu        = APDU for the Trace Infos.                       */
/*      apdu_len    = APDU length for the Trace Infos.                */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      tr_info = Trace Information in IDS_interface.                 */
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


signed32 d2a152_tr_info_ids(
  byte ** apdu,
  signed32 * apdu_len,
  signed32 nb_trace,
  ids_trace_infos * tr_info,
  byte * err_apdu )
{
const char function_name[] = "d2a152_tr_info_ids";
  signed32 i;
  D23_trace_info * d23_tinfo = (D23_trace_info *) *apdu;
  byte          * scan = *apdu;
  ids_trace_info * tinfo;

	if ((tr_info->list = tinfo = D27_MALLOC
		     (svc_c_sev_warning,nb_trace*sizeof(ids_trace_info)))==NULL)
	    {
	     d27_010_set_error_apdu((D23_rserror *)err_apdu,D2_TEMP_ERR,
				  D2_SYS_ERR, D21_ALLOC_ERROR, apdu_len);
	     D2_ASN1_ERR_RETURN_WITH_TRACE
	     }
	for (i=0; d23_tinfo->d23_trtag == D23_TRTAG;
			 i++, tinfo++, d23_tinfo = (D23_trace_info *)scan)
	    {
	     tinfo->op_prog.name_res_ph = d23_tinfo->d23_op_prog.d23_opnm_rs;
	     tinfo->op_prog.next_rdn_defined = (d23_tinfo->d23_op_prog.d23_opnm_rs
					 == D2_PROCEEDING)? TRUE:FALSE;
	     tinfo->op_prog.next_rdn = d23_tinfo->d23_op_prog.d23_op_nxtrdn;
	     scan += D23_S_TRACE;
	     if (d2a121_dn_ids(&scan, &tinfo->dsa, err_apdu,apdu_len)
		      == D2_ERROR)
		{
		free (tr_info->list);
		D2_ASN1_ERR_RETURN_WITH_TRACE
		}
	     if (((D23_ds_name *)scan)->d23_dstag == D23_NAMTAG)
	      {
		if (d2a121_dn_ids(&scan, &tinfo->target, err_apdu,
					apdu_len)== D2_ERROR)
		  {
		  free (tr_info->list);
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		  }
	       }
	     else
		tinfo->target.nb_rdnames = IDS_UNDEFINED;

	     }
	tr_info->nb = i;
	*apdu = scan;
	return(D2_NOERROR);
  }

