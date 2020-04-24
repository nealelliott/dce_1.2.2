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
static char sccsid[] = "@(#)bld_cln.c	1.10 (93/01/25) DTET release 1.0b";
#endif

/************************************************************************

SCCS:           @(#)bld_cln.c    1.10 1/25/93
NAME:           bld_cln.c
PRODUCT:        DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:         This source file is based on Release 1.9 of the TET
DATE CREATED:   February 1992
CONTENTS: 	Functions relating to the build and clean

MODIFICATIONS:
		DTET Development
		David G. Sawyer
		John-Paul Leyland
		UniSoft Ltd.

	Andrew Dingwall, UniSoft Ltd., October 1992
	allow a DTET API-conforming build/clean tool to send results
	to the tet_combined file administered by XRESD

************************************************************************/

#include <tcc_env.h>
#include <tcc_mac.h>
#include <dtet/tet_jrnl.h>
#include <tcc_prot.h>


char line[257];            /* line buffer used in printing var jnl entries */
static char linein[JNL_LINE_MAX]; /* line buffer used to get data from pipe */
int  tmode;                /* tool mode: BUILD, or CLEAN */
extern char zeetime[];     /* Time array */
static ENV_T *environment; /* Environment for the particular clean or build */


/*
 *  Execute the specified tool.
 */
