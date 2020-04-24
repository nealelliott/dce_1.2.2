/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21adh.c,v $
 * Revision 1.1.10.2  1996/02/18  18:16:41  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:39:12  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:53:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:09:43  root]
 * 
 * Revision 1.1.8.4  1994/06/21  14:46:36  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:38:36  marrek]
 * 
 * Revision 1.1.8.3  1994/05/10  15:53:45  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:31:15  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:14:24  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:27:59  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:38:29  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:30:51  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  06:05:36  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  10:52:09  marrek]
 * 
 * Revision 1.1.4.4  1993/02/02  10:13:37  marrek
 * 	Fixes for OT 6565
 * 	insert <pthread.h>
 * 	changes for prototyping
 * 	[1993/02/02  08:10:17  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  19:46:39  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:24:28  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  21:09:05  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  13:35:24  marrek]
 * 
 * Revision 1.1.2.3  1992/06/30  20:58:32  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  20:38:56  melman]
 * 
 * Revision 1.1.2.2  1992/06/02  03:15:45  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:36:37  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21adh.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:16:41 $";
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
* TYPE         :  MODULE                                             *
*                                                                    *
* NAME         :  d21adh.c                                           *
*                                                                    *
* AUTHOR       :  Albert Biber                                       *
* DATE         :  22.01.88                                           *
*                                                                    *
* datoff *************************************************************
*/

#include <gds.h>
#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>

#include        <d21dua.h>

/* LOCAL VARIABLES */

/**********************************************************/

signed16 d21_ad_01_check_m_k(
  D21_p7                  *parblk,
  d21_bind_info    *bnd_tbl,
  D2_a_info               **mk_ptr)
{
D2_a_info       *at_ptr;
signed16           opmode;

at_ptr = (D2_a_info *)parblk->d21_7_at_info;
opmode = D21_NO_MK ; /* assume : no master knowledge supplied */
while (at_ptr != NULL)
	{
	if (!D2_ATTCMP(&at_ptr->d2_a_type, D2_ALIASED_OBJECT))
	  { *mk_ptr = at_ptr;
	    opmode = D21_ALIAS;
	    break;
	   }
	if (!D2_ATTCMP(&at_ptr->d2_a_type, D2_MASTER_KNOWLEDGE))
		{       /* dsa in m_k = BIND-DSA ? */
		*mk_ptr = at_ptr;
		if (d21_c82_dsname_cmp(at_ptr->d2_a_val->d2_at_value.d2_obj_name_a_v,
				       bnd_tbl->d21_b_name_dsa) == TRUE)
			{
			opmode = D21_LOCAL_MK ;
			}
		else    {
			opmode = D21_REMOTE_MK ;
			}
		break ;
		}
	else
		{
		at_ptr = at_ptr->d2_a_next;
		}
	} /* end of while-loop */

return(opmode);
} /* end of d21_ad_01_check_m_k */


signed16 d21_ad_02_add_to_dsa(
  signed16			  opmode,
  D21_p7                  *parblk,
  d21_bind_info    *bnd_tbl,
  char                    *req_buf,
  signed32                  req_len,
  char                    **res_buf,
  signed32                  *res_len )
{

D21_req_info    req_info;
d2_ret_val           ret_value;
signed16           ret_value1 = D2_NO_ERR;
D23_inaddobjt   *inv_apdu;

const char function_name[] = "d21_ad_02";

    /* set bind-id in apdu */
    inv_apdu = (D23_inaddobjt *) req_buf ;
    inv_apdu->d23_Mhdr.d23_bindid = 0;

    /*
    Set the Parameter-Block for Request
    */
    req_info.d21_r_opcode = (opmode==D21_STD_ADD)? D23_ADDOBJT :
							D2_ADM_OPID+D23_ADDOBJT;
    req_info.d21_r_bufapdu = req_buf;
    req_info.d21_r_lenapdu = req_len;
    req_info.d21_r_ipcinfo = bnd_tbl->d21_b_ipcinf;
    /*
    Send the Request to DSA
    */
    if (d21_send_invoke (&req_info,bnd_tbl->d21_b_usr_id,
					parblk->d21_7pbhead) != D2_NOERROR)
      {
      return (D2_ERROR);
      }
    /*
    Receive Result from DSA in res_buf
    */
    req_info.d21_r_bufapdu = *res_buf;
    req_info.d21_r_lenapdu = *res_len;

    ret_value = d21_recv_result (&req_info, bnd_tbl->d21_b_usr_id,
							parblk->d21_7pbhead);

    *res_buf = req_info.d21_r_bufapdu;
    *res_len = req_info.d21_r_lenapdu;

    /*
    set ret_value of function
    */
    if ((ret_value == D2_NOERROR) && (req_info.d21_r_opcode == D23_RERROR))
	{
	DUA_TRACE_ERROR_PDU_RECEIVED
	ret_value1 = D21_EPDU;
	}
return (ret_value1);
} /* end of d21_ad_02_add_to_dsa */


