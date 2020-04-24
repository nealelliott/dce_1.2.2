/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d24abind.c,v $
 * Revision 1.1.10.2  1996/02/18  19:42:38  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:23:27  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:23:12  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:53:23  root]
 * 
 * Revision 1.1.8.5  1994/07/06  15:06:36  marrek
 * 	July 1994 code drop.
 * 	[1994/07/06  10:00:09  marrek]
 * 
 * Revision 1.1.8.4  1994/06/21  14:44:07  marrek
 * 	June 1994 code submission.
 * 	[1994/06/21  11:48:49  marrek]
 * 
 * Revision 1.1.8.3  1994/05/10  15:50:30  marrek
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/02  13:23:52  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:15:05  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:07:13  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:14:26  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:50:32  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:39:46  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  10:17:06  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  12:31:42  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  18:12:15  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:09:19  bbelch]
 * 
 * Revision 1.1.4.3  1992/11/27  15:53:45  marrek
 * 	November 1992 code drop
 * 	[1992/11/17  12:47:28  marrek]
 * 
 * Revision 1.1.4.2  1992/09/28  12:00:34  marrek
 * 	Remove dummy functions and add check for d_octet.nb .
 * 	[1992/09/28  11:59:25  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:04:20  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:01:05  melman]
 * 
 * $EndLog$
 */
#if !defined lint && defined GDS_RCS_ID
static char rcsid[] = "@(#)$RCSfile: d24abind.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:42:38 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton	***************************************************************/
/*								      */
/* TYP		: C-File					      */
/*								      */
/* NAME         : d24abind.c                                        */
/*								      */
/* AUTHOR       : Praveen Gupta, SIEMENS INDIA                        */
/* DATE         : 9. 1. 1990                                          */
/*								      */
/* DESCRIPTION  : This C-File contains routines to transform BIND-apdu*/
/*                to (thorn)IDS Format.				      */
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

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d2a002_apdu_bind_arg_ids()                         */
/*                                                                    */
/* AUTHOR       :  Praveen Gupta, Siemens India.                      */
/* DATE         :  22.01.90                                           */
/*                                                                    */
/* DESCRIPTION  :  Converts APDU-format of bind_arg to ids_format.    */
/*		   This function is called only for simple credentials */
/*                                                                    */
/* RETURN-VALUE :  D2_NOERROR                                         */
/*                 D2_ERROR                                           */
/*                                                                    */
/* exoff **************************************************************/

signed32 d2a002_apdu_bind_arg_ids(
  byte * apdu,
  signed32 * apdu_len,
  ids_bind_arg * bind_arg,
  byte * err_apdu )
{
   D23_ds_name      * ds_name;  /* pointer to distinguish name */
				/* in the message block        */
   D23_pw_info      * pw_info;  /* pointer to Password block   */
				/* in the message block        */
   dua_octet_string   d_octet;
   byte * scan = (byte *) apdu;
const char function_name[] = "d2a002_apdu_bind_arg_ids";

     ds_name = (D23_ds_name *) scan;  /* User name */

     if (d2a121_dn_ids(&scan, &bind_arg->credentials.simple.name,
	       err_apdu, apdu_len) == D2_ERROR) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}

   if                          /*  tag is not password tag           */
	    (((D23_pw_info *)scan)->d23_pwtag != D23_PWDTAG)
       {   
           bind_arg->credentials.simple.password_defined = FALSE;
	   
	   return(D2_NOERROR);
       } 

      pw_info = (D23_pw_info *) scan;
      d_octet.nb = pw_info->d23_pwlen;

      bind_arg->credentials.simple.password_defined =TRUE;
      if (d_octet.nb)
        {
        if ((d_octet.contents =
		   D27_MALLOC(svc_c_sev_warning,pw_info->d23_pwlen)) == NULL)
	   {
	      d27_010_set_error_apdu((D23_rserror *)err_apdu, D2_TEMP_ERR,
				       D2_SYS_ERR,D21_ALLOC_ERROR,apdu_len);
	      D2_ASN1_ERR_RETURN_WITH_TRACE
	     }
        memcpy(d_octet.contents,(char *)(pw_info + 1), pw_info->d23_pwlen);
        bind_arg->credentials.simple.password.nb = 0;
			/* to indicate that password has no memory */
        if (dua_octet_string_ids(&d_octet,
		&bind_arg->credentials.simple.password) != D2_NOERROR)
	  {
	  d27_010_set_error_apdu((D23_rserror *)err_apdu, D2_TEMP_ERR,
				D2_SYS_ERR,D21_ALLOC_ERROR,apdu_len);
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	  }
        free(d_octet.contents);
        }
      else
        {
        bind_arg->credentials.simple.password.nb = 0;
        }

