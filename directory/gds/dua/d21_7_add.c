/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21_7_add.c,v $
 * Revision 1.1.10.2  1996/02/18  18:16:20  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:38:54  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:52:42  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:09:28  root]
 * 
 * Revision 1.1.8.3  1994/05/10  15:53:38  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:30:41  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:14:20  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:27:46  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:38:12  marrek
 * 	February 1994 code drop
 * 	[1994/02/21  08:44:00  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:29:30  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  05:57:14  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  10:50:10  marrek]
 * 
 * Revision 1.1.4.4  1993/02/02  10:12:28  marrek
 * 	insert <pthread.h> <string.h> <time.h>
 * 	changes for prototyping
 * 	fixes for OT 6554 and 6565
 * 	[1993/02/01  18:04:07  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  19:46:04  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:23:57  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  21:06:49  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  13:33:40  marrek]
 * 
 * Revision 1.1.2.3  1992/06/30  20:58:06  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  20:38:27  melman]
 * 
 * Revision 1.1.2.2  1992/06/02  03:10:48  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:35:57  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21_7_add.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:16:20 $";
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
/* NAME         :  d21_7_add.c                                        */
/*                                                                    */
/* AUTHOR       :  Albert Biber                                       */
/* DATE         :  02.02.88                                           */
/*                                                                    */
/* DESCRIPTION  :  This module contain the DUA function               */
/*                 DS_ADD_OBJECT which allows the DUA-Application     */
/*                 to add an object either to the DIB of the DSA      */
/*                 or to the DUA-cache database.                      */
/*                                                                    */
/*                                                                    */
/* Vers.Nr. |   Date   |   Updates                   | KZ | CR# FM#   */
/*datoff **************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <d21dua.h>
#ifdef OSFCDS
#include <d21_cdssw.h>
#endif

/********** LOCAL ***********/

static signed32 d21_ca_len_inadd( D21_p7 * );

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d21_7_add_object                                   */
/*                                                                    */
/* AUTHOR       :  Albert Biber,  Softlab GmbH.                       */
/* DATE         :  18.01.88                                           */
/*                                                                    */
/* SYNTAX       :  d2_ret_val d21_7_add_object (parblk)                    */
/*                 D21_p7  *parblk;                                   */
/*                                                                    */
/*                                                                    */
/* DESCRIPTION  :  The function d21_7_add_object allows the appli-    */
/*                 cation to insert an object into the DIB of a       */
/*                 previous binded DSA or to the database of the      */
/*                 local DUA-Cache.                                   */
/*                                                                    */
/* INPUT-PARAMETER :                                                  */
/*                                                                    */
/* OUTPUT-PARAMETER :                                                 */
/*                                                                    */
/* RETURN-VALUE :    D2_NOERROR   Function terminated successfully     */
/*                   D2_ERROR    Function terminated with error       */
/*                                                                    */
/*                                                                    */
/*exoff ***************************************************************/

