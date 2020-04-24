/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21_4_list.c,v $
 * Revision 1.1.10.2  1996/02/18  18:16:13  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:38:46  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:52:25  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:09:22  root]
 * 
 * Revision 1.1.8.3  1994/05/10  15:53:34  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:30:19  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:14:15  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:27:30  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:38:06  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:28:48  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  05:52:18  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  10:49:08  marrek]
 * 
 * Revision 1.1.4.4  1993/02/02  10:11:36  marrek
 * 	Insert <pthread.h>
 * 	use const declarations and d21_l01_init() instead of d21_l01_init_log()
 * 	fixes for OT 6576
 * 	[1993/02/01  17:59:34  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  19:45:51  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:23:42  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  21:05:16  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  13:32:38  marrek]
 * 
 * Revision 1.1.2.3  1992/06/30  20:57:46  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  20:38:03  melman]
 * 
 * Revision 1.1.2.2  1992/06/02  03:05:11  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:35:38  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21_4_list.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:16:13 $";
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
/* NAME         :  d21_4_list.c                                       */
/*                                                                    */
/* AUTHOR       :  J.Forster                                          */
/* DATE         :  1.2.88                                             */
/*                                                                    */
/* COMPONENT    :  Directory Service-V2 / xxxx                        */
/*                                                                    */
/* DOK.-REF.    :  Design-Specification, Leistungsbeschreibung        */
/*                                                                    */
/* PRD#/VERS.   :                                                     */
/*                                                                    */
/* DESCRIPTION  :  List   function                                    */
/*                                                                    */
/* D2_SYSTEM-DEPENDENCES:                                             */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |   Date   |   Updates                   | KZ | CR# FM#   */
/*    0.1   | 88-02-01 | Erstellung                  | jf |           */
/*    0.2   | 88-02-24 | DSA-Zugriff / complete Beh. | jf |           */
/*    1.2   | 88-08-24 | message-reduction           | jf |           */
/*                                                                    */
/*                                                                    */
/*datoff **************************************************************/

/*
**********************************************************************
*                                                                    *
*       I N C L U D E S                                              *
*                                                                    *
**********************************************************************
*/

#include <gds.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include <d21dua.h>
#ifdef OSFCDS
#include <d21_cdssw.h>
#endif

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d21_4_list                                         */
/*                                                                    */
/* AUTHOR       :  J.Forster,     Softlab GmbH.                       */
/* DATE         :  1.2.88                                             */
/*                                                                    */
/* SYNTAX       :  Short d21_4_list (parblk)                          */
/*                   D21_p4 *parblk;                                  */
/*                                                                    */
/*                                                                    */
/* DESCRIPTION  :  Ausgabe ist abhaengig davon, ob                    */
/*                 b) auf DSA gearbeitet wird                         */
/*                                                                    */
/*                 b) Relative Distinguished Names der "Kinder" des   */
/*                       vorgegebenen "Vaterknotens"                  */
/*                                                                    */
/* INPUT-PARAMETER :                                                  */
/*                                                                    */
/* OUTPUT-PARAMETER :                                                 */
/*                                                                    */
/* RETURN-VALUE :  0: Ausfuehrung erfolgreich                         */
/*                -1: Fehler: --> Fehlercode ueberpruefen             */
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

