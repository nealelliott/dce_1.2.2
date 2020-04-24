/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21dutil.c,v $
 * Revision 1.1.10.2  1996/02/18  19:42:00  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:22:41  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:19:54  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:52:39  root]
 * 
 * Revision 1.1.8.4  1994/06/21  14:43:51  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:25:04  marrek]
 * 
 * Revision 1.1.8.3  1994/05/10  15:50:04  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:14:11  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:06:52  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:13:17  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:35:34  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:36:15  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  10:47:23  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:38:42  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:10:27  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:06:49  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  14:59:24  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  16:12:22  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:00:08  melman
 * 	New GDS merged drop
 * 	[1992/05/28  20:43:27  melman]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21dutil.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:42:00 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* NAME         : d21dutil.o                                          */
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
#include <d2dump.h>
#include <d20proto.h>

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

Bool    d21_cre_shd = FALSE;    /* a flag indicating that shadows have*/
				/* to be created; if shadow creation  */
				/* fails, the original shadowing job  */
				/* file will be used further on       */
signed32 d21time_upd[D21_T_OCC];/* indicates the occurrency of update */
				/* times in the shadowing job file    */

static char *d21_dsa;           /* destination DSA for restoring      */
static char *d21_file;          /* dump file name                     */

/* LOCAL FUNCTIONS */

static d2_ret_val d21d003_err_subtree( Name_string , signed32 , signed32 ,
				Errcode * , signed32 * , signed32 * , 
				Subtree ** );
static d2_ret_val d21d007_update_job( char * , signed32 , signed32 , Shd_job * ,
					Shd_job * , Bool * , Errcode * );
static d2_ret_val d21d020_cre_shd( signed16 , Errcode * , char * , char * ,
						signed32 , char * , Shd_job * );
static d2_ret_val d21getjb( Errcode * , signed32 , Shd_job * );

/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  d21d002_check_job()                                    */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function checks whether the new job is a correct one that*/
/*      doesn't overlap with existing one.                            */
/*      Furthermore if parameter "check" is set, a check will be done */
/*      whether there are overlapping shadowing jobs or whether       */
/*      existing jobs will be overwritten.                            */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      signed16     bind_id;                                         */
/*      char        *path;      path name of directory installation   */
/*      signed32     dir_id;    directory ID                          */
/*      Shd_job     *new_job;   new shadowing job to be added         */
/*      Bool         check;     a flag indicating whether overlapping */
/*                              subtrees can be ignored or not (if    */
/*                              existing jobs are subsets of the one) */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      Errcode     *err;       error codes                           */
/*      Subtree    **subtree;   overlapping subtrees (jobs)           */
/*      Bool        *remove;    a flag indicating whether existing    */
/*                              jobs have to be removed (existing jobs*/
/*                              are subsets of the new job)           */
/*      signed32    *mode;      indicates whether file was empty      */
/*                              (mode == READ_FIRST)                  */
/*                              or contains at least one element      */
/*                              (mode == READ_NEXT)                   */
/*                                                                    */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_NOERROR    = function terminates successful                */
/*      D2_ERROR       = error occurred                               */
/*                                                                    */
/*exoff ***************************************************************/

