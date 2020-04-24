/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d25abind.c,v $
 * Revision 1.1.10.2  1996/02/18  19:42:54  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:23:45  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:23:59  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:53:35  root]
 * 
 * Revision 1.1.8.5  1994/07/06  15:06:39  marrek
 * 	July 1994 code drop.
 * 	[1994/07/06  10:00:26  marrek]
 * 
 * Revision 1.1.8.4  1994/06/21  14:44:20  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:26:58  marrek]
 * 
 * Revision 1.1.8.3  1994/05/10  15:50:33  marrek
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/02  13:23:56  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:15:22  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:08:21  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:15:15  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:51:02  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:42:03  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  10:49:56  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  12:34:40  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  18:12:49  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:10:12  bbelch]
 * 
 * Revision 1.1.4.3  1992/11/27  15:57:23  marrek
 * 	November 1992 code drop
 * 	[1992/11/17  12:50:25  marrek]
 * 
 * Revision 1.1.4.2  1992/09/28  12:07:37  marrek
 * 	Remove dummy functions and rename d_octet.nb to pw_info->d23_pwlen .
 * 	[1992/09/28  12:06:53  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:05:58  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:02:17  melman]
 * 
 * $EndLog$
 */
#if !defined lint && defined GDS_RCS_ID
static char rcsid[] = "@(#)$RCSfile: d25abind.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:42:54 $";
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
/* NAME         : d25abind.c                                          */
/*								      */
/* AUTHOR       : Praveen Gupta, SIEMENS INDIA                        */
/* DATE         : 9. 1. 1990                                          */
/*								      */
/* DESCRIPTION  : This C-File contains routines to transform BIND-apdu*/
/*                to (thorn)IDS Format.				      */
/*								      */
/* SYSTEM DEPENDENCIES:  COMMON                                       */
/*                                                                    */
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

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d2a003_ids_bind_arg_apdu()                         */
/*                                                                    */
/* AUTHOR       :  Praveen Gupta, Siemens India.                      */
/* DATE         :  22.01.90                                           */
/*                                                                    */
/* DESCRIPTION  :  Converts APDU-format of bind_arg to ids_format.    */
/*		   (used only for simple credentials)		      */
/*                                                                    */
/* INPUT-PARAMETER : byte * apdu;                                    */
/*                   signed32 * apdu_len;                                  */
/*                                                                    */
/* OUTPUT-PARAMETER: dua_bind_arg * bind_arg;                         */
/*                                                                    */
/* RETURN-VALUE :  D2_NOERROR                                         */
/*                 D2_ERROR                                           */
/*                                                                    */
/* exoff **************************************************************/

signed32 d2a003_ids_bind_arg_apdu(
  byte ** apdu,
  signed32 * apdu_len,
  ids_bind_arg * bind_arg )
{
byte * scan;
D23_pw_info    *pw_info;  /* pointer to Password block   */
			  /* in the message block        */
signed32 max_length;
byte er_apdu[D2A_ERR_APDU_SIZE];  /* ignored in the s_stub */
signed32  er_len;
const char function_name[] = "d2a003_ids_bind_arg_apdu";

max_length = D2A_APDU_MEM;
scan = *apdu;

if (d2a122_dn_apdu(D23_NAMTAG, &scan,
		   &(bind_arg->credentials.simple.name), apdu, &max_length,
		   er_apdu,&er_len) == D2_ERROR)
  {
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }

if (bind_arg->credentials.simple.password_defined == TRUE)
  {
  pw_info = (D23_pw_info *) scan;
  pw_info->d23_pwtag = D23_PWDTAG;
  pw_info->d23_pwlen = bind_arg->credentials.simple.password.nb;
  pw_info->d23_pwoff = D2_ALIGN(pw_info->d23_pwlen);
  if (pw_info->d23_pwlen > 0)
    {
    memcpy((char *)(pw_info +1),
    bind_arg->credentials.simple.password.contents, pw_info->d23_pwlen);
    }

  scan += sizeof(D23_pw_info) + pw_info->d23_pwoff;
  } /* of password */

return(D2_NOERROR);
}

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d2a005_apdu_bind_res_ids()                         */
/*                                                                    */
/* AUTHOR       :  Praveen Gupta, Siemens India.                      */
/* DATE         :  22.01.90                                           */
/*                                                                    */
/* DESCRIPTION  :  Converts APDU-format of bind_res to ids_format.    */
/*		   It's used only for simple credentials - this means */
/*		   the DSA name without password.		      */
/*                                                                    */
/* RETURN-VALUE :  D2_NOERROR                                         */
/*                 D2_ERROR                                           */
/*                                                                    */
/* exoff **************************************************************/

