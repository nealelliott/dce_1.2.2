/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: d26dread.c,v $
 * Revision 1.1.4.2  1996/02/18  19:45:01  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:26:11  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:43:52  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:06:32  root]
 * 
 * Revision 1.1.2.3  1994/06/09  18:44:54  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:17:41  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:52:02  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:20:24  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:09:11  keutel
 * 	creation
 * 	[1994/03/21  15:52:41  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d26dread.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 19:45:01 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton	***************************************************************/
/*								      */
/* TYPE         : MODULE                                              */
/*								      */
/* NAME         : d26gbind.c                                          */
/*								      */
/* AUTHOR       : Schreck, D AP 11                                    */
/* DATE         : 31.03.89                                            */
/*								      */
/* COMPONENT    : DS                                                  */
/*								      */
/* DOC.-NR.     : DS-Design-Spec.                                     */
/*			.					      */
/*			.					      */
/*								      */
/* PRD#/VERS.   : <Productnr. from Productstructure - <Versionnr.>    */
/*								      */
/* DESCRIPTION  : This module contains the function to build the      */
/*                invoke APDU for bind to remote DSA for daemon       */
/*                process.                                            */
/*								      */
/* SYSTEM DEPENDENCIES: None                                          */
/*								      */
/* HISTORY      :                                                     */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | ID | CR# EM# */
/*   0.1    |31.03.89 |  Original                      | as |         */
/*          |         |                                |    |         */
/*								      */
/*								      */
/*datoff **************************************************************/

/**********************************************************************/
/*								      */
/*			  I N C	L U D E	S			      */
/*								      */
/**********************************************************************/

#include <d26dsa.h>

/*****  Operating-System-Includes  *****/

#include <stdio.h>

/*****  external Includes    *****/

/*****  internal Includes    *****/

#include <dce/d27util.h>
#include <malgcfe.h>

#define DSA
#include <d2asn1.h>

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

static void d26_m16_set_search_pars(byte *msg, byte *s_in);
static boolean d26_m17_read_allowed(byte *msg_in);

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME  :      d26_m15_get_bind_info                                 */
/*								      */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        31.03.89                                              */
/*								      */
/* DESCRIPTION: This function builds the bind invoke APDU for bind    */
/*              to remote DSA which is used by the daemon process     */
/*              of delta-updates.                                     */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*      D23_ingbind     *msg_in        invoke message                 */
/*                                                                    */
/*      Int             *length        INPUT OUTPUT parameter         */
/*                                     -> memory length of msg_out    */
/*                                     <- data length of msg_out      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      D23_inbind     **msg_out       result (or error) message      */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*     Int      D2_NO_ERR     no error occurred                       */
/*                                                                    */
/*              D26_ERROR     any error occurred (error APDU supplied)*/
/*                                                                    */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_m15_get_bind_info(
	byte 		*msg_in, 
	signed32	in_length,
	byte 		**msg_out,
    	signed32 	*length)

{                               /*  d26_m15_get_bind_message          */

  static char *function         = "d26_m15_get_bind_message";

  signed16 return_value = D2_NO_ERR;
  signed16 rv;
				    
  byte     *sm_in;		    /* message blocks for DS_SEARCH   */
  signed32 remlen;                  /* remaining message length       */

				    /* parameters for DS_SEARCH       */
				    /* exact  dereferenced object     */
  byte     exact_obj[D2_DNL_MAX + 1];
  D26_attr_info attr[D2_AT_MAX + 1];
  D26_acl  acl[D2_ACLIDX_MAX];
  signed32 oct_idx[D26_OCL_VAL_COUNT + 1];
  signed32 mk;
  boolean  rec;
  char     *record;
  boolean  al_flag;
  signed16 ob_nr;

  D23_rserror  *err_apdu_ptr = (D23_rserror *) NULL;

/**********/
/* entry  */
/**********/
DCE_SVC_LOG((GDS_S_ENTRY_FUNCTION_MSG,function));

			     /*  initialize variables                  */
/* check wether request is only for DSA-Object with auth.-method and PSAP */
if (d26_m17_read_allowed(msg_in))
{
  if                           /*  error from allocating memory  */
     ((sm_in = (byte *)D27_MALLOC(svc_c_sev_warning,
	in_length + sizeof(D23_srcharg))) == NULL)
  {                               /*  set return_value and error_codes  */
    d27_010_set_error_apdu((D23_rserror *)*msg_out,D2_TEMP_ERR,D2_REM_ERR,
			   D2_UNWILLING,length);
    return_value = D26_ERROR;
  }

  /* set search parameters */
  d26_m16_set_search_pars(msg_in, sm_in);

  /* dread message has no chaining */
  d26_base_object = (D2_name_string)NULL;
  d26_chain_arg = (D23_chain_arg *)NULL;

 /* call internal search */
  if                         /* search was not successful     */
    ((rv = d26_u00_search(FALSE,D23_DREAD,sm_in,length,msg_out,
     &remlen,exact_obj,attr,oct_idx,acl,&mk,&rec,&record,&al_flag,&ob_nr))
     != D2_NO_ERR)
  {                          /* error log */
     *length -= remlen;
     return_value = D26_ERROR;
  }
  else
  {
     *length -= remlen;
  }
}
else
{
  d27_010_set_error_apdu((D23_rserror *)*msg_out,D2_TEMP_ERR,D2_REM_ERR,
			   D2_UNWILLING,length);
  return_value = D26_ERROR;
}


/********/
/* Exit */
/********/
if (return_value != D2_NO_ERR)
{   
     err_apdu_ptr = (D23_rserror *) *msg_out;
} 

DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,return_value));
return(return_value);
}                            /*  d26_m15_get_bind_message             */

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME  :      d26_m16_set_search_pars                               */
/*								      */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        03.04.89                                              */
/*								      */
/* DESCRIPTION: This function builds the inpout message for internal  */
/*              search function.                                      */
/*								      */
/* RETURNVALUE:                                                       */
/*     signed32 D2_NO_ERR     no error occurred                       */
/*                                                                    */
/*exoff ***************************************************************/

