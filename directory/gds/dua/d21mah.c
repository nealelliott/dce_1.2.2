/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21mah.c,v $
 * Revision 1.1.10.2  1996/02/18  18:17:07  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:39:49  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:54:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:10:09  root]
 * 
 * Revision 1.1.8.4  1994/06/21  14:46:44  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:39:17  marrek]
 * 
 * Revision 1.1.8.3  1994/05/10  15:54:00  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:32:27  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:14:32  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:28:45  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  18:06:03  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:33:27  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  06:21:31  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  10:56:10  marrek]
 * 
 * Revision 1.1.4.4  1993/02/02  10:15:56  marrek
 * 	Fixes for OT 6598
 * 	insert <pthread.h>
 * 	[1993/02/02  09:23:37  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  19:47:50  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:25:20  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  21:32:27  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  13:39:02  marrek]
 * 
 * Revision 1.1.2.2  1992/06/02  03:27:43  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:37:46  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21mah.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:17:07 $";
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
* NAME         :  d21mah.c                                           *
*                                                                    *
* AUTHOR       :  J.Forster                                          *
* DATE         :  22.04.88                                           *
*                                                                    *
* datoff *************************************************************
*/

#include <gds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <d21dua.h>

static signed32 d21_ca_calc_modinf_len( D2_a_modinf * );

/*******************************************************/

d2_ret_val d21_m01_check_params(
  D21_pC * parblk)
{
signed16   errval = D2_NO_ERR;
const char function_name[] = "d21_m01";

do
{   /*
       check version
    */
    if (!D2_CHECK_MINMAX(D21MIN1,D21MAX1,parblk->d21_Cpbhead->d2_version))
    {                       /* illegal version supplied             */
	errval = D2_ILL_VERS;
	break;
    }

    /*
       check service controls
    */
    d21_check_comarg(parblk->d21_Cc_a, &errval, function_name,
		     parblk->d21_Cbind_id);
    if (errval != D2_NO_ERR)
    {   /* error in service controls */
	break;
    }
    else
    {
      /*
      Check if Service-Controls correct
      */
      errval = d21_ca4_check_sc(parblk->d21_Cc_a);

      if (errval != D2_NO_ERR)
      {   /* error in service controls */
	  break;
      }
    }

    /*
       check entry name
    */
    if (parblk->d21_Centry != NULL &&
	strlen((char *)parblk->d21_Centry) > D2_DNL_MAX)
    {   errval = D21_BAD_NAME;
	break;
    }
    if (parblk->d21_Centry == NULL)
    {   errval = D21_DN_MISSING;
	break;
    }
}
while (FALSE); /* end of do-while loop */

if (errval != D2_NO_ERR) {
	if (errval != D2_ILL_VERS) {
		parblk->d21_Cpbhead->d2_retcode = D2_CALL_ERR;
		parblk->d21_Cpbhead->d2_errclass = D2_PAR_ERR;
		parblk->d21_Cpbhead->d2_errvalue = errval;
		}
	else    {
		parblk->d21_Cpbhead->d2_retcode = D2_CALL_ERR;
		parblk->d21_Cpbhead->d2_errclass = D2_ILL_VERS;
		parblk->d21_Cpbhead->d2_errvalue = D2_NOT_SPEC;
		}
	return (D2_ERROR);
	}
else    {
	return(D2_NOERROR);
	}
}

