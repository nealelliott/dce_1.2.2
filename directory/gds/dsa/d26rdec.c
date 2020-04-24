/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d26rdec.c,v $
 * Revision 1.1.8.2  1996/02/18  19:45:45  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:27:05  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:46:44  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:07:31  root]
 * 
 * Revision 1.1.6.5  1994/07/06  15:06:58  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:13:34  marrek]
 * 
 * Revision 1.1.6.4  1994/06/21  14:45:43  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  15:49:00  marrek]
 * 
 * Revision 1.1.6.3  1994/05/10  15:52:32  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:23:23  marrek]
 * 
 * Revision 1.1.6.2  1994/03/23  15:09:34  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:22:06  keutel]
 * 
 * Revision 1.1.6.1  1994/02/22  16:06:21  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  17:18:12  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:16:31  marrek]
 * 
 * Revision 1.1.4.2  1993/08/10  14:28:32  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  13:14:49  marrek]
 * 
 * Revision 1.1.2.7  1993/02/02  14:51:11  marrek
 * 	*** empty log message ***
 * 
 * Revision 1.1.2.6  1993/02/02  11:24:38  marrek
 * 	Bug fixes for OT 6587
 * 	[1993/02/01  15:00:24  marrek]
 * 
 * Revision 1.1.2.5  1992/12/31  19:41:31  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:20:29  bbelch]
 * 
 * Revision 1.1.2.4  1992/12/17  10:01:02  marrek
 * 	Allow logical unbind after DSP reject
 * 	Only standard operation IDs are sent, when tranfer syntax
 * 	ASN.1 has been negotiated.
 * 	[1992/12/16  13:04:38  marrek]
 * 
 * Revision 1.1.2.3  1992/12/02  14:44:36  marrek
 * 	November 1992 code drop
 * 	insert cast of void (*)() in twalk to eliminate
 * 	the strange function ordering.
 * 	[1992/12/02  14:42:54  marrek]
 * 
 * Revision 1.1.2.2  1992/11/27  19:49:31  marrek
 * 	November 1992 code drop
 * 	[1992/11/26  16:23:35  marrek]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d26rdec.c,v $ $Revision: 1.1.8.2 $ $Date: 1996/02/18 19:45:45 $";
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
/* NAME         : d26rdec.c       [d26rdec]                           */
/*                                                                    */
/* AUTHOR       : Sanjay Jain, Siemens(India)                         */
/* DATE         : 22.07.92                                            */
/*                                                                    */
/* COMPONENT    : request decomposition in search/list operations     */
/*                                                                    */
/* DOC.-NR.     : <relevante Dokumente>                               */
/*                      .                                             */
/*                      .                                             */
/*                                                                    */
/* PRD#/VERS.   : <Produktnr. aus Produktstruktur> - <Versionsnr.>    */
/*                                                                    */
/* DESCRIPTION  :   This module contains the functions, that chain    */
/*              search subrequests to other DSAs and merge results of */
/*              these subrequests.                                    */
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

#include <stdlib.h>
#include <sys/types.h>
#include <search.h>

/*****  external Includes    *****/

#include <dce/d27util.h>
#include <dce/d2dir.h>
#define DSA
#include <d2asn1.h>

/*****  internal Includes    *****/

#include <d26schema.h>

/**********************************************************************/
/*                                                                    */
/*                        D E F I N E S                               */
/*                                                                    */
/**********************************************************************/

		    /* codes to define the state of association.      */
#define D26_IPC_BIND_ERROR      1       /* IPC-bind to S-Stub failed. */
#define D26_INV_ERROR           2       /* bind/subrequest not sent.  */
#define D26_BIND_PENDING        3       /* bind invoke successful.    */
#define D26_A_BIND_PENDING      4       /* bind invoke successful for */
					/* abandon request            */
#define D26_BIND_OVER           5       /*an existing bind table entry*/
					/* has been used.             */
#define D26_BIND_ERROR          6       /* error in bind.             */
#define D26_BIND_ABORT          7       /* bind aborted.              */
#define D26_IDLE                8       /* DSP bind is open to reuse  */
#define D26_REQUEST_PENDING     9       /* subrequest invoke successful.*/
#define D26_REQUEST_ERROR      10       /* error in subrequest.       */
#define D26_REQUEST_ABORT      11       /* subrequest aborted.        */
#define D26_UNBIND_PENDING     12       /* unbind invoke successful.  */
#define D26_UNBOUND            13       /* result of unbind request   */
					/* received.                  */

/* constant used in realloc for access point list.      */

#define D26_SURP_ACCP          10       /* no. of surplus entries for */
					/* which space is allocated in*/
					/* access point list during   */
					/* realloc.                   */


/* constants to decide whether master or shadow entries should be     */
/* collected from subresults.                                         */

#define D26_MASTER_LOOP         1   /* indicates that master entries  */
				    /* should be extracted from sub-  */
				    /* result.                        */
#define D26_SHADOW_LOOP         0   /* indicates that shadow entries  */
				    /* should be extracted from sub-  */
				    /* result.                        */

				    /* constants indicating purpose   */
				    /* of a bind table entry          */
#define D26_SUBREQUEST          0   /* indicates subreqest            */
#define D26_ABANDON             1   /* indicates abandon              */

/**********************************************************************/
/*                                                                    */
/*                        M A C R O S                                 */
/*                                                                    */
/**********************************************************************/

#define D26_SKIP_DS_NAME(message) \
	if (((D23_ds_name *)(message))->d23_dscomplete == TRUE)\
		(message) += sizeof(D23_ds_name) + \
		((D23_ds_name *)(message))->d23_dsoff; \
	else \
		{ \
		(message) += sizeof(D23_ds_name) + \
				((D23_ds_name *)(message))->d23_dsoff; \
		(message) += sizeof(D23_av_value) + \
				((D23_av_value *)(message))->d23_avoff; \
		}

/**********************************************************************/
/*                                                                    */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*                                                                    */
/**********************************************************************/

typedef struct
    {
      D26_referral    *d26_referral;  /* referral to be used to chain */
				      /* the subrequest.              */
      signed16        d26_assid;      /* Association-ID, to match     */
				      /* request and result.          */
      byte            *d26_refms;     /* IPC-buffer Reference.        */
      signed16        d26_lenms;      /* IPC-buffer length.           */
      signed16        d26_state;      /* state info. of association.  */
      byte           *d26_result;     /* result of subrequest (not in */
				      /* ASN.1)                       */
      signed32        d26_result_len; /* total length of result block.*/
      signed16        d26_result_type;/* type of result (ENT_RESULT or*/
				      /* ERR_RESULT or REF_RESULT)    */
      time_t          d26_last_usage; /* time of last usage of this   */
				      /* DSP bind                     */
      unsigned16      d26_contid;     /* Context id                   */
      signed32        d26_invid;      /* Invoke Id                    */
      D2_str          d26_psap;       /* PSAP address of access point */
      signed16        d26_route_nr;   /* number of routing DSA used   */
      boolean	      d26_auth_m[D2_MAX_AUTH_MECHS];
      unsigned32      d26_sec_ctx_id; /* security context ID          */
      signed16	      d26_used_am;    /* auth. m. used for bind	      */
    } D26_bind_tab_ent;

/**********************************************************************/
/*                                                                    */
/*                D E C L A R A T I O N S        DATA                 */
/*                                                                    */
/**********************************************************************/

				      /* Bind table definition        */
static D26_bind_tab_ent d26_bind_table[D26_REF_BLOCKSIZE];

static boolean  d26_size_ex;          /* size limit exceeded          */
static void     *d26_res_root;        /* root of binary tree of       */
				      /* entries in final result      */

static D2_str   d26_own_psap;         /* own PSAP address             */

/**********************************************************************/
/*                                                                    */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*                                                                    */
/**********************************************************************/

/******** LOCAL  ********/

static signed32 d26_d08_strip_chaining_results(byte *to, byte *from,
    signed32 result_len);
static void d26_d22_init_bind_table(D26_referral *referral,
    signed16 route_nr, signed16 purpose);
static signed16 d26_d23_handle_results(byte *dap_message, signed32 input_len,
    signed16 op_id, D23_rsrqpb *rsrqpb);
static void d26_d24_handle_subrequest(byte *dap_message, signed32 input_len,
    signed16 op_id, D26_bind_tab_ent *bind_tab_ptr, D23_inrqpb *inrqpb);
static void d26_d25_anal_subrq_result(D23_wtrspb *wtrspb, byte **result,
    signed32 *result_len, signed16 err_id, D26_bind_tab_ent *bind_tab_ptr,
    signed16 op_id);
static signed16 d26_d26_handle_reference(D23_rf_info **rf_info);
static byte     *d26_d27_skip_till_entry(byte *result);
static byte     *d26_d28_skip_entry_subord(byte *result);
static signed16 d26_d29_gen_final_res(byte **result, signed32 *result_len,
    signed32 *remlen, signed16 *result_type, signed16 op_id);
static int      d26_d30_ds_name_cmp(const void *entry1, const void *entry2);
static signed16 d26_d31_copy_ent_to_res(byte **result_scan,
    signed32 *last_bind_tab_index, boolean loop);
static void     d26_d32_call_tdelete(const void *node, VISIT order,
    int level);
static signed16 d26_d33_merge_results(byte **result, signed32 *result_len,
    signed32 *remlen);
static signed16 d26_d35_send_dsp_unbinds(void);
static void     d26_d36_find_routing_acp(void);
static void     d26_d37_free_routing_acp(void);
static D2_str   *d26_d38_next_routing_psap(D26_referral *referral,
    signed16 *route_nr, signed16 purpose, boolean *auth_mechs, 
    char **princ_name, D2_name_string *dsa_name);
static D26_bind_tab_ent *d26_d39_reusable_bte(D2_str *psap);
static signed16 d26_d40_dupl_req_entry(D26_referral *referral,D2_str *psap);
static signed16 d26_d41_dncmp(D2_name_string dn_dp1, D23_av_value *dn_ep1,
    D2_name_string dn_dp2, D23_av_value *dn_ep2);
static void     d26_d42_reset_bt_index(signed16 bt_index1,
    signed16 bt_index2, signed16 route_nr);
static signed16 d26_d44_build_abandon_request(byte **request,
    signed32 *request_len, signed32 inv_id);

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_d08_strip_chaining_results                        */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        16.05.91                                              */
/*                                                                    */
/* DESCRIPTION: This function converts a DSP-message into IPC-format  */
/*              if necessary and strips the chaining results.         */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      signed32    > 0             length of stripped message        */
/*                                                                    */
/*inoff ***************************************************************/

static signed32 d26_d08_strip_chaining_results(
    byte *to,           /* OUT   -  stripped message                  */
    byte *from,         /* IN    -  message to be stripped            */
    signed32 result_len)/* IN    -  length of message to be stripped  */

{                       /* d26_d08_strip_chaining_results             */

  byte *message;        /* actual message pointer                     */

  message = from + sizeof(D23_chain_res);

  while                 /* still cross references present             */
       (((D23_cross_ref *)message)->d23_creftag == D23_CREFTAG)
  {                     /* skip the context prefix                    */
    D26_SKIP_DS_NAME(message);
			/* skip the access point structure            */
    message += sizeof(D23_acc_info);
			/* skip the DSA-name                          */
    D26_SKIP_DS_NAME(message);
			/* skip the PSAP-Address                      */
    message += sizeof(D23_av_value) + ((D23_av_value *)message)->d23_avoff;
  }

  result_len -= message - from;
  memcpy(to,message,result_len);

  return(result_len);

}                       /* d26_d08_strip_chaining_results             */

/**********************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_d21_req_decomp                                    */
/*                                                                    */
/* AUTHOR:      Sanjay Jain, Siemens (India)                          */
/* DATE:        22.07.92                                              */
/*                                                                    */
/* DESCRIPTION: This function is the entry point for request decomp.  */
/*      algorithm in search or list operation.                        */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      D2_NO_ERR       no error.                                     */
/*      D26_ERROR       hard error.                                   */
/*                                                                    */
/**********************************************************************/

signed16 d26_d21_req_decomp(
    byte *dap_message,  /* IN    -  DAP message for request           */
    signed32 input_len, /* IN    -  length of DAP message             */
    byte **result,      /* INOUT -  result message to return          */
			/* INOUT -  length of allocated memory for    */
    signed32 *result_len,           /* result                         */
    signed32 *remlen,   /* INOUT -  remaining length of result        */
    signed16 op_id,     /* IN    -  operation ID of request           */
			/* INOUT -  type of the result                */
    signed16 *result_type,
    D23_rsrqpb *rsrqpb) /* IN    -  parameters for d23_result         */

