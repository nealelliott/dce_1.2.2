/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_proc.c,v $
 * Revision 1.1.13.2  1996/02/18  00:18:50  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:07:27  marty]
 *
 * Revision 1.1.13.1  1995/12/08  17:53:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:21:02  root]
 * 
 * Revision 1.1.8.5  1994/08/25  20:47:37  sekhar
 * 	Put back SVC messages for fatal_st() and error_st() calls.
 * 	[1994/08/25  20:26:33  sekhar]
 * 
 * Revision 1.1.8.4  1994/08/16  10:36:49  mdf
 * 	Bug fixes from Hewlett Packard
 * 	[1994/08/15  19:51:35  mdf]
 * 
 * Revision 1.1.8.2  1994/08/04  16:13:45  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:11:51  mdf]
 * 
 * 	Not sure what's going on, bsubmit requires a bmerge
 * 	[1993/10/27  16:47:32  mdf]
 * 
 * 	Redirect stdin, stdout, and stderr to /dev/null when becoming
 * 	a daemon so that starting secd via a remote shell will work.
 * 
 * 	Associated defect numbers: OT6076 and CHFts09272
 * 	[1993/10/27  14:39:00  mdf]
 * 
 * Revision 1.1.8.1  1994/06/02  21:22:50  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 
 * 	hp_sec_to_osf_3 drop, fix 486 build problem
 * 	[1994/05/19  14:57:20  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 	[1994/05/18  11:56:21  mdf]
 * 
 * 	HP revision /main/KK/ODESSA_2/1  1994/05/04  20:38 UTC  hanfei
 * 	 merge
 * 
 * 	HP revision /main/KK/hanfei_ch_master/1  1994/05/04  20:15 UTC  hanfei
 * 	merge
 * 
 * Revision 1.1.6.3  1993/09/15  15:50:42  root
 * 	    Loading drop DCE1_0_3b03
 * 
 * Revision 1.1.4.4  1993/02/01  20:36:51  hinman
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  16:15:32  hinman]
 * 
 * Revision 1.1.4.3  1992/12/29  16:36:14  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:25:08  zeliff]
 * 	Revision 1.1.6.2  1993/01/11  16:13:06  hinman
 * 	[hinman] - Check in merged SNI version
 * 
 * Revision 9.5.1.3  92/11/13  12:55:21  meg
 * 	[meg] - Called setsid() instead of setpgrp()
 * 
 * Revision 9.5.3.2  92/11/13  12:39:34  meg
 * 	called setsid instead of setpgrp on SNI_SVR4_POSIX
 * 
 * Revision 9.5.1.2  92/11/05  17:35:55  hinman
 * 	Auto checkin of OSF rev 1.1.4.2
 * 
 * Revision 1.1.4.2  1992/10/20  15:51:12  sommerfeld
 * 	Change _HPUX_SOURCE to __hpux in #ifdef
 * 	[1992/10/05  16:18:53  sommerfeld]
 * 
 * 	call setsid rather than setpgrp on HP-UX.
 * 	[1992/10/01  19:48:20  sommerfeld]
 * 
 * Revision 1.1.2.2  1992/03/30  16:35:25  ahop
 * 	 support locksmith mode
 * 	[1992/03/28  00:00:54  ahop]
 * 
 * Revision 1.1  1992/01/19  14:43:51  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rs_proc.c V=2 05/10/91 //littl/prgy/src/server/rs
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 *      Registry Server - mainline process control
 * 
 *  Deal with process creation and backgrounding.  This code is isolated from
 *  the server mainline to allow it to avoid conflicts with cma system call
 *  wrappers.  We need to perform the initial process fork outside the CMA
 *  context.
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)rs_proc.c	2 - 05/10/91";
#endif

/*
 * Make sure we don't pick up CMA wrappers for i/o functions
 */
#ifdef OSF_DCE
#   undef OSF_DCE
#endif
#define _CMA_NOWRAPPERS_

#include <dce/dce.h>
#include <dce/nbase.h>
#include <rs_proc.h>
#include <args.h>
#include <fcntl.h>

#include <un_io.h>
#include <dce/secsts.h>
#include <sec_svc.h>

