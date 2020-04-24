/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21_C_ma.c,v $
 * Revision 1.1.10.2  1996/02/18  18:16:30  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:39:02  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:52:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:09:35  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:53:42  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:31:00  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  17:38:20  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:30:10  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  06:01:07  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  10:51:11  marrek]
 * 
 * Revision 1.1.4.4  1993/02/02  10:13:12  marrek
 * 	fixes for OT 6600
 * 	insert <pthread.h>
 * 	use const declarations
 * 	move to d21_l01_init()
 * 	use opmode D21_ALIAS_UPDATE
 * 	[1993/02/01  18:08:35  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  19:46:18  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:24:11  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  21:08:01  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  13:34:37  marrek]
 * 
 * Revision 1.1.2.3  1992/06/30  20:58:26  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  20:38:49  melman]
 * 
 * Revision 1.1.2.2  1992/06/02  03:12:34  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:36:16  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21_C_ma.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:16:30 $";
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
* TYPE         :  MODUL                                              *
*                                                                    *
* NAME         :  d21_C_modify_attribute                             *
*                                                                    *
* AUTHOR       :  W. Schmid                                          *
* DATE         :  05.11.90                                           *
*                                                                    *
* datoff *************************************************************
*/

#include <gds.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <d21dua.h>
#ifdef OSFCDS
#include <d21_cdssw.h>
#endif	/* OSFCDS */ 

/*
* exon ***************************************************************
*                                                                    *
* TYPE         :  C-FUNCTION                                         *
*                                                                    *
* NAME         :  d21_C_modify_attribute                             *
*                                                                    *
* AUTHOR       :  J.Forster,     Softlab GmbH.                       *
* DATE         :  29.2.88                                            *
*                                                                    *
* SYNTAX       :  d2_ret_val d21_C_modify_attribute(parblk)               *
*                   D21_pC *parblk;                                  *
*                                                                    *
*                                                                    *
* DESCRIPTION  :  allows modifications on attribute values           *
*                 within the Information Base of a DSA;              *
*                 supports following modes of modifications :        *
*                 . add                                              *
*                 . delete                                           *
*                 . replace                                          *
*                                                                    *
*                 Modifications may be done on Shadow- or Master-    *
*                 Entries. Modifications of Master-Entries may       *
*                 include modifications on "naming shadows".         *
*                                                                    *
* exoff **************************************************************
*/

