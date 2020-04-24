/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d2aref.c,v $
 * Revision 1.1.10.2  1996/02/18  19:43:20  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:24:17  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:25:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:53:56  root]
 * 
 * Revision 1.1.8.3  1994/06/21  14:44:39  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:28:36  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:08:42  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:17:45  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  18:38:56  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:47:37  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  11:12:29  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  12:40:03  marrek]
 * 
 * Revision 1.1.4.5  1993/02/02  10:10:43  marrek
 * 	Fixes for OT 6572
 * 	[1993/02/01  17:53:10  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  18:13:44  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:11:28  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/04  18:03:13  marrek
 * 	November 1992 code drop
 * 	Replace cmx.h by cmx_addr.h
 * 	Replace T_NO_NSAP by T_MAXNSAP
 * 	[1992/12/04  10:45:05  marrek]
 * 
 * Revision 1.1.4.2  1992/11/27  16:04:07  marrek
 * 	November 1992 code drop
 * 	[1992/11/17  12:55:32  marrek]
 * 
 * Revision 1.1.2.3  1992/06/30  20:55:05  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  20:34:07  melman]
 * 
 * Revision 1.1.2.2  1992/06/01  20:09:37  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:04:00  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d2aref.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:43:20 $";
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
/* NAME         : d2aref.c                                            */
/*								      */
/* AUTHOR       : Praveen Gupta, Siemens India                        */
/* DATE         : 9. 1. 1990                                          */
/*								      */
/* DESCRIPTION  : This source files contains subroutiones to          */
/*                transform APDUs to IDS interface.                   */
/*								      */
/*datoff **************************************************************/

#include <gds.h>
#include <stdlib.h>
#include <string.h>

#include <d2asn1.h>
#include <d2asn1_int.h>

/******** LOCAL *********/

static signed32 d2a_copy_psap ( dua_psap * , dua_psap * );

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a_copy_psap ()                                        */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*           This function copies psap information.                   */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      in_psap     = PSAP address in DUA structure.                  */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      out_psap    = PSAP address in DUA structure.                  */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_ERROR        transformation fails                          */
/*      D2_NOERROR      transformation is successful                  */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: Praveen Gupta                                             */
/**********************************************************************/

static signed32 d2a_copy_psap (
  dua_psap *in_psap,
  dua_psap *out_psap )
{
const char function_name[] = "d2a_copy_psap";
  signed32 i;

    if (in_psap->psel.contents == NULL)
	out_psap->psel_defined = FALSE;
    else
     {
	out_psap->psel_defined = TRUE;
      if ((out_psap->psel.contents = D27_MALLOC (svc_c_sev_warning,in_psap->psel.nb)) == NULL)
       {   
	   D2_ASN1_ERR_RETURN_WITH_TRACE
	 }
      memcpy(out_psap->psel.contents, in_psap->psel.contents, in_psap->psel.nb);
      out_psap->psel.nb = in_psap->psel.nb;
     };

    if (in_psap->ssel.contents == NULL)
	out_psap->ssel_defined = FALSE;
    else
     {
	out_psap->ssel_defined = TRUE;
      if ((out_psap->ssel.contents = D27_MALLOC (svc_c_sev_warning,in_psap->ssel.nb)) == NULL)
       {   
	   D2_ASN1_ERR_RETURN_WITH_TRACE
	 }
      memcpy(out_psap->ssel.contents, in_psap->ssel.contents, in_psap->ssel.nb);
      out_psap->ssel.nb = in_psap->ssel.nb;
     };

    if (in_psap->tsel.contents == NULL)
	out_psap->tsel_defined = FALSE;
    else
     {
	out_psap->tsel_defined = TRUE;
      if ((out_psap->tsel.contents = D27_MALLOC (svc_c_sev_warning,in_psap->tsel.nb)) == NULL)
       {   
	   D2_ASN1_ERR_RETURN_WITH_TRACE
	 }
      memcpy(out_psap->tsel.contents, in_psap->tsel.contents, in_psap->tsel.nb);
      out_psap->tsel.nb = in_psap->tsel.nb;
     };

    out_psap->nb_nsap = in_psap->nb_nsap;
    if (in_psap->nb_nsap != 0)
     {
      if ((out_psap->nsap =
		D27_MALLOC (svc_c_sev_warning,in_psap->nb_nsap * sizeof(octet_string))) == NULL)
       {   
	   D2_ASN1_ERR_RETURN_WITH_TRACE
	 }
      for (i=0; i< in_psap->nb_nsap; i++)
      {
        if ((out_psap->nsap[i].contents =
				D27_MALLOC (svc_c_sev_warning,in_psap->nsap[i].nb)) == NULL)
          {   
	   D2_ASN1_ERR_RETURN_WITH_TRACE
	   }
         memcpy(out_psap->nsap[i].contents, in_psap->nsap[i].contents, in_psap->nsap[i].nb);
         out_psap->nsap[i].nb = in_psap->nsap[i].nb;
       };
      }
    return( D2_NOERROR);

}

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a039_put_referral()                                   */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*           This function transforms referral errors in APDU format  */
/*  to referral error in IDS format.                                  */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      apdu        = Error APDU with referral error(rf_info of APDU).*/
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      referral    = Referral error in IDS Format.                   */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_ERROR        transformation fails                          */
/*      D2_NOERROR      transformation is successful                  */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: Praveen Gupta                                             */
/**********************************************************************/