d2_ret_val d21d002_check_job(
  signed16 bind_id,
  char * path,
  signed32 dir_id,
  Shd_job * new_job,
  Bool check,
  Errcode * err,
  Subtree ** subtree,
  Bool * remove,
  signed32 * mode)
{
Errcode     ret_code;
Shd_job     act_job;
d2_ret_val  ret_value = D2_NOERROR;
signed32    obj_cmp;
signed32    cmp_len;
signed32    new_len;            /* length of new object/subtree        */
signed32    act_len;            /* length of actual object/subtree     */
signed32    max_mem_len = 0;    /* max. allocated memory length        */
signed32    act_mem_len = 0;    /* actual used length in memory        */


*remove = FALSE;
*subtree = (Subtree *) NULL;

				/* open and lock shadowing job file   */
if (d21d008_open_and_lock(path, dir_id, TRUE, err) == D2_ERROR)
{   return(D2_ERROR);
}

				/* read shadowing jobs                */
for (*mode = READ_FIRST; d21getjb(err, *mode, &act_job) == D2_NOERROR; )
{   *mode = READ_NEXT;
				/* check new job with actual job      */
    if (strcmp(new_job->shd_dsa, act_job.shd_dsa) != 0)
				/* different shadowing jobs           */
    {   continue;
    }

				/* compare object/subtree names       */
    new_len = strlen(new_job->shd_objname);
    act_len = strlen(act_job.shd_objname);
    cmp_len = new_len > act_len ? act_len : new_len;
    obj_cmp = strncmp(new_job->shd_objname, act_job.shd_objname, cmp_len);

    if (obj_cmp == 0 && new_len == act_len &&
	act_job.shd_subset == OBJECT_SUBORDINATES)
    {                           /* new job is a subset of an existing one */
				/* put existing subtree in error list     */
	(void)d21d003_err_subtree((Name_string)(act_job.shd_objname),
				act_job.shd_subset, act_len, err,
				&act_mem_len, &max_mem_len, subtree);
	ret_value = D2_ERROR;
	break;
    }

    if (new_len > act_len)
    {   if (obj_cmp == 0 && (new_job->shd_objname[act_len] == D2_RDN_SEP ||
	    act_len == 0) && act_job.shd_subset == OBJECT_SUBORDINATES)
	{                       /* new job is a subset of an existing one */
				/* put existing subtree in error list     */
	    (void)d21d003_err_subtree((Name_string)(act_job.shd_objname),
				act_job.shd_subset, act_len, err,
				&act_mem_len, &max_mem_len, subtree);
	    ret_value = D2_ERROR;
	    break;
	}
    }
    else
    {   if                      /* new job is subtree of root and actual */
				/* job is any job not equal root         */
	   ((new_len == 0 && act_len != 0) ||
				/* actual job is a real subset of new job */
	    (obj_cmp == 0 && act_job.shd_objname[new_len] == D2_RDN_SEP &&
	     new_job->shd_subset == OBJECT_SUBORDINATES) ||
				/* jobs are equal (in DNs) */
	    (obj_cmp == 0 && act_len == new_len))
	{                       /* existing job is a subset of the new one */
	    if (check == TRUE)
	    {   ret_value = D2_ERROR;
				/* put existing subtree in error list */
		if (d21d003_err_subtree((Name_string)(act_job.shd_objname),
					act_job.shd_subset, act_len, err,
					&act_mem_len, &max_mem_len, subtree)
					== D2_ERROR)
		{   break;
		}
	    }
	    else
	    {                   /* set flag that entries have to be removed*/
		*remove = TRUE;
	    }
	}
    }
}

if (err->errvalue != DS_EOF)
{   ret_value = D2_ERROR;
				/* close shadowing job file           */
    (void)d21getjb(&ret_code, TERMINATE, &act_job);
}
else
{   if (ret_value == D2_NOERROR)
    {   err->retcode  = err->errclass = NOERR;
	err->errvalue = NOERROR;
    }
}

if (ret_value == D2_ERROR)
{                               /* unlock lock-file                     */
    fcntl(d21lock_no, F_SETLKW, &d21lock_arg);
    d21lock_no = -1;
    fclose(d21fp_lock);
    d21fp_lock = (FILE *) 0;
}

return(ret_value);
}

/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  d21d003_err_subtree()                                  */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function builds the list of overlapping subtree.         */
/*      It checks the allocated memory and reallocates memory if      */
/*      necessary. The given subtree info is chained in the list.     */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      Name_string   objname;      object name                       */
/*      signed32      subset;       object interpretation             */
/*      signed32      len;          length of new object name         */
/*      Errcode      *err;          pointer to error info block       */
/*      signed32     *act_mem_len;  actual position in memory         */
/*      signed32     *max_mem_len;  maxmimum length of allocated      */
/*                                  memory                            */
/*      Subtree     **subtree;      erroneous subtrees                */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      signed32     *act_mem_len;  new actual position in memory     */
/*      signed32     *max_mem_len;  new maxmimum length of allocated  */
/*                                  memory                            */
/*      Subtree     **subtree;      new pointer to erroneous subtrees */
/*                                                                    */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_NOERROR    = function terminates successful                */
/*      D2_ERROR    = some error				      */
/*                                                                    */
/*exoff ***************************************************************/

