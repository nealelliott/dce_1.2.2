/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: d27svc.h,v $
 * Revision 1.1.4.2  1996/02/18  23:35:54  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:26:08  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:00:58  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:13:58  root]
 * 
 * Revision 1.1.2.5  1994/07/06  15:07:32  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:16:23  marrek]
 * 
 * Revision 1.1.2.4  1994/06/09  18:46:18  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:19:22  devsrc]
 * 
 * Revision 1.1.2.3  1994/05/10  15:55:53  marrek
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/04  14:24:03  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:38:08  marrek]
 * 
 * Revision 1.1.2.2  1994/03/23  15:18:26  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:31:26  keutel]
 * 
 * Revision 1.1.2.1  1994/02/22  19:21:15  marrek
 * 	Change rcsid string format.
 * 	[1994/02/09  08:12:03  marrek]
 * 
 * 	Creation for dce1.1.
 * 	[1994/02/02  10:53:24  marrek]
 * 
 * $EndLog$
 */
/**********************************************************************/
/*                                                                    */
/* Component    : GDS                                                 */
/*                                                                    */
/* File name    : d27svc.h                                            */
/*                                                                    */
/* Description  : This Include-File contains "typedefs" and "defines" */
/*                used for the serviceability in GDS.                 */
/*                                                                    */
/* Date         : 11/18/1993                                          */
/*                                                                    */
/* Author       : E. Kraemer, BU BA NM12                              */
/*                                                                    */
/**********************************************************************/

#ifndef _D27SVC_H
#define _D27SVC_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char d27svc_rcsid[] = "@(#)$RCSfile: d27svc.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:35:54 $";
#endif

#include <stdio.h>
#include <unistd.h>

			    /* constants for exit status              */
			    /* Do not use values > 31                 */
#define D27_INV_PAR       1 /* invalid command line parameter         */
#define D27_INV_DIR_ID    2 /* invalid directory ID                   */
#define D27_CHDIR_ERR     3 /* program cannot change directory        */
#define D27_FORK_ERR      4 /* program cannot fork                    */

#define D27_CHK_KEY_ERR   9 /* check key error. Actual error code     */
			    /* should be added. Values up to 20 are   */
			    /* reserved                               */

			    /* segment for cache                      */
#define D27_INV_RES	 10 /* invalid -r switch                      */
#define D27_INV_PRIV	 11 /* invalid -p switch                      */
#define D27_INV_NORM	 12 /* invalid -n switch                      */
#define D27_INV_CPATH	 13 /* invalid -b switch value                */
#define D27_INV_SWITCH	 14 /* invalid switch                         */
#define D27_INV_VALUE	 15 /* invalid switch value                   */

			    /* segment for stubs                      */
#define D27_INV_MXIASSOC 20 /* invalid count of maximum IPC assoc.    */
#define D27_INV_MXRASSOC 21 /* invalid count of maximum ROS assoc.    */

			    /* segment for DSA                        */
#define D27_NO_AUTH_MECH  22/* no authentication mechanism given      */
#define D27_INV_AUTH_MECH 23/* invalid authentication mechanism       */

			    /* segment for monitor                    */
#define D27_INV_IVTIME	 30 /* invalid interval time                  */

			    /* macros for the call of system functions*/
#define D27_MALLOC(attributes,len)  \
	d27_malloc(__FILE__,__LINE__,attributes,len)

#define D27_CALLOC(attributes,nitem,size) \
	d27_calloc(__FILE__,__LINE__,attributes,nitem,size)

#define D27_REALLOC(attributes,ptr,len) \
	d27_realloc(__FILE__,__LINE__,attributes,ptr,len)

#define D27_TSEARCH(attributes,key,rootp,compar) \
	d27_tsearch(__FILE__,__LINE__,attributes,key, \
		    rootp,compar)

#define D27_FOPEN(attributes,name,mode) \
	d27_fopen(__FILE__,__LINE__,attributes,name,mode)

#define D27_FCLOSE(attributes,fp) \
	d27_fclose(__FILE__,__LINE__,attributes,fp)

#define D27_FFLUSH(attributes,fp) \
	d27_fflush(__FILE__,__LINE__,attributes,fp)