{                          /*  d26_d21_req_decomp                     */

			   /* function identifier for logging         */
  static String function = "d26_d21_req_decomp";

  signed32 ref_index;               /* index to scan referral table   */
  signed32 bind_tab_index;          /* index to scan bind table       */

  boolean  abandoned = FALSE;       /* local abandoned flag           */

  signed16 ret_value = D2_NO_ERR;   /* return value code              */

  DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s"),GDS_S_GENERAL,svc_c_debug1,
    GDS_S_ENTRY_FUNCTION,function));

  if                    /* routing must eventually be performed       */
    (d26_routing_prio > 0)
  {                     /* find the routing access points             */
    d26_d36_find_routing_acp();
  }

  if                    /* local result was generated                 */
    (*result_type == D26_RD_RESULT)
  {                     /* log local result message                   */
    DCE_SVC_LOG((GDS_S_APDU_RESULT_MSG,op_id,*result_len - *remlen,*result));
  }
  else
  {                     /* log local error message                    */
    DCE_SVC_LOG((GDS_S_APDU_ERROR_MSG,*result_len - *remlen,*result));
  }

  for                   /* for all existing referrals                 */
    (ref_index = 0; ref_index < d26_ref_count; ref_index++)
  {                     /* set the bind table index invalid           */
    d26_referral[ref_index].d26_bt_index = D26_IDX_NOT_DEF;
  }

			/* initialize global variables                */
  d26_bind_count = 0;
  d26_size_ex = FALSE;
  d26_res_root = NULL;
  d26_entry_count = 0;
  d26_start_p_o_q = NULL;

	      /* initialize first bind table entry with local result. */
  d26_bind_table[0].d26_referral = (D26_referral *)NULL;
  d26_bind_table[0].d26_result_len = *result_len - *remlen;
  if                    /* memory for local result cannot be allocated*/
    ((d26_bind_table[0].d26_result = D27_MALLOC(
      svc_c_sev_warning,d26_bind_table[0].d26_result_len)) == NULL)
  {                     /* set the return value                       */
    ret_value = D26_ERROR;
  }
  else                  /* copy local result into bind table entry    */
  {
    memcpy(d26_bind_table[0].d26_result,*result,
      d26_bind_table[0].d26_result_len);
    d26_bind_table[0].d26_result_type = *result_type;
    d26_bind_count++;

    *remlen = *result_len;

    for                 /* all locally generated referrals            */
       (ref_index = 0; ref_index < d26_ref_count; ref_index++)
    {                   /* check the referral                         */
      if                /* referral must be handled                   */
	(d26_referral[ref_index].d26_ignore == FALSE)
      {                 /* check for loops                            */
	DCE_SVC_LOG((GDS_S_TARGET_DSA_MSG,d26_referral[ref_index].d26_target,
	  d26_accp_list[d26_referral[ref_index].d26_acp[0]].d26_dsaname));
	if              /* reference leads to loop                    */
	  (d26_a08_loop_detected(
	   d26_accp_list[d26_referral[ref_index].d26_acp[0]].d26_dsaname,
	   d26_accp_list[d26_referral[ref_index].d26_acp[0]].d26_dsa_ep,
	   d26_referral[ref_index].d26_target,
	   d26_referral[ref_index].d26_targ_ep,
	   (D23_op_pr_info *)&(d26_referral[ref_index].d26_op_prog)) == TRUE)
	{               /* ignore the reference                       */
	  DCE_SVC_LOG((GDS_S_LOOP_AVOIDED_MSG));
	  d26_referral[ref_index].d26_ignore = TRUE;
	}
      }
    }

    for                 /* all locally generated referrals            */
      (ref_index = 0; ref_index < d26_ref_count; ref_index++)
    {                   /* check the referral                         */
      d26_m11_distcmd(SIGUSR1);
      d26_referral[ref_index].d26_ignore =
	d26_abandoned == TRUE || ret_value != D2_NO_ERR ?
	TRUE : d26_referral[ref_index].d26_ignore;
      if                /* request is abandoned but abandoned error   */
			/* not sent                                   */
	(d26_abandoned == TRUE && abandoned == FALSE)
      {                 /* send the abandoned error                   */
	abandoned = TRUE;
	if              /* abandoned error cannot be sent             */
	  (d26_u84_send_abandoned(op_id,result,remlen,result_len,rsrqpb)
	   != D2_NO_ERR)
	{               /* set return value                           */
	  ret_value = D26_ERROR;
	}
      }
      if                /* referral must be handled                   */
	(d26_referral[ref_index].d26_ignore == FALSE)
      {                 /* create bind table entry and bind           */
	d26_d22_init_bind_table(&d26_referral[ref_index],0,D26_SUBREQUEST);
      }
    }

    D27_022_ENTER_TIME_TRACE(GDS_S_DSP_BINDS_SENT,function,0,0);

    if                  /* results could be handled                   */
      (d26_d23_handle_results(dap_message,input_len,op_id,rsrqpb)
       == D2_NO_ERR)
    {
      D27_022_ENTER_TIME_TRACE(GDS_S_DSP_RESULTS_READY,function,0,0);
      if                /* request was not abandoned                  */
	(d26_abandoned == FALSE)
      {                 /* merge results                              */
	if              /* results cannot be merged                   */
	  (d26_d29_gen_final_res(result,result_len,remlen,result_type,op_id)
	   != D2_NO_ERR)
	{
	  ret_value = D26_ERROR;
	}
        D27_022_ENTER_TIME_TRACE(GDS_S_DSP_RESULTS_MERGED,function,0,0);
      }
    }
    else
    {                   /* set return value                           */
      ret_value = D26_ERROR;
    }
  }

  for                   /* all bind table entries                     */
     (bind_tab_index = 0; bind_tab_index < d26_bind_count; bind_tab_index++)
  {                     /* check allocation of result                 */
    if                  /* memory for result was allocated            */
      (d26_bind_table[bind_tab_index].d26_result !=(byte *)NULL)
    {                   /* free it                                    */
      free(d26_bind_table[bind_tab_index].d26_result);
    }
  }

  if                    /* routing must eventually be performed       */
    (d26_routing_prio > 0)
  {                     /* free the routing psap addresses            */
    d26_d37_free_routing_acp();
  }


			/* free nodes of binary tree of entries       */
  twalk(d26_res_root,d26_d32_call_tdelete);

  DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s%d"),GDS_S_GENERAL,svc_c_debug1,
    GDS_S_EXIT_FUNCTION,function,ret_value));
  return(ret_value);

}                       /* end of d26_d21_req_decomp                  */

/**********************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_d22_init_bind_table                               */
/*                                                                    */
/* AUTHOR:      Sanjay Jain, Siemens (India)                          */
/* DATE:        22.07.92                                              */
/*                                                                    */
/* DESCRIPTION: This function creates an entry in the bind table for  */
/*              the referral. It sets result pointer to NULL. It      */
/*              makes an IPC-bind to S-Stub and sets the association  */
/*              id. in the entry. It sends a bind message to referral */
/*              DSA and sets the state of association in the entry.   */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      none                                                          */
/*                                                                    */
/**********************************************************************/

static void d26_d22_init_bind_table(
			/* IN     - reference to process              */
    D26_referral *referral,
			/* IN     - number of route to use            */
    signed16     route_nr,
			/* IN     - purpose of the bind table entry   */
    signed16     purpose)

{                    /*  d26_d22_init_bind_table                     */

			     /* parameter-block for IPC-bind         */
  static D23_bindpb bindpb = {{D23_V02}};
			     /* parameter-block for invoke           */
  static D23_inrqpb inrqpb = {{D23_V02}};
			     /* parameter-block for unbind           */
  static D23_ubndpb ubndpb = {{D23_V02}};
			     /* function identifier for logging      */
  static char *function = "d26_d22_init_bind_table";

  signed16 bind_tab_index;   /* index to scan bind table             */
  signed32 in_bt_index;      /* bind table index of incoming referral*/
  D2_str   *psap;            /* pointer to PSAP to use for bind      */
			     /* array of auth. mechs		     */
  boolean  auth_mechs[D2_MAX_AUTH_MECHS];
  char	   *princ_name;      /* principal name			     */
  boolean    try_again;
  D2_name_string t_dsa_name; /* target DSA name			     */
  D26_bind_tab_ent *ru_bte;  /* reusable bind table entry            */
  signed16 i;

  DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s"),GDS_S_GENERAL,svc_c_debug3,
    GDS_S_ENTRY_FUNCTION,function));

  in_bt_index = referral->d26_bt_index;

  while                 /* PSAP to bind to available                  */
       ((psap = d26_d38_next_routing_psap(referral,&route_nr,purpose,
		auth_mechs, &princ_name, &t_dsa_name))
	!= NULL)
  {                     /* check whether request would be duplicated  */
    if                  /* bind table entry is used for subrequest    */
      (purpose == D26_SUBREQUEST)
    {                   /* check whether request is duplicated        */
      if                /* request would be duplicated                */
	((bind_tab_index = d26_d40_dupl_req_entry(referral,psap)) > 0)
      {                 /* ignore the referral                        */
	DCE_SVC_LOG((GDS_S_REF_DUPLICATED_MSG));
	referral->d26_bt_index = bind_tab_index;
	break;
      }
      else
      {                 /* set new bind table indices                 */
	referral->d26_bt_index = bind_tab_index = d26_bind_count;
      }
    }
    else
    {                   /* set a new bind table index                 */
      bind_tab_index = d26_bind_count;
    }
			/* create a new bind table entry              */
    d26_bind_count++;

    if                  /* reusable bind table entry found            */
      ((ru_bte = d26_d39_reusable_bte(psap)) != NULL)
    {                   /* copy existing bind table entry             */
			/* use same ipc association                   */
      d26_bind_table[bind_tab_index].d26_assid = ru_bte->d26_assid;
      d26_bind_table[bind_tab_index].d26_refms = ru_bte->d26_refms;
      d26_bind_table[bind_tab_index].d26_lenms = ru_bte->d26_lenms;
      d26_bind_table[bind_tab_index].d26_contid = ru_bte->d26_contid;
    }

    d26_bind_table[bind_tab_index].d26_referral = referral;

    d26_bind_table[bind_tab_index].d26_psap.d2_size = psap->d2_size;
    if                  /* no memory for PSAP address                 */
      ((d26_bind_table[bind_tab_index].d26_psap.d2_value =
       (byte *)D27_MALLOC(svc_c_sev_warning,
       d26_bind_table[bind_tab_index].d26_psap.d2_size)) == NULL)
    {                   /* log error and return                       */
      d26_bind_count--;
      return;
    }
    memcpy(d26_bind_table[bind_tab_index].d26_psap.d2_value,psap->d2_value,
	   d26_bind_table[bind_tab_index].d26_psap.d2_size);

    for (i = 0; i < D2_MAX_AUTH_MECHS; i++)
	d26_bind_table[bind_tab_index].d26_auth_m[i] = auth_mechs[i];

    d26_bind_table[bind_tab_index].d26_route_nr = route_nr;

    d26_bind_table[bind_tab_index].d26_result = (byte *)NULL;
    d26_bind_table[bind_tab_index].d26_result_len = 0;

    if                  /* association is reused                      */
      (ru_bte != NULL)
    {                   /* check for limits                           */
      if                /* limits exceeded                            */
	(d26_time_ex || d26_size_ex || d26_dbop_flag == D26_DB_NOOP)
      {                 /* don't send more subrequests                */
	d26_bind_table[bind_tab_index].d26_state = D26_IDLE;
	d26_bind_table[bind_tab_index].d26_last_usage =
						    time((time_t)NULL);
      }
      else
      {                 /* reset status of bind table entry           */
	d26_bind_table[bind_tab_index].d26_state = D26_BIND_OVER;
      }
      break;
    }
    else
    {
      if                /* limits exceeded                            */
	(d26_time_ex || d26_size_ex || d26_dbop_flag == D26_DB_NOOP)
      {                 /* don't do more binds                        */
	d26_bind_table[bind_tab_index].d26_state = D26_UNBOUND;
	free(d26_bind_table[bind_tab_index].d26_psap.d2_value);
	break;
      }
      else
      {                 /* bind to referral DSA                       */
	bindpb.d23_4svrid = D23_SSTSID + d26_dir_id;


	if              /* IPC-bind as a client to S-Stub fails       */
	  (d23_bind(&bindpb) == D23_ERROR)
	{               /* set state and break                        */
	  d26_bind_table[bind_tab_index].d26_state =
					D26_IPC_BIND_ERROR;
	  break;
	}
	else
	{               /* IPC-bind successful                        */
	  d26_bind_table[bind_tab_index].d26_assid = bindpb.d23_4assid;
	  d26_bind_table[bind_tab_index].d26_refms = bindpb.d23_4refms;
	  d26_bind_table[bind_tab_index].d26_lenms = bindpb.d23_4lenms;

	  if            /* bind to DSA failed                         */
	    (d26_d02_bind_request(&bindpb,&inrqpb,
	     &d26_bind_table[bind_tab_index].d26_psap,
	     t_dsa_name, auth_mechs, princ_name, &try_again,
	     &d26_bind_table[bind_tab_index].d26_sec_ctx_id,
	     &d26_bind_table[bind_tab_index].d26_used_am) != D2_NO_ERR)
	  {             /* set bind table state                       */
	    d26_bind_table[bind_tab_index].d26_state = D26_INV_ERROR;

			/* make ipc unbind                            */
	    ubndpb.d23_5assid  = bindpb.d23_4assid;
	    d23_unbind(&ubndpb);
	  }
	  else
	  {             /* bind invoke successful                     */
	    if          /* bind is for subrequest                     */
	      (purpose == D26_SUBREQUEST)
	    {           /* set the state                              */
	      d26_bind_table[bind_tab_index].d26_state = D26_BIND_PENDING;
	    }
	    else
	    {           /* set state and invoke ID                    */
	      d26_bind_table[bind_tab_index].d26_state = D26_A_BIND_PENDING;
	      d26_bind_table[bind_tab_index].d26_invid =
		d26_bind_table[in_bt_index].d26_invid;
	    }
	    break;
	  }
	}
      }
    }
  }

  if                    /* assigned bind table index was defined      */
    (in_bt_index != D26_IDX_NOT_DEF)
  {                     /* reset the assigned bind table index        */
    d26_d42_reset_bt_index(in_bt_index,bind_tab_index,route_nr - 1);
  }

  DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s%d"),GDS_S_GENERAL,svc_c_debug3,
    GDS_S_EXIT_FUNCTION,function,0));

  return;

}                       /* d26_d22_init_bind_table                    */

/**********************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_d23_handle_results                                */
/*                                                                    */
/* AUTHOR:      Sanjay Jain, Siemens (India)                          */
/* DATE:        22.07.92                                              */
/*                                                                    */
/* DESCRIPTION: This function collects results as long as some results*/
/*      are pending. The actions performed depend on the              */
/*      state of the association to which the result corresponds.     */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      D2_NO_ERR   signed16        no error occurred                 */
/*      D26_ERROR                   hard error occurred               */
/*                                                                    */
/**********************************************************************/

static signed16 d26_d23_handle_results(
    byte *dap_message,  /* IN    -  original DAP message              */
    signed32 input_len, /* IN    -  length of DAP message             */
    signed16 op_id,     /* IN    -  operation ID                      */
    D23_rsrqpb *rsrqpb) /* IN    -  parameters for d23_result         */

