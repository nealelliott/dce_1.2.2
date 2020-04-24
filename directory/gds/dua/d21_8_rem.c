/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21_8_rem.c,v $
 * Revision 1.1.10.2  1996/02/18  18:16:22  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:38:57  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:52:47  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:09:31  root]
 * 
 * Revision 1.1.8.3  1994/05/10  15:53:40  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:30:47  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:14:22  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:27:51  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:38:15  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:29:44  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  05:58:39  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  10:50:34  marrek]
 * 
 * Revision 1.1.4.4  1993/02/02  10:12:44  marrek
 * 	Insert <pthread.h>
 * 	use const declarations
 * 	change from d21_l01_init_log() to d21_l01_init()
 * 	[1993/02/01  18:05:31  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  19:46:09  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:24:02  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  21:07:14  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  13:34:02  marrek]
 * 
 * Revision 1.1.2.3  1992/06/30  20:58:14  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  20:38:35  melman]
 * 
 * Revision 1.1.2.2  1992/06/02  03:12:14  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:36:04  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21_8_rem.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:16:22 $";
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
/* NAME         :  d21_8_rem.c                                        */
/*                                                                    */
/* AUTHOR       :  Albert Biber                                       */
/* DATE         :  03.02.88                                           */
/*                                                                    */
/* DESCRIPTION  :  This module contain the DUA function               */
/*                 DS_REMOVE_OBJECT which allows the DUA-Application  */
/*                 to remove an object either from the DIB of the     */
/*                 DSA or from the DUA-cache database.                */
/*                                                                    */
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
#endif	/* OSFCDS */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d21_8_remove_object                                */
/*                                                                    */
/* AUTHOR       :  Albert Biber / Johann Forster ,  Softlab GmbH.     */
/* DATE         :  02.02.88                                           */
/*                                                                    */
/* SYNTAX       :  d2_ret_val d21_8_remove_object (parblk)                 */
/*                 D21_p8  *parblk;                                   */
/*                                                                    */
/*                                                                    */
/* DESCRIPTION  :  The function d21_8_remove_object allows the        */
/*                 application to remove an object (master-entry,     */
/*                 shodow-entry or alias-entry) either from the       */
/*                 DIB of the bind-DSA or from the database           */
/*                 of the local DUA-Cache.                            */
/*                 "Naming shadows" will be removed if neccessary.    */
/*                                                                    */
/* INPUT-PARAMETER :                                                  */
/*                                                                    */
/* OUTPUT-PARAMETER :                                                 */
/*                                                                    */
/* RETURN-VALUE :    D2_NOERROR   Function terminated successfully     */
/*                   D2_ERROR   Function terminated with error        */
/*                                                                    */
/*exoff ***************************************************************/