static void d26_m16_set_search_pars(
			/* IN    -  invoke message                    */
    byte     *msg,
			/* OUT   -  input message for internal search */
    byte     *s_in)

{                               /*  d26_m16_set_search_pars           */

  byte           *scan;         /* actual pointer to incoming     */
				/* message                        */
  byte           *message;      /* actual pointer to message      */
  D23_com_arg    *com_arg;      /* pointer to common arguments    */



  scan    = msg;
  message = s_in;
				    /*  copy header                   */
  *((D23_header *)message) = *((D23_header *)scan);
  scan    += sizeof(D23_header);
  message += sizeof(D23_header);
  d26_a02_analyse_chaining_args(&scan);

  com_arg     = (D23_com_arg *)message;
  d27_501_copy_common_arguments(&scan,&message);
  com_arg->d23_cmctr.d23_scslm = 1;
				    /*  create search arguments       */
  ((D23_srcharg *)message)->d23_Itag    = D23_ITAG;
  ((D23_srcharg *)message)->d23_Iobjsub = D2_BASE_OBJECT;
  message += sizeof(D23_srcharg);
  d27_502_copy_distinguished_name(&scan,&message);
  d26_c07_copy_req_attributes(&scan,&message);
  ((D23_eom *)message)->d23_eomtag = D23_EOMTAG;

}                               /*  d26_m16_set_search_pars           */

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME  :      d26_m17_read_allowed                                  */
/*								      */
/* AUTHOR:      Loose, BU BA NM123				      */
/* DATE:        20.01.94                                              */
/*								      */
/* DESCRIPTION: This function looks for the attribute information     */
/*              of PSAP and authentication mechanism. Others are      */
/*		not allowed!					      */
/*								      */
/* RETURNVALUE:                                                       */
/*     boolean  TRUE		read request allowed		      */
/*		FALSE		read request not allowed	      */
/*                                                                    */
/*exoff ***************************************************************/

static boolean d26_m17_read_allowed(
    byte *msg_in)	/* IN    -  incoming DREAD message		*/

{                               /*  d26_m17_find_attribute            */

  boolean return_value = TRUE;

  byte        *msg_ptr;    /* pointer to ipc message block   */
  D23_ds_name *ds_name;
  D23_at_type *at_type;
  D2_a_type    type;


/* go to attribute information */
msg_ptr = msg_in + D23_S_INR;
d26_a02_analyse_chaining_args(&msg_ptr);
d26_a03_analyse_common_arguments (&msg_ptr);
ds_name = (D23_ds_name *) msg_ptr;
msg_ptr = (byte *) (ds_name + 1) + ds_name->d23_dsoff;
msg_ptr += D23_S_REQ;

at_type = (D23_at_type *) msg_ptr;

/* look for attribute */
while (at_type->d23_atttag == D23_ATTTAG)
{   
  type.d2_typ_len = at_type->d23_atlen;
  type.d2_type    = (Ob_id_string) (at_type + 1);

  /* check wether type is one of the allowed ones */
  if (D2_ATTCMP(&type,D2_PSAP_ADDRESS) && D2_ATTCMP(&type, D2_AUTH_MECH) &&
      D2_ATTCMP(&type,D2_PR_NM))
  {
    return_value = FALSE;
    break;
  }

  at_type = (D23_at_type *) ((byte *) (at_type + 1) + at_type->d23_atoff);
}

return(return_value);

}                               /*  d26_m17_read_allowed               */