signed32 d2a039_put_referral(
  ids_cont_reference * referral,
  byte ** apdu,
  byte * err_apdu,
  signed32 * er_len )
{
const char function_name[] = "d2a039_put_referral";
  D23_rf_info * rf_info = (D23_rf_info *) *apdu;
  byte * scan = *apdu + D23_S_REF;
  signed32   i, len;
  D23_av_value * at_value;
  ids_acc_pt * acc_pts;

 /* copying operation progress */
    referral->op_prog.name_res_ph = rf_info->d23_op_pr.d23_opnm_rs;
    referral->op_prog.next_rdn_defined =
	 rf_info->d23_op_pr.d23_op_nxtrdn == FALSE ?
		FALSE :  TRUE;
    referral->op_prog.next_rdn =  rf_info->d23_op_pr.d23_op_nxtrdn;

    referral->alias_rdn_defined =
	 rf_info->d23_rf_alirdn == D2_NOT_DEFINED ?
		FALSE :  TRUE;
    referral->alias_rdn = rf_info->d23_rf_alirdn;

    referral->rdn_resolved_defined =
	 rf_info->d23_rf_rdnres == D2_NOT_DEFINED ?
		FALSE :  TRUE;
    referral->rdn_resolved = rf_info->d23_rf_rdnres;

    referral->ref_type_defined =
	 rf_info->d23_rf_type == D2_NOT_DEFINED ?
		FALSE :  TRUE;
    referral->ref_type = rf_info->d23_rf_type;

    referral->nb_acc_pt = 1 ;

    if (d2a121_dn_ids(&scan, &referral->target,err_apdu,
				er_len) == D2_ERROR)
	 {
	   D2_ASN1_ERR_RETURN_WITH_TRACE
	  }


    if ((referral->acc_pts =
		D27_MALLOC(svc_c_sev_warning,rf_info->d23_nm_acc * sizeof(ids_acc_pt))) == NULL)
      { 
	d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_TEMP_ERR,
			   D2_SYS_ERR, D21_ALLOC_ERROR,er_len);
	D2_ASN1_ERR_RETURN_WITH_TRACE
       }

    acc_pts = referral->acc_pts;
    for (i=0; i<rf_info->d23_nm_acc &&
	 ((D23_acc_info *)scan)->d23_acctag == D23_ACCTAG; i++, acc_pts++)
    {
      scan += D23_S_ACC;
      if (d2a121_dn_ids(&scan, &acc_pts->name,
				     err_apdu,er_len) == D2_ERROR)
	{ free (referral->acc_pts);
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	  }
      at_value = (D23_av_value *) scan;
      len = at_value->d23_avlen;
      scan += D23_S_ATV;
      if (d2a049_put_psap ( scan, (dua_psap *)&acc_pts->addr, err_apdu, &len)
					    == D2_ERROR)
	    { free (referral->acc_pts);
	      *er_len = len;
	      D2_ASN1_ERR_RETURN_WITH_TRACE
	     }
      }
    referral->nb_acc_pt = i;
    *apdu = scan + at_value->d23_avlen;
    return (D2_NOERROR);
}

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a049_put_psap ()                                      */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*           This function transforms errors in APDU format to        */
/*  Errors in IDS structure.                                          */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/* 	apdu 	   = APDU containing psap as attribute. 	      */
/* 	len         = length of PSAP address in APDU.                 */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      psap        = PSAP address in IDS structure.                  */
/*	err_apdu   = Error APDU returned in the case of error.        */
/* 	len        = Error length in the case of errro.               */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_ERROR        transformation fails                          */
/*      D2_NOERROR      transformation is successful                  */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: Praveen Gupta                                             */
/**********************************************************************/