/************************************************************************/
/*                                                                      */
/*      bind to dsa holding master of superior of object to be added    */
/*              dsa of superior is read in current bind-dsa             */
/*              if master dsa is bind-dsa : return D21_LOCAL_MK         */
/*              else try to bind to master-dsa of superior              */
/*                   if bind succeeded : set master-bind-id (m_b_id)    */
/*                                                                      */
/************************************************************************/


signed16 d21_ad_05_sup_master_bind(
  D21_p7                  *parblk,
  d21_bind_info    *bnd_tbl,
  signed16                   *m_b_id)
{

D21_p3          read_pb;
D2_pbhead       read_head;
D2_c_arg        read_carg;
D2_req_info     read_r_info;
D2_c_res        read_c_res;
D2_a_type       at_mk;
D21_p1          bind_pb;
D2_pbhead       bind_head;
char           sup_name[D2_DNL_MAX];

signed16           rv;
signed16 ret_value;

at_mk.d2_typ_len = sizeof(D2_MASTER_KNOWLEDGE) - 1;
at_mk.d2_type    = (Ob_id_string)D2_MASTER_KNOWLEDGE;
/*
build DS_READ parameter block
*/
read_head.d2_version     = parblk->d21_7pbhead->d2_version;
read_head.d2_retcode     = read_head.d2_errclass = read_head.d2_errvalue
			 = D2_NOERROR;
read_carg.d2_serv_cntrls = (0 | D2_USEDSA | D2_DONT_STORE| D2_NOREFERRAL);
read_carg.d2_time_limit  = D2_T_UNLIMITED;
read_carg.d2_size_limit  = 1;
read_carg.d2_al_rdn = 0;
read_carg.d2_op_progress.d2_nr_phase = D2_NOT_STARTED;
read_carg.d2_op_progress.d2_next_rdn = 0;
read_carg.d2_extension = (D2_extension *) NULL;
read_carg.d2_priority = D2_PRIO_LOW;
read_carg.d2_scope_ref = D2_WORLD_SCOPE;
read_r_info.d2_r_val_at  = D2_R_VAL_TYP;
read_r_info.d2_r_no_at   = 1;
read_r_info.d2_r_type_at = &at_mk;

read_pb.d21_3pbhead  = &read_head;
read_pb.d21_3c_a     = &read_carg;
read_pb.d21_3bind_id = parblk->d21_7bind_id;
read_pb.d21_3r_info  = &read_r_info;

d21_c30_get_sup(parblk->d21_7entry_name, (D2_name_string)sup_name);

if(*sup_name == D2_EOS)
{                       /* first level object is added                */
			/* ADD to first level DSA                     */
  return(D21_LOCAL_MK);
}

read_pb.d21_3entry = (D2_name_string)sup_name;
read_pb.d21_3err = NULL;
read_pb.d21_3res = NULL;
read_pb.d21_3cres = &read_c_res;
/*
read master knowledge of superior entry
*/
ret_value = d21_3_read(&read_pb);
if (ret_value != D2_NOERROR)
	{
	  parblk->d21_7err = read_pb.d21_3err;
	  if  (read_head.d2_errvalue == D2_NFOUND)
		{
		 parblk->d21_7err   =   read_pb.d21_3err;
		 parblk->d21_7pbhead->d2_retcode  = D2_CALL_ERR;
		 parblk->d21_7pbhead->d2_errclass = D2_PAR_ERR;
		 parblk->d21_7pbhead->d2_errvalue = D2_NFOUND;
		 return (D2_ERROR);
		}

	/*      read returned other error     */

	if (read_head.d2_errvalue == D2_ATT_ERROR  && (
	read_pb.d21_3err->d2_e_problem.d2_e_atproblem->d2_ae_prob->d2_ap_prob
					 == D2_ATTRIBUTE_MISSING ||
	read_pb.d21_3err->d2_e_problem.d2_e_atproblem->d2_ae_prob->d2_ap_prob
					 == D2_UNDEFINED_ATTR_TYPE ))
	 {
		free (read_pb.d21_3err);
	       /*      no master knowledge info returned  */
	       /*      ADD to non SIEMENS_DSA */
	       rv = D21_LOCAL_MK;
	   }
	else
	{
	 /*
	 set error-values
	 */
	 switch (read_head.d2_errvalue) {
	  case D2_TOO_BUSY:
	  case D2_NO_RESPONSE:
	  case D2_ACCESS_RIGHTS_INSUFFICIENT:
	       parblk->d21_7pbhead->d2_retcode  = read_head.d2_retcode;
	       parblk->d21_7pbhead->d2_errclass = read_head.d2_errclass;
	       parblk->d21_7pbhead->d2_errvalue = read_head.d2_errvalue;
	       rv = D2_ERROR;
	       break;

	  default:
	       parblk->d21_7pbhead->d2_retcode  = D2_TEMP_ERR;
	       parblk->d21_7pbhead->d2_errclass = D2_SYS_ERR;
	       parblk->d21_7pbhead->d2_errvalue = D2_NOT_SPEC;
	       rv = D2_ERROR;
	       break;
	 } /* switch (errvalue) */
	} /* of else */
	return (rv);
	}
/*
analyze result of read
*/
if ((read_pb.d21_3res->d2_ei_ai == NULL) ||
    (read_pb.d21_3res->d2_ei_ai->d2_a_no_val != 1))
	{
	/*      no master knowledge info returned  */
	/*      ADD to non SIEMENS_DSA */
	rv = D21_LOCAL_MK;
	free(read_pb.d21_3res);
	return (rv);
	}
/*
master dsa of superior ==  bind dsa ?
*/

if (d21_c82_dsname_cmp(read_pb.d21_3res->d2_ei_ai->d2_a_val->d2_at_value.d2_obj_name_a_v
		       , bnd_tbl->d21_b_name_dsa) == TRUE )
	{
	/*  yes, then return D21_LOCAL_MK */
	free(read_pb.d21_3res);
	return(D21_LOCAL_MK);
	}
/*
bind to master dsa of superior
*/
bind_head.d2_version = parblk->d21_7pbhead->d2_version;
bind_head.d2_retcode = bind_head.d2_errclass = bind_head.d2_errvalue = D2_NO_ERR;

bind_pb.d21_1pbhead    = &bind_head;
bind_pb.d21_1auth_mech = bnd_tbl->d21_b_auth_mech;
bind_pb.d21_1dir_id    = bnd_tbl->d21_b_dir_id;
bind_pb.d21_1u_credent = (D2_credent *)bnd_tbl->d21_b_credent ;
bind_pb.d21_1ret_dsa   = NULL;
bind_pb.d21_1name_dsa  = (D2_name_string)
		read_pb.d21_3res->d2_ei_ai->d2_a_val->d2_at_value.d2_obj_name_a_v;
bind_pb.d21_1addr_dsa  =  NULL;

ret_value = d21_1_bind(&bind_pb);
if (bind_pb.d21_1ret_dsa != NULL)
  {
  free(bind_pb.d21_1ret_dsa);
  }
if (ret_value != D2_NOERROR)
	{
	/*
	set error_values
	*/
	parblk->d21_7pbhead->d2_retcode = D2_TEMP_ERR;
	parblk->d21_7pbhead->d2_errclass = D2_REM_ERR;
	parblk->d21_7pbhead->d2_errvalue = D2_NO_RESPONSE;
	free(read_pb.d21_3res);
	return (D2_ERROR);
	}
/*
bind succeeded, set bind_id
*/
free(read_pb.d21_3res);
*m_b_id = bind_pb.d21_1bind_id;
return(D2_NOERROR) ;
}   /* end of d21_ad_05_sup_master_bind  */