static  int         pipe_fd[2];
static  boolean32   be_server = true;

/*  When secd is run in the background, stdout, stderr 
 *  and stdin are first closed. Then stdout and stderr
 *  are redirected to a log file. This is so that
 *  secd can be started remotely via remsh/rsh . 
 *  The log file name is obtained by concatenating
 *  dcelocal_path with SECD_LOG_FILE .
 *
 *  secd must also change to a well known directory 
 *  so that core dumps can be easily located . This
 *  directory is obtained by concatenating dcelocal_path
 *  with SECD_LOG_FILE .
 *
 *  The definitions ensure that the log file and the 
 *  core dump are in the same directory.
 *
 */

#define SECD_DIRECTORY  "/var/security"
#define SECD_LOGFILE    DCE_CONCAT(SECD_DIRECTORY, "/secd.log")

boolean32 rs_process_make_server
  (
    int     *argc,
    char    *argv[]
  )
{
    int             i;
    int             j;
    int             arg_count;
    int             child_id;
    int             pgid;
    int 	    fd;
    char            *logfile ;
    char            *secd_dir;

    /*
     * Look for the "-debug" and "-locksmith" arguments -
     * this indicates we should not put the
     * server in the background.
     */

    arg_count = *argc;
    for (i = 1, j = 1; i < arg_count; i++) {
        argv[j] = argv[i];
        if (match_command("-debug", argv[i], 2)) {
            be_server = false;
            (*argc)--;
        } else if (match_command("-locksmith", argv[i], 7)) {
            be_server = false;
            j++;
        } else {
            j++;
        }
    }

    if (be_server) {
        if (pipe(pipe_fd) != 0)
            dce_svc_printf(SEC_RS_PIPE_NOT_CREATED_MSG);

        child_id = fork();
        /*
         * Parent Branch - just wait for the child to be ready to service requests
         */
        if (child_id != 0) {
            char    buf;
	
            close(pipe_fd[1]);
            read(pipe_fd[0], &buf, sizeof(buf));
            exit(0);
        } else {
            /*
             * Child Branch - Normal processing
             */             
            close(pipe_fd[0]);

            /*
             * Dis-associate ourself from the controlling tty (to avoid hangup
             * signals).
             */

#if defined(__hpux) || defined(SNI_SVR4_POSIX) /* !!! should really be "ifdef POSIX" here.. */
	    pgid = setsid();
#else
            pgid = setpgrp(0, getpid());
#endif
            /* check for malloc() before closing file descriptors 
             * so malloc() failure will be logged */
            logfile  = (char *) malloc(   strlen(dcelocal_path) 
                                       + strlen(SECD_LOGFILE) + 1);
            secd_dir = (char *) malloc(   strlen(dcelocal_path) 
                                       + strlen(SECD_DIRECTORY) + 1);
            if (logfile == NULL || secd_dir == NULL)
                dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                               sec_s_rs,
                               svc_c_sev_fatal | svc_c_action_abort,
                               sec_s_no_memory);
            
            /* all of the following required so secd can be started remotely */
            if ((fd = open("/dev/null", O_RDWR,
                       S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)) != -1)
            {
                (void)dup2(fd,0);
                (void)dup2(fd,1);
                (void)dup2(fd,2);
                (void)close(fd);
            }
            /* redirect stdout, stderr to a log file */
                   
            strcpy(logfile, dcelocal_path);
            strcat(logfile, SECD_LOGFILE);
            if ((fd = open(logfile,
                           O_WRONLY|O_CREAT|O_TRUNC,
                           S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)) != -1)
            {
                (void)dup2(fd,2); 
                (void)dup2(fd,1);
            }
            close(fd);
            free(logfile);
 
            /* cd to a known location */
            strcpy(secd_dir, dcelocal_path); 
            strcat(secd_dir, SECD_DIRECTORY);
            chdir(secd_dir);
            free(secd_dir);
        }
    }
    
    return (be_server);
}

void rs_process_ready_to_listen (
    void
)
{
    if (be_server) {
	/*
	 * Inform parent that child is ready to process requests (we simply close
	 * the pipe - which will cause the parent to wake up)
	 */
	close(pipe_fd[1]);
    }
}
	