static d2_ret_val d21d003_err_subtree(
  Name_string objname,
  signed32 subset,
  signed32 len,
  Errcode * err,
  signed32 * act_mem_len,
  signed32 * max_mem_len,
  Subtree ** subtree)
{
static char function[] = "d21d003_err_subtree";
signed32        length;
Subtree        *h_subtree = (Subtree *) NULL;
signed32            offset;
static Subtree *act_subtree;    /* actual position in subtree info    */
				/* block                              */

DCE_SVC_LOG((GDS_S_ENTRY_FUNCTION_MSG,function));

length = D2_ALIGN(sizeof(Subtree)) + D2_ALIGN(len + 1);
if (*act_mem_len + length > *max_mem_len)
{                               /* memory exceeds                     */
    *max_mem_len += D2_ALIGN(sizeof(Subtree)) + D2_ALIGN(DNL_MAX);
    if (*subtree == NULL)
    {   *subtree = (Subtree *) D27_MALLOC(svc_c_debug1, *max_mem_len);
	act_subtree = NULL;
    }
    else
    {   h_subtree = *subtree;
	*subtree = (Subtree *) D27_REALLOC(svc_c_debug1, (char *) *subtree, 
		*max_mem_len);
    }
    if (*subtree == NULL)
    {   err->retcode  = PERMERR;
	err->errclass = SYSERR;
	err->errvalue = DS_ALLOC_ERROR;
        DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,err->errvalue));
	return(D2_ERROR);
    }

    if (h_subtree != NULL)
    {                           /* memory has been reallocated        */
	offset = *subtree - h_subtree;
	act_subtree = (Subtree *) ((char *) act_subtree + offset);
	h_subtree = *subtree;
				/* restore pointers in subtree info   */
	do
	{   h_subtree->sub_next = (Subtree *)
				  ((char *) h_subtree->sub_next + offset);
	    h_subtree->sub_name = (Name_string)(h_subtree->sub_name+offset);
	    h_subtree = h_subtree->sub_next;
	}
	while (h_subtree->sub_next != NULL);
	h_subtree->sub_name = (Name_string) (h_subtree->sub_name + offset);
    }
}

				/* store new subtree info             */
if (act_subtree != NULL)
{   act_subtree->sub_next = (Subtree *) ((char *) *subtree + *act_mem_len);
}
else
{   act_subtree = *subtree;
}

*act_mem_len += length;

act_subtree->sub_next = NULL;
act_subtree->sub_name = (Name_string)((char *)act_subtree + D2_ALIGN(sizeof(Subtree)));
act_subtree->subset   = subset;
strcpy((char *)(act_subtree->sub_name),(char *)objname);

err->retcode  = CALLERR;
err->errclass = PARERR;
err->errvalue = DS_OVERLAY;

DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,err->errvalue));
return(D2_NOERROR);
}

/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  d21d004_add_job()                                      */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function adds the new_job to the shadowing job file.     */
/*      If parameter "remove" is set, existing jobs that are subsets  */
/*      of the new job will be removed.                               */
/*      The new job is added at the end of the file.                  */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      signed16     bind_id;                                         */
/*      Shd_job     *new_job;   new shadowing job to be added         */
/*      Bool         remove;    a flag indicating whether existing    */
/*                              jobs that are subsets of the new one  */
/*                              have to be removed                    */
/*      char         path;      environment of directory installation */
/*      signed32     dir_id;    directory-ID                          */
/*      signed32     file_mode; a flag indicating whether shadowing   */
/*                              job file is empty or not              */
/*      char        *file_name; name of dump file                     */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      Errcode     *err;       error codes                           */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_NOERROR    = function terminates successful                */
/*      D2_ERROR       = error occurred                               */
/*                                                                    */
/*exoff ***************************************************************/