d2_ret_val d21_7_add_object (
  D21_p7 *parblk)
{
d21_bind_info b_tab;
d21_ipc_info *ipc_tbl;
D21_req_info req_info;
D23_inaddobjt *inv_apdu;
char         *req_buf;
char          *res_buf ;
char          *req_end;
d2_ret_val           ret_value;
signed16           ret_value1 = D2_NO_ERR;
signed32        res_len ;
signed32           buf_len;
signed16           opmode;
signed32        req_len;
D2_a_info     *mk_ptr;
D21_p3	readpb;

const char function_name[] = "d21_7_add";
signed32            sc;
signed32           m_k=0 ;
signed16           s_b_id;
d21_bind_info s_b_tab;
signed16           m_b_id;
d21_bind_info m_b_tab;
D2_a_info       mk_info ;
D2_a_value      mk_val ;
Bool		adm_syntax = FALSE;
Bool		found;
Bool		no_sni;
Bool new_ipc = FALSE;
Bool m_new_ipc = FALSE;
Bool s_new_ipc = FALSE;

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
d21_L04_trace_iapl_input(parblk->d21_7entry_name,parblk->d21_7c_a);

ret_value = D2_NOERROR;
parblk->d21_7pbhead->d2_retcode = parblk->d21_7pbhead->d2_errclass =
				parblk->d21_7pbhead->d2_errvalue = D2_NO_ERR;

parblk->d21_7err = NULL;

s_b_id = m_b_id = parblk->d21_7bind_id;
/*
Check if new Entry-Name specified
*/
if (parblk->d21_7entry_name == NULL)
{
  parblk->d21_7pbhead->d2_retcode = D2_CALL_ERR;
  parblk->d21_7pbhead->d2_errclass = D2_PAR_ERR;
  parblk->d21_7pbhead->d2_errvalue = D21_NO_ENTRYNAME;
  DUA_TRACE_IAPL_EXIT(parblk->d21_7pbhead->d2_errvalue)
  return (D2_ERROR);
}

if (!D2_CHECK_MINMAX(D21V022,D21V022,parblk->d21_7pbhead->d2_version))
	{			/* illegal version supplied		*/
	parblk->d21_7pbhead->d2_retcode = D2_CALL_ERR;
	parblk->d21_7pbhead->d2_errclass = D2_ILL_VERS;
	parblk->d21_7pbhead->d2_errvalue = D2_NOT_SPEC;
	DUA_TRACE_IAPL_EXIT(parblk->d21_7pbhead->d2_errvalue)
	return (D2_ERROR);
	}

  /*
  Check if Service-Controls correct
  */
  ret_value1 = d21_ca4_check_sc(parblk->d21_7c_a);

  if (ret_value1 != D2_NO_ERR)
  {
    parblk->d21_7pbhead->d2_retcode = D2_CALL_ERR;
    parblk->d21_7pbhead->d2_errclass = D2_PAR_ERR;
    parblk->d21_7pbhead->d2_errvalue = ret_value1;
    DUA_TRACE_IAPL_EXIT(parblk->d21_7pbhead->d2_errvalue)
    return (D2_ERROR);
  }
  sc = parblk->d21_7c_a->d2_serv_cntrls;

# ifdef OSFCDS
    /* 	Check to see if this naming operation should be 
	carried out in the CDS name space.  	
								*/
    if ((ret_value = d21_cds_switch (parblk->d21_7entry_name)) == DSW_NSPACE) {
	ret_value = d21_7_switch (parblk);
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
  if (d21_bh8_get_bind_info(parblk->d21_7bind_id,&b_tab,&new_ipc,
				parblk->d21_7pbhead,FALSE) != D2_NOERROR)
    {
    DUA_TRACE_IAPL_EXIT(parblk->d21_7pbhead->d2_errvalue)
    return (D2_ERROR);
    }
	
  if (b_tab.d21_b_syntax != D27_ASN1_TS)
    {
    adm_syntax = TRUE;
    }

  /*
  check whether master-knowledge supplied or not,
  and whether it points to bind-dsa or not
  */
  m_k = d21_ad_01_check_m_k(parblk,&b_tab,&mk_ptr);
  }
  /*
  set opmode derived from service-controls and m_k
  */
if ( (sc & D2_DUACACHE) == D2_DUACACHE)
  {
  opmode = D21_CACHE_ADD ;
  }
else
  {
  if ( !((sc & D2_PREF_ADM_FUN) && adm_syntax) )
    {
    opmode = D21_STD_ADD;
    }
  else
    {
    if ( (sc & D2_NOCACHE) && !(sc & D2_LOCALSCOPE))
      {
      if ((m_k == D21_ALIAS) || (m_k == D21_NO_MK) || (m_k == D21_LOCAL_MK))
	{
	opmode = D21_M_LOCAL ;
	}
      else    
	{
	opmode = D21_M_REMOTE ;
	}
      }
    if ( (sc & D2_LOCALSCOPE) && !(sc & D2_NOCACHE))
      {
      if (m_k != D21_REMOTE_MK && m_k != D21_ALIAS)
	{
			/* Shadow must have master-knowledge attribute
			   and it must not point to bind-dsa */
	parblk->d21_7pbhead->d2_retcode = D2_CALL_ERR;
	parblk->d21_7pbhead->d2_errclass = D2_PAR_ERR;
	parblk->d21_7pbhead->d2_errvalue = D21_BAD_ARGUMENT ;
	DUA_TRACE_IAPL_EXIT(parblk->d21_7pbhead->d2_errvalue)
 	return (D2_ERROR);
	}
      else
	{
	opmode = D21_SHADOW_ADD ;
	}
      }
    }
  }
  /*
  Find the Length of the Request-APDU 
  */
if ((buf_len = d21_ca_len_inadd(parblk)) == D2_ERROR)
  {
    parblk->d21_7pbhead->d2_retcode = D2_CALL_ERR;
    parblk->d21_7pbhead->d2_errclass = D2_PAR_ERR;
    parblk->d21_7pbhead->d2_errvalue = D21_BAD_ARGUMENT;
    DUA_TRACE_IAPL_EXIT(parblk->d21_7pbhead->d2_errvalue)
    return (D2_ERROR);
  }

if (m_k == D21_NO_MK)
  {
		/* add space for mk-attribute */
	buf_len += D23_S_ATT + D23_S_AV;
	buf_len += D2_ALIGN(strlen((char *)b_tab.d21_b_name_dsa) + 1) ;
  }
if (opmode != D21_CACHE_ADD)
  {
    if (buf_len <  b_tab.d21_b_ipcinf->d21_i_lenms)
	buf_len = b_tab.d21_b_ipcinf->d21_i_lenms;
  }
  /*
  Allocate memory for the Invoke-APDU
  */
  if ((req_buf = D27_MALLOC(svc_c_sev_warning,buf_len)) == NULL)   
  {
    parblk->d21_7pbhead->d2_retcode = D2_TEMP_ERR;
    parblk->d21_7pbhead->d2_errclass = D2_SYS_ERR;
    parblk->d21_7pbhead->d2_errvalue = D21_ALLOC_ERROR;
    DUA_TRACE_IAPL_EXIT(parblk->d21_7pbhead->d2_errvalue)
    return (D2_ERROR);
  }
  /*
  Allocate memory for the result-APDU
  */
  if ((res_buf = D27_MALLOC(svc_c_sev_warning,buf_len)) == NULL)
  {
    parblk->d21_7pbhead->d2_retcode = D2_TEMP_ERR;
    parblk->d21_7pbhead->d2_errclass = D2_SYS_ERR;
    parblk->d21_7pbhead->d2_errvalue = D21_ALLOC_ERROR;
    DUA_TRACE_IAPL_EXIT(parblk->d21_7pbhead->d2_errvalue)
    free(req_buf) ;
    return (D2_ERROR);
  }
  res_len = buf_len ;
  /*
  Generate the corresponding Invoke-APDU 
  */
  /*  Set the APDU-Header  */
  inv_apdu = (D23_inaddobjt *) req_buf;
  inv_apdu->d23_Mhdr.d23_versno = D23_APDUV22;

  if (opmode == D21_CACHE_ADD)
  { inv_apdu->d23_Mhdr.d23_dirid = parblk->d21_7bind_id;
    inv_apdu->d23_Mhdr.d23_bindid = 0;
  }else{
    inv_apdu->d23_Mhdr.d23_dirid = b_tab.d21_b_dir_id;
    inv_apdu->d23_Mhdr.d23_bindid = 0;
  }
  req_end = (char *) (inv_apdu +1);

  d21_cm03_build_comarg( parblk->d21_7c_a, &req_end);

  /*  Copy the Entry-Name to the message  */

  d21_cab_dsname_iput (parblk->d21_7entry_name,
				req_end, &req_end);
  /*
  execute the appropriate functions
  */
  switch(opmode) {

	case D21_CACHE_ADD :
		DUA_TRACE_OPMODE_CACHE_MODIFY

		/*  Copy the Entry-Information  */
		ret_value1 = d21_c22_attr_iput (TRUE,parblk->d21_7_at_info,
					req_end, &req_end);
		if (ret_value1 != D2_NO_ERR)
		    {
		     parblk->d21_7pbhead->d2_retcode = D2_CALL_ERR;
		     parblk->d21_7pbhead->d2_errclass = D2_PAR_ERR;
		     parblk->d21_7pbhead->d2_errvalue = ret_value1;
		     ret_value = D2_ERROR;
		     break;
		    }

		/*  Copy the End of Message Tag  */
		d21_ca9_eom_iput (req_end, &req_end);
		req_len = req_end - req_buf ;

			/*  Establish an Association to DUA_Cache  */
		if (d21_bh4_i_bind (D23_DCASID,&ipc_tbl,parblk->d21_7pbhead)
						       != D2_NOERROR) {
			ret_value = D2_ERROR ;
			break ;
			}
			/* Set the Parameter-Block for Request    */
		req_info.d21_r_opcode = D23_ADDOBJT;
		req_info.d21_r_bufapdu = (char *)req_buf;
		req_info.d21_r_lenapdu = (req_end - req_buf);
		req_info.d21_r_ipcinfo = ipc_tbl;
			/* Send the Request to DUA-Cache    */
		if (d21_send_invoke (&req_info, ipc_tbl->d21_i_assid,
					parblk->d21_7pbhead) != D2_NOERROR) {
			ret_value = D2_ERROR;
			break ;
			}
			/* Receive Result from DUA-Cache    */
		req_info.d21_r_lenapdu = buf_len;
		ret_value = d21_recv_result (&req_info, ipc_tbl->d21_i_assid,
							parblk->d21_7pbhead);
			/*    Release the Association to DUA_Cache    */
		d21_bh5_i_unbind (ipc_tbl->d21_i_assid);
			/*    Check if Add-Object to DUA_Cache okay    */
		if ((ret_value != D2_NOERROR) ||
			(req_info.d21_r_opcode == D23_RERROR)) {
			ret_value = D2_ERROR;
			break ;
			}
		break ;  /* end of D21_CACHE */

	case D21_SHADOW_ADD :
	case D21_STD_ADD :
		if (opmode == D21_SHADOW_ADD)
		    DUA_TRACE_OPMODE_SHADOW_MODIFY
		else
		    DUA_TRACE_OPMODE_STD_MODIFY

		if (d21_bh8_get_bind_info(parblk->d21_7bind_id,&b_tab,&new_ipc,
				parblk->d21_7pbhead,TRUE) != D2_NOERROR)
    		  {
		  ret_value = D2_ERROR;
    		  break;
    		  }

		/*  Copy the Entry-Information  */
		ret_value1 = d21_c22_attr_iput (TRUE,parblk->d21_7_at_info,
					req_end, &req_end);
		if (ret_value1 != D2_NO_ERR)
		    {
		     parblk->d21_7pbhead->d2_retcode = D2_CALL_ERR;
		     parblk->d21_7pbhead->d2_errclass = D2_PAR_ERR;
		     parblk->d21_7pbhead->d2_errvalue = ret_value1;
		     ret_value = D2_ERROR;
		     break;
		    }

		/*  Copy the End of Message Tag  */
		d21_ca9_eom_iput (req_end, &req_end);
		req_len = req_end - req_buf ;

		ret_value1 = d21_ad_02_add_to_dsa(opmode, parblk,&b_tab,
			(char *)req_buf,req_len,(char **)&req_buf,&req_len);
		if (ret_value1 != D2_NOERROR)     {
			ret_value = D2_ERROR;
			if (ret_value1 == D21_EPDU)  {
				d21_m08_proc_epdu(
					    parblk->d21_7bind_id,
					    parblk->d21_7c_a->d2_serv_cntrls,
					    parblk->d21_7pbhead,
					    req_buf,req_len,
					    &parblk->d21_7err);
				}
			}

		break ; /* end of D21_SHADOW_ADD, D21_STD_ADD */

	case D21_M_LOCAL :
		ret_value1 = d21_ad_05_sup_master_bind(parblk, &b_tab, &s_b_id);
		if (ret_value1 == D2_ERROR) {
			ret_value = D2_ERROR;
			break ;
			}
		if (ret_value1 == D21_LOCAL_MK)
		  {
		  if(d21_bh8_get_bind_info(parblk->d21_7bind_id,&b_tab,&new_ipc,
				parblk->d21_7pbhead,TRUE) != D2_NOERROR)
    		    {
		    ret_value = D2_ERROR;
    		    break;
    		    }

			/*  Copy the Entry-Information  */
		  ret_value = d21_c22_attr_iput (TRUE,
						parblk->d21_7_at_info,
					req_end, &req_end);
		  if (ret_value != D2_NOERROR)
			   {
			     parblk->d21_7pbhead->d2_retcode = D2_CALL_ERR;
			     parblk->d21_7pbhead->d2_errclass = D2_PAR_ERR;
			     parblk->d21_7pbhead->d2_errvalue = ret_value;
			     ret_value = D2_ERROR;
			     break;
			    }

			/*  Copy the End of Message Tag  */
		  d21_ca9_eom_iput (req_end,&req_end);
		  req_len = req_end - req_buf ;

			/* master of superior in BIND_DSA */
			/* send request to bind_dsa */
		  ret_value1 = d21_ad_02_add_to_dsa(opmode,parblk, &b_tab,
					req_buf, req_len, &req_buf, &req_len) ;
		  if (ret_value1 != D2_NOERROR)  {
				ret_value = D2_ERROR;
				if (ret_value1 == D21_EPDU)  {
					d21_m08_proc_epdu(
					    parblk->d21_7bind_id,
					    parblk->d21_7c_a->d2_serv_cntrls,
					    parblk->d21_7pbhead,
					    req_buf,req_len,
					    &parblk->d21_7err);
					}
				else    {
					ret_value = D2_ERROR;
					}
				}
		  }
		else       /* bound to master of superior */
			   /* add shadow at dsa of superior */
		  {
			/* read parblk object with master_k attribute */
		  ret_value = d21_ad_09_read_mk(parblk->d21_7entry_name,
					s_b_id,parblk,&readpb,&found,&no_sni);
		  if (ret_value == D2_ERROR)
			  {
			  /* error values already set */
			  d21_ad_07_unbind(parblk, s_b_id) ;
			  break;
			  }
		  if(d21_bh8_get_bind_info(parblk->d21_7bind_id,&b_tab,&new_ipc,
				parblk->d21_7pbhead,TRUE) != D2_NOERROR)
    		    {
		    ret_value = D2_ERROR;
    		    break;
    		    }

 
			/*  if object exists and master knowledge not bind dsa:
			    unbind from s_b_id;
			    return (object already exists); */
		  if (found == TRUE)
			  {
			  if((no_sni == TRUE) ||
			     (readpb.d21_3res->d2_ei_ai != NULL
			  && readpb.d21_3res->d2_ei_ai->d2_a_no_val == 1
			  && d21_c82_dsname_cmp(
				readpb.d21_3res->d2_ei_ai->d2_a_val->d2_at_value.d2_obj_name_a_v,
				b_tab.d21_b_name_dsa) != TRUE))
			    {
			    /* unbind */
			    d21_ad_07_unbind(parblk, s_b_id) ;
			    /* object already exists */
			    parblk->d21_7pbhead->d2_retcode=D2_CALL_ERR;
			    parblk->d21_7pbhead->d2_errclass=D2_PAR_ERR;
			    parblk->d21_7pbhead->d2_errvalue=D2_ALREADY_EXISTS;
			    /* break */
			    ret_value = D2_ERROR;
			    break;
			    }
			  }

			/*  Copy the Entry-Information  */
		  if (m_k == D21_NO_MK) {
			   /* no master-knowledge supplied by caller :
			      insert master knowledge
			   */
			   mk_info.d2_a_type.d2_typ_len  =
				    sizeof(D2_MASTER_KNOWLEDGE) - 1 ;
			   mk_info.d2_a_type.d2_type =
					(Ob_id_string)D2_MASTER_KNOWLEDGE;
			   mk_info.d2_a_no_val = 1 ;
			   mk_info.d2_a_val    = &mk_val ;
			   if (parblk->d21_7_at_info == NULL) {
			      mk_info.d2_a_next = NULL ;
			   }
			   else    {
			      /* link supplied attributes */
			      mk_info.d2_a_next = parblk->d21_7_at_info ;
			   }
			   mk_val.d2_a_v_len = strlen(
					(char *)b_tab.d21_b_name_dsa) + 1 ;
			   mk_val.d2_a_rep = D2_DISTNAME;
			   mk_val.d2_at_value.d2_t61_a_v = (T61_string)b_tab.d21_b_name_dsa ;
			   /* move entry-information including inserted
			      master-knowledge
			   */
			   ret_value = d21_c22_attr_iput(TRUE,&mk_info,
					req_end, &req_end) ;
			}
		  else  {
			   ret_value = d21_c22_attr_iput (TRUE,
						parblk->d21_7_at_info,
					req_end, &req_end);
			}
		  if (ret_value != D2_NOERROR)
			   {
			     parblk->d21_7pbhead->d2_retcode = D2_CALL_ERR;
			     parblk->d21_7pbhead->d2_errclass = D2_PAR_ERR;
			     parblk->d21_7pbhead->d2_errvalue = ret_value;
			     ret_value = D2_ERROR;
			     d21_ad_07_unbind(parblk, s_b_id) ;
			     break;
			    }

			/*  Copy the End of Message Tag  */
		  d21_ca9_eom_iput (req_end, &req_end);
		  req_len = req_end - req_buf ;

		  if (d21_bh8_get_bind_info(s_b_id,&s_b_tab,&s_new_ipc,
				parblk->d21_7pbhead,TRUE) != D2_NOERROR)
			  {
		  	  ret_value = D2_ERROR;
			  break;
			  }
			  
			/* if object not exists and DSA is SNI-DSA :
				add to master of superior */
		  if (found == FALSE &&
				s_b_tab.d21_b_syntax != D27_ASN1_TS )
			  {
			  ret_value1 = d21_ad_02_add_to_dsa(opmode,parblk,
			     &s_b_tab, req_buf, req_len, &res_buf, &res_len) ;
			  }
				/* unbind from master-dsa of superior */
		  d21_ad_07_unbind(parblk, s_b_id) ;
		  if (ret_value1 != D2_NOERROR)  {
				ret_value = D2_ERROR;
				if (ret_value1 == D21_EPDU)  {
					d21_m08_proc_epdu(
					    parblk->d21_7bind_id,
					    parblk->d21_7c_a->d2_serv_cntrls,
					    parblk->d21_7pbhead,
					    res_buf,res_len,
					    &parblk->d21_7err);
					}
				else    {
					ret_value = D2_ERROR;
					}
				break ;
				}
				/* add of shadow succeeded :
				   add object at bind-dsa */
		  ret_value1 = d21_ad_02_add_to_dsa(opmode,parblk, &b_tab,
					req_buf, req_len, &req_buf, &req_len) ;
		  if (ret_value1 != D2_NOERROR)  {
				ret_value = D2_ERROR;
				if (ret_value1 == D21_EPDU)  {
					d21_m08_proc_epdu(
					    parblk->d21_7bind_id,
					    parblk->d21_7c_a->d2_serv_cntrls,
					    parblk->d21_7pbhead,
					    req_buf,req_len,
					    &parblk->d21_7err);
					}
				else    {
					ret_value = D2_ERROR;
					}
				}

			}
	break ; /* end of D21_M_LOCAL */

	case D21_M_REMOTE :
		/* bind to add-dsa
		   (add-dsa : dsa indicated by given master-knowledge ) */
		ret_value = d21_ad_08_bind(parblk, &b_tab ,
			    (char *)mk_ptr->d2_a_val->d2_at_value.d2_t61_a_v,
			    &m_b_id) ;
		if (ret_value == D2_ERROR)      {
			break ;
			}
  /* now  only a copy of the bind table entry is needed; it will
     be checked later if ipc connection of directory bind can be used or not */
		if (d21_bh8_get_bind_info(m_b_id,&m_b_tab,&m_new_ipc,
				parblk->d21_7pbhead,FALSE) != D2_NOERROR)
		  {
		  ret_value = D2_ERROR;
		  break;
		  }
			  
		/* bind to master-dsa of superior */
		/* master-knowledge will be read in add-DSA !!! */
		ret_value = d21_ad_05_sup_master_bind(parblk,&m_b_tab,&s_b_id);
		if (ret_value == D2_ERROR) {
			/* unbind from add-dsa */
			d21_ad_07_unbind(parblk, m_b_id) ;
			break ;
			}
		if (d21_bh8_get_bind_info(m_b_id,&m_b_tab,&m_new_ipc,
				parblk->d21_7pbhead,TRUE) != D2_NOERROR)
		  {
		  ret_value = D2_ERROR;
		  break;
		  }
			  
		if (ret_value == D21_LOCAL_MK) {
			/* master of superior in add_DSA */
			/* send request to add_dsa */

			/*  Copy the Entry-Information  */
			ret_value = d21_c22_attr_iput (TRUE,
						parblk->d21_7_at_info,
					req_end, &req_end);
			if (ret_value != D2_NOERROR)
			   {
			     parblk->d21_7pbhead->d2_retcode = D2_CALL_ERR;
			     parblk->d21_7pbhead->d2_errclass = D2_PAR_ERR;
			     parblk->d21_7pbhead->d2_errvalue = ret_value;
			     ret_value = D2_ERROR;
			     d21_ad_07_unbind(parblk, m_b_id) ;
			     break;
			    }

			/*  Copy the End of Message Tag  */
			d21_ca9_eom_iput (req_end, &req_end);
			req_len = req_end - req_buf ;

			ret_value1 = d21_ad_02_add_to_dsa(opmode,parblk,
			&m_b_tab, req_buf, req_len, &req_buf, &req_len) ;
			/* unbind from add-DSA */
			d21_ad_07_unbind(parblk, m_b_id) ;
			if (ret_value1 != D2_NOERROR)  {
				ret_value = D2_ERROR;
				if (ret_value1 == D21_EPDU)  {
					d21_m08_proc_epdu(
					    parblk->d21_7bind_id,
					    parblk->d21_7c_a->d2_serv_cntrls,
					    parblk->d21_7pbhead,
					    req_buf,req_len,
					    &parblk->d21_7err);
					}
				else    {
					ret_value = D2_ERROR;
					}
				}
			}
		else    {  /* bound to master of superior */
			   /* add shadow at dsa of superior */

			/*  Copy the Entry-Information  */
			if (m_k == D21_NO_MK) {
			   /* no master-knowledge supplied by caller :
			      insert master knowledge
			   */
			   mk_info.d2_a_type.d2_typ_len  =
				    sizeof(D2_MASTER_KNOWLEDGE) - 1 ;
			   mk_info.d2_a_type.d2_type =
					(Ob_id_string)D2_MASTER_KNOWLEDGE;
			   mk_info.d2_a_no_val = 1 ;
			   mk_info.d2_a_val    = &mk_val ;
			   if (parblk->d21_7_at_info == NULL) {
			      mk_info.d2_a_next = NULL ;
			   }
			   else    {
			      /* link supplied attributes */
			      mk_info.d2_a_next = parblk->d21_7_at_info;
			   }
			   mk_val.d2_a_v_len = strlen(
					(char *)b_tab.d21_b_name_dsa) + 1 ;
			   mk_val.d2_a_rep = D2_DISTNAME;
			   mk_val.d2_at_value.d2_t61_a_v = (T61_string)b_tab.d21_b_name_dsa ;
			   /* move entry-information including inserted
			      master-knowledge
			   */
			   ret_value = d21_c22_attr_iput(TRUE,&mk_info,
					req_end, &req_end) ;
			}
			else  {
			   ret_value = d21_c22_attr_iput (TRUE,
						parblk->d21_7_at_info,
					req_end, &req_end);
			}
			if (ret_value != D2_NOERROR)
			   {
			     parblk->d21_7pbhead->d2_retcode = D2_CALL_ERR;
			     parblk->d21_7pbhead->d2_errclass = D2_PAR_ERR;
			     parblk->d21_7pbhead->d2_errvalue = ret_value;
			     ret_value = D2_ERROR;
			     d21_ad_07_unbind(parblk, s_b_id) ;
			     break;
			    }

			/*  Copy the End of Message Tag  */
			d21_ca9_eom_iput (req_end, &req_end);
			req_len = req_end - req_buf ;

			if (d21_bh8_get_bind_info(s_b_id,&s_b_tab,&s_new_ipc,
				parblk->d21_7pbhead,TRUE) != D2_NOERROR)
		    	  {
		  	  ret_value = D2_ERROR;
		  	  break;
		  	  }
			  
			ret_value1 = d21_ad_02_add_to_dsa(opmode,parblk,
			&s_b_tab, req_buf, req_len, &res_buf, &res_len) ;
				/* unbind from master-dsa of superior */
			d21_ad_07_unbind(parblk, s_b_id) ;
			if (ret_value1 != D2_NOERROR)  {
				/* unbind from add-dsa */
				d21_ad_07_unbind(parblk, m_b_id) ;
				ret_value = D2_ERROR;
				if (ret_value1 == D21_EPDU)  {
					d21_m08_proc_epdu(
					    parblk->d21_7bind_id,
					    parblk->d21_7c_a->d2_serv_cntrls,
					    parblk->d21_7pbhead,
					    res_buf,res_len,
					    &parblk->d21_7err);
					}
				else    {
					ret_value = D2_ERROR;
					}
				break ;
				}
				/* add of shadow succeeded :
				   add object at bind-dsa */
			ret_value1=d21_ad_02_add_to_dsa(opmode,parblk,&m_b_tab,
					req_buf, req_len, &req_buf, &req_len) ;
			/* unbind from add-dsa , its all done */
			d21_ad_07_unbind(parblk, m_b_id) ;
			if (ret_value1 != D2_NOERROR)  {
				ret_value = D2_ERROR;
				if (ret_value1 == D21_EPDU)  {
					d21_m08_proc_epdu(
					    parblk->d21_7bind_id,
					    parblk->d21_7c_a->d2_serv_cntrls,
					    parblk->d21_7pbhead,
					    req_buf,req_len,
					    &parblk->d21_7err);
					}
				else    {
					ret_value = D2_ERROR;
					}
				}

			}
	break ; /* end of D21_M_REMOTE */

	default :
		DUA_TRACE_UNKNOWN_OPMODE
		 parblk->d21_7pbhead->d2_retcode = D2_PERM_ERR;
		 parblk->d21_7pbhead->d2_errclass = D2_SYS_ERR;
		 parblk->d21_7pbhead->d2_errvalue = D2_NOT_SPEC;
		ret_value = D2_ERROR ;

  } /* end of switch(opmode) */
free(req_buf);
free(res_buf);

d21_ipc_clear(parblk->d21_7bind_id,&new_ipc,&b_tab);
if (s_b_id != parblk->d21_7bind_id) d21_ipc_clear(s_b_id,&s_new_ipc,&s_b_tab);
if (m_b_id != parblk->d21_7bind_id) d21_ipc_clear(m_b_id,&m_new_ipc,&m_b_tab);

DUA_TRACE_IAPL_EXIT(parblk->d21_7pbhead->d2_errvalue)

return (ret_value);
}                                     /*  end d21_7_add_object  */

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d21_ca_len_inadd()                                 */
/*                                                                    */
/* AUTHOR       :  G. Loose                                           */
/* DATE         :  25.09.90                                           */
/*                                                                    */
/* DESCRIPTION  :  to calculate length needed to make APDU for        */
/*                 ADD REQUEST in DUA.                                */
/*                                                                    */
/* INPUT-PARAMETER :  parblk : ptr to parameter block                 */
/*                                                                    */
/* OUTPUT-PARAMETER :  none                                           */
/*                                                                    */
/* RETURN-VALUE :  calculated length                                  */
/*                                                                    */
/* exoff **************************************************************/

static signed32 d21_ca_len_inadd(
  D21_p7 * parblk)
{
	signed32 d23_len,len;

	d23_len = D23_S_INADD + D23_S_EOM + d21_ca_calc_carglen(parblk->d21_7c_a);

	if (parblk->d21_7entry_name != NULL) {
		d23_len += D23_S_NM
		+ D2_ALIGN(1+strlen ((char *) parblk->d21_7entry_name));
	}

	if ((len = d21_ca_calc_atinfo_len(parblk->d21_7_at_info)) == D2_ERROR)
		return(D2_ERROR);
	d23_len += len;
	return(D2_ALIGN(d23_len));
}

