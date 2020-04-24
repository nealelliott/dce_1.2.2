/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21_3_read.c,v $
 * Revision 1.1.10.2  1996/02/18  18:16:11  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:38:44  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:52:21  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:09:20  root]
 * 
 * Revision 1.1.8.3  1994/05/10  15:53:33  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:30:13  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:14:13  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:27:25  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:38:04  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:28:35  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  05:50:56  marrek
 * 	time limit is exceeded also if max_time=curr_time.
 * 	[1993/08/03  09:29:34  marrek]
 * 
 * 	July 1993 code drop.
 * 	[1993/07/30  10:48:50  marrek]
 * 
 * Revision 1.1.4.4  1993/02/02  10:11:20  marrek
 * 	Insert <pthread.h>
 * 	use d21_l01_init() instead of d21_l01_init_log()
 * 	use const declarations
 * 	[1993/02/01  17:57:47  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  19:45:45  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:23:37  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  21:04:45  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  13:32:16  marrek]
 * 
 * Revision 1.1.2.3  1992/06/30  20:57:40  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  20:37:55  melman]
 * 
 * Revision 1.1.2.2  1992/06/02  03:04:57  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:35:31  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21_3_read.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:16:11 $";
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
/* NAME         :  d21_3_read.c                                       */
/*                                                                    */
/* AUTHOR       :  J.Forster                                          */
/* DATE         :  4.1.88                                             */
/*                                                                    */
/* COMPONENT    :  Directory Service-V2 /DIR-X                        */
/*                                                                    */
/* DOK.-REF.    :  Design-Specification, Leistungsbeschreibung        */
/*                                                                    */
/* PRD#/VERS.   :                                                     */
/*                                                                    */
/* DESCRIPTION  :  Read function                                      */
/*                                                                    */
/* D2_SYSTEM-DEPENDENCES:                                             */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |   Date   |   Updates                   | KZ | CR# FM#   */
/*    0.1   | 88-01-04 | Erstellung                  | jf |           */
/*                                                                    */
/*datoff **************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <d21dua.h>
#ifdef OSFCDS
#include <d21_cdssw.h>
#endif

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d21_3_read                                         */
/*                                                                    */
/* AUTHOR       :  J.Forster,     Softlab GmbH.                       */
/* DATE         :  4.1.88                                             */
/*                                                                    */
/* SYNTAX       :  Short d21_3_read (parblk)                          */
/*                   D21_p3 *parblk;                                  */
/*                                                                    */
/*                                                                    */
/* DESCRIPTION  :  read attributes from directory on the              */
/*                 basis of Service-Control-Bits                      */
/*                 . from Cache only                                  */
/*                 . from DSA only                                    */
/*                 . from Cache first and, then, from DSA             */
/*                 . from DSA first, and, then from Cache             */
/*                                                                    */
/* INPUT-PARAMETER :                                                  */
/*                                                                    */
/* OUTPUT-PARAMETER :                                                 */
/*                                                                    */
/* RETURN-VALUE :  D2_NOERROR: In the case of Success                 */
/*                   D2_ERROR: In the case of error                   */
/*                                                                    */
/*                                                                    */
/* STANDARDHEAD :                                                     */
/*                                                                    */
/* GLOBAL DATA (used) :                                               */
/*                                                                    */
/* GLOBAL DATA (modified) :                                           */
/*                                                                    */
/* COMMENTS :                                                         */
/*                                                                    */
/*exoff ***************************************************************/

