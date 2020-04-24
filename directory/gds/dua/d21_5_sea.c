/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21_5_sea.c,v $
 * Revision 1.1.10.2  1996/02/18  18:16:15  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:38:48  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:52:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:09:24  root]
 * 
 * Revision 1.1.8.3  1994/05/10  15:53:36  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:30:27  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:14:17  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:27:36  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:38:07  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:29:02  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  05:54:01  marrek
 * 	time limit is exceeded also if max_time=curr_time.
 * 	[1993/08/03  09:29:56  marrek]
 * 
 * 	July 1993 code drop.
 * 	[1993/07/30  10:49:30  marrek]
 * 
 * Revision 1.1.4.4  1993/02/02  10:11:54  marrek
 * 	Insert <pthread.h>
 * 	fixes for OT 6554 and 6583
 * 	[1993/02/01  18:00:50  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  19:45:55  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:23:46  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  21:05:48  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  13:32:59  marrek]
 * 
 * Revision 1.1.2.3  1992/06/30  20:57:52  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  20:38:12  melman]
 * 
 * Revision 1.1.2.2  1992/06/02  03:06:39  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:35:45  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21_5_sea.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:16:15 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         :  MODUL                                              */
/*                                                                    */
/* NAME         :  d21_5_sea.c                                        */
/*                                                                    */
/* AUTHOR       :  J.Forster                                          */
/* DATE         :  1.2.88                                             */
/*                                                                    */
/* COMPONENT    :  Directory Service-V2 / xxxx                        */
/*                                                                    */
/* PRD#/VERS.   :                                                     */
/*                                                                    */
/* DESCRIPTION  :  Search operation                                   */
/*                                                                    */
/* D2_SYSTEM-DEPENDENCES:                                             */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |   Date   |   Updates                   | KZ | CR# FM#   */
/*                                                                    */
/*datoff **************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include <d21dua.h>
#ifdef OSFCDS
#include <d21_cdssw.h>
#endif  /* OSFCDS */

