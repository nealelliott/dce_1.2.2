/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d26bind.c,v $
 * Revision 1.1.718.2  1996/02/18  19:44:50  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:25:58  marty]
 *
 * Revision 1.1.718.1  1995/12/08  15:43:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:06:18  root]
 * 
 * Revision 1.1.716.6  1994/07/06  15:06:51  marrek
 * 	July 1994 code drop.
 * 	[1994/07/05  09:10:32  marrek]
 * 
 * Revision 1.1.716.5  1994/06/21  14:45:11  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:30:49  marrek]
 * 
 * Revision 1.1.716.4  1994/05/11  12:30:47  keutel
 * 	fix for two failing subtree tests: OT 10602
 * 	[1994/05/11  12:24:13  keutel]
 * 
 * Revision 1.1.716.3  1994/05/10  15:51:48  marrek
 * 	Use correct rcs id string.
 * 	[1994/05/02  14:46:33  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:19:39  marrek]
 * 
 * Revision 1.1.716.2  1994/03/23  15:09:07  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:19:52  keutel]
 * 
 * Revision 1.1.716.1  1994/02/22  16:05:12  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:08:56  marrek]
 * 
 * Revision 1.1.714.2  1993/10/14  17:16:17  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:07:56  keutel]
 * 
 * Revision 1.1.714.1  1993/10/13  17:30:24  keutel
 * 	October 1993 code drop
 * 	[1993/10/11  08:25:27  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  13:39:22  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  13:02:04  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:58:57  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:18:20  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  19:35:11  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  11:04:22  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:45:38  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:53:15  zeliff]
 * 
 * $EndLog$
 */
#if !defined lint && defined GDS_RCS_ID
static char rcsid[] = "@(#)$RCSfile: d26bind.c,v $ $Revision: 1.1.718.2 $ $Date: 1996/02/18 19:44:50 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*								      */
/* TYPE         : MODULE                                              */
/*								      */
/* NAME         : d26bind.c   < DS_BIND >                             */
/*								      */
/* AUTHOR       : Volpers                                             */
/* DATE         : 05.01.88                                            */
/*								      */
/* COMPONENT    : DS                                                  */
/*								      */
/* DOC.-NR.     : DS-Design-Spec.                                     */
/*			.					      */
/*			.					      */
/*								      */
/* PRD#/VERS.   : <Productnr. from Productstructure - <Versionnr.>    */
/*								      */
/* DESCRIPTION  :                                                     */
/*								      */
/* SYSTEM DEPENDENCIES:                                               */
/*								      */
/* HISTORY      :                                                     */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | ID | CR# EM# */
/*   0.1    |05.01.88 |  Original                      | HV |         */
/*								      */
/*datoff **************************************************************/

/**********************************************************************/
/*								      */
/*			  I N C	L U D E	S			      */
/*								      */
/**********************************************************************/

#include <d26dsa.h>

/*****  internal Includes    *****/

#include <dce/d27util.h>
#include <d26schema.h>
#include <gds_sec.h>

/*****  Operating-System-Includes  *****/

#include <sys/types.h>
#include <stdlib.h>

/**********************************************************************/
/*								      */
/*			  D E F	I N E S				      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                 D E C L A R A T I O N S        DATA                */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*								      */
/**********************************************************************/

/******** LOCAL  ********/

static void d26_103_bind_rs(signed16 c_kind, signed16 sec_mech,
	D2_str *return_token, byte *a_out, signed32 *s_len);

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME  :      d26_1_bind                                            */
/*								      */
/* AUTHOR:      Volpers D AP 11                                       */
/* DATE:        05.01.88                                              */
/*								      */
/* DESCRIPTION: This function checks whether the user is authenticated*/
/*              to access the directory.                              */
/*								      */
/* INPUT PARAMETERS:                                                  */
/*                D23_inbind      *a_in;    invoke message            */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*                D23_rsbind      *a_out;   result (or error) message */
/*                Ushort          *length;  length of message         */
/*								      */
/*								      */
/* RETURN VALUE :   D2_NO_ERR:              no error occured          */
/*                  D2_CREDENTIALS_INVALID: user is not registered    */
/*                                          directory or password     */
/*                                          unknown                   */
/*                  D26_ERROR:              any error occurred        */
/*								      */
/*exoff ***************************************************************/