signed32 d2a049_put_psap (
  byte * apdu,
  dua_psap *psap,
  byte * err_apdu,
  signed32 * len  )  /* used as input for apdu->d23_avlen and output for err_len */
{
const char function_name[] = "d2a049_put_psap";
  signed32 sel_syntax,i;
  dua_psap d_psap;
 
    sel_syntax = SEL_SETSYNTAX(SEL_BINARY, SEL_BINARY, SEL_BINARY);
    if ((d_psap.nsap = D27_MALLOC (svc_c_sev_warning,T_MAXNSAP *
					sizeof(octet_string))) == NULL)
       {   
	   D2_ASN1_ERR_RETURN_WITH_TRACE
	 }
    d_psap.psel.contents = d_psap.ssel.contents = d_psap.tsel.contents = (unsigned char *) NULL ;
      for (i=0; i< T_MAXNSAP; i++)
        d_psap.nsap[i].contents = (unsigned char *)  NULL;

    if(d2p_decode_psap_addr((Psap_info *)(&d_psap.psel),
	  (Psap_info *)(&d_psap.ssel),(Psap_info *)(&d_psap.tsel),
     &sel_syntax,(signed32 *)(&d_psap.nb_nsap), (Psap_info *)(d_psap.nsap),
							apdu) < 0)
       {   
	   D2_ASN1_ERR_RETURN_WITH_TRACE
	 }
	 
    if (d2a_copy_psap( &d_psap, psap) == D2_ERROR) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
    free(d_psap.nsap);
    return( D2_NOERROR);

}

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a053_put_ref()                                        */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*           This function puts referral error in IDS Structure to    */
/*  rf_info error in APDU format.                                     */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      ref_err     = Referral error in the IDS Structure.            */
/*      beg_apdu    = Beginning of APDU 			      */
/*      max_len     = length indicating total memory allocated for APDU*/
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      apdu        = Error APDU with rs_error.                       */
/*      apdu_len    = APDU length in the case of error in function.   */
/*	err_apdu    = Error APDU to be returned in case of error in   */
/* 			Function. 				      */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_ERROR        transformation fails                          */
/*      D2_NOERROR      transformation is successful                  */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: Praveen Gupta                                             */
/**********************************************************************/


signed32 d2a053_put_ref(
  ids_cont_reference * ref_err,
  byte ** apdu,
  byte ** beg_apdu,
  signed32 * max_len,
  byte * err_apdu,
  signed32 * apdu_len )
{
const char function_name[] = "d2a053_put_ref";
  byte        * scan = *apdu;
  D23_rf_info  * rf_info;
  D23_acc_info * acc_info;
  ids_acc_pt   * acc_pt;
  signed32            i;

  if (d2a033_check_memsize (sizeof(D23_rf_info), beg_apdu, &scan,
	      max_len, (D23_rserror *)err_apdu, apdu_len) != D2_NOERROR)
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
  rf_info = (D23_rf_info *) scan;
  rf_info->d23_rftag = D23_REFTAG;

  rf_info->d23_rf_alirdn = (ref_err->alias_rdn_defined == TRUE) ?
			    ref_err->alias_rdn : FALSE;
  rf_info->d23_rf_rdnres = (ref_err->rdn_resolved_defined == TRUE) ?
			    ref_err->rdn_resolved : FALSE;
  rf_info->d23_rf_type = (ref_err->ref_type_defined == TRUE) ?
			    ref_err->ref_type : FALSE;
 /* copying operation progress */
  rf_info->d23_op_pr.d23_opnm_rs = ref_err->op_prog.name_res_ph;
  rf_info->d23_op_pr.d23_op_nxtrdn =
     (ref_err->op_prog.next_rdn_defined == TRUE) ?
				   ref_err->op_prog.next_rdn : FALSE;
  rf_info->d23_nm_acc = ref_err->nb_acc_pt;
  scan += D23_S_REF;

  /* put target object */
  if (d2a122_dn_apdu(D23_NAMTAG, &scan, &ref_err->target,
		   apdu, max_len, err_apdu, apdu_len) == D2_ERROR)
       	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}

  for (i=0, acc_pt = ref_err->acc_pts; i<ref_err->nb_acc_pt; acc_pt++, i++)
    {
    if (d2a033_check_memsize (sizeof(D23_acc_info), beg_apdu, &scan,
	      max_len, (D23_rserror *)err_apdu, apdu_len) != D2_NOERROR)
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}

    acc_info = (D23_acc_info *) scan;
    acc_info->d23_acctag = D23_ACCTAG;
    scan += D23_S_ACC;

    /* DSA-name APDU */
    if (d2a122_dn_apdu(D23_NAMTAG, &scan, &acc_pt->name,
		   apdu, max_len, err_apdu, apdu_len) == D2_ERROR)
       	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}

    /* DSA-ADDRESS APDU */
    if (d2a061_get_psap( (dua_psap *)&(acc_pt->addr), TRUE, &scan,
			apdu, max_len, err_apdu, apdu_len) == D2_ERROR)
	  	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
   }  /* of for loop */
