/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21dpsub.c,v $
 * Revision 1.1.10.2  1996/02/18  19:41:58  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:22:39  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:19:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:52:37  root]
 * 
 * Revision 1.1.8.3  1994/05/10  15:50:03  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:14:03  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:06:51  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:13:09  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:35:33  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:35:59  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  10:46:07  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:38:22  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:10:24  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:06:44  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  14:58:53  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  16:12:02  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  19:59:57  melman
 * 	New GDS merged drop
 * 	[1992/05/28  20:43:20  melman]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21dpsub.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:41:58 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* NAME         : d21dpsub.o                                          */
/*								      */
/* AUTHOR       : Schmid ,D AP 11                                     */
/* DATE         : 16.01.89                                            */
/*								      */
/*datoff **************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <dce/d21iapl.h>
#include <gdsext.h>
#include <d2delupd.h>
#include <d2dump.h>
#include <d20proto.h>

/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  d21_16_dump()                                          */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function will dump a given object/subtree into a dump    */
/*      file.                                                         */
/*      It will mask all the objects that are taken from the DSA      */
/*      named in "dsa-name".                                          */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*	signed16     bind_id;					      */
/*      signed16     version;                                         */
/*      Name_string  dsa_name;                                        */
/*      Name_string  object;                                          */
/*      signed32     subset;                                          */
/*      C_arg       *c_arg;                                           */
/*      String       file_name;                                       */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      Errcode     *return_code;       error values                  */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_NOERROR     = The function has been performed successfully.*/
/*      D2_ERROR       = function fails                               */
/*                                                                    */
/*exoff ***************************************************************/

d2_ret_val d21_16_dump(
  signed16	bind_id,
  Errcode * return_code,
  signed16 version,
  Name_string dsa_name,
  Name_string object,
  signed32 subset,
  C_arg * c_arg,
  String file_name)
{
static char function[] = "d21_16_dump";
char        dir_path[D27_LFILE_NAME];
char       *path = dir_path;
D20_pB      pb;
D21_p3	    pb_rd;
D2_pbhead   head;
signed32    dir_id;
D2_c_arg    dump_c_arg;
D2_c_arg    rd_c_arg;
char        shd_log_file[D27_LFILE_NAME];
d2_ret_val  ret_value = D2_NOERROR;
struct stat file_state;
char        obj_norm[D2_DNL_MAX];
char        dsa_norm[D2_DNL_MAX];
char        local_dsa[D2_DNL_MAX];
char        comment[D21_MAX_LOG_COMM];
D2_c_res 	c_res;
D2_a_type       req_att;
D2_req_info    	req_info;

DCE_SVC_LOG((GDS_S_ENTRY_FUNCTION_MSG,function));

req_att.d2_typ_len = sizeof(D2_ALIASED_OBJECT)-1;
req_att.d2_type    = (Ob_id_string) D2_ALIASED_OBJECT;

req_info.d2_r_val_at = D2_R_VAL_TYP;
req_info.d2_r_no_at = 1;
req_info.d2_r_type_at = &req_att;

				/* trace shadowing job                */
d21d010_trace_job(dsa_name, object, subset);

				/* check version, bind-id; get path */
if (d21d006_param_check(bind_id, return_code, version, &path, &dir_id)
			== D2_ERROR)
{   
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,return_code->errvalue));
    return(D2_ERROR);
}
pb.d20_Bdir_id = dir_id;

				/* check object interpretation        */
if (subset != OBJECT && subset != OBJECT_SUBORDINATES)
{   return_code->retcode  = CALLERR;
    return_code->errclass = PARERR;
    return_code->errvalue = DS_INV_SEARCH_SEL;
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,return_code->errvalue));
    return(D2_ERROR);
}

				/* perform bind as DSA                */