d2_ret_val d21_8_remove_object (
  D21_p8 * parblk)
{
d21_bind_info b_tab;
d21_bind_info m_b_tab;
d21_bind_info s_b_tab;
d21_ipc_info *ipc_tbl;
D21_req_info req_info;
D23_inremobjt *inv_apdu;
char *req_buf;
char *req_end;
char *res_buf;
d2_ret_val ret_value;
signed16 ret_value1;
signed32 buf_len;
signed32 res_len;

char            sup_name[D2_DNL_MAX];
char            master_dsa[D2_DNL_MAX];
char            s_master_dsa[D2_DNL_MAX];

D2_pbhead       call_head;
signed16           s_b_id;
signed16           m_b_id;

const char function_name[] = "d21_8_rem";
signed32            sc;
signed16             opmode;
D2_error       *err_info;
Bool            alias_flag;
Bool		adm_syntax = FALSE;

Bool new_ipc = FALSE;
Bool s_new_ipc = FALSE;
Bool m_new_ipc = FALSE;

  /*
  **********************************************************************
  *                                                                    *
  *                 C O D E                                            *
  *                                                                    *
  **********************************************************************
  */
	       /* initialize logging and mutexes */
d21_L01_init();

DUA_TRACE_IAPL_ENTRY

/* log IAPL input */
d21_L04_trace_iapl_input(parblk->d21_8entry,parblk->d21_8c_a);

  /*
     initialize return code and referral-pointer ...
  */
parblk->d21_8pbhead->d2_retcode = parblk->d21_8pbhead->d2_errclass =
parblk->d21_8pbhead->d2_errvalue = D2_NO_ERR;
ret_value = D2_NOERROR;
ret_value1 = D2_NOERROR;

  /*
     ... and some other variables
  */
call_head.d2_retcode = call_head.d2_errclass =
			 call_head.d2_errvalue = D2_NO_ERR;
call_head.d2_version = parblk->d21_8pbhead->d2_version;
m_b_id = s_b_id = parblk->d21_8bind_id;

  /*
  Check if Entry-Name of the Object is specified
  */
if (parblk->d21_8entry == NULL)
  {
    parblk->d21_8pbhead->d2_retcode = D2_CALL_ERR;
    parblk->d21_8pbhead->d2_errclass = D2_PAR_ERR;
    parblk->d21_8pbhead->d2_errvalue = D21_NO_ENTRYNAME;
    DUA_TRACE_IAPL_EXIT(parblk->d21_8pbhead->d2_errvalue)
    return (D2_ERROR);
  }

if (!D2_CHECK_MINMAX(D21MIN1,D21MAX1,parblk->d21_8pbhead->d2_version))
	{			/* illegal version supplied		*/
	parblk->d21_8pbhead->d2_retcode = D2_CALL_ERR;
	parblk->d21_8pbhead->d2_errclass = D2_ILL_VERS;
	parblk->d21_8pbhead->d2_errvalue = D2_NOT_SPEC;
	DUA_TRACE_IAPL_EXIT(parblk->d21_8pbhead->d2_errvalue)
	return (D2_ERROR);
	}

  /*
  Check if Service-Controls correct
  */
ret_value1 = d21_ca4_check_sc(parblk->d21_8c_a);

if (ret_value1 != D2_NOERROR)
  {
    parblk->d21_8pbhead->d2_retcode = D2_CALL_ERR;
    parblk->d21_8pbhead->d2_errclass = D2_PAR_ERR;
    parblk->d21_8pbhead->d2_errvalue = ret_value1;
    DUA_TRACE_IAPL_EXIT(parblk->d21_8pbhead->d2_errvalue)
    return (D2_ERROR);
  }
sc = parblk->d21_8c_a->d2_serv_cntrls;

# ifdef OSFCDS
    /* 	Check to see if this naming operation should be 
	carried out in the CDS name space.  	
								*/
    if (d21_cds_switch (parblk->d21_8entry) == DSW_NSPACE) {
	ret_value = d21_8_switch (parblk);
	return (ret_value);
    }
# endif /* OSFCDS */

  /*
  Check if Bind-ID okay and get the Bind-Information
  */
  if (sc & D2_USEDSA)
  {
  /* now  only a copy of the bind table entry is needed; it will
     be checked later if ipc connection of directory bind can be used or not */
  if (d21_bh8_get_bind_info(parblk->d21_8bind_id,&b_tab,&new_ipc,
				parblk->d21_8pbhead,FALSE) != D2_NOERROR)
    {
    DUA_TRACE_IAPL_EXIT(parblk->d21_8pbhead->d2_errvalue)
    return (D2_ERROR);
    }
	
  if (b_tab.d21_b_syntax != D27_ASN1_TS)
    {
    adm_syntax = TRUE;
    }
  } /* if (use dsa) */

  /*
  Find the Length of the Request-APDU 
  */
buf_len = d21_ca_len_inremove(parblk);

/*
if (buf_len < b_tab.d21_b_ipcinf->d21_i_lenms) 
    {
    buf_len = b_tab.d21_b_ipcinf->d21_i_lenms; 
    }
*/
  /*
  Allocate memory for the Invoke-APDU and result-APDU
  */
res_len = D2_DNL_MAX;
if ((req_buf = D27_MALLOC(svc_c_sev_warning,buf_len)) == NULL ||
    (res_buf = D27_MALLOC(svc_c_sev_warning,res_len)) == NULL)
  {
    parblk->d21_8pbhead->d2_retcode = D2_TEMP_ERR;
    parblk->d21_8pbhead->d2_errclass = D2_SYS_ERR;
    parblk->d21_8pbhead->d2_errvalue = D21_ALLOC_ERROR;
    DUA_TRACE_IAPL_EXIT(parblk->d21_8pbhead->d2_errvalue)
    return (D2_ERROR);
  }
  /*
  Generate the corresponding Invoke-APDU 
  */
  /*  Set the APDU-Header  */
  inv_apdu = (D23_inremobjt *) req_buf;
  inv_apdu->d23_Phdr.d23_versno = D23_APDUV22;
  if ((parblk->d21_8c_a->d2_serv_cntrls & D2_DUACACHE) == D2_DUACACHE)
  {  inv_apdu->d23_Phdr.d23_dirid = parblk->d21_8bind_id;
     inv_apdu->d23_Phdr.d23_bindid = 0;
  }else{
     inv_apdu->d23_Phdr.d23_dirid = b_tab.d21_b_dir_id;
     inv_apdu->d23_Phdr.d23_bindid = 0;
  }
  req_end = (char *) (inv_apdu +1);

  d21_cm03_build_comarg( parblk->d21_8c_a, &req_end);

  /*  Copy the Entry-Name of the Object  */
  d21_cab_dsname_iput (parblk->d21_8entry, req_end, &req_end);
  /*  Copy the End of Message Tag  */
  d21_ca9_eom_iput (req_end, &req_end); 
  /*
  Set the Parameter-Block for Request
  */
  req_info.d21_r_opcode = D23_REMOBJT;
  req_info.d21_r_bufapdu = req_buf;
  req_info.d21_r_lenapdu = (req_end - req_buf);
  /*
  Check if Object shall be removed from DUA-Cache or DSA-DIB
  */
  if ((parblk->d21_8c_a->d2_serv_cntrls & D2_DUACACHE) == D2_DUACACHE)
	{
	DUA_TRACE_OPMODE_CACHE_MODIFY
	/*
	Establish an Association to DUA_Cache
	*/
	if (d21_bh4_i_bind (D23_DCASID,&ipc_tbl,parblk->d21_8pbhead)
			!= D2_NOERROR)
		{
		free (req_buf);
		free (res_buf);
		DUA_TRACE_IAPL_EXIT(parblk->d21_8pbhead->d2_errvalue)
		return (D2_ERROR);
		}
	/*
	Send the Request to DUA-Cache
	*/
	req_info.d21_r_ipcinfo = ipc_tbl;
	if (d21_send_invoke (&req_info,ipc_tbl->d21_i_assid,
				 parblk->d21_8pbhead) == D2_ERROR)
		{
		free (res_buf);
		free (req_buf);
		DUA_TRACE_IAPL_EXIT(parblk->d21_8pbhead->d2_errvalue)
		return (D2_ERROR);
		}
	/*
	Receive Result from DUA-Cache
	*/
	req_info.d21_r_lenapdu = buf_len;
	ret_value = d21_recv_result (&req_info, ipc_tbl->d21_i_assid,
							parblk->d21_8pbhead);
	/*
	Release the Association to DUA_Cache
	*/
	d21_bh5_i_unbind (ipc_tbl->d21_i_assid);
	/*
	Check if REMOVE Object from DUA_Cache okay
	*/
	if ((ret_value == D2_ERROR)||(req_info.d21_r_opcode == D23_RERROR))
		{
		d21_cac_copy_error(req_buf, parblk->d21_8pbhead,
				   &parblk->d21_8err);
		ret_value = D2_ERROR;
		}
	else    {
		ret_value = D2_NOERROR;
		}
	free (res_buf);
	free (req_buf);
	DUA_TRACE_IAPL_EXIT(parblk->d21_8pbhead->d2_errvalue)
	return(ret_value);
	}  /* end of add to DUA-Cache */

if ( !((parblk->d21_8c_a->d2_serv_cntrls & D2_PREF_ADM_FUN) && adm_syntax) )
  {
  opmode = D21_STD_UPDATE;
  }
else
  {
  if ( (parblk->d21_8c_a->d2_serv_cntrls & D2_LOCALSCOPE) == D2_LOCALSCOPE)
    {
    DUA_TRACE_OPMODE_SHADOW_MODIFY
    }

  /*
  does Bind-DSA hold master-copy or shadow ?
	  if BIND-DSA holds MASTER-Entry :
	  d21_m02 will return  D21_BOUND_TO_MASTER_DSA
  */
  ret_value1 = d21_m02_bind_to_master_dsa(&call_head,
			         parblk->d21_8c_a->d2_serv_cntrls,
			         parblk->d21_8bind_id,
			         b_tab.d21_b_name_dsa,
			         parblk->d21_8entry,
			         &b_tab, (D2_name_string) NULL,
			         (D2_name_string)master_dsa,
			         &m_b_id, &err_info, &alias_flag);

  if (alias_flag == TRUE)
  {   /* set opmode */
      opmode = D21_ALIAS_UPDATE ;
      DUA_TRACE_OPMODE_ALIAS_MODIFY
  }
  else
    {   /*
        delete SHADOW at BIND-DSA requested ?
        */
      if ((parblk->d21_8c_a->d2_serv_cntrls & D2_LOCALSCOPE) == D2_LOCALSCOPE)
      {   if (ret_value1 > D21_MASTER_DSA_OUT_OF_SCOPE)
	  {   /* map returncodes of d21_m02 */
	      d21_m04_map_rc(ret_value1,&call_head,
			     parblk->d21_8pbhead, err_info);
	      DUA_TRACE_IAPL_EXIT(parblk->d21_8pbhead->d2_errvalue)
	      free (res_buf);
	      free (req_buf);
	      parblk->d21_8err = err_info;
	      return(D2_ERROR);
	  }
  
	  if (ret_value1 == D21_BOUND_TO_MASTER_DSA)  {
	      parblk->d21_8pbhead->d2_retcode = D2_CALL_ERR;
	      parblk->d21_8pbhead->d2_errclass = D2_PAR_ERR;
	      parblk->d21_8pbhead->d2_errvalue = D2_NO_SHADOW;
	      DUA_TRACE_IAPL_EXIT(parblk->d21_8pbhead->d2_errvalue)
	      free (res_buf);
	      free (req_buf);
	      return(D2_ERROR);
	  }
  
	  DUA_TRACE_OPMODE_SHADOW_MODIFY
	  /* set opmode */
	  opmode = D21_SHADOW_UPDATE ;
      }
      else
      {   if (ret_value1 != D21_BOUND_TO_MASTER_DSA)
	  {   /* map returncodes of d21_m02 */
	      d21_m04_map_rc(ret_value1,&call_head,
			     parblk->d21_8pbhead, err_info);
	      DUA_TRACE_IAPL_EXIT(parblk->d21_8pbhead->d2_errvalue)
	      free (res_buf);
	      free (req_buf);
	      parblk->d21_8err = err_info;
	      return(D2_ERROR);
	  }

	  if (master_dsa[0] == D2_EOS) {
		DUA_TRACE_OPMODE_ONE_MODIFY
	      opmode = D21_ONE_UPDATE ;
	  } else {
	      /*
	      bind to master-dsa of superior
	      */
	      /* get DN of superior entry  */
	      d21_c30_get_sup(parblk->d21_8entry,(D2_name_string)sup_name);
  
	      s_b_id = m_b_id;
	      ret_value1 = d21_m02_bind_to_master_dsa(&call_head,
					    parblk->d21_8c_a->d2_serv_cntrls,
					    parblk->d21_8bind_id,
					    b_tab.d21_b_name_dsa,
					    (D2_name_string)sup_name,
					    &b_tab, (D2_name_string)master_dsa,
					    (D2_name_string)s_master_dsa,
					    &s_b_id, &err_info,&alias_flag);
  
	      if (ret_value1 != D21_BOUND_TO_MASTER_DSA)
	      {   /* map returncodes of d21_m02 */
		  d21_m04_map_rc(ret_value1,&call_head,
			         parblk->d21_8pbhead, err_info);
		  DUA_TRACE_IAPL_EXIT(parblk->d21_8pbhead->d2_errvalue)
			   /* unbind from master-dsa of entry */
		  d21_m03_unbind(m_b_id, parblk->d21_8bind_id) ;
		  free (res_buf);
		  free (req_buf);
		  parblk->d21_8err = err_info;
		  return(D2_ERROR);
	      }

	      if (s_b_id == m_b_id) {
		     DUA_TRACE_OPMODE_ONE_MODIFY
		     opmode = D21_ONE_UPDATE ;
		     s_b_id = parblk->d21_8bind_id;
	      }
	      else {
		     DUA_TRACE_OPMODE_TWO_MODIFY
		     opmode = D21_DOUBLE_UPDATE ;
	      }
	  } /* if (master_dsa[0] == D2_EOS) */
      }
    }
  } /* of not STD_UPDATE */

if (d21_bh8_get_bind_info(parblk->d21_8bind_id,&b_tab,&new_ipc,
				parblk->d21_8pbhead,TRUE) != D2_NOERROR)
    {
    DUA_TRACE_IAPL_EXIT(parblk->d21_8pbhead->d2_errvalue)
    return (D2_ERROR);
    }
	
/*
perform the required updates
*/

if (opmode == D21_SHADOW_UPDATE ||
    opmode == D21_ALIAS_UPDATE ||
    opmode == D21_STD_UPDATE )
  {
  switch (opmode)
    {
    case D21_STD_UPDATE:	DUA_TRACE_OPMODE_STD_MODIFY
				break;
    case D21_SHADOW_UPDATE:	DUA_TRACE_OPMODE_SHADOW_MODIFY
				break;
    case D21_ALIAS_UPDATE:	DUA_TRACE_OPMODE_ALIAS_MODIFY
				break;
    }

  ret_value = d21_rm_02_rem_from_dsa(opmode,parblk, &b_tab, req_buf,
				       (req_end - req_buf),
				       &res_buf, &res_len);
  if (ret_value != D2_NOERROR)
    {
    if ( (parblk->d21_8pbhead->d2_errvalue == D2_REF_ERROR) &&
	(sc & D2_NOREFERRAL) )
	{
	/* dont return referral : set D2_UNWILLING-error */
	parblk->d21_8pbhead->d2_retcode  = D2_TEMP_ERR;
	parblk->d21_8pbhead->d2_errclass = D2_REM_ERR;
	parblk->d21_8pbhead->d2_errvalue = D2_UNWILLING;
	}
    else
	{
	d21_cac_copy_error(res_buf,parblk->d21_8pbhead,
							&parblk->d21_8err);
	}
    }
  }
else
  {
  if (m_b_id != parblk->d21_8bind_id)
    {
    if (d21_bh8_get_bind_info(m_b_id,&m_b_tab,&m_new_ipc,
				parblk->d21_8pbhead,TRUE) != D2_NOERROR)
      {
      DUA_TRACE_IAPL_EXIT(parblk->d21_8pbhead->d2_errvalue)
      d21_ipc_clear(parblk->d21_8bind_id,&new_ipc,&b_tab);
      return (D2_ERROR);
      }
    }
  else
    {
    m_b_tab = b_tab;
    }
  ret_value = d21_rm_02_rem_from_dsa(opmode,parblk, &m_b_tab, req_buf,
				       (req_end - req_buf),
				       &res_buf, &res_len);
  d21_m03_unbind(m_b_id,parblk->d21_8bind_id);
  if (ret_value == D2_NOERROR)
    {   
	if (opmode == D21_DOUBLE_UPDATE)
	{  
	  if (s_b_id != parblk->d21_8bind_id)
	    {
  	    if (d21_bh8_get_bind_info(s_b_id,&s_b_tab,&s_new_ipc,
				parblk->d21_8pbhead,TRUE) != D2_NOERROR)
    		{
    		DUA_TRACE_IAPL_EXIT(parblk->d21_8pbhead->d2_errvalue)
    		d21_ipc_clear(parblk->d21_8bind_id,&new_ipc,&b_tab);
    		if (m_b_id != parblk->d21_8bind_id)
				d21_ipc_clear(m_b_id,&m_new_ipc,&m_b_tab);
    		return (D2_ERROR);
    		}
	    }
	  else
	    {
	    s_b_tab = b_tab;
	    }
	    ret_value = d21_rm_02_rem_from_dsa(opmode,parblk,&s_b_tab,req_buf,
					       (req_end - req_buf),
					       &res_buf, &res_len);
	    d21_m03_unbind(s_b_id, parblk->d21_8bind_id);
	}
    }
  }

free (res_buf);
free (req_buf);

DUA_TRACE_IAPL_EXIT(parblk->d21_8pbhead->d2_errvalue)

d21_ipc_clear(parblk->d21_8bind_id,&new_ipc,&b_tab);
if (s_b_id != parblk->d21_8bind_id) d21_ipc_clear(s_b_id,&s_new_ipc,&s_b_tab);
if (m_b_id != parblk->d21_8bind_id) d21_ipc_clear(m_b_id,&m_new_ipc,&m_b_tab);

return(ret_value);
}
