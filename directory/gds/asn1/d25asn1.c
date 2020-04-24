/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d25asn1.c,v $
 * Revision 1.1.10.2  1996/02/18  19:43:08  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:23:59  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:24:39  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:53:45  root]
 * 
 * Revision 1.1.8.5  1994/07/06  15:06:40  marrek
 * 	July 1994 code drop.
 * 	[1994/07/06  10:00:38  marrek]
 * 
 * Revision 1.1.8.4  1994/06/21  14:44:29  marrek
 * 	June 1994 code submission.
 * 	[1994/06/21  09:26:20  marrek]
 * 
 * Revision 1.1.8.3  1994/05/10  15:50:35  marrek
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/02  13:24:22  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:15:37  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:08:32  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:16:50  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  18:37:19  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:45:51  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  11:01:58  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  12:37:15  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:13:18  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:10:54  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  16:00:34  marrek
 * 	November 1992 code drop
 * 	[1992/11/17  12:52:54  marrek]
 * 
 * Revision 1.1.2.3  1992/07/01  16:52:08  melman
 * 	GDS drop of 6/26/92
 * 	[1992/07/01  16:20:40  melman]
 * 
 * Revision 1.1.2.2  1992/06/01  20:08:48  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:03:21  melman]
 * 
 * $EndLog$
 */
#if !defined lint && defined GDS_RCS_ID
static char rcsid[] = "@(#)$RCSfile: d25asn1.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:43:08 $";
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
/* NAME         : d25asn1.c                                           */
/*								      */
/* AUTHOR       : Praveen Gupta, SIEMENS INDIA.                       */
/* DATE         : 9. 1. 1990                                          */
/*								      */
/* DESCRIPTION  : This source-File which contains interface for       */
/*		  converting apdus to ASN1 encoded formats.           */
/*								      */
/* SYSTEM DEPENDENCIES:  COMMON                                       */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   0.1    | 90-01-09| Original                       | WS |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

#include <gds.h>
#include <string.h>
#include <stdlib.h>

#include <d2asn1.h>
#include <d2asn1_int.h>

/**********************************************************************/
/*                                                                    */
/*       D E C L A R A T I O N of data				      */
/*                                                                    */
/**********************************************************************/

static char  null_res[]   = { (char)0x05, (char)0x00};

/******** LOCAL *********/

static void i_error_free ( signed32 , ids_error * );

static signed32 d2a021_apdu_bind_res_asn (byte *,signed32,asn1_field *);
static signed32 d2a023_apdu_read_res_asn (byte *,signed32,asn1_field *);
static signed32 d2a088_apdu_list_res_asn (byte *,signed32,asn1_field *);
static signed32 d2a090_apdu_search_res_asn (byte *,signed32,asn1_field *);
static signed32 d2a092_apdu_comp_res_asn (byte *,signed32,asn1_field *);

static signed32 d2a040_asn_bind_arg_apdu (byte **,signed32 *,asn1_field *);
static signed32 d2a042_asn_read_arg_apdu (byte **,byte **,signed32 *,
							asn1_field *,byte *);
static signed32 d2a066_asn_rem_arg_apdu (byte **,byte **,signed32 *,
							asn1_field *,byte *);
static signed32 d2a074_asn_add_arg_apdu (byte **,byte **,signed32 *,
							asn1_field *,byte *);
static signed32 d2a093_asn_mod_arg_apdu (byte **,byte **,signed32 *,
							asn1_field *,byte *);
static signed32 d2a094_asn_list_arg_apdu (byte **,byte **,signed32 *,
							asn1_field *,byte *);
static signed32 d2a096_asn_srch_arg_apdu (byte **,byte **,signed32 *,
							asn1_field *,byte *);
static signed32 d2a098_asn_comp_arg_apdu (byte **,byte **,signed32 *,
							asn1_field *,byte *);
static signed32 d2a129_asn_modrdn_apdu (byte **,byte **,signed32 *,
							asn1_field *,byte *);


/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d2a000_apdu_asn1()                                 */
/*                                                                    */
/* AUTHOR       :  Praveen Gupta, SIEMENS INDIA.                      */
/* DATE         :  09.01.90                                           */
/*                                                                    */
/* DESCRIPTION  : This source-File which contains interface for       */
/*		  converting apdus to ASN1 encoded formats.           */
/*                                                                    */
/* INPUT-PARAMETER :  op_code: for defining apdu_type		      */
/*                    apdu: pointer to the apdu                       */
/*                    apdu_len: length of the apdu                    */
/*                    abs_id:  abstract id of protocol                */
/*                                                                    */
/* OUTPUT-PARAMETER : asn1_cod: for returning the ASN1 encoded string */
/*                                                                    */
/* RETURN-VALUE :  D2_NOERROR                                         */
/*                 D2_ERROR                                           */
/*                                                                    */
/* exoff **************************************************************/

