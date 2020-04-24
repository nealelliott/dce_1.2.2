/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21_6_comp.c,v $
 * Revision 1.1.10.2  1996/02/18  18:16:18  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:38:51  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:52:36  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:09:27  root]
 * 
 * Revision 1.1.8.3  1994/05/10  15:53:37  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:30:34  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:14:19  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:27:40  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:38:10  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:29:16  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  05:55:38  marrek
 * 	time limit is exceeded also if max_time=curr_time.
 * 	[1993/08/03  09:30:17  marrek]
 * 
 * 	July 1993 code drop.
 * 	[1993/07/30  10:49:49  marrek]
 * 
 * Revision 1.1.4.4  1993/02/02  10:12:11  marrek
 * 	Insert <pthread.h>
 * 	fixes for OT 6554
 * 	[1993/02/01  18:02:01  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  19:46:00  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:23:51  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  21:06:19  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  13:33:20  marrek]
 * 
 * Revision 1.1.2.3  1992/06/30  20:57:59  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  20:38:19  melman]
 * 
 * Revision 1.1.2.2  1992/06/02  03:08:06  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:35:51  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21_6_comp.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:16:18 $";
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
* NAME         :  d21_6_compare.c                                    *
*                                                                    *
* AUTHOR       :  J.Forster                                          *
* DATE         :  1.2.88                                             *
*                                                                    *
* DESCRIPTION  :  Compare function                                   *
*                                                                    *
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
#endif

/*
* exon ***************************************************************
*                                                                    *
* TYPE         :  C-FUNCTION                                         *
*                                                                    *
* NAME         :  d21_6_compare                                      *
*                                                                    *
* AUTHOR       :  J.Forster,     Softlab GmbH.                       *
* DATE         :  1.2.88                                             *
*                                                                    *
* exoff **************************************************************
*/