int bld_cln_tc(testcase_p,what_mode,remote_flag, sysid, group_flag, group_pipe_fd)
char * testcase_p;
int    what_mode;
int    remote_flag;
int    sysid;
int    group_flag;
int *  group_pipe_fd;
{
    char *ic_start_mark_p, *output_capture_p, *tc_p = NULL;
    char tc_dir[_POSIX_PATH_MAX];
    char source_path[_POSIX_PATH_MAX];
    int rc, b_rc;
    int tcend_status;
    struct sigaction on_alarm;
    int ret_val;
    char *ts_root;

    DBUG(DBGFDESC) (stderr,"bld_cln_tc(%s,mode:%d,remote:%d,sysid:%d,group:%d,group_pipe_fd)\n", testcase_p, what_mode, remote_flag, sysid, group_flag);

    /* get the relevant environment for the exec */
    environment = get_env_ptr(what_mode, remote_flag, sysid);

    /* tmode is used in the event of a timeout */
    tmode = what_mode;

    /* Put a start message into the journal file - for the tool */
    if(tmode == BUILD)
        jnl_tool_start(exec_ctr,testcase_p, "Build Start", tmode, sysid);
    else
        jnl_tool_start(exec_ctr,testcase_p, "Clean Start", tmode, sysid);

    tc_p = (char *) TET_MALLOC((size_t)(strlen(testcase_p)+1));
    (void) strcpy(tc_p, testcase_p);

    if ((ic_start_mark_p = strchr(tc_p,IC_START_MARK)) != NULL)
	tc_p[(ic_start_mark_p - tc_p)] = '\0';

    if(mode_message(QUERY))
    {
        (void) get_time();
        (void) fprintf(stdout,"%s  %s  %s", zeetime, ((what_mode == BUILD) ? "Build  " : "Clean  "), tc_p);
	if(remote_flag == FALSE)
	    (void) fprintf(stdout,"\n");
	else
	    (void) fprintf(stdout," on system %3.3d\n", sysid);
	(void) fflush(stdout);
    }

    /* Is output capture set for this mode */
    output_capture_p = get_tetenv("TET_OUTPUT_CAPTURE",environment);

    if ( (output_capture_p != NULL) && ((*output_capture_p == 'T') || 
                         (*output_capture_p == 't')) )
        oc_set = TRUE;
    else
        oc_set = FALSE;

    /*
     *  build up the dir we want to be in, and then we change directory to it.
     */

    (void) strcpy(tc_dir, tc_p);
    /* make tc_dir the right length */
    *(strrchr(tc_dir, DIR_SEP_CHAR)) = '\0';

    tet_srcpath(source_path, tc_dir, remote_flag, sysid);

    if(tet_chdir(source_path,remote_flag,sysid) == FAILURE)
    {
        (void) sprintf(error_mesg,"Can't change directory to %s\n", source_path);
        ERROR(error_mesg);
	if(tc_p != NULL)
	    TET_FREE((void *) tc_p);
        return(FAILURE);
    }

    /* Is this is a child process of a parallel execution ? */
    /* For build/clean this syncronisation is not needed... it doesn't
     * do any harm though.
     */
    if(group_flag == TRUE)
    {
	/* Send signal to parent to indicate that it is ready to continue */
	signal_parent(group_pipe_fd);

	/* Wait for signal from parent */
	child_wait();

	/* Child will get to this point when it receives a SIGUSR1 */
    }

    /* Obtain a lock in order to execute the tool */
    ret_val = obtain_lock(EXCLUSIVE, tc_dir, remote_flag, sysid, FALSE);
    if(ret_val == SUCCESS)
    {
	if(get_alt_exec_dir(remote_flag, sysid) != NULL)
	    ret_val = obtain_alt_lock(tc_dir, remote_flag, sysid);
    }
    else
    {
	release_locks();
        jnl_tool_end(exec_ctr, TET_ESTAT_LOCK, ((tmode == BUILD) ? "Build End" : "Clean End"), tmode);
	if(tc_p != NULL)
	    TET_FREE((void *) tc_p);
        return(FAILURE);
    }

    /* Set the alrm call for the timeout */
    on_alarm.sa_handler = tool_sig_hdlr;
    on_alarm.sa_flags = 0;
    (void) sigaction(SIGALRM, &on_alarm, (struct sigaction *)NULL);
    (void) alarm((unsigned int)gtimeout());

    /*
     *  If this is an DTC then call do_bld_cln_tc() to try and a run 
     *  prebuild tool before we do the normal build.
     */
    if((tmode == BUILD) && remote_flag)
    {
        tmode = PREBUILD;
	b_rc = do_bld_cln_tc(tc_p, remote_flag, sysid, &tcend_status);
        tmode = BUILD;
    }

    /*
     * Pass the full test-case name & let do_bld_cln_tc()
     * itself strip off directory prefix where necessary.
     */
    b_rc = do_bld_cln_tc(tc_p, remote_flag, sysid, &tcend_status);

    /* If they haven't already been cleared; clear the cpid and timer */
    if(cpid != 0)
    {
        cpid = 0;
        (void) alarm(0);
    }
    on_alarm.sa_handler = SIG_DFL;
    on_alarm.sa_flags = 0; 
    (void) sigaction(SIGALRM, &on_alarm, (struct sigaction *)NULL);

    /* remove the lock */
    release_locks();

    /* Put a end message into the journal file - for the tool */
    if(tmode == BUILD)
        (void) strcpy(linein, "Build End");
    else
        (void) strcpy(linein, "Clean End");
    if(line[0] != '\0')
        (void) strcat(linein, line);

    jnl_tool_end(exec_ctr, tcend_status, linein, tmode);

    if((ts_root = get_ts_root(remote_flag, sysid)) != (char *) NULL)
    {
        /* Reposition ourselves in the test suite root directory */
        if(tet_chdir(ts_root,remote_flag,sysid) == FAILURE)
        {
	    (void) sprintf(error_mesg,"chdir to test suite root directory : %s", ts_root);
            BAIL_OUT2(error_mesg);
        }
    }

    if(tc_p != NULL)
	TET_FREE((void *) tc_p);
    return(b_rc); 
}


/*
 * Execute the tool
 * Arguments:
 * pointer to full test-case name
 * file descriptor of journal file
 * array of environment structures
 * timeout period (in seconds)
 */