signed16 d26_1_bind(byte *a_in, byte *a_out, signed32 *length,
    unsigned16 cont_id, signed32 usr_id)

{                                     /*  d26_1_bind()                */

  static        char *function = "d26_1_bind";

  byte          *scan = a_in;
  signed16	name_len;
  byte		*cred_info;	    /* pointer to credential info     */

  signed16   desired_sec_mech = -1; /* client's desired sec. mechanism*/
  D2_str	client_token;	    /* client's security token	      */
  D2_str	return_token;	    /* security token returned by sec.*/
				    /* function			      */
  Bool   	master_bind;	    /* flag for master_bind	      */
  signed32	sec_ret_val;	    /* return value of sec. function  */
  D2_sec_status status;		    /* error status of sec. function  */

  D2_name_string ptr;
				    /* length of return token in sec. */
				    /* function 		      */
  const signed16 D26_1_TOKEN_LEN = 4096;
				    /* parameter block for add usrcred*/
  D23_addcrdpb	auc_pb = {{D23_V02,0,0,0},0,0,0,0};

if                                  /* error during DSA initialization*/
   (d26_u09_init_dsa(0))
{                                   /*  return(D26_ERROR)             */
  d27_010_set_error_apdu((D23_rserror *) a_out, D2_PERM_ERR, D2_SYS_ERR,
			  D2_NO_RESPONSE,length);
  return(D26_ERROR);
}                                   

				    /*  trace function entry          */
DCE_SVC_LOG((GDS_S_ENTRY_FUNCTION_MSG,function));
if                              /*  check version number              */
  (!(D2_CHECK_MINMAX (D23_APDUMIN, D23_APDUMAX,
		      ((D23_inbind *)scan)->d23_Aversno)))
{                               /*  set error message                 */
  d27_010_set_error_apdu((D23_rserror *) a_out, D2_CALL_ERR, D2_ILL_VERS,
			   D2_NO_RESPONSE, length);
  DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR));
  return(D26_ERROR);
} 

				/*  initialize anonymous requestor    */
d26_requestor.d26_req_type = D26_NORMAL_REQ;
d26_requestor.d26_sm_flag  = D26_NO_SM_PENDING;
d26_requestor.d26_sec_meth = D2_ANONYMOUS;
strcpy(d26_requestor.d26_req_name,D26_ANONYMOUS);
strcpy(d26_requestor.d26_normed_req_name,D26_ANONYMOUS);

cred_info = scan + D23_S_INB;
				/*  analyse tag                       */
switch (((D23_inbind *)scan)->d23_Acrkind)
{
  case D23_NO_CRED:
			desired_sec_mech = D2_ANONYMOUS;
			client_token.d2_value = NULL;
			client_token.d2_size  = 0;
			break;
  case D23_SIMPLE_CRED:
			desired_sec_mech = D2_SIMPLE;
			client_token.d2_value = cred_info;
			client_token.d2_size  = D23_S_SIMCR + D23_S_NM + 
			((D23_ds_name *)(client_token.d2_value + D23_S_SIMCR))->d23_dslen;
			break;
  case D23_STRONG_CRED:
			desired_sec_mech = D2_STRONG;
			client_token.d2_value = cred_info + D23_S_SSCR;
			client_token.d2_size  = 
				((D23_strcr_info *) cred_info)->d23_strlen;
			break;
  case D23_EXTERNAL_CRED:
			if (((D23_extcr_info *) cred_info)->d23_extkind 
			    == D2_DCE_AUTH)
			{
			  desired_sec_mech = D2_DCE_AUTH;
			  client_token.d2_value = cred_info + D23_S_EXTCR;
			  client_token.d2_size  = 
				((D23_extcr_info *) cred_info)->d23_extlen;
			}
			/* other external mechanisms not yet supported  */
			break;
  default:
			/* cannot occur because APDU is created during	*/
			/* ASN.1 decoding - there are no other values 	*/
			/* for credential kind 				*/
			break;
}

/* check wether desired security mechanism is supported */
if (desired_sec_mech == -1 || d26_auth_mech[desired_sec_mech] == FALSE)
{
    d27_010_set_error_apdu((D23_rserror *) a_out, D2_CALL_ERR, D2_PAR_ERR,
			    D2_INAPPR_AUTH, length);
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR));
    return (D26_ERROR);
}

