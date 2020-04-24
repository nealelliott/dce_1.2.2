/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d26dsp.c,v $
 * Revision 1.1.10.2  1996/02/18  19:45:07  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:26:18  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:44:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:06:40  root]
 * 
 * Revision 1.1.8.6  1994/08/16  08:16:04  marrek
 * 	Fix for delta update (ot 11617).
 * 	[1994/08/12  12:00:03  marrek]
 * 
 * Revision 1.1.8.5  1994/07/06  15:06:55  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:13:07  marrek]
 * 
 * Revision 1.1.8.4  1994/06/21  14:45:19  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:31:38  marrek]
 * 
 * Revision 1.1.8.3  1994/05/10  15:52:06  marrek
 * 	Bug fix for April 1994 submission.
 * 	[1994/05/10  09:44:01  marrek]
 * 
 * 	Bug fixes.
 * 	[1994/04/29  09:29:15  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:20:48  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:09:16  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:20:36  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  16:05:33  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:11:11  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  13:54:59  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  13:06:12  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  19:39:33  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:19:08  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/17  10:00:20  marrek
 * 	Make logical unbind after DSP reject
 * 	[1992/12/16  13:01:35  marrek]
 * 
 * Revision 1.1.4.2  1992/11/27  19:40:42  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  11:08:03  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:47:40  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:54:27  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d26dsp.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:45:07 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODUL                                               */
/*                                                                    */
/* NAME         : d26dsp.c       [d26dsp]                             */
/*                                                                    */
/* AUTHOR       : E. Kraemer, D AP 11                                 */
/* DATE         : 13.02.91                                            */
/*                                                                    */
/* COMPONENT    : DSA, retrieval functions                            */
/*                                                                    */
/* DOC.-NR.     : <relevante Dokumente>                               */
/*                      .                                             */
/*                      .                                             */
/*                                                                    */
/* PRD#/VERS.   : <Produktnr. aus Produktstruktur> - <Versionsnr.>    */
/*                                                                    */
/* DESCRIPTION  :   The module contains the functions, that do the    */
/*                  chaining to other DSAs.                           */
/*                                                                    */
/* SYSTEM-DEPENDENCY  :                                               */
/*                      SINIX                                         */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Datum  |  Aenderungen                   | KZ | CR# FM# */
/*          | 14.05.91| birth                          | ek |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*                                                                    */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                        I N C L U D E S                             */
/*                                                                    */
/**********************************************************************/

#include <d26dsa.h>

/*****  Operating-System-Includes  *****/

#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

/*****  external Includes    *****/

#include <dce/d27util.h>
#include <gds_sec.h>
#define DSA
#include <d2asn1.h>

/*****  internal Includes    *****/

/**********************************************************************/
/*                                                                    */
/*                        D E F I N E S                               */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                D E C L A R A T I O N S        DATA                 */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*                                                                    */
/**********************************************************************/

/******** IMPORT ********/

/******** LOCAL  ********/

static signed16 d26_d03_write_bind_message(byte **bind_message,
    signed32 *buf_len, signed32 *mes_len, signed16 assid, D2_str *psap_addr, 
    D2_name_string dsa_name, boolean *auth_mechs, char *princ_name, 
    boolean *try_again, unsigned32 *sec_ctx_id, signed16 *used_am);
static byte *d26_d07_enhance_chaining_args(byte **request,
    signed32 *request_len, byte *message, signed32 *remlen,
    signed16 ref_idx,signed16 bind_sec_meth, boolean *sec_mechs);

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_d01_chain_request                                 */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        14.05.91                                              */
/*                                                                    */
/* DESCRIPTION: This function binds to another DSA, using the         */
/*              information of the global referral structure. It      */
/*              creates a chaining message, using the chaining        */
/*              arguments from the global variable and the DAP-message*/
/*              from the parameters. The returned result is put into  */
/*              the parameters.                                       */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      D2_NO_ERR       signed16    no error occurred                 */
/*      D26_ERROR                   hard error occurred.              */
/*      D26_DSP_ERROR               error/referral result returned    */
/*				    by request decomposition.	      */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_d01_chain_request(
    byte *dap_message,  /* IN    -  DAP message for request           */
    signed32 input_len, /* IN    -  length of DAP message             */
    byte **output,      /* INOUT -  result message to return          */
			/* INOUT -  length of allocated memory for    */
    signed32 *output_len,           /* result                         */
    signed32 *remlen,   /* INOUT -  remaining length of result        */
    signed16 op_id,     /* IN    -  operation ID of request           */
    D23_rsrqpb *rsrqpb) /* IN    -  parameters for d23_result         */

{                       /*  d26_d01_chain_request                     */

  signed16 result_type; /* result type                                */
  signed16 ret_value;   /* return value of request decomposition      */

  if                    /* local result cannot be generated           */
  (d26_u63_write_error_referral(output,output_len,remlen,d26_referral)
   != D2_NO_ERR)
  {
    return(D26_ERROR);
  }

  result_type = D26_RD_REFERRAL;
  ret_value = d26_d21_req_decomp(dap_message,input_len,output,output_len,
    remlen,op_id,&result_type,rsrqpb);

  if                    /* errror occurred                            */
    (ret_value == D2_NO_ERR && result_type != D26_RD_RESULT)
  {                     /* set return value                           */
    ret_value = D26_DSP_ERR;
  }

  return(ret_value);

}                       /*  d26_d01_chain_request                     */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_d02_bind_request                                  */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        14.05.91                                              */
/*                                                                    */
/* DESCRIPTION: This function binds to another DSA, using the PSAP-   */
/*              address and the security parameters given.	      */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      D2_NO_ERR   signed16        no error occurred                 */
/*      D26_ERROR                   hard error occurred.              */
/*                                                                    */
/*inoff ***************************************************************/

signed16 d26_d02_bind_request(
    D23_bindpb *bindpb, /* IN    -  parameter-block for IPC-bind      */
    D23_inrqpb *inrqpb, /* INOUT -  parameter-block for IPC-invoke    */
    D2_str *psap_addr,  /* IN    -  PSAP address		      */
    D2_name_string dsa_name,  /* IN    -  target DSA name	      */
    boolean *auth_mechs,/* IN    -  supported auth. mechs	      */
    char   *princ_name, /* IN    -  principal name		      */
    boolean *try_again, /* INOUT -  				      */
    unsigned32 *sec_ctx_id, /* OUT -  context ID		      */
    signed16 *used_am)  /* OUT   -  used auth. mech in bind	      */

{                       /*  d26_d02_bind_request                      */

				    /* function identifier for logging*/
  static  String  function = "d26_d02_bind_request";

  byte     *bind_message;   /* bind IPC-message                       */
  signed32 mes_len;         /* length of bind message                 */
  signed32 buf_len;         /* length of bind message                 */

  byte     *message;        /* actual message pointer                 */
  signed16 len;             /* length of data to write                */
  signed16 off;             /* offset to next data block              */
  signed32 remlen;          /* remaining length of bind message       */

			    /* memory for error apdu                  */
  byte     err_apdu[sizeof(D23_rserror) + sizeof(D23_eom)];
			    /* structure for encoded request          */
  asn1_field asn1_request;
			    /* return value                           */
  signed16 ret_value = D2_NO_ERR;

  inrqpb->d23_6assid  = bindpb->d23_4assid;
  inrqpb->d23_6invinfo.d23_invid  = D23_UNUSED;
  inrqpb->d23_6invinfo.d23_contid = D23_SAC(D27_DSP_AC) |
    D23_SACM(D27_LOC_AC) | D23_SAS(D27_DSP_AS);
  inrqpb->d23_6invinfo.d23_usrid  = inrqpb->d23_6assid;
  inrqpb->d23_6oprid  = D23_BIND;
  inrqpb->d23_6rfidt  = bindpb->d23_4refms;
  if                    /* no more memory available                   */
    ((bind_message = (byte *)D27_MALLOC(svc_c_sev_warning,D26_BLOCK_LEN))
     == NULL)
  {                     /* set return value                           */
    ret_value = D26_ERROR;
  }
  else
  {                     /* write the bind message                     */
    buf_len        = D26_BLOCK_LEN;
    asn1_request.l = 0;
    if                  /* error from writing bind message            */
      (d26_d03_write_bind_message(&bind_message,&buf_len,&mes_len,
       bindpb->d23_4assid, psap_addr, dsa_name, auth_mechs, princ_name, 
       try_again, sec_ctx_id, used_am) != D2_NO_ERR)
    {                   /* set return value                           */
      ret_value = D26_ERROR;
    }
    else
    {                   /* log and encode message                     */
      DCE_SVC_LOG((GDS_S_APDU_REQUEST_MSG,D23_BIND,mes_len,bind_message));
      if                /* error from encoding bind request           */
	(d2a000_apdu_asn1(D23_BIND,D27_DSP_AS,bind_message,mes_len,
	 &asn1_request,err_apdu) == D2_ERROR)
      {                 /* set return value                           */
        D27_022_ENTER_TIME_TRACE(GDS_S_ENCODING_ERR,function,0,0);
	ret_value = D26_ERROR;
	/* delete credentials */
	d26_d11_conf_bind_result(D26_ERROR,0,NULL,NULL,*used_am,*sec_ctx_id);
      }
      else
      {                 /* copy bind request after PSAP address       */
        D27_022_ENTER_TIME_TRACE(GDS_S_ENCODING_OK,function,0,0);
	message = bind_message;
	remlen  = buf_len;
	len     = sizeof(D23_av_value) + D2_ALIGN(psap_addr->d2_size);
	if              /* error from check buffer                    */
	  ((message = d26_u40_check_buffer(len,&buf_len,&remlen,&bind_message,
	   message)) == NULL)
	{               /* set return value                           */
	  ret_value = D26_ERROR;
	}
	else
	{               /* write PSAP-address                         */
	  len = psap_addr->d2_size;
	  off = D2_ALIGN(len);
	  ((D23_av_value *)message)->d23_avlen = len;
	  ((D23_av_value *)message)->d23_avoff = off;
	  message += sizeof(D23_av_value);
	  remlen  -= sizeof(D23_av_value);
	  memcpy(message,psap_addr->d2_value,off);
	  message += off;
	  remlen  -= off;

	  len      = (signed16)asn1_request.l;
	  if            /* error from check buffer                    */
	    ((message = d26_u40_check_buffer(len,&buf_len,&remlen,
	     &bind_message,message)) == NULL)
	  {             /* set return value                           */
	    ret_value = D26_ERROR;
	  }
	  else
	  {             /* write ASN1-coded bind message              */
	    memcpy(message,asn1_request.v,len);
	    message += len;
	    remlen  -= len;
	    mes_len  = buf_len - remlen;
	    if          /* error from sending bind request            */
	      (d26_d04_send_request(inrqpb,bindpb->d23_4lenms,bind_message,
	       mes_len) != D2_NO_ERR)
	    {           /* set return value                           */
	      ret_value = D26_ERROR;
	    }
	  }
	}
	free(asn1_request.v);
      }
    }
    free(bind_message);
  }

  return(ret_value);

}                       /* d26_d02_bind_request                       */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_d03_write_bind_message                            */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        14.05.91                                              */
/*                                                                    */
/* DESCRIPTION: This function writes the message, that is transferred */
/*              to bind another DSA in IPC-format.                    */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      Short   D2_NO_ERR           no error occurred                 */
/*              D26_ERROR           hard error occurred.              */
/*                                                                    */
/*inoff ***************************************************************/

static signed16 d26_d03_write_bind_message(
    byte **bind_message, /* INOUT - pointer to bind message to write  */
    signed32 *buf_len,   /* INOUT - maximum length of bind message    */
    signed32 *mes_len,  /* OUT   -  length of bind message            */
    signed16 assid,	/* IN    -  ID to use as bind_id for sec. fct */
    D2_str *psap_addr,  /* IN    -  PSAP address		      */
    D2_name_string dsa_name, /* IN - target DSA name		      */
    boolean *auth_mechs,/* IN    -  supported auth. mechs of target DSA */
    char   *princ_name, /* IN    -  principal name		      */
    boolean *try_again, /* INOUT -  indicates wether bind should be   */
			/* 	    tried once again (anonymous)      */
    unsigned32 *sec_ctx_id, /* OUT -  context ID		      */
    signed16 *used_am)  /* OUT   -  used auth. mech in bind	      */

{                       /*  d26_d03_write_bind_message                */

  byte     *message;        /* actual message pointer                 */
  signed32 remlen;          /* remaining length of bind message       */
  signed16 len;             /* length of data actually to write       */
  signed16 struct_len;
  signed16 off;             /* offset to next IPC-block               */
  signed16	i;		/* loop variable		      */
  D2_str	sec_token;	/* security token		      */
  D2_sec_status	status;		/* status of security function	      */
  signed32 	sec_ret_val;	/* return value of security function  */
  D2_name_string nm_ptr;	/* pointer to name for sec. fct. call */

  /* determine security mechainsm for the bind */

  *used_am = -1;
  if (*try_again == TRUE)	/* do anonymous bind because higher	*/
  {				/* level failed				*/
    *used_am = D2_ANONYMOUS;
  }
  else				/* find highest common security level	*/
  {
    for (i = D2_MAX_AUTH_MECHS; i >= 0; i--)
    {
      if (auth_mechs[i] == TRUE && d26_auth_mech[i] == TRUE)
      {
        *used_am = i;
	break;
      }
    }

    /* if target DSA doesn't have the AM-attribute 			*/
    /* use highest own sec. level, if this fails, make anonymous bind! 	*/
    if (*used_am == -1)
    {
      for (i = D2_MAX_AUTH_MECHS; i >= 0; i--)
      {
        if (d26_auth_mech[i] == TRUE)
	{
	  *used_am = i;
	  *try_again = TRUE;
	  break;
	}
      }

      /* if DSA supports both DCE and Strong use DCE Security		*/
      if (*used_am == D2_STRONG && d26_auth_mech[D2_DCE_AUTH] == TRUE)
      {
	*used_am = D2_DCE_AUTH;
      }
      /* for testing: don't try again */
      *try_again = FALSE;
      *used_am = D2_ANONYMOUS;
    }
  } /* else find highest ... */

  /* init security context */

  if (*used_am == D2_DCE_AUTH)
	nm_ptr = (D2_name_string) princ_name;
  else
	nm_ptr = dsa_name;

  sec_ret_val = dsa_init_sec_ctx(*used_am, nm_ptr, NULL, sec_ctx_id,
		&sec_token, &status);

  while (sec_ret_val != D2_NOERROR && status.d2_errvalue == D2_S_EXPIRED_CRED)
  {
    /* delete old and acquire new credentials */
    if ((sec_ret_val = dsa_del_cred(*used_am,&status)) == D2_ERROR)
    {
      DCE_SVC_LOG((GDS_S_DEL_CRED_ERR_MSG,*used_am,status.logstring));
      return(D26_ERROR);
    }
    else if ((sec_ret_val = dsa_acquire_cred(*used_am,d26_myname,
	      &status)) == D2_ERROR)
    {
      DCE_SVC_LOG((GDS_S_ACQU_CRED_ERR_MSG,*used_am,status.logstring));
      return(D26_ERROR);
    }
    else
    {
      sec_ret_val = dsa_init_sec_ctx(*used_am, nm_ptr, NULL, sec_ctx_id,
		&sec_token, &status);
    }
  }

  if (sec_ret_val != D2_NOERROR)
  {
    DCE_SVC_LOG((GDS_S_INIT_SEC_ERR_MSG,*used_am,nm_ptr,status.logstring));
    *try_again = FALSE;
    return(D26_ERROR);
  }

  /*  write bind header data                    */

  message = *bind_message;
  remlen  = *buf_len;
  len     = sizeof(D23_inbind);

  ((D23_inbind *)message)->d23_Aversno = D23_APDUV22;
  ((D23_inbind *)message)->d23_Adirid  = 0;

  switch (*used_am)
  {
    case D2_SIMPLE:
		((D23_inbind *)message)->d23_Acrkind = D23_SIMPLE_CRED;
		struct_len = D23_S_SIMCR;
		break;
    case D2_DCE_AUTH:
		((D23_inbind *)message)->d23_Acrkind = D23_EXTERNAL_CRED;
		struct_len = D23_S_EXTCR;
		break;
    case D2_STRONG:
		((D23_inbind *)message)->d23_Acrkind = D23_STRONG_CRED;
		struct_len = D23_S_SSCR;
		break;
    case D2_ANONYMOUS:
    default:
		((D23_inbind *)message)->d23_Acrkind = D23_NO_CRED;
		struct_len = 0;
		break;
  }

  message += len;
  remlen  -= len;

  len = struct_len + sec_token.d2_size;
  if                    /*  error from check buffer                   */
      ((message = d26_u40_check_buffer(len,buf_len,&remlen,bind_message,
	   message)) == NULL)
  {                     /*  return error                              */
	return(D26_ERROR);
  }

  switch (*used_am)
  {
    case D2_SIMPLE:
		((D23_simcr_info *)message)->d23_scrtag = D23_SIMTAG;
		((D23_simcr_info *) message)->d23_scrfil = 0;
		off = sec_token.d2_size;
		message += D23_S_SIMCR;
		remlen -= D23_S_SIMCR;
		break;
    case D2_DCE_AUTH:
		((D23_extcr_info *) message)->d23_exttag =  D23_EXTTAG;
		((D23_extcr_info *) message)->d23_extkind = D2_DCE_AUTH;
		((D23_extcr_info *) message)->d23_extlen =  sec_token.d2_size;
		((D23_extcr_info *) message)->d23_extoff =  off =
						D2_ALIGN(sec_token.d2_size);
		message += D23_S_EXTCR;
		remlen -= D23_S_EXTCR;
		break;
    case D2_STRONG:
		((D23_strcr_info *) message)->d23_strtag = D23_STRTAG;
		((D23_strcr_info *) message)->d23_strfil = 0;
		((D23_strcr_info *) message)->d23_strlen = sec_token.d2_size;
		((D23_strcr_info *) message)->d23_stroff = off =
						D2_ALIGN(sec_token.d2_size);
		message += D23_S_SSCR;
		remlen -= D23_S_SSCR;
		break;
    case D2_ANONYMOUS:
    default:
		off = 0;
		break;
  }

  /* copy credential to message */
  if (sec_token.d2_size > 0)
  {
    memcpy(message, sec_token.d2_value, sec_token.d2_size);
  }

  message += off;
  remlen  -= off;

                     /*  write PSAP-address                        */
  len = psap_addr->d2_size;
  off = D2_ALIGN(len);
  ((D23_av_value *)message)->d23_avlen = len;
  ((D23_av_value *)message)->d23_avoff = off;
  ((D23_av_value *)message)->d23_avrep = D2_PSAP_STX;
  message += sizeof(D23_av_value);
  remlen  -= sizeof(D23_av_value);
  memcpy(message,psap_addr->d2_value,off);
  message += off;
  remlen  -= off;

  len = sizeof(D23_eom);
  if                /*  error from check buffer                   */
	((message = d26_u40_check_buffer(len,buf_len,&remlen,bind_message,
	 message)) == NULL)
  {                 /*  return error                              */
	return(D26_ERROR);
  }
  else
  {                 /*  write EOM-tag                             */
    ((D23_eom *)message)->d23_eomtag = D23_EOMTAG;
	message += sizeof(D23_eom);
	remlen  -= sizeof(D23_eom);
	*mes_len = *buf_len - remlen;
  }                 

return(D2_NO_ERR);

}                       /*  d26_d03_write_bind_message                */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_d04_send_request                                  */
/*                                                                    */
/* AUTHOR:      E. Kraemer, DAP 11                                    */
/* DATE:        15.05.91                                              */
/*                                                                    */
/* DESCRIPTION: The function splits the request message into blocks   */
/*              and sends it via IPC to the server.                   */
/*                                                                    */
/* RETURN VALUE :                                                     */
/*      D2_NO_ERR   signed16        no error occurred                 */
/*      D26_ERROR                   error occurred                    */
/*                                                                    */
/*inoff ***************************************************************/

signed16 d26_d04_send_request(
    D23_inrqpb *inrqpb, /* IN    -  parameter block vor IPC-invoke    */
    signed16   meslen,  /* IN    -  length of IPC-block               */
    byte       *request,/* IN    -  complete request to send          */
			/* IN    -  length of request message         */
    signed32   request_len)

{                                   /*  d26_d04_send_request          */

				    /* function identifier for logging*/
  static char *function = "d26_d04_send_request";
  signed16 block_count = 0;         /* count of result blocks         */

			/* send the invoke message                    */
  inrqpb->d23_6invinfo.d23_invid = D23_UNUSED;
  while                 /* still data to send                         */
       (request_len > 0)
  {                     /*  copy data to IPC-block                    */
    if                  /*  more data than one block present          */
      (request_len > meslen)
    {                   /*  set length and MORE-DATA-indicator        */
      inrqpb->d23_6imdat = D23_MRDATA;
      inrqpb->d23_6lnidt = meslen;
    }
    else
    {                   /*  set length and LAST FRAGMENT indicator    */
      inrqpb->d23_6imdat = D23_LASTFG;
      inrqpb->d23_6lnidt = request_len;
    }
    memcpy(inrqpb->d23_6rfidt,request,inrqpb->d23_6lnidt);
    if                  /* error from writing message                 */
      (d23_invoke(inrqpb) == D23_ERROR)
    {                   /* set return value                           */
      return(D26_ERROR);
    }
    else
    {                   /* reset length and request pointer           */
      request     += inrqpb->d23_6lnidt;
      request_len -= inrqpb->d23_6lnidt;
      block_count++;
    }
  }

  D27_022_ENTER_TIME_TRACE(GDS_S_FCT_SENT_BLOCKS,function,block_count,0);

			/* return                                     */
  return(D2_NO_ERR);

}                                   /*  d26_d04_send_request          */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_d05_collect_result()                              */
/*                                                                    */
/* AUTHOR:      Kraemer D AP 11                                       */
/* DATE:        15.05.91                                              */
/*                                                                    */
/* DESCRIPTION: The function collects all blocks of result from IPC   */
/*                                                                    */
/* RETURN VALUE :                                                     */
/*      D2_NO_ERR   signed16        no error occurred                 */
/*      D26_ABORTED                 IPC error of association aborted  */
/*      D26_MEM_ERR                 malloc error occurred             */
/*      D26_LOG_ERR                 error result received             */
/*      D26_REJECTED                request was rejected              */
/*                                                                    */
/*inoff ***************************************************************/

signed16 d26_d05_collect_result(
    signed16 op_id,     /* IN     - operation ID                      */
    D23_rsrqpb *rsrqpb, /* IN    -  parameters for d23_result         */
    D23_wtrspb *wtrspb, /* INOUT  - parameters for wait for result    */
    byte **result,      /* OUT    - pointer for result message        */
			/* OUT    - length of result message          */
    signed32 *result_len,
    signed32 *remlen,   /* INOUT  - remaining length of result message*/
    signed16 *err_id)   /* OUT    - error ID of error message         */

{                                     /*  d26_d05_collect_result      */

				    /* function identifier for logging*/
  static char *function = "d26_d05_collect_result";

				    /* structure for sigaction call   */
  static struct sigaction siginfo = {0};

				    /* indicates abandoned request    */
  boolean  abandoned = d26_abandoned;
  boolean  more_blocks;             /* indicates following blocks     */
  signed16 ret_value;               /* return value                   */
  byte     *message;                /* actual message pointer         */

  signed16 block_count = 0;         /* count of message blocks        */

  sigemptyset(&siginfo.sa_mask);
  for                               /*  all incoming blocks           */
     (message = *result, more_blocks = D23_MRDATA, ret_value = D2_NO_ERR;
      more_blocks == D23_MRDATA; )
  {                                 /*  wait for next result block    */
    d26_m11_distcmd(SIGUSR1);
    if                  /* request is abandoned but abandoned error   */
			/* not sent                                   */
      (d26_abandoned == TRUE && abandoned == FALSE)
    {                   /* send the abandoned error                   */
      abandoned = TRUE;
      if                /* abandoned error cannot be sent             */
	(d26_u84_send_abandoned(op_id,result,remlen,result_len,rsrqpb)
	 != D2_NO_ERR)
      {                 /* set return value                           */
	ret_value = D26_ERROR;
      }
			/* abandon all pending subrequests            */
      d26_d43_abandon_subrequests();
    }

    siginfo.sa_handler = d26_m06_hdl_sigdistcmd;
    D27_SIGACTION(svc_c_sev_warning,SIGUSR1,&siginfo,
      (struct sigaction *)NULL);
    if                              /*  error from waiting for result */
				    /*  or request is aborted or      */
				    /*  rejected                      */
      (d23_waitrs(wtrspb) == D23_ERROR || wtrspb->d23_7rltid == D23_ABORT)
    {                               /*  set return value              */
      ret_value   = D26_ABORTED;
    }
    else if                         /* signal has occurred            */
				    /* (never with threads)           */
	   (wtrspb->d23_7rltid == D23_NOOPR)
    {                               /* continue to collect results    */
      pthread_yield();
      continue;
    }
    else if                         /*  request was rejected          */
	   (D23_RREJECT <= wtrspb->d23_7rltid &&
	    wtrspb->d23_7rltid <= D23_RREJECT + D23_MXRREJ)
    {                               /*  set return value              */
      ret_value   = D26_REJECTED;
    }
    else if                         /*  no error so far               */
	   (ret_value == D2_NO_ERR)
    {                               /*  check buffer                  */
      if                /*  error from check buffer                   */
	((message = d26_u40_check_buffer(wtrspb->d23_7lnrdt,result_len,remlen,
	 result,message)) == NULL)
      {                 /*  return error                              */
	ret_value = D26_MEM_ERR;
      }
      else
      {                 /*  copy result-block                         */
	memcpy(message,wtrspb->d23_7rfrdt,wtrspb->d23_7lnrdt);
	message += wtrspb->d23_7lnrdt;
	*remlen -= wtrspb->d23_7lnrdt;
      }
    }
    more_blocks = wtrspb->d23_7rmdat;
    block_count++;
    siginfo.sa_handler = SIG_IGN;
    D27_SIGACTION(svc_c_sev_warning,SIGUSR1,&siginfo,
      (struct sigaction *)NULL);
  }

  *err_id = (D23_RERROR <= wtrspb->d23_7rltid &&
    wtrspb->d23_7rltid <= D23_RERROR + D23_MXRERR) ?
    wtrspb->d23_7rltid - D23_RERROR : 0;

  if                                /*  no error occurred             */
    (*err_id != 0 &&
     (ret_value == D2_NO_ERR && *err_id != D23S_DSA_REFERRAL ||
     ret_value == D26_MEM_ERR))
  {                                 /*  reset return value            */
    ret_value = D26_LOG_ERR;
  }

  D27_022_ENTER_TIME_TRACE(GDS_S_FCT_RECEIVED_BLOCKS,function,block_count,0);

				    /*  return                        */
  return(ret_value);

}                                   /*  d26_d05_collect_result        */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_d06_build_chained_request                         */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        16.05.91                                              */
/*                                                                    */
/* DESCRIPTION: This function writes the DSP-message, that is         */
/*              transferred to another DSA to chain any request.      */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      D2_NO_ERR   signed16        no error occurred                 */
/*      D26_ERROR                   hard error occurred.              */
/*                                                                    */
/*inoff ***************************************************************/

signed16 d26_d06_build_chained_request(
    byte **request,     /* OUT   -  chained request to be generated   */
			/* OUT   -  length of chained request         */
    signed32 *request_len,
    byte *dap_message,  /* IN    -  DAP message to chain              */
    signed32 mes_len,   /* IN    -  length of DAP message to chain    */
    signed16 ref_idx,   /* IN    -  index of referral to use          */
    signed16 bind_sec_meth,/* IN -  security mech. used in DSP bind   */
    boolean  *sec_mechs)   /* IN -  sec. mechs. supp. by target DSA   */

{                       /*  d26_d06_build_chained_request             */

  byte     *message;        /* actual message pointer                 */
  signed32 remlen;          /* remaining length of bind message       */
  signed32 len;             /* length of data to write                */
  signed16 ret_value = D2_NO_ERR;   /* return value                   */

  *request_len = (mes_len / D26_BLOCK_LEN + 1) * D26_BLOCK_LEN;

  if                        /*  error from allocating request memory  */
    ((*request = (byte *)D27_MALLOC(svc_c_sev_warning,*request_len)) == NULL)
  {                         /*  log error and return                  */
    return(D26_ERROR);
  }
  else
  {                         /*  check whether chaining arguments are  */
			    /*  present                               */
    message = *request;
    remlen  = *request_len;

    len     = sizeof(D23_header);
    if                  /*  error from check buffer                   */
      ((message = d26_u40_check_buffer(len,request_len,&remlen,request,
       message)) == NULL)
    {                   /*  set return value                          */
      ret_value = D26_ERROR;
    }
    else
    {                   /*  write header data                         */
      ((D23_header *)message)->d23_versno = D23_APDUV22;
      ((D23_header *)message)->d23_dirid  = 0;
      ((D23_header *)message)->d23_bindid = 0;
      message += len;
      remlen  -= len;
      if                    /*  time has run out already              */
	(d26_time_ex == TRUE)
      {                     /*  set return value                      */
	ret_value = D26_ERROR;
      }
      else
      {                     /*  check whether time limit was given    */
	if                  /*  time limit was given                  */
	  (d26_time_limit > 0)
	{                   /*  reset time-limit                      */
	  d26_time_limit = (signed32)alarm(0);
	  alarm(d26_time_limit);
	}
	if                  /*  error from enhancing chain. arguments */
	  ((message = d26_d07_enhance_chaining_args(request,request_len,
	   message,&remlen,ref_idx,bind_sec_meth, sec_mechs)) == NULL)
	{                   /*  set return value                      */
	  ret_value = D26_ERROR;
	}
	else if         /*  error from check buffer                   */
	       ((message = d26_u40_check_buffer(len,request_len,&remlen,
		request,message)) == NULL)
	{               /*  set return value                          */
	  ret_value = D26_ERROR;
	}
	else
	{               /*  write DAP message                         */
	  memcpy(message,dap_message,mes_len);
	  message      += mes_len;
	  remlen       -= mes_len;
	  *request_len -= remlen;
	}
      }
    }
    if                      /*  error occurred                        */
      (ret_value != D2_NO_ERR)
    {                       /*  free the request and return error     */
      free(*request);
    }
  }

  return(ret_value);

}                       /*  d26_d06_build_chained_request             */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_d07_enhance_chaining_args                         */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        17.05.91                                              */
/*                                                                    */
/* DESCRIPTION: This function creates the outgoing chaining arguments */
/*              by updating and enhancing the incoming ones. It       */
/*              writes them into the IPC-message.                     */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      request     String *        pointer to DSP-message to write   */
/*      request_len Int *           maximum length of message         */
/*      message     String          actual message pointer            */
/*      remlen      Int *           remaining message length          */
/*      ref_idx     Short           index of referral to use          */
/*                                                                    */
/* OUTPUT-PARAMETERS: None                                            */
/*      request     String *        pointer to DSP-message to write   */
/*      request_len Int *           maximum length of message         */
/*      remlen      Int *           remaining message length          */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      String  != D2_NULL          actual message pointer            */
/*                 D2_NULL          hard error occurred.              */
/*                                                                    */
/*inoff ***************************************************************/

static byte *d26_d07_enhance_chaining_args(byte **request,
    signed32 *request_len, byte *message, signed32 *remlen, signed16 ref_idx,
    signed16 bind_sec_meth, boolean *sec_mechs)

{                       /*  d26_d07_enhance_chaining_args             */

  signed16 len;             /* length of data actually to write       */
  signed16 off;             /* offset to next structure               */
  D26_referral *referral;   /* pointer to used referral               */

  signed16 bo_len;          /* length of base object                  */
  signed16 to_len;          /* length of target object                */
  Bool     to_complete;     /* indicates whether target object is     */
			    /* completely decoded                     */
  byte     norm_base_obj[D2_DNL_MAX + 1];   /* normed base object     */

referral = d26_referral + ref_idx;

len      = sizeof(D23_chain_arg);
if                      /*  error from check buffer                   */
  ((message = d26_u40_check_buffer(len,request_len,remlen,request,message))
   == NULL)
{                       /*  set return value                          */
  return(NULL);
}
else
{                       /*  write chaining arguments header           */
  ((D23_chain_arg *)message)->d23_chtag    = D23_CHTAG;
  if                    /*  alias has been dereferenced               */
    (referral->d26_al_rdns > 0)
  {                     /*  set alias components of chaining arguments*/
    ((D23_chain_arg *)message)->d23_al_deref = TRUE;
    ((D23_chain_arg *)message)->d23_al_RDN   = referral->d26_al_rdns;
  }
  else
  {                     /*  reset alias components                    */
    ((D23_chain_arg *)message)->d23_al_deref = FALSE;
    ((D23_chain_arg *)message)->d23_al_RDN   = D2_NOT_DEFINED;
  }                     
  ((D23_chain_arg *)message)->d23_entry_only = referral->d26_entry_only;
  ((D23_chain_arg *)message)->d23_ret_cref = FALSE;
  if                    /*  no namepart resolved                      */
    (referral->d26_op_prog.d2_nr_phase == D2_NOT_STARTED ||
     referral->d26_op_prog.d2_nr_phase == D2_PROCEEDING &&
     referral->d26_op_prog.d2_next_rdn == 1)
  {                     /*  get reference type from master knowledge  */
    ((D23_chain_arg *)message)->d23_ref_type = D2_CROSS;
  }
  else if               /*  name resolution is proceeding             */
	 (referral->d26_op_prog.d2_nr_phase == D2_PROCEEDING)
  {                     /*  get reference type from difference of     */
			/*  nameparts                                 */
    ((D23_chain_arg *)message)->d23_ref_type =
	referral->d26_op_prog.d2_next_rdn - referral->d26_rdn_res > 1 ?
	    D2_CROSS : D2_SUBORDINATE;
  }
  else
  {                     /*  set subordinate type                      */
    ((D23_chain_arg *)message)->d23_ref_type = D2_SUBORDINATE;
  }                     
  ((D23_chain_arg *)message)->d23_chtmlt = d26_time_limit;
  ((D23_chain_arg *)message)->d23_op_prog.d23_opnm_rs =
					  referral->d26_op_prog.d2_nr_phase;
  ((D23_chain_arg *)message)->d23_op_prog.d23_op_nxtrdn =
					  referral->d26_op_prog.d2_next_rdn;
  ((D23_chain_arg *)message)->d23_max_tr =
	d26_chain_arg == (D23_chain_arg *)NULL ?
	    1 : d26_chain_arg->d23_max_tr + 1;
  message += len;
  *remlen -= len;
  if                    /*  error from writing trace information      */
    ((message = d26_u68_write_trace_info(request,request_len,message,
     remlen,referral)) == NULL)
  {                     /*  return error                              */
    return(NULL);
  }
  else
  {                     /*  check requestor                           */
    if                  /*  requestor is not anonymous                */
                        /*  and security method is the same or higher */
      (strcmp(d26_requestor.d26_req_name,D26_ANONYMOUS) != 0 &&
       (d26_requestor.d26_sec_meth >= bind_sec_meth ||
	/* target DSA is in migration phase -> support DCE and SIMPLE !!! */
	(d26_requestor.d26_sec_meth == D2_SIMPLE && sec_mechs[D2_SIMPLE] &&
	 sec_mechs[D2_DCE_AUTH])))
    {                   /*  write it as originator into the message   */
      len = sizeof(D23_ds_name) + strlen(d26_requestor.d26_req_name) + 1;
      if                /*  error from check buffer                   */
	((message = d26_u40_check_buffer(len,request_len,remlen,request,
	 message)) == NULL)
      {                 /*  set return value                          */
	return(NULL);
      }
      else
      {                 /*  write originator                          */
	((D23_ds_name *)message)->d23_dstag = D23_RQTAG;
	((D23_ds_name *)message)->d23_dscomplete = TRUE;
	len = strlen(d26_requestor.d26_req_name) + 1;
	off = D2_ALIGN(len);
	((D23_ds_name *)message)->d23_dslen = len;
	((D23_ds_name *)message)->d23_dsoff = off;
	message += sizeof(D23_ds_name);
	*remlen -= sizeof(D23_ds_name);
	strcpy((char *)message,d26_requestor.d26_req_name);
	message += off;
	*remlen -= off;
      }                 
    }                   

    if                  /*  DAP base object is ROOT                   */
      (d26_dap_b_object == NULL)
    {                   /*  create empty string for comparison        */
      strcpy((char *)norm_base_obj,"");
    }
    else
    {                   /*  norm DAP base object                      */
      bo_len = strlen((char *)d26_dap_b_object) + 1;
      d27_007_norm_name((char *)d26_dap_b_object,bo_len,
	(char *)norm_base_obj);
      d27_012_remove_blanks((char *)norm_base_obj,bo_len,
	(char *)norm_base_obj);
    }                   

    if                  /*  target object is different from base      */
			/*  object from DAP-message                   */
      (strcmp((char *)referral->d26_target,(char *)norm_base_obj) != 0 ||
       d26_dap_bo_ep != (D23_av_value *)NULL ||
       referral->d26_targ_ep != (D23_av_value *)NULL)
    {                   /*  check buffer for target object            */
      to_len = strlen((char *)referral->d26_etarget) + 1;
      len = sizeof(D23_ds_name) + to_len;
      to_complete = referral->d26_targ_ep == (D23_av_value *)NULL ?
		    TRUE : FALSE;
      if                /*  error from check buffer                   */
	((message = d26_u40_check_buffer(len,request_len,remlen,request,
	 message)) == NULL)
      {                 /*  set return value                          */
	return(NULL);
      }
      else
      {                 /*  write target object                       */
	((D23_ds_name *)message)->d23_dstag = D23_BONTAG;
	((D23_ds_name *)message)->d23_dscomplete = to_complete;
	len = to_len > 1 ? to_len : 0;
	off = D2_ALIGN(len);
	((D23_ds_name *)message)->d23_dslen = len;
	((D23_ds_name *)message)->d23_dsoff = off;
	message += sizeof(D23_ds_name);
	*remlen -= sizeof(D23_ds_name);
	strcpy((char *)message,(char *)referral->d26_etarget);
	message += off;
	*remlen -= off;
	if              /*  target object not completely decoded      */
	  (to_complete == FALSE)
	{               /*  set offset and length                     */
	  off = referral->d26_targ_ep->d23_avoff;
	  len = sizeof(D23_av_value) + off;
	  if            /*  error from check buffer                   */
	    ((message = d26_u40_check_buffer(len,request_len,remlen,request,
	     message)) == NULL)
	  {             /*  set return value                          */
	    return(NULL);
	  }
	  else
	  {             /*  write undecoded part of target object     */
	    *(D23_av_value *)message = *referral->d26_targ_ep;
	    message += sizeof(D23_av_value);
	    *remlen -= sizeof(D23_av_value);
	    memcpy(message,(String)(referral->d26_targ_ep + 1),off);
	    message += off;
	    *remlen -= off;
	  }             
	}               
      }                 
    }                   
  }                     
}                       

return(message);

}                       /*  d26_d07_enhance_chaining_args             */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_d10_unbind_request                                */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        24.06.91                                              */
/*                                                                    */
/* DESCRIPTION: This function sends unbind to other DSA.              */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      inrqpb      D23_inrqpb *    parameter-block for IPC-invoke    */
/*      wtrspb      D23_wtrspb *    parameter-block for IPC-wait-res. */
/*                                                                    */
/* OUTPUT-PARAMETERS: None                                            */
/*                                                                    */
/* RETURNVALUE: None                                                  */
/*	D2_NO_ERR	unbind invoke successful.		      */
/*	D26_ERROR	invoke unsuccessful or no invoke sent.	      */	
/*                                                                    */
/*inoff ***************************************************************/

signed16 d26_d10_unbind_request(D23_inrqpb *inrqpb, D23_wtrspb *wtrspb)

{                       /*  d26_d10_unbind_request                    */

				    /* function identifier for logging*/
  static   char *function = "d26_d10_unbind_request";

			/* dsp request message                        */
  byte     request[sizeof(D23_header) + sizeof(D23_eom)];
			/* length of request                          */
  signed32 request_len;

			/* structure for encoded request              */
  asn1_field asn1_request;

			/* return value                               */
  signed16 ret_value = D26_ERROR;

			/* memory for error apdu                      */
  byte     err_apdu[sizeof(D23_rserror) + sizeof(D23_eom)];

  if                    /*  association was not aborted               */
    (wtrspb->d23_7rltid != D23_ABORT)
  {                     /*  write unbind message                      */
    ((D23_inunbind *)request)->d23_Chdr.d23_versno = D23_APDUV22;
    ((D23_inunbind *)request)->d23_Chdr.d23_dirid  = 0;
    ((D23_inunbind *)request)->d23_Chdr.d23_bindid = 0;
    ((D23_inunbind *)request)->d23_Ceom.d23_eomtag = D23_EOMTAG;
    request_len = sizeof(D23_header) + sizeof(D23_eom);
    DCE_SVC_LOG((GDS_S_APDU_REQUEST_MSG,D23_UNBIND,request_len,request));
    if                  /*  bind result was in ASN1                   */
      (D23_GTS(wtrspb->d23_7invinfo.d23_contid) == D27_ASN1_TS ||
       D23_GTS(wtrspb->d23_7invinfo.d23_contid) == D27_ASN1_ADM_TS)
    {                   /*  encode request in ASN1                    */
      d2a000_apdu_asn1(D23_UNBIND,D27_DSP_AS,request,request_len,&asn1_request,
	err_apdu);
      memcpy(inrqpb->d23_6rfidt,asn1_request.v,asn1_request.l);
      inrqpb->d23_6lnidt = asn1_request.l;
      D27_022_ENTER_TIME_TRACE(GDS_S_ENCODING_OK,function,0,0);
    }
    else
    {                   /*  copy IPC-request and set the length       */
      memcpy(inrqpb->d23_6rfidt,request,request_len);
      inrqpb->d23_6lnidt = request_len;
    }
    inrqpb->d23_6oprid = D23_UNBIND;
    inrqpb->d23_6invinfo.d23_invid = D23_UNUSED;
    D23_CTS(inrqpb->d23_6invinfo.d23_contid,
      D23_GTS(wtrspb->d23_7invinfo.d23_contid));
    inrqpb->d23_6imdat = D23_LASTFG;

    if                  /*  no error from sending unbind message      */
      (d23_invoke(inrqpb) != D23_ERROR)
    {                   /*  wait for unbind result                    */
      ret_value = D2_NO_ERR;
    }
    D27_022_ENTER_TIME_TRACE(GDS_S_FCT_SENT_BLOCKS,function,1,0);
  }

  return(ret_value);

}                       /*  d26_d10_unbind_request                    */

/************************************************************************/
/*								      	*/
/* Function Name : d26_d11_conf_bind_result				*/
/*								      	*/
/* Description	 : This function calls th appropriate security		*/
/*		   functions to confirm and delete the security context.*/
/*								      	*/
/* Return Value  : -							*/
/*								      	*/
/* Author        : Loose, SNI BU BA NM123                               */
/* Date          : 16.02.94						*/
/*								      	*/
/************************************************************************/

void d26_d11_conf_bind_result(
  signed16      bind_ret_val,   /* IN  - return value of bind request */
  unsigned16    cont_id,        /* IN  - context ID                   */
  byte          **result,       /* INOUT-bind result                  */
  signed32      *result_len,    /* INOUT-result len                   */
  signed16      used_am,        /* IN  - security mechanism           */
  unsigned32    bind_id)        /* IN  - security context id          */
{

/**********************************************************************/
/*                       D E C L A R A T I O N S                      */
/**********************************************************************/

				    /* function identifier for logging*/
  static char *function = "d26_d11_conf_bind_result";

  signed16 syntax;                  /* transfer-syntax of the result  */
  asn1_field asn1_result;           /* encoded result                 */

				    /* memory for error apdu          */
  byte err_apdu[sizeof(D23_rserror) + sizeof(D23_eom)];

byte	*scan;
D2_str	sec_token;
D2_str	*token_ptr;
D2_sec_status	status;

if (bind_ret_val == D2_NO_ERR)
{
  syntax = D23_GTS(cont_id);

  if                    /* result or error must be decoded            */
    (syntax == D27_ASN1_TS || syntax == D27_ASN1_ADM_TS)
  {                     /* decode the result first                    */
    asn1_result.l = *result_len;
    asn1_result.v = *result;

    if                  /* result could not be decoded                */
      (d2a001_asn1_apdu(D23_BIND + D2_RES_OPID,D27_DSP_AS,result,result_len,
       &asn1_result,err_apdu) == D2_ERROR)
    {                   /* indicate error result !                    */
      D27_022_ENTER_TIME_TRACE(GDS_S_DECODING_ERR,function,0,0);
      bind_ret_val = D26_REJECTED;
    }

    D27_022_ENTER_TIME_TRACE(GDS_S_DECODING_OK,function,0,0);
    free(asn1_result.v);
  }
}
if (bind_ret_val == D2_NO_ERR)
{
  scan = *result + D23_S_RSB;
  
  switch (used_am)
  {
    case D2_SIMPLE:
		sec_token.d2_size = *result_len - D23_S_RSB - D23_S_SIMCR;
		sec_token.d2_value = scan + D23_S_SIMCR;
		break;
    case D2_DCE_AUTH:
		sec_token.d2_size = ((D23_extcr_info *) scan)->d23_extlen;
		sec_token.d2_value = scan + D23_S_EXTCR;
		break;
    case D2_STRONG:
		sec_token.d2_size = ((D23_strcr_info *) scan)->d23_strlen;
		sec_token.d2_value = scan + D23_S_SSCR;
		break;
    case D2_ANONYMOUS:
    default:
		sec_token.d2_size = 0;
		sec_token.d2_value = NULL;
		break;
  }
  token_ptr = &sec_token;
}
else
{
  token_ptr = NULL;	/* indicate error result ! */
}

if (dsa_conf_sec_ctx(used_am, bind_id, token_ptr, &status)
	== D2_ERROR)
{
  /* error logging */
    DCE_SVC_LOG((GDS_S_CONF_SEC_ERR_MSG,used_am,status.logstring));
}

}