/*
DS_BIND to master-dsa of an entry
*/
signed16 d21_m02_bind_to_master_dsa(
  D2_pbhead               *call_head,
  signed32                     call_sc,
  signed16                    call_bid,
  D2_name_string           init_dsa,
  D2_name_string           entry_name,
  d21_bind_info    *bt_ptr,
  D2_name_string           dsa,           /* -> name of DSA to which a    */
					  /*    DS_BIND has been done     */
					  /*    (for object name in update*/
					  /*    operations)               */
					  /*    == NULL, if no reference  */
					  /*       DSA is there (if object*/
					  /*       itself and not superior*/
					  /*       has to be read)        */
  D2_name_string           master_dsa,    /* <- name of master DSA of     */
					  /*    entry_name                */
  signed16                *m_bind_id,     /* <- bind-ID for accessing     */
					  /*    entry_name via IAPL calls */
  D2_error               **err_info,      /* <- error information         */
  Bool                    *is_alias)      /* <- indicator whether entry_  */
					/*    name is an alias or not   */
{
D21_p3          read_pb;
D2_pbhead       head;
D2_c_arg        read_carg;
D2_c_res        com_res;
D2_req_info     read_r_info;
D2_a_type       at_mk;
D2_a_type       at_al;
D2_a_type       aliased_and_mk[2];
D2_a_type       psap_addr;
D2_a_info      *a_info;
D2_name_string  current_dsa;
signed16           current_bid;
D21_p1          bind_pb;
signed16           ret_value;
signed16           at_err;
d2_ret_val ret_bind;

*is_alias = FALSE;
/*
   set current dsa to init_dsa
*/
current_dsa = init_dsa;
current_bid = call_bid;


if(entry_name[0] == D2_EOS)
{                   /* master of root is required                     */
		    /* every DSA is master of root                    */
  strcpy((char *)master_dsa,(char *)current_dsa);
  *m_bind_id = current_bid;
  return(D21_BOUND_TO_MASTER_DSA);
}

at_mk.d2_typ_len = sizeof(D2_MASTER_KNOWLEDGE) - 1;
at_mk.d2_type    = (Ob_id_string)D2_MASTER_KNOWLEDGE;
at_al.d2_typ_len = sizeof(D2_ALIASED_OBJECT) - 1;
at_al.d2_type    = (Ob_id_string)D2_ALIASED_OBJECT;
psap_addr.d2_typ_len = sizeof(D2_PSAP_ADDRESS) - 1;
psap_addr.d2_type    = (Ob_id_string)D2_PSAP_ADDRESS;

/*
   build DS_READ parameter block
*/
head.d2_version     = call_head->d2_version;
read_carg.d2_serv_cntrls = (D2_USEDSA | D2_DONT_STORE);
if ((call_sc & D2_LOCALSCOPE) == D2_LOCALSCOPE)
{   read_carg.d2_serv_cntrls |= D2_LOCALSCOPE ;
    /* check whether bind-dsa holds master-entry or shadow */
}
else
{   read_carg.d2_serv_cntrls |= D2_NOREFERRAL ;
    /* bind to master-dsa of entry even it may be a remote dsa */
}

read_carg.d2_priority    = D2_PRIO_LOW;
read_carg.d2_time_limit  = D2_T_UNLIMITED;
read_carg.d2_size_limit  = 1;
read_carg.d2_al_rdn      = 0;
read_carg.d2_scope_ref   = D2_WORLD_SCOPE;
read_carg.d2_op_progress.d2_nr_phase = D2_NOT_STARTED;
read_carg.d2_op_progress.d2_next_rdn = 0;
read_carg.d2_extension = NULL;
read_r_info.d2_r_val_at  = D2_R_VAL_TYP;
read_r_info.d2_r_no_at   = 1;

read_pb.d21_3pbhead  = &head;
read_pb.d21_3c_a     = &read_carg;
read_pb.d21_3r_info  = &read_r_info;
read_pb.d21_3cres    = &com_res;
read_pb.d21_3res     = (D2_ei_info *) 0;

/*
   DS_BIND parameter block
*/
bind_pb.d21_1pbhead = &head;
bind_pb.d21_1dir_id = bt_ptr->d21_b_dir_id;
bind_pb.d21_1auth_mech = bt_ptr->d21_b_auth_mech;
bind_pb.d21_1u_credent = bt_ptr->d21_b_credent;

*err_info = NULL;

do
{   /*
       read master knowledge of entry
    */
    read_pb.d21_3entry   = entry_name;
    read_pb.d21_3bind_id = current_bid;

    read_carg.d2_serv_cntrls |= D2_DREFALIAS ;
    read_r_info.d2_r_no_at   = 2;
    aliased_and_mk[0] = at_al;
    aliased_and_mk[1] = at_mk;
    read_r_info.d2_r_type_at = aliased_and_mk;

    ret_value = d21_3_read(&read_pb);
    /* reset common used parameters */
    read_carg.d2_serv_cntrls &= ~D2_DREFALIAS ;
    read_r_info.d2_r_no_at   = 1;
    if (ret_value == D2_NOERROR && read_pb.d21_3res != NULL)
    {   /* read returned result :
	   check whether an alias was found
	*/
	if ((read_pb.d21_3res->d2_ei_ai != NULL) &&
	    (!D2_ATTCMP(&read_pb.d21_3res->d2_ei_ai->d2_a_type,
			D2_ALIASED_OBJECT)))
	{   /* object is an alias object */
	    *is_alias = TRUE;
	    *m_bind_id = current_bid;
	    free(read_pb.d21_3res);
	    return(D21_BOUND_TO_MASTER_DSA);
	}

	/* read returned result :
	   master-DSA(entry) == current_dsa ?
	*/
	a_info = read_pb.d21_3res->d2_ei_ai;
	if (a_info == NULL || a_info->d2_a_no_val != 1)
	{   /* no master knowledge returned */
	    master_dsa[0] = D2_EOS;
	    *m_bind_id = current_bid;
	    ret_value = D21_BOUND_TO_MASTER_DSA;
	    break;  /* master-dsa found */
	}
	else
	{   /* master knowledge found */
	    strcpy((char *)master_dsa,
			(char *)a_info->d2_a_val->d2_at_value.d2_obj_name_a_v);
	    if (d21_c82_dsname_cmp(master_dsa, current_dsa) == TRUE)
	    {   /*
		   yes, return name of master-dsa
		*/
		*m_bind_id = current_bid;
		ret_value = D21_BOUND_TO_MASTER_DSA;
		break;  /* master-dsa found */
	    }

	    /*
	       check if reference dsa is there and this DSA (== MK of
	       object) is the same as of superior
	    */

	    if (dsa != (D2_name_string) 0)
	    {   if (d21_c82_dsname_cmp(dsa, master_dsa) == TRUE)
		{   /*
		       yes, return name of master-dsa
		       (bind-Id will not be assigned, because it's the
		       same as for accessing the object itself [it's set
		       before calling this subroutine)
		    */
		    ret_value = D21_BOUND_TO_MASTER_DSA;
		    break;  /* master-dsa found */
		}
	    }

	    /*
	       master knowledge points to another dsa
	    */
	    if ((call_sc & D2_LOCALSCOPE) == D2_LOCALSCOPE)
	    {   /*
		   operation restricted to bind-dsa
		*/
		ret_value = D21_MASTER_DSA_OUT_OF_SCOPE;
		break;
	    }

	    /*
	       read PSAP-ADDRESS of master-dsa from
	       dsa-entry in current-dsa (save name of master-dsa)
	    */
	    read_pb.d21_3entry   = master_dsa;
	    read_r_info.d2_r_type_at = &psap_addr;
	    read_pb.d21_3bind_id = current_bid;
	    free(read_pb.d21_3res);
	    read_pb.d21_3res = (D2_ei_info *) 0;
	    ret_value = d21_3_read(&read_pb);
	    if (ret_value == D2_NOERROR && read_pb.d21_3res != NULL)
	    {   /*
		   psap address from dsa
		*/
		bind_pb.d21_1addr_dsa = (D2_a_value *)
					read_pb.d21_3res->d2_ei_ai ;
	    }
	    else
	    {   /*
		   no psap-address from dsa
		*/
		bind_pb.d21_1addr_dsa = NULL ;
		if (read_pb.d21_3err != NULL)
		{   free(read_pb.d21_3err);
		}
	    }

	    /*
	       unbind from current dsa if neccessary
	    */
	    d21_m03_unbind(current_bid, call_bid);

	    /*
	       try DS_BIND to master_dsa
	    */
	    bind_pb.d21_1name_dsa = master_dsa ;
	    bind_pb.d21_1ret_dsa  = NULL;

	    ret_bind = d21_1_bind(&bind_pb);

	    if (bind_pb.d21_1ret_dsa != NULL)
	      {
	      free(bind_pb.d21_1ret_dsa);
	      }

	    if (ret_bind == D2_ERROR)
	    {   /*
		  error reported from DS_BIND
		*/
		ret_value = D21_BIND_TO_MASTER_DSA_FAILED;
		break;
	    }

	    /*
	       DS_BIND okay
	    */
	    current_bid = *m_bind_id = bind_pb.d21_1bind_id;
	    ret_value = D21_BOUND_TO_MASTER_DSA;
	    break;
	}
    }

    if (ret_value == D2_ERROR && (*err_info = read_pb.d21_3err) != NULL)
    {   /*
	   unbind from current dsa if neccessary
	*/
	d21_m03_unbind(current_bid, call_bid);

	/* any error when reading master knowledge of entry */

	if ((*err_info)->d2_e_type == D2_AT_PROBLEM)
	{   at_err=(*err_info)->d2_e_problem.d2_e_atproblem->d2_ae_prob->d2_ap_prob;
	    if (at_err == D2_ATTRIBUTE_MISSING ||
		at_err == D2_UNDEFINED_ATTR_TYPE)
	    {   /*      no master knowledge info returned  */
		/*      Bind to non SIEMENS_DSA */
		master_dsa[0] = D2_EOS;
		*m_bind_id = current_bid;
		ret_value = D21_BOUND_TO_MASTER_DSA;
		free(*err_info);
		*err_info = NULL;
		break;
	    }
	}
	/*
	   return error problem
	*/
	call_head->d2_retcode = read_pb.d21_3pbhead->d2_retcode;
	call_head->d2_errclass = read_pb.d21_3pbhead->d2_errclass;
	call_head->d2_errvalue = read_pb.d21_3pbhead->d2_errvalue;
	break;
    }

    /*
       any other error from READ
    */
    call_head->d2_retcode = read_pb.d21_3pbhead->d2_retcode;
    call_head->d2_errclass = read_pb.d21_3pbhead->d2_errclass;
    call_head->d2_errvalue = read_pb.d21_3pbhead->d2_errvalue;
    ret_value = D21_READ_ERR;
}
while (FALSE); /* end of do-while loop */

/*
   unbind form current dsa if bind to master-dsa did not succeed
*/
if (ret_value != D21_BOUND_TO_MASTER_DSA)
{   d21_m03_unbind(current_bid,call_bid);
}
if (read_pb.d21_3res != NULL)
{   free(read_pb.d21_3res);
}

return(ret_value);
}