/* allocate memory for the return token */
if ((return_token.d2_value = D27_MALLOC(svc_c_sev_warning, D26_1_TOKEN_LEN))
     == NULL)
{
    d27_010_set_error_apdu((D23_rserror *) a_out, D2_TEMP_ERR, D2_SYS_ERR,
			    D2_UNWILLING, length);
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR));
    return (D26_ERROR);
}

return_token.d2_size = D26_1_TOKEN_LEN;

ptr = (D2_name_string) d26_requestor.d26_req_name;
sec_ret_val = dsa_accept_sec_ctx(desired_sec_mech, &client_token, 
	&return_token, &ptr, &master_bind, &status);

while (sec_ret_val != D2_NOERROR && status.d2_errvalue == D2_S_EXPIRED_CRED)
{
  /* delete old and acquire new credentials */
  if ((sec_ret_val = dsa_del_cred(desired_sec_mech,&status)) == D2_ERROR)
  {
    DCE_SVC_LOG((GDS_S_DEL_CRED_ERR_MSG,desired_sec_mech,status.logstring));
  }
  else if ((sec_ret_val = dsa_acquire_cred(desired_sec_mech,d26_myname,&status))
	    == D2_ERROR)
  {
    DCE_SVC_LOG((GDS_S_ACQU_CRED_ERR_MSG,desired_sec_mech,status.logstring));
  }

  if (sec_ret_val == D2_ERROR)
  {
    d27_010_set_error_apdu((D23_rserror *) a_out, status.d2_retcode, 
	status.d2_errclass, status.d2_errvalue, length);
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR));
    return (D26_ERROR);
  }

  sec_ret_val = dsa_accept_sec_ctx(desired_sec_mech, &client_token, 
	&return_token, &ptr, &master_bind, &status);
}

if (sec_ret_val != D2_NOERROR)
{
      DCE_SVC_LOG((GDS_S_ACCEPT_ERR_MSG,desired_sec_mech,status.logstring));
      d27_010_set_error_apdu((D23_rserror *) a_out, status.d2_retcode, 
	status.d2_errclass, status.d2_errvalue, length);
      DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR));
      return (D26_ERROR);
}

if                      /* requestor is not anonymous                 */
  (strcmp(d26_requestor.d26_req_name,D26_ANONYMOUS) != 0)
{                       /* put normed user name to d26_requestor      */
  name_len = strlen(d26_requestor.d26_req_name) + 1;
  d27_007_norm_name(d26_requestor.d26_req_name,name_len,
      d26_requestor.d26_normed_req_name);
  d27_012_remove_blanks(d26_requestor.d26_normed_req_name,name_len,
	d26_requestor.d26_normed_req_name);
}
d26_requestor.d26_sec_meth = desired_sec_mech;

if (master_bind)
{
  d26_requestor.d26_req_type = D26_PRIVIL_REQ;
}

/* write bind result */
d26_103_bind_rs(((D23_inbind *)scan)->d23_Acrkind, desired_sec_mech,
	&return_token, a_out,length);

if (return_token.d2_value != NULL)
{
  free(return_token.d2_value);
}

if
  (D23_GAC(cont_id) == D27_DAP_AC || 
   (D23_GAC(cont_id) == D27_DSP_AC && desired_sec_mech != D2_DCE_AUTH))
{ 
  d26_requestor.d26_trust_level = TDT_TRUSTED_FOR_ALL;
}
else
{
  if (dsa_get_tdt_entry((D2_name_string) d26_requestor.d26_req_name,
      &(d26_requestor.d26_trust_level), &status) != D2_NOERROR)
  {
    d27_010_set_error_apdu((D23_rserror *) a_out, status.d2_retcode, 
	status.d2_errclass, status.d2_errvalue, length);
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR));
    return (D26_ERROR);
  }
}

  /* fill parameter block for d23_add_usr_cred */
  auc_pb.d23_Mcontid = cont_id;
  auc_pb.d23_Musrid = usr_id;
  auc_pb.d23_Mcredsize = D26_REQ_LEN;
  auc_pb.d23_Mcredref = (byte *) &d26_requestor;

  if                            /*  error from writing user credential*/
    (d23_add_usr_cred(&auc_pb) != D2_NOERROR)
  {                             /*  write error message               */
    d27_010_set_error_apdu((D23_rserror *) a_out, D2_TEMP_ERR, D2_REM_ERR,
			    D2_NO_RESPONSE, length);
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D26_ERROR));
    return(D26_ERROR);
  } 

