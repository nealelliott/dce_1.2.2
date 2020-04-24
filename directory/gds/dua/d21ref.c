/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21ref.c,v $
 * Revision 1.1.10.2  1996/02/18  18:17:27  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:40:16  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:54:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:10:27  root]
 * 
 * Revision 1.1.8.4  1994/06/21  14:46:45  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:39:23  marrek]
 * 
 * Revision 1.1.8.3  1994/05/10  15:54:05  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:32:41  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:14:36  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:28:57  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  18:42:35  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:35:10  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  06:32:47  marrek
 * 	time limit is exceeded also if max_time=curr_time.
 * 	[1993/08/03  09:23:39  marrek]
 * 
 * 	July 1993 code drop.
 * 	[1993/07/30  10:58:44  marrek]
 * 
 * Revision 1.1.4.5  1993/02/02  10:16:19  marrek
 * 	Fixes for OT 6587
 * 	insert <pthread.h>
 * 	use const declarations
 * 	[1993/02/02  09:25:27  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  19:48:34  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:25:56  bbelch]
 * 
 * Revision 1.1.4.3  1992/11/27  21:35:57  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  13:41:36  marrek]
 * 
 * Revision 1.1.4.2  1992/10/05  11:33:25  marrek
 * 	Changes for bug 5205.
 * 	[1992/10/05  11:32:24  marrek]
 * 
 * Revision 1.1.2.4  1992/07/01  16:53:05  melman
 * 	GDS drop of 6/26/92
 * 	[1992/07/01  16:21:57  melman]
 * 
 * Revision 1.1.2.3  1992/06/02  03:29:17  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:38:04  zeliff]
 * 
 * Revision 1.1.2.2  1992/04/27  19:57:24  melman
 * 	Files from Helmut's visit
 * 	[1992/04/27  19:52:26  melman]
 * 
 * Revision 1.1  1992/01/19  15:13:52  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21ref.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:17:27 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*
* daton **************************************************************
*                                                                    *
* NAME         :  d21ref.c                                           *
*                                                                    *
* AUTHOR       :  J.Heigert                                          *
* DATE         :  09.12.87                                           *
*                                                                    *
* datoff *************************************************************
*/

#include <gds.h>
#include <stdio.h>
#include <stdlib.h>

#include <d21dua.h>

/*
* exon ***************************************************************
*                                                                    *
* TYPE         :  C-FUNCTION                                         *
*                                                                    *
* NAME         :  d21_b03_ref_bind()                                 *
*                                                                    *
* AUTHOR       :  J.Heigert,     Softlab GmbH.                       *
* DATE         :  03.02.88                                           *
*                                                                    *
* DESCRIPTION  :  Bind to DSA to be called during referral handling  *
*                                                                    *
* INPUT-PARAMETER : name_dsa: DSA-name                               *
*                   addr_dsa: ptr to DSA-address                     *
*                   bind_info: ptr to bind-tab entry of original bind*
*                                                                    *
* OUTPUT-PARAMETER : pbhead                                          *
*                    bind_id: bind_id of successful bind, (-1 otherw)*
*                                                                    *
* exoff **************************************************************
*/


d2_ret_val d21_b03_ref_bind(
  D2_pbhead * pbhead,
  D2_name_string name_dsa,
  D2_a_value * addr_dsa,
  d21_bind_info	* bind_info,
  signed16 * bind_id)
{
D21_p1	parblk;

d2_ret_val   xret;

*bind_id = -1;

/*
try to bind with given DSA-NAME and PSAP-ADDRESS
*/
parblk.d21_1pbhead = pbhead;
parblk.d21_1pbhead->d2_errclass  =
parblk.d21_1pbhead->d2_errvalue  =
parblk.d21_1pbhead->d2_retcode  = D2_NO_ERR;
parblk.d21_1dir_id = bind_info->d21_b_dir_id;
parblk.d21_1name_dsa = name_dsa;
parblk.d21_1addr_dsa = addr_dsa;
parblk.d21_1ret_dsa = NULL;
parblk.d21_1bind_id = 0;
parblk.d21_1auth_mech = bind_info->d21_b_auth_mech;

parblk.d21_1u_credent = bind_info->d21_b_credent;

xret = d21_bh1_exec_bind(&parblk,TRUE);
				/* referrals are not returned from BIND	*/
				/* in this case!			*/

if (parblk.d21_1ret_dsa != NULL)
	free(parblk.d21_1ret_dsa);

*pbhead = *(parblk.d21_1pbhead);
if ( (xret == D2_NOERROR) && (pbhead->d2_retcode == D2_NO_ERR) )
	{
	*bind_id = parblk.d21_1bind_id;
	return(D2_NOERROR);
	}
else
	{
	return(D2_ERROR);
	}

} /* end of d21_b03_ref_bind() */