int do_bld_cln_tc(pathname, remote, sysid, tcend_status)
char * pathname;
int    remote;
int    sysid;
int  * tcend_status;
{
    int rc, count = 0, tp_end_status = SUCCESS, fd;
    int statloc, statloc2, argc = 0, waitstatus, bft_pid;
    int user_interrupt = FALSE, argvfree;
    char *argv[50], *tool_file_p, *bft_p;
    char *str1, *str2, *str3, *this_tool, *tc_name;
    static int timed_out, return_val;
    FILE *res_fp, *exec_fp;
    struct sigaction on_int, old_int;
    char *file;
    struct stat stbuf;
    int err;
    extern int tc_errno;
    extern char *ptrepcode();

    DBUG(DBGFDESC) (stderr,"do_bld_cln_tc(%s,remote:%d,sysid:%d,*tcend_status)\n", pathname, remote, sysid);

    /* These two are static because they would be used after a longjmp */
    timed_out = 0;
    return_val = SUCCESS;

    /* Get the tool name from the tet environment */
    switch(tmode)
    {
        case BUILD:
            this_tool = get_tetenv("TET_BUILD_TOOL",environment);
	    break;
        case CLEAN:
            this_tool = get_tetenv("TET_CLEAN_TOOL",environment);
	    break;
        case PREBUILD:
	    this_tool = get_tetenv("TET_PREBUILD_TOOL",environment);
	    break;
    }

    if ((this_tool == NULL) || (*this_tool == '\0'))
    {
        if (tmode == BUILD)
        {
            ERROR("TET_BUILD_TOOL has NULL value.\n");
	    return(FAILURE);
        }
        else if(tmode == CLEAN)
        {
            ERROR("TET_CLEAN_TOOL has NULL value.\n");
	    return(FAILURE);
        }
        else if(tmode == PREBUILD)
	{

	    DBUG(DBGFINFO) (stderr,"No PREBUILD has been done.\n");

	    /* OK for TET_PREBUILD_TOOL to not be set */
	    return(SUCCESS);
	}

        return(FAILURE);
    }
    else
    {
        /* set str2 to the same so that we can free the space afterwards */
        str1 = str2 = store_str(this_tool);

        /*
         * This bit gets any parameters supplied with the tool
         * and makes sure they are used during the exec() call.
         */

        str3 = strtok(str1," \t");

        argv[argc] = (char *) TET_MALLOC (strlen(str3) + 1);
	argvfree = argc;
        (void) strcpy(argv[argc++], str3);

        /* Dont want to change the env variable so... */
        this_tool = store_str(str3);
        (void) strcpy(this_tool, str3);

        str3 = strtok((char *) NULL," \t");
        while( str3 != NULL)
        {
	    argv[argc++] = store_str(str3);
            str3 = strtok((char *) NULL," \t");
        }

        TET_FREE((void *) str2);
    }

    /* get the test case name leaf name from the full relative path */
    tc_name = basename(pathname);

    if (oc_set == TRUE)
    {
        /*
         *  Establish whether there is a tool file and if so make sure that
         *  it is included in the list of parameters that is passed to the
         *  tool at execution time.
         */

	switch(tmode)
	{
	    case BUILD:
                tool_file_p = get_tetenv("TET_BUILD_FILE",environment);
                break;
	    case CLEAN:
                tool_file_p = get_tetenv("TET_CLEAN_FILE",environment);
		break;
	    case PREBUILD:
		tool_file_p = get_tetenv("TET_PREBUILD_FILE",environment);
		break;
	}

        if (tool_file_p != NULL)
        {
            /* set str2 to the same so that we can free the space afterwards */
            str1 = str2 = store_str(tool_file_p);

            str3 = strtok(str1," \t");

            argv[argc] = (char *) TET_MALLOC (strlen(str3) + 1);
            (void) strcpy(argv[argc++], str3);

            str3 = strtok((char *) NULL," \t");
            while( str3 != NULL)
            {
		
                argv[argc++] = store_str(str3);
                str3 = strtok((char *) NULL," \t");
            }

            TET_FREE((void *) str2);
        }

        /* The leaf name of the TC comes last in the list of argv */
	argv[argc] = (char *) TET_MALLOC (strlen(tc_name) + 1);
        (void) strcpy(argv[argc++], tc_name);
    }

    /* Make sure that argv is NULL terminated, and that argc is correct */
    argv[argc++] = NULL;


    for(count = 0; argv[count] != NULL; count++)
    {
        DBUG(DBGFINFO) (stderr,"argv[%d] = %s\n", count, argv[count]);
    }

    /* make sure that no old tet_xres file exists */
    (void) tet_unlink(XRES_FILENAME, remote, sysid); 

    /* make sure that no old tet_captured file exists */
    (void) tet_unlink(OC_FILENAME, remote, sysid); 

    if (remote == FALSE || sysid == MASTER)
    {
	/*
	 * create an empty tet_combined file here in case a
	 * non-API-conforming build/clean tool doesn't do so
	 */
	if ((fd = creat(COM_XRES_FILENAME, (S_IRWXU|S_IRWXG|S_IRWXO))) == FAILURE)
	{
	    (void) sprintf(error_mesg, "Can't creat \"%s\", errno = %d\n",
		COM_XRES_FILENAME, errno);
            ERROR(error_mesg);
	    return(FAILURE);
	}
	else
	{
	    DBUG(DBGFILES)(stderr,"open(%d) via creat()\n", (int) fd);
	}

	DBUG(DBGFILES)(stderr,"close(%d)\n", (int) fd);
	(void) close(fd);
    }

    /* Export the tet environment */
    rc = export(environment,exec_ctr,remote,sysid);

    ERROR_ON(rc,"Failure in export().\n");

    if(remote == FALSE || sysid == MASTER)
    {
        /* Have up to five goes at getting a successful exec before giving up */
        count = 0;
        while((cpid = fork()) == FAILURE)
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

	    (void) execvp(this_tool, argv);

	    /* The exec failed */
            exit(EXIT_BAD_CHILD);
        }
    }
    else
    {
	if(oc_set == TRUE)
	{
	    cpid = tc_texec(sysid, this_tool, argv, OC_FILENAME, snid, (long) -1);
	}
	else
	{
	    cpid = tc_texec(sysid, this_tool, argv, NULL, snid, (long) -1);
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
	    (void) sprintf(error_mesg,"failed tc_texec() of \"%s\" on system %d, tc_errno = %d (%s)\n", this_tool, sysid, tc_errno, ptrepcode(tc_errno));
	    FATAL_ERROR(error_mesg);
	}
    }

    /* free argv arguments */
    for ( ; argv[argvfree] != NULL; argvfree++)
        TET_FREE((void *) argv[argvfree]);

    TET_FREE((void *) this_tool);

    /* In the event of a timeout control will return to this point and
     * timed_out will be set.
     */
    timed_out = sigsetjmp(jmpbuf,1);
    if (timed_out)
    {

        DBUG(DBGFINFO) (stderr,"Back in do_bld_cln_tc after time out.\n");

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

    /* If the child recieved a signal */
    if(WIFSIGNALED(statloc))
    {
        waitstatus = WTERMSIG(statloc);

        /* if user interrupt */
        if((waitstatus == SIGTERM && interrupted == 2) ||
                            (waitstatus == SIGINT && interrupted == 1))
            user_interrupt = TRUE;

        /* Sort out the status for the jnl_tool_end entry */
        *tcend_status = waitstatus + 1000;
    }
    else
    {
        /* Sort out the status for the jnl_tool_end entry */
        *tcend_status = WEXITSTATUS(statloc);
        if( *tcend_status == EXIT_BAD_CHILD) /* exec() call failed */
        {
            /* Note this jnl_tool_end is here coz of the BAIL_OUT */
            switch(tmode)
            {
                case PREBUILD:
                    jnl_tool_end(exec_ctr,TET_ESTAT_EXEC_FAILED,"Build End, Exec() of prebuild tool", BUILD);
                    BAIL_OUT2("Prebuild tool failure.\n");
                    break;
                case BUILD:
                    jnl_tool_end(exec_ctr,TET_ESTAT_EXEC_FAILED,"Build End, Exec() of build tool", BUILD);
                    BAIL_OUT2("Build tool failure.\n");
                    break;
                case CLEAN:
                    jnl_tool_end(exec_ctr,TET_ESTAT_EXEC_FAILED,"Clean End, Exec() of clean tool", CLEAN);
                    BAIL_OUT2("Clean tool failure.\n");
                    break;
            }
        }
    }

    /* make sure user interrupt flag is reset */
    interrupted = 0;

    /* No more Build/Clean Tool and no more timeout */
    if( cpid != 0)
    {
        cpid = 0;
        (void) alarm(0);
    }

    /*
     * If output capture wasn't enabled then get the results 
     * from the results file.
     */
    if ( oc_set == FALSE)
    {
	/*
	 *  If the file "tet_combined" on the master system is not zero length
	 *  then ALL the results are contained in that file.
	 *
	 *  Otherwise gather the results from the "tet_xres" files on each of
	 *  the system.
	 */

	/* see if we have a non-empty tet_combined file on the master system */
	stbuf.st_size = 0;
	if (remote == FALSE || sysid == MASTER)
	{
	    file = COM_XRES_FILENAME;
	    if (stat(file, &stbuf) < 0)
	    {
		(void) sprintf(error_mesg, "Can't stat \"%s\", errno = %d\n",
		    file, errno);
		ERROR(error_mesg);
		return(FAILURE);
	    }
	}

	/* if not, then use the tet_xres file */
	if (stbuf.st_size == 0)
	{
            if((file = get_output_file(XRES_FILENAME, remote, sysid)) == NULL)
            {
                (void) sprintf(error_mesg,"get_output_file(\"%s\") returned NULL\n", XRES_FILENAME);
                ERROR(error_mesg);
		return(FAILURE);
	    }
	}

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

            /*
	     * if we are dealing with a remote system and the results file
	     * is empty, then assume that the build/clean tool used the DTET
	     * API and that the results will be in tet_combined on the master
	     * system.
	     *
	     * A non-zero return here indicates the build tool failed
	     */
	    if (remote == TRUE && sysid != MASTER && stbuf.st_size == 0)
		tp_end_status = 0;
	    else
		tp_end_status = copy_results_file(res_fp, tmode);

	    DBUG(DBGFILES)(stderr,"fclose(%ld)\n", (long) res_fp);
            (void) fclose(res_fp);
        }

        clear_output_file(file, remote, sysid);
    }
    else /* OUTPUT_CAPTURE == TRUE */
    {
        /* Write output capture file to journal */
        tool_oc_jnl(remote, sysid);
    }

    if ( (tp_end_status != SUCCESS) || (WIFEXITED(statloc) == 0) || 
                                                 (WEXITSTATUS(statloc) != 0))
    {
        if(tmode == PREBUILD)
        {
            DBUG(DBGFINFO) (stderr, "Prebuild tool '%s' executed but failed.\n",this_tool);
        }

        bft_p = get_tetenv("TET_BUILD_FAIL_TOOL",environment);
        if( (tmode == BUILD) && (bft_p != NULL) )
        {
            /*
             * The build tool executed but failed - so see if we can 
             * execute a build fail tool.
             */

            /* No interruptions whilst invoking the Build Fail Tool */
            on_int.sa_handler = SIG_IGN;
            on_int.sa_flags = 0;
            if (sigaction(SIGINT, &on_int, &old_int) == -1)
            {
                old_int.sa_handler = siginthdlr;
                old_int.sa_flags = 0;
            }

		/*
		 *  XXX Needs to be made generic ! XXX
		 */

		
                /* Give fork several chances to work */
                count = 0;
                while ((bft_pid = fork()) == -1 && count <= 5)
                {
                    (void) sleep(5);
                    count++;
                }
                if (bft_pid == 0)
                {  /* child */

                    DBUG(DBGFINFO) (stderr,"About to exec %s\n", bft_p);

                    if (execlp(bft_p,bft_p, tc_name, NULL) == FAILURE)
                    {
                        (void) perror("execlp");
                        exit(EXIT_BAD_CHILD);
                    }
                }

            BAIL_OUT_ON(bft_pid,"Failure of fork() for build fail tool.\n");

		/*
		 *  XXX Needs to be made generic ! XXX
		 */

                /* Wait for the child to complete */
                rc = waitpid((pid_t)bft_pid,&statloc2,0);

            if(rc == FAILURE || WIFEXITED(statloc2) == 0)
	    {
                return_val = FAILURE;
	    }
            else
	    {
		if(WEXITSTATUS(statloc2) != 0)
		{
               	    /* Check to see if exec() call failed */
		    if(WEXITSTATUS(statloc2) == EXIT_BAD_CHILD)
		    {
                        jnl_tool_end(exec_ctr,TET_ESTAT_EXEC_FAILED,"Build End, Exec() call of build fail tool failed.", tmode);
                        BAIL_OUT2("Build Fail Tool failure.\n");
		    }
		}
		else
		{
                    return_val = SUCCESS;
		}
	    }

            /* Reset SIGINT handling */
            (void) sigaction(SIGINT,&old_int,(struct sigaction *)NULL);
        }
        else /* bft == NULL or mode != BUILD */
        {
            return_val = FAILURE;
        }
    }

    line[0] = '\0';
    if(*tcend_status >= 1000) /* ie signalled */
    {
        (void) sprintf(line,", Tool terminated by signal %d", waitstatus);
        if( user_interrupt == TRUE)
            (void) strcat(line," (User Interrupt)");
    }

    return(return_val);
}