d2_ret_val d21_5_search(
  D21_p5        *parblk )
{
			  /* memory area for search request       */
char            *req_ptr;
signed32          req_len;  /* length of search request             */
			  /* memory area for search result        */
char            *res_ptr;
signed32          res_len;  /* length of search result              */
d2_ret_val           ret_value;
signed32           ret_value1;
char            *next_mem; /* location for next part of result    */
signed32            sc,maxentries;
time_t maxtime,curr_time;
signed16           entries_found = 0;
Bool            store_in_file;
signed16             opmode ;
int             out_file; /* file descriptor */
char            *link;
D23_rserror     *res_err_ptr;
D23_rf_info     *ref_ptr = NULL;
d21_bind_info  b_tab;  /* copy of bind table entry */
signed32 mem_len;
D23_rf_info *r_ptr = NULL;
Bool adm_syntax = FALSE;

D21_lb_info lb_info[D21_REF_MAX];
signed16 lb_nr=0;

const char function_name[] = "d21_5_sea";
Bool new_ipc = FALSE;

	       /* initialize logging and mutexes */
d21_L01_init();

DUA_TRACE_IAPL_ENTRY

/* log IAPL input */
d21_L04_trace_iapl_input(parblk->d21_5base_obj,parblk->d21_5c_a);

/*
initialize return code
*/
parblk->d21_5pbhead->d2_retcode = parblk->d21_5pbhead->d2_errclass =
parblk->d21_5pbhead->d2_errvalue = D2_NO_ERR;
ret_value = D2_NOERROR;
ret_value1 = D2_NOERROR;
/*
and variables
*/
next_mem = NULL;
parblk->d21_5res = NULL;
link = req_ptr = res_ptr = NULL;
store_in_file = FALSE;

/*
check params
*/
if (d21_sc01_check_params(parblk) != D2_NOERROR)
  {
  DUA_TRACE_WRONG_IAPL_INPUT(parblk->d21_5pbhead->d2_errvalue)
  return(D2_ERROR);
  }

#ifdef OSFCDS
    /*  Check to see if this naming operation should be
	carried out in the CDS name space.
								*/
    if ( d21_cds_switch (parblk->d21_5base_obj) == DSW_NSPACE) {
	/*      Since the search operation is not supported by CDS,
		return an unwillingToPerform service error.             */
	d21_cds_unwilling(parblk->d21_5pbhead);
	return (D2_ERROR);
    }
#endif /* OSFCDS */

sc = parblk->d21_5c_a->d2_serv_cntrls;
d21_cm01_serv_ctrl(sc, &opmode);

if (opmode != D21_CACHE_ONLY)
  {
  if (d21_bh8_get_bind_info(parblk->d21_5bind_id,&b_tab,&new_ipc,
				parblk->d21_5pbhead,TRUE) != D2_NOERROR)
    {
    DUA_TRACE_IAPL_EXIT(parblk->d21_5pbhead->d2_errvalue)
    return (D2_ERROR);
    }
	
  if (b_tab.d21_b_syntax != D27_ASN1_TS)
    {
    adm_syntax=TRUE && (parblk->d21_5c_a->d2_serv_cntrls & D2_PREF_ADM_FUN);
    }
  } /* if (not only cache) */

if (parblk->d21_5file != NULL)
{   /* store result to file */

    umask(000);
    if ( sc & D2_FILE_MODE )
    {   /* append mode */
	while ( ((out_file = open(parblk->d21_5file,
			O_RDWR | O_CREAT | O_APPEND , D21_F_CREA_MODE)) == -1)
		&& (errno == EINTR)) ;
    }
    else
    {   /* overwrite mode */
	while ( ((out_file = open(parblk->d21_5file,
			O_RDWR | O_CREAT | O_TRUNC , D21_F_CREA_MODE)) == -1)
		&& (errno == EINTR)) ;
    }
    if (out_file == -1)
    {   parblk->d21_5pbhead->d2_retcode = D2_TEMP_ERR;
	parblk->d21_5pbhead->d2_errclass = D2_SYS_ERR;
	parblk->d21_5pbhead->d2_errvalue = D2_NOT_SPEC;
	d21_ipc_clear(parblk->d21_5bind_id,&new_ipc,&b_tab);
	DUA_TRACE_IAPL_EXIT(parblk->d21_5pbhead->d2_errvalue)
	return(D2_ERROR);
    }
    else
    {   /* set store_in_file flag */
	store_in_file = TRUE;
    }
}

/*
calculate length of request-apdu
*/
req_len = d21_sc02_len_insearch (parblk) ;
if (req_len == D2_ERROR)
{
    parblk->d21_5pbhead->d2_retcode = D2_CALL_ERR;
    parblk->d21_5pbhead->d2_errclass = D2_PAR_ERR;
    parblk->d21_5pbhead->d2_errvalue = D21_BAD_ARGUMENT;
    if ( parblk->d21_5file != NULL)
    {   /* result written to file */
	while ((close(out_file) == -1) && (errno == EINTR)) ;
    }
    d21_ipc_clear(parblk->d21_5bind_id,&new_ipc,&b_tab);
    DUA_TRACE_IAPL_EXIT(parblk->d21_5pbhead->d2_errvalue)
    return(D2_ERROR) ;
}

/*
allocate buffer for apdu
*/
if ((req_ptr = D27_MALLOC(svc_c_sev_warning,req_len)) == NULL  )
{   parblk->d21_5pbhead->d2_retcode = D2_TEMP_ERR;
    parblk->d21_5pbhead->d2_errclass = D2_SYS_ERR;
    parblk->d21_5pbhead->d2_errvalue = D21_ALLOC_ERROR;
    if ( parblk->d21_5file != NULL)
    {   /* result written to file */
	while ((close(out_file) == -1) && (errno == EINTR)) ;
    }
    d21_ipc_clear(parblk->d21_5bind_id,&new_ipc,&b_tab);
    DUA_TRACE_IAPL_EXIT(parblk->d21_5pbhead->d2_errvalue)
    return(D2_ERROR);
}

/*
set time-limit
*/
if (parblk->d21_5c_a->d2_time_limit == D2_T_UNLIMITED)
{   maxtime = D2_T_UNLIMITED;
}
else
{   maxtime = time(0L) + parblk->d21_5c_a->d2_time_limit;
}
/*
and search-limit
*/
if (parblk->d21_5c_a->d2_size_limit == D2_T_UNLIMITED)
{   maxentries = D2_S_UNLIMITED;
}
else
{   maxentries = parblk->d21_5c_a->d2_size_limit;
}

parblk->d21_5res       = NULL;
parblk->d21_5rf        = NULL;
parblk->d21_5dist_name = NULL;
parblk->d21_5lim_prob  = D2_NO_LIMIT_EX;
parblk->d21_5uav_crit_ext = FALSE;
parblk->d21_5err       = NULL;
parblk->d21_5cres->d2_al_deref  = FALSE;

/* build request-apdu */

if (opmode != D21_CACHE_ONLY)
{
  ret_value = d21_sc03_build_req(parblk,req_ptr,b_tab.d21_b_dir_id, 0);
}
else
{
  ret_value = d21_sc03_build_req(parblk,req_ptr,parblk->d21_5bind_id, -1);
}

if (ret_value == D2_NOERROR)
{
  if                            /* referrals to handle are expected   */
    (!(sc & D2_LOCALSCOPE) && sc & D2_NOREFERRAL &&
     (opmode != D21_CACHE_ONLY) )
  {
				/*                                      */
				/* add BIND DSA in the LoopBack Table   */
				/*                                      */
    if (d21_cr1_add_lb(lb_info,&lb_nr,parblk->d21_5base_obj,
	&parblk->d21_5c_a->d2_op_progress,
	b_tab.d21_b_name_dsa) != D2_NOERROR)
    {
      parblk->d21_5pbhead->d2_retcode = D2_TEMP_ERR;
      parblk->d21_5pbhead->d2_errclass = D2_SYS_ERR;
      parblk->d21_5pbhead->d2_errvalue = D21_ALLOC_ERROR;
      d21_ipc_clear(parblk->d21_5bind_id,&new_ipc,&b_tab);
			       /* clear LoopBack table */
      d21_cr4_purge_lb_tab(lb_info,&lb_nr);
      return(D2_ERROR);
    }
  }

 /*
  select appropriate search routine
 */

 switch(opmode)
 {   case D21_CACHE_ONLY:
	    {   DUA_TRACE_OPMODE_CACHE_ONLY

	     if (store_in_file)
		{   /*
		    call d21_sc10 with from_cache_flag set
		    */
		    ret_value = d21_sc10_write_output_to_file(
				     parblk->d21_5pbhead,&b_tab,req_ptr,req_len,
				      &ref_ptr, &entries_found,
			      adm_syntax? D23_SEARCH+D2_ADM_OPID : D23_SEARCH,
					&parblk->d21_5lim_prob,
				      out_file,TRUE);
		    break;
		}

		/*
		result to memory
		*/
		ret_value = d21_sc04_cache_search(parblk,req_ptr,req_len,
						  &req_ptr,&req_len);
		if (ret_value != D2_NOERROR )
		    break;
		else
		 if (parblk->d21_5pbhead->d2_retcode == D2_NO_ERR &&
		     parblk->d21_5pbhead->d2_errclass == D2_NO_ERR &&
		     parblk->d21_5pbhead->d2_errvalue == D2_NO_ERR)
		  {   if (parblk->d21_5pbhead->d2_errvalue == D2_NO_ERR)
		      {   /* move result to user memory */
			  ret_value = d21_sc05_move_ei_info_to_mem(
					   parblk, req_ptr, req_len,
					   &next_mem,&ref_ptr,
					   &link, &entries_found);
		      }
		   }
		  else
		   {   /* no error-referral, "real error" */
		       d21_cac_copy_error(req_ptr, parblk->d21_5pbhead,
					   &parblk->d21_5err);
			ret_value = D2_ERROR;
		    }
		break;
	    }       /* end of CACHE_ONLY_SEARCH */

    case D21_DSA_ONLY:
	    {   DUA_TRACE_OPMODE_DSA_ONLY
	     if (store_in_file)
		{   /*
		    move result to file
		    */
		    ret_value = d21_sc10_write_output_to_file(
				  parblk->d21_5pbhead, &b_tab, req_ptr,
				  req_len, &ref_ptr, &entries_found,
			  adm_syntax? D23_SEARCH+D2_ADM_OPID : D23_SEARCH,
					&parblk->d21_5lim_prob,
				  out_file, FALSE);
		  if ((ret_value == D2_NOERROR) &&
		      (ref_ptr != NULL))
		    {   /*
			handle referrals in result
			*/
		       if (sc & D2_NOREFERRAL)
		       {   /* handle referral */
			 ret_value = d21_sc13_handle_file_ref(lb_info,&lb_nr,
					parblk->d21_5pbhead,
					req_ptr,req_len,maxtime,ref_ptr,
					&entries_found,
				adm_syntax? D23_SEARCH+D2_ADM_OPID : D23_SEARCH,
				       &parblk->d21_5lim_prob,&b_tab,out_file);
			}
		       else
			{   /* no error-referral, "real error" */

			   r_ptr = ref_ptr;
			   mem_len = d21_ca_len_ref_inf ((char **)&r_ptr);
			   r_ptr = ref_ptr;
			   if ((parblk->d21_5rf = D27_MALLOC(svc_c_sev_warning,
						         mem_len)) == NULL)
			   {   parblk->d21_5pbhead->d2_retcode = D2_TEMP_ERR;
			       parblk->d21_5pbhead->d2_errclass = D2_SYS_ERR;
			       parblk->d21_5pbhead->d2_errvalue = D21_ALLOC_ERROR;
    			       d21_ipc_clear(parblk->d21_5bind_id,&new_ipc,&b_tab);
       						/* clear LoopBack table */
    			       d21_cr4_purge_lb_tab(lb_info,&lb_nr);
			       return(D2_ERROR);
			    }
			   ret_value = d21_cac_copy_referral((char **)&r_ptr,
							parblk->d21_5rf);
			}
		     }
		    break;
		}

		/*
		result to memory
		*/
		/* allocate 1 ipc block for result */
		res_len = b_tab.d21_b_ipcinf->d21_i_lenms;
		res_ptr = D27_MALLOC(svc_c_sev_warning,res_len);
		if (res_ptr == NULL)
		{   parblk->d21_5pbhead->d2_retcode = D2_TEMP_ERR;
		    parblk->d21_5pbhead->d2_errclass = D2_SYS_ERR;
		    parblk->d21_5pbhead->d2_errvalue = D21_ALLOC_ERROR;
		    ret_value = D2_ERROR;
		    break;
		}
		ret_value1 = d21_sc06_dsa_only_search(adm_syntax,parblk,
						req_ptr, req_len, &res_ptr,
						     &res_len, &b_tab);
		if (ret_value1 != D2_NOERROR)
		{   /*
		       IPC error or memory error
		    */
		    ret_value = D2_ERROR;
		    break;
		}

		/*
		process result
		*/
		if (parblk->d21_5pbhead->d2_retcode == D2_NO_ERR &&
		    parblk->d21_5pbhead->d2_errclass == D2_NO_ERR &&
		    parblk->d21_5pbhead->d2_errvalue == D2_NO_ERR)
		{   /* DSA returned result */
		    ret_value = d21_sc05_move_ei_info_to_mem(
						 parblk, res_ptr, res_len,
						 &next_mem, &ref_ptr,
					         &link, &entries_found);
		    if ((ret_value == D2_NOERROR) && (ref_ptr != NULL))
		    {   
			/*
			handle referrals in result
			*/
			ret_value = d21_sc08_handle_ref(lb_info,&lb_nr,
						adm_syntax,parblk,
						req_ptr,req_len,&res_ptr,
						    &res_len,maxtime,
						    maxentries,&next_mem,
						    &ref_ptr,&link,
						    &entries_found,&b_tab);
		    }
		    break;
		}

		/*
		check for error-referral
		*/
	      if (parblk->d21_5pbhead->d2_errvalue == D2_REF_ERROR)
	       {   res_err_ptr = (D23_rserror *)res_ptr;
		    ref_ptr = (D23_rf_info *)(res_err_ptr + 1);
		    /*  DSA returned referral */
		    if (sc & D2_NOREFERRAL)
		    {   /* handle referral */
			ret_value = d21_sc08_handle_ref(lb_info,&lb_nr,
						adm_syntax,
					     parblk,req_ptr,req_len,
					     &res_ptr,&res_len,maxtime,
					     maxentries,&next_mem,
					     &ref_ptr,&link,
					     &entries_found,&b_tab);

			if (ret_value != D2_NOERROR)
			  {
		          d21_cac_copy_error(res_ptr,
					parblk->d21_5pbhead, &parblk->d21_5err);
		       	  ret_value = D2_ERROR;
			  }
		     }
		    else
		     {   /* no error-referral, "real error" */

		       d21_cac_copy_error(res_ptr, parblk->d21_5pbhead,
						 &parblk->d21_5err);
		       ret_value = D2_ERROR;
		     }
		 }
	      else
		{   /* no error-referral, "real error" */
		    d21_cac_copy_error(res_ptr, parblk->d21_5pbhead,
						 &parblk->d21_5err);
		    ret_value = D2_ERROR;
		 }
	    } /* end of dsa only search */
	    break;

    case D21_CACHE_FIRST:
	    {   DUA_TRACE_OPMODE_CACHE_FIRST

		/* allocate 1 ipc block for result */
		res_len = b_tab.d21_b_ipcinf->d21_i_lenms;
		res_ptr = D27_MALLOC(svc_c_sev_warning,res_len);
		if (res_ptr == NULL)
		{   parblk->d21_5pbhead->d2_retcode = D2_TEMP_ERR;
		    parblk->d21_5pbhead->d2_errclass = D2_SYS_ERR;
		    parblk->d21_5pbhead->d2_errvalue = D21_ALLOC_ERROR;
		    ret_value = D2_ERROR;
		    break;
		}

		if (store_in_file)
		{   /*
		    call d21_sc10 with "from_cache_flag" set
		    */
		    ret_value = d21_sc10_write_output_to_file(
				     parblk->d21_5pbhead, &b_tab, req_ptr,
				     req_len, &ref_ptr, &entries_found,
			     adm_syntax? D23_SEARCH+D2_ADM_OPID : D23_SEARCH,
					&parblk->d21_5lim_prob,
				     out_file, TRUE);
		}
		else
		{   ret_value = d21_sc04_cache_search(parblk,req_ptr,req_len,
						      &res_ptr,&res_len);

		    if (ret_value == D2_NOERROR)
		    {   /* move result to user memory */
			if (parblk->d21_5pbhead->d2_errvalue == D2_NO_ERR)
			{   ret_value = d21_sc05_move_ei_info_to_mem(
						  parblk, res_ptr, res_len,
						  &next_mem,&ref_ptr,
					          &link, &entries_found);
			}
			else
			{   /* no error-referral, "real error" */
		       d21_cac_copy_error(res_ptr, parblk->d21_5pbhead,
					       &parblk->d21_5err);
			    ret_value = D2_ERROR;
			}
		    }
		}

		if ( (ret_value == D2_NOERROR) && (entries_found != 0) )
		{   /* result from Cache --> return */
		    break;
		}

		/* no information from cache */
		curr_time = time(0L);
		if ((maxtime != D2_T_UNLIMITED) && (curr_time >= maxtime))
		{   /* set time_limit_exceeded and return */
		    parblk->d21_5pbhead->d2_retcode = D2_TEMP_ERR;
		    parblk->d21_5pbhead->d2_errclass = D2_REM_ERR;
		    parblk->d21_5pbhead->d2_errvalue = D2_TIME_LIMIT_EXCEEDED;
		    ret_value = D2_ERROR;
		    break;
		}

		/* time limit not exceeded : search in DSA */
		/* (reset error of Cache-search) */
		parblk->d21_5pbhead->d2_retcode =
		parblk->d21_5pbhead->d2_errclass =
		parblk->d21_5pbhead->d2_errvalue = D2_NO_ERR;
		if (store_in_file)
		{   /*
		    move result to file
		    */
		    ret_value = d21_sc10_write_output_to_file(
				parblk->d21_5pbhead, &b_tab, req_ptr, req_len,
				&ref_ptr,&entries_found,
			adm_syntax? D23_SEARCH+D2_ADM_OPID : D23_SEARCH,
					&parblk->d21_5lim_prob,
				out_file, FALSE);
		    if ((ret_value == D2_NOERROR) && (ref_ptr != NULL))
		    {   /*
			handle referrals in result
			*/
		       if (sc & D2_NOREFERRAL)
		       {   /* handle referral */
			 ret_value = d21_sc13_handle_file_ref(lb_info,&lb_nr,
					parblk->d21_5pbhead,
					req_ptr,req_len,maxtime,ref_ptr,
					&entries_found,
				adm_syntax? D23_SEARCH+D2_ADM_OPID : D23_SEARCH,
				       &parblk->d21_5lim_prob,&b_tab,out_file);
			}
		       else
			{   /* no error-referral, "real error" */

			   r_ptr = ref_ptr;
			   mem_len = d21_ca_len_ref_inf ((char **)&r_ptr);
			   r_ptr = ref_ptr;
			   if ((parblk->d21_5rf = D27_MALLOC(svc_c_sev_warning,
						              mem_len)) == NULL)
			   {   parblk->d21_5pbhead->d2_retcode = D2_TEMP_ERR;
			       parblk->d21_5pbhead->d2_errclass = D2_SYS_ERR;
			       parblk->d21_5pbhead->d2_errvalue = D21_ALLOC_ERROR;
    			       d21_ipc_clear(parblk->d21_5bind_id,&new_ipc,&b_tab);
       						/* clear LoopBack table */
    		 	       d21_cr4_purge_lb_tab(lb_info,&lb_nr);
			       return(D2_ERROR);
			    }
			   ret_value = d21_cac_copy_referral((char **)&r_ptr,
							parblk->d21_5rf);
			}
		    }
		    break;
		}

		/*
		result to memory
		*/
		ret_value1 = d21_sc06_dsa_only_search(adm_syntax,parblk, req_ptr,
						     req_len, &res_ptr,
						     &res_len, &b_tab);
		if (ret_value1 != D2_NOERROR)
		{   /*
		       IPC error or memory error
		    */
		    ret_value = D2_ERROR;
		    break;
		}

		/*
		process result
		*/
		if (parblk->d21_5pbhead->d2_retcode == D2_NO_ERR &&
		    parblk->d21_5pbhead->d2_errclass == D2_NO_ERR &&
		    parblk->d21_5pbhead->d2_errvalue == D2_NO_ERR)
		{   /* DSA returned result */
		    ret_value = d21_sc05_move_ei_info_to_mem(
					      parblk, res_ptr, res_len,
					      &next_mem,&ref_ptr,
					      &link, &entries_found);
		    if ((ret_value == D2_NOERROR)&&(ref_ptr != NULL))
		    {   /*
			handle referrals in result
			*/
			ret_value = d21_sc08_handle_ref(lb_info,&lb_nr,
						adm_syntax,parblk,
					req_ptr,req_len,&res_ptr,&res_len,
					     maxtime,maxentries,&next_mem,
					     &ref_ptr,&link,
					     &entries_found, &b_tab);
		    }
		    break;
		}

		/*
		check for error-referral
		*/
		if (parblk->d21_5pbhead->d2_errvalue == D2_REF_ERROR)
		{   res_err_ptr = (D23_rserror *)res_ptr;
		    ref_ptr = (D23_rf_info *)(res_err_ptr + 1);
		    if (sc & D2_NOREFERRAL)
		    {   /* handle referral */
			ret_value = d21_sc08_handle_ref(lb_info,&lb_nr,
						adm_syntax,
					     parblk,req_ptr,req_len,
					     &res_ptr,&res_len,maxtime,
					     maxentries,&next_mem,
					     &ref_ptr,&link,
					     &entries_found,&b_tab);
			if (ret_value != D2_NOERROR)
			  {
		          d21_cac_copy_error(res_ptr,
					parblk->d21_5pbhead, &parblk->d21_5err);
		       	  ret_value = D2_ERROR;
			  }
		    }
		    else
		    {   /* no error-referral, "real error" */
			d21_cac_copy_error(res_ptr,
			  parblk->d21_5pbhead, &parblk->d21_5err);
			ret_value = D2_ERROR;
		    }
		 }
	      else
		{   /* no error-referral, "real error" */
		    d21_cac_copy_error(res_ptr, parblk->d21_5pbhead,
						 &parblk->d21_5err);
		    ret_value = D2_ERROR;
		 }
		break;
	    } /* end of cache first search */

    case D21_DSA_FIRST:
	    {   DUA_TRACE_OPMODE_DSA_FIRST

		/* allocate 1 ipc block for result */
		res_len = b_tab.d21_b_ipcinf->d21_i_lenms;
		res_ptr = D27_MALLOC(svc_c_sev_warning,res_len);
		if (res_ptr == NULL)
		{   parblk->d21_5pbhead->d2_retcode = D2_TEMP_ERR;
		    parblk->d21_5pbhead->d2_errclass = D2_SYS_ERR;
		    parblk->d21_5pbhead->d2_errvalue = D21_ALLOC_ERROR;
		    ret_value = D2_ERROR;
		    break;
		}

		if (store_in_file)
		{   /*
		    move result to file
		    */
		    ret_value = d21_sc10_write_output_to_file(
				     parblk->d21_5pbhead,&b_tab,req_ptr,req_len,
				      &ref_ptr, &entries_found,
			      adm_syntax? D23_SEARCH+D2_ADM_OPID : D23_SEARCH,
					&parblk->d21_5lim_prob,
				      out_file, FALSE);
		    if ((ret_value == D2_NOERROR) && (ref_ptr != NULL))
		    {   /*
			handle referrals in result
			*/
		       if (sc & D2_NOREFERRAL)
		       {   /* handle referral */
			 ret_value = d21_sc13_handle_file_ref(lb_info,&lb_nr,
					parblk->d21_5pbhead,
					req_ptr,req_len,maxtime,ref_ptr,
					&entries_found,
				adm_syntax? D23_SEARCH+D2_ADM_OPID : D23_SEARCH,
					&parblk->d21_5lim_prob,&b_tab,out_file);
			}
		       else
			{   /* no error-referral, "real error" */

			   r_ptr = ref_ptr;
			   mem_len = d21_ca_len_ref_inf ((char **)&r_ptr);
			   r_ptr = ref_ptr;
			   if ((parblk->d21_5rf = D27_MALLOC(svc_c_sev_warning,
						              mem_len)) == NULL)
			   {   parblk->d21_5pbhead->d2_retcode = D2_TEMP_ERR;
			       parblk->d21_5pbhead->d2_errclass = D2_SYS_ERR;
			       parblk->d21_5pbhead->d2_errvalue = D21_ALLOC_ERROR;
    			       d21_ipc_clear(parblk->d21_5bind_id,&new_ipc,&b_tab);
       						/* clear LoopBack table */
    		 	       d21_cr4_purge_lb_tab(lb_info,&lb_nr);
			       return(D2_ERROR);
			    }
			   ret_value = d21_cac_copy_referral((char **)&r_ptr,
							parblk->d21_5rf);
			}
		    }
		}
		else
		{   /*
		    result to memory
		    */
		    ret_value1 = d21_sc06_dsa_only_search(adm_syntax,parblk,
						req_ptr, req_len, &res_ptr,
							 &res_len, &b_tab);
		    if (ret_value1 != D2_NOERROR)
		    {   /*
			   IPC error or memory error
			*/
			ret_value = D2_ERROR;
			break;
		    }

		    if (parblk->d21_5pbhead->d2_retcode == D2_NO_ERR &&
			parblk->d21_5pbhead->d2_errclass == D2_NO_ERR &&
			parblk->d21_5pbhead->d2_errvalue == D2_NO_ERR)
		    {   /* DSA returned result */
			ret_value = d21_sc05_move_ei_info_to_mem(
						  parblk, res_ptr, res_len,
						  &next_mem,&ref_ptr,
					          &link, &entries_found);
		    }

		    /*
		    check for error-referral
		    */
		    if (parblk->d21_5pbhead->d2_errvalue == D2_REF_ERROR)
		    {   res_err_ptr = (D23_rserror *)res_ptr;
			ref_ptr = (D23_rf_info *)(res_err_ptr + 1);
			if (sc & D2_NOREFERRAL)
			{
			 if (store_in_file)
			 {
			  if (sc & D2_NOREFERRAL)
			   {   /* handle referral */
			    ret_value = d21_sc13_handle_file_ref(lb_info,&lb_nr,
					parblk->d21_5pbhead,
					  req_ptr,req_len,maxtime,ref_ptr,
					  &entries_found,
				adm_syntax? D23_SEARCH+D2_ADM_OPID : D23_SEARCH,
					&parblk->d21_5lim_prob,&b_tab,out_file);
			    }
			  else
			   {   /* no error-referral, "real error" */
			   r_ptr = ref_ptr;
			   mem_len = d21_ca_len_ref_inf ((char **)&r_ptr);
			   r_ptr = ref_ptr;
			   if ((parblk->d21_5rf = D27_MALLOC(svc_c_sev_warning,
						              mem_len)) == NULL)
			   {   parblk->d21_5pbhead->d2_retcode = D2_TEMP_ERR;
			       parblk->d21_5pbhead->d2_errclass = D2_SYS_ERR;
			       parblk->d21_5pbhead->d2_errvalue = D21_ALLOC_ERROR;
    			       d21_ipc_clear(parblk->d21_5bind_id,&new_ipc,&b_tab);
       						/* clear LoopBack table */
    		 	       d21_cr4_purge_lb_tab(lb_info,&lb_nr);
			       return(D2_ERROR);
			    }
			   ret_value = d21_cac_copy_referral((char **)&r_ptr,
							parblk->d21_5rf);
			    }
			 }
			 else
			 {   /* handle referral */
			    ret_value = d21_sc08_handle_ref(lb_info,&lb_nr,
						adm_syntax,
						 parblk,req_ptr,req_len,
						 &res_ptr,&res_len,maxtime,
						 maxentries,&next_mem,
						 &ref_ptr,&link,
						 &entries_found,&b_tab);
			 }
			/* IGNORE Errors returned by DSA   */
		       }
		      }
		}

		/* result from DSA ? */
		if ( (ret_value != D2_NOERROR) || (entries_found == 0) )
		{   /* no result from DSA -> try cache */
		    /* (reset error of DSA-search) */
		    parblk->d21_5pbhead->d2_retcode =
		    parblk->d21_5pbhead->d2_errclass =
		    parblk->d21_5pbhead->d2_errvalue = D2_NO_ERR;
		    if (store_in_file)
		    {   /*
			call d21_sc10 with "from_cache_flag" set
			*/
			ret_value = d21_sc10_write_output_to_file(
				  parblk->d21_5pbhead, &b_tab, req_ptr, req_len,
					    &ref_ptr, &entries_found,
			    adm_syntax? D23_SEARCH+D2_ADM_OPID : D23_SEARCH,
					&parblk->d21_5lim_prob,
					    out_file, TRUE);
		    }
		    else
		    {   ret_value = d21_sc04_cache_search(parblk,
						   req_ptr,req_len,&res_ptr,
						   &res_len);
			if (ret_value == D2_NOERROR)
			{   /* move result to user memory */
			    ret_value = d21_sc05_move_ei_info_to_mem(
						 parblk, res_ptr, res_len,
						 &next_mem,&ref_ptr,
					         &link, &entries_found);
			}
		    }
		}

	} /* end of dsa first search */
	break;

    default:
	{   DUA_TRACE_UNKNOWN_OPMODE
	    parblk->d21_5pbhead->d2_retcode = D2_PERM_ERR;
	    parblk->d21_5pbhead->d2_errclass = D2_SYS_ERR;
	    parblk->d21_5pbhead->d2_errvalue = D2_NOT_SPEC;
	    ret_value = D2_ERROR;
	}
 } /* end of switch(opmode) */

} /* of if */

if (req_ptr != NULL)
{   free(req_ptr);
}
if (res_ptr != NULL)
{   free(res_ptr);
}

#ifdef OSFCDS_REFERRALS 
    /*  Check to see if the naming operation should be referred
	to the CDS name space.
								*/
    if (ret_value == D2_CALL_ERR)
	{
	if ( d21_cds_referral(parblk->d21_5base_obj, parblk->d21_5err)
	    == DSW_REFERRED)
	  {
	  d21_cds_unwilling(parblk->d21_5pbhead);
	  ret_value = D2_ERROR;
	  }
	else
	  {
	  parblk->d21_5pbhead->d2_retcode = D2_PERM_ERR;
	  parblk->d21_5pbhead->d2_errclass = D2_SYS_ERR;
	  parblk->d21_5pbhead->d2_errvalue = D2_NOT_SPEC;
	  ret_value = D2_ERROR;
	  }
	}
#endif  /* OSFCDS_REFERRALS*/

if ( parblk->d21_5file != NULL)
{   /* result written to file */
    while ((close(out_file) == -1) && (errno == EINTR));
}

d21_ipc_clear(parblk->d21_5bind_id,&new_ipc,&b_tab);
       					/* clear LoopBack table */
d21_cr4_purge_lb_tab(lb_info,&lb_nr);

if (ret_value1 == D21_INTERRUPTED)
  {
  DUA_TRACE_INTERRUPTED

#ifdef THREADSAFE
  RAISE(pthread_cancel_e);
#endif
  }

DUA_TRACE_IAPL_EXIT(parblk->d21_5pbhead->d2_errvalue)

return(ret_value);

} /* end of d21_5_search() */
