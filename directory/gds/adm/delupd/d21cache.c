/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21cache.c,v $
 * Revision 1.1.10.2  1996/02/18  19:41:53  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:22:38  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:19:43  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:52:36  root]
 * 
 * Revision 1.1.8.3  1994/05/10  15:50:01  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:14:00  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:06:50  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:13:05  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:35:32  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:35:48  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  10:45:02  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:38:07  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  18:10:21  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:06:40  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/18  08:28:59  marrek
 * 	Various bug fixes for mask handling found at SNI
 * 	[1992/12/17  16:29:42  marrek]
 * 
 * Revision 1.1.4.2  1992/11/27  14:58:35  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  16:11:45  marrek]
 * 
 * Revision 1.1.2.3  1992/06/30  20:54:39  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  20:33:37  melman]
 * 
 * Revision 1.1.2.2  1992/06/01  19:59:50  melman
 * 	New GDS merged drop
 * 	[1992/05/28  20:43:14  melman]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21cache.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:41:53 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* NAME         : d21cache.o                                          */
/*								      */
/* AUTHOR       : Schmid ,D AP 11                                     */
/* DATE         : 05.01.89                                            */
/*								      */
/*datoff **************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <gdsext.h>
#include <d2delupd.h>
#include <d20proto.h>

/**********************************************************************/
/*								      */
/*			  D E F	I N E S				      */
/*								      */
/**********************************************************************/

#define D21_MAX_HOUR      24    /* maximal value for hours (used if   */
				/* update frequency is UPD_LOW_1 or   */
				/* UPD_LOW_2)                         */
#define D21_MAX_MIN       56

/**********************************************************************/
/*								      */
/*		   D E C L A R A T I O N S       DATA		      */
/*								      */
/**********************************************************************/

extern struct flock d21lock_arg;
extern int          d21lock_no;
extern FILE        *d21fp_lock; /* file pointer for lock file         */
extern FILE        *d21fp_job;  /* file pointer to shadowing job file */
				/* job file name                      */
extern char         d21shd_file_name[];

/* LOCAL FUNCTIONS */

static d2_ret_val d21d024_gen_cache_cron_entry ( char * , signed32,Upd_time * );
static d2_ret_val d21d026_upd_job_file( signed32 , Shd_job * , char * ,
							signed32 , Errcode * );
static d2_ret_val d21d028_update_job( char * , signed32 , signed32 , Shd_job * ,
					Shd_job * , Bool * , Errcode * );

/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  d21d024_gen_cache_cron_entry()                         */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function will create a new entry in "crontab" for CACHE  */
/*      update.                                                       */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      path        = path name of directory installation             */
/*      dir_id      = directory ID                                    */
/*      upd_t       = update times                                    */
/*                    == NULL, if job is to be deactivated            */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_NOERROR     = The function has been performed successfully.*/
/*      D2_ERROR       = function fails                               */
/*                                                                    */
/*exoff ***************************************************************/