#define D27_FPCHECK(attributes,fp) \
	d27_fpcheck(__FILE__,__LINE__,attributes,fp)

#define D27_FGETS(attributes,ptr,len,fp) \
	d27_fgets(__FILE__,__LINE__,attributes,ptr,len,fp)

#define D27_FREAD(attributes,ptr,size,nitem,fp) \
	d27_fread(__FILE__,__LINE__,attributes,ptr,size, \
		  nitem,fp)

#define D27_FWRITE(attributes,ptr,size,nitem,fp) \
	d27_fwrite(__FILE__,__LINE__,attributes,ptr,size,\
		   nitem,fp)

#define D27_FORK(attributes) \
	d27_fork(__FILE__,__LINE__,attributes)

#define D27_SYSTEM(attributes,cmd) \
	d27_system(__FILE__,__LINE__,attributes,cmd)

#define D27_WAIT(attributes,stat_loc) \
	d27_wait(__FILE__,__LINE__,attributes,stat_loc)

#define D27_SIGACTION(attributes,sig,act,oact) \
	d27_svcsigaction(__FILE__,__LINE__,attributes,sig,act,oact)

#define D27_EXECV(attributes,path,argv) \
	d27_execv(__FILE__,__LINE__,attributes,path,argv)

#define D27_022_ENTER_TIME_TRACE(message_index,text,nr_access1,nr_access2) \
	d27_022_enter_time_trace(__FILE__,__LINE__,message_index,text, \
		nr_access1,nr_access2)

			    /* declarations for external data         */
			    /* serviceability handle                  */
extern dce_svc_handle_t gds_svc_handle;

			/* exported by d27svc.c                       */

extern void     d27_001_svc_init(int argc, char **argv, error_status_t *st);
extern void     d27_003_reset_routing(char *where, error_status_t *st);

			    /* declarations of system call replacement*/
extern void     *d27_malloc(char *file, int line, unsigned32 attributes,
		    size_t len);
extern void     *d27_calloc(char *file, int line, unsigned32 attributes,
		    size_t count, size_t size);
extern void     *d27_realloc(char *file, int line, unsigned32 attributes,
		    void *mem, size_t size);

extern void     *d27_tsearch(char *file, int line, unsigned32 attributes,
		    const void *key, void **rootp,
		    int (*compar)(const void *, const void *));

extern FILE     *d27_fopen(char *file, int line, unsigned32 attributes,
		    const char *name, const char *mode);
extern int      d27_fclose(char *file, int line, unsigned32 attributes,
		    FILE *fp);
extern int      d27_fflush(char *file, int line, unsigned32 attributes,
		    FILE *fp);
extern void     d27_fpcheck(char *file, int line, unsigned32 attributes,
		    FILE *fp);
extern char     *d27_fgets(char *file, int line, unsigned32 attributes,
		    char *ptr, int n_bytes, FILE *fp);
extern size_t   d27_fread(char *file, int line, unsigned32 attributes,
		    void *ptr, size_t size, size_t nitems, FILE *fp);
extern size_t   d27_fwrite(char *file, int line, unsigned32 attributes,
		    const void *ptr, size_t size, size_t nitems, FILE *fp);
extern int      d27_fprintf(char *file, int line, unsigned32 attributes,
		    FILE *fp, const char *, ...);

extern pid_t    d27_fork(char *file, int line, unsigned32 attributes);
extern int      d27_svcsigaction(char *file, int line, unsigned32 attributes,
		    int sig, const struct sigaction *act,
		    struct sigaction *oact);
extern pid_t    d27_wait(char *file, int line, unsigned32 attributes,
		    int *stat_loc);
extern int      d27_system(char *file, int line, unsigned32 attributes,
		    const char *cmd);
extern int      d27_execv(char *file, int line, unsigned32 attributes,
		    const char *path, char *const *argv);

			/* exported by d27pfm.c                       */

extern void     d27_021_reset_time_trace(void);
extern void     d27_022_enter_time_trace(char *file, int line,
	unsigned32 message_index, char *text, signed32 nr_access1,
	signed32 nr_access2);
extern void     d27_023_dump_time_trace(void);

#endif