signed32 d2a000_apdu_asn1(
  signed16 op_code,
  signed16 abs_id,
  byte * apdu,
  signed32 apdu_len,
  asn1_field * asn1_cod,
  byte *err_apdu )
{ 
const char function_name[] = "d2a000_apdu_asn1";
  ids_ch_res i_ch_res;
  ids_ch_arg i_ch_arg;
  byte * scan = apdu;
  signed32 res = D2_NOERROR, ch_arg_len, ch_res_len;
  Bool    chres = FALSE;
  asn1_field asn1_any;

   D2_ASN1_TRACE_ENTER_ENCODING(op_code,abs_id)

   if ( /* NULL RESULT */
	( (op_code == (D23_ADDOBJT + D2_RES_OPID)) ||
	  (op_code == (D23_ADDOBJT + D2_RES_OPID + D2_ADM_OPID)) ||
	  (op_code == (D23_REMOBJT + D2_RES_OPID)) ||
	  (op_code == (D23_REMOBJT + D2_RES_OPID + D2_ADM_OPID)) ||
	  (op_code == (D23_MODENTR + D2_RES_OPID)) ||
	  (op_code == (D23_MODENTR + D2_RES_OPID + D2_ADM_OPID)) ||
	  (op_code == (D23_REPLRDN + D2_RES_OPID)) ||
	  (op_code == (D23_REPLRDN + D2_RES_OPID + D2_ADM_OPID)) ||
	  (op_code == (D23_ABANDON + D2_RES_OPID)) ||
	  (op_code == (D23_ABANDON + D2_RES_OPID + D2_ADM_OPID))
	)
			&& abs_id == D27_DSP_AS
      )
      {
	i_ch_res.domain_info.l = IDS_UNDEFINED;
	i_ch_res.nb_cross_ref = IDS_UNDEFINED;
	chres = TRUE;
       }

   if ( ( /* NULL RESULT */
	    (op_code == (D23_ADDOBJT + D2_RES_OPID)) ||
	    (op_code == (D23_ADDOBJT + D2_RES_OPID + D2_ADM_OPID)) ||
	    (op_code == (D23_REMOBJT + D2_RES_OPID)) ||
	    (op_code == (D23_REMOBJT + D2_RES_OPID + D2_ADM_OPID)) ||
	    (op_code == (D23_MODENTR + D2_RES_OPID)) ||
	    (op_code == (D23_MODENTR + D2_RES_OPID + D2_ADM_OPID)) ||
	    (op_code == (D23_REPLRDN + D2_RES_OPID)) ||
	    (op_code == (D23_REPLRDN + D2_RES_OPID + D2_ADM_OPID)) ||
	    (op_code == (D23_ABANDON + D2_RES_OPID)) ||
	    (op_code == (D23_ABANDON + D2_RES_OPID + D2_ADM_OPID))
	  )
			&& abs_id == D27_DAP_AS
      )
	{
	  memcpy(apdu,null_res, 2);
	  asn1_cod->l = 2;
	  asn1_cod->v = apdu;
	  return(D2_NOERROR);
	}

    if (op_code == (D23_UNBIND + D2_RES_OPID))
	{
	  asn1_cod->l = 0;
	  asn1_cod->v = apdu;
	  return(D2_NOERROR);
	}
    if (op_code == D23_UNBIND)
	{
	  asn1_cod->l = 0;
	  asn1_cod->v = apdu;
	  return(D2_NOERROR);
	}

  switch (op_code) {
	case (D23_BIND + D2_RES_OPID) :
		    res = d2a021_apdu_bind_res_asn(scan,apdu_len,asn1_cod);
		    break;

	case D23_BIND :
		    res = d2a020_apdu_bind_arg_asn(scan,&apdu_len,
							asn1_cod,err_apdu);
		    break;

	case D23_READ :
	case (D23_READ + D2_ADM_OPID) :
		 if (abs_id  == D27_DSP_AS)
		    if (d2a150_apdu_charg_ids(&scan,&apdu_len,&i_ch_arg,
						    err_apdu) == D2_ERROR)
			 {
			 D2_ASN1_ERR_RETURN_WITH_TRACE
			 }
		 res = d2a022_apdu_read_arg_asn(scan,&apdu_len,asn1_cod,err_apdu);
		 break;

	case D23_REMOBJT :
	case (D23_REMOBJT + D2_ADM_OPID) :
		 if (abs_id  == D27_DSP_AS)
		    if (d2a150_apdu_charg_ids(&scan,&apdu_len,&i_ch_arg,
						    err_apdu) == D2_ERROR)
			 {
			 D2_ASN1_ERR_RETURN_WITH_TRACE
			 }
		 res = d2a065_apdu_rem_arg_asn(scan,&apdu_len,asn1_cod,err_apdu);
		 break;

	case D23_ADDOBJT :
	case (D23_ADDOBJT + D2_ADM_OPID) :
		 if (abs_id  == D27_DSP_AS)
		    if (d2a150_apdu_charg_ids(&scan,&apdu_len,&i_ch_arg,
						    err_apdu) == D2_ERROR)
			 {
			 D2_ASN1_ERR_RETURN_WITH_TRACE
			 }
		 res = d2a073_apdu_add_arg_asn(scan,&apdu_len,asn1_cod,err_apdu);
		 break;

	case D23_MODENTR :
	case (D23_MODENTR + D2_ADM_OPID) :
		 if (abs_id  == D27_DSP_AS)
		    if (d2a150_apdu_charg_ids(&scan,&apdu_len,&i_ch_arg,
						    err_apdu) == D2_ERROR)
			 {
			 D2_ASN1_ERR_RETURN_WITH_TRACE
			 }
		 res = d2a086_apdu_mod_arg_asn(scan,&apdu_len,asn1_cod,err_apdu);
		 break;

	case D23_LIST :
	case (D23_LIST + D2_ADM_OPID) :
		 if (abs_id  == D27_DSP_AS)
		    if (d2a150_apdu_charg_ids(&scan,&apdu_len,&i_ch_arg,
						    err_apdu) == D2_ERROR)
			 {
			 D2_ASN1_ERR_RETURN_WITH_TRACE
			 }
		 res = d2a087_apdu_list_arg_asn(scan,&apdu_len,asn1_cod,err_apdu);
		 break;

	case D23_SEARCH :
	case (D23_SEARCH + D2_ADM_OPID) :
		 if (abs_id  == D27_DSP_AS)
		    if (d2a150_apdu_charg_ids(&scan,&apdu_len,&i_ch_arg,
						    err_apdu) == D2_ERROR)
			 {
			 D2_ASN1_ERR_RETURN_WITH_TRACE
			 }
		 res = d2a089_apdu_search_arg_asn(scan,&apdu_len,asn1_cod,err_apdu);
		 break;

	case D23_COMPARE :
	case (D23_COMPARE + D2_ADM_OPID) :
		 if (abs_id  == D27_DSP_AS)
		    if (d2a150_apdu_charg_ids(&scan,&apdu_len,&i_ch_arg,
						    err_apdu) == D2_ERROR)
			 {
			 D2_ASN1_ERR_RETURN_WITH_TRACE
			 }
		 res = d2a091_apdu_comp_arg_asn(scan,&apdu_len,asn1_cod,err_apdu);
		 break;
	case D23_REPLRDN :
	case (D23_REPLRDN + D2_ADM_OPID) :
		 if (abs_id  == D27_DSP_AS)
		    if (d2a150_apdu_charg_ids(&scan,&apdu_len,&i_ch_arg,
						    err_apdu) == D2_ERROR)
			 {
			 D2_ASN1_ERR_RETURN_WITH_TRACE
			 }
		 res = d2a128_apdu_modrdn_asn(scan,&apdu_len,asn1_cod,err_apdu);
		 break;
	case (D23_READ + D2_RES_OPID) :
	case (D23_READ + D2_RES_OPID + D2_ADM_OPID) :
		if (abs_id  == D27_DSP_AS)
		  {
		    chres = TRUE;
		    if (d2a161_apdu_chres_ids(&scan,&apdu_len,&i_ch_res,err_apdu)
							 ==D2_ERROR)
			 {
			 D2_ASN1_ERR_RETURN_WITH_TRACE
			 }
		   }
		res = d2a023_apdu_read_res_asn(scan,apdu_len,asn1_cod);
		break;

	case (D23_LIST + D2_RES_OPID) :
	case (D23_LIST + D2_RES_OPID + D2_ADM_OPID) :
		if (abs_id  == D27_DSP_AS)
		  {
		    chres = TRUE;
		    if (d2a161_apdu_chres_ids(&scan,&apdu_len,&i_ch_res,err_apdu)
							 ==D2_ERROR)
			 {
			 D2_ASN1_ERR_RETURN_WITH_TRACE
			 }
		   }
		res = d2a088_apdu_list_res_asn(scan,apdu_len,asn1_cod);
		break;

	case (D23_SEARCH + D2_RES_OPID) :
	case (D23_SEARCH + D2_RES_OPID + D2_ADM_OPID) :
		if (abs_id  == D27_DSP_AS)
		  {
		    chres = TRUE;
		    if (d2a161_apdu_chres_ids(&scan,&apdu_len,&i_ch_res,err_apdu)
							 ==D2_ERROR)
			 {
			 D2_ASN1_ERR_RETURN_WITH_TRACE
			 }
		   }
		res = d2a090_apdu_search_res_asn(scan,apdu_len,asn1_cod);
		break;

	case (D23_COMPARE + D2_RES_OPID) :
	case (D23_COMPARE + D2_RES_OPID + D2_ADM_OPID) :
		if (abs_id  == D27_DSP_AS)
		  {
		    chres = TRUE;
		    if (d2a161_apdu_chres_ids(&scan,&apdu_len,&i_ch_res,err_apdu)
							 ==D2_ERROR)
			 {
			 D2_ASN1_ERR_RETURN_WITH_TRACE
			 }
		   }
		res = d2a092_apdu_comp_res_asn(scan,apdu_len,asn1_cod);
		break;

	case (D23_BIND + D2_REJ_OPID) :
		res = d2a026_apdu_bind_error_asn(scan,apdu_len,asn1_cod);
		break;

	case D23_ABANDON :
		res = d2a999_apdu_abandon_asn(scan,&apdu_len,asn1_cod,err_apdu);
		break;

	default:
		break;
	}

   if (res == D2_ERROR) return(res);

   if ((abs_id == D27_DSP_AS) 
       && (op_code != (D23_BIND + D2_RES_OPID)) && (op_code != D23_BIND)
       && (chres == FALSE) && (op_code != (D23_BIND + D2_REJ_OPID)))
    {
	asn1_any.v = asn1_cod->v; asn1_any.l = asn1_cod->l;
	ch_arg_len = ids_ch_arg_len (-1,&i_ch_arg, asn1_any);
	if ((asn1_cod->v= D27_MALLOC (svc_c_sev_warning,ch_arg_len)) == NULL)
	    {
	     D2_ASN1_ERR_RETURN_WITH_TRACE
	     }
	asn1_cod->l= ids_ch_arg_cod( asn1_cod->v,-1,0,
				&i_ch_arg, asn1_any) - asn1_cod->v;
	i_ch_arg_free(&i_ch_arg, asn1_any);
     }

   if ((abs_id == D27_DSP_AS) && (chres == TRUE))
    {
     if ( /* NOT NULL RESULT */
	  (op_code != (D23_ADDOBJT + D2_RES_OPID)) &&
	  (op_code != (D23_ADDOBJT + D2_RES_OPID + D2_ADM_OPID)) &&
	  (op_code != (D23_REMOBJT + D2_RES_OPID)) &&
	  (op_code != (D23_REMOBJT + D2_RES_OPID + D2_ADM_OPID)) &&
	  (op_code != (D23_MODENTR + D2_RES_OPID)) &&
	  (op_code != (D23_MODENTR + D2_RES_OPID + D2_ADM_OPID)) &&
	  (op_code != (D23_REPLRDN + D2_RES_OPID)) &&
	  (op_code != (D23_REPLRDN + D2_RES_OPID + D2_ADM_OPID)) &&
	  (op_code != (D23_ABANDON + D2_RES_OPID)) &&
	  (op_code != (D23_ABANDON + D2_RES_OPID + D2_ADM_OPID))
        )
      {
	asn1_any.v = asn1_cod->v; asn1_any.l = asn1_cod->l;
	ch_res_len = ids_ch_res_len (-1,&i_ch_res, asn1_any);
	if ((asn1_cod->v= D27_MALLOC (svc_c_sev_warning,ch_res_len)) == NULL)
		{
		 D2_ASN1_ERR_RETURN_WITH_TRACE
		 }
	asn1_cod->l= ids_ch_res_cod( asn1_cod->v,-1,0,
				&i_ch_res,asn1_any) - asn1_cod->v;
	i_ch_res_free(&i_ch_res, asn1_any);
      }
     else
      {
	ch_res_len = ids_ch_null_res_len (-1,&i_ch_res);
	if ((asn1_cod->v= D27_MALLOC (svc_c_sev_warning,ch_res_len)) == NULL)
	   {
	    D2_ASN1_ERR_RETURN_WITH_TRACE
	    }
	asn1_cod->l= ids_ch_null_res_cod( asn1_cod->v,-1,0,
					 &i_ch_res) - asn1_cod->v;
	i_ch_null_res_free(&i_ch_res);
       }
     }

   if  (res == D2_NOERROR)
	{
	D2_ASN1_TRACE_ENCODED_MESSAGE(asn1_cod)
	}

   D2_ASN1_TRACE_EXIT_ENCODING(op_code,abs_id,res)

   return (res);
}

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* AUTHOR       :  Praveen Gupta, SIEMENS INDIA.                      */
/* DATE         :  09.01.90                                           */
/*                                                                    */
/* DESCRIPTION  : This procedure contains interface for               */
/*		  converting ASN1 encoded formats to apdus .          */
/*                                                                    */
/* INPUT-PARAMETER: asn1_cod: for returning the ASN1 encoded string   */
/*                  op_code: for defining apdu_type		      */
/*                  abs_id:  abstract id of protocol                  */
/*                                                                    */
/*  OUTPUT-PARAMETER: apdu: pointer to the apdu                       */
/*                    apdu_len: length of the apdu                    */
/*                                                                    */
/*                                                                    */
/*                                                                    */
/* RETURN-VALUE :  D2_NOERROR                                         */
/*                 D2_ERROR                                           */
/*                                                                    */
/* exoff **************************************************************/