void d21_m03_unbind(
  signed16           current_bind_id,
  signed16           initial_bind_id)
{
D21_p2          unbind_pb;
D2_pbhead       unbind_head;

if (current_bind_id != initial_bind_id)
  {
  unbind_head.d2_version = D21V02;
  unbind_pb.d21_2pbhead = &unbind_head;
  unbind_pb.d21_2bind_id = current_bind_id;
  (void)d21_2_unbind(&unbind_pb);
  }

} /* end of d21_m03_unbind */

void d21_m04_map_rc(
  signed16            rc,
  D2_pbhead       *from,
  D2_pbhead       *to,
  D2_error        *err_info)
{
if (err_info != NULL)
{   /*
       error was returned from DS_READ
       (return err_info at IAPL, set Pbhead of IAPL)
    */
    to->d2_retcode  = from->d2_retcode;
    to->d2_errclass = from->d2_errclass;
    to->d2_errvalue = from->d2_errvalue;
}
else
{   switch(rc)
    {   case D21_MASTER_DSA_OUT_OF_SCOPE :
	case D21_BIND_TO_MASTER_DSA_FAILED :
		    /*
		       log error codes of bind operation
		    */

		    if (from->d2_errvalue == D2_CREDENTIALS_INVALID)
		    {   to->d2_retcode = D2_CALL_ERR;
			to->d2_errclass = D2_PAR_ERR;
			to->d2_errvalue = from->d2_errvalue;
		    }
		    else
		    {   to->d2_retcode = D2_TEMP_ERR;
			to->d2_errclass = D2_REM_ERR;
			to->d2_errvalue = D2_NO_RESPONSE;
		    }
		    break;
	case D21_READ_ERR :
		    switch (from->d2_errvalue)
		    {   case D2_ACCESS_RIGHTS_INSUFFICIENT:
				to->d2_retcode = D2_CALL_ERR;
				to->d2_errclass = D2_PAR_ERR;
				to->d2_errvalue = from->d2_errvalue;
				break;
			case D2_TOO_BUSY:
				to->d2_retcode = D2_TEMP_ERR;
				to->d2_errclass = D2_INT_ERR;
				to->d2_errvalue = D2_TOO_BUSY;
				break;
			case D2_NO_RESPONSE:
				to->d2_retcode = D2_TEMP_ERR;
				to->d2_errclass = D2_REM_ERR;
				to->d2_errvalue = D2_NO_RESPONSE;
				break;
			default:to->d2_retcode = D2_PERM_ERR;
				to->d2_errclass = D2_SYS_ERR;
				to->d2_errvalue = D2_NOT_SPEC;
				break;
		    }
		    break;
       default :    /*
		       log program error
		    */
		    to->d2_retcode = D2_PERM_ERR;
		    to->d2_errclass = D2_SYS_ERR;
		    to->d2_errvalue = D2_NOT_SPEC;
		    break;
    }
}

}