{                       /* d26_d23_handle_results                     */

			/* parameter-block for wait result            */
  static D23_wtrspb wtrspb = {{D23_V02},D23_INFIN};
			/* parameter-block for invoke                 */
  static D23_inrqpb inrqpb = {{D23_V02}};
			/* parameter-block for unbind                 */
  static D23_ubndpb ubndpb = {{D23_V02}};
			/* function identifier for logging            */
  static char *function = "d26_d23_handle_results";

  byte *result;         /* starting address to store result           */
  signed32 result_len;  /* total length of result buffer              */
  signed32 remlen;      /* length of empty part of result buffer      */

			/* indicates whether result memory should be  */
  boolean  malloc_flag = TRUE;      /* allocated                      */
			/* pointer to matching bind table entry.      */
  D26_bind_tab_ent *bind_tab_ptr;
  signed16 err_id;      /* error code returned by collect_result      */
			/* index to scan bind table                   */
  signed32 bind_tab_index;
			/* return value                               */
  signed16 ret_value = D2_NO_ERR;
			/* return value                               */
  signed16 collect_ret_value = D2_NO_ERR;
			/* number of routing access point             */
  signed16 route_nr;

  DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s"),GDS_S_GENERAL,svc_c_debug3,
    GDS_S_ENTRY_FUNCTION,function));

  for                   /* ever                                       */
     (;;)
  {                     /* look if some result is pending             */
    for                 /* all entries of the bind table              */
       (bind_tab_index = 1; bind_tab_index<d26_bind_count; bind_tab_index++)
    {                   /* check the state                            */
      if (d26_bind_table[bind_tab_index].d26_state == D26_BIND_PENDING    ||
	  d26_bind_table[bind_tab_index].d26_state == D26_REQUEST_PENDING ||
	  d26_bind_table[bind_tab_index].d26_state == D26_UNBIND_PENDING  ||
	  d26_bind_table[bind_tab_index].d26_state == D26_BIND_OVER)
      {                 /* still something to do                      */
	break;
      }
    }

    if                  /* nothing more to do                         */
      (bind_tab_index == d26_bind_count)
    {                   /* break loop                                 */
      break;
    }

    if                  /* something is pending for the entry         */
      (d26_bind_table[bind_tab_index].d26_state != D26_BIND_OVER)
    {                   /* check memory flag                          */
      if                /* memory for subresult must be allocated     */
	(malloc_flag)
      {                 /* do it                                      */
	if              /* error from allocating result memory        */
	  ((result = (byte *)D27_MALLOC(svc_c_sev_warning,D26_BLOCK_LEN))
	   == NULL)
	{               /* set return value and break                 */
	  result_len = 0;
	  ret_value = D26_ERROR;
	}
	else
	{               /* set memory length and flag                 */
	  result_len  = D26_BLOCK_LEN;
	  malloc_flag = FALSE;
	}
      }

      remlen = result_len;
      collect_ret_value = d26_d05_collect_result(op_id,rsrqpb,&wtrspb,
	&result,&result_len,&remlen,&err_id);
      for               /* all bind table entries                     */
	 (bind_tab_index = 1; bind_tab_index < d26_bind_count;
	  bind_tab_index++)
      {                 /* check state and association ID             */
	if              /* assigned bind table entry is found         */
	  (d26_bind_table[bind_tab_index].d26_assid == wtrspb.d23_7assid &&
	   (d26_bind_table[bind_tab_index].d26_state == D26_BIND_PENDING ||
	    d26_bind_table[bind_tab_index].d26_state == D26_A_BIND_PENDING ||
	    d26_bind_table[bind_tab_index].d26_state == D26_REQUEST_PENDING))
	{               /* break loop                                 */
	  break;
	}
      }

      if                /* no assigned bind table entry found         */
	(bind_tab_index == d26_bind_count)
      {                 /* set return value and break loop            */
	ret_value = D26_ERROR;
	continue;
      }
    }

    bind_tab_ptr = &d26_bind_table[bind_tab_index];
			/* prepare ipc invoke block                   */
    inrqpb.d23_6assid  = bind_tab_ptr->d26_assid;
    inrqpb.d23_6invinfo.d23_contid = D23_SAC(D27_DSP_AC) |
      D23_SACM(D27_LOC_AC) | D23_SAS(D27_DSP_AS);
    inrqpb.d23_6invinfo.d23_usrid  = inrqpb.d23_6assid;
    inrqpb.d23_6rfidt  = bind_tab_ptr->d26_refms;
    D23_CTS(inrqpb.d23_6invinfo.d23_contid,
      D23_GTS(wtrspb.d23_7invinfo.d23_contid));
			/* prepare ipc unbind block                   */
    ubndpb.d23_5assid = bind_tab_ptr->d26_assid;

    switch(bind_tab_ptr->d26_state)
    {
      case D26_BIND_OVER:           /* association is reused          */
	collect_ret_value = D2_NO_ERR;
      case D26_BIND_PENDING:        /* result of bind request         */
	d26_d11_conf_bind_result(collect_ret_value,
	  wtrspb.d23_7invinfo.d23_contid,&result,&result_len,
	  bind_tab_ptr->d26_used_am,bind_tab_ptr->d26_sec_ctx_id);
	if              /* no error in collecting bind result         */
	  (collect_ret_value == D2_NO_ERR)
	{               /* check abandoned flag                       */
	  if            /* request is abandoned                       */
	    (d26_abandoned == TRUE)
	  {             /* set state idle and time stamp              */
	    bind_tab_ptr->d26_state      = D26_IDLE;
	    bind_tab_ptr->d26_last_usage = time((time_t)NULL);
	  }
	  else
	  {             /* build and send subrequest                  */
	    d26_d24_handle_subrequest(dap_message,input_len,op_id,
	      bind_tab_ptr,&inrqpb);
	    if          /* subrequest could not be sent               */
	      (bind_tab_ptr->d26_state == D26_INV_ERROR)
	    {           /* set state idle and time stamp              */
	      bind_tab_ptr->d26_state      = D26_IDLE;
	      bind_tab_ptr->d26_last_usage = time((time_t)NULL);
	    }
	  }
	}
	else
	{
	  if            /* routing access point used                  */
	    ((route_nr = bind_tab_ptr->d26_route_nr) > 1 &&
	     route_nr <= d26_nr_routing_dsas + 1)
	  {             /* reset the routing access point             */
	    free(d26_routing_access_points[route_nr - 2].d26_psap.d2_value);
	    d26_routing_access_points[route_nr - 2].d26_psap.d2_value = NULL;
	  }
	  bind_tab_ptr->d26_state =
	    collect_ret_value == D26_MEM_ERR ||
	    collect_ret_value == D26_LOG_ERR ||
	    collect_ret_value == D26_REJECTED ?
	    D26_BIND_ERROR : D26_BIND_ABORT;
			/* make ipc unbind from sstub                 */
	  d23_unbind(&ubndpb);
	  if            /* request is not abandoned                   */
	    (d26_abandoned == FALSE)
	  {             /* retry bind                                 */
	    d26_d22_init_bind_table(bind_tab_ptr->d26_referral,route_nr,
	      D26_SUBREQUEST);
	  }
	}
	break;
      case D26_A_BIND_PENDING:      /* result of bind for abandon     */
	if              /* no error in collecting bind result         */
	  (collect_ret_value == D2_NO_ERR)
	{               /* check abandoned flag                       */
			/* build and send abandon request             */
	  d26_d24_handle_subrequest(dap_message,input_len,D23_ABANDON,
	    bind_tab_ptr,&inrqpb);
	  if            /* abandon could not be sent                  */
	    (bind_tab_ptr->d26_state == D26_INV_ERROR)
	  {             /* set state idle and time stamp              */
	    bind_tab_ptr->d26_state      = D26_IDLE;
	    bind_tab_ptr->d26_last_usage = time((time_t)NULL);
	  }
	}
	else
	{
	  bind_tab_ptr->d26_state =
	     collect_ret_value == D26_MEM_ERR ||
	     collect_ret_value == D26_LOG_ERR ||
	     collect_ret_value == D26_REJECTED ?
	     D26_BIND_ERROR : D26_BIND_ABORT;
			/* make ipc unbind from sstub                 */
	  d23_unbind(&ubndpb);
	}
	break;
      case D26_REQUEST_PENDING:
	if              /* message received                           */
	  (collect_ret_value == D2_NO_ERR ||
	   collect_ret_value == D26_LOG_ERR)
	{               /* analyse result or referral                 */
	  malloc_flag = TRUE;
	  if            /* request was not abandoned                  */
	    (d26_abandoned == FALSE)
	  {             /* analyse the subrequest result              */
	    result_len -= remlen;
	    d26_d25_anal_subrq_result(&wtrspb,&result,&result_len,err_id,
	      bind_tab_ptr,op_id);
	  }
	}
	else
	{               /* set state aborted                          */
	  bind_tab_ptr->d26_state = D26_REQUEST_ABORT;
	}

	if (collect_ret_value != D26_ABORTED)
	{                       /* mark this bind as IDLE to be reused. */
	  bind_tab_ptr->d26_state      = D26_IDLE;
	  bind_tab_ptr->d26_last_usage = time((time_t)NULL);
	}
	else
	{                             /* make ipc unbind from sstub     */
	  d23_unbind(&ubndpb);
	}
	break;
    }
  }

  if                    /* memory was allocated, but not used         */
    (malloc_flag == FALSE)
  {                     /* free the memory                            */
    free(result);
  }

  DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s%d"),GDS_S_GENERAL,svc_c_debug3,
    GDS_S_EXIT_FUNCTION,function,ret_value));

  return(ret_value);

}                       /* d26_d23_handle_results                     */

/**********************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_d24_handle_subrequest                             */
/*                                                                    */
/* AUTHOR:      Sanjay Jain, Siemens (India)                          */
/* DATE:        22.07.92                                              */
/*                                                                    */
/* DESCRIPTION: This function builds and sends the subrequest. Time   */
/*      limit is checked in the routine which builds the request.     */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*                                                                    */
/**********************************************************************/

static void d26_d24_handle_subrequest(
    byte *dap_message,  /* IN    -  DAP message                       */
    signed32 input_len, /* IN    -  length of DAP message             */
    signed16 op_id,     /* IN    -  operation ID                      */
			/* INOUT -  bind table entry                  */
    D26_bind_tab_ent *bind_tab_ptr,
    D23_inrqpb *inrqpb) /* INOUT -  parameter for d23_invoke          */

{                   /*  d26_d24_handle_subrequest                     */

				    /* function identifier for logging*/
  static char *function = "d26_d24_handle_subrequest";

  byte *request = (byte *)NULL;     /* dsp request message            */
  signed32 request_len;             /* length of request              */
  signed16 rop_id;                  /* reduced operation ID           */
				    /* memory for error apdu          */
  byte err_apdu[sizeof(D23_rserror) + sizeof(D23_eom)];
  asn1_field asn1_request;          /* structure for encoded request  */
				    /* to keep track if any error has */
  boolean err_till_now = FALSE;     /* occurred.                      */

  DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s"),GDS_S_GENERAL,svc_c_debug3,
    GDS_S_ENTRY_FUNCTION,function));


  if                                /* subrequest should be sent      */
    (op_id != D23_ABANDON)
  {                                 /* build subrequest APDU          */
    if                              /* error in building subrequest.  */
      (d26_d06_build_chained_request(&request,&request_len,dap_message,
       input_len,bind_tab_ptr->d26_referral - d26_referral,
       bind_tab_ptr->d26_used_am, bind_tab_ptr->d26_auth_m) == D26_ERROR)
    {                               /* set error indicator            */
      err_till_now = TRUE;
    }
  }
  else
  {                                 /* build abandon APDU             */
    if                              /* error in building abandon req. */
      (d26_d44_build_abandon_request(&request,&request_len,
       bind_tab_ptr->d26_invid) == D26_ERROR)
    {                               /* set error indicator            */
      err_till_now = TRUE;
    }
  }
  if                                /* no error occurred              */
    (err_till_now == FALSE)
  {                                 /* set the operation ID           */
    rop_id = op_id > D2_ADM_OPID ? op_id - D2_ADM_OPID : op_id;
    inrqpb->d23_6oprid =
      D23_GTS(inrqpb->d23_6invinfo.d23_contid) == D27_ASN1_TS ?
      rop_id : op_id;

    if                              /* bind result was in ASN1.       */
      (D23_GTS(inrqpb->d23_6invinfo.d23_contid) == D27_ASN1_TS ||
       D23_GTS(inrqpb->d23_6invinfo.d23_contid) == D27_ASN1_ADM_TS)
    {                               /* encode the request             */
      DCE_SVC_LOG((GDS_S_APDU_REQUEST_MSG,op_id,request_len,request));

      if                            /* encode request in ASN1         */
	(d2a000_apdu_asn1(rop_id,D27_DSP_AS,request,request_len,
	 &asn1_request,err_apdu) == D2_ERROR)
      {                             /* set error indicator            */
	err_till_now = TRUE;
        D27_022_ENTER_TIME_TRACE(GDS_S_ENCODING_ERR,function,0,0);
      }
      else
      {                             /* reset request and length       */
	free(request);
	request     = asn1_request.v;
	request_len = asn1_request.l;
        D27_022_ENTER_TIME_TRACE(GDS_S_ENCODING_OK,function,0,0);
      }
    }

    if                              /* no error in encoding request.  */
      (err_till_now == FALSE)
    {
      if                            /* error from sending request.    */
	(d26_d04_send_request(inrqpb,bind_tab_ptr->d26_lenms,request,
	 request_len) == D26_ERROR)
      {                             /* set error indicator            */
	err_till_now = TRUE;
      }
    }
  }

  if (err_till_now == TRUE)
  {
    bind_tab_ptr->d26_state = D26_INV_ERROR;
  }
  else
  {
    bind_tab_ptr->d26_state = D26_REQUEST_PENDING;
    bind_tab_ptr->d26_invid = inrqpb->d23_6invinfo.d23_invid;
  }

  if (request != (byte *)NULL)
  {
    free(request);
  }

  DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s%d"),GDS_S_GENERAL,svc_c_debug3,
    GDS_S_EXIT_FUNCTION,function,0));

  return;

}                   /* d26_d24_handle_subrequest                      */

/**********************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_d25_anal_subrq_result                             */
/*                                                                    */
/* AUTHOR:      Sanjay Jain, Siemens (India)                          */
/* DATE:        22.07.92                                              */
/*                                                                    */
/* DESCRIPTION: This function analyses the result of subrequest.      */
/*      Result could be result with continuation references or error  */
/*      referral or some other kind of error result. This function    */
/*      tries to use references in the result for more binds.         */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      none                                                          */
/*                                                                    */
/**********************************************************************/

static void d26_d25_anal_subrq_result(
    D23_wtrspb *wtrspb, /* IN    -  parameter for d23_waitrs          */
    byte **result,      /* INOUT -  result of subrequest              */
			/* INOUT -  length of result of subrequest    */
    signed32 *result_len,
    signed16 err_id,    /* IN    -  error ID for error results        */
			/* OUT   -  bind table entry                  */
    D26_bind_tab_ent *bind_tab_ptr,
    signed16 op_id)     /* IN    -  operation ID                      */