if ((ret_value = d21d019_master_bind(bind_id, return_code, path,
			      (D2_name_string)NULL, pb.d20_Bdir_id,
			      &pb.d20_B1bind_id, local_dsa)) == D2_NOERROR)
{   /* check if root of the dump tree is an ALIAS.  */
    /* read the root object and look for ALIASED object attribute type. */

	if (strlen((char *)object) != 0)
		{
		/* object is not ROOT of DIT.	*/

    		/* generate parameter block */
    		head.d2_version = D21V022;
    		pb_rd.d21_3pbhead = &head ;

    		rd_c_arg.d2_extension   = (D2_extension *)NULL;
    		rd_c_arg.d2_op_progress.d2_nr_phase = D2_NOT_STARTED;
    		rd_c_arg.d2_op_progress.d2_next_rdn = 0;
    		rd_c_arg.d2_al_rdn       = 0;
    		rd_c_arg.d2_serv_cntrls  = D2_NOREFERRAL | D2_USEDSA | 
					   D2_DREFALIAS | D2_DONT_STORE | 
					   D2_LOCALSCOPE | D2_PREF_ADM_FUN;
    		rd_c_arg.d2_priority     = D2_PRIO_LOW;
    		rd_c_arg.d2_time_limit   = D2_T_UNLIMITED;
    		rd_c_arg.d2_size_limit   = D2_S_UNLIMITED;
    		rd_c_arg.d2_scope_ref    = D2_WORLD_SCOPE;

    		pb_rd.d21_3c_a = &rd_c_arg;
    		pb_rd.d21_3bind_id = pb.d20_B1bind_id;
    		pb_rd.d21_3entry = object;
    		pb_rd.d21_3r_info = &req_info ;

    		pb_rd.d21_3err = (D2_error *)NULL;
    		pb_rd.d21_3res = (D2_ei_info *) 0;
    		pb_rd.d21_3cres = &c_res;

    		ret_value = d21_3_read(&pb_rd);

    		if (pb_rd.d21_3res != (D2_ei_info *) 0)
    			free((char *) pb_rd.d21_3res);

    		if (ret_value == D2_NOERROR)
    			{   
			/* ALIASED object attribute type exists.        */
			/* object is an ALIAS.                          */
			return_code->retcode  = CALLERR;
			return_code->errclass = PARERR;
			return_code->errvalue = D2_ON_ALIAS_NOT_ALLOWED;
    			DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,
				return_code->errvalue));
			return(D2_ERROR);
    			}
    		else
    			{   
			if (pb_rd.d21_3err->d2_e_type != D2_AT_PROBLEM)
				{   
				free((char *) pb_rd.d21_3err);
	    			return_code->retcode  = PERMERR;
	    			return_code->errclass = SYSERR;
	    			return_code->errvalue = (head.d2_errvalue == 
					D2_NFOUND) ? D2_NFOUND : DS_SEARCH_ERR;
    				DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,
					return_code->errvalue));
	    			return(D2_ERROR);
				}
    			}
		}

    ret_value = D2_NOERROR;
    pb.d20_Bpbhead = &head;
    head.d2_version = DS_V02;
    pb.d20_Bo_file = NULL;
    if (strlen((char *)object) == 0)
    {   pb.d20_Bsubtree = (Name_string)"";
    }
    else
    {   strcpy(obj_norm, (char *)object);
	d20_norm(obj_norm, strlen(obj_norm));
	pb.d20_Bsubtree = (Name_string)obj_norm;
    }
    pb.d20_Bsingle_ob = (subset == OBJECT) ? TRUE : FALSE;
    pb.d20_B1c_a = &dump_c_arg;
    dump_c_arg.d2_serv_cntrls = D2_NOREFERRAL | D2_USEDSA | D2_DREFALIAS |
				D2_DONT_STORE | D2_PREF_ADM_FUN;
    if ((c_arg->serv_cntrls & NOCACHE) == NOCACHE)
    {   dump_c_arg.d2_serv_cntrls |= D2_NOCACHE;
    }
    if ((c_arg->serv_cntrls & LOCALSCOPE) == LOCALSCOPE)
    {   dump_c_arg.d2_serv_cntrls |= D2_LOCALSCOPE;
    }
    dump_c_arg.d2_priority = D2_PRIO_LOW ;
    dump_c_arg.d2_time_limit = D2_T_UNLIMITED ;
    dump_c_arg.d2_size_limit = D2_S_UNLIMITED ;
    pb.d20_Bn_file = file_name;
    strcpy(dsa_norm, (char *)dsa_name);
    d20_norm(dsa_norm, strlen(dsa_norm));
    pb.d20_Bdsa = (Name_string)dsa_norm;

				/* get logfile name for delta updates */
    sprintf(shd_log_file, "%s%s%s%d%s", path, D27_DSA_DIR, D27_SUBDSA_DIR,
	    pb.d20_Bdir_id, D2_SHDLOG);

    for ( ; ; )
    {                           /* dump object/subtree                */
	DCE_SVC_LOG((GDS_S_SUP_DEBUG_INFO_MSG,"before HANDLE_SUBTREE"));
	if                      /* dumping fails                      */
	   (d20_B_handle_subtrees(&pb, (Name_string)NULL))
	{   return_code->retcode  = PERMERR;
	    return_code->errclass = SYSERR;
	    return_code->errvalue = pb.d20_Bpbhead->d2_errvalue == D2_NFOUND
				    ? DS_SEARCH_ERR
				    : pb.d20_Bpbhead->d2_errvalue;
	    ret_value = D2_ERROR;
	    break;
	}
	DCE_SVC_LOG((GDS_S_SUP_DEBUG_INFO_MSG,"after HANDLE_SUBTREE"));

				/* check whether modification were done */
				/* while dumping objects                */
	if (stat(shd_log_file, &file_state) != 0)
	{   if (errno == ENOENT)
	    {                   /* SHD_LOG file not found               */
	        DCE_SVC_LOG((GDS_S_SUP_DEBUG_INFO_MSG,
			"SHDLOG not found --> stop"));
		break;
	    }
	    else
	    {   return_code->retcode  = PERMERR;
		return_code->errclass = SYSERR;
		return_code->errvalue = DS_FILE_ERR;
		ret_value = D2_ERROR;
		DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s%d"),GDS_S_GENERAL,
			svc_c_debug1,GDS_S_STAT_ERR,shd_log_file,errno));
		break;
	    }
	}
	else                    /* check last modification time       */
	{   if (file_state.st_mtime < pb.d20_Bstime ||
		file_state.st_mtime > pb.d20_Betime)
	    {   break;          /* no modifcations                    */
	    }
	    else
	    {                   /* repeat dumping                     */
		sprintf(comment, "Dump (start): %ld", pb.d20_Bstime);
		DCE_SVC_LOG((GDS_S_SUP_DEBUG_INFO_MSG,"comment"));
		sprintf(comment, "Dump (end): %ld", pb.d20_Betime);
		DCE_SVC_LOG((GDS_S_SUP_DEBUG_INFO_MSG,"comment"));
		sprintf(comment, "Mod(SHDLOG): %ld", file_state.st_mtime);
		DCE_SVC_LOG((GDS_S_SUP_DEBUG_INFO_MSG,"comment"));
		DCE_SVC_LOG((GDS_S_SUP_DEBUG_INFO_MSG,
			"SHDLOG modified; repeat dump"));
	    }
	}
    }
				/* unbind                             */
    d21d022_unbind(pb.d20_B1bind_id);
}

DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,return_code->errvalue));

return(ret_value);
}