d2_ret_val d21_6_compare (
  D21_p6 *parblk)
{
char                  *req_ptr = NULL ; /* memory area for compare request */
signed32		      req_len  ; /* length of compare request             */
char                  *res_ptr = NULL ; /* area for result     */
signed32		      res_len  ; /* length of result    */
d2_ret_val                 ret_value;
signed32                 ret_value1;
signed16                   opmode;
signed32                  sc;
time_t maxtime , curr_time ;
D23_rf_info           *ref_ptr ;
d21_bind_info  b_tab;  /* copy of bind table entry */

D21_lb_info lb_info[D21_REF_MAX];
signed16 lb_nr=0;

const char function_name[] = "d21_6_compare";

Bool new_ipc = FALSE;

/* store the time - used for time limit checks */
curr_time = time(0L);

/* initializing logging and mutexes */
d21_L01_init();

DUA_TRACE_IAPL_ENTRY

/* log IAPL input */
d21_L04_trace_iapl_input(parblk->d21_6entry,parblk->d21_6c_a);

/* initialize */
parblk->d21_6pbhead->d2_retcode  = parblk->d21_6pbhead->d2_errclass =
parblk->d21_6pbhead->d2_errvalue = D2_NO_ERR;
ret_value = D2_NOERROR;
parblk->d21_6match               = FALSE ;
parblk->d21_6dist_name           = NULL;

/* ... pointer to result and referral ... */
parblk->d21_6err                 = NULL;
parblk->d21_6cres->d2_al_deref   = FALSE;

/* ... and some other variables */
req_ptr = res_ptr = NULL;

/* check params */
if (d21_cp01_check_params(parblk) != D2_NOERROR)
  {
  DUA_TRACE_WRONG_IAPL_INPUT(parblk->d21_6pbhead->d2_errvalue)
  return(D2_ERROR);
  }

#ifdef OSFCDS
    /*  Check to see if this naming operation should be
	carried out in the CDS name space.
								*/
    if ( d21_cds_switch (parblk->d21_6entry) == DSW_NSPACE) {
	ret_value = d21_6_switch (parblk);
	return (ret_value);
    }
#endif /* OSFCDS */

sc = parblk->d21_6c_a->d2_serv_cntrls;
d21_cm01_serv_ctrl(sc, &opmode);

if (opmode != D21_CACHE_ONLY)
  {
  if (d21_bh8_get_bind_info(parblk->d21_6bind_id,&b_tab,&new_ipc,
				parblk->d21_6pbhead,TRUE) != D2_NOERROR)
    {
    DUA_TRACE_IAPL_EXIT(parblk->d21_6pbhead->d2_errvalue)
    return (D2_ERROR);
    }
  } /* if (not only cache) */
  
/* get length of invoke-apdu */
if ((req_len = d21_ca_len_incompare(parblk)) == D2_ERROR)
{   parblk->d21_6pbhead->d2_retcode = D2_CALL_ERR;
    parblk->d21_6pbhead->d2_errclass = D2_PAR_ERR;
    parblk->d21_6pbhead->d2_errvalue = D21_BAD_ARGUMENT;
    d21_ipc_clear(parblk->d21_6bind_id,&new_ipc,&b_tab);
    DUA_TRACE_IAPL_EXIT(parblk->d21_6pbhead->d2_errvalue)
    return(D2_ERROR);
}

/* and memory-space */
if ((req_ptr = D27_MALLOC(svc_c_sev_warning,req_len)) == NULL  )
{   parblk->d21_6pbhead->d2_retcode = D2_TEMP_ERR;
    parblk->d21_6pbhead->d2_errclass = D2_SYS_ERR;
    parblk->d21_6pbhead->d2_errvalue = D21_ALLOC_ERROR;
    d21_ipc_clear(parblk->d21_6bind_id,&new_ipc,&b_tab);
    return(D2_ERROR);
}

/* set time_limit */
if (parblk->d21_6c_a->d2_time_limit ==  D2_T_UNLIMITED)
{   maxtime = D2_T_UNLIMITED;
}
else
{   maxtime = time(0L) + parblk->d21_6c_a->d2_time_limit;
}

/* build request-apdu */

if (opmode != D21_CACHE_ONLY)
{
  ret_value = d21_cp03_build_req(parblk,req_ptr,b_tab.d21_b_dir_id, 0);
}
else
{
  ret_value = d21_cp03_build_req(parblk,req_ptr,parblk->d21_6bind_id, -1);
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
    if (d21_cr1_add_lb(lb_info,&lb_nr,parblk->d21_6entry,
    			&parblk->d21_6c_a->d2_op_progress,
    b_tab.d21_b_name_dsa) != D2_NOERROR)
    {
	parblk->d21_6pbhead->d2_retcode = D2_TEMP_ERR;
	parblk->d21_6pbhead->d2_errclass = D2_SYS_ERR;
	parblk->d21_6pbhead->d2_errvalue = D21_ALLOC_ERROR;
    	d21_ipc_clear(parblk->d21_6bind_id,&new_ipc,&b_tab);
			       /* clear LoopBack table */
	d21_cr4_purge_lb_tab(lb_info,&lb_nr);
	return(D2_ERROR);
    }
  }

      switch(opmode)
      {   case D21_CACHE_ONLY:
		   {   
		      DUA_TRACE_OPMODE_CACHE_ONLY

		         /* execute compare */
			  ret_value = d21_cp04_cache_compare(parblk,req_ptr,
						     req_len,&req_ptr,&req_len);

			  if (ret_value != D2_NOERROR)
			    break;
			  else
			    if (parblk->d21_6pbhead->d2_retcode == D2_NO_ERR &&
				parblk->d21_6pbhead->d2_errclass == D2_NO_ERR &&
				parblk->d21_6pbhead->d2_errvalue == D2_NO_ERR)
			     {   /* set result */
				 ret_value = d21_cp05_set_result (parblk, req_ptr);
			     }
			     else
			     {   /* "real error" */
				 d21_cac_copy_error(req_ptr,
						    parblk->d21_6pbhead, &parblk->d21_6err);
				 ret_value = D2_ERROR;
			     }
		  }  /* end of CACHE_ONLY */
		  break;

	  case D21_DSA_ONLY:
		  {   DUA_TRACE_OPMODE_DSA_ONLY

		     /* allocate 1 ipc block for result */
		     res_len = b_tab.d21_b_ipcinf->d21_i_lenms;
		     if ((res_ptr = D27_MALLOC(svc_c_sev_warning,res_len))
									== NULL)
		      {   ret_value1 = D21_ALLOC_ERROR;
			  break;
		      }

		      /*
		      exec DSA ONLY COMPARE
		      */
		      ret_value1 = d21_cp06_dsa_compare(parblk,req_ptr,req_len,
						       &res_ptr, &res_len,&b_tab);
		      if (ret_value1 != D2_NOERROR)
		      {   /*
			  error when comparing in DSA :
			  DSA did not return result, error or referral
			  */
			  ret_value = D2_ERROR;
			  break;
		      }

		      /*
		      DSA returned result, error or referral
		      */
		      if (parblk->d21_6pbhead->d2_retcode == D2_NO_ERR &&
			  parblk->d21_6pbhead->d2_errclass == D2_NO_ERR &&
			  parblk->d21_6pbhead->d2_errvalue == D2_NO_ERR)
		      {   /*
			  DSA returned result : set result
			  */
			  ret_value = d21_cp05_set_result (parblk, res_ptr);
			  break;
		      } /* end of DSA returned result */

		      /* check for referral */
		      ref_ptr = (D23_rf_info *)(res_ptr + sizeof(D23_rserror));

		      if ((ref_ptr->d23_rftag == D23_REFTAG)
			  && (sc & D2_NOREFERRAL))
		      {      /* handle referral */
			     parblk->d21_6pbhead->d2_retcode =
			     parblk->d21_6pbhead->d2_errclass =
			     parblk->d21_6pbhead->d2_errvalue = D2_NO_ERR;
			     ret_value = d21_handle_ref(lb_info,&lb_nr,
					&ref_ptr, parblk->d21_6pbhead,
				    maxtime, parblk->d21_6bind_id,D23_COMPARE,
			        &res_ptr, &res_len, req_ptr, req_len,&b_tab);

			     if (parblk->d21_6pbhead->d2_retcode == D2_NO_ERR &&
				 parblk->d21_6pbhead->d2_errclass == D2_NO_ERR &&
				 parblk->d21_6pbhead->d2_errvalue == D2_NO_ERR)
			     {      /* DSA returned result : move result */
				    ret_value = d21_cp05_set_result (parblk, res_ptr);
			     } /* end of DSA returned read-result */
			     else
			     {      /* no error-referral, "real error" */
				    d21_cac_copy_error(res_ptr,
						parblk->d21_6pbhead, &parblk->d21_6err);
				    ret_value = D2_ERROR;
			     }
		      }
		      else
		      {      /* no error-referral, "real error" */
			     d21_cac_copy_error(res_ptr, parblk->d21_6pbhead,
							      &parblk->d21_6err);
			     ret_value = D2_ERROR;
		      }
		  } /* end DSA_ONLY_COMPARE */
		  break;

	  case D21_CACHE_FIRST:
		  {   DUA_TRACE_OPMODE_CACHE_FIRST

		      /* allocate 1 ipc block for result */
		      res_len = b_tab.d21_b_ipcinf->d21_i_lenms;
		      res_ptr = D27_MALLOC(svc_c_sev_warning,res_len);
		      if (res_ptr == NULL)
		      {   ret_value1 = D21_ALLOC_ERROR;
			  break;
		      }

		      /* compare in DUACACHE first */
		      ret_value = d21_cp04_cache_compare(parblk,req_ptr,req_len,
							 &res_ptr,&res_len);
		      if (ret_value == D2_NOERROR)
		        {
			if (parblk->d21_6pbhead->d2_retcode == D2_NO_ERR &&
				parblk->d21_6pbhead->d2_errclass == D2_NO_ERR &&
				parblk->d21_6pbhead->d2_errvalue == D2_NO_ERR)
			  { /* set result */
			  ret_value = d21_cp05_set_result (parblk, res_ptr);
			  break;
			  }
		        }

		      /*
		      error reported from cache :
		      try to compare in DSA, if timelimit not yet exceeded
		      */
		      curr_time = time(0L);
		      if ((maxtime != D2_T_UNLIMITED) && (curr_time >= maxtime))
		      {   /* set time_limit_exceeded and return */
			  parblk->d21_6pbhead->d2_retcode = D2_TEMP_ERR;
			  parblk->d21_6pbhead->d2_errclass = D2_REM_ERR;
			  parblk->d21_6pbhead->d2_errvalue = D2_TIME_LIMIT_EXCEEDED;
			  ret_value = D2_ERROR;
			  break;
		      }
		      /*
		      timelimit not exceeded
		      (reset error from CACHE-compare)
		      */
		      parblk->d21_6pbhead->d2_retcode =
		      parblk->d21_6pbhead->d2_errclass =
		      parblk->d21_6pbhead->d2_errvalue = D2_NO_ERR;
		      ret_value1 = d21_cp06_dsa_compare(parblk,req_ptr,req_len,
						       &res_ptr, &res_len,&b_tab);
		      if (ret_value1 != D2_NOERROR)
		      {   /* error when comparing in DSA */
			  ret_value = D2_ERROR;
			  break;
		      }
		      if (parblk->d21_6pbhead->d2_retcode == D2_NO_ERR &&
			  parblk->d21_6pbhead->d2_errclass == D2_NO_ERR &&
			  parblk->d21_6pbhead->d2_errvalue == D2_NO_ERR)
		      {   /* DSA returned result */
			  ret_value = d21_cp05_set_result (parblk, res_ptr);
			  break;
		      }

		      /* check for referral */
		      ref_ptr = (D23_rf_info *)(res_ptr + sizeof(D23_rserror));

		      if ((ref_ptr->d23_rftag == D23_REFTAG)
			  && (sc & D2_NOREFERRAL))
		      {      /* handle referral */
			     parblk->d21_6pbhead->d2_retcode =
			     parblk->d21_6pbhead->d2_errclass =
			     parblk->d21_6pbhead->d2_errvalue = D2_NO_ERR;
			     ret_value = d21_handle_ref(lb_info,&lb_nr,
					&ref_ptr, parblk->d21_6pbhead,
				    maxtime, parblk->d21_6bind_id,D23_COMPARE,
				 &res_ptr, &res_len, req_ptr, req_len,&b_tab);

			     if (parblk->d21_6pbhead->d2_retcode == D2_NO_ERR &&
				 parblk->d21_6pbhead->d2_errclass == D2_NO_ERR &&
				 parblk->d21_6pbhead->d2_errvalue == D2_NO_ERR)
			     {      /* DSA returned result : move result */
				    ret_value = d21_cp05_set_result (parblk, res_ptr);
			     } /* end of DSA returned read-result */
			     else
			     {      /* no error-referral, "real error" */
				 d21_cac_copy_error(res_ptr,
					parblk->d21_6pbhead, &parblk->d21_6err);
				 ret_value = D2_ERROR;
			     }
		      }
		      else
		      {      /* no error-referral, "real error" */
			     d21_cac_copy_error(res_ptr, parblk->d21_6pbhead,
							      &parblk->d21_6err);
			     ret_value = D2_ERROR;
		      }
		  } /* end of D21_CACHE_FIRST_COMP */
		  break;

	  case D21_DSA_FIRST:
		  {   DUA_TRACE_OPMODE_DSA_FIRST
		      /* compare in DSA */

		      /* allocate 1 ipc block for result */
		      res_len = b_tab.d21_b_ipcinf->d21_i_lenms;
		      res_ptr = D27_MALLOC(svc_c_sev_warning,res_len);
		      if (res_ptr == NULL)
		      {   ret_value1 = D21_ALLOC_ERROR;
			  break;
		      }
		      ret_value1 = d21_cp06_dsa_compare(parblk,req_ptr,req_len,
						       &res_ptr, &res_len,&b_tab);
		      if (ret_value1 != D2_NOERROR)
		      {   /*
			     IPC error or memory error
			  */
			  ret_value = D2_ERROR;
			  break;
		      }

		      if (parblk->d21_6pbhead->d2_retcode == D2_NO_ERR &&
			  parblk->d21_6pbhead->d2_errclass == D2_NO_ERR &&
			  parblk->d21_6pbhead->d2_errvalue == D2_NO_ERR)
		      {   /* DSA returned result */
			  ret_value = d21_cp05_set_result (parblk, res_ptr);
			  break;
		      }

		      /* check for referral */
		      ref_ptr = (D23_rf_info *)(res_ptr + sizeof(D23_rserror));

		      if ((ref_ptr->d23_rftag == D23_REFTAG)
			  &&  (sc & D2_NOREFERRAL))
		      {     /* handle referral */
			    parblk->d21_6pbhead->d2_retcode =
			    parblk->d21_6pbhead->d2_errclass =
			    parblk->d21_6pbhead->d2_errvalue = D2_NO_ERR;
			    ret_value = d21_handle_ref(lb_info,&lb_nr,
					&ref_ptr, parblk->d21_6pbhead,
				   maxtime, parblk->d21_6bind_id,D23_COMPARE,
				   &res_ptr, &res_len, req_ptr, req_len,&b_tab);
			    if (parblk->d21_6pbhead->d2_retcode == D2_NO_ERR &&
				parblk->d21_6pbhead->d2_errclass == D2_NO_ERR &&
				parblk->d21_6pbhead->d2_errvalue == D2_NO_ERR)
			    {   /* DSA returned result : move result */
				     ret_value = d21_cp05_set_result (parblk, res_ptr);
			    } /* end of DSA returned read-result */
		      }

		      /*
		      DSA did not return the required information
		      ->compare in cache
		      */
		      curr_time = time(0L);
		      if ((maxtime != D2_T_UNLIMITED) && (curr_time >= maxtime))
		      {   /* set time_limit_exceeded and return */
			  parblk->d21_6pbhead->d2_retcode = D2_TEMP_ERR;
			  parblk->d21_6pbhead->d2_errclass = D2_REM_ERR;
			  parblk->d21_6pbhead->d2_errvalue = D2_TIME_LIMIT_EXCEEDED;
			  ret_value = D2_ERROR;
			  break;
		      }

		      /* reset error from DSA-compare */
		      parblk->d21_6pbhead->d2_retcode =
		      parblk->d21_6pbhead->d2_errclass =
		      parblk->d21_6pbhead->d2_errvalue = D2_NO_ERR;
		      ret_value = d21_cp04_cache_compare(parblk,req_ptr,req_len,
							 &res_ptr,&res_len);
		      if (ret_value == D2_NOERROR)
		      {   /* return result from CACHE */
			  ret_value = d21_cp05_set_result (parblk, res_ptr);
			  break;
		      }
		      else
		      {
			  /* no result when reading from cache */
			  /* return error, if present       */
			  d21_cac_copy_error(res_ptr, parblk->d21_6pbhead,
					     &parblk->d21_6err);
			  ret_value = D2_ERROR;
		       }
		  } /* end of D21_DSA_FIRST_COMP */
		  break;

	  default:
		  {   DUA_TRACE_UNKNOWN_OPMODE
		      parblk->d21_6pbhead->d2_retcode = D2_PERM_ERR;
		      parblk->d21_6pbhead->d2_errclass = D2_SYS_ERR;
		      parblk->d21_6pbhead->d2_errvalue = D2_NOT_SPEC;
		      ret_value = D2_ERROR;
		      break;
		  }
      } /* end of switch(opmode) */
}

