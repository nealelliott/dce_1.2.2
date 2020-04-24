/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21update.c,v $
 * Revision 1.1.10.2  1996/02/18  19:42:11  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:22:54  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:20:39  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:52:47  root]
 * 
 * Revision 1.1.8.3  1994/05/10  15:50:13  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:14:31  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:06:59  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:13:47  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:35:46  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:37:46  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  10:57:18  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:40:43  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:10:51  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:07:26  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  15:01:50  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  16:14:22  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:01:21  melman
 * 	New GDS merged drop
 * 	[1992/05/28  20:45:29  melman]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21update.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:42:11 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* NAME         : d21update.o                                         */
/*								      */
/* AUTHOR       : Schmid ,D AP 11                                     */
/* DATE         : 16.01.89                                            */
/*								      */
/*datoff **************************************************************/

#include <gds.h>
#include <stdio.h>
#include <string.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <gdsext.h>
#include <d2delupd.h>
#include <d20proto.h>

/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  d21_19_updat()                                         */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function makes an update of the given shadowing job      */
/*      in the internal shadowing update file.                        */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*	signed16      bind_id;					      */
/*      signed16      version;                                        */
/*      signed32      upd_mode;             update mode               */
/*      Name_string   object;               object name (in job)      */
/*      signed32      subset;               object interpretation     */
/*      Name_string   destination_dsa;                                */
/*      Upd_time     *update_time;                                    */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      Errcode      *return_code;          error values              */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_NOERROR     = The function has been performed successfully.*/
/*      D2_ERROR       = function fails                               */
/*                                                                    */
/*exoff ***************************************************************/

d2_ret_val d21_19_updat(
  signed16 bind_id,
  Errcode * return_code,
  signed16 version,
  signed32 upd_mode,
  Name_string object,
  signed32 subset,
  Name_string destination_dsa,
  Upd_time * update_time)
{
static char function[] = "d21_19_updat";
Shd_job  job;
char     dir_path[D27_LFILE_NAME];
char    *path = dir_path;
signed32 dir_id;

DCE_SVC_LOG((GDS_S_ENTRY_FUNCTION_MSG,function));

				/* check version, bind_id-id; get path */
if (d21d006_param_check(bind_id, return_code, version, &path, &dir_id)
			== D2_ERROR)
{   
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function, return_code->errvalue));
    return(D2_ERROR);
}

				/* check update mode                  */
if (upd_mode != ACTIVATE && upd_mode != DEACTIVATE &&
    upd_mode != CHANGE_UPD_TIME && upd_mode != UPD_IMMEDIATELY)
{   return_code->retcode  = CALLERR;
    return_code->errclass = PARERR;
    return_code->errvalue = DS_INVMODE;
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function, return_code->errvalue));
    return(D2_ERROR);
}

				/* check object interpretation        */
if (subset != OBJECT && subset != OBJECT_SUBORDINATES &&
    upd_mode <= UPD_IMMEDIATELY)
{   return_code->retcode  = CALLERR;
    return_code->errclass = PARERR;
    return_code->errvalue = DS_INV_SEARCH_SEL;
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function, return_code->errvalue));
    return(D2_ERROR);
}

if (upd_mode >= ACTIVATE && upd_mode <= UPD_IMMEDIATELY)
{                               /* trace shadowing job                */
    d21d010_trace_job(destination_dsa, object, subset);
}

if (update_time != (Upd_time *) NULL)
{                               /* trace update times                 */
    d21d011_trace_upd_mode_and_times(upd_mode, update_time);
}

				/* define job to look for             */
if (upd_mode <= UPD_IMMEDIATELY)
				/* DSA update job                     */
{   if (object != NULL)
    {   strcpy(job.shd_objname,(char *)object);
	d20_norm(job.shd_objname, strlen(job.shd_objname));
    }
    else
    {   *(job.shd_objname) = D2_EOS;
    }
    job.shd_subset = subset;
    strcpy(job.shd_dsa,(char *)destination_dsa);
    d20_norm(job.shd_dsa, strlen(job.shd_dsa));
}

if (upd_mode == ACTIVATE || upd_mode == CHANGE_UPD_TIME)
{   job.shd_time = *update_time;
				/* check update time                  */
    if (d21d001_check_time(update_time) == D2_ERROR)
    {   return_code->retcode  = CALLERR;
	return_code->errclass = PARERR;
	return_code->errvalue = DS_INVTIME;
        DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function, return_code->errvalue));
	return(D2_ERROR);
    }
}
else
{   job.shd_time.upd_t_mode = job.shd_time.upd_min = job.shd_time.upd_hour =
    job.shd_time.upd_1d_of_week = job.shd_time.upd_1hour =
    job.shd_time.upd_2d_of_week = job.shd_time.upd_2hour = 0;
}

				/* update shadowing job file          */
if (d21d005_upd_job_file(bind_id, upd_mode, &job, path, dir_id,
			 return_code) == D2_ERROR)
{   
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function, return_code->errvalue));
    return(D2_ERROR);
}

DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function, return_code->errvalue));
return(D2_NOERROR);
}