signed16 d21_m06_build_req(
  D21_pC * parblk,
  char * req,
  d21_bind_info * bt_ptr)
{
D23_inmodentr   *mp_head;
D23_md_info     *md_info_ptr;
D2_a_modinf     *mod_ptr;
signed16              ret_value;
signed32              no_val;
Bool	is_attr = TRUE;

/*
build first part of request
*/
mp_head = (D23_inmodentr *) req;
d21_cm02_build_hdr( &mp_head->d23_Nhdr, bt_ptr);
req += D23_S_INMOD;
d21_cm03_build_comarg( parblk->d21_Cc_a, &req);

/*
move Distinguished name
*/
d21_cab_dsname_iput(parblk->d21_Centry, req, &req);

/*
move modification infos
*/
mod_ptr = parblk->d21_Cmod;
while (mod_ptr != NULL)
{   /*
       build md-info - block
    */
    md_info_ptr = (D23_md_info *) req;
    md_info_ptr->d23_mdtag = D23_MODTAG;
    md_info_ptr->d23_mdmod = mod_ptr->d2_m_mod;

    no_val = mod_ptr->d2_m_attr.d2_a_no_val;
    if (mod_ptr->d2_m_mod == D2_DELATT)
    {   /*
	   attribute value will be ignored
	*/
	mod_ptr->d2_m_attr.d2_a_no_val = 0;
	is_attr = FALSE;
    }

    req = (char *) (md_info_ptr + 1);
    /*
       build av_info - block
    */
    if ((ret_value = d21_c22_attr_iput(is_attr,&mod_ptr->d2_m_attr, req, &req))
				       != D2_NOERROR)
    {   mod_ptr->d2_m_attr.d2_a_no_val = no_val;
	return(ret_value);
    }
    mod_ptr->d2_m_attr.d2_a_no_val = no_val;

    /*
       next entry in queue of modifications
    */
    mod_ptr = mod_ptr->d2_m_next;
}

/*
move eom-tag
*/
d21_ca9_eom_iput(req, &req);

return(D2_NO_ERR);
}  /* end of d21_m06_build_req(); */