d2_ret_val d21_4_list (
  D21_p4 *parblk)
{
char            *req_ptr;          /* memory area for list request apdu  */
signed32          req_len;           /* length of list request apdu        */
char            *res_ptr;          /* memory area for list result        */
signed32          res_len;           /* length of list result              */
char            *next_mem;         /* location for next part of result   */
				   /*  in user-memory                    */
signed32            sc,maxentries;
time_t		maxtime, curr_time;
signed16             opmode;
signed16           entries_found = 0; /* counter */
char            *link;             /* link pointer for "chain" of names  */
D23_rserror     *res_err_ptr;
D23_rf_info     *ref_ptr = NULL;
int             out_file; /* file descriptor */
d2_ret_val      ret_value;
signed32      ret_value1;
d21_bind_info  b_tab;     /* copy of bind table entry */
D23_rf_info     *r_ptr = NULL;
const char function_name[] = "d21_4_list";
signed32             mem_len;
Bool new_ipc = FALSE;

D21_lb_info lb_info[D21_REF_MAX];
signed16 lb_nr=0;

/* store the time - used for time limit checks */
curr_time = time(0L);

	/* initializing logging and mutexes */
d21_L01_init();

DUA_TRACE_IAPL_ENTRY

/* log IAPL input */
d21_L04_trace_iapl_input(parblk->d21_4parent,parblk->d21_4c_a);

/* initialize params */
parblk->d21_4pbhead->d2_retcode = parblk->d21_4pbhead->d2_errclass =
parblk->d21_4pbhead->d2_errvalue = D2_NO_ERR;
ret_value = D2_NOERROR;
ret_value1 = D2_NOERROR;
parblk->d21_4rf        = NULL;
parblk->d21_4res       = NULL;
parblk->d21_4dist_name = NULL;
parblk->d21_4lim_prob  = D2_NO_LIMIT_EX;
parblk->d21_4uav_crit_ext = FALSE;
parblk->d21_4err       = NULL;
parblk->d21_4cres->d2_al_deref = FALSE;

/* and variables */
next_mem = NULL;
link = req_ptr = res_ptr = NULL;

/*check params */
if (d21_ls01_check_params(parblk) != D2_NOERROR)
  {
  DUA_TRACE_WRONG_IAPL_INPUT(parblk->d21_4pbhead->d2_errvalue)
  return(D2_ERROR);
  }

#ifdef OSFCDS
    /*  Check to see if this naming operation should be
	carried out in the CDS name space.
								*/
    if ( d21_cds_switch (parblk->d21_4parent) == DSW_NSPACE) {
	ret_value = d21_4_switch (parblk);
	return (ret_value);
    }
#endif /* OSFCDS */

sc = parblk->d21_4c_a->d2_serv_cntrls;
d21_cm01_serv_ctrl(sc, &opmode);

if (opmode != D21_CACHE_ONLY)
  {
  if (d21_bh8_get_bind_info(parblk->d21_4bind_id,&b_tab,&new_ipc,
				parblk->d21_4pbhead,TRUE) != D2_NOERROR)
    {
    DUA_TRACE_IAPL_EXIT(parblk->d21_4pbhead->d2_errvalue)
    return (D2_ERROR);
    }
  } /* if (not only cache) */

/* result to file ? */
if (parblk->d21_4file != NULL)
{   
    umask(000);
    if ( sc & D2_FILE_MODE )
    {   /* append mode */
	while ( (out_file = open(parblk->d21_4file,
			O_RDWR | O_CREAT | O_APPEND , D21_F_CREA_MODE) == -1 )
		&& (errno == EINTR)) ;
    }
    else
    {   /* overwrite mode */
	while ( (out_file = open(parblk->d21_4file,
			O_RDWR | O_CREAT | O_TRUNC , D21_F_CREA_MODE) == -1 )
		&& (errno == EINTR)) ;
    }

    if (out_file == -1)
    {   parblk->d21_4pbhead->d2_retcode = D2_TEMP_ERR;
	parblk->d21_4pbhead->d2_errclass = D2_SYS_ERR;
	parblk->d21_4pbhead->d2_errvalue = D2_NOT_SPEC;
	d21_ipc_clear(parblk->d21_4bind_id,&new_ipc,&b_tab);
	DUA_TRACE_IAPL_EXIT(parblk->d21_4pbhead->d2_errvalue)
	return(D2_ERROR);
    }
}

/* calculate length of request-apdu */
req_len = d21_ca_len_inlist(parblk);

/* request memory and build request-apdu */
if ((req_ptr = D27_MALLOC(svc_c_sev_warning,req_len)) == NULL  )
{   parblk->d21_4pbhead->d2_retcode = D2_TEMP_ERR;
    parblk->d21_4pbhead->d2_errclass = D2_SYS_ERR;
    parblk->d21_4pbhead->d2_errvalue = D21_ALLOC_ERROR;
    if ( parblk->d21_4file != NULL)
    {   /* result written to file */
	while ((close(out_file) == -1) && (errno == EINTR)) ;
    }
    d21_ipc_clear(parblk->d21_4bind_id,&new_ipc,&b_tab);
    DUA_TRACE_IAPL_EXIT(parblk->d21_4pbhead->d2_errvalue)
    return(D2_ERROR);
}

/* set time-limit */
if (parblk->d21_4c_a->d2_time_limit == D2_T_UNLIMITED)
{   maxtime = D2_T_UNLIMITED;
}
else
{   maxtime = time(0L) + parblk->d21_4c_a->d2_time_limit;
}

/* and list-limit */
if (parblk->d21_4c_a->d2_size_limit == D2_T_UNLIMITED)
{   maxentries = D2_S_UNLIMITED;
}
else
{   maxentries =  parblk->d21_4c_a->d2_size_limit;
}

/* build request-apdu */

if (opmode != D21_CACHE_ONLY)
  {
  d21_ls03_build_req(parblk,req_ptr,b_tab.d21_b_dir_id, 0);
  }
else
  {
  d21_ls03_build_req(parblk,req_ptr,parblk->d21_4bind_id, -1);
  }

  if                            /* referrals to handle are expected   */
    (!(sc & D2_LOCALSCOPE) && sc & D2_NOREFERRAL &&
     (opmode != D21_CACHE_ONLY) )
  {
				/*                                      */
				/* add BIND DSA in the LoopBack Table   */
				/*                                      */
    if (d21_cr1_add_lb(lb_info,&lb_nr,parblk->d21_4parent,
	&parblk->d21_4c_a->d2_op_progress,
	b_tab.d21_b_name_dsa) != D2_NOERROR)
    {
	parblk->d21_4pbhead->d2_retcode = D2_TEMP_ERR;
	parblk->d21_4pbhead->d2_errclass = D2_SYS_ERR;
	parblk->d21_4pbhead->d2_errvalue = D21_ALLOC_ERROR;
	d21_ipc_clear(parblk->d21_4bind_id,&new_ipc,&b_tab);
			       /* clear LoopBack table */
	d21_cr4_purge_lb_tab(lb_info,&lb_nr);
	return(D2_ERROR);
    }
  }
    /* select appropriate search routine */
switch(opmode)
    {  
	 case D21_CACHE_ONLY:
		 DUA_TRACE_OPMODE_CACHE_ONLY
    		 d21_ipc_clear(parblk->d21_4bind_id,&new_ipc,&b_tab);
       						/* clear LoopBack table */
    		 d21_cr4_purge_lb_tab(lb_info,&lb_nr);
		 return (D2_ERROR);

	case D21_DSA_ONLY:
		{   DUA_TRACE_OPMODE_DSA_ONLY

		    if (parblk->d21_4file != NULL)
		    {     /* move result to file */
			  ret_value = d21_sc10_write_output_to_file(
					     parblk->d21_4pbhead,&b_tab,req_ptr,req_len,
					     &ref_ptr, &entries_found,
					     D23_LIST, &parblk->d21_4lim_prob,
					     out_file,FALSE);
			  if ((ret_value == D2_NOERROR) &&
			      (ref_ptr != NULL))
			  {      /* handle referrals in result */
				 if (sc & D2_NOREFERRAL)
				 {
				     ret_value = d21_sc13_handle_file_ref(
					lb_info,&lb_nr,parblk->d21_4pbhead,
					     req_ptr,req_len,maxtime,ref_ptr,
					     &entries_found,D23_LIST,
					  &parblk->d21_4lim_prob,&b_tab,out_file);
				 }
				 else
				 {   /* no error-referral, "real error" */
				     r_ptr = ref_ptr;
				     mem_len = d21_ca_len_ref_inf(
							(char **)&r_ptr);
				     r_ptr = ref_ptr;
				     if ((parblk->d21_4rf = D27_MALLOC(
					   svc_c_sev_warning,mem_len)) == NULL)
				     {   parblk->d21_4pbhead->d2_retcode = D2_TEMP_ERR;
					 parblk->d21_4pbhead->d2_errclass = D2_SYS_ERR;
					 parblk->d21_4pbhead->d2_errvalue = D21_ALLOC_ERROR;
    					 d21_ipc_clear(parblk->d21_4bind_id,&new_ipc,&b_tab);
       						/* clear LoopBack table */
    		 			 d21_cr4_purge_lb_tab(lb_info,&lb_nr);
					 return(D2_ERROR);
				     }
				     ret_value = d21_cac_copy_referral(
					(char **)&r_ptr, parblk->d21_4rf);
				  }
			  }
			  break;
		    }

		    /* result to memory */

		    /* allocate 1 ipc block for result */
		    res_len = b_tab.d21_b_ipcinf->d21_i_lenms;
		    res_ptr = D27_MALLOC(svc_c_sev_warning,res_len);
		    if (res_ptr == NULL)
		    {   parblk->d21_4pbhead->d2_retcode = D2_TEMP_ERR;
			parblk->d21_4pbhead->d2_errclass = D2_SYS_ERR;
			parblk->d21_4pbhead->d2_errvalue = D21_ALLOC_ERROR;
			ret_value = D2_ERROR;
			break;
		    }

		    ret_value1 = d21_ls07_dsa_list(parblk,req_ptr,req_len,
						  &res_ptr,&res_len,&b_tab);
		    if (ret_value1 != D2_NOERROR)
		    {   /*
			   IPC error or memory error
			*/
			ret_value = D2_ERROR;
			break;
		    }

		    /* process result */
		    if (parblk->d21_4pbhead->d2_retcode == D2_NO_ERR &&
			parblk->d21_4pbhead->d2_errclass == D2_NO_ERR &&
			parblk->d21_4pbhead->d2_errvalue == D2_NO_ERR)
		    {   /* DSA returned result */
			ret_value = d21_ls05_move_names_to_mem(
						  parblk, res_ptr, res_len,
						  &next_mem, &ref_ptr,
						  &link, &entries_found);
			if ((ret_value == D2_NOERROR) && (ref_ptr != NULL))
			  {   
			    /* handle referrals in result */
			    ret_value = d21_ls08_handle_ref(lb_info,&lb_nr,
							parblk,req_ptr,
							req_len,&res_ptr,
							&res_len,maxtime,
							maxentries,&next_mem,
						        &ref_ptr,&link,
							&entries_found,&b_tab);
			}
			break;
		    }

		    /* check for error-referral */
		    if (parblk->d21_4pbhead->d2_errvalue == D2_REF_ERROR)
		    {   res_err_ptr = (D23_rserror *)res_ptr;
			ref_ptr = (D23_rf_info *)(res_err_ptr + 1);
			/*  DSA returned referral */
			if (sc & D2_NOREFERRAL)
			{   /* handle referral */
			    ret_value = d21_ls08_handle_ref(lb_info,&lb_nr,
						 parblk,req_ptr,req_len,
						 &res_ptr,&res_len,maxtime,
						 maxentries,&next_mem,
						 &ref_ptr,&link,
						 &entries_found,&b_tab);
			    if (ret_value != D2_NOERROR)
			      {
		              d21_cac_copy_error(res_ptr,
					parblk->d21_4pbhead, &parblk->d21_4err);
		       	      ret_value = D2_ERROR;
			      }
			}
			else
			{   /* no error-referral, "real error" */

			    d21_cac_copy_error(res_ptr, parblk->d21_4pbhead,
						     &parblk->d21_4err);
			    ret_value = D2_ERROR;
			}
		     }
		     else
		     {  /* no error-referral, "real error" */
			d21_cac_copy_error(res_ptr, parblk->d21_4pbhead,
						     &parblk->d21_4err);
			ret_value = D2_ERROR;
		     }

		break;
	    } /* endcase DSA */

	case D21_DSA_FIRST:
		 DUA_TRACE_OPMODE_DSA_FIRST
    		 d21_ipc_clear(parblk->d21_4bind_id,&new_ipc,&b_tab);
       						/* clear LoopBack table */
    		 d21_cr4_purge_lb_tab(lb_info,&lb_nr);
		 return (D2_ERROR);

	default:
	    {   DUA_TRACE_UNKNOWN_OPMODE
		parblk->d21_4pbhead->d2_retcode = D2_PERM_ERR;
		parblk->d21_4pbhead->d2_errclass = D2_SYS_ERR;
		parblk->d21_4pbhead->d2_errvalue = D2_NOT_SPEC;
		ret_value = D2_ERROR;
	    }
    } /* end of switch (opmode) */

if (req_ptr != NULL)
{   free(req_ptr);
}
if (res_ptr != NULL)
{   free(res_ptr);
}

#ifdef OSFCDS_REFERRALS
    /*  Check to see if the naming operation should be
	referred to the CDS name space.
							*/
    if (ret_value == D2_CALL_ERR)
	{
	if ( d21_cds_referral (parblk->d21_4parent, parblk->d21_4err)
	    == DSW_REFERRED)
	  {
	  ret_value = d21_4_switch (parblk);
	  }
	else
	  {
	  parblk->d21_4pbhead->d2_retcode = D2_CALL_ERR;
	  parblk->d21_4pbhead->d2_errclass = D2_PAR_ERR;
	  parblk->d21_4pbhead->d2_errvalue = D2_NOT_SPEC;
	  ret_value = D2_ERROR;
	  }
	}
#endif  /* OSFCDS_REFERRALS */

if ( parblk->d21_4file != NULL)
{   /* result written to file */
    while ((close(out_file) == -1) && (errno == EINTR)) ;
}

d21_ipc_clear(parblk->d21_4bind_id,&new_ipc,&b_tab);
					/* clear LoopBack table */
d21_cr4_purge_lb_tab(lb_info,&lb_nr);

if (ret_value1 == D21_INTERRUPTED)
  {
  DUA_TRACE_INTERRUPTED

#ifdef THREADSAFE
  RAISE(pthread_cancel_e);
#endif
  }

DUA_TRACE_IAPL_EXIT(parblk->d21_4pbhead->d2_errvalue)

return(ret_value);

} /* end of d21_4_list() */
