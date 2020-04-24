/*
 * Copyright 1990 Open Software Foundation (OSF)
 * Copyright 1990 Unix International (UI)
 * Copyright 1990 X/Open Company Limited (X/Open)
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of OSF, UI or X/Open not be used in 
 * advertising or publicity pertaining to distribution of the software 
 * without specific, written prior permission.  OSF, UI and X/Open make 
 * no representations about the suitability of this software for any purpose.  
 * It is provided "as is" without express or implied warranty.
 *
 * OSF, UI and X/Open DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, 
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO 
 * EVENT SHALL OSF, UI or X/Open BE LIABLE FOR ANY SPECIAL, INDIRECT OR 
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF 
 * USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR 
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR 
 * PERFORMANCE OF THIS SOFTWARE.
 *
 */

#ifndef lint
static char sccsid[] = "@(#)error.c	1.12 (93/01/24) DTET release 1.0b";
#endif

/************************************************************************

SCCS:           @(#)error.c    1.12 1/24/93
NAME:		error.c
PRODUCT:        DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:         This source file is based on Release 1.9 of the TET
DATE CREATED:   February 1992
CONTENTS: 	signal handling, shutdown, cleanup and other error functions

MODIFICATIONS:
		DTET Development
		David G. Sawyer
		John-Paul Leyland
		UniSoft Ltd.

************************************************************************/

#include <tcc_env.h>
#include <tcc_mac.h>
#include <dtet/tet_jrnl.h>
#include <tcc_prot.h>


int remote_tmp_dirs = FALSE; /* Have the remote tmp dirs been created ? */

/*
 *  Something has gone wrong - we now attempt to exit gracefully. 
 *  NOTE: No BAIL_OUTS are permitted in this function !
 */
void tet_shutdown()
{
    int exit_val;

    DBUG(DBGFDESC) (stderr, "tet_shutdown()\n");

    tet_cleanup();

    if(setup_complete(QUERY))
        exit_val = EXIT_BAD_MISC;
    else
        exit_val = EXIT_BAD_INIT;

    DBUG(DBGFINFO) (stderr, "Tcc exiting via shutdown with exit value of %d\n", exit_val);

    exit(exit_val);
}


/*
 *  Timeout during a Test Case - SIGALRM has been generated.
 */
/*ARGSUSED*/
void exec_sig_hdlr(throw_away)
int throw_away;
{
    int statloc,rc;


    DBUG(DBGSIGNALS) (stderr, "exec_sig_hdlr(%d) cpid: %d\n", throw_away, cpid);

	
    if (cpid != 0)
    {
        DBUG(DBGSIGNALS) (stderr, "Sending signal SIGTERM to process ID %d\n", cpid);
        (void) kill((pid_t)cpid,SIGTERM);

        rc = waitpid((pid_t)cpid,&statloc,0);
        COMPLAIN_ON(rc,"waitpid child in timeout handler ");

        cpid = 0;

        siglongjmp(jmpbuf,0);
    }

}


/*
 *  Timeout during a Tool Execution - SIGALRM has been generated.
 */
/* ARGSUSED */
void tool_sig_hdlr(throw_away)
int throw_away;
{
    int statloc;
    int rc;


    DBUG(DBGSIGNALS) (stderr, "tool_sig_hdlr(%d) cpid: %d\n", throw_away, cpid);


    if (cpid != 0)
    {
        DBUG(DBGSIGNALS) (stderr, "Sending signal SIGTERM to process ID %d\n", cpid);
        (void) kill((pid_t)cpid,SIGTERM);

        rc = waitpid((pid_t)cpid,&statloc,0);
        COMPLAIN_ON(rc,"waitpid child in timeout handler ");

        cpid = 0;

        siglongjmp(jmpbuf,0);
    }
}


/*
 *  What do we do in the event of a SIGINT - we don't panic thats what we do
 */
