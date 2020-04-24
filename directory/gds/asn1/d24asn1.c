/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d24asn1.c,v $
 * Revision 1.1.10.2  1996/02/18  19:42:51  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:23:41  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:23:50  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:53:32  root]
 * 
 * Revision 1.1.8.5  1994/07/06  15:06:37  marrek
 * 	July 1994 code drop.
 * 	[1994/07/06  10:00:22  marrek]
 * 
 * Revision 1.1.8.4  1994/06/21  14:44:16  marrek
 * 	June 1994 code submission.
 * 	[1994/06/21  09:25:59  marrek]
 * 
 * Revision 1.1.8.3  1994/05/10  15:50:31  marrek
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/02  13:24:08  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:15:17  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:08:18  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:15:08  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:50:57  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:41:43  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  10:47:32  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  12:34:08  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:12:43  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:10:02  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  15:56:54  marrek
 * 	November 1992 code drop
 * 	[1992/11/17  12:49:53  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:05:39  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:02:07  melman]
 * 
 * $EndLog$
 */
#if !defined lint && defined GDS_RCS_ID
static char rcsid[] = "@(#)$RCSfile: d24asn1.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:42:51 $";
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
/* NAME         : d24asn1.c                                           */
/*								      */
/* AUTHOR       : Praveen Gupta, SIEMENS INDIA.                       */
/* DATE         : 9. 1. 1990                                          */
/*								      */
/* DESCRIPTION  : This source-File which contains interface for       */
/*		  converting apdus to ASN1 encoded formats.           */
/*								      */
/* SYSTEM DEPENDENCIES:  COMMON                                       */
/*								      */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   0.1    | 90-01-09| Original                       | WS |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

#include <gds.h>
#include <stdlib.h>
#include <string.h>

#include <d2asn1.h>
#include <d2asn1_int.h>

/**********************************************************************/
/*                                                                    */
/*       D E C L A R A T I O N of data				      */
/*                                                                    */
/**********************************************************************/

#ifndef DSA
static char  null_res[]   = { (char)0x05, (char)0x00};
#endif /* of not defined DSA */

/******** LOCAL *********/

static void id_error_free ( signed32 , ids_error * );

#ifndef DSA

static signed32 d2a020_apdu_bind_arg_asn (byte *,signed32 *,asn1_field *,byte *);
static signed32 d2a022_apdu_read_arg_asn (byte *,signed32 *,asn1_field *,byte *);
static signed32 d2a065_apdu_rem_arg_asn (byte *,signed32 *,asn1_field *,byte *);
static signed32 d2a073_apdu_add_arg_asn (byte *,signed32 *,asn1_field *,byte *);
static signed32 d2a086_apdu_mod_arg_asn (byte *,signed32 *,asn1_field *,byte *);
static signed32 d2a087_apdu_list_arg_asn (byte *,signed32 *,asn1_field *,byte *);
static signed32 d2a089_apdu_search_arg_asn (byte *,signed32 *,asn1_field *,byte *);
static signed32 d2a091_apdu_comp_arg_asn (byte *,signed32 *,asn1_field *,byte *);
static signed32 d2a128_apdu_modrdn_asn (byte *,signed32 *,asn1_field *,byte *);
static signed32 d2a999_apdu_abandon_asn (byte *,signed32 *,asn1_field *,byte *);

static signed32 d2a041_asn_bind_res_apdu (byte **,signed32 *,asn1_field *,byte *);
static signed32 d2a043_asn_read_res_apdu (byte **,byte **,signed32 *,
							asn1_field *,byte *);
static signed32 d2a095_asn_list_res_apdu (byte **,byte **,signed32 *,
							asn1_field *,byte *);
static signed32 d2a097_asn_srch_res_apdu (byte **,byte **,signed32 *,
							asn1_field *,byte *);
static signed32 d2a099_asn_comp_res_apdu (byte **,byte **,signed32 *,
							asn1_field *,byte *);
static signed32 d2a046_asn_bind_error_apdu (byte **,signed32 *,asn1_field *,byte *);

#else /* if defined DSA */

static signed32 d2a165_dsa_referral_apdu (byte **,signed32 *,ids_dsa_referral *,byte *);

#endif 

#ifndef DSA

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
/*                                                                    */
/* OUTPUT-PARAMETER : asn1_cod: for returning the ASN1 encoded string */
/*                                                                    */
/* RETURN-VALUE :  D2_NOERROR                                         */
/*                 D2_ERROR                                           */
/*                                                                    */
/* exoff **************************************************************/