d2_ret_val d21_handle_ref(
  D21_lb_info * lb_info,
  signed16 * lb_nr,
  D23_rf_info ** ref_ptr,
  D2_pbhead * pbhead,
  time_t maxtime,
  signed16 bind_id,
  signed16 op_id,
  char ** res_ptr,
  signed32 * res_len,
  char * req_ptr,
  signed32 req_len,
  d21_bind_info * bt_ptr)
{
time_t curr_time;
d2_ret_val ret_value = D2_NOERROR;
D2_name_string         rdsa_name ;
char * r_target ;
D23_av_value * r_psap;
D2_a_value psap;
signed16 rbind_id = D2_NO_BIND;
signed16 i;
signed16 offset,old_offset;
signed32 old_req_len;
D21_p2 unbd_parblk;
D2_pbhead u_pbhead;
d21_bind_info r_b_tab;
D23_rf_info * r_ptr = *ref_ptr;
D21_req_info req_info;
char * req_ptr1 = req_ptr;
char * apdu_ptr;
const char * function_name = "d21_handle_ref";
Bool cpy = FALSE;
Bool r_new_ipc = FALSE;

/*
   set pbheads
*/
u_pbhead.d2_version =  pbhead->d2_version;

/*
   start handling of referrals
*/
for (D2_EVER)
{
    r_target = (char *)(r_ptr+1);
    if (((D23_ds_name *)r_target)->d23_dslen == 0)
	r_target = NULL;
    else
	r_target += D23_S_NM;


/* 
copy target object as base object in the next operation request ????
*/
    switch (op_id) {
       case (D23_READ+D2_ADM_OPID):
       case D23_READ: /* copy target object to inread APDU */
       case D23_LIST: /* copy target object to inlist APDU */
       case D23_COMPARE: /* copy target object to incompare APDU */
       case D23_ADDOBJT: /* copy target object to inaddobjt APDU */
       case D23_REMOBJT: /* copy target object to inremobjt APDU */
       case D23_MODENTR: /* copy target object to inmodentr APDU */
   			offset =  D23_S_HDR + D23_S_CARG;
                        break;
       case (D23_SEARCH+D2_ADM_OPID):
       case D23_SEARCH: /* copy target object to insearch APDU */
   			offset =  D23_S_HDR + D23_S_CARG+D23_S_SCHARG;
                        break;
       case D23_REPLRDN: /* copy target object to inreprdn APDU */
   			offset =  D23_S_HDR + D23_S_CARG+D23_S_REP;
                        break;
	default :;
    }
   apdu_ptr =  req_ptr + offset;
   if (((D23_ds_name *)apdu_ptr)->d23_dslen == 0)
     apdu_ptr = NULL;
   else 
     apdu_ptr += D23_S_NM;
   if (apdu_ptr != NULL && r_target != NULL && strcmp(apdu_ptr, r_target) != 0)
   {
    if (strlen(apdu_ptr) >= strlen(r_target))
   	 { strcpy(apdu_ptr, r_target);
	  ((D23_ds_name *)(apdu_ptr-D23_S_NM))->d23_dslen = strlen(r_target)+1;
          }
     else
	{
	  old_req_len = req_len;
          req_len += strlen(r_target)-strlen(apdu_ptr); /* increment req_len */
          req_len = D2_ALIGN(req_len) ;
	  old_offset = offset += D23_S_NM; 
	  old_offset +=  ((D23_ds_name *)(apdu_ptr-D23_S_NM))->d23_dsoff;
	  cpy = TRUE;
	 }
  }
  else
   if (apdu_ptr == NULL && r_target != NULL)
    {
	  old_req_len = req_len;
          req_len += strlen(r_target) + 1; /* increment req_len */
          req_len = D2_ALIGN(req_len) ;
          old_offset = offset += D23_S_NM;
          apdu_ptr =  req_ptr + offset;
	  cpy = TRUE;
     }
    else
     if (apdu_ptr != NULL && r_target == NULL)
      {
	*apdu_ptr = (char)0;
	((D23_ds_name *)(apdu_ptr-D23_S_NM))->d23_dslen = 0;
       }

  if (cpy == TRUE)
   {
     if ((req_ptr1=D27_MALLOC(svc_c_sev_warning,req_len)) == NULL)
      {
        pbhead->d2_retcode = D2_TEMP_ERR;
        pbhead->d2_errclass = D2_SYS_ERR;
        pbhead->d2_errvalue = D21_ALLOC_ERROR;
        ret_value = D2_ERROR;
        break;
        }
     memcpy(req_ptr1, req_ptr, offset);
     apdu_ptr = req_ptr1+offset;
     strcpy(apdu_ptr, r_target);
     ((D23_ds_name *)(apdu_ptr-D23_S_NM))->d23_dslen = strlen(r_target)+1;
     ((D23_ds_name *)(apdu_ptr-D23_S_NM))->d23_dsoff
						= D2_ALIGN(strlen(r_target)+1);
     offset += ((D23_ds_name *)(apdu_ptr-D23_S_NM))->d23_dsoff;
     for (i=old_offset; i < (old_req_len-1); i++,offset++)
                *(req_ptr1+offset) = *(req_ptr+i);
    }
				/*                                        */
				/*  navigate access points to get one bind*/
				/*                                        */
    for (i=0; i<r_ptr->d23_nm_acc; i++)
    {
     apdu_ptr = (char *)(r_ptr+1);
     apdu_ptr += ((D23_ds_name *)apdu_ptr)->d23_dsoff + D23_S_NM;
     rdsa_name = (D2_name_string)(apdu_ptr + D23_S_ACC + D23_S_NM);
     r_psap    = (D23_av_value *)(rdsa_name + 
                  ((D23_ds_name *)(rdsa_name-D23_S_NM))->d23_dsoff);
     psap.d2_a_v_len = r_psap->d23_avlen;
     psap.d2_a_rep   = D2_PSAP_STX;
     psap.d2_at_value.d2_oct_a_v = (Octet_string)(r_psap +1);
     apdu_ptr = (char *) r_psap + r_psap->d23_avlen + D23_S_ATV;

    /*
       Check for LoopBack
    */
    if (d21_cr3_check_lb(lb_info,lb_nr,(D2_name_string)r_target,
			(D2_op_progress *)&r_ptr->d23_op_pr, rdsa_name)
	  == TRUE)
    {   DUA_TRACE_LOOP_DETECTED
	pbhead->d2_retcode = D2_CALL_ERR;
	pbhead->d2_errclass = D2_PAR_ERR;
	pbhead->d2_errvalue = D2_LOOP_DETECTED;
	ret_value = D2_ERROR;
	break;
    }
    /*
      Try bind to DSA if not sucessful then read next access point(i.e. DSA)
    */
    if ((ret_value = d21_b03_ref_bind(pbhead, rdsa_name, &psap,
					  bt_ptr, &rbind_id)) == D2_NOERROR)
    {
      break;
     }
   } /* of for loop for acc_points */

   if ((i == r_ptr->d23_nm_acc) || (ret_value == D2_ERROR))
    {
      /* NO bind to any of the referral dsas */
      ret_value = D2_ERROR;
      *ref_ptr = (D23_rf_info *) apdu_ptr;
      break;
     }

				/*                                      */
				/* add BIND DSA in the LoopBack Table   */
				/*                                      */
    if ((ret_value = d21_cr1_add_lb(lb_info,lb_nr,(D2_name_string)r_target,
		 (D2_op_progress *)&r_ptr->d23_op_pr,rdsa_name)) != D2_NOERROR)
     {   
	 pbhead->d2_retcode = D2_TEMP_ERR;
	 pbhead->d2_errclass = D2_SYS_ERR;
	 pbhead->d2_errvalue = D21_ALLOC_ERROR;
	 ret_value = D2_ERROR;
	 break;
      }

   for (i=i+1; i<r_ptr->d23_nm_acc; i++)
    {
      rdsa_name = (D2_name_string)(apdu_ptr + D23_S_ACC + D23_S_NM);
      r_psap    = (D23_av_value *)(rdsa_name +
		((D23_ds_name *)(rdsa_name-D23_S_NM))->d23_dsoff);
      apdu_ptr = (char *) r_psap + r_psap->d23_avlen + D23_S_ATV;
     }
   *ref_ptr = (D23_rf_info *) apdu_ptr;

   /*
      enough time to start handling of the referral ?
   */
   curr_time = time(0L);
   if ((maxtime != D2_T_UNLIMITED) && (curr_time >= maxtime))
   {   /* set time_limit_exceeded and return */
       pbhead->d2_retcode = D2_TEMP_ERR;
       pbhead->d2_errclass = D2_REM_ERR;
       pbhead->d2_errvalue = D2_TIME_LIMIT_EXCEEDED;
       /*
	  unbind
       */
       unbd_parblk.d21_2pbhead = &u_pbhead;
       unbd_parblk.d21_2bind_id = rbind_id;
       d21_bh2_exec_unbind(&unbd_parblk);
       return(D2_ERROR);
   }

   /*
     set time-limit in read-request and send read-request to remote dsa
   */
   if (d21_bh8_get_bind_info(rbind_id,&r_b_tab,&r_new_ipc, pbhead,TRUE)
							 != D2_NOERROR)
     {
     return (D2_ERROR);
     }

   ((D23_header *)req_ptr1)->d23_bindid = 0;

   /* copy other elements of referral to common args */
   /* DAP Protocol dependent part */

   ((D23_com_arg *)(req_ptr1+D23_S_HDR))->d23_cmali
					= r_ptr->d23_rf_alirdn;

   if (maxtime != D2_T_UNLIMITED)
   { ((D23_com_arg *)(req_ptr1+D23_S_HDR))->d23_cmctr.d23_sctlm
					= maxtime - curr_time;
   }
   /*
      copy operation progress
   */
   ((D23_com_arg *)(req_ptr1+D23_S_HDR))->d23_cmopr
					= r_ptr->d23_op_pr;
   /*
      set ipc-request block
   */
   req_info.d21_r_opcode = op_id;
   req_info.d21_r_bufapdu = req_ptr1;
   req_info.d21_r_lenapdu = req_len;
   req_info.d21_r_ipcinfo = r_b_tab.d21_b_ipcinf;
   /*
      send request to DSA
   */
   if ((ret_value = d21_send_invoke(&req_info,r_b_tab.d21_b_usr_id,pbhead))
				    != D2_NOERROR)
   {   
       ret_value = D2_ERROR;
	/*
	   unbind
	 */
	unbd_parblk.d21_2pbhead = &u_pbhead;
	unbd_parblk.d21_2bind_id = rbind_id;
	d21_bh2_exec_unbind(&unbd_parblk);
	break;
     }
     pbhead->d2_errclass  =
     pbhead->d2_errvalue  =
     pbhead->d2_retcode  = D2_NO_ERR;
     /*
	get result from DSA in result area
     */
     req_info.d21_r_bufapdu = *res_ptr;
     req_info.d21_r_lenapdu = *res_len;

     ret_value = d21_recv_result(&req_info,r_b_tab.d21_b_usr_id,pbhead);

     *res_ptr = req_info.d21_r_bufapdu;
     *res_len = req_info.d21_r_lenapdu;

     /*
	 unbind
     */
     unbd_parblk.d21_2pbhead = &u_pbhead;
     unbd_parblk.d21_2bind_id = rbind_id;
     d21_ipc_clear(rbind_id,&r_new_ipc,&r_b_tab);
     d21_bh2_exec_unbind(&unbd_parblk);

     /*
      analyze result of read_request
     */
     if (ret_value != D2_NOERROR)
     {   /* error when reading from dsa */
	 ret_value = D2_ERROR;
	 break;
     }

     /*
	check for referral
     */
    if (pbhead->d2_errvalue != D2_NO_ERR)
     {
	r_ptr = (D23_rf_info *)(*res_ptr + D23_S_RSERR);
	if (pbhead->d2_errvalue != D2_REF_ERROR)
	 {   /* error-apdu other than referral */
	     ret_value = D2_ERROR;
	     break;
	  }
    }
    else    /* referral-apdu --> continue with next referral */
    {   ret_value = D2_NOERROR;
        /*
           unbind
        */
        break;
    }

  if (req_ptr != req_ptr1)
   {
      free(req_ptr1);
      req_ptr1 = req_ptr;
    }

} /* end of for D2_EVER loop */

if (req_ptr != req_ptr1)
  {
  free(req_ptr1);
  }

if (rbind_id != D2_NO_BIND) d21_ipc_clear(rbind_id,&r_new_ipc,&r_b_tab);
return(ret_value);

} /* end of d21_handle_ref() */


