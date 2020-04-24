/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21getjob.c,v $
 * Revision 1.1.12.3  1996/02/18  19:42:01  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:22:43  marty]
 *
 * Revision 1.1.12.2  1995/12/08  15:19:59  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/DCE1_1_WP/1  1995/04/26  19:41 UTC  jrr
 * 	Include errno.h for AIX.
 * 	[1995/12/08  14:52:40  root]
 * 
 * Revision 1.1.8.3  1994/05/10  15:50:06  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:14:14  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:06:53  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:13:20  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:35:36  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:36:25  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  10:48:42  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:39:01  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:10:30  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:06:53  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  14:59:43  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  16:12:42  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:00:16  melman
 * 	New GDS merged drop
 * 	[1992/05/28  20:43:33  melman]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21getjob.c,v $ $Revision: 1.1.12.3 $ $Date: 1996/02/18 19:42:01 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* NAME         : d21getjob.o                                         */
/*								      */
/* AUTHOR       : Schmid ,D AP 11                                     */
/* DATE         : 16.01.89                                            */
/*								      */
/*datoff **************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <gdsext.h>
#include <d2delupd.h>
#include <d21dua.h>
#include <d20proto.h>

/**********************************************************************/
/*								      */
/*		   D E C L A R A T I O N S       DATA		      */
/*								      */
/**********************************************************************/

static signed32 d21_job_nr = 0; /* job number of actual job           */
static struct flock lock_arg = { F_WRLCK, SEEK_SET, 0L, 0L, 0 };

/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  d21_20_getjb()                                         */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function read the shadowing job file and returns one     */
/*      job.                                                          */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      signed16     bind_id;                                         */
/*      signed16     version;           version number of interface   */
/*      signed32     mode;              mode for reading              */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      Errcode     *return_code;       return-values                 */
/*      Shd_job     *job;               shadowing job                 */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_NOERROR     = The function has been performed successfully.*/
/*      D2_ERROR       = function fails                               */
/*                                                                    */
/*exoff ***************************************************************/

d2_ret_val d21_20_getjb(
  signed16 bind_id,
  Errcode * return_code,
  signed16 version,
  signed32 mode,
  Shd_job * job)
{
static char function[] = "d21_20_getjb";
char      dir_path[D27_LFILE_NAME];
char     *path = dir_path;
signed32  dir_id;
signed32  job_nr;
Upd_time *upd_t;
int       lock_no;
FILE     *fp_lock;
FILE     *fp_job = (FILE *) NULL;
char      lock_file_name[D27_LFILE_NAME];
char      shd_file_name[D27_LFILE_NAME];
signed32  time_upd[D21_T_OCC];   /* indicates the occurrency of update*/
				 /* times in the shadowing job file   */
d21_bind_info bind_info;          /* bind table      */
Bool		dummy;
D2_pbhead	dummy_pbhead;


DCE_SVC_LOG((GDS_S_ENTRY_FUNCTION_MSG,function));

return_code->retcode  = return_code->errclass = NOERR;
return_code->errvalue = NOERROR;

if (version != DS_V021)
{   return_code->retcode  = CALLERR;
    return_code->errclass = ILLVERS;
    return_code->errvalue = DS_NOT_SPEC;
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,return_code->errvalue));
    return(D2_ERROR);
}

if (d21_bh8_get_bind_info(bind_id, &bind_info, &dummy, &dummy_pbhead, FALSE) 
				== D2_ERROR)
{   return_code->retcode  = CALLERR;
    return_code->errclass = PARERR;
    return_code->errvalue = DS_NO_BIND;
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,return_code->errvalue));
    return(D2_ERROR);
}
dir_id = (signed32) bind_info.d21_b_dir_id;

if (mode != READ_FIRST && mode != READ_NEXT && mode != TERMINATE)
{   return_code->retcode  = CALLERR;
    return_code->errclass = PARERR;
    return_code->errvalue = DS_INVMODE;
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,return_code->errvalue));
    return(D2_ERROR);
}

if (mode == TERMINATE)
{   
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,return_code->errvalue));
    return(D2_NOERROR);
}

				/* get file name of shadowing job file*/
sprintf(path, "%s%s", dcelocal_path, D27_SERVER_DIR);

				/* lock "LOCKJOBS" file               */
sprintf(lock_file_name, "%s%s%s%d%s", path, D27_DSA_DIR, D27_SUBDSA_DIR,
	dir_id, D2_LOGJOBS);
				/* open lock file                     */