d2_ret_val d21d004_add_job(
  signed16 bind_id,
  Shd_job * new_job,
  Bool remove,
  char * path,
  signed32 dir_id,
  signed32 file_mode,
  char * file_name,
  char * dsa_name,
  Errcode * err)
{
static char function[] = "d21d004_add_job";
Errcode      ret_code;
d2_ret_val   ret_value = D2_NOERROR;
signed32     i;
char        *p_obj;

DCE_SVC_LOG((GDS_S_ENTRY_FUNCTION_MSG,function));

				/* add new job                        */
if (remove == TRUE)
{                               /* look for overlapping subtrees      */
    d21_dsa = dsa_name;
    d21_file = file_name;
    ret_value = d21d005_upd_job_file(bind_id, REMOVE_ALL, new_job, path,
				     dir_id, err);
}
else
{   if (d21_cre_shd == TRUE)
    {                           /* create shadows                     */
	if (d21d020_cre_shd(bind_id, err, path, dsa_name, dir_id,
			    file_name, new_job) == D2_ERROR)
	{   ret_value = D2_ERROR;
	}
    }

				/* increment maximal shadowing job number */
    d21time_upd[D21_T_OCC - 1]++;

    if (file_mode == READ_FIRST && ret_value == D2_NOERROR)
				/* shadowing job file is yet empty    */
    {   for (i = 0; i < D21_T_OCC; i++)
	{   d21time_upd[i] = 0;
	}

	rewind(d21fp_job);
				/* copy update time occurrencies to   */
				/* shadowing job file                 */
	if (D27_FWRITE(svc_c_debug1, (char *)d21time_upd, sizeof(signed32), 
		D21_T_OCC, d21fp_job) == 0)
	{   err->retcode  = PERMERR;
	    err->errclass = SYSERR;
	    err->errvalue = DS_SHD_FILE_ERR;
	    ret_value = D2_ERROR;
	}
    }

    if (ret_value == D2_NOERROR)
				/* copy new job to shadowing job file */
    {   p_obj = (strlen(new_job->shd_objname) == 0) ? D21_ROOT
						    : new_job->shd_objname;
	fprintf(d21fp_job, D2_WJOB_FORM, NOT_ACT_JOB,
		d21time_upd[D21_T_OCC - 1], 0, 0, 0, 0, 0, 0, 0,
		p_obj, new_job->shd_subset, new_job->shd_dsa,
		D21_IDX_NOT_DEF, new_job->shd_t_stamp);
    }

    if (file_mode != READ_FIRST)
				/* update maximal job number          */
    {   rewind(d21fp_job);
				/* copy update time occurrencies to   */
				/* shadowing job file                 */
	if (D27_FWRITE(svc_c_debug1, (char *)d21time_upd, sizeof(signed32), 
		D21_T_OCC, d21fp_job) == 0)
	{   err->retcode  = PERMERR;
	    err->errclass = SYSERR;
	    err->errvalue = DS_SHD_FILE_ERR;
	    ret_value = D2_ERROR;
	}
    }

				/* close shadowing job file           */
    (void)d21getjb(&ret_code, TERMINATE, new_job);
				/* unlock lock-file                   */
    fcntl(d21lock_no, F_SETLKW, &d21lock_arg);
    d21lock_no = -1;
    fclose(d21fp_lock);
    d21fp_lock = (FILE *) 0;
}

DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,err->errvalue));
return(ret_value);
}

/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  d21d005_upd_job_file()                                 */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function removes all overlapping shadowing jobs or       */
/*      removes one specific shadowing job or changes one shadowing   */
/*      job dependant on "upd_mode".                                  */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      signed16     bind_id;                                         */
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

d2_ret_val d21d005_upd_job_file(
  signed16 bind_id,
  signed32 upd_mode,
  Shd_job * job,
  char * path,
  signed32 dir_id,
  Errcode * err)
{
static char function[] = "d21d005_upd_job_file";
char         tmp_file[D27_LFILE_NAME];
FILE        *fp_tmp;
signed32     mode = READ_FIRST;
Upd_time    *upd_t;
Shd_job      act_job;
Errcode      ret_code;
d2_ret_val ret_value = D2_NOERROR;
Bool         e_found = FALSE;
Bool         upd_changed = FALSE;
signed32     name_len;
signed32     act_len;
char        *p_obj;
signed32     job_counter;	/* Job counter				*/

				/* open and lock shadowing job file   */
if (d21d008_open_and_lock(path, dir_id, TRUE, err) == D2_ERROR)
{   return(D2_ERROR);
}

sprintf(tmp_file, "%s%s%s%d%s", path, D27_DSA_DIR, D27_SUBDSA_DIR, dir_id,
	D21_TMP_SHD_JOBS);

umask(0);
				/* open temporary shadowing job file  */
if ((fp_tmp = D27_FOPEN(svc_c_debug1, tmp_file, "w")) == (FILE *) NULL)
{   err->retcode  = PERMERR;
    err->errclass = SYSERR;
    err->errvalue = DS_SHD_FILE_ERR;
				/* unlock lock-file                   */
    fcntl(d21lock_no, F_SETLKW, &d21lock_arg);
    d21lock_no = -1;
    fclose(d21fp_lock);
    d21fp_lock = (FILE *) 0;
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,err->errvalue));
    return(D2_ERROR);
}

