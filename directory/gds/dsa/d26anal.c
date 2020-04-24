/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d26anal.c,v $
 * Revision 1.1.10.2  1996/02/18  19:44:48  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:25:55  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:42:50  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:06:15  root]
 * 
 * Revision 1.1.8.5  1994/07/06  15:06:50  marrek
 * 	July 1994 code drop.
 * 	[1994/07/05  09:10:27  marrek]
 * 
 * Revision 1.1.8.4  1994/06/21  14:45:09  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  11:52:07  marrek]
 * 
 * Revision 1.1.8.3  1994/05/10  15:51:47  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:19:34  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:09:05  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:19:47  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  16:05:09  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:08:41  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  13:37:29  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  13:01:32  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  18:58:52  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:18:13  bbelch]
 * 
 * Revision 1.1.4.3  1992/11/27  19:34:34  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  11:03:55  marrek]
 * 
 * Revision 1.1.4.2  1992/09/23  11:28:13  marrek
 * 	Add sizeof(D23_ds_name) in strcpy.
 * 	[1992/09/23  11:26:15  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:45:26  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:53:05  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d26anal.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:44:48 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODULE                                              */
/*                                                                    */
/* NAME         : d26anal.c                                           */
/*                                                                    */
/* AUTHOR       : E. Kraemer, D AP 11                                 */
/* DATE         : 24.09.90                                            */
/*                                                                    */
/* COMPONENT    : DIR-X, DSA, retrieval functions                     */
/*                                                                    */
/* DESCRIPTION  : The module contains functions, which analyse APDUs  */
/*                                                                    */
/*datoff **************************************************************/

#include <d26dsa.h>

/*****  Operating-System-Includes  *****/

#include <sys/types.h>
#include <stdlib.h>
#include <signal.h>

/*****  external Includes    *****/

#include <dce/d27util.h>

/*****  internal Includes    *****/

#include <d26schema.h>

/*****  local defines        *****/

#define D26L_FCOMMENT    64

/*****  local    functions   *****/

static boolean d26_a09_target_in_trace(byte *message, D2_name_string *target,
    D23_av_value **target_ep);
static boolean d26_a10_cmp_targ_for_loop(D2_name_string target,
    D23_av_value *target_ep, D23_op_pr_info *target_op,
    D2_name_string base_object, D23_av_value *bo_ep, D23_op_pr_info *bo_op);

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_a01_analyse_search_message                      */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 25.01.88                                            */
/*                                                                    */
/* DESCRIPTION  : This function sets global pointers to the message   */
/*                for the base object, the chaining arguments, the    */
/*                common arguments, the search arguments. Pointers to */
/*                the requested attributes and to the filter          */
/*                attributes will be set too.                         */
/*                                                                    */
/* INPUT PARAMETERS:                                                  */
/*      byte          *input;       pointer to DS_SEARCH invoke       */
/*                                  Message                           */
/*      String        *output;      result data or error data         */
/*      Int           *output_len;  length of memory for result data  */
/*      Short         op_id         operation ID                      */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*      Int           *output_len;  length of memory for result data  */
/*      String        *output;      result data or error data         */
/*      D2_name_string norm_base_obj    normed base object            */
/*      D23_rq_info   **r_info;     requested informations            */
/*      D23_ft1_info  **f_info;     filter attributes                 */
/*                                                                    */
/* RETURN VALUE :   Short                                             */
/*      D26_ERROR:              wrong IPC-version detected            */
/*      D2_ADM_LIM_EXCEEDED:    Too much requested attributes         */
/*      D2_LOOP_DETECTED   :    Loop detected                         */
/*      D2_TIME_LIMIT_EXCEEDED  time limit exceeded                   */
/*      D2_UNAV_CRIT_EXTENSION: Unavailable critical extension        */
/*                              required                              */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_a01_analyse_search_message(byte *message, byte **output,
    signed32 *output_len, D2_name_string norm_base_obj, D23_rq_info **r_info,
    D23_ft1_info **f_info, signed16 op_id)

{                                   /*  d26_a01_analyse_search_message*/

  Bool     name_complete;           /* indicates whether DN could be  */
				    /* decodeded completely           */
  signed16 name_len;                /* length of base object          */
  signed16 ret_value;               /* return value                   */

  register signed16   i;            /* loop variable                  */

message += sizeof(D23_header);
if                                  /* error in common arguments      */
  ((ret_value = d26_a03_analyse_common_arguments(&message)) != D2_NO_ERR)
{                                   /* check return value             */
  if                                /* time limit exceeded            */
    (ret_value == D2_TIME_LIMIT_EXCEEDED)
  {                                 /* set a temporary error message  */
    d27_010_set_error_apdu((D23_rserror *)*output,D2_TEMP_ERR,D2_REM_ERR,
      ret_value,output_len);
  }
  else
  {                                 /* set a call error message       */
    d27_010_set_error_apdu((D23_rserror *)*output,D2_CALL_ERR,D2_PAR_ERR,
      ret_value,output_len);
  }
  return(ret_value);
}

				    /*  assign and trace search       */
				    /*  arguments                     */
d26_search_arg = (D23_srcharg *)message;
if                                  /*  chaining arguments are present*/
  (d26_chain_arg != (D23_chain_arg *)NULL && op_id == D23_SEARCH)
{                                   /*  set subset from chaining args */
  d26_subset = d26_chain_arg->d23_entry_only == TRUE ?
    D2_BASE_OBJECT : d26_search_arg->d23_Iobjsub;
}
else
{                                   /*  set subset from search        */
				    /*  arguments only                */
  d26_subset = d26_search_arg->d23_Iobjsub;
}

message += sizeof(D23_srcharg);

name_complete = ((D23_ds_name *)(message))->d23_dscomplete;
if                                  /*  DAP-message-base-object is not*/
				    /*  ROOT                          */
  (((D23_ds_name *)message)->d23_dslen > 0)
{                                   /*  set DAP-base-object           */
  d26_dap_b_object = (D2_name_string)(message + sizeof(D23_ds_name));
}
else
{
  d26_dap_b_object = (D2_name_string)"";
}

message += sizeof(D23_ds_name) + ((D23_ds_name *)message)->d23_dsoff;

				    /* base object not decoded        */
				    /* completely                     */
if(name_complete == FALSE)
{                                   /* set pointer to undecoded part  */
  d26_dap_bo_ep = (D23_av_value *)message;
}
else
{                                   /* set undecoded part to NULL     */
  d26_dap_bo_ep = (D23_av_value *)NULL;
}
				    /* check whether base object known*/
if(d26_base_object == (D2_name_string)NULL)
{                                   /* base object not known from dsp */
				    /* but from dap                   */
  d26_base_object     = d26_dap_b_object;
  d26_bo_encoded_part = d26_dap_bo_ep;
}

if(name_complete == FALSE)
{                                   /* reset message pointer          */
  message += sizeof(D23_av_value) + ((D23_av_value *)message)->d23_avoff;
}

name_len = strlen((char *)d26_base_object) + 1;
d27_007_norm_name((char *)d26_base_object,name_len,(char *)norm_base_obj);
d27_012_remove_blanks((char *)norm_base_obj,name_len,(char *)norm_base_obj);
d27_018_sort_name(norm_base_obj);
if                                  /* loop is detected               */
  (d26_a08_loop_detected(d26_myname,(D23_av_value *)NULL,norm_base_obj,
   d26_bo_encoded_part,d26_op_progress) == TRUE)
{                                   /* return error                   */
  d27_010_set_error_apdu((D23_rserror *)*output,D2_CALL_ERR,D2_PAR_ERR,
			 D2_LOOP_DETECTED,output_len);
  return(D2_LOOP_DETECTED);
}                                   /*                                */

*r_info = (D23_rq_info *)message;

message += sizeof(D23_rq_info);
				    /* skip attribute types           */
for(i = 0; i < (*r_info)->d23_rqnbr; i++)
{
  message += sizeof(D23_at_type) + ((D23_at_type *)message)->d23_atoff;
}
				    /*  set filter information        */
*f_info = (D23_ft1_info *)message;

				    /*  return NO ERROR               */
return(D2_NO_ERR);

}                                   /*  d26_a01_analyse_search_message*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_a02_analyse_chaining_args                       */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 24.09.90                                            */
/*                                                                    */
/* DESCRIPTION  : This function will check, whether chaining arguments*/
/*                are present. If so, it will check the trace         */
/*                information for loops, sets the requestor to the    */
/*                originator and the base object to the target object.*/
/*                                                                    */
/* INPUT PARAMETERS:                                                  */
/*                String        *message;   pointer to DS_SEARCH      */
/*                                          Invoke Message            */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*                String        *message;   pointer to DS_SEARCH      */
/*                                          Invoke Message            */
/*                                                                    */
/* RETURN VALUE :   None                                              */
/*                                                                    */
/*exoff ***************************************************************/

void d26_a02_analyse_chaining_args(byte **message)

{                                   /*  d26_a02_analyse_chaining_args */
  Bool    name_complete;            /* indicates whether DN could be  */
				    /* decodeded completely           */
  signed16 name_len;                /* length of distinguished name   */

  d26_base_object     = (D2_name_string)NULL;
  d26_bo_encoded_part = (D23_av_value *)NULL;
  d26_time_limit      = 0;
  if                                /*  chaining arguments not present*/
    (((D23_chain_arg *)(*message))->d23_chtag != D23_CHTAG)
  {                                 /*  set it to 0                   */
    d26_chain_arg = (D23_chain_arg *)NULL;
  }
  else                              /*  set chaining argument pointer */
  {
    d26_chain_arg   = (D23_chain_arg *)*message;
    d26_al_rdns     = d26_chain_arg->d23_al_RDN;
    d26_time_limit  = d26_chain_arg->d23_chtmlt;
    d26_op_progress = &d26_chain_arg->d23_op_prog;

    *message += sizeof(D23_chain_arg);

			    /* scan trace-information                 */
    while(((D23_trace_info *)(*message))->d23_trtag == D23_TRTAG)
    {                       /* still trace information present        */
			    /* scan trace information structure       */
      *message += sizeof(D23_trace_info);
			    /* scan DSA-name and target-object name   */
      while(((D23_ds_name *)*message)->d23_dstag == D23_NAMTAG)
      {                     /* still distinguished name present       */
	name_complete = ((D23_ds_name *)(*message))->d23_dscomplete;
			    /* scan distinguished name                */
	*message += sizeof(D23_ds_name) +
		    ((D23_ds_name *)*message)->d23_dsoff;
			    /* check completeness of DN               */
	if(name_complete == FALSE)
	{                   /* scan undecoded part of DN              */
	  *message += sizeof(D23_av_value) +
		      ((D23_av_value *)*message)->d23_avoff;
	}
      }
    }

			    /* initialize requestor anonymous         */
    d26_requestor.d26_req_type = D26_NORMAL_REQ;
    d26_requestor.d26_sm_flag  = D26_SM_UNKNOWN;
    {
      strcpy(d26_requestor.d26_req_name,D26_ANONYMOUS);
      strcpy(d26_requestor.d26_normed_req_name,D26_ANONYMOUS);
    }
			    /* scan originator                        */
    if(((D23_ds_name *)*message)->d23_dstag == D23_RQTAG)
    {                       /* originator present                     */
			    /* check whether originator completely    */
			    /* decoded                                */
      name_complete = ((D23_ds_name *)*message)->d23_dscomplete;
      if(name_complete == TRUE)
      {                     /* copy it to the requestor               */
	name_len = ((D23_ds_name *)*message)->d23_dslen;
	strcpy(d26_requestor.d26_req_name,
	  (char *)*message + sizeof(D23_ds_name));
	d27_007_norm_name(d26_requestor.d26_req_name,name_len,
			  d26_requestor.d26_normed_req_name);
	d27_012_remove_blanks(d26_requestor.d26_normed_req_name,name_len,
			      d26_requestor.d26_normed_req_name);
	d27_018_sort_name((D2_name_string)d26_requestor.d26_normed_req_name);
	if (d26_requestor.d26_sec_meth == D2_DCE_AUTH)
	{
	  if (!strcmp(d26_requestor.d26_acl_name,D26_NOT_TRUSTED))
	  {
	    strcpy(d26_requestor.d26_acl_name,D26_ANONYMOUS);
	  }
	  else if (strcmp(d26_requestor.d26_acl_name,D26_ANONYMOUS))
	  {
	    strcpy(d26_requestor.d26_acl_name,d26_requestor.d26_normed_req_name);
	  }
    }
	else
	{
	  strcpy(d26_requestor.d26_acl_name,d26_requestor.d26_normed_req_name);
	}
      }
      *message += sizeof(D23_ds_name) +
		  ((D23_ds_name *)(*message))->d23_dsoff;
      if(name_complete == FALSE)
      {                     /* scan undecoded part of DN              */
	*message += sizeof(D23_av_value) +
		    ((D23_av_value *)*message)->d23_avoff;
      }
    }
	else
	{
      strcpy(d26_requestor.d26_acl_name,D26_ANONYMOUS);
	}

			    /* scan target object                     */
    if(((D23_ds_name *)*message)->d23_dstag == D23_BONTAG)
    {                       /* target object present                  */
			    /* use it as base object                  */
      if                    /* target object is not root              */
	(((D23_ds_name *)*message)->d23_dslen > 0)
      {                     /* set target object to message           */
	d26_base_object = (D2_name_string)((D23_ds_name *)*message + 1);
      }
      else
      {
	d26_base_object = (D2_name_string)"";
      }
      name_complete = ((D23_ds_name *)*message)->d23_dscomplete;
      *message += sizeof(D23_ds_name) +
		  ((D23_ds_name *)(*message))->d23_dsoff;
			    /* undecoded part is following            */
      if(name_complete == FALSE)
      {                     /* set pointer to undecoded part          */
	d26_bo_encoded_part = (D23_av_value *)*message;
	*message += sizeof(D23_av_value) +
		    ((D23_av_value *)*message)->d23_avoff;
      }
    }

			    /* scan domain information                */
    if(((D23_dom_info *)(*message))->d23_dmtag == D23_DMTAG)
    {                       /* domain information present, skip it    */
      *message += sizeof(D23_dom_info);
    }

			    /* scan security parameters               */
    if(((D23_se_parm *)(*message))->d23_setag == D23_SECTAG)
    {                       /* security parameters present, skip them */
      *message += sizeof(D23_se_parm);
    }
  }

}                                   /*  d26_a02_analyse_chaining_args */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_a03_analyse_common_arguments                      */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        23.03.88                                              */
/*                                                                    */
/* DESCRIPTION: This function analyses the common arguments and sets  */
/*              appropriate global variables.                         */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*              message     String *        message containing        */
/*                                          common arguments          */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              message     String *        message after common      */
/*                                          arguments                 */
/*                                                                    */
/* RETURNVALUE:         Short                                         */
/*      D2_NO_ERR                       no error occurred             */
/*      D2_TIME_LIMIT_EXCEEDED          time limit exceeded           */
/*      D2_UNAV_CRIT_EXTENSION          critical extensions detected  */
/*                                                                    */
/*inoff ***************************************************************/

signed16 d26_a03_analyse_common_arguments(byte **message)

{                               /*  d26_a03_analyse_common_arguments  */

				/* structure for sigaction call       */
  static struct sigaction siginfo = {0};
  D23_com_arg *com_arg;         /* common arguments                   */
  signed16    i,offset;         /* offset of bitstring                */
  signed32    *bit_portion;     /* portion of bitstring               */
				/* memory for log data                */
  Bool        name_complete;    /* indicates whether DN could be      */
				/* decodeded completely               */
  signed16    name_len;         /* length of distinguished name       */
				/* return value                       */
  signed16    ret_value = D2_NO_ERR;

  com_arg = (D23_com_arg *)(*message);
  d26_serv_contr  = &com_arg->d23_cmctr;

  if                    /*  chaining arguments are not present        */
    (d26_chain_arg == NULL)
  {                     /*  assign aliased rdns and operation progress*/
    d26_al_rdns     = com_arg->d23_cmali;
    d26_time_limit  = d26_serv_contr->d23_sctlm;
    d26_op_progress = &com_arg->d23_cmopr;
  }

  d26_time_ex = FALSE;
  if                    /*  no time given                             */
    (d26_time_limit == 0)
  {                     /*  set return value                          */
    ret_value = D2_TIME_LIMIT_EXCEEDED;
  }
  else if               /*  time is limited                           */
	 (d26_time_limit > 0)
  {                     /*  wind alarm clock                          */
    siginfo.sa_handler = d26_a07_set_time_ex;
    sigemptyset(&siginfo.sa_mask);
    D27_SIGACTION(svc_c_sev_warning,SIGALRM,&siginfo,
      (struct sigaction *)NULL);
    alarm((unsigned)d26_serv_contr->d23_sctlm);
  }

  *message += sizeof(D23_com_arg);

			    /* scan security parameters               */
  if                        /* security parameters present            */
    (((D23_se_parm *)(*message))->d23_setag == D23_SECTAG)
  {                         /* skip them                              */
    *message += sizeof(D23_se_parm);
  }
			    /* scan requestor                         */
  if                        /* requestor  present                     */
    (((D23_ds_name *)*message)->d23_dstag == D23_RQTAG)
  {                         /* check whether dsp is processed         */
    name_complete = ((D23_ds_name *)*message)->d23_dscomplete;
    if(d26_chain_arg != (D23_chain_arg *)NULL && name_complete == TRUE)
    {                       /* chaining arguments are present         */
      name_len = ((D23_ds_name *)*message)->d23_dslen;
			    /* copy the originator to requestor       */
      strcpy(d26_requestor.d26_req_name,(char *)*message);
      d27_007_norm_name(d26_requestor.d26_req_name,name_len,
			d26_requestor.d26_normed_req_name);
      d27_012_remove_blanks(d26_requestor.d26_normed_req_name,name_len,
			    d26_requestor.d26_normed_req_name);
      d27_018_sort_name((D2_name_string)d26_requestor.d26_normed_req_name);
    }
    *message += sizeof(D23_ds_name) + ((D23_ds_name *)(*message))->d23_dsoff;
			    /* originator incompletely decoded        */
    if(name_complete == FALSE)
    {                       /* reset message pointer by undecoded part*/
      *message += sizeof(D23_av_value) +
		  ((D23_av_value *)*message)->d23_avoff;
    }
  }

			    /* scan extensions                        */
  if                        /* Bit string is present                  */
    (((D23_bit_info *)(*message))->d23_btag == D23_BITTAG)
  {                         /* reset message pointer                  */
    offset    = ((D23_bit_info *)*message)->d23_boff;
    *message += sizeof(D23_bit_info);
			    /* check octets                           */
    for(i = 0, bit_portion = (signed32 *)*message; i < offset;
	i += sizeof(signed32), bit_portion += sizeof(signed32))
    {                       /* critical extension found               */
      if(*bit_portion != 0L)
      {                     /* return error                           */
	ret_value = D2_UNAV_CRIT_EXTENSION;
      }
    }
    *message += offset;
  }

  return(ret_value);

}                       /*  d26_a03_analyse_common_arguments          */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_a05_analyse_filter()                            */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 05.02.88                                            */
/*                                                                    */
/* DESCRIPTION  : This function will analyse the filter information   */
/*                from message. If it contains any attribute, that    */
/*                may not be assigned to any object, an indicator for */
/*                this object will be set.                            */
/*                                                                    */
/* INPUT PARAMETERS:                                                  */
/*      b_obj       D2_name_string  DN of base object of search       */
/*      f_info      D23_ft1_info *  pointer to filter information in  */
/*                                  message                           */
/*      subset      D2_subset       subset flag                       */
/*      output_len  Int *           maximum length of message to write*/
/*      remlen      Int *           remaining length for result data  */
/*      fct_tag     Short           indicates calling function        */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*      output      String *        pointer to beginning of message   */
/*      output_len  Int *           maximum length of message to write*/
/*      remlen      Int *           remaining length for result data  */
/*                                                                    */
/* RETURN VALUE :   D2_NO_ERR               matching objects may exist*/
/*                  D2_ATT_ERROR            any attribute problem     */
/*                                          occurred                  */
/*                  D2_ADM_LIM_EXCEEDED     too much filter items     */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_a05_analyse_filter(D2_name_string b_obj, D23_ft1_info *f_info,
    byte **output, D2_subset subset, signed32 *output_len, signed32 *remlen,
    signed16 fct_tag)

{                                   /*  d26_a05_analyse_filter        */

				    /* return value                   */
  signed16 ret_value = D2_NO_ERR;
  D23_ft1_info *f1_info;            /* pointer to Filter structure    */
  signed16     count = 0;           /* count of filter items          */

f1_info  = f_info;

if                                  /*  filter is present             */
  (f1_info->d23_fp1tag == D23_PATTAG)
{                                   /*  check filter structure        */
  ret_value = d26_f01_check_filter(b_obj,&count,&f1_info,output,subset,
    output_len,remlen,fct_tag);
} 

return(ret_value);

}                                   /*  d26_a05_analyse_filter        */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_a06_write_filter_error()                        */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 26.02.89                                            */
/*                                                                    */
/* DESCRIPTION  : This function will write the error message for the  */
/*                erroneous filter information.                       */
/*                                                                    */
/* INPUT PARAMETERS:                                                  */
/*      b_obj       D2_name_string  DN of base object of search       */
/*      output      String *        pointer to beginning of message   */
/*      output_len  Int *           maximum length of result message  */
/*      remlen      Int *           remaining length for result data  */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*      output      String *        pointer to beginning of message   */
/*      output_len  Int *           length of result message          */
/*      remlen      Int *           remaining length for result data  */
/*                                                                    */
/* RETURN VALUE :   None                                              */
/*                                                                    */
/*inoff ***************************************************************/

void d26_a06_write_filter_error(D2_name_string b_obj, D26_f_err *f_err,
    byte **output, signed32 *output_len, signed32 *remlen)

{                                   /*  d26_a06_write_filter_error    */

  D23_fi1_info *f_item;     /* indicates erroneous filter item        */
  D23_at_type  *f_type;     /* indicates erroneous filter type        */
  D23_av_value *f_value;    /* indicates erroneous filter value       */
  D23_fs1_info *f_substr;   /* indicates erroneous substrings         */
  D2_a_type    type;        /* erroneous attribute type               */
  signed16     at_idx;      /* AT-index of erroneous attribute type   */
  signed16     len;         /* erroneous filter length                */
  unsigned16   rep;         /* erroneous filter representation        */
  byte *value;              /* erroneous filter value                 */
  signed32 meslen = 0;      /* message length                         */

				    /* set pointers to filter item    */
				    /* and type                       */
f_item = f_err->d26_filter;
f_type = (D23_at_type *)(f_item + 1);

				    /* construct attribute type       */
type.d2_typ_len = (signed16)(f_type->d23_atlen);
type.d2_type    = (Ob_id_string)(f_type + 1);

				    /* construct length and value     */
switch(f_item->d23_fi1match)
{
  case D2_PRESENT:
    len   = 0;
    rep   = f_item->d23_fi1syntx;
    value = (byte *)"";
    break;
  case D2_EQUAL:
  case D2_APPROX_MATCH:
    f_value = (D23_av_value *)((byte *)(f_type + 1) + f_type->d23_atoff);
    len   = f_value->d23_avlen;
    rep   = f_value->d23_avrep;
    value = (byte *)(f_value + 1);
    if                              /* attribute known in AT          */
      ((at_idx = d26_u03_attr_search(&type)) >= 0)
    {                               /* check attribute syntax         */
      if                            /* password syntax                */
	(d26_at[at_idx].d26a_syntax == D2_PASSWD_STX)
      {                             /* rescramble the password        */
	d26_u18_scramble_string(value,value,(signed16)len);
      }                             /*                                */
    }                               /*                                */
    break;
  case D2_SUBSTRINGS:
    f_substr = (D23_fs1_info *)((byte *)(f_type + 1) + f_type->d23_atoff);
    len   = 0;
    rep   = f_substr->d23_fs1rep;
    value = (byte *)(f_substr + 1);
    while(f_substr->d23_fs1tag == D23_FISTAG)
    { 
      len += f_substr->d23_fs1len;
      f_substr = (D23_fs1_info *)((String)
	(f_substr + 1) + f_substr->d23_fs1off);
    } 
    break;
}   

d26_u15_attr_error(output,b_obj,1,f_err->d26_error,&type,
  f_item->d23_fi1syntx,rep,len,value,output_len,&meslen);
*remlen -= meslen;

}                                   /*  d26_a06_write_filter_error    */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_a07_set_time_ex                                   */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        23.03.88                                              */
/*                                                                    */
/* DESCRIPTION: This function sets the time exceeded flag, when time  */
/*              limit is exceeded.                                    */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      sig         int         encountered signal                    */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      None                                                          */
/*                                                                    */
/* RETURNVALUE:         None                                          */
/*                                                                    */
/*inoff ***************************************************************/

void d26_a07_set_time_ex(int sig)

{                               /* d26_a07_set_time_ex                */

  d26_time_ex = TRUE;

}                               /* d26_a07_set_time_ex                */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_a08_loop_detected()                               */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        03.06.91                                              */
/*                                                                    */
/* DESCRIPTION: This function checks the chaining arguments for loops.*/
/*                                                                    */
/* INPUT-PARAMETERS:    					      */
/*      dsa_name            D2_name_string      own DSA name          */
/*      dsa_ep              D23_av_value *      encoded part of DSA   */
/*                                              name                  */
/*      base_object         D2_name_string      actual base object    */
/*      bo_ep               D23_av_value *      encoded part of base  */
/*                                              object                */
/*      bo_op               D23_op_pr_info *    operation progress of */
/*                                              base object           */
/*                                                                    */
/* OUTPUT-PARAMETERS:   None                                          */
/*                                                                    */
/* RETURNVALUE:         None                                          */
/*                                                                    */
/* STANDARD-HEADER: None                                              */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*      d26_chain_arg       D23_chain_arg *     chaining arguments    */
/*      d26_dap_b_object    D2_name_string      name of base object   */
/*                                              from DAP-message      */
/*      d26_dap_bo_ep       D23_av_value *      undecoded part of     */
/*                                              DAP-base object       */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*      d26_a09_target_in_trace()                                     */
/*      d26_a10_cmp_targ_for_loop()                                   */
/*      d27_007_norm_name()                                           */
/*      d27_012_remove_blanks()                                       */
/*      d27_018_sort_name()                                           */
/*                                                                    */
/* NOTES:                                                             */
/*                                                                    */
/*exoff ***************************************************************/

boolean d26_a08_loop_detected(D2_name_string dsa_name, D23_av_value *dsa_ep,
    D2_name_string base_object, D23_av_value *bo_ep, D23_op_pr_info *bo_op)

{                               /*  d26_a08_loop_detected             */

  byte           *message;      /* pointer to scan trace information  */
				/* indicates whether DN could be      */
  Bool           name_complete; /* completely decoded                 */

  boolean        dsas_differ;   /* indicates whether DSA-names differ */
  signed16       offset;        /* offset to next structure           */
  signed16       length;        /* length of APDU structure           */
  byte           *trace_info;   /* trace information to scan          */
				/* target object of last matching     */
  D2_name_string target;        /* trace information                  */

  D23_av_value   *target_ep;    /* encoded part of target object      */
  signed16       target_len;    /* length of target object            */
				/* normed name of target object       */
  byte           norm_target[D2_DNL_MAX];
				/* normed name of chaining DSA        */
  byte           norm_ch_dsa[D2_DNL_MAX];

			    /* set message pointer to chaining        */
			    /* arguments or leave                     */
if(d26_chain_arg != (D23_chain_arg *)NULL)
{
  message    = (byte *)(d26_chain_arg + 1);

			    /* find matching trace-information        */
  while(((D23_trace_info *)message)->d23_trtag == D23_TRTAG)
  {                         /* still trace information present        */
			    /* skip trace information structure       */
    trace_info = message;
    message += sizeof(D23_trace_info);
    name_complete = ((D23_ds_name *)message)->d23_dscomplete;
    length   = ((D23_ds_name *)message)->d23_dslen;
    offset   = ((D23_ds_name *)message)->d23_dsoff;
			    /* skip name structure                    */
    message += sizeof(D23_ds_name);
    dsas_differ = FALSE;
    d27_007_norm_name((char *)message,length,(char *)norm_ch_dsa);
    d27_012_remove_blanks((char *)norm_ch_dsa,length,(char *)norm_ch_dsa);
    d27_018_sort_name(norm_ch_dsa);
    if                      /* no decoded part of DSA name            */
      (length == 0)
    {                       /* check length of compare DSA name       */
      if                    /* decoded parts of compare DSA present   */
	(strlen((char *)dsa_name) != 0)
      {                     /* set DSA names different                */
	dsas_differ = TRUE;
      }
    }
    else if                 /* decoded parts of DSA-names differ      */
	   (strcmp((char *)norm_ch_dsa,(char *)dsa_name))
    {                       /* set DSA names different                */
      dsas_differ = TRUE;
    }
    message += offset;
    if                      /* DSA in trace info has encoded parts    */
      (name_complete == FALSE)
    {                       /* reset offset                           */
      length   = ((D23_av_value *)message)->d23_avlen;
      offset   = ((D23_av_value *)message)->d23_avoff;
      message += sizeof(D23_av_value);
      if                    /* DSA name difference not yet detected   */
	(dsas_differ == FALSE)
      {                     /* check whether compare DSA has encoded  */
			    /* nameparts too                          */
	if(dsa_ep == (D23_av_value *)NULL)
	{                   /* set DSA names different                */
	  dsas_differ = TRUE;
	}
	else if             /* length of encoded parts differ         */
	       (length != dsa_ep->d23_avlen)
	{                   /* set DSA names different                */
	  dsas_differ = TRUE;
	}
	else if             /* contents of encoded parts differ       */
	       (memcmp((byte *)(dsa_ep + 1),message,length))
	{                   /* set DSA names different                */
	  dsas_differ = TRUE;
	}
      }
      message += offset;
    }
    else if                 /* DSA name difference not yet detected   */
	   (dsas_differ == FALSE)
    {                       /* check whether compare DSA has encoded  */
			    /* nameparts                              */
      if(dsa_ep != (D23_av_value *)NULL)
      {                     /* set DSA names different                */
	dsas_differ = TRUE;
      }
    }
    if                      /* DSA names match                        */
      (dsas_differ == FALSE)
    {                       /* determine target object                */
      if(trace_info == (byte *)(d26_chain_arg + 1))
      {                     /* reset target object                    */
	target    = d26_dap_b_object;
	target_ep = d26_dap_bo_ep;
      }
      else if               /* target object is not in trace info     */
	     (d26_a09_target_in_trace(trace_info,&target,&target_ep)
	      == FALSE)
      {                     /* use target object of chaining arguments*/
	target    = base_object;
	target_ep = bo_ep;
      }
      target_len = strlen((char *)target) + 1;
      d27_007_norm_name((char *)target,target_len,(char *)norm_target);
      d27_012_remove_blanks((char *)norm_target,target_len,
	(char *)norm_target);
      d27_018_sort_name(norm_target);

      if                    /* base object leads to a loop with this  */
			    /* target object                          */
	(d26_a10_cmp_targ_for_loop(norm_target,target_ep,
	 &((D23_trace_info *)trace_info)->d23_op_prog,base_object,bo_ep,
	 bo_op) == TRUE)
      {
	return(TRUE);
      }
    }
  }
}

return(FALSE);

}                       /*  d26_a08_loop_detected                     */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_a09_target_in_trace                               */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        07.09.92                                              */
/*                                                                    */
/* DESCRIPTION: This function determines the - eventually implied -   */
/*              value of the target object in the trace information   */
/*                                                                    */
/* INPUT-PARAMETERS:    					      */
/*      message             String              pointer to trace      */
/*                                              information in message*/
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      target              D2_name_string *    target object         */
/*      target_ep           D23_av_value **     encoded part of target*/
/*                                              object                */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      D2_TRUE             Bool                target object found   */
/*      D2_FALSE                                no target object found*/
/*                                                                    */
/*enoff ***************************************************************/

static boolean d26_a09_target_in_trace(byte *message, D2_name_string *target,
    D23_av_value **target_ep)

{                               /*  d26_a09_target_in_trace           */

  Bool     name_complete;   /* indicates whether DN could be decoded  */
			    /* completely                             */
  signed16 offset;          /* offset to next structure               */

			    /* scan all following trace informations  */
while(((D23_trace_info *)message)->d23_trtag == D23_TRTAG)
{                           /* still trace information present        */
			    /* skip trace information structure       */
  message += sizeof(D23_trace_info);
  name_complete = ((D23_ds_name *)message)->d23_dscomplete;
  offset   = ((D23_ds_name *)message)->d23_dsoff;
			    /* skip name structure                    */
  message += sizeof(D23_ds_name) + offset;
  if                        /* DSA in trace info has encoded parts    */
    (name_complete == FALSE)
  {                         /* reset offset                           */
    offset   = ((D23_av_value *)message)->d23_avoff;
    message += sizeof(D23_av_value) + offset;
  }
			    /* check whether target-object following  */
  if(((D23_ds_name *)message)->d23_dstag == D23_NAMTAG)
  {                         /* still distinguished name present       */
    offset        = ((D23_ds_name *)message)->d23_dsoff;
    name_complete = ((D23_ds_name *)message)->d23_dscomplete;
			    /* skip distinguished name                */
    *target  = (D2_name_string)(offset > 0 ?
      message + sizeof(D23_ds_name) : NULL);
    message += offset;
    *target_ep = (D23_av_value *)(name_complete == FALSE ? message : NULL);
    return(TRUE);
  }
}

return(FALSE);

}                       /*  d26_a09_target_in_trace                   */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_a10_cmp_targ_for_loop()                           */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        08.09.92                                              */
/*                                                                    */
/* DESCRIPTION: This function checks whether the base object would    */
/*              cause a loop for the found target object              */
/*                                                                    */
/* INPUT-PARAMETERS:    					      */
/*      target              D2_name_string      target object of trace*/
/*                                              information           */
/*      target_ep           D23_av_value *      encoded part of target*/
/*                                              object                */
/*      target_op           D23_op_pr_info *    operation progress of */
/*                                              target object         */
/*      base_object         D2_name_string      actual base object    */
/*      bo_ep               D23_av_value *      encoded part of base  */
/*                                              object                */
/*      bo_op               D23_op_pr_info *    operation progress of */
/*                                              base object           */
/*                                                                    */
/* OUTPUT-PARAMETERS:   None                                          */
/*                                                                    */
/* RETURNVALUE:         None                                          */
/*                                                                    */
/*inoff ***************************************************************/

static boolean d26_a10_cmp_targ_for_loop(D2_name_string target,
    D23_av_value *target_ep, D23_op_pr_info *target_op,
    D2_name_string base_object, D23_av_value *bo_ep, D23_op_pr_info *bo_op)

{                               /*  d26_a10_cmp_targ_for_loop         */

  signed16 targ_cmp;        /* indicates whether DSA-names differ     */
  signed16 len_target;      /* length of target object                */
  signed16 len_bo;          /* length of base object                  */
  signed16 minlen;          /* minimum length of target objects       */
  signed16 ep_len;          /* length of encoded part                 */

len_target = strlen((char *)target);
len_bo     = strlen((char *)base_object);
minlen     = len_target < len_bo ? len_target : len_bo;

if                          /* decoded parts of targets differ        */
  (strncmp((char *)target,(char *)base_object,minlen))
{                           /* return no loop                         */
  return(FALSE);
}
else if                     /* target object is shorter than base obj.*/
       (len_target < len_bo)
{                           /* check whether base object is extension */
			    /* of target object                       */
  if                        /* base object is not extension of target */
    (len_target != 0 && base_object[len_target] != D2_RDN_SEP ||
     target_ep != (D23_av_value *)NULL)
  {                         /* return no loop                         */
    return(FALSE);
  }
  else                      /* base object is an extension of the     */
			    /* target object                          */
  {                         /* set the extension flag                 */
    targ_cmp = -1;
  }
}
else if                     /* target object is same as base object   */
       (len_target == len_bo)
{                           /* check whether base object is extension */
			    /* of target object                       */
  if                        /* target object is fully decoded         */
    (target_ep == (D23_av_value *)NULL)
  {                         /* check encoded part of base object      */
    if                      /* base object contains encoded parts     */
      (bo_ep != (D23_av_value *)NULL)
    {                       /* set the extension flag                 */
      targ_cmp = -1;
    }
    else                    /* base object is equal to target object  */
    {                       /* set the extension flag                 */
      targ_cmp = 0;
    }
  }
  else if                   /* base object is fully decoded           */
	 (bo_ep == (D23_av_value *)NULL)
  {                         /* set the extension flag                 */
    targ_cmp = 1;
  }
  else                      /* set length of undecoded part           */
  {
    ep_len = target_ep->d23_avlen;
    if                      /* lengths of encoded parts differ        */
      (ep_len != bo_ep->d23_avlen)
    {                       /* return no loop                         */
      return(FALSE);
    }
    else if                 /* encoded parts differ                   */
	   (memcmp((byte *)(target_ep + 1),(byte *)(bo_ep + 1),ep_len))
    {
      return(FALSE);
    }
    else                    /* base object is equal to target object  */
    {                       /* set the extension flag                 */
      targ_cmp = 0;
    }
  }
}
else                        /* target object is longer than base obj. */
{                           /* check whether target object is         */
			    /* extension of target object             */
  if                        /* target object is not extension of base */
			    /* object                                 */
    (len_bo != 0 && target[len_bo] != D2_RDN_SEP ||
     bo_ep != (D23_av_value *)NULL)
  {                         /* return no loop                         */
    return(FALSE);
  }
  else                      /* target object is an extension of the   */
			    /* base object                            */
  {                         /* set the extension flag                 */
    targ_cmp = 1;
  }
}

switch(targ_cmp)            /* check extension flag                   */
{

  case -1:
  case 0:
			    /* base object is extension of target obj.*/
			    /* this may not happen in name resolution */
			    /* as target object changes only due to an*/
			    /* alias dereferencing, but an aliased    */
			    /* object contains no alias names         */
			    /* in evaluation phase of a whole subtree */
			    /* search this is regarded as a loop for  */
			    /* SNI DSAs, as this request would        */
			    /* duplicate the results                  */
			    /* entry_only TRUE is caused by a         */
			    /* "one level search", that dereferenced  */
			    /* an alias                               */
    return(bo_op->d23_opnm_rs == D2_COMPLETED ? FALSE : TRUE);
  case 1:                   /* target object is extension of base obj.*/
			    /* return no loop                         */
    return(FALSE);
}

}                       /*  d26_a10_cmp_targ_for_loop                 */