if ((fp_lock = D27_FOPEN(svc_c_debug1, lock_file_name, "a")) == (FILE *) NULL)
{   return_code->retcode  = PERMERR;
    return_code->errclass = SYSERR;
    return_code->errvalue = DS_SHD_FILE_ERR;
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,return_code->errvalue));
    return(D2_ERROR);
}
lock_no = fileno(fp_lock);
				/* lock lock-file                     */
lock_arg.l_type = F_WRLCK;
if (fcntl(lock_no, F_SETLKW, &lock_arg) == -1)
{   return_code->retcode  = PERMERR;
    return_code->errclass = SYSERR;
    return_code->errvalue = DS_SHD_FILE_ERR;
    DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s%d"),GDS_S_GENERAL,
	svc_c_debug1, GDS_S_FCNTL_ERR, F_SETLKW, errno));
    fclose(fp_lock);
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,return_code->errvalue));
    return(D2_ERROR);
}
lock_arg.l_type = F_UNLCK;

				/* open shadowing job file            */
sprintf(shd_file_name, "%s%s%s%d%s", path, D27_DSA_DIR, D27_SUBDSA_DIR,
	dir_id, D21_SHD_JOBS);
umask(0);
if ((fp_job = D27_FOPEN(svc_c_debug1, shd_file_name, "r+")) == (FILE *) NULL)
{   return_code->retcode  = PERMERR;
    return_code->errclass = SYSERR;
    return_code->errvalue = DS_SHD_FILE_ERR;
    fcntl(lock_no, F_SETLKW, &lock_arg);
    fclose(fp_lock);
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,return_code->errvalue));
    return(D2_ERROR);
}

				/* read occurrencies of update times  */
if (D27_FREAD(svc_c_debug1, (char *) time_upd, sizeof(signed32), D21_T_OCC, 
	fp_job) == 0)
{   if (feof(fp_job) != 0)      /* EOF reached                        */
    {   return_code->retcode  = TEMPERR;
	return_code->errclass = SYSERR;
	return_code->errvalue = DS_EOF;
    }
    else                        /* error while reading                */
    {   return_code->retcode  = PERMERR;
	return_code->errclass = SYSERR;
	return_code->errvalue = DS_SHD_FILE_ERR;
    }
    fcntl(lock_no, F_SETLKW, &lock_arg);
    fclose(fp_lock);
    fclose(fp_job);
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,return_code->errvalue));
    return(D2_ERROR);
}

if (mode == READ_FIRST)
{   d21_job_nr = 1;
}
else
{   d21_job_nr++;
}
				/* read entry from shadowing job file */
upd_t = &job->shd_time;
for (job_nr = 0; job_nr < d21_job_nr; job_nr++)
{   switch (fscanf(fp_job, D2_RJOB_FORM, &job->shd_state,
		   &job->shd_nr, &upd_t->upd_t_mode,
		   &upd_t->upd_min, &upd_t->upd_hour,
		   &upd_t->upd_1d_of_week, &upd_t->upd_1hour,
		   &upd_t->upd_2d_of_week, &upd_t->upd_2hour,
		   job->shd_objname, &job->shd_subset,
		   job->shd_dsa, &job->shd_idx_dsa,
		   &job->shd_t_stamp))
    {   case D2_RJOBAR: if (strcmp(job->shd_objname, D21_ROOT) == 0)
			{  job->shd_objname[0] = D2_EOS;
			}
			break;
	case EOF:       return_code->retcode  = TEMPERR;
			return_code->errclass = SYSERR;
			return_code->errvalue = DS_EOF;
			fcntl(lock_no, F_SETLKW, &lock_arg);
			fclose(fp_lock);
			fclose(fp_job);
    			DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,
				return_code->errvalue));
			return(D2_ERROR);
	default:        return_code->retcode  = PERMERR;
			return_code->errclass = SYSERR;
			return_code->errvalue = DS_SHD_FILE_ERR;
			fcntl(lock_no, F_SETLKW, &lock_arg);
			fclose(fp_lock);
			fclose(fp_job);
    			DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,
				return_code->errvalue));
			return(D2_ERROR);
    }
}
fclose (fp_job);

				/* unlock lock-file                     */
fcntl(lock_no, F_SETLKW, &lock_arg);
fclose(fp_lock);

DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,return_code->errvalue));
return(D2_NOERROR);
}