name_len = strlen(job->shd_objname);
				/* read all entries in shadowing job file */
for (	job_counter = 0;
	d21getjb(err, mode, &act_job) == D2_NOERROR;
	job_counter++)
{   if (mode == READ_FIRST)
    {                           /* store old update time occurrencies */
	d21d017_store_old_times();
	if (upd_mode == REMOVE_ALL)
	{                       /* new job has to be added -->            */
				/* increment maximal shadowing job number */
	    d21time_upd[D21_T_OCC - 1]++;
	}
				/* copy update time occurrencies to   */
				/* temporary file                     */
	if (D27_FWRITE(svc_c_debug1, (char *)d21time_upd, sizeof(signed32), 
		D21_T_OCC, fp_tmp) == 0)
	{   err->retcode  = PERMERR;
	    err->errclass = SYSERR;
	    err->errvalue = DS_SHD_FILE_ERR;
	    ret_value = D2_ERROR;
	    break;
	}
    }
    mode = READ_NEXT;

    if (upd_mode != REMOVE_ALL)
    {   if (e_found == FALSE)
	{                       /* entry not yet found                */
	    if (strcmp(job->shd_objname, act_job.shd_objname) == 0 &&
		job->shd_subset == act_job.shd_subset &&
		strcmp(job->shd_dsa, act_job.shd_dsa) == 0)
	    {                   /* entry found                        */
		e_found = TRUE;
		if (upd_mode == REMOVE)
		{               /* update time occurrency             */
				/* do cron-entry update if necessary  */
		    if ((act_job.shd_state & ACT_JOB) == ACT_JOB)
		    {   d21d013_del_upd_time(&act_job.shd_time);
			upd_changed = TRUE;
		    }
		    continue;
		}
				/* do job update                      */
		if ((ret_value = d21d007_update_job(path, dir_id,
						upd_mode, job,
						&act_job, &upd_changed,
						err)) == D2_ERROR)
		{   break;
		}
	    }
	}
    }
    else                        /* look for overlapping shadowing job */
    {   act_len = strlen(act_job.shd_objname);
	if ((name_len == 0 && strcmp(job->shd_dsa,act_job.shd_dsa)==0) ||
	    (strncmp(job->shd_objname,act_job.shd_objname,name_len)==0 &&
	    (act_job.shd_objname[name_len] == D2_RDN_SEP ||
	     act_job.shd_objname[name_len] == D2_EOS) &&
	     strcmp(job->shd_dsa, act_job.shd_dsa) == 0 &&
	     ((job->shd_subset==OBJECT_SUBORDINATES && act_len>=name_len)
	     || (job->shd_subset == OBJECT && act_len == name_len))))
	{                       /* update time occurrency             */
				/* do cron-entry update if necessary  */
	    if ((act_job.shd_state & ACT_JOB) == ACT_JOB)
	    {   d21d013_del_upd_time(&act_job.shd_time);
		upd_changed = TRUE;
	    }
	    continue;
	}
    }


				/* store job that had been read       */
    upd_t = &act_job.shd_time;
    p_obj = (strlen(act_job.shd_objname) == 0) ? D21_ROOT
					       : act_job.shd_objname;
    fprintf(fp_tmp, D2_WJOB_FORM, act_job.shd_state, act_job.shd_nr,
	    upd_t->upd_t_mode, upd_t->upd_min, upd_t->upd_hour,
	    upd_t->upd_1d_of_week, upd_t->upd_1hour,
	    upd_t->upd_2d_of_week, upd_t->upd_2hour,
	    p_obj, act_job.shd_subset, act_job.shd_dsa,
	    act_job.shd_idx_dsa, act_job.shd_t_stamp);
}

if (err->errvalue != DS_EOF)
{   ret_value = D2_ERROR;
}
else
{   if (ret_value == D2_NOERROR)
    {   err->retcode  = err->errclass = NOERR;
	err->errvalue = NOERROR;
    }
}