/* ARGSUSED */
void siginthdlr(throw_away)
int throw_away;
{
    int rc;


    DBUG(DBGSIGNALS) (stderr, "siginthdlr(%d) cpid: %d\n", throw_away, cpid);

    /* reset alarm */
    (void) alarm(0);


    /*
     * The interrupted flag is needed to check that a child recived a
     * SIGTERM due to the TCC and not some other source.
     */

    if(cpid != 0) /* A child process was forked other than those for
		   * the implimentation of parallel and remote testcases.
		   */
    {
        DBUG(DBGSIGNALS) (stderr, "Sending signal SIGTERM to process ID %d\n", cpid);
        rc = kill((pid_t) cpid, SIGTERM);
        if(rc == -1 && errno == ESRCH)
            interrupted = 1;
        else
            interrupted = 2;

        (void) jnl_user_abort("User Abort");
        (void) fprintf(stderr,"\n**** TCC USER INTERRUPT ****\n\n");
    }
    else
    {
        /* propagate signal to other processes if they exist */
        if(num_remote_tc > 0)
        {
            signal_group(remote_tc, num_remote_tc, SIGINT);
        }
        else if(num_parallel_tc > 0)
        {
            signal_group(parallel_tc, num_parallel_tc, SIGINT);
        }
        else
        {
            (void) fprintf(stderr,"\nNo child process available to interrupt !\n\nTry SIGQUIT if you wish to abort the tcc.\n\n");
	}
    }
}


/* 
 *  What we do when we recieve any signal other than a SIGINT or SIGALRM
 */
void sighdlr(throw_away)
int throw_away;
{

    DBUG(DBGSIGNALS) (stderr, "sighdlr(%d) cpid: %d\n", throw_away, cpid);


    if(cpid != 0)
    {
        DBUG(DBGSIGNALS) (stderr, "Sending signal SIGTERM to process ID %d\n", cpid);
        (void) kill((pid_t)cpid,SIGTERM);
    }

    (void) sprintf(error_mesg,"**** TCC SHUTDOWN DUE TO SIGNAL %d ****\n",throw_away);

    (void) fprintf(stderr,"%s", error_mesg);
    (void) jnl_tc_message(error_mesg);

    if(num_remote_tc > 0)
    {
        signal_group(remote_tc, num_remote_tc, throw_away);
    }
    else if(num_parallel_tc > 0)
    {
        signal_group(parallel_tc, num_parallel_tc, throw_away);
    }

    tet_shutdown();
}


/*
 *  Try and remove any lock files and any temporary files.
 *
 *  NOTE: No BAIL_OUTS are permitted in this function !
 */