return(D2_NOERROR);
}

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d2a004_ids_bind_res_apdu()                         */
/*                                                                    */
/* AUTHOR       :  Praveen Gupta, Siemens India.                      */
/* DATE         :  22.01.90                                           */
/*                                                                    */
/* DESCRIPTION  :  Converts ids-format of bind_res to APDU_format.    */
/*		   (used only for simple credentials)			*/
/*                                                                    */
/* INPUT-PARAMETER : byte * apdu;                                    */
/*                   signed32 * apdu_len;                                  */
/*                                                                    */
/* OUTPUT-PARAMETER: dua_bind_res * bind_res;                         */
/*                   byte * err_apdu; only in case of an error.      */
/*                                                                    */
/* RETURN-VALUE :  D2_NOERROR                                         */
/*                 D2_ERROR                                           */
/*                                                                    */
/* exoff **************************************************************/

signed32 d2a004_ids_bind_res_apdu(
  byte ** scan,
  signed32 * apdu_len,
  ids_bind_res * bind_res,
  byte * err_apdu )        /*     only in case of an error.      */
{
signed32              max_length;
const char function_name[] = "d2a004_ids_bind_res_apdu";

max_length = D2A_APDU_MEM;

if (d2a122_dn_apdu( D23_NAMTAG, scan,
	     &(bind_res->credentials.simple.name), scan, &max_length,
	     err_apdu, apdu_len) == D2_ERROR)
  { 
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }
	 /* No password is expected in bind results */

return(D2_NOERROR);
  }

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d2a012_ids_bind_error_apdu()                       */
/*                                                                    */
/* AUTHOR       :  Praveen Gupta, Siemens India.                      */
/* DATE         :  22.01.90                                           */
/*                                                                    */
/* DESCRIPTION  :  Converts ids-format of bind_error to APDU-format.  */
/*                                                                    */
/* INPUT-PARAMETER : byte * apdu;                                    */
/*                   signed32 apdu_len;                                    */
/*                                                                    */
/* OUTPUT-PARAMETER: ids_bind_res * bind_res;                         */
/*                                                                    */
/* RETURN-VALUE :  D2_NOERROR                                         */
/*                 D2_ERROR                                           */
/*                                                                    */
/* exoff **************************************************************/