if (ret_value == D2_NOERROR && upd_mode == REMOVE_ALL)
{   if (d21_cre_shd == TRUE)
    {                           /* create shadows                     */
	if (d21d020_cre_shd(bind_id, err, path, d21_dsa, dir_id, d21_file,
			    job) == D2_ERROR)
	{   ret_value = D2_ERROR;
	}
    }

    if (ret_value == D2_NOERROR)
    {                           /* put new job in temporary file      */
	p_obj = (strlen(job->shd_objname) == 0) ? D21_ROOT
						: job->shd_objname;
	fprintf(fp_tmp, D2_WJOB_FORM, NOT_ACT_JOB,
		d21time_upd[D21_T_OCC - 1], 0, 0, 0, 0, 0, 0, 0,
		p_obj, job->shd_subset, job->shd_dsa, D21_IDX_NOT_DEF,
		job->shd_t_stamp);
    }
}

if (ret_value == D2_NOERROR && upd_changed == TRUE)
{                               /* job has been found and update time */
				/* occurrency has been changed        */
				/* --> check whether "crontab" entry  */
				/*     has to be changed              */
    if (d21d016_gen_cron_entry(path, dir_id) == D2_ERROR)
    {   err->retcode  = PERMERR;
	err->errclass = SYSERR;
	err->errvalue = DS_CRON_ERR;
	ret_value = D2_ERROR;
    }
    else                        /* update new occurrencies of update  */
				/* times                              */
    {   rewind(fp_tmp);
	if (D27_FWRITE(svc_c_debug1, (char *)d21time_upd, sizeof(signed32), 
		D21_T_OCC, fp_tmp) == 0)
	{   err->retcode  = PERMERR;
	    err->errclass = SYSERR;
	    err->errvalue = DS_SHD_FILE_ERR;
	    ret_value = D2_ERROR;
	}
    }
}

				/* close temporary shadowing job file */
D27_FCLOSE(svc_c_debug1, fp_tmp);

				/* close shadowing job file           */
(void)d21getjb(&ret_code, TERMINATE, &act_job);

				/* check whether job was found        */
				/* in case of upd_mode != REMOVE_ALL  */
if (e_found == FALSE && upd_mode != REMOVE_ALL)
{   err->retcode  = CALLERR;
    err->errclass = PARERR;
    err->errvalue = DS_NFOUND;
    ret_value = D2_ERROR;
}

if (ret_value == D2_NOERROR && upd_mode != UPD_IMMEDIATELY)
{                               /* copy temporary file to shadowing job */
				/* file                                 */
    if (unlink(d21shd_file_name) < 0)
    {   err->retcode  = PERMERR;
	err->errclass = SYSERR;
	err->errvalue = DS_SHD_FILE_ERR;
	DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s%d"),GDS_S_GENERAL,
		svc_c_debug1, GDS_S_UNLINK_ERR, d21shd_file_name, errno));
				/* unlock lock-file                   */
	fcntl(d21lock_no, F_SETLKW, &d21lock_arg);
	d21lock_no = -1;
	D27_FCLOSE(svc_c_debug1, d21fp_lock);
	d21fp_lock = (FILE *) 0;
        DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,err->errvalue));
	return (D2_ERROR);
    }
    if (link(tmp_file, d21shd_file_name) < 0)
    {   err->retcode  = PERMERR;
	err->errclass = SYSERR;
	err->errvalue = DS_SHD_FILE_ERR;
	DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s%d"),GDS_S_GENERAL,
		svc_c_debug1, GDS_S_LINK_ERR, tmp_file, errno));
				/* unlock lock-file                   */
	fcntl(d21lock_no, F_SETLKW, &d21lock_arg);
	d21lock_no = -1;
	D27_FCLOSE(svc_c_debug1, d21fp_lock);
	d21fp_lock = (FILE *) 0;
        DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,err->errvalue));
	return (D2_ERROR);
    }
}

				/* delete temporary file              */
if (unlink(tmp_file) < 0)
{   err->retcode  = PERMERR;
    err->errclass = SYSERR;
    err->errvalue = DS_SHD_FILE_ERR;
    DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s%d"),GDS_S_GENERAL,
		svc_c_debug1, GDS_S_UNLINK_ERR, tmp_file, errno));
    ret_value = D2_ERROR;
}