free(req_ptr);
if (res_ptr != NULL)
{   free(res_ptr);
}

if (ret_value1 == D21_ALLOC_ERROR)
{   parblk->d21_6pbhead->d2_retcode = D2_TEMP_ERR;
    parblk->d21_6pbhead->d2_errclass = D2_SYS_ERR;
    parblk->d21_6pbhead->d2_errvalue = D21_ALLOC_ERROR;
    ret_value = D2_ERROR;
}

#ifdef OSFCDS_REFERRALS
    /*  Check to see if the naming operation should be
	referred to the CDS name space.
							*/
    if (ret_value == D2_CALL_ERR)
	{
	if ( d21_cds_referral (parblk->d21_6entry, parblk->d21_6err)
	    == DSW_REFERRED)
	  {
	  ret_value = d21_6_switch (parblk);
	  }
	else
	  {
	    parblk->d21_6pbhead->d2_retcode = D2_CALL_ERR;
	    parblk->d21_6pbhead->d2_errclass = D2_PAR_ERR;
	    parblk->d21_6pbhead->d2_errvalue = D2_NO_BIND;
	    ret_value = D2_ERROR;
	  }
	}
#endif  /* OSFCDS_REFERRALS */

d21_ipc_clear(parblk->d21_6bind_id,&new_ipc,&b_tab);
       					/* clear LoopBack table */
d21_cr4_purge_lb_tab(lb_info,&lb_nr);

if (ret_value1 == D21_INTERRUPTED)
  {
  DUA_TRACE_INTERRUPTED

#ifdef THREADSAFE
  RAISE(pthread_cancel_e);
#endif
  }

DUA_TRACE_IAPL_EXIT(parblk->d21_6pbhead->d2_errvalue)

return(ret_value);
} /* end of d21_6_compare() */