d2_ret_val d21_3_read (
  D21_p3 * parblk)
{
char                  *req_ptr; /* memory area for read request         */
signed32		req_len; /* length of read request               */
char                  *res_ptr; /* memory area for read result          */
signed32                 res_len; /* length of read result                */
D23_rf_info             *ref_ptr;
d2_ret_val                   ret_value;
signed32                   ret_value1;
signed16                     opmode;
signed16                   att_complete;
signed32                    sc;
time_t 		maxtime,curr_time;
d21_bind_info    b_tab;  /* copy of bind table entry              */
Bool new_ipc = FALSE;

D21_lb_info lb_info[D21_REF_MAX];
signed16 lb_nr=0;

const char function_name[] = "d21_3_read";
Bool adm_syntax = FALSE;

/* store the time - used for time limit checks */
curr_time = time(0L);

/* initialize logging and mutexes */
d21_L01_init();

DUA_TRACE_IAPL_ENTRY

/* log IAPL input */
d21_L04_trace_iapl_input(parblk->d21_3entry,parblk->d21_3c_a);

/*
initialize return code  ...
*/
parblk->d21_3pbhead->d2_retcode = parblk->d21_3pbhead->d2_errclass =
parblk->d21_3pbhead->d2_errvalue = D2_NO_ERR;
ret_value = D2_NOERROR;

/*
... pointer to result and referral ...
*/
parblk->d21_3res = NULL;
parblk->d21_3err = NULL;
/*
... and some other variables
*/
req_ptr = res_ptr = NULL;

if ((ret_value = d21_rd01_check_params(parblk)) != D2_NOERROR)
  {
  DUA_TRACE_WRONG_IAPL_INPUT(parblk->d21_3pbhead->d2_errvalue)
  return(D2_ERROR);
  }

#ifdef OSFCDS
    /*  Check to see if this naming operation should be
	carried out in the CDS name space.
								*/
    if (d21_cds_switch (parblk->d21_3entry) == DSW_NSPACE) {
	ret_value = d21_3_switch (parblk);
	return (ret_value);
    }
#endif /* OSFCDS */

/*
select appropriate read routine
*/
sc = parblk->d21_3c_a->d2_serv_cntrls;
d21_cm01_serv_ctrl(sc, &opmode);

if (opmode != D21_CACHE_ONLY)
  {
  if (d21_bh8_get_bind_info(parblk->d21_3bind_id,&b_tab,&new_ipc,
				parblk->d21_3pbhead,TRUE) != D2_NOERROR)
    {
    DUA_TRACE_IAPL_EXIT(parblk->d21_3pbhead->d2_errvalue)
    return (D2_ERROR);
    }
	
  if (b_tab.d21_b_syntax != D27_ASN1_TS)
    {
    adm_syntax=TRUE && (parblk->d21_3c_a->d2_serv_cntrls & D2_PREF_ADM_FUN);
    }
  } /* if (not only cache) */

/* calculate length of request-apdu */

req_len = d21_ca_len_inread(parblk);
if ((req_ptr = D27_MALLOC(svc_c_sev_warning,req_len)) == NULL)
{   parblk->d21_3pbhead->d2_retcode = D2_TEMP_ERR;
    parblk->d21_3pbhead->d2_errclass = D2_SYS_ERR;
    parblk->d21_3pbhead->d2_errvalue = D21_ALLOC_ERROR;
    if (opmode != D21_CACHE_ONLY)
   	 d21_ipc_clear(parblk->d21_3bind_id,&new_ipc,&b_tab);
    DUA_TRACE_IAPL_EXIT(parblk->d21_3pbhead->d2_errvalue)
    return(D2_ERROR);
}

/*
build request-apdu 
*/
if (opmode != D21_CACHE_ONLY)
{
  d21_rd03_build_req(parblk,req_ptr,b_tab.d21_b_dir_id, 0);
}
else
{
  d21_rd03_build_req(parblk,req_ptr,parblk->d21_3bind_id, -1);
}

/*
set time_limit
*/
if (parblk->d21_3c_a->d2_time_limit ==  D2_T_UNLIMITED)
{   maxtime = D2_T_UNLIMITED;
}
else
{   maxtime = time(0L) + parblk->d21_3c_a->d2_time_limit;
}

if                              /* referrals to handle are expected   */
  (!(sc & D2_LOCALSCOPE) && sc & D2_NOREFERRAL &&
   opmode != D21_CACHE_ONLY)
{
				/*                                      */
				/* add BIND DSA in the LoopBack Table   */
				/*                                      */
  if (d21_cr1_add_lb(lb_info,&lb_nr,parblk->d21_3entry,
	&parblk->d21_3c_a->d2_op_progress, b_tab.d21_b_name_dsa) != D2_NOERROR)
  {
    parblk->d21_3pbhead->d2_retcode = D2_TEMP_ERR;
    parblk->d21_3pbhead->d2_errclass = D2_SYS_ERR;
    parblk->d21_3pbhead->d2_errvalue = D21_ALLOC_ERROR;
    d21_ipc_clear(parblk->d21_3bind_id,&new_ipc,&b_tab);
			       /* clear LoopBack table */
    d21_cr4_purge_lb_tab(lb_info,&lb_nr);
    DUA_TRACE_IAPL_EXIT(parblk->d21_3pbhead->d2_errvalue)
    return(D2_ERROR);
  }
}

switch(opmode)
{   case D21_CACHE_ONLY:
	    {                       /* read from DUA-Cache  */
		DUA_TRACE_OPMODE_CACHE_ONLY

		    /*
		    execute read
		    */
		    ret_value = d21_rd04_cache_read(parblk,req_ptr,req_len,
						    &res_ptr,&res_len);

		    if (ret_value != D2_NOERROR)
			break;
		    else
		     if (parblk->d21_3pbhead->d2_retcode == D2_NO_ERR &&
			 parblk->d21_3pbhead->d2_errclass == D2_NO_ERR &&
			 parblk->d21_3pbhead->d2_errvalue == D2_NO_ERR)
		      {   /*
			  move result to user-memory
			  */
			  ret_value = d21_rd05_move_ei_info_to_mem(parblk,
						    res_ptr);
			  parblk->d21_3cres->d2_al_deref = FALSE;
			  parblk->d21_3cres->d2_performer = NULL;
		      }
		      else
		      {   /* no error-referral, "real error" */
			  d21_cac_copy_error(res_ptr,
				parblk->d21_3pbhead, &parblk->d21_3err);
			  ret_value = D2_ERROR;
		      }
	    }  /* end of CACHE_ONLY */
	    break;

    case D21_DSA_ONLY:
		{
		DUA_TRACE_OPMODE_DSA_ONLY
		/*
		allocate 1 ipc block for result
		*/
		res_len = b_tab.d21_b_ipcinf->d21_i_lenms;
		if ((res_ptr = D27_MALLOC(svc_c_sev_warning,res_len)) == NULL)
		{   ret_value1 = D21_ALLOC_ERROR;
		    break;
		}
		/*
		exec DSA ONLY READ
		*/
		ret_value1 = d21_rd06_dsa_read(adm_syntax,parblk,req_ptr,
					req_len, &res_ptr,&res_len,&b_tab);
		if (ret_value1 != D2_NOERROR)
		{   /*
		    error when reading from DSA :
		    DSA did not return result, error or referral
		    */
		    ret_value = D2_ERROR;
		    break;
		}

		/*
		DSA returned result, error or referral
		*/
		if (parblk->d21_3pbhead->d2_retcode == D2_NO_ERR &&
		    parblk->d21_3pbhead->d2_errclass == D2_NO_ERR &&
		    parblk->d21_3pbhead->d2_errvalue == D2_NO_ERR)
		{   /*
		    DSA returned result : move result
		    */
		    ret_value = d21_rd05_move_ei_info_to_mem(parblk,res_ptr);
		    break;
		} /* end of DSA returned read-result */

		ref_ptr = (D23_rf_info *)(res_ptr + sizeof(D23_rserror));

		if ((ref_ptr->d23_rftag == D23_REFTAG)
		    && (sc & D2_NOREFERRAL))
			{   /*
			    handle referral
			    */
			    parblk->d21_3pbhead->d2_retcode =
			    parblk->d21_3pbhead->d2_errclass =
			    parblk->d21_3pbhead->d2_errvalue = D2_NO_ERR;
			    ret_value =
			      d21_handle_ref(lb_info,&lb_nr,&ref_ptr,
			parblk->d21_3pbhead, maxtime, parblk->d21_3bind_id,
				   adm_syntax? D23_READ+D2_ADM_OPID : D23_READ,
				   &res_ptr, &res_len, req_ptr, req_len,&b_tab);
			      if (parblk->d21_3pbhead->d2_retcode == D2_NO_ERR &&
				  parblk->d21_3pbhead->d2_errclass == D2_NO_ERR &&
				  parblk->d21_3pbhead->d2_errvalue == D2_NO_ERR)
			      {   /*
				  DSA returned result : move result
				  */
			        ret_value = d21_rd05_move_ei_info_to_mem(parblk,
							res_ptr);
			      } /* end of DSA returned read-result */
			      else
			       {   /* no error-referral, "real error" */
				    d21_cac_copy_error(res_ptr,
						       parblk->d21_3pbhead,
						       &parblk->d21_3err);
				    ret_value = D2_ERROR;
			       }
			}
		    else
		      {   /* no error-referral, "real error" */
			  d21_cac_copy_error(res_ptr,
					parblk->d21_3pbhead, &parblk->d21_3err);
			  ret_value = D2_ERROR;
		       }
	    } /* endcase DSA_ONLY */
	    break;

    case D21_CACHE_FIRST:
	    {   
		DUA_TRACE_OPMODE_CACHE_FIRST
		/*
		read from DUACACHE first
		*/
		ret_value = d21_rd04_cache_read(parblk,req_ptr,req_len,
						&res_ptr,&res_len);
		/*
		all requested attributes read ?
		*/
		if ( (ret_value == D2_NOERROR)  &&
		      parblk->d21_3pbhead->d2_retcode == D2_NO_ERR &&
		      parblk->d21_3pbhead->d2_errclass == D2_NO_ERR &&
		      parblk->d21_3pbhead->d2_errvalue == D2_NO_ERR &&
		     ((att_complete = d21_rd07_check_attr(parblk,res_ptr,
						     res_len)) == D2_NOERROR)
		   )
		{   /*
		    yes, then return result read from CACHE
		    */
		    ret_value = d21_rd05_move_ei_info_to_mem(parblk,
						  res_ptr);
		    parblk->d21_3cres->d2_al_deref = FALSE;
		    parblk->d21_3cres->d2_performer = NULL;
		    break;
		}

		/*
		error or incomplete result when reading from cache :
		try to read from DSA, if timelimit not yet exceeded
		*/
		curr_time = time(0L);
		if ((maxtime != D2_T_UNLIMITED) && (curr_time >= maxtime))
		{   /* set time_limit_exceeded and return */
		    parblk->d21_3pbhead->d2_retcode = D2_TEMP_ERR;
		    parblk->d21_3pbhead->d2_errclass = D2_REM_ERR;
		    parblk->d21_3pbhead->d2_errvalue=D2_TIME_LIMIT_EXCEEDED;
		    ret_value = D2_ERROR;
		    break;
		}

		/*
		timelimit not exceeded : read from DSA
		(reset error from CACHE-read)
		*/
		parblk->d21_3pbhead->d2_retcode =
		parblk->d21_3pbhead->d2_errclass =
		parblk->d21_3pbhead->d2_errvalue = D2_NO_ERR;
		ret_value1 = d21_rd06_dsa_read(adm_syntax,parblk,req_ptr,req_len,
				      &res_ptr,&res_len,&b_tab);
		if (ret_value1 != D2_NOERROR)
		{   /* error when reading from DSA */
		    ret_value = D2_ERROR;
		    break;
		}

		if (parblk->d21_3pbhead->d2_retcode == D2_NO_ERR &&
		    parblk->d21_3pbhead->d2_errclass == D2_NO_ERR &&
		    parblk->d21_3pbhead->d2_errvalue == D2_NO_ERR)
		{   /* DSA returned  read-result */
		    ret_value = d21_rd05_move_ei_info_to_mem(parblk,res_ptr);
		    break;
		} /* end of DSA returned read-result */

		ref_ptr = (D23_rf_info *)(res_ptr + sizeof(D23_rserror));

		if ((ref_ptr->d23_rftag == D23_REFTAG)
		      && (sc & D2_NOREFERRAL))
			{   /*
			    handle referral
			    */
			    parblk->d21_3pbhead->d2_retcode =
			    parblk->d21_3pbhead->d2_errclass =
			    parblk->d21_3pbhead->d2_errvalue = D2_NO_ERR;
			    ret_value =
			      d21_handle_ref(lb_info,&lb_nr,&ref_ptr,
			parblk->d21_3pbhead, maxtime, parblk->d21_3bind_id,
				   adm_syntax? D23_READ+D2_ADM_OPID : D23_READ,
				   &res_ptr, &res_len, req_ptr, req_len,&b_tab);
			      if (parblk->d21_3pbhead->d2_retcode == D2_NO_ERR &&
				  parblk->d21_3pbhead->d2_errclass == D2_NO_ERR &&
				  parblk->d21_3pbhead->d2_errvalue == D2_NO_ERR)
			      {   /*
				  DSA returned result : move result
				  */
			        ret_value = d21_rd05_move_ei_info_to_mem(parblk,
							res_ptr);
			      } /* end of DSA returned read-result */
			      else
			      {   /* no error-referral, "real error" */
				 d21_cac_copy_error(res_ptr,
					parblk->d21_3pbhead, &parblk->d21_3err);
				 ret_value = D2_ERROR;
			      }
		    }
		    else
		    {   /* no error-referral, "real error" */
			d21_cac_copy_error(res_ptr,parblk->d21_3pbhead,
						       &parblk->d21_3err);
			ret_value = D2_ERROR;
		    }
	    } /* end of D21_CACHE_FIRST */
	    break;

    case D21_DSA_FIRST:
	    {   
		DUA_TRACE_OPMODE_DSA_FIRST
		/*
		read from DSA
		*/
		/* allocate 1 ipc block for result */
		res_len = b_tab.d21_b_ipcinf->d21_i_lenms;
		res_ptr = D27_MALLOC(svc_c_sev_warning,res_len);
		if (res_ptr == NULL)
		{   ret_value1 = D21_ALLOC_ERROR;
		    break;
		}
		ret_value1 = d21_rd06_dsa_read(adm_syntax,parblk,req_ptr,
					req_len, &res_ptr,&res_len,&b_tab);
		if (ret_value1 != D2_NOERROR)
		{   /*
		       IPC error or memory error
		    */
		    ret_value = D2_ERROR;
		    break;
		}

		if (parblk->d21_3pbhead->d2_retcode == D2_NO_ERR &&
		    parblk->d21_3pbhead->d2_errclass == D2_NO_ERR &&
		    parblk->d21_3pbhead->d2_errvalue == D2_NO_ERR)
		{   /* DSA returned  read-result */
		    ret_value = d21_rd05_move_ei_info_to_mem(parblk,
						  res_ptr);
		    break;
		}

		ref_ptr = (D23_rf_info *)(res_ptr + sizeof(D23_rserror));

		if ((ref_ptr->d23_rftag == D23_REFTAG)
		    &&  (sc & D2_NOREFERRAL))
			{   /*
			    handle referral
			    */
			    parblk->d21_3pbhead->d2_retcode =
			    parblk->d21_3pbhead->d2_errclass =
			    parblk->d21_3pbhead->d2_errvalue = D2_NO_ERR;
			    ret_value =
			      d21_handle_ref(lb_info,&lb_nr,&ref_ptr,
			parblk->d21_3pbhead, maxtime, parblk->d21_3bind_id,
				   adm_syntax? D23_READ+D2_ADM_OPID : D23_READ,
				   &res_ptr, &res_len, req_ptr, req_len,&b_tab);
			      if (parblk->d21_3pbhead->d2_retcode == D2_NO_ERR &&
				  parblk->d21_3pbhead->d2_errclass == D2_NO_ERR &&
				  parblk->d21_3pbhead->d2_errvalue == D2_NO_ERR)
			      {   /*
				  DSA returned result : move result
				  */
			        ret_value = d21_rd05_move_ei_info_to_mem(parblk,
							res_ptr);
			      } /* end of DSA returned read-result */
		    }

		if (parblk->d21_3res != NULL)
		{   /* result from referred read) */
		    break;
		}
		/*
		DSA did not return the required information ->read from cache
		*/
		curr_time = time(0L);
		if ((maxtime != D2_T_UNLIMITED) && (curr_time >= maxtime))
		{   /* set time_limit_exceeded and return */
		    parblk->d21_3pbhead->d2_retcode = D2_TEMP_ERR;
		    parblk->d21_3pbhead->d2_errclass = D2_REM_ERR;
		    parblk->d21_3pbhead->d2_errvalue=D2_TIME_LIMIT_EXCEEDED;
		    ret_value = D2_ERROR;
		    break;
		}


		/* reset error from DSA-read */
		parblk->d21_3pbhead->d2_retcode =
		parblk->d21_3pbhead->d2_errclass =
		parblk->d21_3pbhead->d2_errvalue = D2_NO_ERR;
		free (res_ptr);
		ret_value = d21_rd04_cache_read(parblk,req_ptr,req_len,
						&res_ptr,&res_len);
		if (ret_value != D2_NOERROR)
		    break;
		else
		    if (parblk->d21_3pbhead->d2_retcode == D2_NO_ERR &&
			parblk->d21_3pbhead->d2_errclass == D2_NO_ERR &&
			parblk->d21_3pbhead->d2_errvalue == D2_NO_ERR)
		    {   /* return result read from CACHE */
			ret_value = d21_rd05_move_ei_info_to_mem(parblk,
							 res_ptr);
			break;
		    }
		    else
		    {   /* no result when reading from cache */
			/* return error, if present       */
			d21_cac_copy_error(res_ptr,
					parblk->d21_3pbhead, &parblk->d21_3err);
			ret_value = D2_ERROR;
		    }
	      } /* end of D21_DSA_FIRST */
	      break;

    default:
	    { 
		DUA_TRACE_UNKNOWN_OPMODE
		parblk->d21_3pbhead->d2_retcode = D2_PERM_ERR;
		parblk->d21_3pbhead->d2_errclass = D2_SYS_ERR;
		parblk->d21_3pbhead->d2_errvalue = D2_NOT_SPEC;
		ret_value = D2_ERROR;
		break;
	    }
} /* end of switch(opmode) */

if (req_ptr != NULL)
{   free(req_ptr);
}

if (res_ptr != NULL)
{   free(res_ptr);
}

if (ret_value1 == D21_ALLOC_ERROR)
{   parblk->d21_3pbhead->d2_retcode = D2_TEMP_ERR;
    parblk->d21_3pbhead->d2_errclass = D2_SYS_ERR;
    parblk->d21_3pbhead->d2_errvalue = D21_ALLOC_ERROR;
    ret_value = D2_ERROR;
}

#ifdef OSFCDS_REFERRALS
    /*  Check to see if the naming operation should be
	referred to the CDS name space.
							*/
    if (ret_value == D2_CALL_ERR)
	{
	if ( d21_cds_referral (parblk->d21_3entry, parblk->d21_3err)
	    == DSW_REFERRED)
	  {
	    ret_value = d21_3_switch (parblk);
	  }
	else
	  {
	  parblk->d21_3pbhead->d2_retcode = D2_CALL_ERR;
	  parblk->d21_3pbhead->d2_errclass = D2_PAR_ERR;
	  parblk->d21_3pbhead->d2_errvalue = D2_NO_BIND;
	  ret_value = D2_ERROR;
	  }
	}
#endif  /* OSFCDS_REFERRALS */

if (opmode != D21_CACHE_ONLY)
	d21_ipc_clear(parblk->d21_3bind_id,&new_ipc,&b_tab);

			       /* clear LoopBack table */
d21_cr4_purge_lb_tab(lb_info,&lb_nr);

if (ret_value1 == D21_INTERRUPTED)
  {
  DUA_TRACE_INTERRUPTED

#ifdef THREADSAFE
  RAISE(pthread_cancel_e);
#endif
  }

DUA_TRACE_IAPL_EXIT(parblk->d21_3pbhead->d2_errvalue)

return(ret_value);

} /* end of d21_3_read() */