{                   /*  d26_d25_anal_subrq_result                     */

				    /* function identifier for logging*/
  static char *function = "d26_d25_anal_subrq_result";

  signed16 syntax;                  /* transfer-syntax of the result  */
  asn1_field asn1_result;           /* encoded result                 */

				    /* memory for error apdu          */
  byte err_apdu[sizeof(D23_rserror) + sizeof(D23_eom)];

		      /* pointer to referral info in apdu format.     */
		      /* used for scanning referrals in the result.   */
  D23_rf_info *rf_info;

		      /* variable to save value of d26_ref_count      */
		      /* before calling handle_reference              */
  signed32 old_ref_count;

  byte *message;                    /* actual message pointer         */
			/* return value of asn1 decoding functions.   */
  signed32 asn1_dec_ret;

  DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s"),GDS_S_GENERAL,svc_c_debug3,
    GDS_S_ENTRY_FUNCTION,function));

  syntax = D23_GTS(wtrspb->d23_7invinfo.d23_contid);

  if                    /* result or error must be decoded            */
    (syntax == D27_ASN1_TS || syntax == D27_ASN1_ADM_TS)
  {                     /* decode the result first                    */
    asn1_result.l = *result_len;
    asn1_result.v = (byte *)*result;

    if                  /* result was returned                        */
      (wtrspb->d23_7rltid == D23_RESULT)
    {                   /* decode result                              */
      asn1_dec_ret = d2a001_asn1_apdu(op_id + D2_RES_OPID,D27_DSP_AS,result,
	result_len,&asn1_result,err_apdu);
    }
    else
    {                   /* decode error                               */
      asn1_dec_ret = d2a047_asn_error_apdu(result,result_len,(signed32)err_id,
	D27_DSP_AS,&asn1_result,err_apdu);
    }
    if                  /* result or error could not be decoded       */
      (asn1_dec_ret == D2_ERROR)
    {                   /* free the coded version                     */
      D27_022_ENTER_TIME_TRACE(GDS_S_DECODING_ERR,function,0,0);

			  /* result pointer in bind table remains NULL. */
      free(asn1_result.v);

      DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s%d"),GDS_S_GENERAL,svc_c_debug3,
	GDS_S_EXIT_FUNCTION,function,0));
      return;
    }

    D27_022_ENTER_TIME_TRACE(GDS_S_DECODING_OK,function,0,0);
    free(asn1_result.v);
  }


  if                                /* result is returned.            */
    (wtrspb->d23_7rltid == D23_RESULT)
  {                                 /* skip to partial outcome qual.  */
    DCE_SVC_LOG((GDS_S_APDU_RESULT_MSG,op_id,*result_len,*result));

    message = d26_d27_skip_till_entry(*result);


    while                           /* entries/subord still present.  */
	 (((D23_en_info *)message)->d23_entag == D23_ENTTAG ||
	  ((D23_subord_info *)message)->d23_sotag == D23_SOITAG)
    {                               /* skip entry/subord information  */
      message = d26_d28_skip_entry_subord(message);
    }

    if                  /* partial outcome qualifier present          */
      (((D23_part_oq *)message)->d23_oqtag == D23_OQTAG)
    {                   /* analyse partial outcome qualifier          */
      if (((D23_part_oq *)message)->d23_oq_limit == D2_SIZE_LIMIT_EX)
      {
	d26_size_ex = TRUE;
      }
      else if (((D23_part_oq *)message)->d23_oq_limit == D2_TIME_LIMIT_EX)
      {
	d26_time_ex = TRUE;
      }
      else
      {                 /* take care of D2_ADM_LIMIT_EX               */
      }
			       /* message points to first referral.   */
      message += sizeof(D23_part_oq);
			       /* process referrals                   */
      rf_info = (D23_rf_info *)message;

      while             /* referrals still present                    */
	   (rf_info->d23_rftag == D23_REFTAG)
      {                 /* handle the reference                       */
		      /* save value of d26_ref_count before calling   */
		      /* handle_ref. If d26_ref_count is not changed  */
		      /* after the call, it means this referral leads */
		      /* to a loop or repetition.                     */
	old_ref_count = d26_ref_count;
	if              /* new referral is not created in ref. table  */
	  (d26_d26_handle_reference(&rf_info) == D26_ERROR)
	{               /* no new bind table entry is created for this*/
			/* refr. Result pointer in bind table is NULL.*/
	  free(*result);
	  DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s%d"),GDS_S_GENERAL,
	    svc_c_debug3,GDS_S_EXIT_FUNCTION,function,0));
	  return;
	}

	if              /* no loop detected                           */
	  (old_ref_count != d26_ref_count)
	{               /* initialize a new entry in bind table.      */
	  d26_d22_init_bind_table(&d26_referral[old_ref_count],0,
	    D26_SUBREQUEST);
	}
      }
      D27_022_ENTER_TIME_TRACE(GDS_S_DSP_REFER_PROCESSED,function,0,0);
    }
	      /* current referral result processed successfully.      */
	      /* bind table point to decoded result                   */
    bind_tab_ptr->d26_result      = *result;
    bind_tab_ptr->d26_result_len  = *result_len;
    bind_tab_ptr->d26_result_type = D26_RD_RESULT;
  }
  else
  {
    DCE_SVC_LOG((GDS_S_APDU_ERROR_MSG,*result_len,*result));
    if                              /* referral is returned.          */
      (wtrspb->d23_7rltid >= D23_RERROR && (syntax == D27_ASN1_TS &&
       err_id == D23S_DSA_REFERRAL || syntax == D27_ASN1_ADM_TS &&
       err_id == D23S_DSA_REFERRAL || syntax == D27_PRIV_TS &&
       ((D23_rserror *)*result)->d23_Zerrval == D2_REF_ERROR))
    {
      bind_tab_ptr->d26_result_type = D26_RD_REFERRAL;
      rf_info = (D23_rf_info *)(*result + sizeof(D23_rserror) +
	sizeof(D23_drf_info));

	   /* save value of d26_ref_count before calling handle_ref.  */
	   /* if d26_ref_count is not changed after the call, it means*/
	   /* this referral leads to a loop.                          */
      old_ref_count = d26_ref_count;
      if                /* new referral is not created in ref. table  */
	(d26_d26_handle_reference(&rf_info) == D26_ERROR)
      {                 /* no new bind table entry is created         */
			/* result pointer in bind table is NULL       */
	free(*result);
	DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s%d"),GDS_S_GENERAL,
	  svc_c_debug3,GDS_S_EXIT_FUNCTION,function,0));
	return;
      }

      if                /* no loop detected                           */
	(old_ref_count != d26_ref_count)
      {                 /* initialize a new entry in bind table       */
	d26_d22_init_bind_table(&d26_referral[old_ref_count],0,
	  D26_SUBREQUEST);
      }
			/* bind table point to decoded result         */
      bind_tab_ptr->d26_result = *result;
      bind_tab_ptr->d26_result_len = *result_len;
      D27_022_ENTER_TIME_TRACE(GDS_S_DSP_REFER_PROCESSED,NULL,0,0);
    }
    else
    {                   /* any other kind of error detected           */
      bind_tab_ptr->d26_result_type = D26_RD_ERROR;
			      /* bind table point to decoded result.  */
      bind_tab_ptr->d26_result = *result;
      bind_tab_ptr->d26_result_len = *result_len;

    }
  }

  DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s%d"),GDS_S_GENERAL,svc_c_debug3,
    GDS_S_EXIT_FUNCTION,function,0));
  return;

}                                     /* d26_d25_anal_subrq_result.   */

/**********************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_d26_handle_reference                              */
/*                                                                    */
/* AUTHOR:      Sanjay Jain, Siemens (India)                          */
/* DATE:        22.07.92                                              */
/*                                                                    */
/* DESCRIPTION: This function handles a reference info. which is there*/
/*      in apdu format. It checks if this reference generates a loop. */
/*      If not it puts rf_info into referral table.                   */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      D26_ERROR       signed16    hard error occurred               */
/*      D2_NO_ERR                   no error.                         */
/*                                                                    */
/**********************************************************************/

static signed16 d26_d26_handle_reference(
			/* INOUT -  pointer to reference in APDU      */
    D23_rf_info **rf_info)

{                       /* d26_d26_handle_reference                   */

			/* function identifier for logging            */
  static char *func_id = "d26_d26_handle_reference";

  D23_ds_name     *ds_name_ptr;
  D23_acc_info    *acc_info_ptr;
  D23_av_value    *av_value_ptr;

			/* variable used to store d26_accp_list in    */
			/* case of realloc.                           */
  D26_acc_point   *old_accp_list;

			/* to index access point array in a referral. */
  signed16        acp_count;

			/* result returned by tsearch                 */
  void            **tsearch_result;

			/* pointer to scan input message.             */
  byte            *message;

			/* variable to handle encoded part of DSA name*/
  D23_av_value    *dsa_ep;

			/* variable to handle decoded part of DSA name*/
  D2_name_string  dsa_dp;

				/* memory for normed DSA name         */
  char            dsa_normed[D2_DNL_MAX + 1];

  register signed16 acp_idx;    /* loop variable                      */
  register D26_acc_point *acp;  /* loop pointer to access point list  */

  DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s"),GDS_S_GENERAL,svc_c_debug3,
    GDS_S_ENTRY_FUNCTION,func_id));

			/* fill referral structure in referral table. */
  ds_name_ptr = (D23_ds_name *)((byte *)(*rf_info) + sizeof(D23_rf_info));

  if                    /* no memory for normed target object name    */
    ((d26_referral[d26_ref_count].d26_target = (D2_name_string)D27_MALLOC(
     svc_c_sev_warning,ds_name_ptr->d23_dslen)) == NULL)
  {                     /* log and return error                       */
    DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s%d"),GDS_S_GENERAL,svc_c_debug3,
      GDS_S_EXIT_FUNCTION,func_id,D26_ERROR));
    return(D26_ERROR);
  }
  else if               /* no memory for exact target object name     */
	 ((d26_referral[d26_ref_count].d26_etarget =
	  (D2_name_string)D27_MALLOC(svc_c_sev_warning,
	  ds_name_ptr->d23_dslen)) == NULL)
  {                     /* log and return error                       */
    free(d26_referral[d26_ref_count].d26_target);
    DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s%d"),GDS_S_GENERAL,svc_c_debug3,
      GDS_S_EXIT_FUNCTION,func_id,D26_ERROR));
    return(D26_ERROR);
  }

  if                    /* target object is root                      */
    (ds_name_ptr->d23_dslen == 0)
  {                     /* copy an empty string                       */
    strcpy((char *)d26_referral[d26_ref_count].d26_etarget,"");
    strcpy((char *)d26_referral[d26_ref_count].d26_target,"");
  }
  else
  {                     /* copy target object from the message        */
    strcpy((char *)d26_referral[d26_ref_count].d26_etarget,
	   ((char *)ds_name_ptr + sizeof(D23_ds_name)));
    d27_007_norm_name(((char *)ds_name_ptr + sizeof(D23_ds_name)),
      ds_name_ptr->d23_dslen,(char *)d26_referral[d26_ref_count].d26_target);
    d27_012_remove_blanks((char *)d26_referral[d26_ref_count].d26_target,
      ds_name_ptr->d23_dslen,(char *)d26_referral[d26_ref_count].d26_target);
    d27_018_sort_name(d26_referral[d26_ref_count].d26_target);
  }

  d26_referral[d26_ref_count].d26_new_rdn = (D2_name_string)NULL;
  if                    /* target object name is completely decoded   */
    (ds_name_ptr->d23_dscomplete == TRUE)
  {                     /* set encoded part to NULL                   */
    d26_referral[d26_ref_count].d26_targ_ep = (D23_av_value *)NULL;
  }
  else
  {                     /* assign encoded part of target object name. */
    d26_referral[d26_ref_count].d26_targ_ep = (D23_av_value *)
      ((byte *)ds_name_ptr + sizeof(D23_ds_name) + ds_name_ptr->d23_dsoff);
  }

  d26_referral[d26_ref_count].d26_op_prog.d2_nr_phase =
    (*rf_info)->d23_op_pr.d23_opnm_rs;
  d26_referral[d26_ref_count].d26_op_prog.d2_next_rdn =
    (*rf_info)->d23_op_pr.d23_op_nxtrdn;

  d26_referral[d26_ref_count].d26_al_rdns   = (*rf_info)->d23_rf_alirdn;
  d26_referral[d26_ref_count].d26_rdn_res   = (*rf_info)->d23_rf_rdnres;
  d26_referral[d26_ref_count].d26_acp_count = 0;

			/* set pointer to access info list in apdu.   */
  message = (byte *)ds_name_ptr;
  D26_SKIP_DS_NAME(message);
  acc_info_ptr = (D23_acc_info *)message;

			/* fill access point array of referral        */
  acp_count = 0;
			/* extract all access points from referral.   */
  while                 /* access points following                    */
       (acc_info_ptr->d23_acctag == D23_ACCTAG)
  {                     /* set pointer to DSA name structure          */
    ds_name_ptr = (D23_ds_name *)((byte *)acc_info_ptr +
      sizeof(D23_acc_info));

			/* handle decoded part of DSA name.           */
    if                  /* decoded part of DSA name present           */
      (ds_name_ptr->d23_dslen != 0)
    {                   /* normalize it                               */
      dsa_dp = (D2_name_string)((byte *)ds_name_ptr + sizeof(D23_ds_name));
      d27_007_norm_name((char *)dsa_dp,ds_name_ptr->d23_dslen,dsa_normed);
      d27_012_remove_blanks(dsa_normed,ds_name_ptr->d23_dslen,dsa_normed);
      d27_018_sort_name((D2_name_string)dsa_normed);
    }
    else
    {                   /* set it empty                               */
      dsa_dp = (D2_name_string)"";
      strcpy(dsa_normed,(char *)dsa_dp);
    }
			/* handle encoded part of DSA name.           */
    if                  /* DSA name completely decoded                */
      (ds_name_ptr->d23_dscomplete == TRUE)
    {                   /* set encoded part to NULL                   */
      dsa_ep = (D23_av_value *)NULL;
    }
    else
    {                   /* assign encoded part of DSA name            */
      dsa_ep = (D23_av_value *)((byte *)ds_name_ptr + sizeof(D23_ds_name) +
	ds_name_ptr->d23_dsoff);
    }
			/* set pointer to psap address value structure*/
    message = (byte *)ds_name_ptr;
    D26_SKIP_DS_NAME(message);
    av_value_ptr = (D23_av_value *)message;

			/* set pointer to next access point info.     */
    acc_info_ptr = (D23_acc_info *)(
      (byte *)av_value_ptr + sizeof(D23_av_value) + av_value_ptr->d23_avoff);

    DCE_SVC_LOG((GDS_S_TARGET_DSA_MSG,d26_referral[d26_ref_count].d26_target,
      (char *)ds_name_ptr + sizeof(D23_ds_name)));
			/* check if this access point causes loop.    */
    if                  /* loop detected for access point             */
      (d26_a08_loop_detected(dsa_dp,dsa_ep,
       d26_referral[d26_ref_count].d26_target,
       d26_referral[d26_ref_count].d26_targ_ep,
       &((*rf_info)->d23_op_pr)) == TRUE)
    {                   /* log that and continue                      */
      DCE_SVC_LOG((GDS_S_LOOP_AVOIDED_MSG));
      continue;
    }

    for                 /* all access points in access point list     */
       (acp_idx = 0, acp = d26_accp_list; acp_idx < d26_accp_count;
	acp_idx++, acp++)
    {                   /* compare with current access point          */
      if                /* DSA names match                            */
	(!d26_d41_dncmp(acp->d26_dsaname,acp->d26_dsa_ep,
	  (D2_name_string)dsa_normed,dsa_ep))
      {                 /* break loop                                 */
	break;
      }
    }

    if                  /* DSA name found in access point list        */
      (acp_idx < d26_accp_count)
    {                   /* reuse this access point for the referral   */
      d26_referral[d26_ref_count].d26_acp[acp_count++] = acp_idx;
    }
    else if             /* access point is own access point           */
	   (!d26_d41_dncmp((D2_name_string)d26_myname,NULL,
	    (D2_name_string)dsa_normed,dsa_ep))
    {                   /* indicate own access point in referral      */
      d26_own_psap.d2_size  = av_value_ptr->d23_avlen;
      d26_own_psap.d2_value = (Octet_string)(av_value_ptr + 1);
      d26_referral[d26_ref_count].d26_acp[acp_count++] = D26_OWNNAME;
    }
    else
    {                   /* create new access point entry.             */
      if                /* access point list is full                  */
	(d26_accp_count == d26_accp_max)
      {                 /* reallocate access point list               */
	old_accp_list = d26_accp_list;
	d26_accp_list = (D26_acc_point *)D27_REALLOC(
	  svc_c_sev_warning,(byte *)old_accp_list,
	  ((d26_accp_max + D26_SURP_ACCP)* sizeof(D26_acc_point)));
	if              /* realloc fails                              */
	  (d26_accp_list == (D26_acc_point *) NULL)
	{               /* exit function                              */
	  d26_accp_list = old_accp_list;
	  DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s%d"),GDS_S_GENERAL,
	    svc_c_debug3,GDS_S_EXIT_FUNCTION,func_id,D26_ERROR));
	  return(D26_ERROR);
	}
	else
	{               /* initialize the name alloc flag             */
	  for           /* all new access point structures            */
	     (acp_idx = 0, acp = d26_accp_list + d26_accp_max;
	      acp_idx < D26_SURP_ACCP; acp_idx++, acp++)
	  {             /* initialized name alloc flag                */
	    acp->d26_nam_alloc = FALSE;
	  }
	}
	d26_accp_max += D26_SURP_ACCP;
      }

			/* handle DSA name in access point list str.  */
      d26_accp_list[d26_accp_count].d26_dsa_ename =
	(D2_name_string)((byte *)ds_name_ptr + sizeof(D23_ds_name));
      if                /* no memory for normed dsaname available     */
	((d26_accp_list[d26_accp_count].d26_dsaname =
	 (D2_name_string)D27_MALLOC(svc_c_sev_warning,
	 ds_name_ptr->d23_dslen)) == NULL)
      {                 /* log and return error                       */
	free(d26_referral[d26_ref_count].d26_target);
	free(d26_referral[d26_ref_count].d26_etarget);
	DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s%d"),GDS_S_GENERAL,
	  svc_c_debug3,GDS_S_EXIT_FUNCTION,func_id,D26_ERROR));
	return(D26_ERROR);
      }
      else
      {                 /* set "name allocated" flag                  */
	d26_accp_list[d26_accp_count].d26_nam_alloc = TRUE;
	strcpy((char *)d26_accp_list[d26_accp_count].d26_dsaname,dsa_normed);
      }

      if                /* DSA name is completely decoded             */
	(ds_name_ptr->d23_dscomplete == TRUE)
      {                 /* set encoded part to NULL                   */
	d26_accp_list[d26_accp_count].d26_dsa_ep = (D23_av_value *)NULL;
      }
      else
      {                 /* assign encoded part of DSA name            */
	d26_accp_list[d26_accp_count].d26_dsa_ep = (D23_av_value *)
	  ((byte *)ds_name_ptr + sizeof(D23_ds_name) +
	  ds_name_ptr->d23_dsoff);
      }

      if                /* no memory for psap address available       */
	((d26_accp_list[d26_accp_count].d26_psap.d2_value =
	  (byte *)D27_MALLOC(svc_c_sev_warning,av_value_ptr->d23_avlen))
	  == NULL)
      {                 /* exit function                              */
	free(d26_referral[d26_ref_count].d26_target);
	free(d26_referral[d26_ref_count].d26_etarget);
	DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s%d"),GDS_S_GENERAL,
	  svc_c_debug3,GDS_S_EXIT_FUNCTION,func_id,D26_ERROR));
	return(D26_ERROR);
      }

      d26_accp_list[d26_accp_count].d26_psap.d2_size =
	av_value_ptr->d23_avlen;

			/* copy PSAP-Address into memory              */
      memcpy(d26_accp_list[d26_accp_count].d26_psap.d2_value,
	((byte *)av_value_ptr + sizeof(D23_av_value)),
	av_value_ptr->d23_avlen);

      d26_accp_list[d26_accp_count].d26_dnl_idx = D26_IDX_NOT_DEF;

      d26_referral[d26_ref_count].d26_acp[acp_count++] = d26_accp_count++;
    }
  }

			/* a referral should not be put in referral   */
			/* table if it leads to loop or it is already */
			/* present in referral table.                 */

  if                    /* new referral present and doesn't cause loop*/
    (acp_count != 0)
  {                     /* assign components of referral              */
    d26_referral[d26_ref_count].d26_acp_count   = acp_count;

    d26_referral[d26_ref_count].d26_master_dsa  = D26_IDX_NOT_DEF;
    d26_referral[d26_ref_count].d26_ref_index   = d26_ref_count;
    d26_referral[d26_ref_count].d26_predecessor = D26_IDX_NOT_DEF;
    d26_referral[d26_ref_count].d26_own_sh_dsas = FALSE;
    d26_referral[d26_ref_count].d26_ignore      = FALSE;

			/* put referral in binary tree if it is not   */
			/* already present.                           */
    tsearch_result = (void **)D27_TSEARCH(svc_c_sev_warning,
      (void *)(d26_referral + d26_ref_count),&d26_ref_root,d26_u38_refcmp);

    if                  /* no memory available                        */
      (tsearch_result == (void **)NULL)
    {                   /* return error                               */
      free(d26_referral[d26_ref_count].d26_target);
      free(d26_referral[d26_ref_count].d26_etarget);
      DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s%d"),GDS_S_GENERAL,svc_c_debug3,
	GDS_S_EXIT_FUNCTION,func_id,D26_ERROR));
      return(D26_ERROR);
    }


    if                  /* referral could be inserted in binary tree  */
      (*tsearch_result == (void *)(d26_referral + d26_ref_count))
    {
      DCE_SVC_LOG((GDS_S_REF_GENERATED_MSG));
      d26_ref_count++;
    }
    else
    {                   /* free target object memory                  */
      DCE_SVC_LOG((GDS_S_REF_DUPLICATED_MSG));
      free(d26_referral[d26_ref_count].d26_target);
      free(d26_referral[d26_ref_count].d26_etarget);
    }
  }
  else
  {                     /* free target object memory                  */
    free(d26_referral[d26_ref_count].d26_target);
    free(d26_referral[d26_ref_count].d26_etarget);
  }


				      /* reset the output parameter.  */
  *rf_info = (D23_rf_info *)acc_info_ptr;

  DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s%d"),GDS_S_GENERAL,svc_c_debug3,
    GDS_S_EXIT_FUNCTION,func_id,D2_NO_ERR));
  return(D2_NO_ERR);

}                                     /* d26_d26_handle_reference()   */