/************************************************************************/
/*                                                                      */
/*      unbind from a dsa                                               */
/*                                                                      */
/************************************************************************/

void d21_ad_07_unbind(
  D21_p7  *parblk,
  signed16   bind_id)
{
D21_p2          unbind_pb;
D2_pbhead       unbind_head ;

unbind_head.d2_version = parblk->d21_7pbhead->d2_version;
unbind_pb.d21_2pbhead = &unbind_head;
unbind_pb.d21_2bind_id = bind_id;
d21_2_unbind(&unbind_pb);
}       /* end d21_ad_07_unbind */

/************************************************************************/
/*                                                                      */
/*      bind to dsa which shall hold the mastercopy of the new object   */
/*                                                                      */
/************************************************************************/

d2_ret_val d21_ad_08_bind(
  D21_p7          *parblk,
  d21_bind_info    *bnd_tbl,
  char            *dsa_name,
  signed16           *bind_id)
{
D21_p1          bind_pb;
D2_pbhead       bind_head;
d2_ret_val           ret_value ;
bind_head.d2_version = parblk->d21_7pbhead->d2_version;
bind_head.d2_retcode =
bind_head.d2_errclass =
bind_head.d2_errvalue = D2_NO_ERR;
bind_pb.d21_1pbhead    = &bind_head;
bind_pb.d21_1dir_id    = bnd_tbl->d21_b_dir_id ;
bind_pb.d21_1auth_mech = bnd_tbl->d21_b_auth_mech;
bind_pb.d21_1u_credent = (D2_credent *)bnd_tbl->d21_b_credent ;
bind_pb.d21_1ret_dsa   = NULL;
bind_pb.d21_1name_dsa  = (D2_name_string)dsa_name ;
bind_pb.d21_1addr_dsa  =  NULL ;

ret_value = d21_1_bind(&bind_pb);
if (bind_pb.d21_1ret_dsa != NULL)
  {
  free(bind_pb.d21_1ret_dsa);
  }

if (ret_value == D2_NOERROR)
  {
  *bind_id = bind_pb.d21_1bind_id;
  }
else
  {
  bind_head.d2_version = parblk->d21_7pbhead->d2_version;
  bind_head.d2_retcode =
  bind_head.d2_errclass =
  bind_head.d2_errvalue = D2_NO_ERR;
  bind_pb.d21_1pbhead    = &bind_head;
  bind_pb.d21_1dir_id    = bnd_tbl->d21_b_dir_id ;
  bind_pb.d21_1auth_mech = D2_ANONYMOUS;
  bind_pb.d21_1u_credent = NULL ;
  bind_pb.d21_1ret_dsa   = NULL;
  bind_pb.d21_1name_dsa  = (D2_name_string)dsa_name ;
  bind_pb.d21_1addr_dsa  =  NULL ;

  ret_value = d21_1_bind(&bind_pb);
  if (bind_pb.d21_1ret_dsa != NULL)
    {
    free(bind_pb.d21_1ret_dsa);
    }

  if (ret_value == D2_NOERROR)
    {
    *bind_id = bind_pb.d21_1bind_id;
    }
  }

if (ret_value != D2_NOERROR)
	{
	/*
	set error_values
	*/
	parblk->d21_7pbhead->d2_retcode = D2_TEMP_ERR;
	parblk->d21_7pbhead->d2_errclass = D2_REM_ERR;
	parblk->d21_7pbhead->d2_errvalue = D2_NO_RESPONSE;
	}
return(ret_value) ;
} /* end of d21_ad_08_bind */


