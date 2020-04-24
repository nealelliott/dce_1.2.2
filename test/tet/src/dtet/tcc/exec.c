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
static char sccsid[] = "@(#)exec.c	1.9 (93/01/25) DTET release 1.0b";
#endif

/************************************************************************

SCCS:           @(#)exec.c    1.9 1/25/93
NAME:		exec.c
PRODUCT:        DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:         This source file is based on Release 1.10 of the TET
DATE CREATED:   February 1992
CONTENTS: 	Functions relating to the execution mode of the tcc

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


static char linein[JNL_LINE_MAX]; /* Misc input line */
int  exec_ctr = 0;         /* The execution counter */
static char lineout[BIG_BUFFER];  /* The line to be output to the journal */
extern char zeetime[];     /* Time array */

int exec_all_flag = 0;	   /* Used to tell exec_tc() to add "all" par. */

sigjmp_buf jmpbuf;         /* this is the allocated jmpbuf */

static ENV_T *environment; /* A pointer to the environment variables for this
			    * particular execution.
			    */



/*
 * Execute a test case
 */
int exec_tc(fnbc_p, remote_flag, sysid, group_flag, group_pipe_fd, override_eip)
char * fnbc_p;
int    remote_flag;
int    sysid;
int    group_flag;
int *  group_pipe_fd;
int    override_eip;
{
    char *command_name_p, *arg_p, *ic_end_mark_p;
    char *ic_start_mark_p, *ic_string, *output_capture_p;
    char *exec_in_place, tc_dir[_POSIX_PATH_MAX];
    char source_path[_POSIX_PATH_MAX];
    char exe_dir[_POSIX_PATH_MAX];
    char *rm_dir;
    char *ts_root;
    int exec_rc;
    int tcend_status;
    struct sigaction on_alarm;
    int ret_val;


    DBUG(DBGFDESC) (stderr,"exec_tc(%s, %d, %d, %d, group_pipe_fd, %d)\n",fnbc_p, remote_flag, sysid, group_flag, override_eip);

    /* get the relevant environment for the exec */
    environment = get_env_ptr(EXEC, remote_flag, sysid);

    command_name_p = store_str(fnbc_p);

    if(mode_message(QUERY))
    {
        (void) get_time();
        (void) fprintf(stdout,"%s  Execute  %s", zeetime, fnbc_p);
	if(remote_flag == FALSE)
	    (void) fprintf(stdout,"\n");
	else
	    (void) fprintf(stdout," on system %3.3d\n", sysid);
	(void) fflush(stdout);
    }

    /* extract Invocable Component name(s) */
    /* search for beginning "{" */
    ic_start_mark_p = strchr(command_name_p,IC_START_MARK);
    if( ic_start_mark_p != NULL)
    {
        ic_string = (char *) TET_MALLOC(strlen(ic_start_mark_p) + 2);
        (void) strcpy(ic_string, ic_start_mark_p);

        /* Make command_name_p the right length */
        *ic_start_mark_p = '\0';
        
        ic_start_mark_p = ic_string;

        /* Get a nice list of ICs with no surrounding braces */
        ic_start_mark_p++;
        ic_end_mark_p = strchr(ic_string,IC_END_MARK);
        if(ic_end_mark_p == NULL)
            BAIL_OUT2("No closing brace for IC list\n");
        *ic_end_mark_p = '\0';
    }

    if (ic_start_mark_p == NULL)
        arg_p = NULL;
    else
    {
        arg_p = store_str(ic_start_mark_p);
	TET_FREE((void *) ic_string);
    }

    /* 
     *  If there is a specific IC list then set the journal entry buffer.
     *  If set the exec_all_flag gives a non-specific range. ie: From X -> END
     */
    if(arg_p != NULL && exec_all_flag == 0)
        (void) sprintf(line,", ICs {%s}", arg_p);
    else
        line[0] = '\0';

    /* Make an entry in the journal to say what we're about to exec */
    (void) jnl_entry_invoke_tc(exec_ctr,command_name_p,line,sysid);

    /* reset journal entry line buffer*/
    line[0] = '\0';

    /* Is exec in place set for execution */
    if(override_eip != FALSE)
    {

	DBUG(DBGFINFO) (stderr,"Overriding TET_EXEC_IN_PLACE.\n");

	eip_set = FALSE;
    }
    else
    {
        exec_in_place = get_tetenv("TET_EXEC_IN_PLACE",environment);
        if( (exec_in_place == NULL)
	     ||
	    ((*exec_in_place == 'F') || (*exec_in_place == 'f')))
            eip_set = FALSE;
        else
            eip_set = TRUE;
    }

    /* Is output capture set for execution */
    output_capture_p = get_tetenv("TET_OUTPUT_CAPTURE",environment);
    if ( (output_capture_p != NULL) && ((*output_capture_p == 'T') || 
                         (*output_capture_p == 't')) )
        oc_set = TRUE;
    else
        oc_set = FALSE;

    /* tcdir = scenario file pathname - test case file name */
    (void) strcpy(tc_dir, fnbc_p);
    *(strrchr(tc_dir, DIR_SEP_CHAR)) = '\0';

    /* Where is the test case executable to be found ? */
    tet_exepath(source_path, tc_dir, remote_flag, sysid);

    if(tet_chdir(source_path,remote_flag,sysid) == FAILURE)
    {
        (void) sprintf(error_mesg,"Can't change to test case dir: %s\n", source_path);
        ERROR(error_mesg);
        return(FAILURE);
    }

    if ( eip_set == FALSE)
    {
        /*
         *  Because we don't want to execute the TCs in place we do a recursive
         *  copy of the execution directory to the temp directory 
         *  Locking is taken care of in this function aswell.
	 *
         * Make a note of the directory were in so that we can remove it
         * after we have run the TC. This means we won't start eating up
         * loads of disc space.
         */
        rm_dir = do_copy(source_path, tc_dir, group_flag, remote_flag, sysid);
        if(rm_dir == NULL)
            return(FAILURE);

        /* Set the execution directory name */
	(void) strcpy(exe_dir, rm_dir);
	(void) strcat(exe_dir, tc_dir);
    }
    else
    {
        /* We want to exec in place so we need to get an exclusive lock */
        if(get_alt_exec_dir(remote_flag, sysid) != NULL)
        {
            ret_val = obtain_lock(EXCLUSIVE, tc_dir, remote_flag, sysid, TRUE);
        }
        else
        {
            ret_val = obtain_lock(EXCLUSIVE, tc_dir, remote_flag, sysid, FALSE);
        }
        if(ret_val == FAILURE)
        {
            jnl_tc_end(exec_ctr,TET_ESTAT_LOCK,"TC End");
            return(FAILURE);
        }

        /* Set the execution directory name */
	(void) strcpy(exe_dir, source_path);
    }

    /* make sure that no old tet_xres file exists */
    (void) tet_unlink(XRES_FILENAME, remote_flag, sysid); 

    /* make sure that no old tet_captured file exists */
    (void) tet_unlink(OC_FILENAME, remote_flag, sysid); 

    if(remote_flag == FALSE || sysid == MASTER)
    {
        /* make sure that no old tet_combined file exists */
        (void) tet_unlink(COM_XRES_FILENAME, remote_flag, sysid); 
    }

    if(group_flag == TRUE)
    {
	/* Send signal to parent indicating that child is ready to contine */	
	signal_parent(group_pipe_fd);

	/* Wait for signal from parent before continuing */
	child_wait();

	/* Child will get to this point if it receives a SIGUSR1 */
    }


    /* Set the timeout alarm call */
    on_alarm.sa_handler = exec_sig_hdlr;
    on_alarm.sa_flags = 0;
    (void) sigaction(SIGALRM, &on_alarm, (struct sigaction *)NULL);
    (void) alarm((unsigned int)gtimeout());

    /* This is the function that actually execs the TC */
    exec_rc = do_exec_tc(command_name_p,arg_p,remote_flag,sysid,&tcend_status, exe_dir);

    /* if things haven't been reset already */
    if(cpid != 0)
    {
        /* No more child */
        cpid = 0;

        /* Reset the timeout alarm call */
        (void) alarm(0);
    }
    on_alarm.sa_handler = SIG_DFL;
    on_alarm.sa_flags = 0; 
    (void) sigaction(SIGALRM, &on_alarm, (struct sigaction *)NULL);

    /* Save any files that we listed as wanting to be saved */
    savefiles(command_name_p, remote_flag, sysid);

    if (eip_set == FALSE)
    {
        /* Need to get rid of the directory we copied across */
        (void) remove_dir(rm_dir, remote_flag, sysid);
	TET_FREE(rm_dir);
    }    
    else
    {
	release_locks();
    }

    /* Append the Test Case finish line to the journal */
    jnl_tc_end(exec_ctr,tcend_status,line);

    if((ts_root = get_ts_root(remote_flag, sysid)) != (char *) NULL)
    {
        /* Reposition ourselves in the test suite root directory */
        if(tet_chdir(ts_root,remote_flag,sysid) == FAILURE)
        {
	    BAIL_OUT2("Unable to change to test suite root.");
        }
    }

    if (arg_p != NULL)
        TET_FREE((void *)arg_p);

    TET_FREE((void *) command_name_p);

    return(exec_rc);
}