void tet_cleanup()
{
    int count;
    char *dir_to_rm;
    static int been_here = 0;

    DBUG(DBGFDESC) (stderr, "tet_cleanup()\n");

    if(been_here != 0)
    {
	if(been_here == 1)
	{
	    ++been_here;

	    (void) fprintf(stderr,"*** TCC FATAL LOOPING ERROR : recursive call to tet_cleanup() ***\n");
	}
	else
	{
	    /* AArrrrrrgggghhh ! */
	    exit(EXIT_BAD_LOOP);
	}
    }
    else
    {
        ++been_here;

        /* release any lock files we have */
        release_locks();

        /* If we are logged on to any remote system, logoff */
        (void) logonoff_sys(LOGOFF, -1);

        /* Check this is the parent (we only want this to be done once) */

        if(mypid == 0)
        {
	    /* Remove the temporary directory on the master system */
            dir_to_rm = get_temp_dir(FALSE, MASTER);
	    if(dir_to_rm != NULL)
	    {
                (void) remove_dir(dir_to_rm, FALSE, MASTER);
	    }

	    /* Remove the temporary directory on the slave systems */
	    if((total_sysid > 0) && (remote_tmp_dirs == TRUE))
	    {
	        /* Logon to TCCDs so that temporary dirs can be removed */
	        (void) start_tccd(total_sysid, all_sysids);

	        if(remote_tmp_dirs == TRUE)
		{
		    /* Remove the temporary directory on each system */
	            for(count = 0; count < total_sysid; count++)
	            {
	                if( (all_sysids[count].sysid != MASTER)
	 	                     && (all_sysids[count].logged_on == TRUE))
	                {
       	                    dir_to_rm = get_temp_dir(TRUE, all_sysids[count].sysid);
	                    if(dir_to_rm != NULL)
	                        (void) remove_dir(dir_to_rm, TRUE, all_sysids[count].sysid);
		        }
	            }

		    remote_tmp_dirs = FALSE;
		}

		/* Remove the results sub-dir if it's empty */
	        for(count = 0; count < total_sysid; count++)
	        {
	            if( (all_sysids[count].sysid != MASTER)
	 	                     && (all_sysids[count].logged_on == TRUE))
	            {
       	                dir_to_rm = rem_results_dir((char *) NULL, all_sysids[count].sysid, FALSE);
	                if(dir_to_rm != NULL)
	                    (void) remove_dir(dir_to_rm, TRUE, all_sysids[count].sysid);
		    }
	        }

	        /* Logoff TCCD's, XRESD and SYNCD */
                end_dtcc(total_sysid, all_sysids);

	        clear_sysid_store(&all_sysids, total_sysid);
	        total_sysid = 0;
	    }

            /* Get back to where we started */
            (void) chdir(start_dir);
        }
    }
}


/*
 *  Sort out format  of error messages.
 *  NOTE: No BAIL_OUTS are permitted in this function !
 */
void tcc_error(fmt, flag, err_file, line_no)
char *fmt; 
int flag;
char *err_file;
int line_no;
{
    char jnl_fmt[BIG_BUFFER];
    char pidbuf[512];
    extern int gmysysid;

    DBUG(DBGFDESC)(stderr,"tcc_error(%s,%d,%s,%d)\n", fmt, flag, err_file, line_no);
    if(gnumsys != 0 && gmysysid != -1 && mypid != 0)
    {
	/* If we are in a remote thread indicate which thread this is */
	(void) sprintf(pidbuf,"(%05d) [%03d]", (int) getpid(), gmysysid);
    }
    else
    {
	(void) sprintf(pidbuf,"(%05d)", (int) getpid());
    }

    if(flag == FATAL)
    {
	(void) fprintf(stderr,"tcc %s fatal error (%s, %d): %s\n", pidbuf, 
							err_file, line_no, fmt);
    }

    (void) sprintf(jnl_fmt, "tcc %s (%s, %d): %s", pidbuf, err_file, line_no, 
									fmt);

    /* Make sure the line is not too long */
    jnl_fmt[JNL_LINE_MAX-6] = '\0';

    /* Make sure the journal line ends with a newline */
    jnl_fmt[strlen(jnl_fmt) - 1] = '\n';

    /* If the journal file descriptor is not set the error will go to stderr */
    (void) jnl_tc_message(jnl_fmt);
}


void dtccerror(errnum, file, lineno, s1, s2)
int errnum, lineno;
char *file, *s1, *s2;
{
	register char *p = error_mesg;
	extern int sys_nerr;
	extern char *sys_errlist[];

	(void) sprintf(p, "%s", s1);
	p += strlen(p);
	if (s2 && *s2) {
		(void) sprintf(p, " %s", s2);
		p += strlen(p);
	}
	if (errnum > 0 && errnum < sys_nerr)
		(void) sprintf(p, ": %s", sys_errlist[errnum]);

	tcc_error(error_mesg, NONFATAL, file, lineno);
}

void dtccfatal(errnum, file, lineno, s1, s2)
int errnum, lineno;
char *file, *s1, *s2;
{
	dtccerror(errnum, file, lineno, s1, s2);

	tet_shutdown();
}