signed32 d2a001_asn1_apdu(
  signed16 op_code,
  signed16 abs_id,
  byte ** apdu,                               
  signed32 *apdu_len,                            
  asn1_field * asn1_cod,
  byte *err_apdu )
{ 
const char function_name[] = "d2a001_asn1_apdu";
  signed32 res;
  int invoke_id;
  D23_header  * hdr;
  byte * scan;
  Bool   op_arg = FALSE;
  Bool   op_res = FALSE;
  ids_ch_res ch_res;
  ids_ch_arg ch_arg;
  asn1_field asn1_any,asn1_sav;

  D2_ASN1_TRACE_ENTER_DECODING(op_code,abs_id)

  D2_ASN1_TRACE_ENCODED_MESSAGE(asn1_cod)

  *apdu_len = 0;

 if ( abs_id == D27_DSP_AS &&
      ( /* NULL RESULT */
	(op_code == (D23_ADDOBJT + D2_RES_OPID)) ||
	(op_code == (D23_ADDOBJT + D2_RES_OPID + D2_ADM_OPID)) ||
	(op_code == (D23_REMOBJT + D2_RES_OPID)) ||
	(op_code == (D23_REMOBJT + D2_RES_OPID + D2_ADM_OPID)) ||
	(op_code == (D23_MODENTR + D2_RES_OPID)) ||
	(op_code == (D23_MODENTR + D2_RES_OPID + D2_ADM_OPID)) ||
	(op_code == (D23_REPLRDN + D2_RES_OPID)) ||
	(op_code == (D23_REPLRDN + D2_RES_OPID + D2_ADM_OPID)) ||
	(op_code == (D23_ABANDON + D2_RES_OPID)) ||
	(op_code == (D23_ABANDON + D2_RES_OPID + D2_ADM_OPID))
      )
    )
    {
    *apdu_len = D23_S_CHRES + D23_S_EOM;
    if ((*apdu = D27_MALLOC(svc_c_sev_warning,*apdu_len)) == NULL)
	{
	d27_010_set_error_apdu((D23_rserror *)err_apdu, D2_TEMP_ERR,
				    D2_SYS_ERR,D21_ALLOC_ERROR,apdu_len);
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
    ((D23_chain_res *) *apdu)->d23_chrtag = D23_CHRTAG;
    ((D23_chain_res *) *apdu)->d23_max_cref = 0;
    ((D23_eom *) (*apdu + D23_S_CHRES))->d23_eomtag = D23_EOMTAG;
    return(D2_NOERROR);
    }

 if (abs_id == D27_DAP_AS &&
      (
        ( /* NULL RESULT */
	  (op_code == (D23_ADDOBJT + D2_RES_OPID)) ||
	  (op_code == (D23_ADDOBJT + D2_RES_OPID + D2_ADM_OPID)) ||
	  (op_code == (D23_REMOBJT + D2_RES_OPID)) ||
	  (op_code == (D23_REMOBJT + D2_RES_OPID + D2_ADM_OPID)) ||
	  (op_code == (D23_MODENTR + D2_RES_OPID)) ||
	  (op_code == (D23_MODENTR + D2_RES_OPID + D2_ADM_OPID)) ||
	  (op_code == (D23_REPLRDN + D2_RES_OPID)) ||
	  (op_code == (D23_REPLRDN + D2_RES_OPID + D2_ADM_OPID)) ||
	  (op_code == (D23_ABANDON + D2_RES_OPID)) ||
	  (op_code == (D23_ABANDON + D2_RES_OPID + D2_ADM_OPID))
        ) ||
	op_code == (D23_UNBIND + D2_RES_OPID) || op_code == D23_UNBIND
      )
    )
	{
	  if ((*apdu = D27_MALLOC(svc_c_sev_warning,sizeof(D23_eom))) == NULL)
	    {
	       d27_010_set_error_apdu((D23_rserror *)err_apdu, D2_TEMP_ERR,
			    D2_SYS_ERR,D21_ALLOC_ERROR,apdu_len);
	       D2_ASN1_ERR_RETURN_WITH_TRACE
	      }
	  ((D23_eom *) *apdu)->d23_eomtag = D23_EOMTAG;
	  *apdu_len = sizeof(D23_eom);
	  return(D2_NOERROR);
	}

 if ((op_code != D23_BIND) && (op_code != (D23_BIND + D2_RES_OPID))
      && (op_code != (D23_BIND + D2_REJ_OPID)))
  {
      if ((scan = D27_MALLOC(svc_c_sev_warning,D2A_APDU_MEM))==NULL)
       {
	  d27_010_set_error_apdu((D23_rserror *)err_apdu, D2_TEMP_ERR,
			       D2_SYS_ERR,D21_ALLOC_ERROR, apdu_len);
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      *apdu = scan;
      *apdu_len = D2A_APDU_MEM;
   }

  if ((op_code != D23_BIND) && (op_code != (D23_BIND + D2_RES_OPID))
      && (op_code != (D23_BIND + D2_REJ_OPID))
      && (op_code == D23_READ || op_code == (D23_READ + D2_ADM_OPID) ||
	op_code == D23_REMOBJT || op_code == (D23_REMOBJT + D2_ADM_OPID) ||
	op_code == D23_ADDOBJT || op_code == (D23_ADDOBJT + D2_ADM_OPID) ||
	op_code == D23_MODENTR || op_code == (D23_MODENTR + D2_ADM_OPID) ||
	op_code == D23_LIST || op_code == (D23_LIST + D2_ADM_OPID) ||
	op_code == D23_SEARCH || op_code == (D23_SEARCH + D2_ADM_OPID) ||
	op_code == D23_REPLRDN || op_code == (D23_REPLRDN + D2_ADM_OPID) ||
	op_code == D23_COMPARE || op_code == (D23_COMPARE + D2_ADM_OPID) ||
	op_code == D23_ABANDON ))
    op_arg = TRUE;

  if (op_arg == TRUE)
   {
    hdr = (D23_header *) scan;

    hdr->d23_versno = D23_APDUV22;
    hdr->d23_dirid = D2MAXDIR_ID;
    hdr->d23_bindid = 0;  /* not sure, Why is it needed */
    scan += D23_S_HDR;
    }

   if (op_code == D23_ABANDON)
    {
         if ( ids_abandon_arg_dec (asn1_cod->v,asn1_cod->v+asn1_cod->l,
				&invoke_id) == 0)
	   {
	     D2_ASN1_ERR_RETURN_WITH_TRACE
	    }
	  *((signed32 *)scan) = invoke_id;
	  scan += sizeof(signed32);
	  ((D23_eom *) scan)->d23_eomtag = D23_EOMTAG;
	  *apdu_len = (scan - *apdu) + sizeof(D23_eom);
	  return(D2_NOERROR);
     }

  if (op_arg == TRUE && abs_id == D27_DSP_AS)
   {
    if ( ids_ch_arg_dec (asn1_cod->v,asn1_cod->v+asn1_cod->l,
				&ch_arg,&asn1_any) == 0)
	   {
	     ids_ch_arg_free(&ch_arg,asn1_any);
	     D2_ASN1_ERR_RETURN_WITH_TRACE
	    }
    else
      if (d2a160_ids_charg_apdu(&scan,apdu,apdu_len,&ch_arg,err_apdu) == D2_ERROR)
	      {
	      free(*apdu);
	      D2_ASN1_ERR_RETURN_WITH_TRACE
	      }
      asn1_sav = *asn1_cod;
      *asn1_cod = asn1_any;
   }

  if ((op_code != D23_BIND) && (op_code != (D23_BIND + D2_RES_OPID))
      && (op_code != (D23_BIND + D2_REJ_OPID))
      && (op_code == (D23_READ + D2_RES_OPID) ||
	op_code == (D23_READ + D2_RES_OPID + D2_ADM_OPID) ||
	op_code == (D23_SEARCH + D2_RES_OPID) ||
	op_code == (D23_SEARCH + D2_RES_OPID + D2_ADM_OPID) ||
	op_code == (D23_COMPARE + D2_RES_OPID) ||
	op_code == (D23_COMPARE + D2_RES_OPID + D2_ADM_OPID) ||
	op_code == (D23_LIST + D2_RES_OPID) ||
	op_code == (D23_LIST + D2_RES_OPID + D2_ADM_OPID) ))
    {
    op_res = TRUE;
    }

  if ( op_res == TRUE && abs_id == D27_DSP_AS)
   {
    if ( ids_ch_res_dec (asn1_cod->v,asn1_cod->v+asn1_cod->l,
				&ch_res,&asn1_any) == 0)
	   {
	     ids_ch_res_free(&ch_res,asn1_any);
	     D2_ASN1_ERR_RETURN_WITH_TRACE
	    }
    else
      if (d2a151_ids_chres_apdu(&scan,apdu,apdu_len,&ch_res,err_apdu) == D2_ERROR)
	      {
	      free(*apdu);
	      D2_ASN1_ERR_RETURN_WITH_TRACE
	      }
      asn1_sav = *asn1_cod;
      *asn1_cod = asn1_any;
   }

  switch (op_code) {

	case D23_BIND :
		res = d2a040_asn_bind_arg_apdu(apdu,apdu_len,asn1_cod);
		break;

	case (D23_BIND + D2_RES_OPID) :
		res = d2a041_asn_bind_res_apdu(apdu,apdu_len,asn1_cod,err_apdu);
		break;

	case D23_READ :
	case (D23_READ + D2_ADM_OPID) :
		res = d2a042_asn_read_arg_apdu(&scan,apdu,apdu_len,
							asn1_cod,err_apdu);
		break;

	case D23_REMOBJT :
	case (D23_REMOBJT + D2_ADM_OPID) :
		res = d2a066_asn_rem_arg_apdu(&scan,apdu,apdu_len,
							asn1_cod,err_apdu);
		break;

	case D23_ADDOBJT :
	case (D23_ADDOBJT + D2_ADM_OPID) :
		res = d2a074_asn_add_arg_apdu(&scan,apdu,apdu_len,
							asn1_cod,err_apdu);
		break;

	case D23_MODENTR :
	case (D23_MODENTR + D2_ADM_OPID) :
		res = d2a093_asn_mod_arg_apdu(&scan,apdu,apdu_len,
							asn1_cod,err_apdu);
		break;

	case D23_LIST :
	case (D23_LIST + D2_ADM_OPID) :
		res = d2a094_asn_list_arg_apdu(&scan,apdu,apdu_len,
							asn1_cod,err_apdu);
		break;

	case D23_SEARCH :
	case (D23_SEARCH + D2_ADM_OPID) :
		res = d2a096_asn_srch_arg_apdu(&scan,apdu,apdu_len,
							asn1_cod,err_apdu);
		break;

	case D23_REPLRDN :
	case (D23_REPLRDN + D2_ADM_OPID) :
		res = d2a129_asn_modrdn_apdu(&scan,apdu,apdu_len,
							asn1_cod,err_apdu);
		break;

	case D23_COMPARE :
	case (D23_COMPARE + D2_ADM_OPID) :
		res = d2a098_asn_comp_arg_apdu(&scan,apdu,apdu_len,
							asn1_cod,err_apdu);
		break;

	case (D23_READ + D2_RES_OPID) :
	case (D23_READ + D2_RES_OPID + D2_ADM_OPID) :
		res = d2a043_asn_read_res_apdu(&scan,apdu,apdu_len,
							asn1_cod,err_apdu);
		break;

	case (D23_LIST + D2_RES_OPID) :
	case (D23_LIST + D2_RES_OPID + D2_ADM_OPID) :
		res = d2a095_asn_list_res_apdu(&scan,apdu,apdu_len,
							asn1_cod,err_apdu);
		break;

	case (D23_SEARCH + D2_RES_OPID) :
	case (D23_SEARCH + D2_RES_OPID + D2_ADM_OPID) :
		res = d2a097_asn_srch_res_apdu(&scan,apdu,apdu_len,
							asn1_cod,err_apdu);
		break;

	case (D23_COMPARE + D2_RES_OPID) :
	case (D23_COMPARE + D2_RES_OPID + D2_ADM_OPID) :
		res = d2a099_asn_comp_res_apdu(&scan,apdu,apdu_len,
							asn1_cod,err_apdu);
		break;

	case (D23_BIND + D2_REJ_OPID) :
		res = d2a046_asn_bind_error_apdu(apdu,apdu_len,asn1_cod,
								err_apdu);
		break;

	default:
		res = D2_ERROR;
		break;
	}

  if (op_res == TRUE)
    {
      *apdu_len = scan - *apdu;
      if (abs_id == D27_DSP_AS)
	{
	  ids_ch_res_free(&ch_res, asn1_any);
	  *asn1_cod = asn1_sav;
	 }
     }

  if (op_arg == TRUE)
    {
      *apdu_len = scan - *apdu;
      if (abs_id == D27_DSP_AS)
	{
	  ids_ch_arg_free(&ch_arg, asn1_any);
	  *asn1_cod = asn1_sav;
	 }
     }

   D2_ASN1_TRACE_EXIT_DECODING(op_code,abs_id,res)
   return (res);
}

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* AUTHOR       :  Praveen Gupta, SIEMENS INDIA.                      */
/* DATE         :  09.01.90                                           */
/*                                                                    */
/* DESCRIPTION  : This procedure converts apdu format to ASN1 	      */
/* 		  encoded message (for BIND_RESULT message).	      */
/*                                                                    */
/* INPUT-PARAMETER :  apdu: pointer to the apdu                       */
/*                    apdu_len: length of the apdu                    */
/*                                                                    */
/* OUTPUT-PARAMETER : asn1_cod: for returning the ASN1 encoded string */
/*                                                                    */
/* RETURN-VALUE :  D2_NOERROR                                         */
/*                 D2_ERROR                                           */
/*                                                                    */
/* exoff **************************************************************/

static signed32 d2a021_apdu_bind_res_asn(
  byte * apdu,
  signed32 apdu_len,
  asn1_field * asn1_cod )
{
const char function_name[] = "d2a021_apdu_bind_res_asn";
signed32 res;
signed32 bind_res_len;
ids_bind_res i_bind_res;

byte err_apdu[D2A_ERR_APDU_SIZE]; /* dummy */
signed32 er_length; /* dummy */
D23_rsbind * rsbind = (D23_rsbind *) apdu;
byte * scan = apdu;

scan += D23_S_RSB;

if (rsbind->d23_Bversion != D23_V1988)
  {
  d27_010_set_error_apdu((D23_rserror *)err_apdu,D2_PERM_ERR,
			D2_ILL_VERS,D2_NOT_SPEC, &er_length);
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }
else
  {
  i_bind_res.version = V1988;
  }

switch (rsbind->d23_Bcrkind)
  {
  case D23_NO_CRED:
	i_bind_res.cred_defined = FALSE;
	break;

  case D23_SIMPLE_CRED:
	i_bind_res.cred_defined = TRUE;
	i_bind_res.cred_kind = CREDENTIAL_SIMPLE;
		/* Validity is not supported, as yet */
	i_bind_res.credentials.simple.validity_defined = FALSE;
	scan += sizeof(D23_simcr_info);
	if ((res=d2a005_apdu_bind_res_ids(scan,&i_bind_res))
							== D2_ERROR)
	  {
	  return(res);
	  }
	break;

  case D23_EXTERNAL_CRED:
	i_bind_res.cred_defined = TRUE;
	i_bind_res.cred_kind = CREDENTIAL_EXTERNAL;

	if ((res=d2a600_ext_cred_to_asn1(scan,&apdu_len,
		&(i_bind_res.credentials.external),err_apdu)) == D2_ERROR)
	  {
	  return(res);
	  }

	break;

  case D23_STRONG_CRED:
	asn1_cod->l = ((D23_strcr_info *)scan)->d23_strlen;
	if ((asn1_cod->v= D27_MALLOC (svc_c_sev_warning, asn1_cod->l)) == NULL)
	  {
	  d27_010_set_error_apdu((D23_rserror *)err_apdu, D2_TEMP_ERR,
				D2_SYS_ERR,D21_ALLOC_ERROR,&er_length);
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	  }

	scan += sizeof(D23_strcr_info);
	memcpy(asn1_cod->v,scan,asn1_cod->l);
	return(D2_NOERROR);

  default:
	d27_010_set_error_apdu((D23_rserror *)err_apdu,D2_CALL_ERR,
					D2_PAR_ERR,D2_INAPPR_AUTH,&er_length);
	D2_ASN1_ERR_RETURN_WITH_TRACE
  } /* end of switch */

/* for strong credentials all work is done; for all other credential kinds: */

bind_res_len =ids_bind_res_len(-1,&i_bind_res);
if ((asn1_cod->v= D27_MALLOC(svc_c_sev_warning,bind_res_len)) == NULL)
  { 
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }

asn1_cod->l= ids_bind_res_cod( asn1_cod->v,-1,0,&i_bind_res) - asn1_cod->v;
i_bind_res_free(&i_bind_res);

return(D2_NOERROR);
} /* end of d2a021_apdu_bind_res_asn */

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* AUTHOR       :  Praveen Gupta, SIEMENS INDIA.                      */
/* DATE         :  09.01.90                                           */
/*                                                                    */
/* DESCRIPTION  : This procedure converts apdu format to ASN1 	      */
/* 		  encoded message (for READ_RESULT message).	      */
/*                                                                    */
/* INPUT-PARAMETER :  apdu: pointer to the apdu                       */
/*                    apdu_len: length of the apdu                    */
/*                                                                    */
/* OUTPUT-PARAMETER : asn1_cod: for returning the ASN1 encoded string */
/*                                                                    */
/* RETURN-VALUE :  D2_NOERROR                                         */
/*                 D2_ERROR                                           */
/*                                                                    */
/* exoff **************************************************************/

static signed32 d2a023_apdu_read_res_asn(
  byte * apdu,
  signed32 apdu_len,
  asn1_field * asn1_cod )
{
const char function_name[] = "d2a023_apdu_read_res_asn";
    signed32 read_res_len;
    ids_read_res i_read_res;
    ids_common_res i_com_res;

	if (d2a009_apdu_read_res_ids(apdu,apdu_len,
					&i_read_res,&i_com_res)==D2_ERROR)
			{
			D2_ASN1_ERR_RETURN_WITH_TRACE
			}
	else {
		read_res_len =ids_read_res_len(-1,&i_read_res,&i_com_res);
		if ((asn1_cod->v=  D27_MALLOC (svc_c_sev_warning,read_res_len)) == NULL)
			{
/*                       ids_read_res_free(&i_read_res, &i_com_res);
*/			 D2_ASN1_ERR_RETURN_WITH_TRACE
			 }
		asn1_cod->l= ids_read_res_cod( asn1_cod->v,-1,0,
					&i_read_res,&i_com_res) - asn1_cod->v;
		 i_read_res_free(&i_read_res,&i_com_res);
		return(D2_NOERROR);
		}
}

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* AUTHOR       :  Praveen Gupta, SIEMENS INDIA.                      */
/* DATE         :  09.01.90                                           */
/*                                                                    */
/* DESCRIPTION  : This procedure converts apdu format to ASN1 	      */
/* 		  encoded message (for BIND_ERROR message).	      */
/*                                                                    */
/* INPUT-PARAMETER :  op_code: for defining apdu_type		      */
/*                    apdu: pointer to the apdu                       */
/*                    apdu_len: length of the apdu                    */
/*                                                                    */
/* OUTPUT-PARAMETER : asn1_cod: for returning the ASN1 encoded string */
/*                                                                    */
/* RETURN-VALUE :  D2_NOERROR                                         */
/*                 D2_ERROR                                           */
/*                                                                    */
/* exoff **************************************************************/

signed32 d2a026_apdu_bind_error_asn(
  byte * apdu,
  signed32 apdu_len,
  asn1_field * asn1_cod )
{
signed32 res;
signed32 bind_error_len;
ids_bind_error i_bind_error;
const char function_name[] = "d2a026_apdu_bind_error_asn";

	/* ids_bind_error structure is same as the ids_bind_error
	 * stuctures and, hence, ids_bind_error stucture is passed
	 * as parameter in d2a013_apdu_bind_error_ids procedure */


if ((res=d2a013_apdu_bind_error_ids(apdu,apdu_len, &i_bind_error))==D2_ERROR)
  {
  return(res);
  }

bind_error_len =ids_bind_error_len(-1,&i_bind_error); 
if ((asn1_cod->v= D27_MALLOC (svc_c_sev_warning,bind_error_len)) == NULL)
  {
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }

asn1_cod->l= ids_bind_error_cod( asn1_cod->v,-1,0,&i_bind_error)
								 - asn1_cod->v;
i_bind_error_free(&i_bind_error);

return(D2_NOERROR);
}

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* AUTHOR       :  Praveen Gupta, SIEMENS INDIA.                      */
/* DATE         :  09.01.90                                           */
/*                                                                    */
/* DESCRIPTION  : This procedure converts apdu formats to ASN1 	      */
/* 		 encoded messages (for ERROR messages other than bind)*/
/*                                                                    */
/* INPUT-PARAMETER :  apdu: pointer to the apdu                       */
/*                    apdu_len: length of the apdu                    */
/*                                                                    */
/* OUTPUT-PARAMETER : asn1_cod: for returning the ASN1 encoded string */
/*                                                                    */
/* RETURN-VALUE :  D2_NOERROR                                         */
/*                 D2_ERROR                                           */
/*                                                                    */
/* exoff **************************************************************/

signed32 d2a027_apdu_error_asn(
  byte * apdu,
  signed32 apdu_len,
  signed32 * error,
  asn1_field * asn1_buf,
  signed16   absid )
{
signed32 res;
ids_error error_buf;
signed32 error_len;
ids_dsa_referral  dsa_ref;
D23_rserror * rs_err = (D23_rserror *) apdu;
const char function_name[] = "d2a027_apdu_error_asn";

D2_ASN1_TRACE_ENTER_ERROR_ENCODING(rs_err->d23_Zerrval,absid);

   if (rs_err->d23_Zerrval == D2_ABANDONED)
     {
     *error = ABAN_ERROR;
     asn1_buf->l = 0;
     asn1_buf->v = NULL;
     return(D2_NOERROR);
     }

   if (absid == D27_DSP_AS && 
        ((D23_drf_info *)(apdu+D23_S_RSERR))->d23_drftag == D23_DRFTAG)
   {
     apdu += D23_S_RSERR;
     if (d2a164_apdu_dsa_ref_ids(apdu,&apdu_len,&dsa_ref,apdu) == D2_ERROR)
		{
		D2_ASN1_ERR_RETURN_WITH_TRACE
		}
     error_len = ids_dsa_referral_len(-1,&dsa_ref);
     if ((asn1_buf->v= D27_MALLOC (svc_c_sev_warning,error_len)) == NULL)
	{
	 D2_ASN1_ERR_RETURN_WITH_TRACE
	}
      asn1_buf->l= ids_dsa_referral_cod(asn1_buf->v,-1,0, &dsa_ref)
			- asn1_buf->v;
      *error = ACC_REFERRAL;
      i_dsa_referral_free(&dsa_ref);
      return(D2_NOERROR);
    }

   if ((res=d2a015_error_cod_ids(apdu,apdu_len,error,&error_buf))==D2_ERROR)
	    	{
		D2_ASN1_ERR_RETURN_WITH_TRACE
		}
   error_buf.error_code = *error;

   switch (*error)
    {
	case ATT_ERROR:
	{
	  error_len = ids_att_error_len(-1,&error_buf.contents.att);
	  if ((asn1_buf->v= D27_MALLOC (svc_c_sev_warning,error_len)) == NULL)
		{
		 D2_ASN1_ERR_RETURN_WITH_TRACE
		}
	  asn1_buf->l= ids_att_error_cod(asn1_buf->v,-1,0, &error_buf.contents.att) -
				asn1_buf->v;
	  break;
	}
	case NAME_ERROR:
	{
	  error_len = ids_name_error_len(-1,&(error_buf.contents.name));
	  if ((asn1_buf->v= D27_MALLOC (svc_c_sev_warning,error_len)) == NULL)
		{
		 D2_ASN1_ERR_RETURN_WITH_TRACE
		}
	  asn1_buf->l= ids_name_error_cod(asn1_buf->v,-1,0, &(error_buf.contents.name)) -
				asn1_buf->v;
	  break;
	}
	case SERV_ERROR:
	{
	  error_len = ids_serv_error_len(-1,&error_buf.contents.serv);
	  if ((asn1_buf->v= D27_MALLOC (svc_c_sev_warning,error_len)) == NULL)
		{
		 D2_ASN1_ERR_RETURN_WITH_TRACE
		}
	  asn1_buf->l= ids_serv_error_cod(asn1_buf->v,-1,0, &error_buf.contents.serv) -
				asn1_buf->v;
	  break;
	}
	case REFERRAL:
	{
	  error_len = ids_referral_len(-1,&(error_buf.contents.ref));
	  if ((asn1_buf->v= D27_MALLOC (svc_c_sev_warning,error_len)) == NULL)
		{
		 D2_ASN1_ERR_RETURN_WITH_TRACE
		}
	  asn1_buf->l= ids_referral_cod(asn1_buf->v,-1,0, &(error_buf.contents.ref)) -
				asn1_buf->v;
	  break;
	}

	case SEC_ERROR:
	{
	  error_len = ids_sec_error_len(-1,&error_buf.contents.sec);
	  if ((asn1_buf->v= D27_MALLOC (svc_c_sev_warning,error_len)) == NULL)
		{
		 D2_ASN1_ERR_RETURN_WITH_TRACE
		}
	  asn1_buf->l= ids_sec_error_cod(asn1_buf->v,-1,0, &error_buf.contents.sec) -
				asn1_buf->v;
	  break;
	}
	case ABAN_FAILED:
	{
	  error_len = ids_aban_failed_len(-1,&(error_buf.contents.aban_failed));
	  if ((asn1_buf->v= D27_MALLOC (svc_c_sev_warning,error_len)) == NULL)
		{
		 D2_ASN1_ERR_RETURN_WITH_TRACE
		}
	  asn1_buf->l= ids_aban_failed_cod(asn1_buf->v,-1,0,
			   &(error_buf.contents.aban_failed)) - asn1_buf->v;
	  break;
	}
	case UPDATE_ERROR:
	{
	  error_len = ids_upd_error_len(-1,&error_buf.contents.upd);
	  if ((asn1_buf->v= D27_MALLOC (svc_c_sev_warning,error_len)) == NULL)
		{
		 D2_ASN1_ERR_RETURN_WITH_TRACE
		}
	  asn1_buf->l= ids_upd_error_cod(asn1_buf->v,-1,0, &error_buf.contents.upd) -
				asn1_buf->v;
	}
     }

   i_error_free(*error,&error_buf);

   D2_ASN1_TRACE_ENCODED_MESSAGE(asn1_buf)

   D2_ASN1_TRACE_EXIT_ERROR_ENCODING(*error,absid,D2_NOERROR)

return(D2_NOERROR);
}

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* AUTHOR       :  Praveen Gupta, SIEMENS INDIA.                      */
/* DATE         :  09.01.90                                           */
/*                                                                    */
/* DESCRIPTION  : This procedure releases allocated memory for error  */
/* 		  structures.					      */
/*                                                                    */
/* RETURN-VALUE :  D2_NOERROR                                         */
/*                 D2_ERROR                                           */
/*                                                                    */
/* exoff **************************************************************/

static void i_error_free (
  signed32	   error_code,
  ids_error  * error_buf )
{
	switch (error_code)
	{
	case ATT_ERROR:
	  i_att_error_free (&(error_buf->contents.att));
	  break;
	case NAME_ERROR:
	  i_name_error_free ( &(error_buf->contents.name));
	  break;
	case REFERRAL:
	  i_cont_reference_free (&(error_buf->contents.ref.candidate));
	  break;
	}
}

/* exon ***************************************************************/
/*                                                                    */
/* NAME         :  d2a040_asn_bind_arg_apdu                           */
/*                                                                    */
/* AUTHOR       :  Praveen Gupta, SIEMENS INDIA.                      */
/* DATE         :  09.01.90                                           */
/*                                                                    */
/* DESCRIPTION  : This procedure converts ASN1 encoded messages to    */
/* 		  apdu formats.(for BIND_ARG messages)	 	      */
/*                                                                    */
/* INPUT-PARAMETER :  asn1_cod: for inputing the ASN1 encoded string  */
/*                                                                    */
/* OUTPUT-PARAMETER : apdu: pointer to the apdu                       */
/*                    apdu_len: length of the apdu                    */
/*                                                                    */
/* RETURN-VALUE :  D2_NOERROR                                         */
/*                 D2_ERROR                                           */
/*                                                                    */
/* exoff **************************************************************/

static signed32 d2a040_asn_bind_arg_apdu(
  byte ** apdu,
  signed32 * apdu_len,
  asn1_field * asn1_cod )
{
const char function_name[] = "d2a040_asn_bind_arg_apdu";
signed32 res;
ids_bind_arg i_bind_arg;
byte * scan;
D23_inbind * inbind;
D23_simcr_info * simcr;
D23_strcr_info * strcr;
signed32 needed_length;
byte er_apdu[D2A_ERR_APDU_SIZE];

/* assume a bind argument will come; if we see later */
/* that we get a decoding error all will be reset and freed */

if ((scan = D27_MALLOC(svc_c_sev_warning,D2A_APDU_MEM)) == NULL)
  {
  d27_010_set_error_apdu((D23_rserror *)er_apdu, D2_TEMP_ERR,
					D2_SYS_ERR,D21_ALLOC_ERROR,apdu_len);
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }

*apdu = scan;
inbind = (D23_inbind *) scan;
inbind->d23_Aversno = D23_APDUV22;
inbind->d23_Adirid = 0;
inbind->d23_Afil = 0;
scan += sizeof(D23_inbind);

if ( ids_bind_arg_dec (asn1_cod->v,asn1_cod->v+asn1_cod->l,
							&i_bind_arg) == 0)
  {
  d27_010_set_error_apdu((D23_rserror *)er_apdu, D2_TEMP_ERR,
  				D2_REM_ERR,D2_NO_RESPONSE,apdu_len);
  ids_bind_arg_free(&i_bind_arg);
  free(*apdu);
  *apdu = NULL;
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }
  
if (i_bind_arg.cred_defined == FALSE)
  {
  inbind->d23_Acrkind = D23_NO_CRED;
  }
else
  {
  switch (i_bind_arg.cred_kind)
    {
    case CREDENTIAL_SIMPLE: /* simple credentials */
	inbind->d23_Acrkind = D23_SIMPLE_CRED;
	simcr = (D23_simcr_info *) scan;
	simcr->d23_scrtag = D23_SIMTAG;
	scan += sizeof(D23_simcr_info);
	if ((res=d2a003_ids_bind_arg_apdu(&scan,apdu_len,
					&i_bind_arg))==D2_ERROR)
	  {
	  ids_bind_arg_free(&i_bind_arg);
	  free(*apdu);
	  *apdu = NULL;
	  return(res);
	  }
	break;

    case CREDENTIAL_STRONG: /* strong credentials */
	inbind->d23_Acrkind = D23_STRONG_CRED;
	strcr = (D23_strcr_info *) scan;
	strcr->d23_strtag = D23_STRTAG;
	strcr->d23_strlen = asn1_cod->l;
	strcr->d23_stroff = D2_ALIGN(asn1_cod->l);
	scan += sizeof(D23_strcr_info);
	needed_length = sizeof(D23_inbind) + 
			sizeof(D23_strcr_info) +
			strcr->d23_stroff +
			sizeof(D23_eom);

	if (needed_length > D2A_APDU_MEM)
	  {
	  /* reallocate because of huge token */
	  if ((*apdu = D27_REALLOC(svc_c_sev_warning,*apdu,needed_length)) 
		== NULL)
	    {
	    d27_010_set_error_apdu((D23_rserror *)er_apdu, D2_TEMP_ERR,
					D2_SYS_ERR,D21_ALLOC_ERROR,apdu_len);
	    D2_ASN1_ERR_RETURN_WITH_TRACE
	    }

	  scan = *apdu + sizeof(D23_inbind);
	  strcr = (D23_strcr_info *) scan;
	  scan += sizeof(D23_strcr_info);
	  } /* end of reallocation */

	memcpy(scan,asn1_cod->v,asn1_cod->l);
	scan += strcr->d23_stroff;
	break;

    case CREDENTIAL_EXTERNAL: /* external credentials */
	inbind->d23_Acrkind = D23_EXTERNAL_CRED;
	if ((res=d2a601_ext_cred_from_asn1(&scan,apdu_len,
			&(i_bind_arg.credentials.external),er_apdu))==D2_ERROR)
	  {
	  ids_bind_arg_free(&i_bind_arg);
	  free(*apdu);
	  *apdu = NULL;
	  return(res);
	  }

	break;

    default: /* invalid credentials ? ? ? */
	D2_ASN1_ERR_RETURN_WITH_TRACE
    } /* end of switch for credential kind */
  } /* end of if-else credentials defined */

((D23_eom *) scan)->d23_eomtag = D23_EOMTAG;
((D23_eom *) scan)->d23_eomfil = D2_ALIGN(D23_EOMTAG);
*apdu_len = (scan + sizeof(D23_eom)) - *apdu;

ids_bind_arg_free(&i_bind_arg);
return (D2_NOERROR);
} /* end of d2a040_asn_bind_arg_apdu */
   
/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* AUTHOR       :  Praveen Gupta, SIEMENS INDIA.                      */
/* DATE         :  09.01.90                                           */
/*                                                                    */
/* DESCRIPTION  : This procedure converts ASN1 encoded messages to    */
/* 		  apdu formats.(for READ_ARG messages)	              */
/*                                                                    */
/* INPUT-PARAMETER :  asn1_cod: for inputing the ASN1 encoded string  */
/*                                                                    */
/* OUTPUT-PARAMETER : apdu: pointer to the apdu                       */
/*                    apdu_len: length of the apdu                    */
/*                                                                    */
/* RETURN-VALUE :  D2_NOERROR                                         */
/*                 D2_ERROR                                           */
/*                                                                    */
/* exoff **************************************************************/

static signed32 d2a042_asn_read_arg_apdu(
  byte ** apdu,
  byte ** beg_apdu,
  signed32 * apdu_len,
  asn1_field * asn1_cod,
  byte * err_apdu )
{
const char function_name[] = "d2a042_asn_read_arg_apdu";
    signed32 res;
    ids_read_arg i_read_arg;
    ids_common_arg i_com_arg;

	if ( ids_read_arg_dec (asn1_cod->v,asn1_cod->v+asn1_cod->l,
				&i_read_arg,&i_com_arg) == 0)
		{
		 D2_ASN1_ERR_RETURN_WITH_TRACE
		}
	 else
	   	{
		   if ((res=d2a007_ids_read_arg_apdu(apdu,beg_apdu,apdu_len,
				&i_read_arg,&i_com_arg,err_apdu))==D2_ERROR)
			return(res);
		   ids_read_arg_free(&i_read_arg,&i_com_arg);
		   return(D2_NOERROR);
		}
}

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* AUTHOR       :  Helmut Volpers                                     */
/* DATE         :  26.02.90                                           */
/*                                                                    */
/* DESCRIPTION  : This procedure converts ASN1 encoded messages to    */
/*                apdu formats.(for REM_ARG messages)                 */
/*                                                                    */
/* INPUT-PARAMETER :  asn1_cod: for inputing the ASN1 encoded string  */
/*                                                                    */
/* OUTPUT-PARAMETER : apdu: pointer to the apdu                       */
/*                    apdu_len: length of the apdu                    */
/*                                                                    */
/* RETURN-VALUE :  D2_NOERROR                                         */
/*                 D2_ERROR                                           */
/*                                                                    */
/* exoff **************************************************************/

static signed32 d2a066_asn_rem_arg_apdu(
  byte ** apdu,
  byte ** beg_apdu,
  signed32 * apdu_len,
  asn1_field * asn1_cod,
  byte  * err_apdu )
{
const char function_name[] = "d2a066_asn_rem_arg_apdu";
    signed32 res;
    ids_remove_arg i_remove_arg;
    ids_common_arg i_com_arg;

	if ( ids_remove_arg_dec (asn1_cod->v,asn1_cod->v+asn1_cod->l,
				&i_remove_arg,&i_com_arg) == 0)
		{
		 ids_remove_arg_free(&i_remove_arg,&i_com_arg);
		 D2_ASN1_ERR_RETURN_WITH_TRACE
		}
	 else
	   	{
		   if ((res=d2a068_ids_rem_arg_apdu(apdu,beg_apdu,apdu_len,
			       &i_remove_arg,&i_com_arg,err_apdu))==D2_ERROR)
			return(res);
		   ids_remove_arg_free(&i_remove_arg,&i_com_arg);
		   return(D2_NOERROR);
		}
}

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* AUTHOR       :  Helmut Volpers                                     */
/* DATE         :  26.02.90                                           */
/*                                                                    */
/* DESCRIPTION  : This procedure converts ASN1 encoded messages to    */
/*                apdu formats.(for ADD_ARG messages)                 */
/*                                                                    */
/* INPUT-PARAMETER :  asn1_cod: for inputing the ASN1 encoded string  */
/*                  abs_id:  abstract id of protocol                  */
/*                                                                    */
/* OUTPUT-PARAMETER : apdu: pointer to the apdu                       */
/*                    apdu_len: length of the apdu                    */
/*                                                                    */
/* RETURN-VALUE :  D2_NOERROR                                         */
/*                 D2_ERROR                                           */
/*                                                                    */
/* exoff **************************************************************/

static signed32 d2a074_asn_add_arg_apdu(
  byte ** apdu,
  byte ** beg_apdu,
  signed32 * apdu_len,
  asn1_field * asn1_cod,
  byte  * err_apdu )
{
const char function_name[] = "d2a074_asn_add_arg_apdu";
    signed32 res;
    ids_add_arg i_add_arg;
    ids_common_arg i_com_arg;

	if ( ids_add_arg_dec (asn1_cod->v,asn1_cod->v+asn1_cod->l,
				&i_add_arg,&i_com_arg) == 0)
		{
		 ids_add_arg_free(&i_add_arg,&i_com_arg);
		 D2_ASN1_ERR_RETURN_WITH_TRACE
		}
	    else
		{
		if ((res=d2a076_ids_add_arg_apdu(apdu,beg_apdu,apdu_len,
				    &i_add_arg,&i_com_arg,err_apdu))==D2_ERROR)
		return(res);
		ids_add_arg_free(&i_add_arg,&i_com_arg);
		}
       return(D2_NOERROR);
}

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* AUTHOR       :  Praveen Gupta                                      */
/* DATE         :  26.02.90                                           */
/*                                                                    */
/* DESCRIPTION  : This procedure converts apdu encoded messages to    */
/*                ASN1 formats.(for LIST_RES messages)                */
/*                                                                    */
/* INPUT-PARAMETER :  apdu: pointer to the apdu                       */
/*                    apdu_len: length of the apdu                    */
/*                                                                    */
/* OUTPUT-PARAMETER :  asn1_cod: for inputing the ASN1 encoded string */
/*                     err_apdu: Error apdu in case of error          */
/*                                                                    */
/* RETURN-VALUE :  D2_NOERROR                                         */
/*                 D2_ERROR                                           */
/*                                                                    */
/* exoff **************************************************************/

static signed32 d2a088_apdu_list_res_asn(
  byte * apdu,
  signed32 apdu_len,
  asn1_field * asn1_cod )
{
const char function_name[] = "d2a088_apdu_list_res_asn";
    signed32 res;
    signed32 list_res_len;
    ids_list_res i_list_res;
    ids_common_res i_com_res;

	if ((res=d2a102_apdu_list_res_ids(apdu,apdu_len,
					&i_list_res,&i_com_res))==D2_ERROR)
		return(res);
	else {
		list_res_len =ids_list_res_len(-1,&i_list_res,&i_com_res);
		if ((asn1_cod->v= D27_MALLOC (svc_c_sev_warning,list_res_len)) == NULL)
			{
			 D2_ASN1_ERR_RETURN_WITH_TRACE
			 }
		asn1_cod->l= ids_list_res_cod( asn1_cod->v,-1,0,
					&i_list_res,&i_com_res) - asn1_cod->v;
		i_list_res_free(&i_list_res, &i_com_res);
		return(D2_NOERROR);
		}
}

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* AUTHOR       :  Praveen Gupta                                      */
/* DATE         :  28.02.90                                           */
/*                                                                    */
/* DESCRIPTION  : This procedure converts ASN1 encoded messages to    */
/*                apdu formats.(for COMPARE_ARG messages)             */
/*                                                                    */
/* INPUT-PARAMETER :  asn1_cod: for inputing the ASN1 encoded string  */
/*                  abs_id:  abstract id of protocol                  */
/*                                                                    */
/* OUTPUT-PARAMETER : apdu: pointer to the apdu                       */
/*                    apdu_len: length of the apdu                    */
/*                                                                    */
/* RETURN-VALUE :  D2_NOERROR                                         */
/*                 D2_ERROR                                           */
/*                                                                    */
/* exoff **************************************************************/

static signed32 d2a098_asn_comp_arg_apdu(
  byte ** apdu,
  byte ** beg_apdu,
  signed32 * apdu_len,
  asn1_field * asn1_cod,
  byte  * err_apdu )
{
const char function_name[] = "d2a098_asn_comp_arg_apdu";
    signed32 res;
    ids_compare_arg i_compare_arg;
    ids_common_arg i_com_arg;

	if ( ids_compare_arg_dec (asn1_cod->v,asn1_cod->v+asn1_cod->l,
				&i_compare_arg,&i_com_arg) == 0)
		{
		 ids_compare_arg_free(&i_compare_arg,&i_com_arg);
		 D2_ASN1_ERR_RETURN_WITH_TRACE
		}
	 else
	   	{
		   if ((res=d2a083_ids_compare_arg_apdu(apdu,beg_apdu,apdu_len,
			      &i_compare_arg,&i_com_arg,err_apdu))==D2_ERROR)
			return(res);
		   ids_compare_arg_free(&i_compare_arg, &i_com_arg);
		   return(D2_NOERROR);
		}
}

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* AUTHOR       :  Praveen Gupta                                      */
/* DATE         :  26.02.90                                           */
/*                                                                    */
/* DESCRIPTION  : This procedure converts apdu encoded messages to    */
/*                ASN1 formats.(for SEARCH_RES messages)              */
/*                                                                    */
/* INPUT-PARAMETER :  apdu: pointer to the apdu                       */
/*                    apdu_len: length of the apdu                    */
/*                                                                    */
/* OUTPUT-PARAMETER :  asn1_cod: for inputing the ASN1 encoded string */
/*                     err_apdu: Error apdu in case of error          */
/*                                                                    */
/* RETURN-VALUE :  D2_NOERROR                                         */
/*                 D2_ERROR                                           */
/*                                                                    */
/* exoff **************************************************************/

static signed32 d2a090_apdu_search_res_asn(
  byte * apdu,
  signed32 apdu_len,
  asn1_field * asn1_cod )
{
const char function_name[] = "d2a090_apdu_search_res_asn";
    signed32 res;
    signed32 search_res_len;
    ids_search_res i_search_res;
    ids_common_res i_com_res;

	if ((res=d2a072_apdu_search_res_ids(apdu,apdu_len,
					&i_search_res,&i_com_res))==D2_ERROR)
		return(res);
	else {
		search_res_len =ids_search_res_len(-1,&i_search_res,&i_com_res);
		if ((asn1_cod->v= D27_MALLOC (svc_c_sev_warning,search_res_len)) == NULL)
			{
/*                       ids_search_res_free(&i_search_res, &i_com_res);
*/			 D2_ASN1_ERR_RETURN_WITH_TRACE
			 }
		asn1_cod->l= ids_search_res_cod( asn1_cod->v,-1,0,
					&i_search_res,&i_com_res) - asn1_cod->v;
		 i_search_res_free(&i_search_res, &i_com_res);
		return(D2_NOERROR);
		}
}

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* AUTHOR       :  Praveen Gupta                                      */
/* DATE         :  26.02.90                                           */
/*                                                                    */
/* DESCRIPTION  : This procedure converts apdu encoded messages to    */
/*                ASN1 formats.(for COMP_RES messages)                */
/*                                                                    */
/* INPUT-PARAMETER :  apdu: pointer to the apdu                       */
/*                    apdu_len: length of the apdu                    */
/*                                                                    */
/* OUTPUT-PARAMETER :  asn1_cod: for inputing the ASN1 encoded string */
/*                     err_apdu: Error apdu in case of error          */
/*                                                                    */
/* RETURN-VALUE :  D2_NOERROR                                         */
/*                 D2_ERROR                                           */
/*                                                                    */
/* exoff **************************************************************/

static signed32 d2a092_apdu_comp_res_asn(
  byte * apdu,
  signed32 apdu_len,
  asn1_field * asn1_cod )
{
const char function_name[] = "d2a092_apdu_comp_res_asn";
    signed32 res;
    signed32 compare_res_len;
    ids_compare_res i_compare_res;
    ids_common_res i_com_res;

	if ((res=d2a085_apdu_compare_res_ids(apdu,apdu_len,
					&i_compare_res,&i_com_res))==D2_ERROR)
		{
		D2_ASN1_ERR_RETURN_WITH_TRACE
		}
	else {
		compare_res_len =ids_compare_res_len(-1,&i_compare_res,&i_com_res);
		if ((asn1_cod->v= D27_MALLOC (svc_c_sev_warning,compare_res_len)) == NULL)
			{
/*                       ids_compare_res_free(&i_compare_res);
			 ids_common_res_free(&i_com_res);
*/			 D2_ASN1_ERR_RETURN_WITH_TRACE
			 }
		asn1_cod->l= ids_compare_res_cod( asn1_cod->v,-1,0,
					&i_compare_res,&i_com_res) - asn1_cod->v;
		 i_compare_res_free(&i_compare_res, &i_com_res);
		return(D2_NOERROR);
		}
}

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* AUTHOR       :  Praveen Gupta                                      */
/* DATE         :  28.02.90                                           */
/*                                                                    */
/* DESCRIPTION  : This procedure converts ASN1 encoded messages to    */
/*                apdu formats.(for MOD_ARG messages)                 */
/*                                                                    */
/* INPUT-PARAMETER :  asn1_cod: for inputing the ASN1 encoded string  */
/*                  abs_id:  abstract id of protocol                  */
/*                                                                    */
/* OUTPUT-PARAMETER : apdu: pointer to the apdu                       */
/*                    apdu_len: length of the apdu                    */
/*                                                                    */
/* RETURN-VALUE :  D2_NOERROR                                         */
/*                 D2_ERROR                                           */
/*                                                                    */
/* exoff **************************************************************/

static signed32 d2a093_asn_mod_arg_apdu(
  byte ** apdu,
  byte ** beg_apdu,
  signed32 * apdu_len,
  asn1_field * asn1_cod,
  byte  * err_apdu )
{
const char function_name[] = "d2a093_asn_mod_arg_apdu";
    signed32 res;
    ids_modify_arg i_modify_arg;
    ids_common_arg i_com_arg;

	if ( ids_modify_arg_dec (asn1_cod->v,asn1_cod->v+asn1_cod->l,
				&i_modify_arg,&i_com_arg) == 0)
		{
		 ids_modify_arg_free(&i_modify_arg,&i_com_arg);
		 D2_ASN1_ERR_RETURN_WITH_TRACE
		}
	 else
	   	{
		   if ((res=d2a105_ids_modify_arg_apdu(apdu,beg_apdu,apdu_len,
			      &i_modify_arg,&i_com_arg,err_apdu))==D2_ERROR)
			return(res);
		   ids_modify_arg_free(&i_modify_arg, &i_com_arg);
		    return(D2_NOERROR);
		}
}

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* AUTHOR       :  Praveen Gupta                                      */
/* DATE         :  28.02.90                                           */
/*                                                                    */
/* DESCRIPTION  : This procedure converts ASN1 encoded messages to    */
/*                apdu formats.(for LIST_ARG messages)                */
/*                                                                    */
/* INPUT-PARAMETER :  asn1_cod: for inputing the ASN1 encoded string  */
/*                  abs_id:  abstract id of protocol                  */
/*                                                                    */
/* OUTPUT-PARAMETER : apdu: pointer to the apdu                       */
/*                    apdu_len: length of the apdu                    */
/*                                                                    */
/* RETURN-VALUE :  D2_NOERROR                                         */
/*                 D2_ERROR                                           */
/*                                                                    */
/* exoff **************************************************************/

static signed32 d2a094_asn_list_arg_apdu(
  byte ** apdu,
  byte ** beg_apdu,
  signed32 * apdu_len,
  asn1_field * asn1_cod,
  byte  * err_apdu )
{
const char function_name[] = "d2a094_asn_list_arg_apdu";
    signed32 res;
    ids_list_arg i_list_arg;
    ids_common_arg i_com_arg;

	if ( ids_list_arg_dec (asn1_cod->v,asn1_cod->v+asn1_cod->l,
				&i_list_arg,&i_com_arg) == 0)
		{
		 ids_list_arg_free(&i_list_arg,&i_com_arg);
		 D2_ASN1_ERR_RETURN_WITH_TRACE
		}
	 else
	   	{
		   if ((res=d2a101_ids_list_arg_apdu(apdu,beg_apdu,apdu_len,
				 &i_list_arg,&i_com_arg,err_apdu))==D2_ERROR)
			return(res);
		   ids_list_arg_free(&i_list_arg, &i_com_arg);
		   return(D2_NOERROR);
		}
}

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* AUTHOR       :  Praveen Gupta                                      */
/* DATE         :  28.02.90                                           */
/*                                                                    */
/* DESCRIPTION  : This procedure converts ASN1 encoded messages to    */
/*                apdu formats.(for SEARCH_ARG messages)              */
/*                                                                    */
/* INPUT-PARAMETER :  asn1_cod: for inputing the ASN1 encoded string  */
/*                  abs_id:  abstract id of protocol                  */
/*                                                                    */
/* OUTPUT-PARAMETER : apdu: pointer to the apdu                       */
/*                    apdu_len: length of the apdu                    */
/*                                                                    */
/* RETURN-VALUE :  D2_NOERROR                                         */
/*                 D2_ERROR                                           */
/*                                                                    */
/* exoff **************************************************************/

static signed32 d2a096_asn_srch_arg_apdu(
  byte ** apdu,
  byte ** beg_apdu,
  signed32 * apdu_len,
  asn1_field * asn1_cod,
  byte  * err_apdu )
{
const char function_name[] = "d2a096_asn_srch_arg_apdu";
    signed32 res;
    ids_search_arg i_search_arg;
    ids_common_arg i_com_arg;

	if ( ids_search_arg_dec (asn1_cod->v,asn1_cod->v+asn1_cod->l,
				&i_search_arg,&i_com_arg) == 0)
		{
		 ids_search_arg_free(&i_search_arg,&i_com_arg);
		 D2_ASN1_ERR_RETURN_WITH_TRACE
		}
	 else
	   	{
		   if ((res=d2a070_ids_search_arg_apdu(apdu,beg_apdu,apdu_len,
				   &i_search_arg,&i_com_arg,err_apdu))==D2_ERROR)
			return(res);
		   ids_search_arg_free(&i_search_arg, &i_com_arg);
		   return(D2_NOERROR);
		}
}

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* AUTHOR       :  Praveen Gupta                                      */
/* DATE         :  28.02.90                                           */
/*                                                                    */
/* DESCRIPTION  : This procedure converts ASN1 encoded messages to    */
/*                apdu formats.(for MOD_RDN_ARG messages)             */
/*                                                                    */
/* INPUT-PARAMETER :  asn1_cod: for inputing the ASN1 encoded string  */
/*                  abs_id:  abstract id of protocol                  */
/*                                                                    */
/* OUTPUT-PARAMETER : apdu: pointer to the apdu                       */
/*                    apdu_len: length of the apdu                    */
/*                                                                    */
/* RETURN-VALUE :  D2_NOERROR                                         */
/*                 D2_ERROR                                           */
/*                                                                    */
/* exoff **************************************************************/


static signed32 d2a129_asn_modrdn_apdu(
  byte ** apdu,
  byte ** beg_apdu,
  signed32 * apdu_len,
  asn1_field * asn1_cod,
  byte * err_apdu )
{
const char function_name[] = "d2a129_asn_modrdn_apdu";
    signed32 res;
    ids_modify_rdn_arg i_modrdn_arg;
    ids_common_arg     i_com_arg;

	if ( ids_modify_rdn_arg_dec (asn1_cod->v,asn1_cod->v+asn1_cod->l,
				&i_modrdn_arg,&i_com_arg) == 0)
		{
		 ids_modify_rdn_arg_free(&i_modrdn_arg, &i_com_arg);
		 D2_ASN1_ERR_RETURN_WITH_TRACE
		}
	 else
	   	{
		   if ((res=d2a126_ids_modrdn_arg_apdu(apdu,beg_apdu,apdu_len,
			    &i_modrdn_arg, &i_com_arg,err_apdu)) == D2_ERROR)
			return(res);
		   ids_modify_rdn_arg_free(&i_modrdn_arg, &i_com_arg);
		   return(D2_NOERROR);
		}
}