/**********************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_d27_skip_till_entry                               */
/*                                                                    */
/* AUTHOR:      Sanjay Jain, Siemens (India)                          */
/* DATE:        22.07.92                                              */
/*                                                                    */
/* DESCRIPTION: This function scans a chained/local result and returns*/
/*      ptr. to the beginning of entry information.                   */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      result          ptr. to the beginning of decoded result.      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      ptr. in result string at the beginning of entry information.  */
/*                                                                    */
/**********************************************************************/

static byte *d26_d27_skip_till_entry(byte *result)

{                     /*  d26_d27_skip_till_entry                     */

/* pointer to scan result.      */
byte   *message;

message = result;

/* skip chaining result if present.     */

if (((D23_chain_res *)message)->d23_chrtag == D23_CHRTAG)
	{
	message += sizeof(D23_chain_res);

	/* skip domain info if present. */

	/* skipp cross references.      */
	/* cross references still present.      */
	while (((D23_cross_ref *)message)->d23_creftag == D23_CREFTAG)
		{
		/* skip cross reference structure.      */

		message += sizeof(D23_cross_ref);

		/* skip context prefix.                 */
		D26_SKIP_DS_NAME(message);

		/* skip access point structure.         */
		message += sizeof(D23_acc_info);

		/* skip DSA-name.                       */
		D26_SKIP_DS_NAME(message);

		/* skip PSAP-Address.                   */
		message += sizeof(D23_av_value) +
					((D23_av_value *)message)->d23_avoff;
		}       /* end of skipping cross references.    */
	}       /* end of chaining result present.      */

/* skip base object name if present.    */
if (((D23_ds_name *)message)->d23_dstag == D23_BONTAG)
	D26_SKIP_DS_NAME(message);

return(message);

}       /* end of d26_d27_skip_till_entry().    */

/**********************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_d28_skip_entry_subord                             */
/*                                                                    */
/* AUTHOR:      Sanjay Jain, Siemens (India)                          */
/* DATE:        22.07.92                                              */
/*                                                                    */
/* DESCRIPTION: This function scans a search/list result and skips one*/
/*      entry/subord info.. This function expects that entry/subord   */
/*      info. is really present to be skipped.                        */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      result          ptr. to the decoded result.                   */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      ptr. in result string after skipping one entry/subord.        */
/*                                                                    */
/**********************************************************************/

static byte *d26_d28_skip_entry_subord(byte *result)

{                 /*  d26_d28_skip_entry_subord()                     */

/* pointer to scan result.      */
byte   *message;

message = result;

/* skip entry/subord information.       */

/* skip entry/subord info. structure.   */
if (((D23_en_info *)message)->d23_entag == D23_ENTTAG)
	message += sizeof(D23_en_info);
else
	message += sizeof(D23_subord_info);

/* skip distinguish name.       */
D26_SKIP_DS_NAME(message);

/* skip attribute information.  */
/* attributes may not be present in case of LIST operation. */
/* attributes still present.    */
while (((D23_at_type *)message)->d23_atttag == D23_ATTTAG)
	{
	/* skip type information.       */
	message += sizeof(D23_at_type) +
			((D23_at_type *)message)->d23_atoff;

	/* D23_av_info may not be present e.g. att_type_only. */
	/* case.                                              */
	/* attribute value present.     */
	if (((D23_av_info *)message)->d23_avtag == D23_AVTAG)
		{
		message += sizeof(D23_av_info);

		/* skip attribute values.       */
		/* attribute values still present.      */
		while ((unsigned16)((D23_av_value *)message)->d23_avlen
		       < D23_MINTAG)
			message += sizeof(D23_av_value) +
			((D23_av_value *)message)->d23_avoff;

		}       /* attribute value present.     */

	}       /* end of loop to skip attributes.      */

return(message);

}                             /* end of d26_d28_skip_entry_subord().  */

/**********************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_d29_gen_final_res                                 */
/*                                                                    */
/* AUTHOR:      Sanjay Jain, Siemens (India)                          */
/* DATE:        22.07.92                                              */
/*                                                                    */
/* DESCRIPTION: This function handles results of subrequests to       */
/*      generate final result for the original request.               */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      op_id           operation id                                  */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      *result         pointer to merged result.                     */
/*      *result_len     length of merged result block.                */
/*      *remlen         unused length in the result block.            */
/*      *result_type    final result type                             */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      D2_NO_ERR       no. error.                                    */
/*      D26_ERROR       error in memory allocation for the result or  */
/*                      d26_d31_copy_ent_to_res returns error.        */
/*                                                                    */
/* STANDARD-HEADER: None                                              */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*      malloc()                                                      */
/*      d26_d08_strip_chaining_results()                              */
/*      d26_d31_copy_ent_to_res()                                     */
/*      d26_u63_write_error_referral()                                */
/*                                                                    */
/* NOTES:                                                             */
/*                                                                    */
/**********************************************************************/

static signed16 d26_d29_gen_final_res(
    byte **result,
    signed32 *result_len,
    signed32 *remlen,
    signed16 *result_type,
    signed16 op_id)

{                             /* d26_d29_gen_final_res().             */

/* function identifier for logging      */
static  String  func_id = "d26_d29_gen_final_res";

/* index to scan bind table.    */
signed32        bind_tab_index;

/* temporary message length     */
signed32        meslen;

/* reduced operation id.        */
signed16 rop_id;

DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s"),GDS_S_GENERAL,svc_c_debug3,
  GDS_S_ENTRY_FUNCTION,func_id));

/* check if result with entry info. should be generated.        */
for (bind_tab_index = 0; bind_tab_index < d26_bind_count; bind_tab_index++)
	if (d26_bind_table[bind_tab_index].d26_result != (byte *)NULL &&
	    d26_bind_table[bind_tab_index].d26_result_type == D26_RD_RESULT)
		break;

if (bind_tab_index < d26_bind_count)
	{
	*result_type = D26_RD_RESULT;

	rop_id = op_id > D2_ADM_OPID ? op_id - D2_ADM_OPID : op_id;

	if (rop_id == D23_SEARCH || rop_id == D23_LIST)
		{
		/* merge subresults containing entries/subords .        */
		return(d26_d33_merge_results(result, result_len, remlen));
		}

	}
else
	{
	/* check if error result should be generated.   */
	for (bind_tab_index = 0; bind_tab_index < d26_bind_count;
							bind_tab_index++)
		if (d26_bind_table[bind_tab_index].d26_result !=
							(byte *)NULL &&
		    d26_bind_table[bind_tab_index].d26_result_type ==
								D26_RD_ERROR)
			break;

	if (bind_tab_index < d26_bind_count)
		*result_type = D26_RD_ERROR;
	}

/* result or error should be copied from subresult to result.   */
if (bind_tab_index < d26_bind_count)
	{
	if ((*result = (byte *)D27_MALLOC(svc_c_sev_warning,
	    d26_bind_table[bind_tab_index].d26_result_len)) == (byte *)NULL)
		{
		  DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s%d"),GDS_S_GENERAL,
		    svc_c_debug3,GDS_S_EXIT_FUNCTION,func_id,D26_ERROR));
		  return(D26_ERROR);
		}

	if                  /* result must be copied, DAP came in     */
	  (*result_type == D26_RD_RESULT &&
	   d26_chain_arg == (D23_chain_arg *)NULL)
	{                   /* strip chaining results                 */
	  *result_len = d26_d08_strip_chaining_results(*result,
			    d26_bind_table[bind_tab_index].d26_result,
			    d26_bind_table[bind_tab_index].d26_result_len);
	}
	else                /* just copy the result                   */
	{
	  memcpy(*result, d26_bind_table[bind_tab_index].d26_result,
		 d26_bind_table[bind_tab_index].d26_result_len);

	  *result_len = d26_bind_table[bind_tab_index].d26_result_len;
	}
	*remlen = 0;

	DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s%d"),GDS_S_GENERAL,
	  svc_c_debug3,GDS_S_EXIT_FUNCTION,func_id,D2_NO_ERR));
	return(D2_NO_ERR);
	}

/* referral result is generated if time limit is not exceeded otherwise */
/* error result is generated.                                           */

for (bind_tab_index = 0; bind_tab_index < d26_bind_count; bind_tab_index++)
	if (d26_bind_table[bind_tab_index].d26_result == (byte *)NULL)
		break;

/* should never happen. There should be atleast one bind table entry whose */
/* result is not available.                                                */
if (bind_tab_index == d26_bind_count)
	{
	return(D26_ERROR);
	}

if ((*result = (byte *)D27_MALLOC(svc_c_sev_warning,D26_BLOCK_LEN))
    == NULL)
	{
	  DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s%d"),GDS_S_GENERAL,
	    svc_c_debug3,GDS_S_EXIT_FUNCTION,func_id,D26_ERROR));
	  return(D26_ERROR);
	}

*result_len = *remlen = D26_BLOCK_LEN;

if (d26_time_ex)
	{
	*result_type = D26_RD_ERROR;
	d27_010_set_error_apdu((D23_rserror *)*result,D2_CALL_ERR,D2_PAR_ERR,
			 D2_TIME_LIMIT_EXCEEDED,&meslen);
	*remlen -= meslen;
	DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s%d"),GDS_S_GENERAL,
	  svc_c_debug3,GDS_S_EXIT_FUNCTION,func_id,D2_NO_ERR));
	return(D2_NO_ERR);
	}

*result_type = D26_RD_REFERRAL;
DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s%d"),GDS_S_GENERAL,svc_c_debug3,
  GDS_S_EXIT_FUNCTION,func_id,D2_NO_ERR));
return(d26_u63_write_error_referral(result, result_len, remlen,
			   d26_bind_table[bind_tab_index].d26_referral));

}       /* end of d26_d29_gen_final_res().      */

/**********************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_d30_ds_name_cmp                                   */
/*                                                                    */
/* AUTHOR:      Sanjay Jain, Siemens (India)                          */
/* DATE:        22.07.92                                              */
/*                                                                    */
/* DESCRIPTION: This function defines the criteria to compare two     */
/*              object entries. Comparison is based on object name.   */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      entry1          pointer to first object entry.                */
/*      entry2          pointer to second object entry.               */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      1                       entry1 > entry2                       */
/*      0                       entry1 = entry2                       */
/*      -1                      entry1 < entry2                       */
/*                                                                    */
/**********************************************************************/

static int d26_d30_ds_name_cmp(const void *entry1, const void *entry2)