static d2_ret_val d21d024_gen_cache_cron_entry(
  char * path,
  signed32 dir_id,
  Upd_time * upd_t)
{
char  cron_entry[5 * D21_NDIST_VAL * 16 + 5 * D27_LFILE_NAME + 40];
char *p_str;
signed32   val;

sprintf(cron_entry, ". %s%s/.profile;%s%s%s %s%s%s %s%s%s %d %s ",
	dcelocal_path, D27_CLIENT_DIR, dcelocal_path, D27_BIN_DIR, D20_EXEC,
	dcelocal_path, D27_BIN_DIR, D20_CUP_SCR,
	dcelocal_path, D27_BIN_DIR, D20_CA_UP, dir_id, dcelocal_path);

p_str = cron_entry + strlen(cron_entry);

if (upd_t != (Upd_time *) NULL)
{   switch ((int) upd_t->upd_t_mode)
    {   case UPD_HIGH:
		for (val = 0; val < D21_MAX_MIN; val += upd_t->upd_min)
		{   sprintf(p_str, "%d,", val);
		    p_str += strlen(p_str);
		}
		*(p_str -1) = ' ';
		strcat(p_str, "\\\'\''*'\'\\\' \\\'\''*'\'\\\' ");
		strcat(p_str, "\\\'\''*'\'\\\' \\\'\''*'\'\\\' ");
		break;

	case UPD_MEDIUM:
		strcpy(p_str, "0 ");
		p_str += strlen(p_str);
		for (val = 0; val < D21_MAX_HOUR; val += upd_t->upd_hour)
		{   sprintf(p_str, "%d,", val);
		    p_str += strlen(p_str);
		}
		*(p_str -1) = ' ';
		strcat(p_str, "\\\'\''*'\'\\\' \\\'\''*'\'\\\' ");
		strcat(p_str, "\\\'\''*'\'\\\' ");
		break;

	case UPD_LOW_2:
		strcpy(p_str, "0 ");
		p_str += strlen(p_str);
		sprintf(p_str, "%d,%d ", upd_t->upd_1hour, upd_t->upd_2hour);
		p_str += strlen(p_str);
		strcat(p_str, "\\\'\''*'\'\\\' \\\'\''*'\'\\\' ");
		if (upd_t->upd_1d_of_week == ALL_POSSIBLE_TIMES)
		{   strcat(p_str, "\\\'\''*'\'\\\' ");
		}
		else
		{   p_str += strlen(p_str);
		    sprintf(p_str, "%d,%d ", upd_t->upd_1d_of_week,
			    upd_t->upd_2d_of_week);
		}
		break;

	case UPD_LOW_1:
		strcpy(p_str, "0 ");
		p_str += strlen(p_str);
		sprintf(p_str, "%d ", upd_t->upd_1hour);
		p_str += strlen(p_str);
		strcat(p_str, "\\\'\''*'\'\\\' \\\'\''*'\'\\\' ");
		if (upd_t->upd_1d_of_week == ALL_POSSIBLE_TIMES)
		{   strcat(p_str, "\\\'\''*'\'\\\' ");
		}
		else
		{   p_str += strlen(p_str);
		    sprintf(p_str, "%d ", upd_t->upd_1d_of_week);
		}
		break;

    }
    p_str += strlen(p_str);
}

strcpy(p_str, " 2>/dev/null");
if (system(cron_entry) != 0)
{   return(D2_ERROR);
}

return(D2_NOERROR);
}

/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  d21_025_update()                                       */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function makes an update of the given shadowing job      */
/*      in the internal shadowing update file (CACHEJOB)              */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      signed32      dir_id;                                         */
/*      signed16      version;                                        */
/*      signed32      upd_mode;             update mode               */
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

d2_ret_val d21_025_update(
  signed32 dir_id,
  Errcode * return_code,
  signed16 version,
  signed32 upd_mode,
  Upd_time * update_time)
{
static char function[] = "d21_025_update";
Shd_job  job;
char     dir_path[D27_LFILE_NAME];
char    *path = dir_path;

DCE_SVC_LOG((GDS_S_ENTRY_FUNCTION_MSG,function));

sprintf(path, "%s%s", dcelocal_path, D27_CLIENT_DIR);

				/* check update mode                  */
if (upd_mode != ACTIVATE && upd_mode != DEACTIVATE &&
    upd_mode != CHANGE_UPD_TIME && upd_mode != UPD_IMMEDIATELY)
{   return_code->retcode  = CALLERR;
    return_code->errclass = PARERR;
    return_code->errvalue = DS_INVMODE;
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,DS_INVMODE));
    return(D2_ERROR);
}

if (update_time != (Upd_time *) NULL)
{                               /* trace update times                 */
    d21d011_trace_upd_mode_and_times(upd_mode, update_time);
}

if (upd_mode == ACTIVATE || upd_mode == CHANGE_UPD_TIME)
{   job.shd_time = *update_time;
				/* check update time                  */
    if (d21d001_check_time(update_time) == D2_ERROR)
    {   return_code->retcode  = CALLERR;
	return_code->errclass = PARERR;
	return_code->errvalue = DS_INVTIME;
        DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,DS_INVTIME));
	return(D2_ERROR);
    }
}
else
{   job.shd_time.upd_t_mode = job.shd_time.upd_min = job.shd_time.upd_hour =
    job.shd_time.upd_1d_of_week = job.shd_time.upd_1hour =
    job.shd_time.upd_2d_of_week = job.shd_time.upd_2hour = 0;
}

				/* update shadowing job file          */
