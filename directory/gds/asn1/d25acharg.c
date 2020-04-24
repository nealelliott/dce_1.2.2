/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d25acharg.c,v $
 * Revision 1.1.10.2  1996/02/18  19:42:55  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:23:47  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:24:03  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:53:36  root]
 * 
 * Revision 1.1.8.3  1994/06/21  14:44:21  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:27:02  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:08:22  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:15:18  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:51:04  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:42:15  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  10:51:16  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  12:34:56  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:12:52  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:10:16  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  15:57:45  marrek
 * 	November 1992 code drop
 * 	[1992/11/17  12:50:41  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:06:07  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:02:23  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d25acharg.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:42:55 $";
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
/* NAME         : d25acharg.c                                         */
/*								      */
/* AUTHOR       : Praveen Gupta, Siemens India, Delhi                 */
/* DATE         : 26. 2. 1990                                         */
/*								      */
/* DESCRIPTION  : This source files contains subroutiones to          */
/*                transform IDS-ChainingArgs to APDU interface.       */
/*								      */
/* SYSTEM DEPENDENCIES:  COMMON                                       */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   0.1    | 90-02-26| Original                       | WS |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

#include <gds.h>
#include <stdlib.h>
#include <time.h>

#include <d2asn1.h>
#include <d2asn1_int.h>

/******** LOCAL *********/

static signed32 d2a163_apdu_cr_ref_ids( byte ** , signed32 * , signed32  * ,
					ids_cross_ref * , byte * );

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a160_ids_charg_apdu()                                 */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*           This function transforms ChainingArg's from IDS format to*/
/*  the ChainingArgs of the APDU interface.                           */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      ch_arg  = chaining arguments in IDS_interface.                */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      apdu        = APDU for the Chaining arguments.                */
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

signed32 d2a160_ids_charg_apdu(
  byte ** apdu,
  byte ** beg_apdu,
  signed32 * apdu_len,
  ids_ch_arg * ch_arg,
  byte * err_apdu )
{
const char function_name[] = "d2a160_ids_charg_apdu";
   byte * scan = *apdu;
   signed32 max_length = *apdu_len;
   D23_chain_arg * d23_charg = (D23_chain_arg *) scan;
   struct timeb tmb;

    d23_charg->d23_chtag =D23_CHTAG;

    d23_charg->d23_al_deref =(ch_arg->alias_deref == FALSE)?FALSE:TRUE;
    d23_charg->d23_entry_only =(ch_arg->entry_only == FALSE)?FALSE:TRUE;
    d23_charg->d23_ret_cref =(ch_arg->rt_cross_refs==FALSE)?FALSE:TRUE;
    d23_charg->d23_al_RDN = ch_arg->aliased_rdn;
    d23_charg->d23_ref_type = ch_arg->ref_type;
    d23_charg->d23_chtmlt = D2_T_UNLIMITED;
    if (ch_arg->time_limit.nb != IDS_UNDEFINED)
     {
	asn1_time((char *)ch_arg->time_limit.contents,
			   ch_arg->time_limit.nb, &tmb);
	d23_charg->d23_chtmlt = tmb.time - time(0L);
      }
    d23_charg->d23_op_prog.d23_opnm_rs = ch_arg->op_prog.name_res_ph;
    d23_charg->d23_op_prog.d23_op_nxtrdn = 
                  (ch_arg->op_prog.next_rdn_defined == FALSE)
			       ? FALSE : ch_arg->op_prog.next_rdn;
    d23_charg->d23_max_tr = ch_arg->trace_infos.nb;
    scan += D23_S_CHARG;
    if (d2a162_tr_info_apdu(&scan, &ch_arg->trace_infos, beg_apdu, &max_length,
			     err_apdu, apdu_len) == D2_ERROR)
	{
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	}
    if (ch_arg->originator.nb_rdnames != IDS_UNDEFINED)
     {
      if (d2a122_dn_apdu(D23_RQTAG, &scan, &ch_arg->originator,
	       beg_apdu, &max_length, err_apdu, apdu_len) == D2_ERROR)
	{
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	}
     }

    if (ch_arg->target.nb_rdnames != IDS_UNDEFINED)
     {
       if (d2a122_dn_apdu(D23_BONTAG, &scan, &ch_arg->target,
	       beg_apdu, &max_length, err_apdu, apdu_len) == D2_ERROR)
	{
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	}
     }
/* domain info. and security parameters are not implemented */

    *apdu = scan;
    return(D2_NOERROR);

  }

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a161_apdu_chres_ids()                                 */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*           This function transforms ChainingRes's from IDS format to*/
/*  the ChainingRes of the APDU interface.                           */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      apdu        = APDU for the Chaining results.                  */
/*      apdu_len     = length of error APDU                           */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      ch_res  = chaining results   in IDS_interface.                */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_ERROR        transformation fails                          */
/*                      (err_apdu and err_len will be returned)       */
/*      D2_NOERROR      transformation is successful                  */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: Praveen Gupta                                             */
/**********************************************************************/

signed32 d2a161_apdu_chres_ids(
  byte ** apdu,
  signed32 * apdu_len,
  ids_ch_res * ch_res,
  byte * err_apdu )
{
const char function_name[] = "d2a161_apdu_chres_ids";
  signed32 nb_cref;
  byte * scan = *apdu + D23_S_CHRES;
  D23_chain_res * d23_cres = (D23_chain_res *) scan;

    nb_cref = d23_cres->d23_max_cref;
    if (nb_cref != 0)
    {
     if ((ch_res->cross_refs = D27_MALLOC(svc_c_sev_warning,nb_cref*sizeof(ids_cross_ref)))==NULL)
	{
	 d27_010_set_error_apdu((D23_rserror *)err_apdu,D2_TEMP_ERR,
			     D2_SYS_ERR, D21_ALLOC_ERROR, apdu_len);
	 D2_ASN1_ERR_RETURN_WITH_TRACE
	 }
     if (d2a163_apdu_cr_ref_ids(&scan,apdu_len,&nb_cref,
			ch_res->cross_refs,err_apdu) == D2_ERROR)
	{ free(ch_res->cross_refs);
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	 }
     ch_res->nb_cross_ref = nb_cref;
    }
    else
     ch_res->nb_cross_ref = IDS_UNDEFINED;

  /* Domain info. is not supported  */
    ch_res->domain_info.l = IDS_UNDEFINED;

   *apdu = scan;
   return (D2_NOERROR);
  }


/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a162_tr_info_apdu()                                   */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*           This function transforms traceInfo from IDS format to    */
/*  the traceInformation of the APDU interface.                       */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      tr_info  = Trace Information in IDS_interface.                */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      beg_apdu    = starting of APDU 				      */
/*      max_len     = total length of APDU 			      */
/*      apdu        = APDU for the Trace Information.                 */
/*      err_apdu     = ERROR APDU in the case of error                */
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

signed32 d2a162_tr_info_apdu(
  byte ** apdu,
  ids_trace_infos * tr_infos,
  byte ** beg_apdu,
  signed32 * max_len,
  byte * err_apdu,
  signed32 * er_len )
{
const char function_name[] = "d2a162_tr_info_apdu";
  byte * scan = *apdu;
  D23_trace_info * ap_trinfo;
  ids_trace_info * tr_info = tr_infos->list;
  signed32 i;

    for (i=0; i < tr_infos->nb; i++, tr_info++ )
     {
	ap_trinfo = (D23_trace_info *)scan;
	ap_trinfo->d23_trtag = D23_TRTAG;
	scan += D23_S_TRACE;
	ap_trinfo->d23_op_prog.d23_opnm_rs = tr_info->op_prog.name_res_ph;
	ap_trinfo->d23_op_prog.d23_op_nxtrdn =
	      (tr_info->op_prog.next_rdn_defined == FALSE)
				 ? FALSE : tr_info->op_prog.next_rdn;

	if (d2a122_dn_apdu(D23_NAMTAG, &scan, &tr_info->dsa,
		   beg_apdu, max_len, err_apdu, er_len) == D2_ERROR)
	   {
	     D2_ASN1_ERR_RETURN_WITH_TRACE
	   }

	if (tr_info->target.nb_rdnames != IDS_UNDEFINED)
         {
	   if (d2a122_dn_apdu(D23_NAMTAG, &scan, &tr_info->target,
		   beg_apdu, max_len, err_apdu, er_len) == D2_ERROR)
	   {
	     D2_ASN1_ERR_RETURN_WITH_TRACE
	   }
	 }

      } /* of for */

   *apdu = scan;
   return(D2_NOERROR);

 }

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a163_apdu_cr_ref_ids()                                */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*           This function transforms Cross Refs from APDU format to  */
/*  the Cross refs of the ids interface.                              */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      apdu        = APDU for the Cross refs.                        */
/*      apdu_len    = APDU length for the Cross refs.                 */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      cr_ref  = Cross Reference in IDS_interface.                   */
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


static signed32 d2a163_apdu_cr_ref_ids(
  byte ** apdu,
  signed32 * apdu_len,
  signed32  * nb_cref,
  ids_cross_ref * cr_ref,
  byte * err_apdu )
{
const char function_name[] = "d2a163_apdu_cr_ref_ids";
  signed32 i, len;
  D23_cross_ref * d23_cref = (D23_cross_ref *) *apdu;
  byte         * scan = *apdu;
  D23_av_value * at_value;

   for (i=0; d23_cref->d23_creftag == D23_CREFTAG;
		    i++, cr_ref++, d23_cref = (D23_cross_ref *)scan)
       {
	scan += D23_S_CREF;
	if (d2a121_dn_ids(&scan,&cr_ref->ctx_prefix,err_apdu,apdu_len)
		 == D2_ERROR)
	  {
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	  }
	scan += D23_S_ACC;
	if (d2a121_dn_ids(&scan, &cr_ref->acc_pt.name,
				err_apdu, apdu_len) == D2_ERROR)
	  {
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	  }
	at_value = (D23_av_value *) scan;
	len = at_value->d23_avlen;
	scan += D23_S_ATV;
	if (d2a049_put_psap ( scan,(dua_psap *)&cr_ref->acc_pt.addr,
							err_apdu, &len)
					    == D2_ERROR)
	   { *apdu_len = len;
	     D2_ASN1_ERR_RETURN_WITH_TRACE
	     }
	scan += len;
	}
   *nb_cref = i;
   *apdu = scan;
   return(D2_NOERROR);
  }

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a164_apdu_dsa_ref_ids()                               */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*           This function transforms dsa referral from APDU format to*/
/*  the dsa referral of the ids interface.                            */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      apdu        = APDU for the dsa referral.                      */
/*      apdu_len    = APDU length for the dsa referral.               */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      dsa_ref = dsa referral in IDS_interface.                      */
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


signed32 d2a164_apdu_dsa_ref_ids(
  byte * apdu,
  signed32 * apdu_len,
  ids_dsa_referral * dsa_ref,
  byte * err_apdu)
{
const char function_name[] = "d2a164_apdu_dsa_ref_ids";
   byte * scan = apdu + D23_S_DREF;

    if (d2a039_put_referral(&dsa_ref->candidate, &scan, err_apdu, apdu_len)
	      == D2_ERROR)
	  {
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	  }

    if (((D23_ds_name *)scan)->d23_dstag == D23_NAMTAG)
     {
       if (d2a121_dn_ids(&scan, &dsa_ref->ctx_pref, err_apdu,apdu_len)
								== D2_ERROR)
	  {
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	  }
      }
    else
       dsa_ref->ctx_pref.nb_rdnames = IDS_UNDEFINED;

    if (((D23_drf_info *)apdu)->d23_drf_max_tr > 0)
      {
      if (d2a152_tr_info_ids(&scan, apdu_len,((D23_drf_info *)apdu)->d23_drf_max_tr,
				&dsa_ref->trace_infos, err_apdu) == D2_ERROR)
	  {
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	  }
      dsa_ref->trace_infos_defined = TRUE;
      }
    else
      {
      dsa_ref->trace_infos_defined = FALSE;
      }

    return(D2_NOERROR);
  }