{                         /*  d26_d30_ds_name_cmp                     */

/* varibles to compare two D23_ds_name structure.       */
D23_ds_name *ds_name1, *ds_name2;

/* pointers used when decoding is not complete.         */
D23_av_value *av_value1, *av_value2;

/* minimum length.      */
signed16 min_len;

ds_name1 = (D23_ds_name *)entry1;
ds_name2 = (D23_ds_name *)entry2;

if (ds_name1->d23_dscomplete == TRUE && ds_name2->d23_dscomplete == TRUE)
	return(strcmp((char *)ds_name1 + sizeof(D23_ds_name),
		      (char *)ds_name2 + sizeof(D23_ds_name)));

if (ds_name1->d23_dscomplete == TRUE)
	return(1);

if (ds_name2->d23_dscomplete == TRUE)
	return(-1);

switch(strcmp((char *)ds_name1 + sizeof(D23_ds_name),
	      (char *)ds_name2 + sizeof(D23_ds_name)))
	{
	case 1:
		return(1);
	case -1:
		return(-1);
	case 0:
		av_value1 = (D23_av_value *)((byte *)ds_name1 +
				sizeof(D23_ds_name) + ds_name1->d23_dsoff);
		av_value2 = (D23_av_value *)((byte *)ds_name2 +
				sizeof(D23_ds_name) + ds_name2->d23_dsoff);

		min_len = av_value1->d23_avlen < av_value2->d23_avlen ?
				av_value1->d23_avlen : av_value2->d23_avlen;

		switch(memcmp((byte *)av_value1 + sizeof(D23_av_value),
			(byte *)av_value2 + sizeof(D23_av_value), min_len))
			{
			case 1:
				return(1);
			case -1:
				return(-1);
			case 0:
				if (av_value1->d23_avlen > av_value2->d23_avlen)
					return(1);

				if (av_value1->d23_avlen < av_value2->d23_avlen)
					return(-1);

				if (av_value1->d23_avlen ==av_value2->d23_avlen)
					return(0);

			}
	}

}                         /*  d26_d30_ds_name_cmp                     */


/**********************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_d31_copy_ent_to_res                               */
/*                                                                    */
/* AUTHOR:      Sanjay Jain, Siemens (India)                          */
/* DATE:        22.07.92                                              */
/*                                                                    */
/* DESCRIPTION: This function depending on loop (MASTER or            */
/*      SHADOW loop) copies entries from subresults to result  block. */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      result_scan     dbl. pointer to current pos. in res.          */
/*      loop            indicates if master/shadow entries            */
/*                      reqd.                                         */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      result_scan;                                                  */
/*      last_bind_tab_index     index to last bind table              */
/*                              entry handled. Useful when            */
/*                              size limit exceeds.                   */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      D2_NO_ERR       no. error.                                    */
/*      D26_ERROR       tsearch returns D2_NULL.                      */
/*                                                                    */
/**********************************************************************/

static signed16 d26_d31_copy_ent_to_res(byte **result_scan,
    signed32 *last_bind_tab_index, boolean loop)

{                                     /* d26_d31_copy_ent_to_res().   */

/* AUTO      ...    */

/* index to scan bind table.    */
signed32        bind_tab_index;

/* pointer to distinguish name structure in result block.       */
byte           *ds_name;

/* pointer to attribute info. structure in result block.        */
byte           *at_info;

/* pointer to scan subresults.  */
byte           *sub_result_scan;

/* pointer to the beginning of entry info. in subresult block.  */
byte           *sub_entry_info;

/* pointer to attribute info. structure in subresult block.     */
byte           *sub_at_info;

/* result returned by tsearch     */
void            **tsearch_result;

/* general pointer used to skip D23_ds_name str. in subresult or result. */
byte           *message;

/* collect master/shadow entries as long as size limit not exceeded.    */
for (bind_tab_index = 0; bind_tab_index < d26_bind_count; bind_tab_index++)
	{
	sub_result_scan = d26_bind_table[bind_tab_index].d26_result;

	/* result with entries present for this bind table entry.       */
	if (sub_result_scan != (byte *)NULL &&
	    d26_bind_table[bind_tab_index].d26_result_type == D26_RD_RESULT)
		{
		/* skip till entry info.        */
		sub_result_scan = d26_d27_skip_till_entry(sub_result_scan);

		/* entries still present in the subresult       */
		while (((D23_en_info *)sub_result_scan)->d23_entag == D23_ENTTAG
			|| ((D23_subord_info *)sub_result_scan)->d23_sotag ==
								D23_SOITAG)
			{
			sub_entry_info = sub_result_scan;

			/* move to next entry in subresult.     */
			sub_result_scan = d26_d28_skip_entry_subord(
							sub_result_scan);

			/* operation is search. */
			if (((D23_en_info *)sub_entry_info)->d23_entag ==
								D23_ENTTAG)
				{
			/* entry is shadow entry and function is called from */
			/* master loop.                                      */
				if (((D23_en_info *)sub_entry_info)->d23_enmas
					!= TRUE && loop == D26_MASTER_LOOP)
					continue;

			/* entry is master entry and function is called from */
			/* shadow loop.                                      */
				if (((D23_en_info *)sub_entry_info)->d23_enmas
					== TRUE && loop == D26_SHADOW_LOOP)
					continue;
				}
			else
				{
				/* operation is list.   */

			/* entry is shadow entry and function is called from */
			/* master loop.                                      */
				if (((D23_subord_info *)sub_entry_info)->
				d23_somas != TRUE && loop == D26_MASTER_LOOP)
					continue;

			/* entry is master entry and function is called from */
			/* shadow loop.                                      */
				if (((D23_subord_info *)sub_entry_info)->
				d23_somas == TRUE && loop == D26_SHADOW_LOOP)
					continue;
				}

			/* valid entry in subresult.    */

			if (d26_serv_contr->d23_scslm != D2_S_UNLIMITED &&
			    d26_entry_count == d26_serv_contr->d23_scslm)
				{
				/* size limit exceeded. */
				d26_size_ex = TRUE;
				break;  /* break from analysing this subres.*/
				}

			/* set useful pointers in subresult block.      */
			if (((D23_en_info *)sub_entry_info)->d23_entag ==
								D23_ENTTAG)
				message = sub_entry_info + sizeof(D23_en_info);
			else
				message = sub_entry_info +
							sizeof(D23_subord_info);

			D26_SKIP_DS_NAME(message);
			sub_at_info = message;

			/* copy D23_en_info/D23_subord_info, D23_ds_name */
			/* and entry name till offset.                   */
			memcpy(*result_scan, sub_entry_info,
						sub_at_info - sub_entry_info);

			/* set useful pointers in result block. */
			if (((D23_en_info *)sub_entry_info)->d23_entag ==
								D23_ENTTAG)
				message = ds_name = *result_scan +
							sizeof(D23_en_info);
			else
				message = ds_name = *result_scan +
							sizeof(D23_subord_info);

			D26_SKIP_DS_NAME(message);
			at_info = message;

			tsearch_result = (void **)D27_TSEARCH(
			  svc_c_sev_warning,(void *)ds_name,&d26_res_root,
			  d26_d30_ds_name_cmp);

			if (tsearch_result == (void **)NULL)
				{
				return(D26_ERROR);
				}

			/* entry not already present in the result. */
			if (*tsearch_result == (void *)ds_name)
				{
				memcpy(at_info, sub_at_info,
						sub_result_scan - sub_at_info);

				*result_scan +=
					(sub_result_scan - sub_entry_info);
				d26_entry_count++;
				}

			}       /* end of loop to copy entries into result */

		if (d26_size_ex)
			break;  /* break from analysing rest of the bind   */
				/* table entries.                          */

		}       /* end of condition, result with entries.       */
	}       /* end of going thru. all bind table entries.   */

*last_bind_tab_index = bind_tab_index;

return(D2_NO_ERR);

}       /* end of d26_d31_copy_ent_to_res().    */

/**********************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_d32_call_tdelete                                  */
/*                                                                    */
/* AUTHOR:      Sanjay Jain, Siemens (India)                          */
/* DATE:        22.07.92                                              */
/*                                                                    */
/* DESCRIPTION: This function is the action routine for twalk. 'twalk'*/
/*      and this routine are used to delete all entry nodes from      */
/*      binary tree.                                                  */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      node    address of the node being visited.                    */
/*      order   order could be preorder, postorder, endorder or leaf  */
/*              depending on whether this is the first, second or     */
/*              third time that node is being visited.                */
/*      level   level of the node in the tree, with the root being    */
/*              level 0.                                              */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*                                                                    */
/**********************************************************************/

static void d26_d32_call_tdelete(const void *node, VISIT order, int level)

{                                     /* d26_d32_call_tdelete().      */

if (order == leaf || order == endorder)
	tdelete(node, &d26_res_root, d26_d30_ds_name_cmp);

}                                     /* d26_d32_call_tdelete().      */

/**********************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_d33_merge_results                                 */
/*                                                                    */
/* AUTHOR:      Sanjay Jain, Siemens (India)                          */
/* DATE:        22.07.92                                              */
/*                                                                    */
/* DESCRIPTION: This function is called only for list and search      */
/*      operations. It merges results of subrequests.                 */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      *result         pointer to merged result.                     */
/*      *result_len     length of merged result block.                */
/*      *remlen         unused length in the result block.            */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      D2_NO_ERR       no. error.                                    */
/*      D26_ERROR       error in memory allocation for the result or  */
/*                      d26_d31_copy_ent_to_res returns error.        */
/*                                                                    */
/**********************************************************************/

static signed16 d26_d33_merge_results(byte **result, signed32 *result_len,
    signed32 *remlen)

{                             /* d26_d33_merge_results().             */

/* function identifier for logging      */
static  String  func_id = "d26_d33_merge_results";

/* index to scan bind table.    */
signed32        bind_tab_index;

/* max. result length reqrd. to store the result.       */
signed32        max_result_len;

/* pointer to fill result space.        */
byte           *result_scan;

/* pointer to scan first result to extract Base object name.    */
byte           *first_result_scan;

/* index to scan referral table.        */
signed32 ref_index;

DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s"),GDS_S_GENERAL,svc_c_debug3,
  GDS_S_ENTRY_FUNCTION,func_id));

/* calculate max. of space required for the results.    */
for (bind_tab_index = 0, max_result_len = 0; bind_tab_index < d26_bind_count;
					     bind_tab_index++)
	max_result_len += d26_bind_table[bind_tab_index].d26_result_len;

/* allocate space for results.  */
if ((result_scan = *result = (byte *)D27_MALLOC(svc_c_sev_warning,
    max_result_len)) == NULL)
	{
	DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s%d"),GDS_S_GENERAL,
	  svc_c_debug3,GDS_S_EXIT_FUNCTION,func_id,D26_ERROR));
	return(D26_ERROR);
	}

/* write chaining result if required.   */

/* in the current implementation chaining results in subresults are ignored. */
/* Later on perhaps chaining results of subrequests should also be taken care*/
/* of.                                                                       */

/* incoming request is a DSP request.   */
if (d26_chain_arg != (D23_chain_arg *)NULL)
	{
	((D23_chain_res *)result_scan)->d23_chrtag   = D23_CHRTAG;
	((D23_chain_res *)result_scan)->d23_max_cref = 0;

	result_scan += sizeof(D23_chain_res);
	}

/* handle base object name which is present when base object is an alias */
/* and application wants to dereference alias.                           */
/* copy base object info from first result to current total result.      */

for (bind_tab_index = 0; bind_tab_index < d26_bind_count; bind_tab_index++)
	if (d26_bind_table[bind_tab_index].d26_result != (byte *)NULL &&
	    d26_bind_table[bind_tab_index].d26_result_type == D26_RD_RESULT)
		break;

first_result_scan = d26_bind_table[bind_tab_index].d26_result;

/* if first result is chaining result.  */
if (((D23_chain_res *)first_result_scan)->d23_chrtag == D23_CHRTAG)
	first_result_scan += sizeof(D23_chain_res);

if (((D23_ds_name *)first_result_scan)->d23_dstag == D23_BONTAG)
	{
	if (((D23_ds_name *)first_result_scan)->d23_dscomplete == TRUE)
		memcpy(result_scan, first_result_scan, sizeof(D23_ds_name) +
				((D23_ds_name *)first_result_scan)->d23_dsoff);
	else
		{
		memcpy(result_scan, first_result_scan,
		sizeof(D23_ds_name) +
		((D23_ds_name *)first_result_scan)->d23_dsoff +
		sizeof(D23_av_value) +
		((D23_av_value *)(first_result_scan + sizeof(D23_ds_name) +
		((D23_ds_name *)first_result_scan)->d23_dsoff))->d23_avoff);
		}

	D26_SKIP_DS_NAME(result_scan);
	}

/* collect master entries from subresults.      */
if (d26_d31_copy_ent_to_res(&result_scan, &bind_tab_index, D26_MASTER_LOOP) ==
								D26_ERROR)
	{
	DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s%d"),GDS_S_GENERAL,svc_c_debug3,
	  GDS_S_EXIT_FUNCTION,func_id,D26_ERROR));
	return(D26_ERROR);
	}

/* size limit has not exceeded and shadows could be present in the result. */
if (!d26_size_ex && !(d26_serv_contr->d23_scopt & D2_NOCACHE))
	/* collect shadow entries from subresults.      */
	if (d26_d31_copy_ent_to_res(&result_scan, &bind_tab_index,
					D26_SHADOW_LOOP) == D26_ERROR)
		{
		DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s%d"),GDS_S_GENERAL,
		  svc_c_debug3,GDS_S_EXIT_FUNCTION,func_id,D26_ERROR));
		return(D26_ERROR);
		}

/* size limit exceeded. */
if (d26_size_ex)
	{
	/* for the last bind table entry being handled and rest (results     */
	/* with entries not handled till now), set the result pointers to    */
	/* NULL, so that these bind table entries come in partial outcome    */
	/* qualifier.                                                        */
	for (; bind_tab_index < d26_bind_count; bind_tab_index++)
		if (d26_bind_table[bind_tab_index].d26_result != (byte *)NULL
		    && d26_bind_table[bind_tab_index].d26_result_type ==
							D26_RD_RESULT)
			{
			free(d26_bind_table[bind_tab_index].d26_result);

			d26_bind_table[bind_tab_index].d26_result =
								(byte *)NULL;
			d26_bind_table[bind_tab_index].d26_result_len = 0;
			}
	}

for                 /* for all except local bind table entry            */
   (bind_tab_index = 1; bind_tab_index < d26_bind_count; bind_tab_index++)
{
  if                /* bind table entry has non error results           */
    (d26_bind_table[bind_tab_index].d26_result != (byte *)NULL &&
     d26_bind_table[bind_tab_index].d26_result_type != D26_RD_ERROR)
  {                 /* ignore the referral                              */
    d26_bind_table[bind_tab_index].d26_referral->d26_ignore = TRUE;
  }
}

/* set d26_ref_present flag depending on if there exists at least one   */
/* referral which should not be ignored.                                */

d26_ref_present = FALSE;
for (ref_index = 0; ref_index < d26_ref_count; ref_index++)
{
  if                /* bind table entry was assigned                    */
    ((bind_tab_index = d26_referral[ref_index].d26_bt_index) >= 0)
  {
    if              /* associated reference is ignored                  */
      (d26_bind_table[bind_tab_index].d26_referral->d26_ignore == TRUE)
    {               /* ignore the actual referral too                   */
      d26_referral[ref_index].d26_ignore = TRUE;
    }
  }
  if (d26_referral[ref_index].d26_ignore == FALSE)
  {
    d26_ref_present = TRUE;
  }
}