signed32 d2a005_apdu_bind_res_ids(
  byte * apdu,
  ids_bind_res * bind_res )
{
byte         err_apdu[D2A_ERR_APDU_SIZE];  /* dummy */
signed32           er_length; /* dummy */
byte * scan = apdu;

const char function_name[] = "d2a005_apdu_bind_res_ids";
                              /*  tag is Distinguished name         */
if (d2a121_dn_ids(&scan, &bind_res->credentials.simple.name,
	       err_apdu, &er_length) == D2_ERROR)
  {
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }

bind_res->credentials.simple.password_defined =FALSE;

return(D2_NOERROR);
} /* end of d2a005_apdu_bind_res_ids */

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d2a013_apdu_bind_error_ids()                       */
/*                                                                    */
/* AUTHOR       :  Praveen Gupta, Siemens India.                      */
/* DATE         :  22.01.90                                           */
/*                                                                    */
/* DESCRIPTION  :  Converts APDU-format of bind_error to ids_format.  */
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

signed32 d2a013_apdu_bind_error_ids(
  byte * apdu,
  signed32 apdu_len,
  ids_bind_error *bind_error )
{
byte * scan = apdu;
D23_rserror * rserror = (D23_rserror *) scan;
const char function_name[] = "d2a013_apdu_bind_error_ids";

if (rserror->d23_Zversion == D23_V1988)
       bind_error->version = V1988;

if ((rserror->d23_Zretcod == D2_TEMP_ERR)
	       && (rserror->d23_Zerrcls == D2_REM_ERR))
   {
   if (rserror->d23_Zerrval == D2_TOO_BUSY)
      {
      bind_error->is_service_error = TRUE;
      bind_error->pb.serv_pb.pb = BUSY; 
      return(D2_NOERROR);
      }
   if (rserror->d23_Zerrval == D2_NO_RESPONSE)
      {
      bind_error->is_service_error = TRUE;
      bind_error->pb.serv_pb.pb = UNAVAILABLE; 
      return(D2_NOERROR);
      }
   if (rserror->d23_Zerrval == D2_UNWILLING)
      {
      bind_error->is_service_error = TRUE;
      bind_error->pb.serv_pb.pb = UNWILL_TO_PERFORM; 
      return(D2_NOERROR);
      }
   D2_ASN1_ERR_RETURN_WITH_TRACE
   }
  else
   if ((rserror->d23_Zretcod == D2_CALL_ERR) && (rserror->d23_Zerrcls == D2_PAR_ERR))
    if (rserror->d23_Zerrval == D2_CREDENTIALS_INVALID)
      {
	bind_error->is_service_error = FALSE;
	bind_error->pb.sec_pb.pb = INV_CREDENTIALS;
				    /* sec_problem = invalidcredentials */
	return(D2_NOERROR);
	}
     else
	if (rserror->d23_Zerrval == D2_NOT_SPEC)
	  {
	    bind_error->is_service_error = TRUE;
	    bind_error->pb.serv_pb.pb = BUSY; /*(service_problem = busy */
	    return(D2_NOERROR);
	    }
	  else 
	    {
	    D2_ASN1_ERR_RETURN_WITH_TRACE
	    }
   else 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
}