if (d21d026_upd_job_file(upd_mode, &job, path, dir_id,
			 return_code) == D2_ERROR)
{   
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,return_code->errvalue));
    return(D2_ERROR);
}

DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,return_code->errvalue));

return(D2_NOERROR);
}

/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  d21d026_upd_job_file()                                 */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function changes the update time of the CACHE-JOB        */
/*      dependant on "upd_mode".                                      */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      signed32     upd_mode;  update mode                           */
/*      Shd_job     *job;       job to be looked for                  */
/*      char         path;      environment of directory installation */
/*      signed32     dir_id;    directory-ID                          */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      Errcode     *err;       error codes                           */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_NOERROR    = function terminates successful                */
/*      D2_ERROR       = error occurred                               */
/*                                                                    */
/*exoff ***************************************************************/

static d2_ret_val d21d026_upd_job_file(
  signed32 upd_mode,
  Shd_job * job,
  char * path,
  signed32 dir_id,
  Errcode * err)
{
static char function[] = "d21d026_upd_job_file";
char         tmp_file[D27_LFILE_NAME];
FILE        *fp_tmp;
Upd_time    *upd_t;
Shd_job      act_job;
d2_ret_val          ret_value = D2_NOERROR;
Bool         upd_changed = FALSE;

DCE_SVC_LOG((GDS_S_ENTRY_FUNCTION_MSG,function));

				/* open and lock shadowing job file   */
if (d21d008_open_and_lock(path, dir_id, FALSE, err) == D2_ERROR)
{   
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,err->errvalue));
    return(D2_ERROR);
}

sprintf(tmp_file, "%s%s%s%d%s", path, D27_CACHE_DIR, D27_SUBCACHE_DIR,
	dir_id, D21_TMP_SHD_JOBS);

umask(0);
				/* open temporary shadowing job file  */
if ((fp_tmp = D27_FOPEN(svc_c_debug1,tmp_file, "w")) == (FILE *) NULL)
{   err->retcode  = PERMERR;
    err->errclass = SYSERR;
    err->errvalue = DS_SHD_FILE_ERR;
    fcntl(d21lock_no, F_SETLKW, &d21lock_arg);
    d21lock_no = -1;
    D27_FCLOSE(svc_c_debug1, d21fp_lock);
    d21fp_lock = NULL;
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,err->errvalue));
    return(D2_ERROR);
}

				/* read CACHE-JOB                     */
rewind(d21fp_job);
if (d21d027_getjb(path, err, READ_FIRST, dir_id, &act_job) == D2_NOERROR)
{                               /* do job update                      */
    if ((ret_value = d21d028_update_job(path, dir_id, upd_mode,
					job, &act_job,
					&upd_changed, err)) == D2_NOERROR)
    {                           /* check whether an update in the     */
				/* crontab (for CACHE JOB) has to be  */
				/* done                               */
	if (upd_changed == TRUE)
	{                       /* generate "crontab" entry           */
	    upd_t = (upd_mode == DEACTIVATE) ?
		    (Upd_time *) NULL : &act_job.shd_time;
	    if (d21d024_gen_cache_cron_entry(path, dir_id,
					     upd_t) == D2_ERROR)
	    {   err->retcode  = PERMERR;
		err->errclass = SYSERR;
		err->errvalue = DS_CRON_ERR;
		ret_value = D2_ERROR;
	    }
	}
    }

				/* store job that had been read       */
    upd_t = &act_job.shd_time;
    fprintf(fp_tmp, D2_CACHE_JOB_FORM, act_job.shd_state,
	    upd_t->upd_t_mode, upd_t->upd_min, upd_t->upd_hour,
	    upd_t->upd_1d_of_week, upd_t->upd_1hour,
	    upd_t->upd_2d_of_week, upd_t->upd_2hour);
}
else
{   err->retcode  = CALLERR;
    err->errclass = PARERR;
    err->errvalue = DS_NFOUND;
    ret_value = D2_ERROR;
}

				/* close shadowing job file           */