if( ret_value == D2_NOERROR
    && ((job_counter == 1 && upd_mode == REMOVE) || upd_mode == REMOVE_ALL) )
    d20_turn_shadowing_job( dir_id, D2_OFF );

				/* unlock lock-file                   */
fcntl(d21lock_no, F_SETLKW, &d21lock_arg);
d21lock_no = -1;
fclose(d21fp_lock);
d21fp_lock = (FILE *) 0;

DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,err->errvalue));
return(ret_value);
}

/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  d21d007_update_job()                                   */
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

static d2_ret_val d21d007_update_job(
  char * path,
  signed32 dir_id,
  signed32 upd_mode,
  Shd_job * job,
  Shd_job * act_job,
  Bool * upd_changed,
  Errcode * err)
{
static char function[] = "d21d007_update_job";
d2_ret_val   ret_value = D2_NOERROR;
Upd_time    *upd_t;
char         command[D27_LFILE_NAME + 20];

DCE_SVC_LOG((GDS_S_ENTRY_FUNCTION_MSG,function));

				/* check state of job to be updated   */
if ((act_job->shd_state & ACT_JOB) == ACT_JOB)
				/* job is active                      */
{   switch ((int) upd_mode)
    {   case DEACTIVATE:
				/* actualize occurencies in update    */
				/* time information                   */
		    d21d013_del_upd_time(&act_job->shd_time);
		    *upd_changed = TRUE;
		    act_job->shd_state |= NOT_ACT_JOB;
		    act_job->shd_state &= ~ACT_JOB;
		    upd_t = &act_job->shd_time;
		    upd_t->upd_t_mode =
		    upd_t->upd_t_mode = upd_t->upd_min = upd_t->upd_hour =
		    upd_t->upd_1d_of_week = upd_t->upd_1hour =
		    upd_t->upd_2d_of_week = upd_t->upd_2hour = 0;
		    break;
	case CHANGE_UPD_TIME:
				/* actualize occurencies in update    */
				/* time information                   */
		    d21d013_del_upd_time(&act_job->shd_time);
		    d21d012_add_upd_time(&job->shd_time);
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
				/* actualize occurencies in update    */
				/* time information                   */
		    d21d012_add_upd_time(&job->shd_time);
		    *upd_changed = TRUE;
		    act_job->shd_state |= ACT_JOB;
		    act_job->shd_state &= ~NOT_ACT_JOB;
		    act_job->shd_time = job->shd_time;
		    break;
	default:    break;
    }
}

if (upd_mode == UPD_IMMEDIATELY)
{                               /* start DSA update propagation process */
    sprintf(command, "%s%s%s -d%d -m2 -n%d &", dcelocal_path, D27_BIN_DIR,
					       D20_DAEMON, dir_id,
					       act_job->shd_nr);
    D27_SYSTEM(svc_c_debug1, command);
}

if (ret_value == D2_ERROR)
{   err->retcode = CALLERR;
    err->errclass = PARERR;
    err->errvalue = DS_INVMODE;
}

DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,err->errvalue));
return(ret_value);
}

/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  d21d020_cre_shd()                                      */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function performs the shadow creation in the destination */
/*      DSA. If will call "handle_subtree" and compares all objects   */
/*      in the internal dump file with all the objects in the         */
/*      destination DSA.                                              */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      Errcode     *err;       error codes                           */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_NOERROR    = function terminates successful                */
/*      D2_ERROR       = error occurred                               */
/*                                                                    */
/*exoff ***************************************************************/

