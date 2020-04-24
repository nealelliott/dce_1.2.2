/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21gnshd.c,v $
 * Revision 1.1.10.2  1996/02/18  19:42:02  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:22:45  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:20:03  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:52:41  root]
 * 
 * Revision 1.1.8.3  1994/05/10  15:50:08  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:14:17  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:06:54  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:13:23  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:35:37  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:36:36  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  10:49:35  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:39:11  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:10:32  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:06:57  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  14:59:56  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  16:12:53  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:00:23  melman
 * 	New GDS merged drop
 * 	[1992/05/28  20:43:38  melman]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21gnshd.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:42:02 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* NAME         : d21gnshd.o                                          */
/*								      */
/* AUTHOR       : Schmid ,D AP 11                                     */
/* DATE         : 16.01.89                                            */
/*								      */
/*datoff **************************************************************/

#include <gds.h>
#include <stdio.h>
#include <time.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <gdsext.h>
#include <d2delupd.h>
#include <d2dump.h>
#include <d20proto.h>

/**********************************************************************/
/*								      */
/*		   D E C L A R A T I O N S       DATA		      */
/*								      */
/**********************************************************************/

extern D2_pbhead *d20_head;

extern int        d20_dp_fd;    /* file descriptor for reading        */
				/* internal dump file                 */

extern Bool       d21_cre_shd;  /* a flag indicating that shadows have*/
				/* to be created; if shadow creation  */
				/* fails, the original shadowing job  */
				/* file will be used further on       */

/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  d21_17_gnshd()                                         */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function tries to generate a new job in the shadowing    */
/*      job file.                                                     */
/*      Overlapping shadowing jobs will be recognized.                */
/*      If mode == SHD_AND_JOB, the shadwos will be created in the    */
/*      destination DSA.                                              */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*	signed16      bind_id;					      */
/*      signed16      version;                                        */
/*      signed32      mode;         JOB_ONLY or SHD_AND_JOB           */
/*      Name_string   object;       object/subtree name defining      */
/*                                  the shadowing job                 */
/*                                  in case of mode == JOB_ONLY       */
/*      signed32      subset;       object/subtree interpretation     */
/*                                  in case of mode == JOB_ONLY       */
/*      String        file_name;    dump file name                    */
/*      Name_string   dsa_name;     destination DSA                   */
/*      Bool          check;        check overlapping subtrees or not */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      Errcode      *return_code;  error values                      */
/*      Subtree     **subtree;      overlapping subtrees              */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_NOERROR     = The function has been performed successfully.*/
/*      D2_ERROR       = function fails                               */
/*                                                                    */
/*exoff ***************************************************************/

d2_ret_val d21_17_gnshd(
  signed16 bind_id,
  Errcode * return_code,
  signed16 version,
  signed32 mode,
  Name_string object,
  signed32 subset,
  String file_name,
  Name_string dsa_name,
  Bool check,
  Subtree ** subtree)
{
static char function[] = "d21_17_gnshd";
Shd_job          new_job;
Bool             remove;        /* a flag indicating whether existing */
				/* jobs have to be removed (existing  */
				/* jobs are subsets of the new job)   */
signed32         file_mode;     /* a flag indicating whether shadowing*/
				/* job file is empty                  */
char             dir_path[D27_LFILE_NAME];
char            *path = dir_path;
signed32         dir_id;
D20_delupd_info  del_info;
signed32         val[D20_NOVAL];
d2_ret_val ret_value = D2_NOERROR;

DCE_SVC_LOG((GDS_S_ENTRY_FUNCTION_MSG,function));

				/* check version, bind_id-id; get path */
if (d21d006_param_check(bind_id, return_code, version, &path, &dir_id)
			== D2_ERROR)
{   
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,return_code->errvalue));
    return(D2_ERROR);
}

if (d20_check_bind_dsa((char *) dsa_name,path,dir_id) == TRUE)
	{
	return_code->retcode  = CALLERR;
    	return_code->errclass = PARERR;
    	return_code->errvalue = DS_DEST_LOC;
        DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,return_code->errvalue));
    	return(D2_ERROR);
	}

if (mode != JOB_ONLY && mode != SHD_AND_JOB)
{   return_code->retcode  = CALLERR;
    return_code->errclass = PARERR;
    return_code->errvalue = DS_INVMODE;
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,return_code->errvalue));
    return(D2_ERROR);
}

				/* check object interpretation        */
if (subset != OBJECT && subset != OBJECT_SUBORDINATES)
{   return_code->retcode  = CALLERR;
    return_code->errclass = PARERR;
    return_code->errvalue = DS_INV_SEARCH_SEL;
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,return_code->errvalue));
    return(D2_ERROR);
}

if (mode == JOB_ONLY)
				/* get job from input parameters      */
{   new_job.shd_subset = subset;
    if (object == NULL)
    {   new_job.shd_objname[0] = D2_EOS;
    }
    else
    {   strcpy(new_job.shd_objname,(char *)object);
	d20_norm(new_job.shd_objname, strlen(new_job.shd_objname));
    }
    new_job.shd_t_stamp = time(0L);
}
else                            /* get job from dump file             */
{   if (d20_120_read_delupd_info(file_name, val, &del_info) == D2_ERROR)
    {   return_code->retcode  = PERMERR;
	return_code->errclass = SYSERR;
	return_code->errvalue = d20_head->d2_errvalue;
        DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,return_code->errvalue));
	return(D2_ERROR);
    }
    else
    {   new_job.shd_subset = (del_info.d20_sobj == TRUE) ?
			     OBJECT : OBJECT_SUBORDINATES;
	strcpy(new_job.shd_objname, del_info.d20_obj);
	d21_cre_shd = TRUE;
    }
}
strcpy(new_job.shd_dsa,(char *)dsa_name);
d20_norm(new_job.shd_dsa, strlen(new_job.shd_dsa));

				/* trace shadowing job                */
d21d010_trace_job((Name_string)(new_job.shd_dsa),
		(Name_string)(new_job.shd_objname), new_job.shd_subset);

				/* check new job whether it is allowed*/
if (d21d002_check_job(bind_id, path, dir_id, &new_job, check, return_code,
		      subtree, &remove, &file_mode) == D2_ERROR)
{   
    if (d20_dp_fd != -1)
    {   close(d20_dp_fd);
	d20_dp_fd = -1;
    }
    d21_cre_shd = FALSE;
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,return_code->errvalue));
    return(D2_ERROR);
}

				/* add new job to shadowing job file  */
if (d21d004_add_job(bind_id, &new_job, remove, path, dir_id, file_mode,
		    file_name,(char *)dsa_name, return_code) == D2_ERROR)
{   
    if (d20_dp_fd != -1)
    {   close(d20_dp_fd);
	d20_dp_fd = -1;
    }
    d21_cre_shd = FALSE;
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,return_code->errvalue));
    return(D2_ERROR);
}
d21_cre_shd = FALSE;

DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,return_code->errvalue));
return(ret_value);
}