if (d21fp_job != (FILE *) 0)
{   D27_FCLOSE (svc_c_debug1, d21fp_job);
    d21fp_job = (FILE *) 0;
}

if (ret_value == D2_NOERROR)
{   err->retcode  = err->errclass = NOERR;
    err->errvalue = D2_NOERROR;
}

				/* close temporary shadowing job file */
D27_FCLOSE(svc_c_debug1, fp_tmp);

if (ret_value == D2_NOERROR && upd_mode != UPD_IMMEDIATELY)
{                               /* copy temporary file to shadowing job */
				/* file                                 */
    if (unlink(d21shd_file_name) < 0)
    {   err->retcode  = PERMERR;
	err->errclass = SYSERR;
	err->errvalue = DS_SHD_FILE_ERR;
	DCE_SVC_LOG((GDS_S_UNLINK_ERR_MSG, d21shd_file_name, errno));
				/* unlock lock-file                   */
	fcntl(d21lock_no, F_SETLKW, &d21lock_arg);
	d21lock_no = -1;
	D27_FCLOSE(svc_c_debug1, d21fp_lock);
	d21fp_lock = NULL;
        DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,err->errvalue));
	return (D2_ERROR);
    }
    if (link(tmp_file, d21shd_file_name) < 0)
    {   err->retcode  = PERMERR;
	err->errclass = SYSERR;
	err->errvalue = DS_SHD_FILE_ERR;
	DCE_SVC_LOG((GDS_S_LINK_ERR_MSG, d21shd_file_name, errno));
				/* unlock lock-file                   */
	fcntl(d21lock_no, F_SETLKW, &d21lock_arg);
	d21lock_no = -1;
	D27_FCLOSE(svc_c_debug1, d21fp_lock);
	d21fp_lock = NULL;
        DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,err->errvalue));
	return (D2_ERROR);
    }
}

				/* delete temporary file              */
if (unlink(tmp_file) < 0)
{   err->retcode  = PERMERR;
    err->errclass = SYSERR;
    err->errvalue = DS_SHD_FILE_ERR;
    DCE_SVC_LOG((GDS_S_UNLINK_ERR_MSG, tmp_file, errno));
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,err->errvalue));
    ret_value = D2_ERROR;
}

				/* unlock lock-file                   */
fcntl(d21lock_no, F_SETLKW, &d21lock_arg);
d21lock_no = -1;
D27_FCLOSE(svc_c_debug1, d21fp_lock);
d21fp_lock = NULL;

DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,err->errvalue));
return(ret_value);
}

/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  d21d027_getjb()                                        */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function read the shadowing job file and returns one     */
/*      job.                                                          */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      char        *path;              path name of directory        */
/*                                      installation                  */
/*      signed32     mode;              mode for reading              */
/*      signed32     dir_id;            directory identifier          */
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

d2_ret_val d21d027_getjb(
  char * path,
  Errcode * return_code,
  signed32 mode,
  signed32 dir_id,
  Shd_job * job)
{
Upd_time *upd_t;
static char function[] = "d21d027_getjb";

DCE_SVC_LOG((GDS_S_ENTRY_FUNCTION_MSG,function));

return_code->retcode  = return_code->errclass = NOERR;
return_code->errvalue = NOERROR;

switch ((int) mode)
{   case READ_FIRST:
    case READ_NEXT:             /* read entry from shadowing job file */
				/* open and lock shadowing job file   */
		if (d21d008_open_and_lock(path, dir_id, FALSE, return_code)
					  == D2_ERROR)
		{   
        	    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,
			return_code->errvalue));
		    return(D2_ERROR);
		}

		upd_t = &job->shd_time;
		switch (fscanf(d21fp_job, D2_CACHE_JOB_FORM,
			       &job->shd_state, &upd_t->upd_t_mode,
			       &upd_t->upd_min, &upd_t->upd_hour,
			       &upd_t->upd_1d_of_week, &upd_t->upd_1hour,
			       &upd_t->upd_2d_of_week, &upd_t->upd_2hour))
		{   case D2_CACHE_JOB_PAR:
			     break;
		    case EOF:return_code->retcode  = TEMPERR;
			     return_code->errclass = SYSERR;
			     return_code->errvalue = DS_EOF;
        	    	     DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,
				return_code->errvalue));
			     return(D2_ERROR);
		    default: return_code->retcode  = PERMERR;
			     return_code->errclass = SYSERR;
			     return_code->errvalue = DS_SHD_FILE_ERR;
        	    	     DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,
				return_code->errvalue));
			     return(D2_ERROR);
		}
		break;
    case TERMINATE:
		if (d21fp_job != (FILE *) 0)
		{   D27_FCLOSE (svc_c_debug1, d21fp_job);
		    d21fp_job = (FILE *) 0;

				/* unlock lock-file                     */
		    fcntl(d21lock_no, F_SETLKW, &d21lock_arg);
		    D27_FCLOSE(svc_c_debug1, d21fp_lock);
		    d21fp_lock = NULL;
		}
		break;

    default:
		break;
}


DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,return_code->errvalue));
return(D2_NOERROR);
}

/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  d21d028_update_job()                                   */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function changes the state of job or the update times    */
/*      of the actual read job depending on the update mode.          */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      char         path;      directory system installation         */
/*      signed32     dir_id;    directory ID                          */
/*      signed32     upd_mode;  update mode                           */
/*      Shd_job     *job;       new job                               */
/*      Shd_job     *act_job;   actual job read from file that is to  */
/*                              be updated                            */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      Bool        *upd_changed;   a flag indicating that the        */
/*                                  occurrencies of the update times  */
/*                                  were changed                      */
/*      Errcode     *err;       error codes                           */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_NOERROR    = function terminates successful                */
/*      D2_ERROR       = illegal update modification                  */
/*                                                                    */
/*exoff ***************************************************************/

static d2_ret_val d21d028_update_job(
  char * path,
  signed32 dir_id,
  signed32 upd_mode,
  Shd_job * job,
  Shd_job * act_job,
  Bool * upd_changed,
  Errcode * err)
{
static char function[] = "d21d028_update_job";
d2_ret_val  ret_value = D2_NOERROR;
Upd_time    *upd_t;
char         command[D27_LFILE_NAME + 20];

DCE_SVC_LOG((GDS_S_ENTRY_FUNCTION_MSG,function));

				/* check state of job to be updated   */
if ((act_job->shd_state & ACT_JOB) == ACT_JOB)
				/* job is active                      */
{   switch ((int) upd_mode)
    {   case DEACTIVATE:
		    *upd_changed = TRUE;
		    act_job->shd_state |= NOT_ACT_JOB;
		    act_job->shd_state &= ~ACT_JOB;
		    upd_t = &act_job->shd_time;
		    upd_t->upd_t_mode =
		    upd_t->upd_min = upd_t->upd_hour =
		    upd_t->upd_1d_of_week = upd_t->upd_1hour =
		    upd_t->upd_2d_of_week = upd_t->upd_2hour = 0;
		    break;
	case CHANGE_UPD_TIME:
		    *upd_changed = TRUE;
		    act_job->shd_time = job->shd_time;
		    break;
	case ACTIVATE:
		    ret_value = D2_ERROR;
		    break;
	default:    break;
    }
}
else
				/* job is not active                  */
{   switch ((int) upd_mode)
    {   case DEACTIVATE:
	case CHANGE_UPD_TIME:
		    ret_value = D2_ERROR;
		    break;
	case ACTIVATE:
		    *upd_changed = TRUE;
		    act_job->shd_state |= ACT_JOB;
		    act_job->shd_state &= ~NOT_ACT_JOB;
		    act_job->shd_time = job->shd_time;
		    break;
	default:    break;
    }
}

if (ret_value == D2_ERROR)
{   err->retcode = CALLERR;
    err->errclass = PARERR;
    err->errvalue = DS_INVMODE;
}

if (upd_mode == UPD_IMMEDIATELY)
{                               /* start CACHE update propagation process */
    sprintf(command, "%s%s%s %d %s &", dcelocal_path, D27_BIN_DIR, D20_CA_UP,
	    dir_id, dcelocal_path);
    ret_value = D27_SYSTEM(svc_c_debug1, command);
}

if (ret_value != D2_NOERROR)
  {
  ret_value = D2_ERROR;
  }

DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,ret_value));
return(ret_value);
}