/*
 * Start a test case.
 * Arguments:
 * pointer to pathname of test case
 * pointer to list of Invocable Component(s)
 * Flag to say whether this is a DTC
 */
int do_exec_tc(pathname, arg_p, remote, sysid, tcend_status, exe_dir)
char * pathname; 
char * arg_p;
int    remote;
int    sysid;
int  * tcend_status;
char * exe_dir;
{
    int statloc;
    int rc, argc = 0, count = 0, waitstatus, user_interrupt = FALSE;
    static int return_val, timed_out;
    char *exec_file_p;
    char *argv[16], *tmp_ptr, *tc_name;
    char *str1, *str2, *str3;
    FILE *res_fp, *exec_fp;
    char *res_filename;
    char *file;
    char full_path[_POSIX_PATH_MAX];
    char *exec_all_ptr = NULL;
    int  err;
    extern int tc_errno;
    extern char * ptrepcode();

    DBUG(DBGFDESC) (stderr,"do_exec_tc(%s,%s,remote:%d,*tcend_status)\n", pathname, (arg_p == NULL ? "NULL" : arg_p), remote);

    /* These two are static because they would be used after a longjmp */
    timed_out = 0;
    return_val = SUCCESS;

    /* Make sure all the necessary stuff is in the environment */
    rc = export(environment, exec_ctr, remote, sysid);
    ERROR_ON(rc,"Failure in export().\n");

    /* Use absolute pathname for exec */
    full_path[0] = '\0';
    (void) strcpy(full_path, exe_dir);
    (void) strcat(full_path, strrchr(pathname, DIR_SEP_CHAR));

    argv[argc++] = full_path;

    if(arg_p != NULL)
        argv[argc++] = arg_p;

    /* special case for resume mode where you "all" parameter adding */
    if(exec_all_flag)
    {
        exec_all_flag = 0;
        argv[argc++] = store_str("all");
	exec_all_ptr = argv[argc-1];
    }

    /* Make sure that argv is NULL terminated, and that argc is correct */
    argv[argc++] = NULL;


    for(count = 0; argv[count] != NULL; count++)
    {
        DBUG(DBGFINFO) (stderr,"argv[%d] = %s\n", count, argv[count]);
    }

    if(remote == FALSE || sysid == MASTER)
    {
        /* Have a maximum of five goes at getting the fork() to work */
        while((cpid = fork()) == -1)
        {
            count++;
            if(count >= 5)
                break;
            (void) sleep(5);
        }
        if (cpid == 0)
        {
	    if(oc_set == TRUE)
		create_oc_file();

	    (void) execvp(full_path, argv);

            exit(EXIT_BAD_CHILD);
        }
    }
    else
    {
	if(oc_set == TRUE)
	{
	    cpid = tc_texec(sysid, full_path, argv, OC_FILENAME, snid, (long) -1);
	}
	else
	{
	    cpid = tc_texec(sysid, full_path, argv, NULL, snid, (long) -1);
	}
    }

    if(remote == FALSE || sysid == MASTER)
    {
        /* If fork() failed and return -1 we now bail out */
        BAIL_OUT_ON(cpid, "fork");
    }
    else
    {
	if(cpid == -1)
	{
	    (void) sprintf(error_mesg,"failed tc_texec() of \"%s\" on system %d, tc_errno = %d (%s)\n", full_path, sysid, tc_errno, ptrepcode(tc_errno));
	    FATAL_ERROR(error_mesg);
	}
    }

    if(exec_all_ptr != NULL)
	TET_FREE((void *) exec_all_ptr);

    /* The default string for the TC End message in the journal */
    (void) sprintf(line,"TC End");

    /*
     *  If the test times out this is where control will return to,
     *  and in that event timed_out will be set to non-zero.
     */
    timed_out = sigsetjmp(jmpbuf, 1);

    if (timed_out)
    {
        DBUG(DBGFINFO) (stderr,"Back in do_exec_tc() after time out.\n");

        (void) alarm(0);
        *tcend_status = TET_ESTAT_TIMEOUT;
        return(FAILURE);
    }

    /* Wait for the child to complete */
    if(remote == FALSE || sysid == MASTER)
    {
        while(waitpid((pid_t)cpid,&statloc,0) == FAILURE)
        {
	    err = errno;
            /* If the child recieved a signal */
            if(err == EINTR)
                continue;
	    (void) sprintf(error_mesg,"waitpid() failed, errno = %d\n", err);
            ERROR(error_mesg);
            return(FAILURE);
        }
    }
    else
    {
	/* XXX what about interrupts XXX */
	if(tc_wait(sysid, (long) cpid, -1, &statloc) == FAILURE)
	{
	    (void) sprintf(error_mesg,"tc_wait() on system %d failed, tc_errno = %d (%s)\n", sysid, tc_errno, ptrepcode(tc_errno));
	    ERROR(error_mesg);
	    return(FAILURE);
	}

    }

    if(WIFSIGNALED(statloc))
    {
        waitstatus = WTERMSIG(statloc);

        /* if user interrupt */
        if((waitstatus == SIGTERM && interrupted == 2) ||
                            (waitstatus == SIGINT && interrupted == 1))
        {
            user_interrupt = TRUE;
        }

        *tcend_status = waitstatus + 1000;
    }
    else
    {
        /* Sort out the status for the TC End entry */
        *tcend_status = WEXITSTATUS(statloc);
        if( *tcend_status == EXIT_BAD_CHILD)
        {
             /* exec() of TC failure = non fatal */
             ERROR("Failed exec of Test Case\n");
            *tcend_status = TET_ESTAT_EXEC_FAILED;
            return(FAILURE);
        }
    }

    /* make sure user interrupt flag is cleared */
    interrupted = 0;

    /* If they haven't already been reset... */
    if (cpid != 0)
    {
        /* No more child */
        cpid = 0;

        /* Reset the timeout alarm call */
        (void) alarm(0);
    }

    /*
     *  If no ouput capture then get the results from the 
     *  results file instead.
     */
    if(oc_set == FALSE)
    {
        res_filename = NULL;
        if(remote == TRUE)
        {
	    if(sysid == MASTER)
	    {
	        /* This is the MASTER tcc of a group of remotes so
	         * we want the combined output file
	         */
	        res_filename = store_str(COM_XRES_FILENAME);
	    }
	    else
	    {
	        for(count = 0; count < gnumsys; count++)
		    if(gsyslist[count] == MASTER)
		        break;

	        /* If no MASTER system, we want xres file from 
	         * the remote system.
	         * Otherwise, we only want the MASTER's 'tet_combined'
	         * file which will contain all the results.
	         */
	        if(count == gnumsys)
	        {
		    res_filename = store_str(XRES_FILENAME);
	        }
	    }
        }
        else
        {
	    /* Not within remote so want xres file from tcc */
	    res_filename = store_str(XRES_FILENAME);
        }

        if(res_filename != NULL)
        {
            if((file = get_output_file(res_filename, remote, sysid)) == NULL)
	    {
                (void) sprintf(error_mesg,"get_output_file(\"%s\") returned NULL\n", res_filename);
	        ERROR(error_mesg);
		return_val = FAILURE;
	    }
	    else
	    {
                res_fp = fopen(file,"r");
                if (res_fp == (FILE *) NULL) 
                {
                    (void) sprintf(error_mesg,"Cannot open results file \"%s\", errno = %d\n", file, errno);
	            ERROR(error_mesg);
		    return_val = FAILURE;
                }
                else
                {
	    	    DBUG(DBGFILES)(stderr,"fopen(%ld)\n", (long) res_fp);

                    /* copy the results into the journal file */
                    (void) copy_results_file(res_fp, EXEC);

		    DBUG(DBGFILES)(stderr,"fclose(%ld)\n", (long) res_fp);
                    (void) fclose(res_fp);
                }

	        clear_output_file(file, remote, sysid);
	    }

    	    TET_FREE((void *)res_filename);
        }
    }
    else /* OUTPUT_CAPTURE == TRUE */
    {
        /* Write contents of output capture file to the journal */
        exec_oc_jnl(remote, sysid);

        /* make dummy TP end entry in the journal */
        if((tmp_ptr = rescode_num_to_name(*tcend_status)) == (char *) NULL)
        {
            if( *tcend_status < 1000)
            {
                (void) sprintf(error_mesg,"Couldn't match status %d of Test Case to a result string.\n", *tcend_status);
            }
            else
            {
                (void) sprintf(error_mesg,"Test Case terminated by signal %d.\n", (*tcend_status)%1000);
            }
            ERROR(error_mesg);

            (void) jnl_tp_result(exec_ctr,0,7, "NORESULT");
        }
        else
        {
            (void) jnl_tp_result(exec_ctr,0,*tcend_status,tmp_ptr);

            /* Check for an Abort */
            if(*rescode_num_to_action(*tcend_status) == 'A')
            {
	        /* Set Abort Flag */
	        (void) set_abort_req(SETVALUE);
            }
        }

        /* make dummy IC end entry in the journal */
        (void) get_time();
        (void) sprintf(lineout,"%d|%d 0 1 %s|IC End\n",TET_JNL_IC_END, exec_ctr, zeetime);
        (void) jnl_entry(lineout);
    }

    if(abort_requested())
    {
        (void) jnl_tc_message("Result code action is Abort\n");
    }

    /* set text string for TC end entry in the journal */
    (void) strcpy(line,"TC End");
    if ( WIFEXITED(statloc) == 0)  /* didn't exit -> signalled */
    {
        if(*tcend_status >= 1000)
        {
            (void) sprintf(line, "%s, terminated by signal %d", line, (*tcend_status)%1000);
            if(user_interrupt == TRUE)
                (void) strcat(line, " (User Interrupt)");
        }

    }

    return(return_val);
}