signed32 d2a000_apdu_asn1(
  signed16 op_code,
  byte * apdu,
  signed32 * apdu_len,
  asn1_field * asn1_cod,
  byte *err_apdu )
{
   signed32 res;

   D2_ASN1_TRACE_ENTER_ENCODING(op_code,D27_DAP_AS)

   if ( /* NULL RESULT */
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

	case D23_BIND :
		res = d2a020_apdu_bind_arg_asn(apdu,apdu_len,asn1_cod,err_apdu);
		break;

	case D23_READ :
	case (D23_READ + D2_ADM_OPID) :
		res = d2a022_apdu_read_arg_asn(apdu,apdu_len,asn1_cod,err_apdu);
		break;

	case D23_REMOBJT :
	case (D23_REMOBJT + D2_ADM_OPID) :
		res = d2a065_apdu_rem_arg_asn(apdu,apdu_len,asn1_cod,err_apdu);
		break;

	case D23_ADDOBJT :
	case (D23_ADDOBJT + D2_ADM_OPID) :
		res = d2a073_apdu_add_arg_asn(apdu,apdu_len,asn1_cod,err_apdu);
		break;

	case D23_MODENTR :
	case (D23_MODENTR + D2_ADM_OPID) :
		res = d2a086_apdu_mod_arg_asn(apdu,apdu_len,asn1_cod,err_apdu);
		break;

	case D23_LIST :
	case (D23_LIST + D2_ADM_OPID) :
		res = d2a087_apdu_list_arg_asn(apdu,apdu_len,asn1_cod,err_apdu);
		break;

	case D23_SEARCH :
	case (D23_SEARCH + D2_ADM_OPID) :
		res = d2a089_apdu_search_arg_asn(apdu,apdu_len,asn1_cod,
								err_apdu);
		break;

	case D23_COMPARE :
	case (D23_COMPARE + D2_ADM_OPID) :
		res = d2a091_apdu_comp_arg_asn(apdu,apdu_len,asn1_cod,
								err_apdu);
		break;

	case D23_REPLRDN :
	case (D23_REPLRDN + D2_ADM_OPID) :
		res = d2a128_apdu_modrdn_asn(apdu,apdu_len,asn1_cod,err_apdu);
		break;

	case D23_ABANDON :
		res = d2a999_apdu_abandon_asn(apdu,apdu_len,asn1_cod,err_apdu);
		break;

	default:
		res = D2_ERROR;
		break;

	}

   if  (res == D2_NOERROR) 
     {
     D2_ASN1_TRACE_ENCODED_MESSAGE(asn1_cod)
     }

   D2_ASN1_TRACE_EXIT_ENCODING(op_code,D27_DAP_AS,res)
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
/*                                                                    */
/*  OUTPUT-PARAMETER: apdu: pointer to the apdu                       */
/*                    apdu_len: length of the apdu                    */
/*                                                                    */
/* RETURN-VALUE :  D2_NOERROR                                         */
/*                 D2_ERROR                                           */
/*                                                                    */
/* exoff **************************************************************/

signed32 d2a001_asn1_apdu(
  signed16 op_code,
  byte ** apdu,
  signed32 *apdu_len,
  asn1_field * asn1_cod,
  byte *err_apdu )
{
const char function_name[] = "d2a001_asn1_apdu";
  signed32 res;
  byte * scan;

  D2_ASN1_TRACE_ENTER_DECODING(op_code,D27_DAP_AS)

  D2_ASN1_TRACE_ENCODED_MESSAGE(asn1_cod)

   *apdu_len = 0;

   if ( /* NULL RESULT */
	(op_code == (D23_ADDOBJT + D2_RES_OPID)) ||
	(op_code == (D23_ADDOBJT + D2_RES_OPID + D2_ADM_OPID)) ||
	(op_code == (D23_REMOBJT + D2_RES_OPID)) ||
	(op_code == (D23_REMOBJT + D2_RES_OPID + D2_ADM_OPID)) ||
	(op_code == (D23_MODENTR + D2_RES_OPID)) ||
	(op_code == (D23_MODENTR + D2_RES_OPID + D2_ADM_OPID)) ||
	(op_code == (D23_REPLRDN + D2_RES_OPID)) ||
	(op_code == (D23_REPLRDN + D2_RES_OPID + D2_ADM_OPID)) ||
	(op_code == (D23_UNBIND + D2_RES_OPID)) ||
	(op_code == D23_UNBIND) ||
	(op_code == (D23_ABANDON + D2_RES_OPID)) ||
	(op_code == (D23_ABANDON + D2_RES_OPID + D2_ADM_OPID))
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

  switch (op_code) {

	case (D23_BIND + D2_RES_OPID) :
		res = d2a041_asn_bind_res_apdu(apdu,apdu_len,asn1_cod,err_apdu);
		break;

	case (D23_READ + D2_RES_OPID) :
	case (D23_READ + D2_RES_OPID + D2_ADM_OPID) :
		res = d2a043_asn_read_res_apdu(&scan,apdu,apdu_len,
							asn1_cod,err_apdu);
		*apdu_len = scan - *apdu;
		break;

	case (D23_LIST + D2_RES_OPID) :
	case (D23_LIST + D2_RES_OPID + D2_ADM_OPID) :
		res = d2a095_asn_list_res_apdu(&scan,apdu,apdu_len,
							asn1_cod,err_apdu);
		*apdu_len = scan - *apdu;
		break;

	case (D23_SEARCH + D2_RES_OPID) :
	case (D23_SEARCH + D2_RES_OPID + D2_ADM_OPID) :
		res = d2a097_asn_srch_res_apdu(&scan,apdu,apdu_len,
							asn1_cod,err_apdu);
		*apdu_len = scan - *apdu;
		break;

	case (D23_COMPARE + D2_RES_OPID) :
	case (D23_COMPARE + D2_RES_OPID + D2_ADM_OPID) :
		res = d2a099_asn_comp_res_apdu(&scan,apdu,apdu_len,
							asn1_cod,err_apdu);
		*apdu_len = scan - *apdu;
		break;

	case (D23_BIND + D2_REJ_OPID) :
		res = d2a046_asn_bind_error_apdu(apdu,apdu_len,
							asn1_cod,err_apdu);
		break;

	default:
		res = D2_ERROR;
		break;
	}

   D2_ASN1_TRACE_EXIT_DECODING(op_code,D27_DAP_AS,res)
   return (res);
}

#endif /* if not defined DSA */

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* AUTHOR       :  Praveen Gupta, SIEMENS INDIA.                      */
/* DATE         :  09.01.90                                           */
/*                                                                    */
/* DESCRIPTION  : This procedure converts apdu format to ASN1 	      */
/* 		  encoded message (for BIND_ARG message).	      */
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

#ifndef DSA
static
#endif
signed32 d2a020_apdu_bind_arg_asn(
  byte * apdu,
  signed32 * apdu_len,
  asn1_field * asn1_cod,
  byte * err_apdu )
{
const char function_name[] = "d2a020_apdu_bind_arg_asn";
signed32 res;
signed32 bind_arg_len;
ids_bind_arg i_bind_arg;

D23_inbind * inbind = (D23_inbind *) apdu;
byte * scan = (byte *) apdu;

/* the version no. of apdu is CHECKED. */
if (inbind->d23_Aversno != D23_APDUV22)
  {
  d27_010_set_error_apdu((D23_rserror *)err_apdu,D2_PERM_ERR,
					D2_ILL_VERS,D2_NOT_SPEC, apdu_len);
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }
i_bind_arg.version = V1988;
scan += sizeof(D23_inbind);

switch (inbind->d23_Acrkind)
  {
  case D23_NO_CRED:
	i_bind_arg.cred_defined = FALSE;
	break;

  case D23_SIMPLE_CRED:
	i_bind_arg.cred_defined = TRUE;
	i_bind_arg.cred_kind = CREDENTIAL_SIMPLE;
		/* Validity is not supported, as yet */
	i_bind_arg.credentials.simple.validity_defined = FALSE;

		/* directory_id of apdu is ignored */
	scan += sizeof(D23_simcr_info);
	if ((res=d2a002_apdu_bind_arg_ids(scan,apdu_len,&i_bind_arg,err_apdu))
								== D2_ERROR)
	  {
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	  }
	break;

  case D23_EXTERNAL_CRED:
	i_bind_arg.cred_defined = TRUE;
	i_bind_arg.cred_kind = CREDENTIAL_EXTERNAL;

	if ((res=d2a600_ext_cred_to_asn1(scan,apdu_len,
		&(i_bind_arg.credentials.external),err_apdu)) == D2_ERROR)
	  {
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	  }

	break;

  case D23_STRONG_CRED:
	asn1_cod->l = ((D23_strcr_info *)scan)->d23_strlen;
	if ((asn1_cod->v= D27_MALLOC (svc_c_sev_warning, asn1_cod->l)) == NULL)
	  {
	  d27_010_set_error_apdu((D23_rserror *)err_apdu, D2_TEMP_ERR,
					D2_SYS_ERR,D21_ALLOC_ERROR,apdu_len);
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	  }

	scan += sizeof(D23_strcr_info);
	memcpy(asn1_cod->v,scan,asn1_cod->l);
	return(D2_NOERROR);

  default:
	d27_010_set_error_apdu((D23_rserror *)err_apdu,D2_CALL_ERR,
					D2_PAR_ERR,D2_INAPPR_AUTH, apdu_len);
	D2_ASN1_ERR_RETURN_WITH_TRACE
  } /* end of switch */
	
/* for strong credentials all work is done; for all other credential kinds: */

bind_arg_len =ids_bind_arg_len(-1,&i_bind_arg);
if ((asn1_cod->v= D27_MALLOC (svc_c_sev_warning,(size_t)bind_arg_len)) == NULL)
  {
  d27_010_set_error_apdu((D23_rserror *)err_apdu, D2_TEMP_ERR,
				D2_SYS_ERR,D21_ALLOC_ERROR,apdu_len);
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }

asn1_cod->l= ids_bind_arg_cod( asn1_cod->v,-1,0,&i_bind_arg) - asn1_cod->v;
i_bind_arg_free(&i_bind_arg);
return(D2_NOERROR);
} /* end of d2a020_apdu_bind_arg_asn */

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* AUTHOR       :  Praveen Gupta, SIEMENS INDIA.                      */
/* DATE         :  09.01.90                                           */
/*                                                                    */
/* DESCRIPTION  : This procedure converts apdu format to ASN1 	      */
/* 		  encoded message (for READ_ARG message).	      */
/*                                                                    */
/* INPUT-PARAMETER :  apdu: pointer to the apdu                       */
/*                    apdu_len: length of the apdu                    */
/*                                                                    */
/* OUTPUT-PARAMETER : asn1_cod: for returning the ASN1 encoded string  */
/*                                                                    */
/* RETURN-VALUE :  D2_NOERROR                                         */
/*                 D2_ERROR                                           */
/*                                                                    */
/* exoff **************************************************************/

#ifndef DSA
static
#endif
signed32 d2a022_apdu_read_arg_asn(
  byte * apdu,
  signed32 * apdu_len,
  asn1_field * asn1_cod,
  byte * err_apdu )
{
const char function_name[] = "d2a022_apdu_read_arg_asn";
    signed32 res;
    signed32 read_arg_len;
    ids_read_arg i_read_arg;
    ids_common_arg i_com_arg;

	if ((res=d2a006_apdu_read_arg_ids(apdu,apdu_len,
				&i_read_arg,&i_com_arg,err_apdu))==D2_ERROR)
		{
		D2_ASN1_ERR_RETURN_WITH_TRACE
		}
	else {
		read_arg_len =ids_read_arg_len(-1,&i_read_arg,&i_com_arg);
		if ((asn1_cod->v= D27_MALLOC (svc_c_sev_warning,read_arg_len)) == NULL)
		 {
		 d27_010_set_error_apdu((D23_rserror *)err_apdu, D2_TEMP_ERR,
				D2_SYS_ERR,D21_ALLOC_ERROR,apdu_len);
		  D2_ASN1_ERR_RETURN_WITH_TRACE
		   }
		asn1_cod->l= ids_read_arg_cod( asn1_cod->v,-1,0,
					&i_read_arg,&i_com_arg) - asn1_cod->v;
		 i_read_arg_free(&i_read_arg, &i_com_arg);
		return(D2_NOERROR);
		}
}
   
/* exon ***************************************************************/
/*                                                                    */
/* NAME         :  d2a041_asn_bind_res_apdu                           */
/*                                                                    */
/* AUTHOR       :  Praveen Gupta, SIEMENS INDIA.                      */
/* DATE         :  09.01.90                                           */
/*                                                                    */
/* DESCRIPTION  : This procedure converts ASN1 encoded messages to    */
/* 		  apdu formats.(for BIND_RES messages)	              */
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

#ifndef DSA
static
#endif
signed32 d2a041_asn_bind_res_apdu(
  byte ** apdu,
  signed32 * apdu_len,
  asn1_field * asn1_cod,
  byte * err_apdu )
{
const char function_name[] = "d2a041_asn_bind_res_apdu";
signed32 res;
ids_bind_res i_bind_res;
byte * scan;
D23_rsbind * rsbind;
D23_simcr_info * simcr;
D23_strcr_info * strcr;
signed32 needed_length;

/* assume a bind result will come; if we see later */
/* that we get a decoding error all will be reset and freed */

if ((scan = D27_MALLOC(svc_c_sev_warning,D2A_APDU_MEM)) == NULL)
  {
  d27_010_set_error_apdu((D23_rserror *)err_apdu, D2_TEMP_ERR,
					D2_SYS_ERR,D21_ALLOC_ERROR,apdu_len);
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }

*apdu = scan;
rsbind = (D23_rsbind *) scan;
rsbind->d23_Bhdr.d23_versno = D23_APDUV22;
rsbind->d23_Bhdr.d23_filler = 0;
rsbind->d23_Bhdr.d23_dirid = 0;
rsbind->d23_Bhdr.d23_bindid = 0;
rsbind->d23_Bversion = D23_V1988;
scan += sizeof(D23_rsbind);

if ( ids_bind_res_dec (asn1_cod->v,asn1_cod->v+asn1_cod->l,
							&i_bind_res) == 0)
  {
  d27_010_set_error_apdu((D23_rserror *)err_apdu, D2_TEMP_ERR,
				D2_REM_ERR,D2_NO_RESPONSE,apdu_len);
  ids_bind_res_free(&i_bind_res);
  free(*apdu);
  *apdu = NULL;
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }

if (i_bind_res.cred_defined == FALSE)
  {
  rsbind->d23_Bcrkind = D23_NO_CRED;
  }
else
  {
  switch (i_bind_res.cred_kind)
    {
    case CREDENTIAL_SIMPLE: /* simple credentials */
	rsbind->d23_Bcrkind = D23_SIMPLE_CRED;
	simcr = (D23_simcr_info *) scan;
	simcr->d23_scrtag = D23_SIMTAG;
	scan += sizeof(D23_simcr_info);
	if ((res=d2a004_ids_bind_res_apdu(&scan,apdu_len,
					&i_bind_res,err_apdu))==D2_ERROR)
	  {
	  ids_bind_res_free(&i_bind_res);
	  free(*apdu);
	  *apdu = NULL;
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	  }
	break;

    case CREDENTIAL_STRONG: /* strong credentials */
	rsbind->d23_Bcrkind = D23_STRONG_CRED;
	strcr = (D23_strcr_info *) scan;
	strcr->d23_strtag = D23_STRTAG;
	strcr->d23_strlen = asn1_cod->l;
	strcr->d23_stroff = D2_ALIGN(asn1_cod->l);
	scan += sizeof(D23_strcr_info);
	needed_length = sizeof(D23_rsbind) +
			sizeof(D23_strcr_info) +
			strcr->d23_stroff +
			sizeof(D23_eom);

	if (needed_length > D2A_APDU_MEM)
	  {
	  /* reallocate because of huge token */
	  if ((*apdu = D27_REALLOC(svc_c_sev_warning,*apdu,needed_length)) 
		== NULL)
	    {
	    d27_010_set_error_apdu((D23_rserror *)err_apdu, D2_TEMP_ERR,
					D2_SYS_ERR,D21_ALLOC_ERROR,apdu_len);
	    }

	  scan = *apdu + sizeof(D23_rsbind);
	  strcr = (D23_strcr_info *) scan;
	  scan += sizeof(D23_strcr_info);
	  } /* end of reallocation */

	memcpy(scan,asn1_cod->v,asn1_cod->l);
	scan += strcr->d23_stroff;
	break;

    case CREDENTIAL_EXTERNAL: /* external credentials */
	rsbind->d23_Bcrkind = D23_EXTERNAL_CRED;
	if ((res=d2a601_ext_cred_from_asn1(&scan,apdu_len,
		&(i_bind_res.credentials.external),err_apdu))==D2_ERROR)
	  {
	  ids_bind_res_free(&i_bind_res);
	  free(*apdu);
	  *apdu = NULL;
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	  }
	
	break;

    default: /* invalid credentials ? ? ? */
	D2_ASN1_ERR_RETURN_WITH_TRACE
    } /* end of switch for credential kind */
  } /* end of if-else credentials defined */
	
((D23_eom *) scan)->d23_eomtag = D23_EOMTAG;
((D23_eom *) scan)->d23_eomfil = D2_ALIGN(D23_EOMTAG);
*apdu_len = (scan + sizeof(D23_eom)) - *apdu;

ids_bind_res_free(&i_bind_res);
return (D2_NOERROR);
} /* end of d2a041_asn_bind_res_apdu */

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* AUTHOR       :  Praveen Gupta, SIEMENS INDIA                       */
/* DATE         :  09.01.90                                           */
/*                                                                    */
/* DESCRIPTION  : This procedure converts ASN1 encoded messages to    */
/* 		  apdu formats.(for READ_RESULT messages)	      */
/*                                                                    */
/* INPUT-PARAMETER :   asn1_cod: for inputing the ASN1 encoded string */
/*                                                                    */
/* OUTPUT-PARAMETER : apdu: pointer to the apdu                       */
/*                    apdu_len: length of the apdu                    */
/*                                                                    */
/* RETURN-VALUE :  D2_NOERROR                                         */
/*                 D2_ERROR                                           */
/* exoff **************************************************************/

#ifndef DSA
static
#endif
signed32 d2a043_asn_read_res_apdu(
  byte ** act_apdu,
  byte ** apdu,
  signed32 * apdu_len,
  asn1_field * asn1_cod,
  byte * err_apdu )
{
const char function_name[] = "d2a043_asn_read_res_apdu";
    signed32 res;
    ids_read_res i_read_res;
    ids_common_res i_com_res;

	if ( ids_read_res_dec (asn1_cod->v,asn1_cod->v+asn1_cod->l,
				&i_read_res,&i_com_res) == 0)
		{
		 d27_010_set_error_apdu((D23_rserror *)err_apdu, D2_TEMP_ERR,
					D2_REM_ERR,D2_NO_RESPONSE,apdu_len);
		 ids_read_res_free(&i_read_res,&i_com_res);
		 D2_ASN1_ERR_RETURN_WITH_TRACE
		}
	 else
	   	{
		   if ((res=d2a008_ids_read_res_apdu(act_apdu,apdu,apdu_len,
					&i_read_res,&i_com_res,err_apdu))==D2_ERROR)
			{
			D2_ASN1_ERR_RETURN_WITH_TRACE
			}
		   ids_read_res_free(&i_read_res,&i_com_res);
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
/* DESCRIPTION  : This procedure converts ASN1 encoded messages to    */
/* 		  apdu formats.(for BIND_ERROR messages)	      */
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

#ifndef DSA
static
#endif
signed32 d2a046_asn_bind_error_apdu(
  byte ** apdu,
  signed32 * apdu_len,
  asn1_field * asn1_cod,
  byte * err_apdu )
{
const char function_name[] = "d2a046_asn_bind_error_apdu";
    signed32 res;
    ids_bind_error i_bind_error;

    if (asn1_cod->l == 0)
      {
      D2_ASN1_ERR_RETURN_WITH_TRACE
      }

	if ( ids_bind_error_dec (asn1_cod->v,asn1_cod->v+asn1_cod->l,&i_bind_error) == 0)
		{
		 d27_010_set_error_apdu((D23_rserror *)err_apdu, D2_TEMP_ERR,
					D2_REM_ERR,D2_NO_RESPONSE,apdu_len);
		 D2_ASN1_ERR_RETURN_WITH_TRACE
		}
	 else
	   	{

		   if ((res=d2a012_ids_bind_error_apdu(apdu,apdu_len,
					&i_bind_error,err_apdu))==D2_ERROR)
			{
			D2_ASN1_ERR_RETURN_WITH_TRACE
			}
		   ids_bind_error_free(&i_bind_error);
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
/* DESCRIPTION  : This procedure converts ASN1 encoded messages to    */
/* 		  apdu formats.(for ERROR messages)	 	      */
/*                                                                    */
/* RETURN-VALUE :  D2_NOERROR                                         */
/*                 D2_ERROR                                           */
/*                                                                    */
/* exoff **************************************************************/


signed32 d2a047_asn_error_apdu(
  byte ** apdu,
  signed32 * apdu_len,
  signed32 error_cod,
  signed16 abs_id,
  asn1_field * asn1_buf,
  byte * err_apdu )
{
const char function_name[] = "d2a047_asn_error_apdu";
ids_error 	d_error;
#ifdef DSA
ids_dsa_referral d_referral;
#endif
   
D2_ASN1_TRACE_ENTER_ERROR_DECODING(error_cod,abs_id);
   
D2_ASN1_TRACE_ENCODED_MESSAGE(asn1_buf)

#ifdef DSA
   if (abs_id == D27_DSP_AS && error_cod == SYS_DSA_REFERRAL)
     {
	if (ids_dsa_referral_dec (asn1_buf->v, asn1_buf->v+asn1_buf->l,
			      &d_referral) == 0)
	{
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	 }
	
	if (d2a165_dsa_referral_apdu(apdu,apdu_len,&d_referral,err_apdu)
                                                             == D2_ERROR)
	  { 
	    i_dsa_referral_free(&d_referral); 
	    D2_ASN1_ERR_RETURN_WITH_TRACE
	   }
        else
	  { 
	    i_dsa_referral_free(&d_referral); 
	    return (D2_NOERROR);
	   }
       }
#endif /* DSA */

   switch (d_error.error_code = error_cod)
     {
	case ATT_ERROR:
	{
	  if (ids_att_error_dec (asn1_buf->v, asn1_buf->v+asn1_buf->l,
			     &d_error.contents.att) == 0)
		{
		 D2_ASN1_ERR_RETURN_WITH_TRACE
		}
	  break;
	}
	case NAME_ERROR:
	{
	  if (ids_name_error_dec (asn1_buf->v, asn1_buf->v+asn1_buf->l,
			      &d_error.contents.name) == 0)
		{
		 D2_ASN1_ERR_RETURN_WITH_TRACE
		}
	  break;
	}
	case SERV_ERROR:
	{
	  if (ids_serv_error_dec (asn1_buf->v, asn1_buf->v+asn1_buf->l,
			      &d_error.contents.serv) == 0)
		{
		 D2_ASN1_ERR_RETURN_WITH_TRACE
		}
	  break;
	}
	case REFERRAL:
	{
	  if (ids_referral_dec (asn1_buf->v, asn1_buf->v+asn1_buf->l,
			      &d_error.contents.ref) == 0)
		{
		 D2_ASN1_ERR_RETURN_WITH_TRACE
		}
	  break;
	}
	case ABAN_ERROR:
	{
	  /* no semantic message */
	  break;
	}
	case SEC_ERROR:
	{
	  if (ids_sec_error_dec (asn1_buf->v, asn1_buf->v+asn1_buf->l,
			      &d_error.contents.sec) == 0)
		{
		 D2_ASN1_ERR_RETURN_WITH_TRACE
		}
	  break;
	}
	case ABAN_FAILED:
	{
	  ids_aban_failed_dec (asn1_buf->v, asn1_buf->v+asn1_buf->l,
			      &d_error.contents.aban_failed);
	  break;
	}
	case UPDATE_ERROR:
	{
	  if (ids_upd_error_dec (asn1_buf->v, asn1_buf->v+asn1_buf->l,
			      &d_error.contents.upd) == 0)
		{
		 D2_ASN1_ERR_RETURN_WITH_TRACE
		}
	  break;
	}
    }

    if (d2a014_error_dec_apdu(apdu,apdu_len,&d_error,err_apdu) == D2_ERROR)
	  {
	  id_error_free(error_cod,&d_error); 
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	  }
        else
	  {
	  id_error_free(error_cod,&d_error); 
	  D2_ASN1_TRACE_EXIT_ERROR_DECODING(error_cod,abs_id,D2_NOERROR)
	  return (D2_NOERROR);
	  }

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

static void id_error_free (
  signed32 error_code,
  ids_error * error_buf )
{
	switch (error_code)
	{
	case ATT_ERROR:
	  ids_att_error_free (&(error_buf->contents.att));
	  break;
	case NAME_ERROR:
	  ids_name_error_free ( &(error_buf->contents.name));
	  break;
	case REFERRAL:
	  ids_cont_reference_free (&(error_buf->contents.ref.candidate));
	  break;
	}
}

#ifdef DSA
/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* AUTHOR       :  Praveen Gupta, SIEMENS INDIA.                      */
/* DATE         :  09.01.90                                           */
/*                                                                    */
/* DESCRIPTION  : This procedure converts dsa_referral structure      */
/* 		  to APDU formats.(DSA_REFERRAL_ERROR messages)	      */
/*                                                                    */
/* RETURN-VALUE :  D2_NOERROR                                         */
/*                 D2_ERROR                                           */
/*                                                                    */
/* exoff **************************************************************/

static signed32 d2a165_dsa_referral_apdu(
  byte 	**apdu,
  signed32    	 *apdu_len,
  ids_dsa_referral *dsa_ref,
  byte  	 *err_apdu )
{
const char function_name[] = "d2a165_dsa_referral_apdu";
   signed32  max_len;
   D23_rserror * rs_error;
   byte * scan;

    if ((scan = D27_MALLOC(svc_c_sev_warning,D2A_APDU_MEM))==NULL)
	{
	  d27_010_set_error_apdu((D23_rserror *)err_apdu,D2_TEMP_ERR,
		D2_SYS_ERR,D21_ALLOC_ERROR, apdu_len);
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	}
    *apdu = scan;
    max_len = D2A_APDU_MEM;
    rs_error = (D23_rserror *) scan;
    rs_error->d23_Zretcod = D2_CALL_ERR;
    rs_error->d23_Zerrcls = D2_PAR_ERR;
    rs_error->d23_Zerrval = D2_REF_ERROR;
    rs_error->d23_Zversion = D23_V1988;

    scan += sizeof(D23_rserror);
    ((D23_drf_info *)scan)->d23_drftag = D23_DRFTAG;
    ((D23_drf_info *)scan)->d23_drf_max_tr =
	(dsa_ref->trace_infos_defined == TRUE? dsa_ref->trace_infos.nb : 0);
    scan += D23_S_DREF;
    if (d2a053_put_ref(&dsa_ref->candidate,
	  &scan, apdu, &max_len, err_apdu, apdu_len)== D2_ERROR)
      {
        free( *apdu);
        D2_ASN1_ERR_RETURN_WITH_TRACE
      }

    if (dsa_ref->trace_infos_defined == TRUE)
      {
      if (d2a162_tr_info_apdu(&scan, &dsa_ref->trace_infos,apdu, &max_len,
			     err_apdu, apdu_len) == D2_ERROR)
        {
	D2_ASN1_ERR_RETURN_WITH_TRACE
        }
      }

     ((D23_eom *)scan)->d23_eomtag = D23_EOMTAG;
     scan += D23_S_EOM;
     *apdu_len = scan - *apdu;

     return(D2_NOERROR); 

}
#endif

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* AUTHOR       :  Helmut Volpers                                     */
/* DATE         :  26.02.90                                           */
/*                                                                    */
/* DESCRIPTION  : This procedure converts apdu format to ASN1 	      */
/*                encoded message (for REMOVE_ARG message).           */
/*                                                                    */
/* INPUT-PARAMETER :  apdu: pointer to the apdu                       */
/*                    apdu_len: length of the apdu                    */
/*                                                                    */
/* OUTPUT-PARAMETER : asn1_cod: for returning the ASN1 encoded string  */
/*                                                                    */
/* RETURN-VALUE :  D2_NOERROR                                         */
/*                 D2_ERROR                                           */
/*                                                                    */
/* exoff **************************************************************/

#ifndef DSA
static
#endif
signed32 d2a065_apdu_rem_arg_asn(
  byte * apdu,
  signed32 * apdu_len,
  asn1_field * asn1_cod,
  byte * err_apdu )
{
const char function_name[] = "d2a065_apdu_rem_arg_asn";
    signed32 res;
    signed32 remove_arg_len;
    ids_remove_arg i_remove_arg;
    ids_common_arg i_com_arg;

	if ((res=d2a067_apdu_rem_arg_ids(apdu,apdu_len,
				&i_remove_arg,&i_com_arg,err_apdu))==D2_ERROR)
		{
		D2_ASN1_ERR_RETURN_WITH_TRACE
		}
	else {
		remove_arg_len =ids_remove_arg_len(-1,&i_remove_arg,&i_com_arg);
		if ((asn1_cod->v= D27_MALLOC (svc_c_sev_warning,remove_arg_len)) == NULL)
		 {
		 d27_010_set_error_apdu((D23_rserror *)err_apdu, D2_TEMP_ERR,
				D2_SYS_ERR,D21_ALLOC_ERROR,apdu_len);
		 D2_ASN1_ERR_RETURN_WITH_TRACE
		   }
		asn1_cod->l= ids_remove_arg_cod( asn1_cod->v,-1,0,
					&i_remove_arg,&i_com_arg) - asn1_cod->v;
		 i_remove_arg_free(&i_remove_arg, &i_com_arg);
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
/* DESCRIPTION  : This procedure converts apdu format to ASN1 	      */
/*                encoded message (for ADD_ARG message).              */
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

#ifndef DSA
static
#endif
signed32 d2a073_apdu_add_arg_asn(
  byte * apdu,
  signed32 * apdu_len,
  asn1_field * asn1_cod,
  byte * err_apdu )
{
const char function_name[] = "d2a073_apdu_add_arg_asn";
    signed32 res;
    signed32 add_arg_len;
    ids_add_arg i_add_arg;
    ids_common_arg i_com_arg;

	if ((res=d2a075_apdu_add_arg_ids(apdu,apdu_len,
				&i_add_arg,&i_com_arg,err_apdu))==D2_ERROR)
		{
		D2_ASN1_ERR_RETURN_WITH_TRACE
		}
	else {
		add_arg_len =ids_add_arg_len(-1,&i_add_arg,&i_com_arg);
		if ((asn1_cod->v= D27_MALLOC (svc_c_sev_warning,add_arg_len)) == NULL)
		 {
		 d27_010_set_error_apdu((D23_rserror *)err_apdu, D2_TEMP_ERR,
				D2_SYS_ERR,D21_ALLOC_ERROR,apdu_len);
		 D2_ASN1_ERR_RETURN_WITH_TRACE
		   }
		asn1_cod->l= ids_add_arg_cod( asn1_cod->v,-1,0,
					&i_add_arg,&i_com_arg) - asn1_cod->v;
		 i_add_arg_free(&i_add_arg,&i_com_arg);
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
/*                ASN1 formats.(for MODIFY_ARG messages)              */
/*                                                                    */
/* INPUT-PARAMETER :  apdu: pointer to the apdu                       */
/*                    apdu_len: length of the apdu                    */
/*                    err_apdu: error apdu only in the case of error  */
/*                                                                    */
/* OUTPUT-PARAMETER :  asn1_cod: for inputing the ASN1 encoded string */
/*                                                                    */
/* RETURN-VALUE :  D2_NOERROR                                         */
/*                 D2_ERROR                                           */
/*                                                                    */
/* exoff **************************************************************/

#ifndef DSA
static
#endif
signed32 d2a086_apdu_mod_arg_asn(
  byte * apdu,
  signed32 * apdu_len,
  asn1_field * asn1_cod,
  byte * err_apdu )
{
const char function_name[] = "d2a086_apdu_mod_arg_asn";
    signed32 res;
    signed32 modify_arg_len;
    ids_modify_arg i_modify_arg;
    ids_common_arg i_com_arg;

	if ((res=d2a104_apdu_modify_arg_ids(apdu,apdu_len,
				&i_modify_arg,&i_com_arg,err_apdu))==D2_ERROR)
		{
		D2_ASN1_ERR_RETURN_WITH_TRACE
		}
	else {
		modify_arg_len =ids_modify_arg_len(-1,&i_modify_arg,&i_com_arg);
		if ((asn1_cod->v= D27_MALLOC (svc_c_sev_warning,modify_arg_len)) == NULL)
		 {
		 d27_010_set_error_apdu((D23_rserror *)err_apdu, D2_TEMP_ERR,
				D2_SYS_ERR,D21_ALLOC_ERROR,apdu_len);
		 D2_ASN1_ERR_RETURN_WITH_TRACE
		   }
		asn1_cod->l= ids_modify_arg_cod( asn1_cod->v,-1,0,
					&i_modify_arg,&i_com_arg) - asn1_cod->v;
		 i_modify_arg_free(&i_modify_arg, &i_com_arg);
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
/*                ASN1 formats.(for LIST_ARG messages)                */
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

#ifndef DSA
static
#endif
signed32 d2a087_apdu_list_arg_asn(
  byte * apdu,
  signed32 * apdu_len,
  asn1_field * asn1_cod,
  byte * err_apdu )
{
const char function_name[] = "d2a087_apdu_list_arg_asn";
    signed32 res;
    signed32 list_arg_len;
    ids_list_arg i_list_arg;
    ids_common_arg i_com_arg;

	if ((res=d2a100_apdu_list_arg_ids(apdu,apdu_len,
				&i_list_arg,&i_com_arg,err_apdu))==D2_ERROR)
		{
		D2_ASN1_ERR_RETURN_WITH_TRACE
		}
	else {
		list_arg_len =ids_list_arg_len(-1,&i_list_arg,&i_com_arg);
		if ((asn1_cod->v= D27_MALLOC (svc_c_sev_warning,list_arg_len)) == NULL)
		 {
		 d27_010_set_error_apdu((D23_rserror *)err_apdu, D2_TEMP_ERR,
				D2_SYS_ERR,D21_ALLOC_ERROR,apdu_len);
		 D2_ASN1_ERR_RETURN_WITH_TRACE
		   }
		asn1_cod->l= ids_list_arg_cod( asn1_cod->v,-1,0,
					&i_list_arg,&i_com_arg) - asn1_cod->v;
		i_list_arg_free(&i_list_arg, &i_com_arg);
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
/*                ASN1 formats.(for SEARCH_ARG messages)              */
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

#ifndef DSA
static
#endif
signed32 d2a089_apdu_search_arg_asn(
  byte * apdu,
  signed32 * apdu_len,
  asn1_field * asn1_cod,
  byte * err_apdu )
{
const char function_name[] = "d2a089_apdu_search_arg_asn";
    signed32 res;
    signed32 search_arg_len;
    ids_search_arg i_search_arg;
    ids_common_arg i_com_arg;

	if ((res=d2a069_apdu_search_arg_ids(apdu,apdu_len,
				&i_search_arg,&i_com_arg,err_apdu))==D2_ERROR)
		{
		D2_ASN1_ERR_RETURN_WITH_TRACE
		}
	else {
		search_arg_len =ids_search_arg_len(-1,&i_search_arg,&i_com_arg);
		if ((asn1_cod->v= D27_MALLOC (svc_c_sev_warning,search_arg_len)) == NULL)
		 {
		 d27_010_set_error_apdu((D23_rserror *)err_apdu, D2_TEMP_ERR,
				D2_SYS_ERR,D21_ALLOC_ERROR,apdu_len);
		 D2_ASN1_ERR_RETURN_WITH_TRACE
		   }
		asn1_cod->l= ids_search_arg_cod( asn1_cod->v,-1,0,
					&i_search_arg,&i_com_arg) - asn1_cod->v;
		 i_search_arg_free(&i_search_arg, &i_com_arg);
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
/*                ASN1 formats.(for COMP_ARG messages)                */
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

#ifndef DSA
static
#endif
signed32 d2a091_apdu_comp_arg_asn(
  byte * apdu,
  signed32 * apdu_len,
  asn1_field * asn1_cod,
  byte * err_apdu )
{
const char function_name[] = "d2a091_apdu_comp_arg_asn";
    signed32 res;
    signed32 compare_arg_len;
    ids_compare_arg i_compare_arg;
    ids_common_arg i_com_arg;

	if ((res=d2a082_apdu_compare_arg_ids(apdu,apdu_len,
				&i_compare_arg,&i_com_arg,err_apdu))==D2_ERROR)
		{
		D2_ASN1_ERR_RETURN_WITH_TRACE
		}
	else {
		compare_arg_len =ids_compare_arg_len(-1,&i_compare_arg,&i_com_arg);
		if ((asn1_cod->v= D27_MALLOC (svc_c_sev_warning,compare_arg_len)) == NULL)
		 {
		 d27_010_set_error_apdu((D23_rserror *)err_apdu, D2_TEMP_ERR,
				D2_SYS_ERR,D21_ALLOC_ERROR,apdu_len);
		 D2_ASN1_ERR_RETURN_WITH_TRACE
		   }
		asn1_cod->l= ids_compare_arg_cod( asn1_cod->v,-1,0,
					&i_compare_arg,&i_com_arg) - asn1_cod->v;
		 i_compare_arg_free(&i_compare_arg, &i_com_arg);
		return(D2_NOERROR);
		}
}

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* AUTHOR       :  Praveen Gupta, SIEMENS INDIA                       */
/* DATE         :  09.01.90                                           */
/*                                                                    */
/* DESCRIPTION  : This procedure converts ASN1 encoded messages to    */
/* 		  apdu formats.(for LIST_RESULT messages)	      */
/*                                                                    */
/* INPUT-PARAMETER :   asn1_cod: for inputing the ASN1 encoded string */
/*                                                                    */
/* OUTPUT-PARAMETER : apdu: pointer to the apdu                       */
/*                    apdu_len: length of the apdu                    */
/*                                                                    */
/* RETURN-VALUE :  D2_NOERROR                                         */
/*                 D2_ERROR                                           */
/* exoff **************************************************************/

#ifndef DSA
static
#endif
signed32 d2a095_asn_list_res_apdu(
  byte ** act_apdu,
  byte ** apdu,
  signed32 * apdu_len,
  asn1_field * asn1_cod,
  byte * err_apdu )
{
const char function_name[] = "d2a095_asn_list_res_apdu";
    signed32 res;
    ids_list_res i_list_res;
    ids_common_res i_com_res;

	if ( ids_list_res_dec (asn1_cod->v,asn1_cod->v+asn1_cod->l,
				&i_list_res,&i_com_res) == 0)
		{
		 d27_010_set_error_apdu((D23_rserror *)err_apdu, D2_TEMP_ERR,
					D2_REM_ERR,D2_NO_RESPONSE,apdu_len);
		 ids_list_res_free(&i_list_res,&i_com_res);
		 D2_ASN1_ERR_RETURN_WITH_TRACE
		}
	 else
	   	{
		   if ((res=d2a103_ids_list_res_apdu(act_apdu,apdu,apdu_len,
					&i_list_res,&i_com_res,err_apdu))==D2_ERROR)
			{
			D2_ASN1_ERR_RETURN_WITH_TRACE
			}
		   ids_list_res_free(&i_list_res, &i_com_res);
		   return(D2_NOERROR);
		}
}

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* AUTHOR       :  Praveen Gupta, SIEMENS INDIA                       */
/* DATE         :  09.01.90                                           */
/*                                                                    */
/* DESCRIPTION  : This procedure converts ASN1 encoded messages to    */
/* 		  apdu formats.(for SEARCH_RESULT messages)	      */
/*                                                                    */
/* INPUT-PARAMETER :   asn1_cod: for inputing the ASN1 encoded string */
/*                                                                    */
/* OUTPUT-PARAMETER : apdu: pointer to the apdu                       */
/*                    apdu_len: length of the apdu                    */
/*                                                                    */
/* RETURN-VALUE :  D2_NOERROR                                         */
/*                 D2_ERROR                                           */
/* exoff **************************************************************/

#ifndef DSA
static
#endif
signed32 d2a097_asn_srch_res_apdu(
  byte ** act_apdu,
  byte ** apdu,
  signed32 * apdu_len,
  asn1_field * asn1_cod,
  byte * err_apdu )
{
const char function_name[] = "d2a097_asn_srch_res_apdu";
    signed32 res;
    ids_search_res i_search_res;
    ids_common_res i_com_res;

	if ( ids_search_res_dec (asn1_cod->v,asn1_cod->v+asn1_cod->l,
				&i_search_res,&i_com_res) == 0)
		{
		 d27_010_set_error_apdu((D23_rserror *)err_apdu, D2_TEMP_ERR,
					D2_REM_ERR,D2_NO_RESPONSE,apdu_len);
		 ids_search_res_free(&i_search_res,&i_com_res);
		 D2_ASN1_ERR_RETURN_WITH_TRACE
		}
	 else
	   	{
		   if ((res=d2a071_ids_search_res_apdu(act_apdu,apdu,apdu_len,
					&i_search_res,&i_com_res,err_apdu))==D2_ERROR)
			{
			D2_ASN1_ERR_RETURN_WITH_TRACE
			}
		   ids_search_res_free(&i_search_res, &i_com_res);
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
/*                ASN1 formats.(for MODRDN messages)                  */
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

#ifndef DSA
static
#endif
signed32 d2a128_apdu_modrdn_asn(
  byte * apdu,
  signed32   * apdu_len,
  asn1_field * asn1_cod,
  byte * err_apdu )
{
const char function_name[] = "d2a128_apdu_modrdn_asn";
    signed32 res;
    ids_modify_rdn_arg i_modrdn_arg;
    ids_common_arg     i_com_arg;
    signed32                modrdn_len;

	if ((res = d2a127_modrdn_arg_ids(apdu,apdu_len,
		    &i_modrdn_arg, &i_com_arg, err_apdu))==D2_ERROR)
		{
		D2_ASN1_ERR_RETURN_WITH_TRACE
		}
	else {
		modrdn_len = ids_modify_rdn_arg_len(-1, &i_modrdn_arg,
			      &i_com_arg);
		if ((asn1_cod->v= D27_MALLOC (svc_c_sev_warning,modrdn_len)) == NULL)
		 {
		 d27_010_set_error_apdu((D23_rserror *)err_apdu, D2_TEMP_ERR,
				D2_SYS_ERR,D21_ALLOC_ERROR,apdu_len);
		 D2_ASN1_ERR_RETURN_WITH_TRACE
		   }
		asn1_cod->l= ids_modify_rdn_arg_cod( asn1_cod->v,-1,0,
				&i_modrdn_arg, &i_com_arg) - asn1_cod->v;
		 i_modify_rdn_arg_free(&i_modrdn_arg, &i_com_arg);
		return(D2_NOERROR);
		}
}

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* AUTHOR       :  Praveen Gupta, SIEMENS INDIA                       */
/* DATE         :  09.01.90                                           */
/*                                                                    */
/* DESCRIPTION  : This procedure converts ASN1 encoded messages to    */
/* 		  apdu formats.(for COMP_RESULT messages)	      */
/*                                                                    */
/* INPUT-PARAMETER :   asn1_cod: for inputing the ASN1 encoded string */
/*                                                                    */
/* OUTPUT-PARAMETER : apdu: pointer to the apdu                       */
/*                    apdu_len: length of the apdu                    */
/*                                                                    */
/* RETURN-VALUE :  D2_NOERROR                                         */
/*                 D2_ERROR                                           */
/* exoff **************************************************************/

#ifndef DSA
static
#endif
signed32 d2a099_asn_comp_res_apdu(
  byte ** cur_apdu,
  byte ** apdu,
  signed32 * apdu_len,
  asn1_field * asn1_cod,
  byte * err_apdu )
{
const char function_name[] = "d2a099_asn_comp_res_apdu";
    signed32 res;
    ids_compare_res i_compare_res;
    ids_common_res i_com_res;

	if ( ids_compare_res_dec (asn1_cod->v,asn1_cod->v+asn1_cod->l,
				&i_compare_res,&i_com_res) == 0)
		{
		 d27_010_set_error_apdu((D23_rserror *)err_apdu, D2_TEMP_ERR,
					D2_REM_ERR,D2_NO_RESPONSE,apdu_len);
		 ids_compare_res_free(&i_compare_res,&i_com_res);
		 D2_ASN1_ERR_RETURN_WITH_TRACE
		}
	 else
	   	{
		   if ((res=d2a084_ids_compare_res_apdu(cur_apdu,apdu,apdu_len,
					&i_compare_res,&i_com_res,err_apdu))==D2_ERROR)
			{
			D2_ASN1_ERR_RETURN_WITH_TRACE
			}
		   ids_compare_res_free(&i_compare_res, &i_com_res);
		   return(D2_NOERROR);
		}
}

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* AUTHOR       :  Jochen Keutel                                      */
/* DATE         :  24.03.94                                           */
/*                                                                    */
/* DESCRIPTION  : This procedure converts apdu encoded messages to    */
/*                ASN1 formats.(for ABANDON messages)                 */
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

#ifndef DSA
static
#endif
signed32 d2a999_apdu_abandon_asn(
  byte * apdu,
  signed32   * apdu_len,
  asn1_field * asn1_cod,
  byte * err_apdu )
{
const char function_name[] = "d2a999_apdu_abandon_asn";
D23_inabandon * inabandon;
int invoke_id;
signed32 inabandon_len;

inabandon = (D23_inabandon *) apdu;
invoke_id = inabandon->d23_Rinv_id;

inabandon_len = ids_abandon_arg_len(-1, invoke_id);
if ((asn1_cod->v= D27_MALLOC (svc_c_sev_warning,inabandon_len)) == NULL)
  {
  d27_010_set_error_apdu((D23_rserror *)err_apdu, D2_TEMP_ERR,
				D2_SYS_ERR,D21_ALLOC_ERROR,apdu_len);
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }

asn1_cod->l= ids_abandon_arg_cod( asn1_cod->v,-1,0,invoke_id)  - asn1_cod->v;
return(D2_NOERROR);

}