static d2_ret_val d21d020_cre_shd(
  signed16 bind_id,
  Errcode * err,
  char * path,
  char * dsa_name,
  signed32 dir_id,
  char * file_name,
  Shd_job * new_job)
{
D20_pB       pb;
D2_pbhead    head;
D2_c_arg     restore_c_arg;
char         local_dsa[D2_DNL_MAX];
d2_ret_val   ret_value;

				/* perform bind as DSA                */
if ((ret_value = d21d019_master_bind(bind_id, err, path,
				(D2_name_string)dsa_name, dir_id,
				   &pb.d20_B2bind_id, local_dsa)) == D2_NOERROR)
{                               /* create shadows                     */
    pb.d20_Bpbhead = &head;
    head.d2_version = DS_V02;
    pb.d20_Bo_file = file_name;
    pb.d20_B1bind_id = 0;
    pb.d20_Bsubtree = (*new_job->shd_objname == D2_EOS) ? (D2_name_string)"" : 
					(D2_name_string)new_job->shd_objname;
    pb.d20_Bsingle_ob = (new_job->shd_subset == OBJECT) ? TRUE : FALSE;
    pb.d20_B1c_a = &restore_c_arg;
    restore_c_arg.d2_serv_cntrls = D2_NOREFERRAL | D2_USEDSA | D2_DREFALIAS |
				   D2_LOCALSCOPE | D2_DONT_STORE | 
				   D2_PREF_ADM_FUN;
    restore_c_arg.d2_priority = D2_PRIO_LOW ;
    restore_c_arg.d2_time_limit = D2_T_UNLIMITED ;
    restore_c_arg.d2_size_limit = D2_S_UNLIMITED ;
    pb.d20_Bn_file = NULL;
    pb.d20_B2c_a = &restore_c_arg;
    pb.d20_Bdsa = (Name_string)local_dsa;
    pb.d20_Bdir_id = dir_id;

    if                          /* shadow creation fails              */
       (d20_B_handle_subtrees(&pb, (D2_name_string) dsa_name))
    {   err->retcode  = PERMERR;
	err->errclass = SYSERR;
	err->errvalue = pb.d20_Bpbhead->d2_errvalue == D2_NFOUND ?
			DS_SEARCH_ERR : pb.d20_Bpbhead->d2_errvalue;
	ret_value = D2_ERROR;
    }
    else                        /* take time stamp from dumping       */
    {   new_job->shd_t_stamp = pb.d20_Betime;
    }
				/* unbind                             */
    d21d022_unbind(pb.d20_B2bind_id);
}

return(ret_value);
}

/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  d21getjb()                                             */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function read the shadowing job file and returns one     */
/*      job.                                                          */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
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

static d2_ret_val d21getjb(
  Errcode * return_code,
  signed32 mode,
  Shd_job * job)
{
static char function[] = "d21getjb";
Upd_time *upd_t;

DCE_SVC_LOG((GDS_S_ENTRY_FUNCTION_MSG,function));

return_code->retcode  = return_code->errclass = NOERR;
return_code->errvalue = NOERROR;

switch ((int) mode)
{   case READ_FIRST:
		if (d21fp_job != NULL)
		{   rewind(d21fp_job);
		}

				/* read occurrencies of update times  */
		if (D27_FREAD(svc_c_debug1, (char *) d21time_upd, 
			sizeof(signed32), D21_T_OCC, d21fp_job) == 0)
		{   if (feof(d21fp_job) != 0)
				/* EOF reached                        */
		    {   return_code->retcode  = TEMPERR;
			return_code->errclass = SYSERR;
			return_code->errvalue = DS_EOF;
    			DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,
				return_code->errvalue));
			return(D2_ERROR);
		    }
		    else        /* error while reading                */
		    {   return_code->retcode  = PERMERR;
			return_code->errclass = SYSERR;
			return_code->errvalue = DS_SHD_FILE_ERR;
    			DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,
				return_code->errvalue));
			return(D2_ERROR);
		    }
		}

    case READ_NEXT:             /* read entry from shadowing job file */
		upd_t = &job->shd_time;
		switch (fscanf(d21fp_job, D2_RJOB_FORM, &job->shd_state,
			       &job->shd_nr, &upd_t->upd_t_mode,
			       &upd_t->upd_min, &upd_t->upd_hour,
			       &upd_t->upd_1d_of_week, &upd_t->upd_1hour,
			       &upd_t->upd_2d_of_week, &upd_t->upd_2hour,
			       job->shd_objname, &job->shd_subset,
			       job->shd_dsa, &job->shd_idx_dsa,
			       &job->shd_t_stamp))
		{   case D2_RJOBAR:
			     if (strcmp(job->shd_objname, D21_ROOT) == 0)
			     {  job->shd_objname[0] = D2_EOS;
			     }
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

    case TERMINATE:             /* close shadowing job file           */
		if (d21fp_job != NULL)
		{   fclose (d21fp_job);
		    d21fp_job = NULL;
		}
		break;

    default:
		break;
}

DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,return_code->errvalue));
return(D2_NOERROR);
}