signed16 d21_m07_exec(
  signed16		opmode, /* STD or not */
  D2_pbhead      *pbhead,
  char           *req_ptr,
  signed32           req_len,
  signed16             opcode,
  char          **res_ptr,
  signed32          *res_len,
  const d21_bind_info  *bt_ptr  ) /* bind table entry */
{
D21_req_info     req_info;
d2_ret_val            ret_value;
signed16            ret_value1 = D2_NO_ERR;

/*
set ipc-request block
*/
req_info.d21_r_opcode = opcode;
if ( opmode != D21_STD_UPDATE )
  {
  req_info.d21_r_opcode += D2_ADM_OPID;
  }
req_info.d21_r_bufapdu = req_ptr;
req_info.d21_r_lenapdu = req_len;
req_info.d21_r_ipcinfo = bt_ptr->d21_b_ipcinf;

/*
send request to DSA
*/
ret_value = d21_send_invoke(&req_info,bt_ptr->d21_b_usr_id,pbhead);
if (ret_value != D2_NOERROR)
{  
    return (D2_ERROR);
}

/*
get result from DSA in result area
*/
req_info.d21_r_bufapdu = *res_ptr;
req_info.d21_r_lenapdu = *res_len;

ret_value = d21_recv_result(&req_info,bt_ptr->d21_b_usr_id,pbhead);

*res_ptr = req_info.d21_r_bufapdu;
*res_len = req_info.d21_r_lenapdu;

/*
set ret_value of function
if error occured, recv_result has set values in pbhead
*/
if ((ret_value == D2_NOERROR) && (req_info.d21_r_opcode == D23_RERROR))
{   ret_value1 = D21_EPDU;
}

return (ret_value1);
}