D27_022_ENTER_TIME_TRACE(GDS_S_ADD_USRCRED,NULL,0,0);

DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D2_NO_ERR));

return(D2_NO_ERR);

}                               /*  d26_1_bind()                      */

/*inon ****************************************************************/
/*								      */
/* TYPE         : C-FUNCTION                                          */
/*								      */
/* NAME         : d26_103_bind_rs()                                   */
/*								      */
/* AUTHOR       : Volpers,D AP 11                                     */
/* DATE         : 14.01.88                                            */
/*								      */
/* INPUT PARAMETERS:                                                  */
/*                Short         c_kind;     kind of credentials       */
/*                D23_rsbind    *a_out;     BIND-Result-Message       */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*                D23_rsbind    *a_out;     BIND-Result-Message       */
/*                Ushort         s_len;     length of BIND-Result-    */
/*                                          Message                   */
/*								      */
/* RETURN VALUE :                                                     */
/*								      */
/*inoff ***************************************************************/

static void d26_103_bind_rs(
	signed16 c_kind, 		/* IN  - credential kind	*/
	signed16 sec_mech, 		/* IN  - sec. mechanism		*/
	D2_str   *return_token,		/* IN  - return token from sec.fct. */
	byte     *a_out,		/* OUT - result message		*/
	signed32 *s_len)		/* OUT - output length		*/

{
byte          *scan;
signed16       offset;

				/*  enter result message (rsbind)     */
((D23_rsbind *)a_out)->d23_Bhdr.d23_versno = D23_APDUV22;

((D23_rsbind *)a_out)->d23_Bversion = D23_V1988;
((D23_rsbind *)a_out)->d23_Bcrkind = c_kind;
scan = (byte *)a_out + sizeof(D23_rsbind);

/* copy security return token to bind message */
switch ((int) c_kind)
{
  case D23_NO_CRED: 
    offset = 0;
    break;
  case D23_SIMPLE_CRED: 
    ((D23_simcr_info *) scan)->d23_scrtag = D23_SIMTAG;
    ((D23_simcr_info *) scan)->d23_scrfil = 0;
    scan += D23_S_SIMCR;
    offset = return_token->d2_size;
    break;
  case D23_STRONG_CRED: 
    ((D23_strcr_info *) scan)->d23_strtag = D23_STRTAG;
    ((D23_strcr_info *) scan)->d23_strfil = 0;
    ((D23_strcr_info *) scan)->d23_strlen = return_token->d2_size;
    ((D23_strcr_info *) scan)->d23_stroff = D2_ALIGN(return_token->d2_size);
    offset = ((D23_strcr_info *) scan)->d23_stroff;
    scan += D23_S_SSCR;
    break;
  case D23_EXTERNAL_CRED: 
    ((D23_extcr_info *) scan)->d23_exttag =  D23_EXTTAG;
    ((D23_extcr_info *) scan)->d23_extkind = D2_DCE_AUTH;
    ((D23_extcr_info *) scan)->d23_extlen =  return_token->d2_size;
    ((D23_extcr_info *) scan)->d23_extoff =  D2_ALIGN(return_token->d2_size);
    offset = ((D23_extcr_info *) scan)->d23_extoff;
    scan += D23_S_EXTCR;
    break;
  default:
			break;
}

/* copy value of return_token to message */
if (return_token->d2_size > 0)
{
  memcpy(scan, return_token->d2_value, return_token->d2_size);
			/* NOTE: the length of result message is fixed to */
			/* D26_BLOCK_LEN until now. If the security token */
			/* can be bigger than the result_message_len has  */
			/* to be output parameter in d26_1_bind too !!!   */
}

scan += offset;

((D23_eom *)scan)->d23_eomtag = D23_EOMTAG;
((D23_eom *)scan)->d23_eomfil = 0;
scan += D23_S_EOM;

*s_len =  scan - a_out;
}