*apdu = scan;
return (D2_NOERROR);

 }


/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a061_get_psap()                                       */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*           This function transforms psap from IDS structure to an   */
/*  at_info. or av_info of APDU format.                               */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      psap        = psap address in the IDS Structure.              */
/*        s_apdu    = Beginning of APDU 			      */
/*      bool        = Flag indicating :                               */
/*			FALSE : psap to be added in APDU as at_info   */
/*			TRUE  : psap to be added in APDU as av_info   */
/*      max_len     = length indicating total memory allocated for APDU*/
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      apdu        = Error APDU with rs_error.                       */
/*      apdu_len    = APDU length in the case of error in function.   */
/*	err_apdu    = Error APDU to be returned in case of error in   */
/* 			Function. 				      */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      err_buf     = error structure in IDS Format.                  */
/*      error       = contain the type of error which is in IDS str.  */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_ERROR        transformation fails                          */
/*      D2_NOERROR      transformation is successful                  */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: Praveen Gupta                                             */
/**********************************************************************/


signed32 d2a061_get_psap(
  dua_psap * psap,
  boolean   bool,
  byte ** apdu,
  byte ** s_apdu,
  signed32 * max_len,
  byte * err_apdu,
  signed32 * err_len )
{
const char function_name[] = "d2a061_get_psap";
 signed32 sel_syntax,len;
 D23_at_type  * at_type;
 D23_av_info  * av_info;
 D23_av_value * av_value;
 byte * scan;
 signed32 memreq;


  sel_syntax = SEL_SETSYNTAX(SEL_BINARY, SEL_BINARY, SEL_BINARY);
  if (bool == TRUE)
  { 
    memreq = D23_S_ATV + D2_ALIGN(T_PSAPSIZE);
    if (d2a033_check_memsize(memreq, s_apdu, apdu, max_len, 
                    (D23_rserror *)err_apdu, err_len) == D2_ERROR)
		{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
    scan = *apdu;
    av_value = (D23_av_value *) scan;
    scan += D23_S_ATV;
    if ((len = d2p_encode_psap_addr(
(psap->psel_defined == TRUE)?(Psap_info *)(&psap->psel):(Psap_info *) NULL,
(psap->ssel_defined == TRUE)?(Psap_info *)(&psap->ssel):(Psap_info *) NULL,
(psap->tsel_defined == TRUE)?(Psap_info *)(&psap->tsel):(Psap_info *) NULL,
    sel_syntax,psap->nb_nsap, (Psap_info *)(psap->nsap), scan)) < 0)
       {   
	   D2_ASN1_ERR_RETURN_WITH_TRACE
	 }
    av_value->d23_avlen = len;
    av_value->d23_avoff = D2_ALIGN(av_value->d23_avlen);
    scan += av_value->d23_avoff;
   } 
  else
   {
      memreq = D23_S_ATT +  sizeof(D2_PSAP_ADDRESS);
      if (d2a033_check_memsize(memreq, s_apdu, apdu, max_len,
		      (D23_rserror *)err_apdu, err_len) == D2_ERROR)
	  	{
		D2_ASN1_ERR_RETURN_WITH_TRACE
		}
      scan = *apdu;
      at_type = (D23_at_type *) scan;
      at_type->d23_atttag = D23_ATTTAG;
      at_type->d23_atlen  = sizeof(D2_PSAP_ADDRESS);
      at_type->d23_atoff  = D2_ALIGN(sizeof(D2_PSAP_ADDRESS));
      scan += D23_S_ATT;
      memcpy(scan, D2_PSAP_ADDRESS, sizeof(D2_PSAP_ADDRESS));
      av_info = (D23_av_info *) scan;
      av_info->d23_avtag = D23_AVTAG;
      av_info->d23_avnum = 1; /* PSAP ADDRESS can not be recurring */
      av_info->d23_asyntx = D2_PSAP_STX;
      scan += D23_S_AV;
      if (d2a061_get_psap( psap, TRUE, &scan,
		   apdu, max_len, err_apdu, err_len) == D2_ERROR)
	 return(D2_NOERROR);
   }
 
 *apdu = scan;
 return(D2_NOERROR);
 }