*remlen = max_result_len - (result_scan - *result);
*result_len = max_result_len;

/* write partial outcome qualifier.     */
if (d26_u30_write_part_out_qual(*result + *result_len - *remlen, result, remlen,
							result_len) == NULL)
	{
	DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s%d"),GDS_S_GENERAL,
	  svc_c_debug3,GDS_S_EXIT_FUNCTION,func_id,D26_ERROR));
	return(D26_ERROR);
	}

/* write common result info. and EOM.   */
if (d26_u39_write_eom(*result + *result_len - *remlen,remlen,result,
	result_len,D23_SEARCH) == NULL)
	{
	DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s%d"),GDS_S_GENERAL,
	  svc_c_debug3,GDS_S_EXIT_FUNCTION,func_id,D26_ERROR));
	return(D26_ERROR);
	}

DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s%d"),GDS_S_GENERAL,svc_c_debug3,
  GDS_S_EXIT_FUNCTION,func_id,D2_NO_ERR));
return(D2_NO_ERR);

}                             /* end of d26_d33_merge_results().      */

/**********************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_d34_comp_bind_table                               */
/*                                                                    */
/* AUTHOR:      Sanjay Jain, Siemens (India)                          */
/* DATE:        20.10.93                                              */
/*                                                                    */
/* DESCRIPTION: This function compresses the bind table. It calls the */
/*      function to send DSP unbinds.                                 */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      return value from d26_d35_send_dsp_unbinds()                  */
/*                                                                    */
/**********************************************************************/

signed16 d26_d34_comp_bind_table(void)

{                             /* d26_d34_comp_bind_table().           */

			      /* function identifier for logging      */
static char *func_id = "d26_d34_comp_bind_table";

			      /* index to scan bind table             */
signed32 bind_tab_index, bind_tab_index2, last_latest_usage_index;

time_t   latest_usage;

DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s"),GDS_S_GENERAL,svc_c_debug3,
  GDS_S_ENTRY_FUNCTION,func_id));

/* for same associations keep the latest one only.      */
for (bind_tab_index = 1; bind_tab_index < d26_bind_count; bind_tab_index++)
	{
	if (d26_bind_table[bind_tab_index].d26_state == D26_IDLE)
		{
		for (bind_tab_index2 = bind_tab_index + 1,
		     latest_usage=d26_bind_table[bind_tab_index].d26_last_usage,
		     last_latest_usage_index = bind_tab_index;
		     bind_tab_index2 < d26_bind_count; bind_tab_index2++)
			{
			if ((d26_bind_table[bind_tab_index2].d26_state ==
			     D26_IDLE) &&
			    (d26_bind_table[bind_tab_index2].d26_assid ==
			     d26_bind_table[bind_tab_index].d26_assid))
				{
				if (d26_bind_table[bind_tab_index2].
				    d26_last_usage
				    >= d26_bind_table[last_latest_usage_index].
								d26_last_usage)
					{
					d26_bind_table[last_latest_usage_index].
					d26_state = D26_UNBOUND;

					free(d26_bind_table[
						last_latest_usage_index].
							d26_psap.d2_value);

					latest_usage = d26_bind_table[
						bind_tab_index2].d26_last_usage;
					last_latest_usage_index =
						bind_tab_index2;
					}
				else
					{
					d26_bind_table[bind_tab_index2].
					d26_state = D26_UNBOUND;
					free(d26_bind_table[bind_tab_index2].
							d26_psap.d2_value);
					}
				}
			}

		}
	}

DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s%d"),GDS_S_GENERAL,svc_c_debug3,
  GDS_S_EXIT_FUNCTION,func_id,D2_NO_ERR));
/* if error is returned some unbind requests may be pending and bind table */
/* may be in inconsistent state.                                           */
return(d26_d35_send_dsp_unbinds());

}       /* end of d26_d34_comp_bind_table()     */

/**********************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_d35_send_dsp_unbinds                              */
/*                                                                    */
/* AUTHOR:      Sanjay Jain, Siemens (India)                          */
/* DATE:        20.10.93                                              */
/*                                                                    */
/* DESCRIPTION: It sends the unbind for the bind entries whose usage  */
/*      has expired.                                                  */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      D2_NO_ERR       normal.                                       */
/*      D26_ERROR       malloc or programming error.                  */
/* if error is returned some unbind requests may be pending and bind  */
/* table may be in inconsistent state.                                */
/*                                                                    */
/**********************************************************************/

static signed16 d26_d35_send_dsp_unbinds(void)

{                            /* d26_d35_send_dsp_unbinds().          */

			     /* function identifier for logging      */
static char *func_id = "d26_d35_send_dsp_unbinds";

			     /* parameter-block for wait result      */
static  D23_wtrspb  wtrspb = {{D23_V02},D23_INFIN};

/* parameter-block for invoke           */
static  D23_inrqpb  inrqpb = {{D23_V02}};

/* parameter-block for unbind           */
static  D23_ubndpb  ubndpb = {{D23_V02}};

/* starting address to store result.    */
byte *result;

/* total length of result buffer.       */
signed32 result_len;

/* length of empty part of result buffer.       */
signed32 remlen;

/* error code returned by collect_result.       */
signed16 err_id;

/* index to scan bind table.    */
signed32        bind_tab_index;

signed16        new_bind_count;

DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s"),GDS_S_GENERAL,svc_c_debug3,
  GDS_S_ENTRY_FUNCTION,func_id));

/* unbind those binds whose last_usage has expired.     */
for (bind_tab_index = 1; bind_tab_index < d26_bind_count; bind_tab_index++)
	{
	if (d26_bind_table[bind_tab_index].d26_state == D26_IDLE)
		{
		/* unbind required.     */
		if ((time((time_t)NULL) -
		    d26_bind_table[bind_tab_index].d26_last_usage) >=
							d26_dspunbind_delay)
			{
			/* prepare unbind invoke block. */
			inrqpb.d23_6assid  = d26_bind_table[
						bind_tab_index].d26_assid;
			inrqpb.d23_6invinfo.d23_contid = D23_SAC(D27_DSP_AC)
			  | D23_SACM(D27_LOC_AC) | D23_SAS(D27_DSP_AS);
			inrqpb.d23_6invinfo.d23_usrid  = inrqpb.d23_6assid;
			inrqpb.d23_6rfidt  = d26_bind_table[
						bind_tab_index].d26_refms;

			/* prepare wtrspb for d10_unbind.       */
			wtrspb.d23_7invinfo.d23_contid =
			  d26_bind_table[bind_tab_index].d26_contid;
			wtrspb.d23_7rltid = D23_RESULT;

			/* set transfer syntax                          */
			D23_CTS(inrqpb.d23_6invinfo.d23_contid,
			  D23_GTS(wtrspb.d23_7invinfo.d23_contid));

			/* send unbind request. */
			if (d26_d10_unbind_request(&inrqpb, &wtrspb)==D2_NO_ERR)
				{
				d26_bind_table[bind_tab_index].d26_state =
					D26_UNBIND_PENDING;
				}
			else
				{
				d26_bind_table[bind_tab_index].d26_state =
								D26_INV_ERROR;

				/* prepare ipc unbind block.    */
				ubndpb.d23_5assid  = d26_bind_table[
						bind_tab_index].d26_assid;

				/* make ipc unbind from sstub.*/
				d23_unbind(&ubndpb);
				}
			}
		}
	}

/* allocate memory for unbind result.   */

/*  error from allocating result memory     */
if ((result = (byte *)D27_MALLOC(svc_c_sev_warning,D26_BLOCK_LEN)) == NULL)
	{
	return(D26_ERROR);
	}

result_len = D26_BLOCK_LEN;

remlen = result_len;

/* wait for the unbind results. */
for (;;)
	{
	for (bind_tab_index = 1; bind_tab_index < d26_bind_count;
							bind_tab_index++)
		if (d26_bind_table[bind_tab_index].d26_state ==
							D26_UNBIND_PENDING)
			break;

	if (bind_tab_index == d26_bind_count)
		break;

	/* collect next result. */
	d26_d05_collect_result(D23_UNBIND,(D23_rsrqpb *)NULL,&wtrspb,&result,
	  &result_len,&remlen,&err_id);

	/* look for bind table entry which has same association-id. */
	for (bind_tab_index = 1; bind_tab_index < d26_bind_count;
						bind_tab_index++)
		if (d26_bind_table[bind_tab_index].d26_assid ==
						wtrspb.d23_7assid &&
		    d26_bind_table[bind_tab_index].d26_state ==
							D26_UNBIND_PENDING)
			break;

	if (bind_tab_index == d26_bind_count)
		{
		return(D26_ERROR);
		}

	d26_bind_table[bind_tab_index].d26_state = D26_UNBOUND;

	free(d26_bind_table[bind_tab_index].d26_psap.d2_value);

	/* prepare ipc unbind block.    */
	ubndpb.d23_5assid  = d26_bind_table[bind_tab_index].d26_assid;

	/* make ipc unbind from sstub.*/
	d23_unbind(&ubndpb);
	}

free(result);

new_bind_count = 1;

/* compress bind table. */
for (bind_tab_index = 1; bind_tab_index < d26_bind_count; bind_tab_index++)
{
  if (d26_bind_table[bind_tab_index].d26_state == D26_IDLE)
  {
	  /* copying entry required.      */
    if (bind_tab_index > new_bind_count)
    {
      d26_bind_table[new_bind_count] = d26_bind_table[bind_tab_index];
    }

    d26_bind_table[new_bind_count].d26_referral   = (D26_referral *)NULL;
    d26_bind_table[new_bind_count].d26_result     = (byte *)NULL;
    d26_bind_table[new_bind_count].d26_result_len = 0;

    new_bind_count++;
  }
  else if                   /* PSAP not released (any error state)    */
	 (d26_bind_table[bind_tab_index].d26_state != D26_UNBOUND)
  {                         /* free PSAP address                      */
    free(d26_bind_table[bind_tab_index].d26_psap.d2_value);
  }
}

d26_bind_count = new_bind_count;

DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s%d"),GDS_S_GENERAL,svc_c_debug3,
  GDS_S_EXIT_FUNCTION,func_id,D2_NO_ERR));

return(D2_NO_ERR);

}       /* end of d26_d35_send_dsp_unbinds()    */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_d36_find_routing_acp                              */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        26.10.93                                              */
/*                                                                    */
/* DESCRIPTION: This function reads the presentation addresses of the */
/*              routing DSAs from the database.                       */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      None                                                          */
/*                                                                    */
/*inoff ***************************************************************/

static void d26_d36_find_routing_acp(void)

{                       /*  d26_d36_find_routing_acp                  */

  D26_dninfo dninfo;    /* name structure of DSA name                 */
  D26_rdninfo *l_rdn;   /* last RDN of routing DSA name               */

  register signed16 i;  /* loop variable                              */
  register D26_acc_point *acp;  /* pointer to access point structure  */

  for                   /*  all routing DSAs                          */
     (i = 0, acp = d26_routing_access_points; i < d26_nr_routing_dsas;
      i++, acp++)
  {                     /*  get the name structure of the DSA name    */
    dninfo.d26_rd_anz = 0;
    if                  /*  name structure of routing DSA is ok       */
      (d26_u06_get_name(acp->d26_dsaname,&dninfo) == D2_NO_ERR)
    {                   /*  read the entry from database              */
      if                /*  DSA-object could  be found                */
	(d26_i31_find_object(&dninfo,&l_rdn,d26_ob_record) == D2_NO_ERR)
      {                 /*  read objects PSAP-Address                 */
	d26_i58_get_accp_info(dninfo.d26_filenr,d26_ob_record,&acp->d26_psap,
	  acp->d26_auth_m,&acp->d26_pr_name);
      }
    }
  }

}                       /*  d26_d36_find_routing_acp                  */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_d37_free_routing_acp                              */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        26.10.93                                              */
/*                                                                    */
/* DESCRIPTION: This function frees the presentation addresses of the */
/*              routing DSAs.                                         */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      None                                                          */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      None                                                          */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      None                                                          */
/*                                                                    */
/*inoff ***************************************************************/

static void d26_d37_free_routing_acp(void)

{                       /*  d26_d37_free_routing_acp                  */

register signed16 i;        /* loop variable                          */
register D26_acc_point *acp;/* pointer to access point structure      */

for                     /*  all routing DSAs                          */
   (i = 0, acp = d26_routing_access_points; i < d26_nr_routing_dsas;
    i++, acp++)
{                       /*  get the name structure of the DSA name    */
  if                    /*  presentation address allocated            */
    (acp->d26_psap.d2_value != NULL)
  {                     /*  free it                                   */
    free(acp->d26_psap.d2_value);
  }
}

}                       /*  d26_d37_free_routing_acp                  */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_d38_next_routing_psap                             */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        26.10.93                                              */
/*                                                                    */
/* DESCRIPTION: This function determines the next presentation address*/
/*              to use from the referral and the routing access point */
/*              list according to the configurated priorities         */
/*		For the returned access point also the supported      */
/*		authentication mechanisms and the pricipal name are   */
/*		will be returned.				      */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      NULL        D2_str *            invalid PSAP address          */
/*      != NULL                         PSAP address to use           */
/*                                                                    */
/*inoff ***************************************************************/

static D2_str *d26_d38_next_routing_psap(
			/* IN     - reference to the target DSA       */
    D26_referral *referral,
			/* INOUT  - number of route to use            */
    signed16     *route_nr,
			/* IN     - purpose of the bind table entry   */
    signed16     purpose,
    boolean	 *auth_mechs,	/* OUT - auth. mechanisms	      */
				/*       array of D2_MAX_AUTH_MECHS   */
				/*       of boolean to be provided!   */
    char         **princ_name,	/* OUT - principal name 	      */
    D2_name_string *dsa_name)	/* OUT - target DSA name 	      */