d2_ret_val d21_ad_09_read_mk(
  D2_name_string obj_name,
  signed16		b_id,
  D21_p7	*parblk,
  D21_p3	*read_pb,
  Bool		* found,
  Bool		* no_sni)
{
D2_pbhead       read_head;
D2_c_arg        read_carg;
D2_req_info     read_r_info;
D2_c_res        read_c_res;
D2_a_type       at_mk;

d2_ret_val           ret_value;

*found = TRUE;
*no_sni = FALSE;

at_mk.d2_typ_len = sizeof(D2_MASTER_KNOWLEDGE) - 1;
at_mk.d2_type    = (Ob_id_string)D2_MASTER_KNOWLEDGE;
/*
build DS_READ parameter block
*/
read_head.d2_version     = parblk->d21_7pbhead->d2_version;
read_head.d2_retcode     = read_head.d2_errclass = read_head.d2_errvalue
			 = D2_NOERROR;
read_carg.d2_serv_cntrls = (0 | D2_USEDSA | D2_DONT_STORE| D2_NOREFERRAL);
read_carg.d2_time_limit  = D2_T_UNLIMITED;
read_carg.d2_size_limit  = 1;
read_carg.d2_al_rdn = 0;
read_carg.d2_op_progress.d2_nr_phase = D2_NOT_STARTED;
read_carg.d2_op_progress.d2_next_rdn = 0;
read_carg.d2_extension = (D2_extension *) NULL;
read_carg.d2_priority = D2_PRIO_LOW;
read_carg.d2_scope_ref = D2_WORLD_SCOPE;
read_r_info.d2_r_val_at  = D2_R_VAL_TYP;
read_r_info.d2_r_no_at   = 1;
read_r_info.d2_r_type_at = &at_mk;

read_pb->d21_3pbhead  = &read_head;
read_pb->d21_3c_a     = &read_carg;
read_pb->d21_3bind_id = b_id;
read_pb->d21_3r_info  = &read_r_info;

read_pb->d21_3entry = obj_name;
read_pb->d21_3err = NULL;
read_pb->d21_3res = NULL;
read_pb->d21_3cres = &read_c_res;

/* read master knowledge */

ret_value = d21_3_read(read_pb);
if (ret_value != D2_NOERROR)
	{
	  parblk->d21_7err = read_pb->d21_3err;
	  if  (read_head.d2_errvalue == D2_NFOUND)
		{
	/* o.k.; object does not exist and has to be added */
		*found = FALSE;
		 parblk->d21_7pbhead->d2_retcode  = D2_CALL_ERR;
		 parblk->d21_7pbhead->d2_errclass = D2_PAR_ERR;
		 parblk->d21_7pbhead->d2_errvalue = D2_NFOUND;
		 if (read_pb->d21_3err != NULL)
		   {
		   free(read_pb->d21_3err);
		   }
		 return (D2_NOERROR);
		}

	/*      read returned other error     */

	if (read_head.d2_errvalue == D2_ATT_ERROR  && 
				 read_pb->d21_3err != NULL && (
	read_pb->d21_3err->d2_e_problem.d2_e_atproblem->d2_ae_prob->d2_ap_prob
					 == D2_ATTRIBUTE_MISSING ||
	read_pb->d21_3err->d2_e_problem.d2_e_atproblem->d2_ae_prob->d2_ap_prob
					 == D2_UNDEFINED_ATTR_TYPE ))
	 {
		free (read_pb->d21_3err);
	       /*      no master knowledge info returned  */
	       /*       non SIEMENS_DSA */
	       *no_sni = TRUE;
	       return (D2_NOERROR);
	   }
	else
	  {
	  if (read_pb->d21_3err != NULL)
	    {
	    free(read_pb->d21_3err);
	    }
	  /*
	  set error-values
	  */
	  switch (read_head.d2_errvalue) {
	    case D2_TOO_BUSY:
	    case D2_NO_RESPONSE:
	    case D2_ACCESS_RIGHTS_INSUFFICIENT:
	       parblk->d21_7pbhead->d2_retcode  = read_head.d2_retcode;
	       parblk->d21_7pbhead->d2_errclass = read_head.d2_errclass;
	       parblk->d21_7pbhead->d2_errvalue = read_head.d2_errvalue;
	       break;

	    default:
	       parblk->d21_7pbhead->d2_retcode  = D2_TEMP_ERR;
	       parblk->d21_7pbhead->d2_errclass = D2_SYS_ERR;
	       parblk->d21_7pbhead->d2_errvalue = D2_NOT_SPEC;
	       break;
	    } /* switch (errvalue) */
	  return (D2_ERROR);
	  } /* of else */
	} /* of if error from read */

return(D2_NOERROR);
}