signed32 d2a012_ids_bind_error_apdu(
  byte ** apdu,
  signed32 *apdu_len,
  ids_bind_error *bind_error,
  byte * err_apdu )        /*     only in case of an error.      */
{
byte       * scan;
D23_rserror * rserror;
const char function_name[] = "d2a012_ids_bind_error_apdu";

/* 20 more bytes are allocated just for extra space  */
scan = D27_MALLOC(svc_c_sev_warning,sizeof(D23_rserror)+sizeof(D23_eom)+20);
if (scan == NULL) 
  {
  d27_010_set_error_apdu((D23_rserror *)err_apdu, D2_TEMP_ERR,
				D2_SYS_ERR,D21_ALLOC_ERROR,apdu_len);
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }

*apdu = scan;
rserror = (D23_rserror *) scan;
rserror->d23_Zversion = D23_V1988;
if (bind_error->is_service_error == TRUE)
  {
  switch (bind_error->pb.serv_pb.pb)
       {
	case  BUSY :
		 d27_010_set_error_apdu((D23_rserror *)scan, D2_TEMP_ERR,
				D2_SYS_ERR,D2_TOO_BUSY,apdu_len);
		 break;
	case  UNAVAILABLE:
		 d27_010_set_error_apdu((D23_rserror *)scan, D2_TEMP_ERR,
				D2_SYS_ERR,D2_TOO_BUSY,apdu_len);
		 break;
	case UNWILL_TO_PERFORM:
		 d27_010_set_error_apdu((D23_rserror *)scan, D2_TEMP_ERR,
				D2_REM_ERR,D2_UNWILLING,apdu_len);
		 break;
	default :
		 d27_010_set_error_apdu((D23_rserror *)scan, D2_TEMP_ERR,
				D2_SYS_ERR,D2_NOT_SPEC,apdu_len);
	}
  }
else
		/* Security Error maps to invalid credentials for all cases*/
  {
  d27_010_set_error_apdu((D23_rserror *)scan, D2_TEMP_ERR,
				D2_SYS_ERR,D2_CREDENTIALS_INVALID,apdu_len);
  }

scan += sizeof(D23_rserror);
((D23_eom *) scan)->d23_eomtag = D23_EOMTAG;
((D23_eom *) scan)->d23_eomfil = D2_ALIGN(D23_EOMTAG);
*apdu_len = (scan + sizeof(D23_eom)) - *apdu;

return(D2_NOERROR);
}

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d2a600_ext_cred_to_asn1				*/
/*									*/
/* Description	: This function is used to convert external		*/
/*		  credentials from APDU format to ASN.1.		*/
/*									*/
/* Return Value	: D2_ERROR or D2_NOERROR				*/
/*									*/
/* Date         : April 13, 1994					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

d2_ret_val d2a600_ext_cred_to_asn1(
  byte * apdu,
  signed32 * apdu_len,
  asn1_field * encoded_ext_cred,
  byte * err_apdu )
{
const char function_name[] = "d2a600_ext_cred_to_asn1";
D23_extcr_info * extcr;
GdsExternal gds_external;
signed32 encoded_ext_cred_len;
asn1_field directReference;
asn1_field octetAligned;
extcr = (D23_extcr_info *)apdu;

if (extcr->d23_extkind != D2_DCE_AUTH)
  {
  d27_010_set_error_apdu((D23_rserror *)err_apdu,D2_CALL_ERR,
				D2_PAR_ERR,D2_INAPPR_AUTH, apdu_len);
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }

gds_external.directReference = &directReference;
gds_external.directReference->v = (asn1)D2_DCE_EXT_CRED;
gds_external.directReference->l = strlen(D2_DCE_EXT_CRED);

gds_external.indirectReference = NULL;
gds_external.dataValueDescriptor = NULL;

gds_external.encoding.v.octetAligned = &octetAligned;
gds_external.encoding.v.octetAligned->v = apdu + sizeof(D23_extcr_info);
gds_external.encoding.v.octetAligned->l = extcr->d23_extlen;
gds_external.encoding.x = 2; /* octetAligned */

encoded_ext_cred_len = GdsExternal_len(-1,&gds_external);
if ((encoded_ext_cred_len < 0) ||
	((encoded_ext_cred->v = D27_MALLOC (svc_c_sev_warning,
				encoded_ext_cred_len)) == NULL))
  {
  d27_010_set_error_apdu((D23_rserror *)err_apdu,D2_TEMP_ERR,
				D2_SYS_ERR,D21_ALLOC_ERROR, apdu_len);
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }

encoded_ext_cred->l = GdsExternal_cod(encoded_ext_cred->v,-1,0,
					&gds_external) - encoded_ext_cred->v;

return(D2_NOERROR);

}

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d2a601_ext_cred_from_asn1			*/
/*									*/
/* Description	: This function is used to convert external		*/
/*		  credentials from ASN.1 to APDU format.		*/
/*									*/
/* Return Value	: D2_ERROR or D2_NOERROR				*/
/*									*/
/* Date         : April 13, 1994					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

d2_ret_val d2a601_ext_cred_from_asn1(
  byte ** scan,
  signed32 * apdu_len,
  asn1_field * encoded_ext_cred,
  byte * err_apdu )        /*     only in case of an error.      */
{
const char function_name[] = "d2a601_ext_cred_from_asn1";
GdsExternal gds_external;
asn1_field * octetAligned;
D23_extcr_info * extcr;

if (GdsExternal_dec(encoded_ext_cred->v,
		encoded_ext_cred->v + encoded_ext_cred->l,&gds_external) == 0)
  {
  d27_010_set_error_apdu((D23_rserror *)err_apdu, D2_TEMP_ERR,
				 D2_REM_ERR,D2_NO_RESPONSE,apdu_len);
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }

octetAligned = gds_external.encoding.v.octetAligned;

extcr = (D23_extcr_info *) *scan;
extcr->d23_exttag = D23_STRTAG;
extcr->d23_extkind = D2_DCE_AUTH;
*scan += sizeof(D23_extcr_info);

extcr->d23_extlen = octetAligned->l;
if (octetAligned->l == 0)
  {
  extcr->d23_extoff = 0;
  }
else
  {
  extcr->d23_extoff = D2_ALIGN(extcr->d23_extlen);
  memcpy(*scan,(char *)octetAligned->v,extcr->d23_extoff);
  }

*scan += extcr->d23_extoff;

GdsExternal_free(&gds_external);

return(D2_NOERROR);
}