void d21_m08_proc_epdu(
  signed16 bind_id,
  signed32 serv_cntrls,
  D2_pbhead * head,
  char * res_ptr,
  signed32 res_len,
  D2_error ** err_info)
{
unsigned16          tag;
D23_rserror    *err_ptr;
D23_at_prob    *at_ptr;

err_ptr = (D23_rserror *)res_ptr;
at_ptr  = (D23_at_prob *)(err_ptr + 1);
/*
set error-values
*/
head->d2_retcode  = err_ptr->d23_Zretcod;
head->d2_errclass = err_ptr->d23_Zerrcls;
head->d2_errvalue = err_ptr->d23_Zerrval;

tag     = at_ptr->d23_aptag;
switch ( (unsigned) tag)
  {   case D23_EOMTAG :
		break;
    case D23_APTAG :
    case D23_NAMTAG :
		d21_cac_copy_error(res_ptr, head, err_info);
		break;
    case D23_REFTAG :
		if (serv_cntrls & D2_NOREFERRAL)
		{   /* dont return referral : set D2_UNWILLING-error */
		    head->d2_retcode  = D2_TEMP_ERR;
		    head->d2_errclass = D2_REM_ERR;
		    head->d2_errvalue = D2_UNWILLING;
		}
		else
		{   /* return referral-info to calling application */
		    d21_cac_copy_error(res_ptr, head, err_info);
		}
		break;
    default :   /* unexpected tag */
		head->d2_retcode  = D2_TEMP_ERR;
		head->d2_errclass = D2_SYS_ERR;
		head->d2_errvalue = D21_RESULTAPDU_INVALID;
		break;
  }

} /* end of d21_m08_proc_epdu */


/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d21_ca_len_inmodify()                              */
/*                                                                    */
/* AUTHOR       :  G. Loose                                           */
/* DATE         :  25.09.90                                           */
/*                                                                    */
/* SYNTAX       :  signed32 d21_ca_len_inmodify(parblk)                    */
/*                    D21_pC *parblk;                                 */
/*                                                                    */
/* DESCRIPTION  :  to calculate length needed to make APDU for        */
/*                 MODIFY REQUEST in DUA.                             */
/*                                                                    */
/* INPUT-PARAMETER :  parblk : ptr to parameter block                 */
/*                                                                    */
/* OUTPUT-PARAMETER :  none                                           */
/*                                                                    */
/* RETURN-VALUE :  calculated length                                  */
/*                                                                    */
/* COMMENTS :                                                         */
/*                                                                    */
/* exoff **************************************************************/
/*                                                                    */

signed32 d21_ca_len_inmodify(
  D21_pC * parblk)
{
signed32 d23_len,len;

d23_len = D23_S_INMOD + D23_S_EOM + d21_ca_calc_carglen(parblk->d21_Cc_a);

if (parblk->d21_Centry != NULL) {
    d23_len += D23_S_NM + D2_ALIGN(strlen ((char *) parblk->d21_Centry) + 1);
}

if ((len = d21_ca_calc_modinf_len(parblk->d21_Cmod)) == D2_ERROR)
	return(D2_ERROR);

d23_len += len;

return(D2_ALIGN(d23_len));
}


/* inon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d21_ca_calc_modinf_len()                           */
/*                                                                    */
/* AUTHOR       :  G. Loose                                           */
/* DATE         :  25.09.90                                           */
/*                                                                    */
/* SYNTAX       :  signed32 d21_ca_calc_modinf_len(parblk)                 */
/*                    D2_a_modinf *parblk;                            */
/*                                                                    */
/* DESCRIPTION  :  to calculate length needed to make APDU for        */
/*                 Modify-Entry_Information in DUA.                   */
/*                                                                    */
/* INPUT-PARAMETER :  parblk : ptr to parameter block                 */
/*                                                                    */
/* OUTPUT-PARAMETER :  none                                           */
/*                                                                    */
/* RETURN-VALUE :  calculated length                                  */
/*                                                                    */
/* COMMENTS :                                                         */
/*                                                                    */
/* inoff **************************************************************/

static signed32 d21_ca_calc_modinf_len(
  D2_a_modinf * modinf)
{
signed32         d23_len = 0, len;

while (modinf != NULL) {
    d23_len += D23_S_MOD + D23_S_ATT +
	       D2_ALIGN(modinf->d2_m_attr.d2_a_type.d2_typ_len);
    if (modinf->d2_m_mod != D2_DELATT)
    {
	if ((len = d21_ca_calc_av_info(modinf->d2_m_attr.d2_a_val,
			modinf->d2_m_attr.d2_a_no_val)) == D2_ERROR)
		return(D2_ERROR);
	d23_len += len;
    }
    modinf = modinf->d2_m_next;
}

return(d23_len);
}