{                       /*  d26_d38_next_routing_psap                 */

  signed16 acp_idx;     /* index to entry in access point list        */
  D2_str   *return_str = NULL; /* contains return value		      */
  boolean  *auth_ptr = NULL;   /* pointer to auth. mech. array	      */
  char	   *pr_nm_ptr = NULL;  /* pointer to principal name	      */

  signed16 len;		/* length of princ_name			      */
  register signed16 i;  /* loop variable                              */
			/* pointer to access point structure          */
  register D26_acc_point *acp;

  if                    /* Bind table entry is used for abandon       */
    (purpose == D26_ABANDON)
  {                     /* return the PSAP from the associated bind   */
			/* table entry                                */
    return_str = &d26_bind_table[referral->d26_bt_index].d26_psap;
  }
  else
			/* take access point from referral first      */
  {
    acp_idx = referral->d26_acp[0];
    if                  /*  first call and knowledge has priority or  */
			/*  last call and routing has priority        */
      (*route_nr == 0 && d26_knowledge_prio > d26_routing_prio ||
       *route_nr == d26_nr_routing_dsas + 1 &&
       d26_routing_prio > d26_knowledge_prio && d26_knowledge_prio > 0)
    {                   /*  return PSAP address from referral         */
      (*route_nr)++;
      if                /*  referral leads to own DSA                 */
	(acp_idx == D26_OWNNAME)
      {                 /*  return pointer to own PSAP address        */
	auth_ptr = d26_auth_mech;
	pr_nm_ptr = d26_mypr_name;
	*dsa_name = d26_myname;
	return_str = &d26_own_psap;
      }
      else
      {                 /*  return PSAP from access point list        */
	auth_ptr = d26_accp_list[acp_idx].d26_auth_m;
	pr_nm_ptr = d26_accp_list[acp_idx].d26_pr_name;
	*dsa_name = d26_accp_list[acp_idx].d26_dsaname;
	return_str = &d26_accp_list[acp_idx].d26_psap;
      }
    }
    else if             /*  last call                                 */
	   (*route_nr > d26_nr_routing_dsas)
    {                   /*  return Null pointer for PSAP address      */
      (*route_nr)++;
      return_str = NULL;
    }
    else if             /*  first call and routing has priority       */
	   (*route_nr == 0 && d26_routing_prio > d26_knowledge_prio)
    {                   /*  just reset the call number                */
      (*route_nr)++;
    }

    if (return_str != NULL)
    {
      for                 /*  all routing access points                 */
         (i = *route_nr - 1, acp = d26_routing_access_points + i;
	  i < d26_nr_routing_dsas; i++, acp++)
      {                   /*  check PSAP address                        */
        if                /*  PSAP address present                      */
	  (acp->d26_psap.d2_value != NULL)
        {                 /*  reset call number                         */
	  *route_nr = i + 2;
	  auth_ptr = acp->d26_auth_m;
	  pr_nm_ptr = acp->d26_pr_name;
	  *dsa_name = acp->d26_dsaname;
	  return_str = &acp->d26_psap;
	  break;		/* leave for-loop */
        }
      }
  
      if                  /*  no routing DSA found and knowledge DSA may*/
			  /*  be used                                   */
        (return_str == NULL && d26_knowledge_prio > 0)
      {                   /*  return PSAP address from referral         */
        *route_nr = d26_nr_routing_dsas + 2;
        if                /*  referral leads to own DSA                 */
	  (acp_idx == D26_OWNNAME)
        {                 /*  return pointer to own PSAP address        */
	  auth_ptr = d26_auth_mech;
	  pr_nm_ptr = d26_mypr_name;
	  *dsa_name = d26_myname;
	  return_str = &d26_own_psap;
        }
        else
        {                 /*  return PSAP from access point list        */
	  auth_ptr = d26_accp_list[acp_idx].d26_auth_m;
	  pr_nm_ptr = d26_accp_list[acp_idx].d26_pr_name;
	  *dsa_name = d26_accp_list[acp_idx].d26_dsaname;
	  return_str = &d26_accp_list[acp_idx].d26_psap;
        }
      }
    }
  }

  /* provide principal name and auth. mechs to output parameters */
  if (auth_ptr != NULL)
  {
    for (i = 0; i < D2_MAX_AUTH_MECHS; i++)
	auth_mechs[i] = auth_ptr[i];
  }

  if (pr_nm_ptr != NULL)
  {
    if ((len = strlen(pr_nm_ptr)) == 0)	/* could happen in case of */
	*princ_name = NULL;		/* d26_mypr_name 	   */
    else
    {
       *princ_name = pr_nm_ptr;
    }
  }

  return(return_str);
}                       /*  d26_d38_next_routing_psap                 */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_d39_reusable_bte                                  */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        26.10.93                                              */
/*                                                                    */
/* DESCRIPTION: This function searches for a bind table entry with the*/
/*              same presentation address, where no other entry with  */
/*              a pending state exists.                               */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      psap        D2_str *            presentation address          */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      None                                                          */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      D2_NULL     D26_bind_tab_ent *  no reusable bind table entry  */
/*                                      found                         */
/*      != D2_NULL                      reusable bind table entry     */
/*                                                                    */
/*inoff ***************************************************************/

static D26_bind_tab_ent *d26_d39_reusable_bte(
    D2_str *psap)

{                       /*  d26_d39_reusable_bte                      */

/* index to scan bind table.    */
signed32 bind_tab_index;

/* saved index to an idle bind. */
signed32 sav_bind_tab_index;

      /* look for an idle bind to same PSAP address.                  */
for (bind_tab_index = 1; bind_tab_index < d26_bind_count; bind_tab_index++)
	{
	      /* compare two accesspoints based on the psap address.  */

	if (psap->d2_size != d26_bind_table[bind_tab_index].d26_psap.d2_size)
		continue;

	if (memcmp(psap->d2_value,
		   d26_bind_table[bind_tab_index].d26_psap.d2_value,
		   d26_bind_table[bind_tab_index].d26_psap.d2_size) != 0)
		continue;

	if (d26_bind_table[bind_tab_index].d26_state == D26_IDLE)
		{
		sav_bind_tab_index = bind_tab_index;
		break;
		}
	else
		continue;
	}

/* idle bind to the same DSA found.     */
if (bind_tab_index < d26_bind_count)
	{
	/* look for a bind which has same association and request pending. */
	for (bind_tab_index = 1; bind_tab_index < d26_bind_count;
						bind_tab_index++)
		{
		if (d26_bind_table[bind_tab_index].d26_assid ==
		    d26_bind_table[sav_bind_tab_index].d26_assid)
			if (d26_bind_table[bind_tab_index].d26_state ==
							D26_REQUEST_PENDING)
				break;
			else
				continue;
		else
			continue;
		}

	if (bind_tab_index < d26_bind_count)
		return(NULL);
	else
		return(d26_bind_table + sav_bind_tab_index);
	}
else
	return(NULL);

}                       /*  d26_d39_reusable_bte                      */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_d40_dupl_req_entry                                */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        26.10.93                                              */
/*                                                                    */
/* DESCRIPTION: This function searches for a bind table entry with the*/
/*              same presentation address and the same target object  */
/*              as presented in the referral. If such an entry exists,*/
/*              the function returns D2_TRUE, D2_FALSE otherwise.     */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      referral    D26_referral *      referral to the target DSA    */
/*      psap        D2_str *            presentation address          */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      None                                                          */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      D26_IDX_NOT_DEF     Int         request is not duplicated     */
/*      >  0                            index of duplicating bind     */
/*                                      table entry                   */
/*                                                                    */
/*inoff ***************************************************************/

static signed16 d26_d40_dupl_req_entry(
    D26_referral *referral,
    D2_str       *psap)

{                       /*  d26_d40_dupl_req_entry                    */

				/* index to scan bind table.    */
  signed16 bind_tab_index;

				/* pointer to PSAP address to compare */
  D2_str *cmp_psap;

				/* pointer to referral to compare */
  D26_referral *cmp_referral;

for             /* all bind table entries                             */
   (bind_tab_index = 1; bind_tab_index < d26_bind_count; bind_tab_index++)
{               /* compare lengths of PSAP addresses                  */
  cmp_psap = &d26_bind_table[bind_tab_index].d26_psap;
  if            /* psap addresses have same length                    */
    (psap->d2_size == cmp_psap->d2_size)
  {             /* compare PSAP addresses                             */
    if          /* PSAP addresses match                               */
      (!memcmp(psap->d2_value,cmp_psap->d2_value,psap->d2_size))
    {           /* compare target objects                             */
      cmp_referral = d26_bind_table[bind_tab_index].d26_referral;
      if        /* target objects match                               */
	(!d26_d41_dncmp(referral->d26_target,referral->d26_targ_ep,
	 cmp_referral->d26_target,cmp_referral->d26_targ_ep))
      {         /* return "duplicated request"                        */
	return(bind_tab_index);
      }
    }
  }
}

return(D26_IDX_NOT_DEF);

}                       /*  d26_d40_dupl_req_entry                    */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_d41_dncmp                                         */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        28.10.93                                              */
/*                                                                    */
/* DESCRIPTION: This function compares two distinguished names, which */
/*              consist of a decoded part and an encoded part each.   */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      < 0         signed16            first DN is smaller           */
/*        0                             DNs match                     */
/*      > 0                             second DN is smaller          */
/*                                                                    */
/*inoff ***************************************************************/

static signed16 d26_d41_dncmp(
			/* IN    -  decoded part of first DN          */
    D2_name_string dn_dp1,
			/* IN    -  undecoded part of first DN        */
    D23_av_value *dn_ep1,
			/* IN    -  decoded part of second DN         */
    D2_name_string dn_dp2,
			/* IN    -  undecoded part of second DN       */
    D23_av_value *dn_ep2)

{                       /*  d26_d41_dncmp                             */

  signed16 cmp;         /* indicates result of comparison             */
  signed16 minlen;      /* minimum length of encoded parts            */

  if                    /* decoded part of first dsa is smaller       */
    ((cmp = strcmp((char *)dn_dp1,(char *)dn_dp2)) < 0)
  {                     /* return "smaller"                           */
    return(-1);
  }
  else if               /* decoded parts match                        */
	 (cmp == 0)
  {                     /* check encoded parts                        */
    if                  /* both encoded parts exists                  */
      (dn_ep1 == NULL && dn_ep2 == NULL)
    {                   /* return "equal"                             */
      return(0);
    }
    else if             /* only second encoded part exists            */
	   (dn_ep1 == NULL && dn_ep2 != NULL)
    {                   /* return "smaller"                           */
      return(-1);
    }
    else if             /* only first encoded part exists             */
	   (dn_ep1 != NULL && dn_ep2 == NULL)
    {                   /* return "greater"                           */
      return(1);
    }
    else if             /* both encoded parts exist                   */
	   (dn_ep1 != NULL && dn_ep2 != NULL)
    {                   /* compare lengths                            */
      minlen = dn_ep1->d23_avlen < dn_ep2->d23_avlen ?
	dn_ep1->d23_avlen : dn_ep2->d23_avlen;
      if                /* first encoded part is smaller              */
	((cmp = memcmp((void *)(dn_ep1 + 1),(void *)(dn_ep2 + 1),minlen))
	 < 0)
      {                 /* return "smaller"                           */
	return(-1);
      }
      else if           /* initial parts match                        */
	     (cmp == 0)
      {                 /* compare lengths                            */
	if              /* first encoded part is shorter              */
	  (dn_ep1->d23_avlen < dn_ep2->d23_avlen)
	{               /* return "smaller"                           */
	  return(-1);
	}
	else if         /* encoded parts have equal length            */
	       (dn_ep1->d23_avlen == dn_ep2->d23_avlen)
	{               /* return "equal"                             */
	  return(0);
	}
	else
	{               /* return "greater"                           */
	  return(1);
	}
      }
      else
      {                 /* return "greater"                           */
	return(1);
      }
    }
  }
  else
  {                     /* return "greater"                           */
    return(1);
  }

}                       /*  d26_d41_dncmp                             */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_d42_reset_bt_index                                */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        28.10.93                                              */
/*                                                                    */
/* DESCRIPTION: This function resets the bind table index 1 in all    */
/*              bind table entries by bind table index 2.             */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      bt_index1   Int                 index to be replaced          */
/*      bt_index1   Int                 index to replace              */
/*      route_nr    Int                 number of next routing DSA    */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      None                                                          */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      None                                                          */
/*                                                                    */
/*inoff ***************************************************************/

static void d26_d42_reset_bt_index(
    signed16 bt_index1,
    signed16 bt_index2,
    signed16 route_nr)

{                       /*  d26_d42_reset_bt_index                    */

register signed16 i;        /* loop variable                          */
register D26_referral *referral;/* loop pointer to referral           */

for                     /*  all referrals                             */
   (i = 0, referral = d26_referral; i < d26_ref_count; i++, referral++)
{                       /*  check the ignore flag                     */
  if                    /*  referral must be handled                  */
    (referral->d26_ignore == FALSE)
  {                     /*  check the bind table index                */
    if                  /*  bind table index must be replaced         */
      (referral->d26_bt_index == bt_index1)
    {                   /*  check routing DSAs                        */
      if                /*  still routing DSAs available              */
	(route_nr < d26_nr_routing_dsas + 1)
      {                 /*  initialize bind table entry               */
	referral->d26_bt_index = bt_index2;
      }
      else
      {                 /*  initialize bind table entry               */
	referral->d26_bt_index = D26_IDX_NOT_DEF;
	d26_d22_init_bind_table(referral,route_nr,D26_SUBREQUEST);
      }
    }
  }
}

}                       /*  d26_d42_reset_bt_index                    */

/**********************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_d43_abandon_subrequests                           */
/*                                                                    */
/* AUTHOR:      E. Kraemer, BUBA NM12                                 */
/* DATE:        03.02.94                                              */
/*                                                                    */
/* DESCRIPTION: This function creates bind table entries for all      */
/*              outstanding subrequests with purpose "abandon"        */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

void d26_d43_abandon_subrequests(void)

{                       /* d26_d43_abandon_subrequests                */

			/* function identifier for logging            */
  static char *function = "d26_d43_abandon_subrequests";

  D26_bind_tab_ent *bte;/* bind table entry                           */
  register signed16 i;  /* loop variable                              */

  DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s"),GDS_S_GENERAL,svc_c_debug3,
    GDS_S_ENTRY_FUNCTION,function));

  for                   /* all entries of the bind table              */
     (i = 1, bte = d26_bind_table + 1; i < d26_bind_count; i++, bte++)
  {                     /* check the state                            */
    if                  /* request is pending                         */
      (bte->d26_state == D26_REQUEST_PENDING)
    {                   /* initialize new bind table entry            */
      d26_d22_init_bind_table(bte->d26_referral,bte->d26_route_nr,
	 D26_ABANDON);
    }
  }

  DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s%d"),GDS_S_GENERAL,svc_c_debug3,
    GDS_S_EXIT_FUNCTION,function,0));

  return;

}                       /* d26_d43_abandon_subrequests                */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_d44_build_abandon_request                         */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        04.02.94                                              */
/*                                                                    */
/* DESCRIPTION: This function writes the DSP-message, that is         */
/*              transferred to abandon any chained request.           */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      D2_NO_ERR   signed16        no error occurred                 */
/*      D26_ERROR                   hard error occurred.              */
/*                                                                    */
/*inoff ***************************************************************/

static signed16 d26_d44_build_abandon_request(
    byte **request,     /* OUT   -  chained request to be generated   */
			/* OUT   -  length of chained request         */
    signed32 *request_len,
    signed32 inv_id)    /* IN    -  invoke ID of request to abandon   */

{                       /*  d26_d44_build_abandon_request             */

  byte *message;            /* actual message pointer                 */

  *request_len = sizeof(D23_inabandon) + sizeof(D23_eom);

  if                        /* error from allocating request memory   */
    ((*request = (byte *)D27_MALLOC(svc_c_sev_warning,*request_len)) == NULL)
  {                         /* log error and return                   */
    return(D26_ERROR);
  }
  else
  {                         /* write header information               */
    message = *request;

    ((D23_header *)message)->d23_versno = D23_APDUV22;
    ((D23_header *)message)->d23_dirid  = 0;
    ((D23_header *)message)->d23_bindid = 0;
    message += sizeof(D23_header);
    *(signed32 *)message = inv_id;
    message += sizeof(signed32);
    ((D23_eom *)message)->d23_eomtag    = D23_EOMTAG;
    ((D23_eom *)message)->d23_eom_oqoff = 0;
  }

  return(D2_NO_ERR);

}                       /*  d26_d44_build_abandon_request             */