d2_ret_val d21_C_modify_attribute(
  D21_pC * parblk)
{
d21_bind_info    b_tab, m_b_tab , s_b_tab ;
D2_pbhead               call_head;
d2_ret_val                   ret_value;
signed16                   ret_value1 = D2_NO_ERR;
char                    master_dsa[D2_DNL_MAX];
char                    s_master_dsa[D2_DNL_MAX];
signed16                   m_b_id , s_b_id ;
signed32                   req_len;
char                    *req_ptr = NULL;
signed32                   res_len;
char                    *res_ptr = NULL;
signed16                     opmode ;
const char function_name[] = "d21_C_ma";
char                    sup_name[D2_DNL_MAX];
D2_error               *err_info;
Bool                    alias_flag;
Bool                    dummy_flag;
Bool			adm_syntax = FALSE;
Bool new_ipc = FALSE;
Bool m_new_ipc = FALSE;
Bool s_new_ipc = FALSE;

	/* initializing logging and mutexes */
d21_L01_init();

DUA_TRACE_IAPL_ENTRY

/* log IAPL input */
d21_L04_trace_iapl_input(parblk->d21_Centry,parblk->d21_Cc_a);

/*
initialize return code
*/
parblk->d21_Cpbhead->d2_retcode = parblk->d21_Cpbhead->d2_errclass =
parblk->d21_Cpbhead->d2_errvalue = D2_NO_ERR;
ret_value = D2_NOERROR;

/*
... and some other variables
*/
call_head.d2_retcode = call_head.d2_errclass = call_head.d2_errvalue =
		       D2_NO_ERR;
call_head.d2_version = parblk->d21_Cpbhead->d2_version;
s_b_id = m_b_id = parblk->d21_Cbind_id;

parblk->d21_Cerr = NULL;

/*
check params
*/
if ( (ret_value = d21_m01_check_params(parblk)) != D2_NOERROR)
{   /* error in supplied params */
    DUA_TRACE_WRONG_IAPL_INPUT(parblk->d21_Cpbhead->d2_errvalue)
    return(D2_ERROR);
}

#ifdef OSFCDS
    /* 	Check to see if this naming operation should be 
	carried out in the CDS name space.  	
								*/
    if ( d21_cds_switch (parblk->d21_Centry) == DSW_NSPACE) {

	ret_value = d21_C_switch (parblk);
	return (ret_value);
    }
#endif /* OSFCDS */

  /* now  only a copy of the bind table entry is needed; it will
     be checked later if ipc connection of directory bind can be used or not */
if (d21_bh8_get_bind_info(parblk->d21_Cbind_id,&b_tab,&new_ipc,
				parblk->d21_Cpbhead,FALSE) != D2_NOERROR)
  {
  DUA_TRACE_IAPL_EXIT(parblk->d21_Cpbhead->d2_errvalue)
  return (D2_ERROR);
  }

if (b_tab.d21_b_syntax != D27_ASN1_TS)
  {
  adm_syntax = TRUE;
  }

if  ( !((parblk->d21_Cc_a->d2_serv_cntrls & D2_PREF_ADM_FUN) && adm_syntax) ) 
  {
  opmode = D21_STD_UPDATE;
  DUA_TRACE_OPMODE_STD_MODIFY
  }
else
  {
  /*
  chained modify ?
  */
  if ((parblk->d21_Cc_a->d2_serv_cntrls & D2_PREF_CHAINING)==D2_PREF_CHAINING)
    {   opmode = D21_CHAINED_UPDATE;
        DUA_TRACE_OPMODE_CHAINED_MODIFY
    }
  else
  {   if ( (parblk->d21_Cc_a->d2_serv_cntrls & D2_LOCALSCOPE) == D2_LOCALSCOPE)
      {   DUA_TRACE_OPMODE_SHADOW_MODIFY
      }
  
      /*
      does Bind-DSA hold master-copy or shadow ?
	      if BIND-DSA holds MASTER-Entry :
	      d21_m02 will return  D21_BOUND_TO_MASTER_DSA
      */
      ret_value1 = d21_m02_bind_to_master_dsa(&call_head,
			       parblk->d21_Cc_a->d2_serv_cntrls,
			       parblk->d21_Cbind_id,
			       b_tab.d21_b_name_dsa, parblk->d21_Centry,
			       &b_tab, (D2_name_string) NULL,
			       (D2_name_string)master_dsa, &m_b_id, &err_info,
			       &alias_flag);
  
      if (alias_flag == TRUE)
      {   /* set opmode */
	  opmode = D21_ALIAS_UPDATE ;
	  DUA_TRACE_OPMODE_ALIAS_MODIFY
      }
      else
      {   /*
	  modifiy SHADOW at BIND-DSA requested ?
	  */
	  if ((parblk->d21_Cc_a->d2_serv_cntrls & D2_LOCALSCOPE) ==
	      D2_LOCALSCOPE)
	  {   if (ret_value1 > D21_MASTER_DSA_OUT_OF_SCOPE)
	      {   /* map returncodes of d21_m02 */
		  d21_m04_map_rc(ret_value1, &call_head, parblk->d21_Cpbhead,
			         err_info);
		  DUA_TRACE_IAPL_EXIT(parblk->d21_Cpbhead->d2_errvalue)
		  parblk->d21_Cerr = err_info;
		  return(D2_ERROR);
	      }
  
	      if (ret_value1 == D21_BOUND_TO_MASTER_DSA)
	      {   
		  parblk->d21_Cpbhead->d2_retcode = D2_CALL_ERR;
		  parblk->d21_Cpbhead->d2_errclass = D2_PAR_ERR;
		  parblk->d21_Cpbhead->d2_errvalue = D2_NO_SHADOW;
		  DUA_TRACE_IAPL_EXIT(parblk->d21_Cpbhead->d2_errvalue)
		  return(D2_ERROR);
	      }
  
	      DUA_TRACE_OPMODE_SHADOW_MODIFY
	      /* set opmode */
	      opmode = D21_SHADOW_UPDATE ;
	  }
	  else
	  {   if (ret_value1 != D21_BOUND_TO_MASTER_DSA)
	      {   /* map returncodes of d21_m02 */
		  d21_m04_map_rc(ret_value1, &call_head, parblk->d21_Cpbhead,
			         err_info);
		  DUA_TRACE_IAPL_EXIT(parblk->d21_Cpbhead->d2_errvalue)
		  parblk->d21_Cerr = err_info;
		  return(D2_ERROR);
	      }
  
	      if (master_dsa[0] == D2_EOS)
	      {   DUA_TRACE_OPMODE_ONE_MODIFY
		  opmode = D21_ONE_UPDATE ;
	      }
	      else
	      {   /*
		  bind to master-dsa of superior
		  */
		  d21_c30_get_sup(parblk->d21_Centry, (D2_name_string)sup_name);
  
		  s_b_id = m_b_id;
		  ret_value1 = d21_m02_bind_to_master_dsa(&call_head,
					   parblk->d21_Cc_a->d2_serv_cntrls,
					   parblk->d21_Cbind_id,
					   b_tab.d21_b_name_dsa,
					   (D2_name_string)sup_name,
					&b_tab, (D2_name_string)master_dsa,
				   (D2_name_string)s_master_dsa, &s_b_id,
					&err_info, &dummy_flag);
  
		  if (ret_value1 == D21_BIND_TO_MASTER_DSA_FAILED)
		  {   
		      opmode = D21_ONE_UPDATE ;
		      s_b_id = parblk->d21_Cbind_id;
		  }
		  else
		  {   if (ret_value1 != D21_BOUND_TO_MASTER_DSA)
		      {   /* map returncodes of d21_m02 */
			  d21_m04_map_rc(ret_value1, &call_head,
				         parblk->d21_Cpbhead, err_info);
			  DUA_TRACE_IAPL_EXIT(parblk->d21_Cpbhead->d2_errvalue)
				  /* unbind from master-dsa of entry */
			  d21_m03_unbind(m_b_id, parblk->d21_Cbind_id) ;
			  parblk->d21_Cerr = err_info;
			  return(D2_ERROR);
		      }
  
		      if (s_b_id == m_b_id)
		      {   DUA_TRACE_OPMODE_ONE_MODIFY
			  opmode = D21_ONE_UPDATE ;
			  s_b_id = parblk->d21_Cbind_id;
		      }
		      else
		      {   DUA_TRACE_OPMODE_TWO_MODIFY
			  opmode = D21_DOUBLE_UPDATE ;
		      }
		  }
	      }
	  }
      }
    }
  }

if (d21_bh8_get_bind_info(parblk->d21_Cbind_id,&b_tab,&new_ipc,
				parblk->d21_Cpbhead,TRUE) != D2_NOERROR)
    {
    DUA_TRACE_IAPL_EXIT(parblk->d21_Cpbhead->d2_errvalue)
    return (D2_ERROR);
    }

/*
generate IPC request
*/
	/* calculate length of request-apdu */
if ((req_len = d21_ca_len_inmodify(parblk)) == D2_ERROR)
{   parblk->d21_Cpbhead->d2_retcode = D2_CALL_ERR;
    parblk->d21_Cpbhead->d2_errclass = D2_PAR_ERR;
    parblk->d21_Cpbhead->d2_errvalue = D21_BAD_ARGUMENT;
    d21_m03_unbind(m_b_id,parblk->d21_Cbind_id);
    if (opmode == D21_DOUBLE_UPDATE)
       d21_m03_unbind(s_b_id,parblk->d21_Cbind_id);
    
    DUA_TRACE_IAPL_EXIT(parblk->d21_Cpbhead->d2_errvalue)
    d21_ipc_clear(parblk->d21_Cbind_id,&new_ipc,&b_tab);
    return(D2_ERROR);
}

	/* allocate memory for request  ..  */
req_ptr = D27_MALLOC(svc_c_sev_warning,req_len);
	/*  ..  and for result   */
res_len = b_tab.d21_b_ipcinf->d21_i_lenms;
res_ptr = D27_MALLOC(svc_c_sev_warning,res_len);
if ((res_ptr == NULL) || (req_ptr == NULL))
{   parblk->d21_Cpbhead->d2_retcode = D2_TEMP_ERR;
    parblk->d21_Cpbhead->d2_errclass = D2_SYS_ERR;
    parblk->d21_Cpbhead->d2_errvalue = D21_ALLOC_ERROR;
    if (req_ptr != NULL)
	free(req_ptr);
    if (res_ptr != NULL)
	free(res_ptr);
    d21_m03_unbind(m_b_id,parblk->d21_Cbind_id);
    if (opmode == D21_DOUBLE_UPDATE)
    {   d21_m03_unbind(s_b_id,parblk->d21_Cbind_id);
    }
    DUA_TRACE_IAPL_EXIT(parblk->d21_Cpbhead->d2_errvalue)
    d21_ipc_clear(parblk->d21_Cbind_id,&new_ipc,&b_tab);
    return(D2_ERROR);
}

/*
build modify request
*/
if ((ret_value1 = d21_m06_build_req(parblk, req_ptr, &b_tab)) != D2_NOERROR)
{   parblk->d21_Cpbhead->d2_retcode  = D2_CALL_ERR;
    parblk->d21_Cpbhead->d2_errclass = D2_PAR_ERR;
    parblk->d21_Cpbhead->d2_errvalue = ret_value1;
    free(req_ptr);
    free(res_ptr);
    d21_m03_unbind(m_b_id,parblk->d21_Cbind_id);
    if (opmode == D21_DOUBLE_UPDATE)
    {   d21_m03_unbind(s_b_id,parblk->d21_Cbind_id);
    }
    DUA_TRACE_IAPL_EXIT(parblk->d21_Cpbhead->d2_errvalue)
    d21_ipc_clear(parblk->d21_Cbind_id,&new_ipc,&b_tab);
    return(D2_ERROR);
}

/*
perform the required updates
*/
if (opmode == D21_SHADOW_UPDATE || opmode == D21_CHAINED_UPDATE ||
    opmode == D21_ALIAS_UPDATE || opmode == D21_STD_UPDATE)
{   ret_value1 = d21_m07_exec(opmode,&call_head,req_ptr,req_len,D23_MODENTR,
			     &res_ptr,&res_len,&b_tab);
    if (ret_value1 != D2_NO_ERR)
    {   ret_value = D2_ERROR;
    }
}
else
{   
  if (m_b_id != parblk->d21_Cbind_id)
    {
    if (d21_bh8_get_bind_info(m_b_id,&m_b_tab,&m_new_ipc,
       				parblk->d21_Cpbhead,TRUE) != D2_NOERROR)
      {
      DUA_TRACE_IAPL_EXIT(parblk->d21_Cpbhead->d2_errvalue)
      d21_ipc_clear(parblk->d21_Cbind_id,&new_ipc,&b_tab);
      return (D2_ERROR);
      }
    }
  else
    {
    m_b_tab = b_tab;
    }
    ret_value1 = d21_m07_exec(opmode,&call_head,req_ptr,req_len,D23_MODENTR,
			     &res_ptr,&res_len,&m_b_tab);
    d21_m03_unbind(m_b_id,parblk->d21_Cbind_id);
    if (ret_value1 == D2_NO_ERR)
    {   
	if (opmode == D21_DOUBLE_UPDATE)
	{ 
	  if (s_b_id != parblk->d21_Cbind_id)
	    {
            if (d21_bh8_get_bind_info(s_b_id,&s_b_tab,&s_new_ipc,
       				parblk->d21_Cpbhead,TRUE) != D2_NOERROR)
             {
             DUA_TRACE_IAPL_EXIT(parblk->d21_Cpbhead->d2_errvalue)
	     d21_ipc_clear(parblk->d21_Cbind_id,&new_ipc,&b_tab);
	     if (m_b_id != parblk->d21_Cbind_id)
				d21_ipc_clear(m_b_id,&m_new_ipc,&m_b_tab);
             return (D2_ERROR);
             }
	    }
	  else
	    {
	    s_b_tab = b_tab;
	    }
	   ret_value1 = d21_m07_exec(opmode,&call_head,
					req_ptr,req_len,D23_MODENTR,
				     		&res_ptr,&res_len,&s_b_tab);
	    if (ret_value1 != D2_NO_ERR)
	    {   ret_value = D2_ERROR;
	    }
	    d21_m03_unbind(s_b_id,parblk->d21_Cbind_id);
	}
    }
    else
    {   ret_value = D2_ERROR;
    }
}

/*
set values in D21_Cpbhead
*/
parblk->d21_Cpbhead->d2_retcode  = call_head.d2_retcode;
parblk->d21_Cpbhead->d2_errclass = call_head.d2_errclass;
parblk->d21_Cpbhead->d2_errvalue = call_head.d2_errvalue;
/*
check for error APDU
*/
if (ret_value1 == D21_EPDU)
  {
  d21_m08_proc_epdu(parblk->d21_Cbind_id,
				 parblk->d21_Cc_a->d2_serv_cntrls,
				 parblk->d21_Cpbhead,
				 res_ptr, res_len,
				 &parblk->d21_Cerr);
  }

free(req_ptr);
free(res_ptr);

#ifdef OSFCDS_REFERRALS
    /* 	Check to see if the naming operation should be referred to 
	the CDS name space.  	
								*/
if (ret_value1 == D2_CALL_ERR)
	{
	if ( d21_cds_referral(parblk->d21_Centry, parblk->d21_Cerr)
			== DSW_REFERRED)
	  { 
	  ret_value = d21_C_switch (parblk);
	  }
	else
	  {
	  parblk->d21_Cpbhead->d2_retcode  = D2_CALL_ERR;
	  parblk->d21_Cpbhead->d2_errclass = D2_PAR_ERR;
	  parblk->d21_Cpbhead->d2_errvalue = D2_NO_BIND;
	  ret_value = D2_ERROR;
	  }
	} 
#endif 	/* OSFCDS_REFERRALS */

d21_ipc_clear(parblk->d21_Cbind_id,&new_ipc,&b_tab);
if (m_b_id != parblk->d21_Cbind_id) d21_ipc_clear(m_b_id,&m_new_ipc,&m_b_tab);
if (s_b_id != parblk->d21_Cbind_id) d21_ipc_clear(s_b_id,&s_new_ipc,&s_b_tab);

DUA_TRACE_IAPL_EXIT(parblk->d21_Cpbhead->d2_errvalue)

return(ret_value);
}
