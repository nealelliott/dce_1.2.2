/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21lock.c,v $
 * Revision 1.1.10.2  1996/02/18  19:42:04  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:22:46  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:20:07  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:52:42  root]
 * 
 * Revision 1.1.8.3  1994/05/10  15:50:09  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:14:21  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:06:55  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:13:25  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:35:38  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:36:44  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  10:50:30  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:39:22  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  18:10:34  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:07:01  bbelch]
 * 
 * Revision 1.1.4.3  1992/11/27  15:00:02  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  16:13:03  marrek]
 * 
 * Revision 1.1.4.2  1992/10/12  21:08:54  sommerfeld
 * 	Fix previous change to match what SNI wants.
 * 	[1992/10/09  20:04:46  sommerfeld]
 * 
 * 	HPUX struct flock has fewer fields than SVR4 struct flock.
 * 	Since C guarantees that uninitialized fields will be zeroed, we
 * 	remove the extra zeros from the end of the initalizer list.
 * 	[1992/09/30  21:27:29  sommerfeld]
 * 
 * Revision 1.1.2.2  1992/06/01  20:00:29  melman
 * 	New GDS merged drop
 * 	[1992/05/28  20:43:42  melman]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21lock.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:42:04 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* NAME         : d21lock.o                                           */
/*								      */
/* AUTHOR       : Schmid ,D AP 11                                     */
/* DATE         : 30.01.89                                            */
/*								      */
/*datoff **************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <gdsext.h>
#include <d2delupd.h>
#include <d20proto.h>

/**********************************************************************/
/*								      */
/*		   D E C L A R A T I O N S       DATA		      */
/*								      */
/**********************************************************************/

struct flock d21lock_arg = { F_WRLCK, SEEK_SET, 0L, 0L };
int          d21lock_no = -1;
FILE        *d21fp_lock = (FILE *) NULL;
FILE        *d21fp_job  = (FILE *) NULL;
				/* job file name                      */
char         d21shd_file_name[D27_LFILE_NAME];

/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  d21d008_open_and_lock()                                */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function open and locks the shadowing job (if necessary).*/
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      path        =   path name of directory installation           */
/*      dir_id      =   directory ID                                  */
/*      dsa_jobs    =   TRUE:   DSA shadowing jobs to be handled      */
/*                      FALSE:  CACHE job to be handled               */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      err             error values                                  */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_NOERROR     = The function has been performed successfully.*/
/*      D2_ERROR       = function fails                               */
/*                                                                    */
/*exoff ***************************************************************/

d2_ret_val d21d008_open_and_lock(
  char * path,
  signed32  dir_id,
  Bool dsa_jobs,
  Errcode * err)
{
static char function[] = "d21d008_open_and_lock";
char     lock_file_name[D27_LFILE_NAME];

DCE_SVC_LOG((GDS_S_ENTRY_FUNCTION_MSG,function));

if                              /* lock file not yet locked           */
   (d21fp_lock == (FILE *) 0)
				/* lock "LOCKJOBS" file               */
{   if (dsa_jobs == TRUE)
    {   sprintf(lock_file_name, "%s%s%s%d%s", path, D27_DSA_DIR,
		D27_SUBDSA_DIR, dir_id, D2_LOGJOBS);
    }
    else
    {   sprintf(lock_file_name, "%s%s%s%d%s", path, D27_CACHE_DIR,
		D27_SUBCACHE_DIR, dir_id, D2_LOGJOBS);
    }

				/* open lock file                     */
    if ((d21fp_lock = D27_FOPEN(svc_c_debug1, lock_file_name, "a")) 
	== (FILE *) NULL)
    {   err->retcode  = PERMERR;
	err->errclass = SYSERR;
	err->errvalue = DS_SHD_FILE_ERR;
        DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function, err->errvalue));
	return(D2_ERROR);
    }
    d21lock_no = fileno(d21fp_lock);

				/* lock lock-file                     */
    d21lock_arg.l_type = F_WRLCK;
    if (fcntl(d21lock_no, F_SETLKW, &d21lock_arg) == -1)
    {   err->retcode  = PERMERR;
	err->errclass = SYSERR;
	err->errvalue = DS_SHD_FILE_ERR;
	DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s%d"),GDS_S_GENERAL,
		svc_c_debug1, GDS_S_FCNTL_ERR, F_SETLKW, errno));
	fclose(d21fp_lock);
	d21fp_lock = (FILE *) 0;
        DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,err->errvalue));
	return(D2_ERROR);
    }
    d21lock_arg.l_type = F_UNLCK;

    umask(0);
    if (dsa_jobs == TRUE)
    {   sprintf(d21shd_file_name, "%s%s%s%d%s", path, D27_DSA_DIR,
		D27_SUBDSA_DIR, dir_id, D21_SHD_JOBS);
    }
    else
    {   sprintf(d21shd_file_name, "%s%s%s%d%s", path, D27_CACHE_DIR,
		D27_SUBCACHE_DIR, dir_id, D21_CACHE_JOB);
    }
				/* open shadowing job file            */
    if ((d21fp_job = D27_FOPEN(svc_c_debug1, d21shd_file_name, "r+")) 
	== (FILE *) NULL)
    {   err->retcode  = PERMERR;
	err->errclass = SYSERR;
	err->errvalue = DS_SHD_FILE_ERR;
	fcntl(d21lock_no, F_SETLKW, &d21lock_arg);
	d21lock_no = -1;
	fclose(d21fp_lock);
	d21fp_lock = (FILE *) 0;
        DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,err->errvalue));
	return(D2_ERROR);
    }
}

DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,err->errvalue));
return(D2_NOERROR);
}
